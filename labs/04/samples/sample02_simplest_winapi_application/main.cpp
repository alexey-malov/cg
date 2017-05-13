#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <gl/GL.h>
#include <math.h>

const float M_PI = 3.14159265358979323846f;

TCHAR const CLASS_NAME[] = _T("MainWndClass");
TCHAR const WINDOW_TITLE[] = _T("My OpenGL Application");

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int MainLoop();
bool RegisterWndClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
bool InitOpenGL(HWND hwnd, bool needDepth = true, bool needStencil = false);
void DrawScene();

// �������� ���������� �������� ���� ���������� ��� OpenGL
HDC g_glDC = NULL;
// �������� ���������� OpenGL
HGLRC g_glRC = NULL;

HDC CreateDeviceContext(HWND hwnd, bool depthBuffer, bool stencilBuffer)
{
	HDC hdc = GetDC(hwnd);
	if (hdc == NULL)
	{
		return NULL;
	}

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
		depthBuffer ? 24 : 0;	// ���� �����, ���������� 24-������ ����� �������
	pfd.cStencilBits = 
		stencilBuffer ? 8 : 0;	// ���� �����, ���������� 8-������ ����� ���������

	// �������� �������� ���������� ������ ��������, 
	// �������������� ���������� ����������
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);

	// �������� ���������� ������ ������ �������� � ��������� ����������
	if (
		(pixelFormat == 0) ||
		!SetPixelFormat(hdc, pixelFormat, &pfd)
		)
	{
		// ���� �� ����������, �� ����������� �������� ���������� � �������
		ReleaseDC(hwnd, hdc);
		return NULL;
	}

	// ���� ��� ������, �� ���������� ����������� �������� ����������
	return hdc;
}

HGLRC CreateRenderContext(HDC hdc)
{
	// ������� �������� ���������� OpenGL
	HGLRC hrc = wglCreateContext(hdc);
	if (!hrc)
	{
		return NULL;
	}

	// �������� ������� ������ �������� ���������� �������
	// � ��������� ����������
	if (!wglMakeCurrent(hdc, hrc))
	{
		wglDeleteContext(hrc);
		return NULL;
	}

	// ���������� �������� ���������� � ������ ������
	return hrc;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT /*lpCreateStruct*/)
{
	// �������������� OpenGL � ���������� ���������
	return InitOpenGL(hwnd);
}

bool InitOpenGL(HWND hwnd, bool needDepth, bool needStencil)
{
	// ����������� �������� ���������� ��� ���������� ������ ���� 
	HDC hdc = CreateDeviceContext(hwnd, needDepth, needStencil);
	if (!hdc)
	{
		return false;
	}

	HGLRC hglRC = CreateRenderContext(hdc);
	if (!hglRC)
	{
		ReleaseDC(hwnd, hdc);
		return false;
	}

	// ��������� ���������� � ���������� ����������
	g_glDC = hdc;
	g_glRC = hglRC;

	return TRUE;
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
	DrawScene();
}

// ������ ������ �������
void DrawEllipse(float xCenter, float yCenter, float rx, float ry, int points = 360)
{
	const float step = 2 * M_PI / points;

	// ������ ����������� � ���� ��������� ������� �����, ����������� 
	// points ����� �� ��� ������� � ����� 2*PI/points
	glBegin(GL_LINE_LOOP);
	for (float angle = 0; angle < 2 * M_PI; angle += step)
	{
		const float dx = rx * cosf(angle);
		const float dy = ry * sinf(angle);
		glVertex2f(dx + xCenter, dy + yCenter);
	}
	glEnd();
}

void DrawScene()
{
	/*
	������ ���� ������� ������ ����� (������)
	*/
	glClearColor(1, 1, 0, 1);

	// ������� ����� ����� � ����� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ ������� ���� ������� �����
	glColor3f(0, 0, 1);

	// ������ ������ (��� ������� �����)
	DrawEllipse(300, 200, 150, 120);

	// ���������� ������� � ��������� ������ ����� �������
	SwapBuffers(g_glDC);
}

