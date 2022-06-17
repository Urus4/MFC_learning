#pragma once
//#include <windows.h>
#include <assert.h>
#include <cstdio>
#include "bmpheader.h"
#include <cstring>
#define CLIP(x) (x<0) ? 0 : x > 255 ? 255:x		//픽셀값에 음수(x<0)가 들어가면 안되기 때문에(작은거에서 큰걸 뺄때) / 255보다 크면 안되기 때문에(더해지면 커질 수도 있기 때문에

template <typename T>
class CMyImage
{
protected:
	int m_nChannels;	//채널수
	int m_nHeight;		//세로 픽셀 수
	int m_nWidth;		//가로 픽셀 수
	int m_nWStep;		//행당 데이터 원소 수
	T* m_pImageData;	//픽셀 배열 포인터
	
public:
	CMyImage(void)		// 클래스 디폴트 생성자
		: m_nChannels(0)
		, m_nHeight(0)
		, m_nWidth(0)
		, m_nWStep(0)
		, m_pImageData(NULL)
	{
	}

	CMyImage(int nWidth, int nHeight, int nChannels = 1)					// [ 생성자 ], channel = 1 -> graycolor라는뜻
		: m_nChannels(nChannels)											// 4의 배수에 가장 근사한 값을 만들기 위한 계산 ,
		, m_nHeight(nHeight)												// cpu가 4바이트(32비트) 단위로 계산해야 효율적이므로 계산후 실제 바이트보다 초과되는 바이트를 더미 값을 만들어준다.
		, m_nWidth(nWidth)													// ex ) 21 x 10 해상도 일때 ->  (( 26 * 1 * 1 + 3 ) & ~00000011 ) / 1
		, m_nWStep(((nWidth* nChannels * sizeof(T) + 3) & ~3) / sizeof(T))	//		= (00011101 & 11111100) / 1 = 00011100 = 28																	
	{																				
		m_pImageData = new T[m_nHeight * m_nWStep];							// 책 p482 참고
	}																		

	CMyImage(const CMyImage& myImage)										// [ 복사 생성자 (1) ]
	{
		m_nChannels = myImage.m_nChannels;
		m_nHeight = myImage.m_nHeight;
		m_nWidth = myImage.m_nWidth;
		m_nWStep = myImage.m_nWStep;
		m_pImageData = new T[m_nHeight * m_nWStep];										// 깊은 복사를 위해 동적할당 해줌
		memcpy(m_pImageData, myImage.m_pImageData, m_nHeight * m_nWStep * sizeof(T));	// 메모리 카피
	}

	CMyImage& operator=(const CMyImage& myImage)		// [ 연산자 중복 정의 ]
	{
		if (this == &myImage)
			return *this;						

		m_nChannels = myImage.m_nChannels;
		m_nHeight = myImage.m_nHeight;
		m_nWidth = myImage.m_nWidth;
		m_nWStep = myImage.m_nWStep;

		if (m_pImageData)								// 널포인터면(복사할게 없으면)
			delete[] m_pImageData;

		if (myImage.m_pImageData != NULL)				// 널포인터가 아니면(복사할게 있으면)
		{
			m_pImageData = new T[m_nHeight * m_nWStep]; 
			memcpy(m_pImageData, myImage.m_pImageData, m_nHeight * m_nWStep * sizeof(T));
		}
		return *this;
	}

	template <typename From>
	CMyImage(const CMyImage < From > &myImage)			// [ 복사 생성자 (2) ] --> 복사생성자(1)하고 다른점? --> 클래스는 동일하나 다른 타입의 객체가 전달될 경우 이 생성자가 실행
	{													//													  ( ex ) 원본이 int형인데 복사할것은 double일때 등
		m_nChannels = myImage.GetChannel();
		m_nHeight = myImage.GetHeight();
		m_nWidth = myImage.GetWidth();
		m_nWStep = ((m_nWidth * m_nChannels * sizeof(T) + 3) & ~3) / sizeof(T);
		m_pImageData = new T[m_nHeight * m_nWStep];

		int nWStep = myImage.GetWStep();

		if (sizeof(T) == 1)
		{
			for (int r = 0; r < m_nHeight; r++)		// 타입이 서로 다를때 
			{
				T* pDst = GetPtr(r);					
				From* pSrc = myImage.GetPtr(r);		
				for (int c = 0; c < nWStep; c++)
				{
					pDst[c] = (T)CLIP(pSrc[c]);	
				}
			}
		}
		else
		{
			for (int r = 0; r < m_nHeight; r++)		// 타입이 서로 같을때
			{
				T* pDst = GetPtr(r);
				From* pSrc = myImage.GetPtr(r);
				for (int c = 0; c < nWStep; c++)
				{
					pDst[c] = (T)pSrc[c];
				}
			}
		}
	}

	~CMyImage(void)									// [ 소멸자 ]
	{
		if (m_pImageData) delete[] m_pImageData;
	}

	bool LoadImage(const char* filename)			// [ 이미지 로드 ]
	{
		assert(sizeof(T) == 1);						// BYTE형의 경우만 가능

		if (!strcmp(".bmp", &filename[strlen(filename) - 4]))
		{
			FILE* pFile = NULL;
			pFile=fopen(filename, "rb");		//바이너리 읽기 모드
			if (!pFile)
				return false;

			BITMAPFILEHEADER fileHeader;

			if (!fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pFile))
			{
				fclose(pFile);
				return false;
			}

			if (fileHeader.bfType != 0x4D42)		// 'BM' 문자 검사
			{
				fclose(pFile);
				return false;
			}

			BITMAPINFOHEADER infoHeader;

			if (!fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pFile))
			{
				fclose(pFile);
				return false;
			}

