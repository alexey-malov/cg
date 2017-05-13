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

using namespace Gdiplus;

CSample05_simple_wtl_applicationView::CSample05_simple_wtl_applicationView()
// �������������� �������� � ������ ������, � ����� ��� �����
:m_font(L"Courier New", 25, FontStyleBoldItalic)
{
}

LRESULT CSample05_simple_wtl_applicationView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	// ������� Graphics ��� ��������� � ��������� ����������
	Graphics g(dc);

	// ������������� ����� ���������� ������������� ��� ��������� ������
	g.SetTextRenderingHint(TextRenderingHintAntiAlias);

	// ������� �����, ������� ����� ������������ �����
	SolidBrush brush(Color(255, 0, 0));

	// ������ ������ Hello ��������� ���� �������
	// -1 ��������, ��� ��������� ������ ������������� ������� ��������
	g.DrawString(L"Hello", -1, &m_font, PointF(30, 40), &brush);

	return 0;
}