void OnSize(HWND /*hwnd*/, UINT /*state*/, int width, int height)
{
	// ����������� ���� ���������
	glViewport(0, 0, width, height);

	// ������ ������� ������� ������������� � ����������� �� ���������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);

	// ������ ������� �������� ������� �������������-����
	glMatrixMode(GL_MODELVIEW);
}

// ��������� �������� OpenGL � ����������� �������� ����������
void DestroyOpenGL(HWND hwnd)
{
	if (g_glRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(g_glRC);
		g_glRC = NULL;
	}

	if (g_glDC)
	{
		ReleaseDC(hwnd, g_glDC);
		g_glDC = NULL;
	}
}

void OnDestroy(HWND hWnd)
{
	// ��������� OpenGL
	DestroyOpenGL(hWnd);
	PostQuitMessage(0);
}

BOOL OnEraseBkgnd(HWND /*hwnd*/, HDC /*hdc*/)
{
	// �������� Windows, ��� �������� ������� ���� ���������
	return TRUE;
}

LRESULT CALLBACK WindowProc(
							HWND hwnd,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_SIZE, OnSize);
		HANDLE_MSG(hwnd, WM_ERASEBKGND, OnEraseBkgnd);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass =
	{
		sizeof(wndClass),	//UINT cbSize;
		CS_HREDRAW | CS_VREDRAW, //UINT style;
		&WindowProc,		//WNDPROC lpfnWndProc;
		0,					//int cbClsExtra;
		0,					//int cbWndExtra;
		hInstance,			//HINSTANCE hInstance;
		NULL,				//HICON hIcon;
		LoadCursor(NULL, IDC_ARROW), //HCURSOR hCursor;
		(HBRUSH)(COLOR_BTNFACE + 1), //HBRUSH hbrBackground;
		NULL,				//LPCTSTR lpszMenuName;
		CLASS_NAME,			//LPCTSTR lpszClassName;
		NULL,				//HICON hIconSm;
	};

	return RegisterClassEx(&wndClass) != FALSE;
}


// ������� ������� ���� ����������
HWND CreateMainWindow(HINSTANCE hInstance)
{
	HWND hMainWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW | 
			WS_CLIPCHILDREN |	// Windows-���������� OpenGL �������,
			WS_CLIPSIBLINGS,	// ����� ������ ����� ���� ���� �����������
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	return hMainWindow;
}

int MainLoop()
{
	MSG msg;
	BOOL res;
	while ((res = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (res == -1)
		{
			// ��������� ������ - ����� ���������� �� �, ��������,
			// ��������� ������ ����������
		}
		else
		{
			// ���� ��� ��������� � ������� ����������� �������,
			// �� ��������� � ������� ��������� ���������, ������� ���������� �
			// ���� ��������� ������������� �������
			TranslateMessage(&msg);
			// �������� ��������� � ��������������� ������� ���������
			DispatchMessage(&msg);
		}
	}

	// ���� �� ������� ������ � ��� ������ ���������� ��������� WM_QUIT
	// msg.wParam �������� ��� ��������, ���������� ��� ������ ������� PostQuitMessage()
	return msg.wParam;
}

int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE /*hPrevInstance*/,
				   LPSTR /*lpCmdLine*/,
				   int nCmdShow)
{
	// ������������ ����� �������� ����
	if (!RegisterWndClass(hInstance))
	{
		return 1;
	}

	// ������� ������� ���� ����������
	HWND hMainWindow = CreateMainWindow(hInstance);
	if (hMainWindow == NULL)
	{
		return 1;
	}

	// ���������� ������� ���� ����������
	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	// ��������� ���� ������� ���������, ���� �� ������� 
	// ������ � ���������� ����������
	return MainLoop();
}
