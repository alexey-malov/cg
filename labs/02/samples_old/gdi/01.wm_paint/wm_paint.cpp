// wm_paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

static const char MAIN_WND_CLASS_NAME[] = "My Super window class";
static const char MAIN_WND_NAME[] = "Illustration of WM_PAINT and WM_ERASEBKGND message";

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterMainWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
int MessageLoop();

void OnDestroy(HWND hwnd);
void OnPaint(HWND hwnd);
BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     /*lpCmdLine*/,
                     int       nCmdShow)
{
	if (!RegisterMainWindowClass(hInstance))
	{
		return -1;
	}
	
	HWND hMainWindow = CreateMainWindow(hInstance);
	if (!hMainWindow)
	{
		return -1;
	}
	
	ShowWindow(hMainWindow, nCmdShow);
	
	int result = MessageLoop();
	
	return result;
}

int MessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	return CreateWindowEx(
		0, 
		MAIN_WND_CLASS_NAME, 
		MAIN_WND_NAME, 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		NULL, 
		hInstance, 
		NULL);
}

BOOL RegisterMainWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = &MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAIN_WND_CLASS_NAME;
	wc.hIconSm = NULL;
	
	return RegisterClassEx(&wc) != 0;
	
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_MSG(hWnd, WM_DESTROY,		OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT,			OnPaint);
		HANDLE_MSG(hWnd, WM_ERASEBKGND,		OnEraseBkgnd);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	HBRUSH hatchBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(100, 190, 30));

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	FillRect(hdc, &clientRect, hatchBrush);

	DeleteBrush(hatchBrush);
	return TRUE;
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	// get Paint DC
	HDC dc = BeginPaint(hwnd, &ps);

	RECT rc;
	rc.left = 10;
	rc.top = 50;
	rc.right = rc.left + 100;
	rc.bottom = rc.top + 75;

	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	FillRect(dc, &rc, redBrush);
	DeleteBrush(redBrush);

	EndPaint(hwnd, &ps);
}


void OnDestroy(HWND /*hwnd*/)
{
	PostQuitMessage(0);
}
