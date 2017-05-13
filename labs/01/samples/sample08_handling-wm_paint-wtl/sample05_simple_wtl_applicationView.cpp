// sample05_simple_wtl_applicationView.cpp : implementation of the CSample05_simple_wtl_applicationView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "sample05_simple_wtl_applicationView.h"

BOOL CSample05_simple_wtl_applicationView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CSample05_simple_wtl_applicationView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	// ������� ���� � �����
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 0));

	// �������� ���� � �����
	// CPenHandle � CBrushHandle �� ��������� �������� ���������������� ������� GDI
	// � ������� �� CPen � CBrush
	CPenHandle oldPen = dc.SelectPen(pen);
	CBrushHandle oldBrush = dc.SelectBrush(brush);

	// ������ ������
	dc.Ellipse(100, 50, 250, 150);

	// ��������������� � ��������� ���������� ������� ����� � ����
	dc.SelectPen(oldPen);
	dc.SelectBrush(oldBrush);

	return 0;
}
