// ImageFrameWnd.cpp : implementation file
//

#include "pch.h"
#include "ImageFrameWnd.h"

static int nFrameWndCount = 0;

// CImageFrameWnd

CImageFrameWnd::CImageFrameWnd()
{
}

CImageFrameWnd::CImageFrameWnd(const CByteImage &image, const char *name)
{
	m_view.SetImage(image);

	CString wndName;
	if (name)
		wndName = name;
	else
		wndName.Format("Image view %d", nFrameWndCount);

	CRect rect(30*nFrameWndCount, 30*nFrameWndCount, 
		image.GetWidth()+20+30*nFrameWndCount, image.GetHeight()+40+30*nFrameWndCount);
	Create(NULL, wndName, WS_OVERLAPPEDWINDOW, rect);

	nFrameWndCount++;

	ShowWindow(SW_SHOW);
}

CImageFrameWnd::~CImageFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CImageFrameWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImageFrameWnd message handlers


int CImageFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_view.CreateEx(this);

	return 0;
}


#include "ImageFrameWndManager.h"
extern CImageFrameWndManager gImageFrameWndManager;

void CImageFrameWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
//	gImageFrameWndManager.Delete(this);
//	CFrameWnd::PostNcDestroy();
}


void CImageFrameWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CFrameWnd::OnClose();
	gImageFrameWndManager.Delete(this);
}
