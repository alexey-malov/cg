// gl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Graphics.h"

#define MAX_LOADSTRING 100
#define M_PI 3.1415927

// Global Variables:
HINSTANCE hInst;								// current instance
const TCHAR WINDOW_CLASS_NAME[] = "gl";				// window class name
const TCHAR WINDOW_TITLE[] = "Sine curves with GL_LINE_STRIP";				// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawScene();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;

	// Initialize global strings
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// Main message loop:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				break;
			}
		}
		else
		{
			DrawScene();
			Sleep(10);
		}
	}

	return msg.wParam;
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

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GL);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)NULL;
	wcex.lpszClassName	= WINDOW_CLASS_NAME;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

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

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(
	   WINDOW_CLASS_NAME, 
	   WINDOW_TITLE, 
	   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	   CW_USEDEFAULT, 0, 
	   CW_USEDEFAULT, 0, 
	   NULL, 
	   NULL, 
	   hInstance, 
	   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


DWORD g_lastTick = GetTickCount();
float g_currentAngleX = 0;
float g_currentAngleY = 0;
#define CURVE_LENGTH 100
#define CURVE_STEP_X ((float)(-5.25 * M_PI / 180))
#define CURVE_STEP_Y ((float)(-8.3 * M_PI / 180))
#define SPEED_X 63.52f
#define SPEED_Y 97.52f
void DrawLineStrip()
{
	// вычисляем время с момента предыдущего кадра
	DWORD currentTick = GetTickCount();
	float delta = (currentTick - g_lastTick) * 0.001f;
	g_lastTick = currentTick;

	// Вычисляем начальную фазу колебаний по X
	g_currentAngleX = g_currentAngleX + SPEED_X * delta;
	if (g_currentAngleX >= 360)
	{
		g_currentAngleX -= 360;
	}

	// Вычисляем начальную фазу колебаний по Y
	g_currentAngleY = g_currentAngleX + SPEED_Y * delta;
	if (g_currentAngleY >= 360)
	{
		g_currentAngleY -= 360;
	}

	float angleX = (float)(g_currentAngleX * M_PI / 180);
	float angleY = (float)(g_currentAngleY * M_PI / 180);
	glLineWidth(3);

	// Рисуем синусоиду
	glBegin(GL_LINE_STRIP);
	for (int pnt = 0; pnt < CURVE_LENGTH; ++pnt, angleX += CURVE_STEP_X, angleY += CURVE_STEP_Y)
	{
		glColor3f(
			0.5f + 0.5f*sinf(angleX), 
			0.5f + 0.5f*cosf(angleY), 
			0.5f + 0.5f*sinf(angleX + angleY));
		glVertex2f(cosf(angleX), sinf(angleY));
	}
	glEnd();
}

void DrawScene()
{
	// просто очищаем буфер рисования
	ClearBuffers(0, 0, 0, 0);

	DrawLineStrip();

	// завершающие действия - дожидаемся конца рисования и 
	// меняем внеэкранный и экранный буферы местами
	EndDrawing();
}


void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	// Рисуем сцену - пока это будет просто закрашенная красным цветом клиенсткая область окна
	DrawScene();

	EndPaint(hWnd, &ps);
}

bool OnCreate(HWND hWnd)
{
	// инициализируем OpenGL
	return InitOpenGL(hWnd);
}

void OnSize(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_CREATE:
		if (OnCreate(hWnd))
		{
			return 0;
		}
		else
		{
			return -1;
		}
		break;
	case WM_ERASEBKGND:
		return 1;	// делаем вид, что очистили фон окна
		break;
	case WM_SIZE:
		OnSize(hWnd);
		break;
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_DESTROY:
		DestroyOpenGL();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

