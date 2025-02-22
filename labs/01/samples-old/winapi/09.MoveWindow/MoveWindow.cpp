// MoveWindow.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

static const char MAIN_WND_CLASS_NAME[] = "My Super window class";
static const char MAIN_WND_NAME[] = "This is a timer example";

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterMainWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
int MessageLoop(HWND hMainWindow);

void OnDestroy(HWND /*hwnd*/);

DWORD	g_lastTickTime = 0;

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
	
	int result = MessageLoop(hMainWindow);
	
	return result;
}

int MessageLoop(HWND hMainWindow)
{
	MSG msg;

	float xPhase = 0;
	float yPhase = 0;

	/*
		���������������� ����� ������� ���������.
		������������ ������� PeekMessage(), �������, � ������� �� ������� GetMessage(), �� �������
		����� �������� ��������� � �������, ��� ��������� ��� ��������� ������� ������, � ������
		��������� ��������� ����. ����� ���� �� ������ ��� ������ � ��� ������ �������, ��
		�������� ����������������� �������������� ������, ��� ����� ��� ����� ����������� ���������
		��������� ��������� � ����, � �� �� �������. ����� ����, ���������� �� ������� ����� ��������� �
		�����������.

	*/
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)	
			{
				break;
			}
		}
		else
		{
			// �������������� �������� ���� �� ���������

			DWORD currentTick = GetTickCount();
			float timeDelta = (currentTick - g_lastTickTime) * 0.001f;
			g_lastTickTime = currentTick;

			static const float XSPEED = 10;
			static const float YSPEED = 15;

			xPhase += timeDelta * XSPEED;
			yPhase += timeDelta * YSPEED;

			xPhase = fmodf(xPhase, 360.0f);
			yPhase = fmodf(yPhase, 360.0f);

			// get dimensions of the screen
			int const SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
			int const SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
			int const SCREEN_CENTER_X = SCREEN_WIDTH / 2;
			int const SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

			RECT rcWindow;
			GetWindowRect(hMainWindow, &rcWindow);

			int WINDOW_WIDTH = rcWindow.right - rcWindow.left;
			int WINDOW_HEIGHT = rcWindow.bottom - rcWindow.top;

			const float XAMPLITUDE = (SCREEN_WIDTH - WINDOW_WIDTH) / 2.0f;
			const float YAMPLITUDE = (SCREEN_HEIGHT - WINDOW_HEIGHT) / 2.0f;

			static const float M_PI = 3.14159265f;

			int x = (int)(SCREEN_CENTER_X + (XAMPLITUDE * sinf(M_PI * xPhase / 180.f)) - (WINDOW_WIDTH / 2) );
			int y = (int)(SCREEN_CENTER_Y + (YAMPLITUDE * sinf(M_PI * yPhase / 180.f)) - (WINDOW_HEIGHT / 2) );

			MoveWindow(hMainWindow, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);

			// "��������" ���� ���������� �� 10��, ����� ���� ����������� ���������� ������ ����������� � �������
			Sleep(10);	
		}
	}
	return msg.wParam;
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	g_lastTickTime = GetTickCount();

	return CreateWindowEx(
		0, 
		MAIN_WND_CLASS_NAME, 
		MAIN_WND_NAME, 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		200, 
		200, 
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
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

