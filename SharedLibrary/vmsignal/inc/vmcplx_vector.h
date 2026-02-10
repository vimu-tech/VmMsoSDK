#ifndef VMCPLX_VECTOR_H
#define VMCPLX_VECTOR_H

/********************************************************************************************************************
说明：
1,该数字信号处理库，使用标准的C语言编写的，没有针对任何的平台进行更好的优化。
2,在使用中发现任何的bug或者建议，可以联系我们。

Note:
1,This digital signal processing library, written in the standard C language, is not better optimized for any platform.
2,If you find any bugs or suggestions in use, you can contact us.

	www.vimu.top
	www.instrustar.com
*********************************************************************************************************************/


#include <vector>
#include <complex>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>

template <typename Type, typename _Ax = std::allocator<Type> > 
	class Vector : public std::vector<Type, _Ax>
{
public:
	 // constructors and destructor
	Vector()
	: std::vector<Type,_Ax>()
	{
	};

	Vector(int32_t length)
	: std::vector<Type,_Ax>(length)
	{
	};

	Vector(size_t length)
	: std::vector<Type,_Ax>(length)
	{
	};

	void zeros(size_t beginpos, size_t num)
	{
		if(this->size()<beginpos+num)
			this->resize(beginpos+num);
		for(size_t i=beginpos;i<beginpos+num;i++)
			this->at(i)=0;
	};

	void full(size_t beginpos, size_t num, const std::complex<double> k)
	{
		if(this->size()<beginpos+num)
			this->resize(beginpos+num);
		for(size_t i=beginpos;i<beginpos+num;i++)
			this->at(i)=k;
	};
};

// arithmetic operators
template<typename Type>
Vector<Type> operator-( const Vector<Type>& v)
{
	Vector<Type> tmp(v.size());

    typename Vector<Type>::iterator itrL = tmp.begin();
    typename Vector<Type>::const_iterator itrR = v.begin();

    while( itrL != tmp.end() )
        *itrL++ = -(*itrR++);

    return tmp;
}

template<typename Type>
Vector<Type> operator-(const std::complex<double> k, const Vector<Type>& v)
{
	Vector<Type> tmp(v.size());

    typename Vector<Type>::iterator itrL = tmp.begin();
    typename Vector<Type>::const_iterator itrR = v.begin();

    while( itrL != tmp.end() )
        *itrL++ = k-(*itrR++);

    return tmp;
}

template<typename Type>
Vector<Type> operator*( const Vector<Type>& v, const std::complex<double> k)
{
	Vector<Type> tmp(v.size());

    typename Vector<Type>::iterator itrL = tmp.begin();
    typename Vector<Type>::const_iterator itrR = v.begin();

    while( itrL != tmp.end() )
        *itrL++ = (*itrR++)*k;

    return tmp;
}

template<typename Type>
Vector<Type> operator/( const Vector<Type>& v, const std::complex<double> k)
{
	Vector<Type> tmp(v.size());

    typename Vector<Type>::iterator itrL = tmp.begin();
    typename Vector<Type>::const_iterator itrR = v.begin();

    while( itrL != tmp.end() )
        *itrL++ = (*itrR++)/k;

    return tmp;
}

template<typename Type>
Vector<Type> operator*( const Vector<Type>& a, const Vector<Type>& b)
{
	Vector<Type> tmp(a.size()+b.size()-1);
	tmp.zeros(0,a.size()+b.size()-1);

	for(size_t i=0;i<a.size();i++)
	{
		for(size_t j=0;j<b.size();j++)
		{
			tmp[i+j]+=a[i]*b[j];
		}
	}
	return tmp;
}
/*
Zero-pole to transfer function conversion.
   [NUM,DEN] = ZP2TF(Z,P,K)  forms the transfer function:

                 b(0)+b(1)s^(1)+ ... + b(lb)s^(lb)
       H(s) = ------------------------------------------
                 1.0 +a(1)s^(1)+ ... + a(la)s^(la)
*/

