#pragma once

#include "MyImage.h"

#define IN_IMG(x, lo, hi) (x < lo)? lo : x > hi ? hi : x
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#define MAX(a,b)  ((a) < (b) ? (b) : (a))


template <typename T1, typename T2, typename TO>
void AddImage(const CMyImage<T1>& src1, const CMyImage<T2>& src2, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc1[c] + pSrc2[c]);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc1[c] + pSrc2[c];
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator+(const CMyImage<T1>& src1, const CMyImage<T2>& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	CMyImage<T1> ret(nWidth1, nHeight1, nChan1);
	AddImage(src1, src2, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void AddConst(const CMyImage<T1>& src, const T2 val, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc[c] + val);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc[c] + val;
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator+(const CMyImage<T1>& src, const T2 val)
{
	int nWidth  = src.GetWidth();
	int nHeight = src.GetHeight();
	int nChan   = src.GetChannel();

	CMyImage<T1> ret(nWidth, nHeight, nChan);
	AddConst(src, val, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void SubImage(const CMyImage<T1>& src1, const CMyImage<T2>& src2, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc1[c] - pSrc2[c]);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc1[c] - pSrc2[c];
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator-(const CMyImage<T1>& src1, const CMyImage<T2>& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	CMyImage<T1> ret(nWidth1, nHeight1, nChan1);
	SubImage(src1, src2, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void SubConst(const CMyImage<T1>& src, const T2 val, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc[c] - val);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc[c] - val;
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator-(const CMyImage<T1>& src, const T2 val)
{
	int nWidth  = src.GetWidth();
	int nHeight = src.GetHeight();
	int nChan   = src.GetChannel();

	CMyImage<T1> ret(nWidth, nHeight, nChan);
	SubConst(src, val, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void MulImage(const CMyImage<T1>& src1, const CMyImage<T2>& src2, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel(); 
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc1[c] * pSrc2[c]);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc1[c] * pSrc2[c];
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator*(const CMyImage<T1>& src1, const CMyImage<T2>& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	CMyImage<T1> ret(nWidth1, nHeight1, nChan1);
	MulImage(src1, src2, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void MulConst(const CMyImage<T1>& src, const T2 val, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc[c] * val);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc[c] * val;
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator*(const CMyImage<T1>& src, const T2 val)
{
	int nWidth  = src.GetWidth();
	int nHeight = src.GetHeight();
	int nChan   = src.GetChannel();

	CMyImage<T1> ret(nWidth, nHeight, nChan);
	MulConst(src, val, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void DivImage(const CMyImage<T1>& src1, const CMyImage<T2>& src2, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				if (pSrc2[c]!=0)
					pDst[c] = CLIP(pSrc1[c] / pSrc2[c]);
				else
					pDst[c] = 255;
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc1 = src1.GetPtr(r);
			T2* pSrc2 = src2.GetPtr(r);
			TO* pDst  = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc1[c] / pSrc2[c];
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator/(const CMyImage<T1>& src1, const CMyImage<T2>& src2)
{
	int nWidth1  = src1.GetWidth();
	int nHeight1 = src1.GetHeight();
	int nWidth2  = src2.GetWidth();
	int nHeight2 = src2.GetHeight();
	int nChan1   = src1.GetChannel();
	int nChan2   = src2.GetChannel();

	ASSERT(nWidth1==nWidth2 && nHeight1==nHeight2 && nChan1==nChan2);

	CMyImage<T1> ret(nWidth1, nHeight1, nChan1);
	DivImage(src1, src2, ret);
	return ret;
}

template <typename T1, typename T2, typename TO>
void DivConst(const CMyImage<T1>& src, const T2 val, CMyImage<TO>& dst)
{
	int nWStep  = dst.GetWidth()*dst.GetChannel();
	int nHeight = dst.GetHeight();

	int r, c;

	if (sizeof(TO)==1) // BYTE형 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = CLIP(pSrc[c] / val);
			}
		}
	}
	else // BYTE형 이외의 영상
	{
		for (r=0 ; r<nHeight ; r++)
		{
			T1* pSrc = src.GetPtr(r);
			TO* pDst = dst.GetPtr(r);

			for (c=0 ; c<nWStep ; c++)
			{
				pDst[c] = pSrc[c] / val;
			}
		}
	}
}

template <typename T1, typename T2>
CMyImage<T1> operator/(const CMyImage<T1>& src, const T2 val)
{
	int nWidth  = src.GetWidth();
	int nHeight = src.GetHeight();
	int nChan   = src.GetChannel();

	CMyImage<T1> ret(nWidth, nHeight, nChan);
	DivConst(src, val, ret);
	return ret;
}

// 논리 연산자 선언
void ANDImage(const CByteImage& src1, const CByteImage& src2, CByteImage& dst);
CByteImage operator&(const CByteImage& src1, const CByteImage& src2);
void ANDConst(const CByteImage& src, const BYTE val, CByteImage& dst);
CByteImage operator&(const CByteImage& src, const BYTE val);
void ORImage(const CByteImage& src1, const CByteImage& src2, CByteImage& dst);
CByteImage operator|(const CByteImage& src1, const CByteImage& src2);
void ORConst(const CByteImage& src, const BYTE val, CByteImage& dst);
CByteImage operator|(const CByteImage& src, const BYTE val);
void NOTImage(const CByteImage& src, CByteImage& dst);
CByteImage operator~(const CByteImage& src);
void XORImage(const CByteImage& src1, const CByteImage& src2, CByteImage& dst);
CByteImage operator^(const CByteImage& src1, const CByteImage& src2);
void XORConst(const CByteImage& src, const BYTE val, CByteImage& dst);
CByteImage operator^(const CByteImage& src, const BYTE val);