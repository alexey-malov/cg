// LoadMenu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

static const char MAIN_WND_CLASS_NAME[] = "My Super window class";
static const char MAIN_WND_NAME[] = "Menu example";

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterMainWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
HMENU CreateMainMenu(HINSTANCE hInstance);
int MessageLoop();

void OnDestroy(HWND /*hwnd*/);
void OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/);

HACCEL g_mainMenuAccelerators;

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

	g_mainMenuAccelerators = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATORS));
	
	
	ShowWindow(hMainWindow, nCmdShow);
	
	int result = MessageLoop();
	
	return result;
}

int MessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, g_mainMenuAccelerators, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
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
		CreateMainMenu(hInstance), 
		hInstance, 
		NULL);
}

HMENU CreateMainMenu(HINSTANCE hInstance)
{
	return LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAIN_MENU));
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
		HANDLE_MSG(hWnd, WM_COMMAND,		OnCommand);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/)
{
	switch(id)
	{
	case ID_FILE_EXIT:
		DestroyWindow(hwnd);
		break;
	case ID_HELP_ABOUT:
		MessageBox(hwnd, "Menu example. Created by Aleksey Malov aka Vivid.", "About", MB_OK);
		break;
	}
}

void OnDestroy(HWND /*hwnd*/)
{
	PostQuitMessage(0);
}
