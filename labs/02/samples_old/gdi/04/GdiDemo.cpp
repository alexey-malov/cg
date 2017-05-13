// GdiDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"


// ���������� ����������
HINSTANCE	g_hInstance;							// current instance
const TCHAR g_title[] = "Gdi demo";					// ����� � ��������� ����
const TCHAR g_windowClass[] = "GdiDemoClass";		// ��� ������ ����
const TCHAR g_helloWorld[] = "������ �� �������� � �� ����������!";		// ������, ��� ������ �� ����������� ����

// ����������� �������
HBITMAP		g_dogBitmap = NULL;
HBITMAP		g_catBitmap = NULL;

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

void InitGraphicResources(HWND hWnd)
{
	// ��������� ������ � ��������
	g_dogBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_DOG));
	g_catBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_CAT));
}

void FreeGraphicResources()
{
	DeleteObject(g_dogBitmap);
	DeleteObject(g_catBitmap);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) 
	{
	// ��������� WM_CREATE ��������, ����� ���� ������-������ �������
	case WM_CREATE:
		// ������� �������, ����������� ��� ��� ������ � ��������
		InitGraphicResources(hWnd);
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
	case WM_DESTROY:
		// ����������� ��� ������� �������, ��������� ���� �� ����� ������
		FreeGraphicResources();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void DrawDog(HDC hdc)
{
	// ������� ��������, ����������� � ���������� ����, �� ��������������� � ������
	HDC dcBitmap = CreateCompatibleDC(hdc);
	// � �������� � ���� �������� � ��������
	HGDIOBJ oldBitmap = SelectObject(dcBitmap, g_dogBitmap);

	// �������� ��������� �������� � ��������
	BITMAP bmp;
	GetObject(g_dogBitmap, sizeof(bmp), &bmp);

	int sourceX = 0;
	int sourceY = 0;
	int destinationX = 0;
	int destinationY = 0;
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

void DrawCat(HDC hdc)
{
	// ������� ��������, ����������� � ���������� ����, �� ��������������� � ������
	HDC dcBitmap = CreateCompatibleDC(hdc);
	// � �������� � ���� �������� � ��������
	HGDIOBJ oldBitmap = SelectObject(dcBitmap, g_catBitmap);
	
	// �������� ��������� �������� � ������
	BITMAP bmp;
	GetObject(g_catBitmap, sizeof(bmp), &bmp);
	
	int sourceX = 0;
	int sourceY = 0;
	int destinationX = 360;
	int destinationY = 250;
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

void OnPaint(HDC hdc)
{
	DrawDog(hdc);
	DrawCat(hdc);
}

