
// MyImageEditorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyImageEditor.h"
#include "MyImageEditorDlg.h"
#include "afxdialogex.h"

#include "imageSrc/ImageFrameWndManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyImageEditorDlg 대화 상자



CMyImageEditorDlg::CMyImageEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYIMAGEEDITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyImageEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyImageEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INVERSION, &CMyImageEditorDlg::OnBnClickedButtonInversion)
	ON_BN_CLICKED(IDC_BUTTON_BLENDING, &CMyImageEditorDlg::OnBnClickedButtonBlending)
END_MESSAGE_MAP()


// CMyImageEditorDlg 메시지 처리기

BOOL CMyImageEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyImageEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyImageEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyImageEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyImageEditorDlg::OnBnClickedButtonInversion()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 파일 대화 상자에서 BMP 형식의 파일만 보이도록 하는 필터의 정의
	char szFilter[] = "BMP File (*.BMP) | *.BMP; | All Files(*.*)|*.*||";

	// 파일 대화 상자의 생성
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // 대화 상자 열기
	{
		CString strPathName = dlg.GetPathName(); // 영상 파일 경로 획득

		CByteImage imageIn;
		if (!imageIn.LoadImage(strPathName)) // 영상 읽기
		{
			return; // 영상 읽기에 실패하면 함수를 종료
		}

		// 입력 영상의 화면 출력, 두번째 인자는 출력 창의 이름
		ShowImage(imageIn, "Input");

		int nWidth = imageIn.GetWidth();   // 영상의 너비
		int nHeight = imageIn.GetHeight();  // 영상의 높이
		int nChannel = imageIn.GetChannel(); // 영상의 채널 수

		CByteImage imageOut(nWidth, nHeight, nChannel); // 결과 영상 클래스 생성

		int r, c, h;
		for (r = 0; r < nHeight; r++) // 행 단위 이동
		{
			for (c = 0; c < nWidth; c++) // 열 단위 이동
			{
				for (h = 0; h < nChannel; h++) // 채널 단위 이동
				{
					// 각 채널 밝기 값을 반전
					imageOut.GetAt(c, r, h) = 255 - imageIn.GetAt(c, r, h);
				}
			}
		}

		imageOut.SaveImage("inversion.bmp"); // 결과 영상의 파일 저장
		ShowImage(imageOut, "반전 결과");	 // 결과 영상의 화면 출력
	}
}

void CMyImageEditorDlg::OnBnClickedButtonBlending()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 파일 대화 상자에서 BMP 형식의 파일만 보이도록 하는 필터의 정의
	char szFilter[] = "BMP File (*.BMP) | *.BMP; | All Files(*.*)|*.*||";

	CString strPathName;

	// 입력 영상 1 읽기
	CFileDialog dlg1(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK != dlg1.DoModal()) // 대화 상자 열기
	{
		return;
	}
	CByteImage imageIn1;
	strPathName = dlg1.GetPathName(); // 영상 파일 경로 획득
	if (!imageIn1.LoadImage(strPathName)) // 영상 읽기
	{
		return;
	}
	ShowImage(imageIn1, "입력 영상 1"); // 입력 영상1 의 화면 출력

	// 입력 영상 2 읽기
	CFileDialog dlg2(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK != dlg2.DoModal()) // 대화 상자 열기
	{
		return;
	}
	CByteImage imageIn2;
	strPathName = dlg2.GetPathName(); // 영상 파일 경로 획득
	if (!imageIn2.LoadImage(strPathName)) // 영상 읽기
	{
		return;
	}
	ShowImage(imageIn2, "입력 영상 2"); // 입력 영상1 의 화면 출력

	int nWidth = imageIn1.GetWidth();   // 영상의 너비
	int nHeight = imageIn1.GetHeight();  // 영상의 높이
	int nChannel = imageIn1.GetChannel(); // 영상의 채널 수

	CByteImage imageOut(nWidth, nHeight, nChannel); // 결과 영상 클래스 생성

	int r, c, h;
	for (r = 0; r < nHeight; r++) // 행 단위 이동
	{
		for (c = 0; c < nWidth; c++) // 열 단위 이동
		{
			for (h = 0; h < nChannel; h++) // 채널 단위 이동
			{
				// 두 입력 영상의 평균 값을 결과 영상에 저장
				//if (imageIn2.GetAt(c, r, h) == 0xff)
				//{
				//	imageOut.GetAt(c, r, h) = imageIn1.GetAt(c, r, h);
				//}
				//else
				//	imageOut.GetAt(c, r, h) = imageIn2.GetAt(c, r, h);
				imageOut.GetAt(c, r, h) = (imageIn2.GetAt(c, r, h) == 0xff) ? imageIn1.GetAt(c, r, h) : imageIn2.GetAt(c, r, h);
			}
		}
	}

	imageOut.SaveImage("blend.bmp");		// 결과 영상의 파일 저장
	ShowImage(imageOut, "합성 결과 영상");	// 결과 영상의 화면 출력
}


BOOL CMyImageEditorDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CloseAllImages();
	return CDialogEx::DestroyWindow();
}


