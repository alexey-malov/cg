// GdiDemo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"


// ���������� ����������
HINSTANCE	g_hInstance;							// current instance
const TCHAR g_title[] = "Gdi demo";					// ����� � ��������� ����
const TCHAR g_windowClass[] = "GdiDemoClass";		// ��� ������ ����
const TCHAR g_helloWorld[] = "������ � ���������� �����������!";		// ������, ��� ������ �� ����������� ����

// ����������� �������
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

void DrawHuman(HDC hdc)
{
	// ������� ����������� ������ "����" �������� ����� ������� 5 ��������, �������� �������� �����
	HPEN contourPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));

	HGDIOBJ oldPen = SelectObject(hdc, contourPen);

	// ������ ������
	int headX = 100;
	int headY = 100;
	int headRadius = 25;	// ������ ������
	Ellipse(
		hdc, 
		headX - headRadius, headY - headRadius,
		headX + headRadius, headY + headRadius);

	// ������ ��������
	int bodyX = headX;
	int bodyY = headY + headRadius;
	int bodyHeight = 100;
	MoveToEx(hdc, bodyX, bodyY, NULL);
	LineTo(hdc, bodyX, bodyY + bodyHeight);

	// ������ ����
	int legsTop = bodyY + bodyHeight;
	int legsX = bodyX;
	int legsHeight = 150;
	int legsStep = 40;
	MoveToEx(hdc, legsX, legsTop, NULL);
	LineTo(hdc, legsX - legsStep, legsTop + legsHeight);
	MoveToEx(hdc, legsX, legsTop, NULL);
	LineTo(hdc, legsX + legsStep, legsTop + legsHeight);

	// ������ ����
	int handsX = bodyX;
	int handsY = bodyY;
	int handsLength = 70;
	int handsDown = 100;
	MoveToEx(hdc, handsX, handsY, NULL);
	LineTo(hdc, handsX - handsLength, handsY + handsDown);
	MoveToEx(hdc, handsX, handsY, NULL);
	LineTo(hdc, handsX + handsLength, handsY + handsDown);

	HPEN facePen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	SelectObject(hdc, facePen);

	// ������ �����
	int eyesDx = 10;
	int eyesY = headY - 7;
	Ellipse(hdc, 
		headX - eyesDx - 1, eyesY - 1, 
		headX - eyesDx + 1, eyesY +1 );
	Ellipse(hdc, 
		headX + eyesDx - 1, eyesY - 1, 
		headX + eyesDx + 1, eyesY +1 );



	SelectObject(hdc, oldPen);

	// ������� ��������� �������
	DeleteObject(facePen);
	DeleteObject(contourPen);	
}

void OnPaint(HDC hdc)
{
	// ��������� ���������� ���� ��������� ���������� � �������� ��������� ����
	HGDIOBJ oldFont = SelectObject(hdc, g_helloWorldFont);


	SetTextColor(hdc, RGB(70, 120, 35));
	// ������� ����� �� ����� ����� ������ � �������
	TextOut(hdc, 0, 0, g_helloWorld, strlen(g_helloWorld));

	DrawHuman(hdc);

	// ��������������� ���������� ���� ��������� ����������
	SelectObject(hdc, oldFont);
}

