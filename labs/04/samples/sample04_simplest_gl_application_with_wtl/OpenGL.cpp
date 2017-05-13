#include "StdAfx.h"
#include "OpenGL.h"

COpenGL::COpenGL(HWND hwnd, bool needDepth, bool needStencil)
:m_dc(hwnd)
,m_rc(NULL)
,m_hwnd(hwnd)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));

	// ��������� ���� ��������� PIXELFORMATDESCRIPTOR,
	// �������� �������� ��������� ������ ����� OpenGL
	pfd.nSize = sizeof(pfd);		// �������������� ������ ���������
	pfd.nVersion = 1;				// Windows ������������ ������ ������ 1
	pfd.dwFlags =
		PFD_SUPPORT_OPENGL |		// ����� ������������ ��������� OpenGL
		PFD_DRAW_TO_WINDOW |		// ����� ������������ ��������� � ����
		PFD_DOUBLEBUFFER |			// ������������ ������� �����������
		PFD_GENERIC_ACCELERATED;	// � ���������� ���������
	pfd.iPixelType = PFD_TYPE_RGBA;	// ������ �������� - RGBA
	pfd.cColorBits = 32;			// 32 ���� �� �������� �����
	pfd.cDepthBits = 
		needDepth ? 24 : 0;	// ���� �����, ���������� 24-������ ����� �������
	pfd.cStencilBits = 
		needStencil ? 8 : 0;	// ���� �����, ���������� 8-������ ����� ���������

	int pixelFormat = m_dc.ChoosePixelFormat(&pfd);

	if (!pixelFormat)
	{
		Destroy();
		return;
	}

	if (!m_dc.SetPixelFormat(pixelFormat, &pfd))
	{
		Destroy();
		return;
	}

	m_rc = wglCreateContext(m_dc);
	if (m_rc)
	{
		if (!wglMakeCurrent(m_dc, m_rc))
		{
			Destroy();
		}
	}
}

COpenGL::~COpenGL(void)
{
	Destroy();
}

// ����������� �������� ���������� � �������, ��������� � OpenGL
void COpenGL::Destroy(void)
{
	if (m_rc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_rc);
		m_rc = NULL;
	}

	if (m_dc)
	{
		::ReleaseDC(m_hwnd, m_dc.Detach());
	}
}

// ��������� ����� �������� � �������� ������� �����
void COpenGL::SwapBuffers()
{
	if (m_dc && m_rc)
	{
		::SwapBuffers(m_dc);
	}
}
