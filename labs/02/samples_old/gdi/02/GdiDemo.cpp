// GdiDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"


// ���������� ����������
HINSTANCE	g_hInstance;							// current instance
const TCHAR g_title[] = "Gdi demo";					// ����� � ��������� ����
const TCHAR g_windowClass[] = "GdiDemoClass";		// ��� ������ ����
const TCHAR g_helloWorld[] = "������ �� �������� � �� ����������!";		// ������, ��� ������ �� ����������� ����
HFONT		g_helloWorldFont = NULL;

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
	g_helloWorldFont = CreateFont(
		50,					// ������ ������
		0,					// ������� ������ ������
		0,					// ���� ������� ������������ ��� ������ � ������� ����� �������
		0,					// ���� ������� �������������� ��� ������
		FW_BOLD,			// font weight
		FALSE,				// ������
		FALSE,				// �������������
		FALSE,				// ��������������
		DEFAULT_CHARSET,	// ����� ��������
		OUT_DEFAULT_PRECIS,	// �������� ������
		CLIP_DEFAULT_PRECIS,// �������� ��������� ��������, ��������� �� ������� ����������
		DEFAULT_QUALITY,    // �������� ������
		FF_DONTCARE | DEFAULT_PITCH,	// ���������� ����� ��������� (Pitch) � ��������� ������� (FontFamily)
		"Comic Sans MS"		// ��� ������
		);
}

void FreeGraphicResources()
{
	DeleteObject(g_helloWorldFont);
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
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

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
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

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
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

void OnPaint(HDC hdc)
{
	// ��������� ���������� ���� ��������� ���������� � �������� ��������� ����
	HGDIOBJ oldFont = SelectObject(hdc, g_helloWorldFont);

	SetTextColor(hdc, RGB(70, 120, 35));
	// ������� ����� �� ����� ����� ������ � �������
	TextOut(hdc, 0, 0, g_helloWorld, strlen(g_helloWorld));

	// ��������������� ���������� ���� ��������� ����������
	SelectObject(hdc, oldFont);
}
