// GdiDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"


// ���������� ����������
HINSTANCE	g_hInstance;							// current instance
const TCHAR g_title[] = "Gdi demo";					// ����� � ��������� ����
const TCHAR g_windowClass[] = "GdiDemoClass";		// ��� ������ ����
const TCHAR g_helloWorld[] = "������ �� �������� � �� ����������!";		// ������, ��� ������ �� ����������� ����
const UINT DRAW_TIMER_ID = 1;
UINT		g_drawTimerId = 0;		// ������������� �������, ������� ����� �������� ����������� ������ ����

// ����������� �������
HBITMAP	g_dogBitmap = NULL;
HBITMAP	g_catBitmap = NULL;
HBITMAP	g_catMask = NULL;
HBITMAP	g_backBuffer = NULL;	// Bitmap ��� ������������ ������
HDC		g_backBufferDC = NULL;	// �������� ������������ ������
HGDIOBJ g_oldBackBufferBitmap = NULL;
HBITMAP g_snailBitmap;	// ����������� ������
HBITMAP g_snailMask;	// ����� ������

int g_snailX = 10;		// x - ���������� ������� ������
int g_snailY = 180;		// y - ���������� ������� ������

int g_screenWidth;		// ������ ������������ �������
int g_screenHeight;		// ������ ������������ �������

// ������������� ����������, ����������� ��� ����������� �����������������
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
int					MessageLoop();

// ������������ / ��������������� �������� �������
void				InitGraphicResources(HWND hWnd);
void				FreeGraphicResources();

// ����������� ���������
void				OnPaint(HDC hdc);
void				OnTimer(HWND hWnd, int idTimer);

// ����� ����� � ����������
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	// ������������ ����� ����
	MyRegisterClass(hInstance);

	// �������������� ����������
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}


	int exitCode = MessageLoop();


	return exitCode;
}


// ���� ��������� ��������� ���������
int MessageLoop()
{
	MSG msg;
	// �������� ��������� �� ������� �� ��� ���, ���� ���������� �� �������� ���� ������ - �.�. �� ������� PostQuitMessage
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;	
}

void InitBackBuffer()
{
	// ����������� � ������� ������� ������
	g_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	g_screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// �������� DC ������
	HDC screenDC = GetDC(NULL);
	
	// ������� ����������� � ���������� ������ Bitmap �������� � �����
	g_backBuffer = CreateCompatibleBitmap(
		screenDC,
		g_screenWidth,
		g_screenHeight);
	
	// ������� �������� � ������, ����������� � ���������� ������
	g_backBufferDC = CreateCompatibleDC(screenDC);
	
	// � �������� � ���� ��� Bitmap ������ ������������ ������
	g_oldBackBufferBitmap = SelectObject(g_backBufferDC, g_backBuffer);
	
	// �������� ������ ��� ������ �� �����, ������� �� ��� �����������
	ReleaseDC(NULL, screenDC);
}

void DeleteBackBuffer()
{
	SelectObject(g_backBufferDC, g_oldBackBufferBitmap);
	DeleteObject(g_backBuffer);
	DeleteDC(g_backBufferDC);
}

void InitGraphicResources(HWND hWnd)
{
	// ��������� ������ � ��������
	g_dogBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_DOG));

	g_catBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_CAT));
	g_catMask = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_CAT_MASK));

	g_snailBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_SNAIL));
	g_snailMask = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_SNAIL_MASK));
	
	InitBackBuffer();
}



void FreeGraphicResources()
{
	DeleteObject(g_dogBitmap);

	DeleteObject(g_catBitmap);
	DeleteObject(g_catMask);

	DeleteObject(g_snailBitmap);
	DeleteObject(g_snailMask);

	DeleteBackBuffer();
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	// �������������� ���������, �������� ����� ������ ����
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;	
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;		
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GDIDEMO);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_GDIDEMO;
	wcex.lpszClassName	= g_windowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	// ������������ ����� ������ ����
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   g_hInstance = hInstance; // Store instance handle in our global variable

   // ������� ����
   hWnd = CreateWindow(g_windowClass, g_title, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // ���������� ����
   ShowWindow(hWnd, nCmdShow);

   // ��������� ����, ������� ��� �����������
   UpdateWindow(hWnd);

   return TRUE;
}

void InitDrawTimer(HWND hWnd)
{
	// �������������� ������ ��� ����
	g_drawTimerId = SetTimer(
			hWnd,				// ����� ����, ������� ����� �������� ��������� �� ���������������� �������
			DRAW_TIMER_ID,		// ��������� ������������� �������
			50,				// ������ (� �������������) ����� ��������� �������
			NULL);				// ����� �������-����������� ������� (��� ���� �������� �� �����)
}

