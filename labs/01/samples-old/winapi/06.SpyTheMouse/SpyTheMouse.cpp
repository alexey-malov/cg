// SpyTheMouse.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

static const char MAIN_WND_CLASS_NAME[] = "My Super window class";
static const char MAIN_WND_NAME[] = "Press left mouse button to spy the mouse";

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterMainWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
int MessageLoop();

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
void OnDestroy(HWND hwnd);
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);


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
	/*
		These HANDLE_MSG macros are so-called 'Message crackers' declared in WindowsX.h header file
		They make message handline simpler
	*/
	switch(uMsg)
	{
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN,	OnLButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP,		OnLButtonUp);
		HANDLE_MSG(hWnd, WM_DESTROY,		OnDestroy);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE,		OnMouseMove);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SpyTheMouse(HWND hwnd, const char * format, int x, int y)
{
	// I am REALLY sure this buffer is long enough to store this string
	char buffer[255];
	
	sprintf(buffer, format, x, y);
	SetWindowText(hwnd, buffer);

}

void OnLButtonUp(HWND hwnd, int x, int y, UINT /*keyFlags*/)
{
	SpyTheMouse(hwnd, "Spying finished at x: %d, y: %d", x, y);

	ReleaseCapture();
}

void OnLButtonDown(HWND hwnd, BOOL /*fDoubleClick*/, int x, int y, UINT /*keyFlags*/)
{
	SpyTheMouse(hwnd, "Spying started at x: %d, y: %d", x, y);

	// now our window will capture mouse movements even when mouse is not over our window
	SetCapture(hwnd);	
}

void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	if ((keyFlags & MK_LBUTTON) != 0)
	{
		SpyTheMouse(hwnd, "Mouse moved to x: %d, y: %d", x, y);
	}
}

void OnDestroy(HWND /*hwnd*/)
{
	PostQuitMessage(0);
}
