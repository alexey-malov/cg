// 01_hello_worldView.cpp : implementation of the CMy01_hello_worldView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "01_hello_worldView.h"

BOOL CMy01_hello_worldView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CMy01_hello_worldView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here
	CRect clientRect;
	GetClientRect(&clientRect);
	CString text = _T("Hello, world!");
	CSize textSize;
	dc.GetTextExtent(text, -1, &textSize);
	dc.TextOut((clientRect.Width() - textSize.cx) / 2, (clientRect.Height() - textSize.cy) / 2, text);

	return 0;
}
