// ClickMe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

static const char MAIN_WND_CLASS_NAME[] = "My Super window class";
static const char MAIN_WND_NAME[] = "Click me or Double click me, please!!!";

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterMainWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
int MessageLoop();

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
	wc.style = CS_DBLCLKS;	// ! Our window will be able to accept double clicks
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
	case WM_LBUTTONDOWN:
		SetWindowText(hWnd, "Left mouse button pressed");
		return 0;	//	If an application processes this message, it should return zero.
	case WM_LBUTTONUP:
		SetWindowText(hWnd, "Left mouse button released");
		MessageBeep(-1);
		return 0;	//	If an application processes this message, it should return zero.
	case WM_LBUTTONDBLCLK:	// to receive this message our window must have CS_DBLCLKS style
		SetWindowText(hWnd, "Left mouse double click");
		MessageBox(hWnd, "Don't make me hurt!!!", "Ouch!", MB_OK | MB_ICONEXCLAMATION);
		return 0;	//	If an application processes this message, it should return zero.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
