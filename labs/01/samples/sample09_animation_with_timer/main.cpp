#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <math.h>

TCHAR const CLASS_NAME[] = _T("MainWndClass");
TCHAR const WINDOW_TITLE[] = _T("Animation using timer");



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int MainLoop();
bool RegisterWndClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);

enum
{
	ANIMATION_TIMER_ID = 1
};

DWORD g_lastTick;
UINT_PTR g_timerId = 0;

// ������� ���� �������� (� ��������)
double g_rotationAngle = 0;
// �������� �������� ������� (��������/���)
const double ROTATION_SPEED = 30;
const double M_PI = 3.14159265;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int nCmdShow)
{
	// ������������ ����� �������� ����
	if (!RegisterWndClass(hInstance))
	{
		return 1;
	}

	// ������� ������� ���� ����������
	HWND hMainWindow = CreateMainWindow(hInstance);
	if (hMainWindow == NULL)
	{
		return 1;
	}
	
	// ���������� ������� ���� ����������
	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	// ��������� ���� ������� ���������, ���� �� ������� 
	// ������ � ���������� ����������
	return MainLoop();
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	HWND hMainWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	return hMainWindow;
}

int MainLoop()
{
	MSG msg;
	BOOL res;
	while ((res = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (res == -1)
		{
			// ��������� ������ - ����� ���������� �� �, ��������,
			// ��������� ������ ����������
		}
		else
		{
			// ���� ��� ��������� � ������� ����������� �������,
			// �� ��������� � ������� ��������� ���������, ������� ���������� �
			// ���� ��������� ������������� �������
			TranslateMessage(&msg);
			// �������� ��������� � ��������������� ������� ���������
			DispatchMessage(&msg);
		}
	}

	// ���� �� ������� ������ � ��� ������ ���������� ��������� WM_QUIT
	// msg.wParam �������� ��� ��������, ���������� ��� ������ ������� PostQuitMessage()
	return msg.wParam;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT /*lpCreateStruct*/)
{
	// ���������� ������� �������� �������� �����������
	g_lastTick = GetTickCount();

	g_timerId = SetTimer(hwnd, ANIMATION_TIMER_ID, 20, NULL);

	return (g_timerId != 0);
}

void OnDestroy(HWND hWnd)
{
	// ���������� ����� ��������� ������
	KillTimer(hWnd, ANIMATION_TIMER_ID);

	PostQuitMessage(0);
}

void Animate(HWND hwnd)
{
	// ���������� ���������� ������� (� �������������) � ������� ����������� �����
	DWORD currentTick = GetTickCount();
	int delta = currentTick - g_lastTick;
	g_lastTick = currentTick;

	// ������� �� ������� � ��������
	double deltaSeconds = delta * 0.001;

	// ����������� ���� ��������
	g_rotationAngle += ROTATION_SPEED * deltaSeconds;
	// �������� ���� �������� � ��������� �� 0 �� 360 ��������
	g_rotationAngle = fmod(g_rotationAngle, 360);

	// ��������� ��� ���������� ������� ���� ���������������� � �������������� ����
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void OnTimer(HWND hwnd, UINT id) 
{
	// ������������ ��������� �� �������
	switch (id)
	{
	case ANIMATION_TIMER_ID:
		Animate(hwnd);
		break;
	}
}


void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hwnd, &ps);

	// ������� ���� � �������� ��� � ��������� ����������
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN oldPen = SelectPen(dc, pen);

	// �������� ������� ���������� ������� ���� � ������� ��� �����
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	int centerX = rcClient.right / 2;
	int centerY = rcClient.bottom / 2;

	// ��������� ���� �������� ������� � ��������
	double angleInRadians = g_rotationAngle * M_PI / 180.0;

	// ������ �������
	MoveToEx(dc, centerX, centerY, NULL);
	LineTo(
		dc, 
		int(centerX + 100 * cos(angleInRadians)), 
		int(centerY + 100 * sin(angleInRadians))
		);

	// ��������������� ����� ��������� ���� � ������� ����
	SelectPen(dc, oldPen);
	DeletePen(pen);

	EndPaint(hwnd, &ps);
}



LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
	HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
	HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
	HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass =
	{
		sizeof(wndClass),	//UINT cbSize;
		CS_HREDRAW | CS_VREDRAW, //UINT style;
		&WindowProc,		//WNDPROC lpfnWndProc;
		0,					//int cbClsExtra;
		0,					//int cbWndExtra;
		hInstance,			//HINSTANCE hInstance;
		NULL,				//HICON hIcon;
		LoadCursor(NULL, IDC_ARROW), //HCURSOR hCursor;
		(HBRUSH)(COLOR_BTNFACE + 1), //HBRUSH hbrBackground;
		NULL,				//LPCTSTR lpszMenuName;
		CLASS_NAME,			//LPCTSTR lpszClassName;
		NULL,				//HICON hIconSm;
	};

	return RegisterClassEx(&wndClass) != FALSE;
}