void DeleteDrawTimer(HWND hWnd)
{
	// ��������� ������ (��� �� ������ �� �����)
	KillTimer(hWnd, g_drawTimerId);
	g_drawTimerId = 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) 
	{
	// ��������� WM_CREATE ��������, ����� ���� ������-������ �������
	case WM_CREATE:
		// ������� �������, ����������� ��� ��� ������ � ��������
		InitGraphicResources(hWnd);
		InitDrawTimer(hWnd);
		break;
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			
			// � ����������� WM_PAINT ��� ��������� ������������ ��������� ���������� �����������
			// ������� BeginPaint() ����� ��������� � EndPaint() � ����� ���������
			hdc = BeginPaint(hWnd, &ps);

			OnPaint(hdc);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_ERASEBKGND:
		return 1;	// ������������� ������� � ���, ��� �� �������� ��� ����
	case WM_TIMER:
		{
			int idTimer = wParam;
			OnTimer(hWnd, idTimer);
		}
		break;
	case WM_DESTROY:
		// ����������� ��� ������� �������, ��������� ���� �� ����� ������
		DeleteDrawTimer(hWnd);
		FreeGraphicResources();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void DrawPicture(HDC hdc, HBITMAP picture, int x, int y)
{
	// ������� ��������, ����������� � ���������� ����, �� ��������������� � ������
	HDC dcBitmap = CreateCompatibleDC(hdc);
	// � �������� � ���� �������� � ��������
	HGDIOBJ oldBitmap = SelectObject(dcBitmap, picture);
	
	// �������� ��������� �������� � ��������
	BITMAP bmp;
	GetObject(picture, sizeof(bmp), &bmp);
	
	int sourceX = 0;
	int sourceY = 0;
	int destinationX = x;
	int destinationY = y;
	int destinationWidth = bmp.bmWidth;
	int destinationHeight = bmp.bmHeight;
	
	// ��������� ����������� �������� (Bit block transfer)
	BitBlt(
		hdc,				// ����� DC, �� ������� �� ����� ���������� �������
		destinationX,		// ���������� X �������� ������ ���� �� ������� ������
		destinationY,		// ���������� Y �������� ������ ���� �� ������� ������
		destinationWidth,	// ������ ��������� �������
		destinationHeight,	// ������ ��������� �������
		dcBitmap,			// ����� DC, �� �������� �� ����� ���������� �������
		sourceX,			// ���������� X �� �������-��������� ��������
		sourceY,			// ���������� Y �� �������-��������� ��������
		SRCCOPY);			// ��� ��������� �������� (����������� � ���������)
	
	SelectObject(dcBitmap, oldBitmap);
	DeleteDC(dcBitmap);
}

void DrawSpriteWithMask(HDC hdc, HBITMAP sprite, HBITMAP mask, int x, int y)
{
	// ������� ��������, ����������� � ���������� ����, �� ��������������� � ������
	HDC dcBitmap = CreateCompatibleDC(hdc);
	// � �������� � ���� �������� � ��������
	HGDIOBJ oldBitmap = SelectObject(dcBitmap, sprite);
	
	// �������� ��������� �������� � ������
	BITMAP bmp;
	GetObject(sprite, sizeof(bmp), &bmp);
	
	int sourceX = 0;
	int sourceY = 0;
	int destinationX = x;
	int destinationY = y;
	int destinationWidth = bmp.bmWidth;
	int destinationHeight = bmp.bmHeight;
	int maskX = 0;
	int maskY = 0;
	
	// ��������� ����������� �������� (Bit block transfer)
	MaskBlt(
		hdc,				// ����� DC, �� ������� �� ����� ���������� �������
		destinationX,		// ���������� X �������� ������ ���� �� ������� ������
		destinationY,		// ���������� Y �������� ������ ���� �� ������� ������
		destinationWidth,	// ������ ��������� �������
		destinationHeight,	// ������ ��������� �������
		dcBitmap,			// ����� DC, �� �������� �� ����� ���������� �������
		sourceX,			// ���������� X �� �������-��������� ��������
		sourceY,			// ���������� Y �� �������-��������� ��������
		mask,				// ����������� ������ �����
		maskX,				// ���������� X ������ �������� ���� �����
		maskY,				// ���������� Y ������ �������� ���� �����
		MAKEROP4(SRCCOPY, SRCAND));	// ���� ��������� �������� ��� ��������� � ������� ����.
	
	SelectObject(dcBitmap, oldBitmap);
	DeleteDC(dcBitmap);

}


void ClearBackBuffer()
{
	// ������ ������ ������������� �������� � ������� �����
	HBRUSH fillBrush = CreateSolidBrush(RGB(160, 20, 255));
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = g_screenWidth;
	rc.bottom = g_screenHeight;

	FillRect(g_backBufferDC, &rc, fillBrush);
	DeleteObject(fillBrush);
}

void ShowBackBuffer(HDC hdc)
{
	// �������� ���������� ��������� ������������ ������ �� �������� ����
	BitBlt(
		hdc,			// ����� DC, �� ������� �� ����� ���������� �������
		0,				// ���������� X �������� ������ ���� �� ������� ������
		0,				// ���������� Y �������� ������ ���� �� ������� ������
		g_screenWidth,	// ������ ��������� �������
		g_screenHeight,	// ������ ��������� �������
		g_backBufferDC,	// ����� DC, �� �������� �� ����� ���������� �������
		0,				// ���������� X �� �������-��������� ��������
		0,				// ���������� Y �� �������-��������� ��������
		SRCCOPY);		// ��� ��������� �������� (����������� � ���������)

}

void DrawDog(HDC hdc)
{
	DrawPicture(hdc, g_dogBitmap, 0, 0);
}

void DrawCat(HDC hdc)
{
	DrawSpriteWithMask(hdc, g_catBitmap, g_catMask, 200, 170);
}

void DrawSnail(HDC hdc)
{
	DrawSpriteWithMask(hdc, g_snailBitmap, g_snailMask, g_snailX, g_snailY);
}


void OnPaint(HDC hdc)
{
	// ������� ����������� �����
	ClearBackBuffer();

	// ������������ �������� �� ����������� �����
	DrawDog(g_backBufferDC);
	DrawSnail(g_backBufferDC);
	DrawCat(g_backBufferDC);

	// �������� ����������� ����� �� �������� ����
	ShowBackBuffer(hdc);
}

void OnTimer(HWND hWnd, int idTimer)
{
	// �������� ������� � ������������� ����������� ������ ����
	RECT rc;
	GetClientRect(hWnd, &rc);
	RedrawWindow(hWnd, &rc, NULL, RDW_INVALIDATE);

	// ������ ���������� ������
	if (g_snailX < 500)
	{
		g_snailX += 1;
	}
}
