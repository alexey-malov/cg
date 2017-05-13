// ChildWindows.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

static const char MAIN_WND_CLASS_NAME[] = "My Super window class";
static const char MAIN_WND_NAME[] = "Child windows";

typedef enum
{
	ID_NONE = 0xffff,
	ID_PUSHME_BUTTON = 1,
	ID_DONTPUSHME_BUTTON,
	ID_YOURNAME_EDITBOX,	
} ChildId;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterMainWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
int MessageLoop();
HINSTANCE g_appInstance = NULL;

HWND CreateButton(HWND parent, const char* caption, UINT id, int x, int y, int width, int height);
HWND CreateStaticText(HWND parent, const char *text, UINT id, int x, int y, int width, int height);
HWND CreateEditBox(HWND parent, const char *text, UINT id, int x, int y, int width, int height);

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnDestroy(HWND hwnd);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     /*lpCmdLine*/,
                     int       nCmdShow)
{
	g_appInstance = hInstance;

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
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
		HANDLE_MSG(hWnd, WM_CREATE,			OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY,		OnDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND,		OnCommand);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/)
{
	switch(id)
	{
	case ID_PUSHME_BUTTON:
		MessageBox(hwnd, "Thank you!", "PushMe button pressed", MB_OK | MB_ICONINFORMATION);
		return;
	case ID_DONTPUSHME_BUTTON:
		MessageBox(hwnd, "I told you don't push me!!!", "Grrrrr!!!", MB_OK | MB_ICONSTOP);
		return;
	}

}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT /*lpCreateStruct*/)
{
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);

	CreateButton(hwnd, "Push Me", ID_PUSHME_BUTTON, rcClient.right - 75, rcClient.bottom - 30, 70, 25);
	CreateButton(hwnd, "Don't Push Me", ID_DONTPUSHME_BUTTON, 5, rcClient.bottom - 30, 120, 25);
	CreateButton(hwnd, "I don't care", ID_NONE, (rcClient.right - 120) / 2, rcClient.bottom - 30, 120, 25);

	CreateEditBox(hwnd, "...", ID_YOURNAME_EDITBOX, 130, 5, 200, 25);

	CreateStaticText(hwnd, "Enter your name: ", ID_NONE, 5, 5, 120, 25);

	return TRUE;	// successful creation
}

void OnDestroy(HWND /*hwnd*/)
{
	PostQuitMessage(0);
}

HWND CreateButton(HWND parent, const char* caption, UINT id, int x, int y, int width, int height)
{
	HWND hButtonWindow = CreateWindowEx(
		0, 
		"BUTTON", 
		caption, 
		WS_BORDER | WS_CHILDWINDOW | WS_VISIBLE, 
		x, y, 
		width, 
		height, 
		parent, 
		(HMENU)id, 
		g_appInstance, 
		NULL);

	return hButtonWindow;
}

HWND CreateEditBox(HWND parent, const char *text, UINT id, int x, int y, int width, int height)
{
	HWND hEditBox = CreateWindowEx(
		0, 
		"EDIT", 
		text, 
		WS_BORDER | WS_CHILDWINDOW | WS_VISIBLE, 
		x, y, 
		width, 
		height, 
		parent, 
		(HMENU)id, 
		g_appInstance, 
		NULL);

	return hEditBox;
}

HWND CreateStaticText(HWND parent, const char *text, UINT id, int x, int y, int width, int height)
{
	HWND hStaticText = CreateWindowEx(
		0,			// extended style
		"STATIC",	// static window class name
		text,		// text
		WS_CHILDWINDOW | WS_VISIBLE, 
		x, y,
		width, 
		height, 
		parent, 
		(HMENU)id, 
		g_appInstance, 
		NULL);

	return hStaticText;
}