
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "sample04_simple_mfc_application.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// ������� ���� � ����� � �������������� ������� ���������� MFC
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CBrush brush(RGB(255, 255, 0));

	// �������� �� � �������� ����������
	CPen * pOldPen = dc.SelectObject(&pen);
	CBrush * pOldBrush = dc.SelectObject(&brush);

	// ������ ������
	dc.Ellipse(100, 50, 250, 150);

	// �������� ������� �������
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

	// ����������� CPen � CBrush ������������� ��������� ������� 
	// pen � brush ��� ������ �� ������ �������
}