template<typename Type>
void zp2tfs(const Vector<Type> z, const Vector<Type> p, const double k, Vector<Type>& b, Vector<Type>& a)
{
	Vector<Type> tmp(2);

	b.resize(2);
	b[1]=Type(1.0,0); //s
	b[0]=-z[0];
	for(size_t i=1;i<z.size();i++)
	{
		tmp[1]=Type(1.0,0); //s
		tmp[0]=-z[i];
		b=b*tmp;
	}

	//乘以k
	for(size_t i=0;i<b.size();i++)
		b[i]=b[i]*k;

	a.resize(2);
	a[1]=Type(1.0,0); //s
	a[0]=-p[0];
	for(size_t i=1;i<p.size();i++)
	{
		tmp[1]=Type(1.0,0); //s
		tmp[0]=-p[i];
		a=a*tmp;
	}
}

template<typename Type>
void zp2tfs(const Vector<Type> z, const Vector<Type> p, const double k, std::vector<double>& b, std::vector<double>& a)
{
	Vector<Type> tmp_a,tmp_b;

	zp2tf(z, p, k, tmp_b, tmp_a);

	a.resize(tmp_a.size());
	for(size_t i=0;i<tmp_a.size();i++)
	{
		a[i]=tmp_a[i].real();
		assert(tmp_a[i].imag()<1e-6);
	}

	b.resize(tmp_b.size());
	for(size_t i=0;i<tmp_b.size();i++)
	{
		b[i]=tmp_b[i].real();
		assert(tmp_b[i].imag()<1e-6);
	}
}

/*
Zero-pole to transfer function conversion.
   [NUM,DEN] = ZP2TF(Z,P,K)  forms the transfer function:

			     b(0)+b(1)z^(-1)+ ... + b(lb)z^(-lb)
       H(z) = ------------------------------------------
                 1.0 +a(1)z^(-1)+ ... + a(la)z^(-la)
*/

template<typename Type>
void zp2tfz(const Vector<Type> z, const Vector<Type> p, const double k, Vector<Type>& b, Vector<Type>& a)
{
	Vector<Type> tmp(2);

	b.resize(2);
	b[1]=Type(1.0,0); //s
	b[0]=-z[0];
	for(size_t i=1;i<z.size();i++)
	{
		tmp[1]=Type(1.0,0); //s
		tmp[0]=-z[i];
		b=b*tmp;
	}

	//乘以k
	for(size_t i=0;i<b.size();i++)
		b[i]=b[i]*k;

	a.resize(2);
	a[1]=Type(1.0,0); //s
	a[0]=-p[0];
	for(size_t i=1;i<p.size();i++)
	{
		tmp[1]=Type(1.0,0); //s
		tmp[0]=-p[i];
		a=a*tmp;
	}

	//z^n变成z^-n  形式
	Type t;
	for(size_t i=0;i<(b.size()+1)/2;i++)
	{
		t=b[i];
		b[i]=b[b.size()-1-i];
		b[b.size()-1-i]=t;
	}

	for(size_t i=0;i<(a.size()+1)/2;i++)
	{
		t=a[i];
		a[i]=a[a.size()-1-i];
		a[a.size()-1-i]=t;
	}
}

template<typename Type>
void zp2tfz(const Vector<Type> z, const Vector<Type> p, const double k, std::vector<double>& b, std::vector<double>& a)
{
	Vector<Type> tmp_a,tmp_b;

	zp2tfz(z, p, k, tmp_b, tmp_a);

	a.resize(tmp_a.size());
	for(size_t i=0;i<tmp_a.size();i++)
	{
		a[i]=tmp_a[i].real();
		assert(tmp_a[i].imag()<1e-6);
	}

	b.resize(tmp_b.size());
	for(size_t i=0;i<tmp_b.size();i++)
	{
		b[i]=tmp_b[i].real();
		assert(tmp_b[i].imag()<1e-6);
	}
}


typedef Vector<std::complex<double>> cplx_vector;

#endif