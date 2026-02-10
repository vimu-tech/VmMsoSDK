#include "WaveTrack.h"
#include <assert.h>
#include <thread>
#include <QDebug>

WaveRecord::WaveRecord()
	: dirty(true)
	, ppoints(nullptr)
{
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
};

WaveRecord::~WaveRecord()
{
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
	if(ppoints!= nullptr)
		delete []ppoints;
};

bool WaveRecord::GetRecord(plotpoints* points)
{
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
	if(dirty)
		return false;
	if(*ppoints!=*points)
		return false;

	*points=*ppoints;
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
	//TRACE(_T("PlotRecord::GetRecord\n"));
	return true;
}

void WaveRecord::SetRecord(plotpoints* points)
{
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
	dirty=false;
	if(ppoints== nullptr)
		ppoints=new plotpoints(points->pixel_num);
	else
	{
		if(ppoints->getpixelnum()!=points->getpixelnum())
		{
			delete ppoints;
    		ppoints=new plotpoints(points->pixel_num);
		}
	}
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
	*ppoints=*points;
}

bool WaveRecord::GetPoints(plotpoints* points, int from, int start, int len)
{
	assert(start>=0);
	assert(start+len<=ppoints->pixel_num);
	if((ppoints->minvalue== nullptr)||(ppoints->maxvalue== nullptr))
		return false;

	memcpy(points->minvalue+start,ppoints->minvalue+from,len*sizeof(ppoints->minvalue[0]));
	memcpy(points->maxvalue+start,ppoints->maxvalue+from,len*sizeof(ppoints->maxvalue[0]));
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------
WaveTrack::WaveTrack(const QString& nm, int maxcount, uint64_t len, WaveTrackInfo& waveinfo,
                     double timebegin, double timeblocklen)
: name(nm)
, maxblockcount(maxcount)
, curblockcount(0)
, blocklen(len)
, trackinfo(waveinfo)
, time_begin(timebegin)
, time_blocklen(timeblocklen)
, adddata_count(0)
{
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
}

WaveTrack::~WaveTrack(void)
{
	//DebugString("%s %d %08x %d\n", __FUNCTION__, __LINE__, this, std::this_thread::get_id());
	DestoryAll();
}

void WaveTrack::Reset()
{
	DestoryAll();
}

void WaveTrack::DestoryAll()
{
	for (std::list<BlockFile*>::iterator list_Iter = filelist.begin(); list_Iter != filelist.end(); ++list_Iter) 
	{
		delete (*list_Iter);
	}
	// 如果 list 声明对象为类，则 clear 不能释放该内存，要辅以 erase 或 pop 或 delete
	filelist.erase(filelist.begin(), filelist.end());
	filelist.clear();
	curblockcount=0;
	adddata_count =0;
}

void WaveTrack::MoveTime(double move)
{
	time_begin = time_begin+move;
	plotrecord.Clear();  //临时解决方案，以后考虑更先进的算法
	fullplotrecord.Clear();
}

/*		
20190617 WaveTrack AddData和ChangeData 分开处理 便于算法设计  
1、使用动态长度 maxblockcount==-1 代表无限长度 
*/
bool WaveTrack::AddData(void* buf, uint64_t len)
{
	bool result = AddData(buf, adddata_count, len);
	adddata_count += len;
	return result;
}

//1、使用动态长度 maxblockcount==-1 代表无限长度 
bool WaveTrack::AddData(void* buf, int64_t offset, uint64_t len)
{
	if((maxblockcount==-1)||(curblockcount<maxblockcount))
	{
		//
		int64_t BlockOffset = offset % blocklen;
		//上个块未填充长度
		int64_t LastBlockBlankCount = (BlockOffset==0? 0:blocklen-BlockOffset);
		if (LastBlockBlankCount > len)
			LastBlockBlankCount = len;
		//剩余长度
		int64_t RemainLength = (len - LastBlockBlankCount)<0? 0: len - LastBlockBlankCount;
		//整块的个数
		int64_t BlockNum = RemainLength / blocklen;
		//最后剩余块数据长度
		int64_t NotFullBlockLength = RemainLength % blocklen;
		//
		int64_t BlockIndex = offset / blocklen;
		//DebugString("AddData BlockOffset=%lld BlockNum=%lld NotFullBlockLength=%lld\n", BlockOffset, BlockNum, NotFullBlockLength);

		//处理上个块没填充的剩余部分
		if (BlockOffset != 0)
		{
			std::list<BlockFile*>::iterator list_Iter = filelist.end();
			--list_Iter; 
			(*list_Iter)->ChangeData(BlockOffset, buf, LastBlockBlankCount);
			//DebugString("AddData 1 start=%lld offset=%d len=%lld\n", BlockOffset, 0, LastBlockBlankCount);
		}

		//整块处理
		for (int64_t i = 0; i < BlockNum; i++)
		{
            BlockFileInfo fileinfo(trackinfo, BlockIndex*blocklen, blocklen, tempmng.GetTempFilename(trackinfo.GetPathName()));
			BlockFile* file = new BlockFile(fileinfo);
			filelist.push_back(file);

			//DebugString("AddData 2 start=%lld offset=%d len=%lld\n", LastBlockBlankCount + i * blocklen, 0, blocklen);
			file->ChangeData(0, (char*)buf + (LastBlockBlankCount + i * blocklen) * SAMPLE_SIZE(fileinfo.format), blocklen);

			curblockcount++;
			BlockIndex++;
		}

		//最后不完整块
		if (NotFullBlockLength != 0)
		{
            BlockFileInfo fileinfo(trackinfo, BlockIndex*blocklen, blocklen, tempmng.GetTempFilename(trackinfo.GetPathName()));

			BlockFile* file = new BlockFile(fileinfo);
			filelist.push_back(file);

			//DebugString("AddData 3 start=%lld offset=%d len=%lld\n", LastBlockBlankCount + BlockNum * blocklen, 0, NotFullBlockLength);
			file->ChangeData(0, (char*)buf + (LastBlockBlankCount + BlockNum * blocklen) * SAMPLE_SIZE(fileinfo.format), NotFullBlockLength);
			curblockcount++;
		}

		plotrecord.Dirty();
		fullplotrecord.Dirty();
	}
	return true;
}

bool WaveTrack::GetData(double* buf, int64_t start, uint64_t* len)
{
	assert(curblockcount<=1);
	std::list<BlockFile*>::iterator list_Iter = filelist.begin();
	return (*list_Iter)->Read(start, buf, len);
}

bool WaveTrack::ChangeData(void* buf, int64_t offset, uint64_t len, double timebegin)
{
	//double time_offset = -(trigle*time_blocklen/blocklen);
	//time_begin=time_init_begin+time_offset;
	//time_begin = -(trigle*time_blocklen/blocklen);
	time_begin = timebegin;
	return ChangeData(buf, offset, len);
}

/*
临时解决方案，以后需要升级   用于非记录仪时，ChangeData调用了，但是GetPlotPoints中处理重新利用区域不能识别问题
*/
bool WaveTrack::ChangeData(void* buf, int64_t offset, uint64_t len)
{
	assert(len<=blocklen);
	//
	int64_t BlockOffset = offset % blocklen;
	//上个块未填充长度
	int64_t LastBlockBlankCount = (BlockOffset == 0 ? 0 : blocklen - BlockOffset);
	//剩余长度
	int64_t RemainLength = len - LastBlockBlankCount;
	//整块的个数
	int64_t BlockNum = RemainLength / blocklen;
	//最后剩余块数据长度
	int64_t NotFullBlockLength = RemainLength % blocklen;
	//
	int64_t BlockIndex = offset / blocklen;
	//DebugString("ChangeData 0 blocklen=%lld offset=%lld len=%lld   BlockIndex=%lld curblockcount=%d\n", blocklen, offset, len, BlockIndex, curblockcount);


	//处理上个块没填充的剩余部分
	if (BlockOffset != 0)
	{
		for (std::list<BlockFile*>::iterator list_Iter = filelist.begin(); list_Iter != filelist.end(); ++list_Iter)
		{
			BlockFileInfo& pfileinfo = (*list_Iter)->GetBlockFileInfo();
			if ((offset >= pfileinfo.offset) && (offset < pfileinfo.offset + pfileinfo.curlength))
			{
				(*list_Iter)->ChangeData(BlockOffset, buf, LastBlockBlankCount);
				//DebugString("ChangeData 1 start=%lld offset=%d len=%lld\n", BlockOffset, 0, LastBlockBlankCount);
			}
		}
	}

	//整块处理
	for (int64_t i = 0; i < BlockNum; i++)
	{
		for (std::list<BlockFile*>::iterator list_Iter = filelist.begin(); list_Iter != filelist.end(); ++list_Iter)
		{
			BlockFileInfo& pfileinfo = (*list_Iter)->GetBlockFileInfo();
			int64_t bufindex = LastBlockBlankCount + i * blocklen;
			if ((bufindex >= pfileinfo.offset) && (bufindex < pfileinfo.offset + pfileinfo.curlength))
			{
                BlockFileInfo fileinfo(trackinfo, BlockIndex*blocklen, blocklen, tempmng.GetTempFilename(trackinfo.GetPathName()));
				(*list_Iter)->ChangeBlockFileInfo(fileinfo);

				(*list_Iter)->ChangeData(0, (char*)buf + bufindex * SAMPLE_SIZE(fileinfo.format), blocklen);
				//DebugString("ChangeData 2 start=%lld offset=%d len=%lld\n", bufindex, 0, blocklen);
				BlockIndex++;
			}
		}
	}

	//最后不完整块
	if (NotFullBlockLength != 0)
	{
		for (std::list<BlockFile*>::iterator list_Iter = filelist.begin(); list_Iter != filelist.end(); ++list_Iter)
		{
			BlockFileInfo& pfileinfo = (*list_Iter)->GetBlockFileInfo();
			int64_t bufindex = LastBlockBlankCount + BlockIndex * blocklen;
			if ((bufindex >= pfileinfo.offset) && (bufindex < pfileinfo.offset + pfileinfo.curlength))
			{
                BlockFileInfo fileinfo(trackinfo, BlockIndex*blocklen, blocklen, tempmng.GetTempFilename(trackinfo.GetPathName()));
				(*list_Iter)->ChangeBlockFileInfo(fileinfo);

				(*list_Iter)->ChangeData(0, (char*)buf + bufindex * SAMPLE_SIZE(fileinfo.format), NotFullBlockLength);
				//DebugString("ChangeData 3 start=%lld offset=%d len=%lld\n", bufindex, 0, NotFullBlockLength);
			}

		}
	}

	plotrecord.Dirty();
	fullplotrecord.Dirty();

	return true;
}

bool WaveTrack::GetPlotPoints(plotpoints* points)
{
	return GetPlotPoints(plotrecord, points);
}

bool WaveTrack::GetFullPlotPoints(plotpoints* points)
{
	return GetPlotPoints(fullplotrecord, points);
}

bool WaveTrack::GetPlotPoints(WaveRecord& plotrcd, plotpoints* points)
{
	if(curblockcount<=0)
		return false;

	//曲线不在绘图范围内
	if((points->timemax<=time_begin)||(points->timemin>=time_begin+time_blocklen*curblockcount))
		return false;

	//计算总共时间
	std::list<BlockFile*>::iterator list_Iter = filelist.end();
	--list_Iter;
	double time_max=time_begin+time_blocklen*(curblockcount-1)+(*list_Iter)->GetCurLen()*time_blocklen/blocklen;
    //qDebug()<<"time_begin "<< time_begin << " time_max " << time_max;

	//确定绘图范围
	points->timerealmin=m_max(points->timemin,time_begin);
	points->timerealmax=m_min(points->timemax,time_max);
    //qDebug()<<"timerealmin "<< points->timerealmin << " timerealmax " << points->timerealmax;

	//没有数据
	if(points->timerealmax<=points->timerealmin)
		return false;
	assert(points->timerealmin<=points->timerealmax);

	//绘制区域可以完全的重新的利用
	if(plotrcd.GetRecord(points))
		return true;

	//计算绘图点开始
	int plotbegin=(int)((points->timerealmin-points->timemin)*(points->pixel_num-1)/(points->timemax-points->timemin));

    //每个像素的数据点数
	points->samplesPerPixel=blocklen*(points->timemax-points->timemin)/time_blocklen/ (points->pixel_num - 1);//将上式与plotlen联合

	//计算使用的统计点数
	uint64_t divisor;
    if (points->samplesPerPixel >= blocklen)
		divisor = blocklen;
    else if (points->samplesPerPixel >= 65536)
		divisor = 65536;
    else if (points->samplesPerPixel >= 256)
        divisor = 256;
    else
        divisor = 1;

	//将绘图横坐标填充
	for(int i=0;i<points->pixel_num;i++)
		points->pixel[i]=-1;
	points->cur_pixel_num=0;

	//计算读取数据的开始和长度（不是统计后的数据）
	double readbegin = (points->timerealmin - time_begin) * blocklen / time_blocklen;
	double readlen = (points->timerealmax - points->timerealmin) * blocklen / time_blocklen;
	GenPlotPoints(divisor, plotbegin, readbegin, readbegin, readlen, points);

    plotrcd.SetRecord(points);
	return true;
}

int WaveTrack::GenPlotPoints(uint64_t divisor, int plotbegin, double totalreadbegin, double readbegin, uint64_t readlen, plotpoints* points)
{
	//readbegin不是整数，导致下面按照int读取数据，会导致绘图位置偏移，用readbegin_fractional补偿偏移
	//使用modf函数分离整数和小数部分
	double readbegin_integer = 0;
	double readbegin_fractional = modf(readbegin, &readbegin_integer);
	double totalreadbegin_integer = 0;
	double totalreadbegin_fractional = modf(totalreadbegin, &totalreadbegin_integer);
	 
		//计算第一个块是那个块(从0开始计数)
		uint64_t firstblockindex = readbegin_integer / blocklen;
		//读取的第一个块起始位置
		uint64_t firstblockbegin = (uint64_t)readbegin_integer % blocklen;

		//读取整个块的个数
		uint64_t blocknum=0;  
		if(readlen>blocklen-firstblockbegin)  //读取的长度超过了两个块
		{
			if(firstblockbegin!=0)   //第一个块不从开始读取
			{
				blocknum=(readlen-(blocklen-firstblockbegin))/blocklen+1;
				if(((readlen-(blocklen-firstblockbegin))%blocklen)!=0)
		        	blocknum++;
			}
			else   //第一个块从开始读取
			{
				blocknum=readlen/blocklen;
				if((readlen%blocklen)!=0)
		        	blocknum++;
			}
		}
		else  //总共读取在第一个块
	     	blocknum=1;

        assert(firstblockindex+blocknum<=curblockcount);

		//读取数据缓冲区
		uint64_t buflen=(blocklen+(divisor-1))/divisor*2;//min max
		double *buffer=new double[buflen];
 
		int p=0;
		int k=-1,last_k=points->pixel[points->cur_pixel_num];
		int index=0;
		uint64_t temp=0;
//TRACE(_T("读取的块 begin=%d "),points->cur_pixel_num);

        for (std::list<BlockFile*>::iterator list_Iter = filelist.begin();list_Iter!=filelist.end(); ++list_Iter,p++) 
		{
			if(!((firstblockindex<=p)&&(p<firstblockindex+blocknum)))
				continue;

            uint64_t begin,len;

			//计算每个块起始多余的位置
			if(p==firstblockindex)
			{
				temp=0;
				index=0;
			}
			else
			{
	      		if(firstblockbegin!=0)
				{
					temp=blocklen-firstblockbegin;
					index=1;
				}
				else
				{
					temp=0;
					index=0;
				}
			}
            //计算每个块的读取位置和长度
			if(p==firstblockindex)
			{
				begin=firstblockbegin/divisor;
				len=readlen>blocklen-firstblockbegin? ((blocklen-firstblockbegin+(divisor-1))/divisor):((readlen+(divisor-1))/divisor);
			}
			else if(p==firstblockindex+blocknum-1)
			{
				begin=0;
			    len=(readlen-blocklen*(p-firstblockindex-index)-temp+(divisor-1))/divisor;
			}
			else
			{
				begin=0;
				len=(blocklen+(divisor-1))/divisor;
			}
            //assert(len>0);
			if(len==0) continue;

			if(divisor==1)
			{
	     		(*list_Iter)->Read(begin, buffer, &len);
		     	for(size_t i=0;i<len&&points->cur_pixel_num<points->pixel_num;i++)
	          	{
					assert((temp+(p-firstblockindex-index)*blocklen+i*divisor)<=readlen);
					k=(int)((temp+(p-firstblockindex-index)*blocklen+i*divisor+readbegin_integer - totalreadbegin_integer - readbegin_fractional) / points->samplesPerPixel) + plotbegin;

					if (k < 0)
						continue;

					/*if(k>=points->pixel_num)
					{
						assert(k<=points->pixel_num+1);
						DebugString("WaveTrack k>=points->pixel_num i=%d len=%d %d\n",i, len, __LINE__);
						continue;
					}*/
			     	if(last_k!=k)
			    	{
						assert(k<points->pixel_num);
		    			points->pixel[points->cur_pixel_num]=k;
		    			points->minvalue[points->cur_pixel_num]=buffer[i];
	                 	points->maxvalue[points->cur_pixel_num]=buffer[i];
						points->cur_pixel_num++;
			    	}
					else
					{
     		    	    points->minvalue[points->cur_pixel_num-1]=points->minvalue[points->cur_pixel_num-1]<buffer[i]? points->minvalue[points->cur_pixel_num-1]:buffer[i];
	    	    	    points->maxvalue[points->cur_pixel_num-1]=points->maxvalue[points->cur_pixel_num-1]>buffer[i]? points->maxvalue[points->cur_pixel_num-1]:buffer[i];
					}
					last_k=k;
	        	}	
			}
			else if(divisor==256)
			{
	    		(*list_Iter)->Read256(buffer, begin, &len);

				/*for(int i=0;i<len;i++)
				{
					TRACE("min %f max %f \n",buffer[i*2],buffer[i*2+1]);
				}*/

		    	for(size_t i=0;i<len&&points->cur_pixel_num<points->pixel_num;i++)//points->cur_pixel_num<points->pixel_num 需要这样的条件是因为256，分块时，+255的缘故
	        	{
					assert((temp+(p-firstblockindex-index)*blocklen+i*divisor)<=readlen);
			    	k=(int)((temp+(p-firstblockindex-index)*blocklen+i*divisor+ readbegin_integer - totalreadbegin_integer - readbegin_fractional)/points->samplesPerPixel)+plotbegin;
						
					if (k < 0)
						continue;

					/*if(k>=points->pixel_num)
					{
						assert(k<=points->pixel_num+1);
						DebugString("WaveTrack k>=points->pixel_num i=%d len=%d %d\n",i, len, __LINE__);
						continue;
					}*/
		        	if(last_k!=k)
		        	{
						assert(k<points->pixel_num);
				    	points->pixel[points->cur_pixel_num]=k;
				    	points->minvalue[points->cur_pixel_num]=buffer[i*2];
	                 	points->maxvalue[points->cur_pixel_num]=buffer[i*2+1];
						points->cur_pixel_num++;
						//TRACE("min %f max %f \n",points->minvalue[points->cur_pixel_num-1],points->maxvalue[points->cur_pixel_num-1]);
			    	}
					else
					{
		          		points->minvalue[points->cur_pixel_num-1]=points->minvalue[points->cur_pixel_num-1]<buffer[i*2]? points->minvalue[points->cur_pixel_num-1]:buffer[i*2];
		        		points->maxvalue[points->cur_pixel_num-1]=points->maxvalue[points->cur_pixel_num-1]>buffer[i*2+1]? points->maxvalue[points->cur_pixel_num-1]:buffer[i*2+1];
					}
			    	last_k=k;
	        	}
			}
	    	else if(divisor==65536)
			{
		    	(*list_Iter)->Read64K(buffer, begin, &len);
	    		for(size_t i=0;i<len&&points->cur_pixel_num<points->pixel_num;i++)//points->cur_pixel_num<points->pixel_num
	        	{
					assert((temp+(p-firstblockindex-index)*blocklen+i*divisor)<=readlen);
			    	k=(int)((temp+(p-firstblockindex-index)*blocklen+i*divisor+ readbegin_integer - totalreadbegin_integer - readbegin_fractional)/points->samplesPerPixel)+plotbegin;

					if (k < 0)
						continue;

					/*if(k>=points->pixel_num)
					{
						assert(k<=points->pixel_num+1);
						DebugString("WaveTrack k>=points->pixel_num i=%d len=%d %d\n",i, len, __LINE__);
						continue;
					}*/
		        	if(last_k!=k)
		        	{
						assert(k<points->pixel_num);
				    	points->pixel[points->cur_pixel_num]=k;
				    	points->minvalue[points->cur_pixel_num]=buffer[i*2];
	                 	points->maxvalue[points->cur_pixel_num]=buffer[i*2+1];
						points->cur_pixel_num++;
			    	}
		          	else
					{
						points->minvalue[points->cur_pixel_num - 1] = points->minvalue[points->cur_pixel_num - 1] < buffer[i * 2] ? points->minvalue[points->cur_pixel_num - 1] : buffer[i * 2];
						points->maxvalue[points->cur_pixel_num - 1] = points->maxvalue[points->cur_pixel_num - 1] > buffer[i * 2 + 1] ? points->maxvalue[points->cur_pixel_num - 1] : buffer[i * 2 + 1];
					}
			    	last_k=k;
	        	}
			}
			else if(divisor==blocklen)
			{
				double bufmin, bufmax;
				(*list_Iter)->ReadMinMax(&bufmin, &bufmax);
				k=(int)((temp+(p-firstblockindex-index)*blocklen+divisor+ readbegin_integer - totalreadbegin_integer - readbegin_fractional)/points->samplesPerPixel)+plotbegin;

				if (k < 0)
					continue;

				/*if(k>=points->pixel_num)
				{
					assert(k<=points->pixel_num+1);
					DebugString("WaveTrack k>=points->pixel_num %d\n",__LINE__);
					continue;
				}*/
				if(last_k!=k)
				{
					assert(k<points->pixel_num);
					points->pixel[points->cur_pixel_num]=k;
				    points->minvalue[points->cur_pixel_num]=bufmin;
	                points->maxvalue[points->cur_pixel_num]=bufmax;
					points->cur_pixel_num++;
				}
				else
				{
					points->minvalue[points->cur_pixel_num-1]=points->minvalue[points->cur_pixel_num-1]<bufmin? points->minvalue[points->cur_pixel_num-1]:bufmin;
		        	points->maxvalue[points->cur_pixel_num-1]=points->maxvalue[points->cur_pixel_num-1]>bufmax? points->maxvalue[points->cur_pixel_num-1]:bufmax;
				}
				last_k=k;
			}
		}
		delete []buffer;

	//TRACE(_T("count = %d pixel[cur_pixel_num-1]=%d\n"), points->cur_pixel_num,points->pixel[points->cur_pixel_num-1]);
	return points->cur_pixel_num;
}

//现在没有打开导入计算rms功能，需要读取原始数据计算
double WaveTrack::ReadAndCalRma(uint64_t readbegin, uint64_t readlen)
{
	double rms_temp = 0;
	uint64_t rms_count = 0;

	//计算第一个块是那个块(从0开始计数)
	uint64_t firstblockindex = readbegin / blocklen;
	//读取的第一个块起始位置
	uint64_t firstblockbegin = readbegin % blocklen;

	//读取整个块的个数
	uint64_t blocknum = 0;
	if (readlen > blocklen - firstblockbegin)  //读取的长度超过了两个块
	{
		if (firstblockbegin != 0)   //第一个块不从开始读取
		{
			blocknum = (readlen - (blocklen - firstblockbegin)) / blocklen + 1;
			if (((readlen - (blocklen - firstblockbegin)) % blocklen) != 0)
				blocknum++;
		}
		else   //第一个块从开始读取
		{
			blocknum = readlen / blocklen;
			if ((readlen % blocklen) != 0)
				blocknum++;
		}
	}
	else  //总共读取在第一个块
		blocknum = 1;

	assert(firstblockindex + blocknum <= curblockcount);

	//读取数据缓冲区
	uint64_t buflen = blocklen;
	double* buffer = new double[buflen];

	int p = 0;
	int index = 0;
	uint64_t temp = 0;
	//TRACE(_T("读取的块 begin=%d "),points->cur_pixel_num);

	for (std::list<BlockFile*>::iterator list_Iter = filelist.begin(); list_Iter != filelist.end(); ++list_Iter, p++)
	{
		if (!((firstblockindex <= p) && (p < firstblockindex + blocknum)))
			continue;

		uint64_t begin, len;

		//计算每个块起始多余的位置
		if (p == firstblockindex)
		{
			temp = 0;
			index = 0;
		}
		else
		{
			if (firstblockbegin != 0)
			{
				temp = blocklen - firstblockbegin;
				index = 1;
			}
			else
			{
				temp = 0;
				index = 0;
			}
		}
		//计算每个块的读取位置和长度
		if (p == firstblockindex)
		{
			begin = firstblockbegin;
			len = readlen > blocklen - firstblockbegin ? (blocklen - firstblockbegin) : readlen;
		}
		else if (p == firstblockindex + blocknum - 1)
		{
			begin = 0;
			len = (readlen - blocklen * (p - firstblockindex - index) - temp);
		}
		else
		{
			begin = 0;
			len = blocklen;
		}
		//assert(len>0);
		if (len == 0) continue;

        //DebugString("Read = %lld len=%lld\n", begin, len);
		(*list_Iter)->Read(begin, buffer, &len);
		for (size_t i = 0; i < len; i++)
		{
			rms_temp += buffer[i] * buffer[i];
			rms_count++;
		}
	}

	delete[]buffer;

	//TRACE(_T("count = %d pixel[cur_pixel_num-1]=%d\n"), points->cur_pixel_num,points->pixel[points->cur_pixel_num-1]);
	return (rms_count > 0)? sqrt(rms_temp / rms_count) : 0;
}

double WaveTrack::ReadAndCalFromMinToMaxRma(uint64_t readbegin, uint64_t* readlen)
{
	double rms_temp = 0;

	//计算第一个块是那个块(从0开始计数)
	uint64_t firstblockindex = readbegin / blocklen;
	//读取的第一个块起始位置
	uint64_t firstblockbegin = readbegin % blocklen;

	//读取整个块的个数
	uint64_t blocknum = 0;
	if (*readlen > blocklen - firstblockbegin)  //读取的长度超过了两个块
	{
		if (firstblockbegin != 0)   //第一个块不从开始读取
		{
			blocknum = (*readlen - (blocklen - firstblockbegin)) / blocklen + 1;
			if (((*readlen - (blocklen - firstblockbegin)) % blocklen) != 0)
				blocknum++;
		}
		else   //第一个块从开始读取
		{
			blocknum = *readlen / blocklen;
			if ((*readlen % blocklen) != 0)
				blocknum++;
		}
	}
	else  //总共读取在第一个块
		blocknum = 1;

	assert(firstblockindex + blocknum <= curblockcount);

	//读取数据缓冲区
	uint64_t buflen = blocklen * blocknum;
	double* buffer = new double[buflen];
	if (buffer != nullptr)
	{
		uint64_t buffer_start = 0;

		int p = 0;
		int index = 0;
		uint64_t temp = 0;
		//TRACE(_T("读取的块 begin=%d "),points->cur_pixel_num);

		for (std::list<BlockFile*>::iterator list_Iter = filelist.begin(); list_Iter != filelist.end(); ++list_Iter, p++)
		{
			if (!((firstblockindex <= p) && (p < firstblockindex + blocknum)))
				continue;

			uint64_t begin, len;

			//计算每个块起始多余的位置
			if (p == firstblockindex)
			{
				temp = 0;
				index = 0;
			}
			else
			{
				if (firstblockbegin != 0)
				{
					temp = blocklen - firstblockbegin;
					index = 1;
				}
				else
				{
					temp = 0;
					index = 0;
				}
			}
			//计算每个块的读取位置和长度
			if (p == firstblockindex)
			{
				begin = firstblockbegin;
				len = *readlen > blocklen - firstblockbegin ? (blocklen - firstblockbegin) : *readlen;
			}
			else if (p == firstblockindex + blocknum - 1)
			{
				begin = 0;
				len = (*readlen - blocklen * (p - firstblockindex - index) - temp);
			}
			else
			{
				begin = 0;
				len = blocklen;
			}
			//assert(len>0);
			if (len == 0) continue;

            //DebugString("Read = %lld len=%lld\n", begin, len);
			(*list_Iter)->Read(begin, buffer + buffer_start, &len);
			buffer_start = buffer_start + len;
		}

		double min_v = buffer[0];
		uint64_t min_v_index = 0;
		double max_v = buffer[0];
		uint64_t max_v_index = 0;
		for (size_t i = 1; i < buffer_start; i++)
		{
			if (buffer[i] < min_v)
			{
				min_v = buffer[i];
				min_v_index = i;
			}

			if (buffer[i] > max_v)
			{
				max_v = buffer[i];
				max_v_index = i;
			}
		}

        //DebugString("buffer_start = %lld min_v_index = %lld max_v_index = %lld\n", buffer_start, min_v_index, max_v_index);

		uint64_t start_index = m_min(min_v_index, max_v_index);
		uint64_t stop_index = m_max(min_v_index, max_v_index);
		for (size_t i = start_index; i <= stop_index; i++)
		{
			rms_temp += buffer[i] * buffer[i];
		}
		*readlen = stop_index - start_index + 1;

		delete[]buffer;
	}
	//TRACE(_T("count = %d pixel[cur_pixel_num-1]=%d\n"), points->cur_pixel_num,points->pixel[points->cur_pixel_num-1]);
	return (*readlen > 0)? sqrt(rms_temp / *readlen) : 0;
}

bool WaveTrack::GetXAxisRange(double *min, double *max)
{
	*min=time_begin;
	if(curblockcount>0)
	{
		std::list<BlockFile*>::iterator list_Iter = filelist.end();
		--list_Iter;
		*max=time_begin+(curblockcount-1)*time_blocklen+(double)((*list_Iter)->GetCurLen())*time_blocklen/(double)blocklen;
	}
	else
     	*max=time_begin+curblockcount*time_blocklen;
	return true;
}

uint64_t WaveTrack::GetCurLength()
{
	uint64_t len=0;
	if(curblockcount>0)
	{
		std::list<BlockFile*>::iterator list_Iter = filelist.end();
		--list_Iter;
		len=(curblockcount-1)*blocklen+(*list_Iter)->GetCurLen();
	}
	return len;
}

char WaveTrack::GetIntervalFromMinToMaxRms(int pixel_start, double x_start, int pixel_stop, double x_stop, double* rms)
{ 
	char result = 0;
	
    //DebugString("pixel_start %d %0.3f    pixel_stop %d %0.3f\n", pixel_start, x_start, pixel_stop, x_stop);

	//如果绘图范围内直接获取最大最小值
	if (plotrecord.IsInRange(x_start, x_stop))
	{
		point min_point, max_point;
		if (plotrecord.GetMinMaxPoints(pixel_start, x_start, pixel_stop, x_stop, &min_point, &max_point))
		{
            //DebugString("\tIsInRange min_point time=%0.3f %0.3f    max_point time=%0.3f %0.3f\n", min_point.x, min_point.y, max_point.x, max_point.y);

			//读取数据缓冲区
			double time_start = m_min(min_point.x, max_point.x);
			double time_stop = m_max(min_point.x, max_point.x);

			uint64_t readbegin = (time_start - time_begin) * blocklen / time_blocklen;
			uint64_t readlen = (time_stop - time_start) * blocklen / time_blocklen;

            //DebugString("\t readbegin %lld    readlen %lld \n", readbegin, readlen);

			*rms = ReadAndCalRma(readbegin, readlen);
			//*rms_data_num = readlen;
			result = 1;
		}
	}
	else
	{
		//读取数据缓冲区
		double time_start = m_min(x_start, x_stop);
		double time_stop = m_max(x_start, x_stop);

		uint64_t readbegin = (time_start - time_begin) * blocklen / time_blocklen;
		uint64_t readlen = (time_stop - time_start) * blocklen / time_blocklen;

        //DebugString("\t readbegin %lld    readlen %lld \n", readbegin, readlen);

		*rms = ReadAndCalFromMinToMaxRma(readbegin, &readlen);
		//*rms_data_num = readlen;
		result = 1;
	}

	return result;
}
