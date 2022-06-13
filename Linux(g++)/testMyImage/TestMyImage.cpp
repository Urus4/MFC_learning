#include "MyImage.h"
#include <iostream>
using namespace std;

int main()
{
	CByteImage image1(640, 480);	//생성자 호출
	image1.SetConstValue(0);		//0으로 초기화
	
	CByteImage image2(image1);		//복사 생성자 호출
	CByteImage image3;				//기본 생성자 호출
	CByteImage image4;
	image3 = image1;
	image4 = image1;

	int nWidth = image1.GetWidth();		//영상의 너비
	int nHeight = image1.GetHeight();	//영상의 높이
	int nChannel = image1.GetChannel();	//영상의 채널 수

	double incX = 256.0 / nWidth;		// 256 / 640 = 0.4 
	double incY = 256.0 / nHeight;		// 256 / 480 = 0.533
	int r, c;
	for (r = 0; r < nHeight; r++)		// 행 단위 이동
	{
		for(c=0; c < nWidth; c++)		// column 가로 , row 세로
		{ 
			image2.GetAt(c, r) = (BYTE)(c * incX);	// column 그라데이션 -> 같은 row(열) 들은 같은 값을 가지므로 가로로 그라데이션이 생김
			image3.GetAt(c, r) = (BYTE)(r * incY);	// row 그라데이션 -> 같은 column(행) 들은 같은 값을 가지므로 세로로 그라데이션이 생김

			image4.GetAt(c, r) = (image2.GetAt(c, r) + image3.GetAt(c, r))/2; // 대각선 기준 그라데이션

			if (c < 10 || 630 < c)					// 픽셀 데이터 확인용
				cout << (int)(r * incY) << " ";
		}
		cout << endl;
	}

	//결과 영상 저장
	image1.SaveImage("Black.bmp");
	image2.SaveImage("GradationX.bmp");
	image3.SaveImage("GradationY.bmp");
	image4.SaveImage("Gradation.bmp");
	return 0;
}
