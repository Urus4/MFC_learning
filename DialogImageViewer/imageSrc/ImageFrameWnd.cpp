// ImageFrameWnd.cpp : implementation file
//

#include "pch.h"
#include "ImageFrameWnd.h"
#include "ImageView.h"

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
		wndName.Format("Image view %d", nFrameWndCount); // 넘어온 윈도우 이름이 없으면 다음과 같이 이름을 설정해라

	CRect rect(30*nFrameWndCount, 30*nFrameWndCount, 
		image.GetWidth()+30*nFrameWndCount, image.GetHeight()+30*nFrameWndCount); // +30을 해주는 이유 -> 새창을 띄울때 그댈 띄워버리면 새창과 이전 창이 완전 겹쳐버리기 때문에
	Create(NULL, wndName, WS_OVERLAPPEDWINDOW, rect);

	nFrameWndCount++;	// 이름값을 받지 않은 창을 여러개 만들 수 있으니

	ShowWindow(SW_SHOW);
}

CImageFrameWnd::~CImageFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CImageFrameWnd, CFrameWnd)
	ON_WM_CREATE()
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
	gImageFrameWndManager.Delete(this);
//	CFrameWnd::PostNcDestroy();
}