			if (infoHeader.biBitCount != 8 && infoHeader.biBitCount != 24)
			{
				fclose(pFile);
				return false;
			}

			if (m_nWidth != infoHeader.biWidth && m_nHeight != infoHeader.biHeight && m_nChannels != infoHeader.biBitCount / 8)
			{
				if (m_pImageData)
					delete[] m_pImageData;

				m_nChannels = infoHeader.biBitCount / 8;
				m_nHeight = infoHeader.biHeight;
				m_nWidth = infoHeader.biWidth;
				m_nWStep = (m_nWidth * m_nChannels * sizeof(T) + 3) & ~3;

				m_pImageData = new T[m_nHeight * m_nWStep];
			}

			fseek(pFile, fileHeader.bfOffBits, SEEK_SET);

			int r;
			for (r = m_nHeight - 1; r >= 0; r--)
			{
				if (!fread(&m_pImageData[r * m_nWStep], sizeof(BYTE), m_nWStep, pFile))
				{
					fclose(pFile);
					return false;
				}
			}

			fclose(pFile);

			return true;
		}
		else
		{
			return false;
		}
	}

	bool SaveImage(const char* filename)
	{
		assert(sizeof(T) == 1); // BYTE형의 경우만 가능

		if (!strcmp(".bmp", &filename[strlen(filename) - 4]))	// "black.bmp" 로 예를들어 설명하면 -> strlen(filename) = 8 -> 8-4 = 4 -> '.'의 위치를 가리킴
		{
			FILE* pFile = NULL;
			pFile=fopen(filename, "wb");					// _s는 비주얼스튜디오에서만 사용가능한 문법(리눅스에서 사용못하니 되도록 사용하지 말자)
			if (!pFile)
 				return false;									// 파일을 못 열었으면 끄기

			BITMAPFILEHEADER fileHeader;
			fileHeader.bfType = 0x4D42; //'BM'
			fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_nWStep * m_nHeight + (m_nChannels == 1) * 1024;

			fileHeader.bfReserved1 = 0;
			fileHeader.bfReserved2 = 0;
			fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (m_nChannels == 1) * 256 * sizeof(RGBQUAD);

			fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);

			BITMAPINFOHEADER infoHeader;
			infoHeader.biSize = sizeof(BITMAPINFOHEADER);
			infoHeader.biWidth = m_nWidth;
			infoHeader.biHeight = m_nHeight;
			infoHeader.biPlanes = 1;
			infoHeader.biBitCount = m_nChannels * 8;
			infoHeader.biCompression =0;
			infoHeader.biSizeImage = m_nWStep * m_nHeight;
			infoHeader.biClrImportant = 0;
			infoHeader.biClrUsed = 0;
			infoHeader.biXPelsPerMeter = 0;
			infoHeader.biYPelsPerMeter = 0;

			fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

			if (m_nChannels == 1)													
			{
				for (int l = 0; l < 256; l++)
				{																	//팔레트 데이터, RGBQUAD 구조체를 통해 명시
					RGBQUAD GrayPalette = { (BYTE)l,(BYTE)l,(BYTE)l,(BYTE)0 };		// { R, G, B, 0} --> R,G,B에 0~255의 수를 같게 넣은것이, 그레이컬러의 팔레트 정보이다.
					fwrite(&GrayPalette, sizeof(RGBQUAD), 1, pFile);				// 네번째 값은 투명도(알파값) 등을 위한 예약공간
				}																	// 4바이트의 크기를 갖는다
			}

			int r;
			for (r = m_nHeight - 1; r >= 0; r--)
			{
				fwrite(&m_pImageData[r * m_nWStep], sizeof(BYTE), m_nWStep, pFile);	// 헤더의 크기는 1076바이트로 1078부터가 픽셀데이터가 저장된다.
			}

			fclose(pFile);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsEmpty() const
	{
		return m_pImageData ? false : true;
	}

	void SetConstValue(T val)										// 초기화
	{
		if (val == 0)  // 조건 (1)
		{
			memset(m_pImageData, 0, m_nWStep * m_nHeight * sizeof(T));
			return;
		}

		if (sizeof(T) == 1)  // 조건 (2)
		{
			memset(m_pImageData, val, m_nWStep * m_nHeight);
		}
		else  // 나머지 경우들
		{
			T* pData = m_pImageData;
			for (int r = 0; r < m_nHeight; r++)
			{
				for (int c = 0; c < m_nWidth; c++)
				{
					pData[c] = val;
				}
				pData += m_nWStep;
			}
		}
	}

	inline T& GetAt(int x, int y, int c = 0) const					// 픽셀 위치
	{
		assert(x >= 0 && x < m_nWidth && y >= 0 && y < m_nHeight);
		return m_pImageData[m_nWStep * y + m_nChannels * x + c];
	}

	int GetChannel() const { return m_nChannels; }
	int GetHeight()  const { return m_nHeight; }
	int GetWidth()   const { return m_nWidth; }
	int GetWStep()   const { return m_nWStep; }
	T* GetPtr(int r = 0, int c = 0) const { return m_pImageData + r * m_nWStep + c; }
};

typedef CMyImage <BYTE  > CByteImage;
typedef CMyImage <int   > CIntImage;
typedef CMyImage <float > CFloatImage;
typedef CMyImage <double> CDoubleImage;
