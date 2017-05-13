// 01_raytrace_baseView.cpp : implementation of the CRaytraceView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "FrameBuffer.h"

#include "RaytraceView.h"

CRaytraceView::CRaytraceView()
:m_pFrameBuffer(new CFrameBuffer(800, 600))
{
}

CRaytraceView::~CRaytraceView()
{
	// ���������� ���������� ������� ������ ������� CRenderer �� ����������
	// ������ ������ CRaytraceView, �.�. CRenderer ���������� ��� ����� ������ 
	// ������ �������, � ���������, ����� �����, ��������� ������� ����� ������
	// ����� ��������� CRenderer
	m_renderer.Stop();
}

BOOL CRaytraceView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CRaytraceView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// �������� �������, ��� ���������� ������� ������ �� ���������
	return 1;
}

LRESULT CRaytraceView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	// �������� ������� ���������� ������� ����
	CRect clientRect;
	GetClientRect(clientRect);
	int clientWidth = clientRect.Width();
	int clientHeight = clientRect.Height();

	// �����, ������������ ��� ������� ����
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(0xffffff);

	// �������� �� ������ ���������� ������ �����
	if (m_pFrameBuffer.get())
	{
		int w = m_pFrameBuffer->GetWidth();
		int h = m_pFrameBuffer->GetHeight();
		// ������ ����� ����� � ����� ������� ���� 
		// ���������� ������� ����
		DrawFrameBuffer(dc, 0, 0);

		// �.�. �� ��������� ������� ������� ���� ������,
		// ������� ������ � ����� �� ����������� �������� 
		// �������� �������

		// ������� ������ ����� ���������� �������
		if (w < clientWidth)
		{
			dc.FillRect(CRect(w, 0, clientWidth, h), whiteBrush);
		}

		// ������� ������ ����� ���������� �������
		if (h < clientHeight)
		{
			dc.FillRect(CRect(0, h, clientWidth, clientHeight), whiteBrush);
		}
	}
	else	// ����� ����� �� �����-�� �������� �� ���������
	{
		ATLASSERT(!"Something bad with the frame buffer");
	}

	return 0;
}

void CRaytraceView::DrawFrameBuffer(CDC & dc, int x, int y)
{
	int w = m_pFrameBuffer->GetWidth();
	int h = m_pFrameBuffer->GetHeight();

	// ��������� ��������� BITMAPINFO (��. MSDN), �����
	// ���������� ���������� ������ ����� �� ����� ��� �������� �������������� bitmap-��
	BITMAPINFO bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	BITMAPINFOHEADER & hdr = bmpInfo.bmiHeader;
	hdr.biSize = sizeof(hdr);
	hdr.biWidth = w;
	// �� ��������� BMP ���� �������� "����� ������" (������� ������ ��� ��������)
	// ����� ������������ ��������� ������� �������� �������� ������ ������ ���� �������������
	hdr.biHeight = -h;
	hdr.biPlanes = 1;						// ���������� �������� ���������� � �����������
	hdr.biBitCount = sizeof(boost::uint32_t) << 3;	// ���� ������� ������� �������� 32 ����
	hdr.biCompression = BI_RGB;
	hdr.biSizeImage = w * h * static_cast<int>(sizeof(boost::uint32_t));

	// ��������� ������� SetDIBitsToDevice (��. MSDN) ������������ ������ ��������
	// �� ������ � �������� ����������
	dc.SetDIBitsToDevice(
		x, y, // ���������� ������ ������ ��������� ����������
		w, h, // ������ � ������ �����������
		0, 0, // ���������� �������� ������� �����������
		0, h, // ����� ��������� ������ � ���������� �����
		m_pFrameBuffer->GetPixels(),	// ����� ��������
		&bmpInfo, // ����� ���������� � ��������
		DIB_RGB_COLORS);// ������������� � ���, ��� �������� � ������� 
						// bmpInfo.bmiColors ���������������� ��� RGB ��������,
						// � �� ������� ���������� �������
}

LRESULT CRaytraceView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// ��������� ������� ������������
	m_renderer.Render(*m_pFrameBuffer);

	// �������������� ������ ��� ����������� ������������ �����������
	SetTimer(FRAMEBUFFER_UPDATE_TIMER, 500);
	return 0;
}

bool CRaytraceView::UpdateFrameBuffer()
{
	// ���������� ����������� ����
	RedrawWindow();

	unsigned totalChunks = 0;
	unsigned renderedChunks = 0;

	// ���������� true, ���� ����������� � ������ ����� ��������� ���������
	return m_renderer.GetProgress(renderedChunks, totalChunks);
}

LRESULT CRaytraceView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	WPARAM timerId = wParam;
	switch (timerId)
	{
	case FRAMEBUFFER_UPDATE_TIMER:
		// ���� ����������, ��� ����������� ���������, ��������� ������,
		// �������������� ����� ����� � ����
		if (UpdateFrameBuffer())
		{
			KillTimer(FRAMEBUFFER_UPDATE_TIMER);
		}
		break;
	}

	return 0;
}
