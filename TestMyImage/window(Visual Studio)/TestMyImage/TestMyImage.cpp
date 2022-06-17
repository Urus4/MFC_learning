#include "imageSrc/MyImage.h"
#include <iostream>
using namespace std;

int main()
{
	CByteImage image1(640, 480);	//������ ȣ��
	image1.SetConstValue(0);		//0���� �ʱ�ȭ
	
	CByteImage image2(image1);		//���� ������ ȣ��
	CByteImage image3;				//�⺻ ������ ȣ��
	CByteImage image4;
	image3 = image1;
	image4 = image1;

	int nWidth = image1.GetWidth();		//������ �ʺ�
	int nHeight = image1.GetHeight();	//������ ����
	int nChannel = image1.GetChannel();	//������ ä�� ��

	double incX = 256.0 / nWidth;		// 256 / 640 = 0.4 
	double incY = 256.0 / nHeight;		// 256 / 480 = 0.533
	int r, c;
	for (r = 0; r < nHeight; r++)		// �� ���� �̵�
	{
		for(c=0; c < nWidth; c++)		// column ���� , row ����
		{ 
			image2.GetAt(c, r) = (BYTE)(c * incX);	// column �׶��̼� -> ���� row(��) ���� ���� ���� �����Ƿ� ���η� �׶��̼��� ����
			image3.GetAt(c, r) = (BYTE)(r * incY);	// row �׶��̼� -> ���� column(��) ���� ���� ���� �����Ƿ� ���η� �׶��̼��� ����

			image4.GetAt(c, r) = (image2.GetAt(c, r) + image3.GetAt(c, r))/2; // �밢�� ���� �׶��̼�

			if (c < 10 || 630 < c)					// �ȼ� ������ Ȯ�ο�
				cout << (int)(r * incY) << " ";
		}
		cout << endl;
	}

	//��� ���� ����
	image1.SaveImage("Black.bmp");
	image2.SaveImage("GradationX.bmp");
	image3.SaveImage("GradationY.bmp");
	image4.SaveImage("Gradation.bmp");
	return 0;
}