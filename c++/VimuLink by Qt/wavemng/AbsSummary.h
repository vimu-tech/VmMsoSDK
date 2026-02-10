#ifndef ABSSUMMARY_H
#define ABSSUMMARY_H

/********************************************************************************************************************
    This file is used as part of the vimu Qt demo and you can modify and use it as you please
    www.vimu.top
*********************************************************************************************************************/

#include "BlockFileInfo.h"
#include "SummaryInfo.h"
#include <math.h>
#include <memory.h>

class AbsSummary
{
public:
	AbsSummary(BlockFileInfo *pblockfileinfo);
	virtual ~AbsSummary(void);

	virtual bool CalSummary(void *buf, int64_t start, uint64_t len)=0;
     /// Returns the 256 byte summary data block
    virtual bool Read256(double *buffer, int64_t start, uint64_t len)=0;
    /// Returns the 64K summary data block
    virtual bool Read64K(double *buffer, int64_t start, uint64_t len)=0;
	//
	bool ReadMinMax(double *bufmin, double *bufmax)
	{
		*bufmin = mMin;
		*bufmax = mMax;
		return true;
	}
protected:
	template<typename T> void CalSummary2(char *fullSummary, T* buffer, int64_t start, uint64_t len)
	{
		/*
		buffer ++++++++256++++++++++256*2++++++++++++256*3++++++++start+++++++256*4++++++++++++++++++++++++++++++++++++++end++++++++256*n+++++++++++++++++++++++++++++++
																	+--------------------------len------------------------+  end=srart+len
																256*3+5=773					   500							1273=256*4+249
		*/
		T* buf = buffer - start;  //buf偏移到0位置，读取的时候正好读取实际的buffer
		uint64_t end = start + len;  //1273

		//复制Summary头
		memcpy(fullSummary, headerTag, headerTagLen);

		double *summary64K = (double *)(fullSummary + mSummaryInfo.offset64K);
		double *summary256 = (double *)(fullSummary + mSummaryInfo.offset256);

		//-------------------------------------------------计算256点summaries----------------------------------------------
		uint64_t sumStart256 = start / 256;  //3
		uint64_t sumoffset256 = start % 256;  //5
		uint64_t sumEnd256 = (end + 255) / 256;  //5

		assert(sumEnd256 <= mSummaryInfo.frames256);
		//DebugString("start=%lld len=%lld   sumoffset=%lld,   sumStart=%lld, sumEnd256=%lld\n", start, len, sumoffset256, sumStart256, sumEnd256);

		//处理多余
		if (sumoffset256 != 0)
		{
			double min = summary256[sumStart256 * 2];
			double max = summary256[sumStart256 * 2 + 1];

			//DebugString("sumoffset256  start=%lld  end=%lld\n", start, start + 256 - sumoffset256);
			for (uint64_t k = start; k < start + 256 - sumoffset256; k++)
			{
				if (buf[k] < min)
					min = buf[k];
				else if (buf[k] > max)
					max = buf[k];
			}

			summary256[sumStart256 * 2] = min;
			summary256[sumStart256 * 2 + 1] = max;

			sumStart256++;  //4
		}

		//处理整块
		for (uint64_t i = sumStart256; i < sumEnd256 && i < mSummaryInfo.frames256; i++)
		{
			uint64_t index = i * 256;
			double min = buf[index];
			double max = buf[index];

			uint64_t jcount = 256;
			if (index + jcount > end)
			{
				jcount = 256 - (index + jcount - end); 
				//DebugString("            start=%lld  end=%lld\n", index, index + jcount);
			}
				
			for (uint64_t j = 1; j < jcount; j++)
			{
				double f1 = buf[index + j];
				if (f1 < min)
					min = f1;
				else if (f1 > max)
					max = f1;
			}

			summary256[i * 2] = min;
			summary256[i * 2 + 1] = max;
		}
		//填充最后剩余
		//DebugString("            sumEnd256=%lld  mSummaryInfo.frames256=%d\n", sumEnd256, mSummaryInfo.frames256);
		for (uint64_t i = sumEnd256; i < mSummaryInfo.frames256; i++)
		{                                                            //使用最后的数据填充，而不要使用0
			summary256[i * 2] = summary256[(sumEnd256 - 1) * 2];			//summary256[i * 2] = 0.0f;
			summary256[i * 2 + 1] = summary256[(sumEnd256 - 1) * 2 + 1];	//summary256[i * 2 + 1] = 0.0f;
		}

		//-------------------------------------------------计算64K点summaries----------------------------------------------
		//注意这里整个64K区域重新计算，避免设计繁杂
		uint64_t sumLen64K = (end + 65535) / 65536;   //sumLen = (start + len + 65535) / 65536; 20190517修改
		assert(sumLen64K <= mSummaryInfo.frames64K);

		for (uint64_t i = 0; i < sumLen64K && i < mSummaryInfo.frames64K; i++)
		{
			double min = summary256[2 * i * 256];
			double max = summary256[2 * i * 256 + 1];

			for (uint64_t j = 1; j < 256; j++)
			{
				if (summary256[2 * (i * 256 + j)] < min)
					min = summary256[2 * (i * 256 + j)];
				if (summary256[2 * (i * 256 + j) + 1] > max)
					max = summary256[2 * (i * 256 + j) + 1];
			}
			summary64K[i * 2] = min;
			summary64K[i * 2 + 1] = max;
		}
		//填充最后
		for (uint64_t i = sumLen64K; i < mSummaryInfo.frames64K; i++)
		{                                                            //使用最后的数据填充，而不要使用0
			summary64K[i * 2] = summary64K[(sumLen64K - 1) * 2];                //summary64K[i * 2] = 0.0f;
			summary64K[i * 2 + 1] = summary64K[(sumLen64K - 1) * 2 + 1];        //summary64K[i * 2 + 1] = 0.0f;
		}

		//-------------------------------------------------计算整个块summaries----------------------------------------------
		mMin = summary64K[0];
		mMax = summary64K[1];

		for (uint64_t i = 1; i < sumLen64K; i++)
		{
			if (summary64K[2 * i] < mMin)
				mMin = summary64K[2 * i];
			if (summary64K[2 * i + 1] > mMax)
				mMax = summary64K[2 * i + 1];
		}
	}

protected:
	BlockFileInfo *pblockfileinfo;
    SummaryInfo mSummaryInfo;	
 
	double mMin;
	double mMax;
};

#endif
