// gl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Graphics.h"
#include "Utils.h"

#define MAX_LOADSTRING 100
#define M_PI 3.1415927

// Global Variables:
HINSTANCE hInst;								// current instance
const TCHAR WINDOW_CLASS_NAME[] = "gl";				// window class name
const TCHAR WINDOW_TITLE[] = "Texture environment modes";				// The title bar text

GLuint g_eagleTexture = 0;

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
	wcex.lpszMenuName	= (LPCSTR)IDR_MAIN_MENU;
	wcex.lpszClassName	= WINDOW_CLASS_NAME;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

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


GLint g_textureEnv = GL_MODULATE;

void DrawQuad()
{
	if (g_textureEnv != 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g_eagleTexture);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, g_textureEnv);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	glBegin(GL_QUADS);
	{
		glColor3f(1, 0, 0);
		glTexCoord2f(0, 0);
		glVertex2f(-1, -1);
		
		glColor3f(0, 1, 0);
		glTexCoord2f(1, 0);
		glVertex2f(1, -1);
		
		glColor3f(0, 0, 1);
		glTexCoord2f(1, 1);
		glVertex2f(1, 1);
		
		glColor3f(1, 0, 1);
		glTexCoord2f(0, 1);
		glVertex2f(-1, 1);
		
	}
	glEnd();
}

void SetProjection(double aspect)
{
	// ������������� ������� ������������� (aspect - ��������� ������ ������ � ��� ������)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect, 0.2, 20);
	glMatrixMode(GL_MODELVIEW);
}

void SetCamera()
{
	glLoadIdentity();
	gluLookAt(
		0, 0, 2,	// ������� �����������
		0, 0, 0,	// �����, �� ������� �������
		0, 1, 0);	// ����������� ������� "����" � 3�-������������
}

float angleX = 0;
float angleY = 0;
float angleZ = 0;

DWORD lastTick = GetTickCount();

void RotateObject()
{
//	glTranslatef(-0.5, 0, 0);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);

	// ������������ �����, ��������� � ������� ���������� ������������� �����
	DWORD currentTick = GetTickCount();
	float time = (currentTick - lastTick) * 0.001f;
	lastTick = currentTick;

	angleX += 23 * time;
	if (angleX >= 360)
	{
		angleX -= 360;
	}

	angleY += 17 * time;
	if (angleY >= 360)
	{
		angleY -= 360;
	}

	angleZ += 19 * time;
	if (angleZ >= 360)
	{
		angleZ -= 360;
	}

}

void DrawBasis()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(3);
	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(2, 0, 0);
		
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 2, 0);
		
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 2);
		
	}
	glEnd();
}

void DrawScene()
{
	// ������ ������� ����� ���������
	ClearBuffers(0, 0, 0, 0);

	// ������ ������ � �������� �����
	SetCamera();

	//SetLights();
	//SetCubeMaterial();

	// ������� ������
	//RotateObject);


	// �������� ���� ������� ��� ����������� ����������� ��������� ������������
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// ������ �������������
	DrawQuad();

	DrawBasis();

	// ����������� �������� - ���������� ����� ��������� � 
	// ������ ����������� � �������� ������ �������
	EndDrawing();
}


void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	// ������ ����� - ���� ��� ����� ������ ����������� ������� ������ ���������� ������� ����
	DrawScene();

	EndPaint(hWnd, &ps);
}

bool LoadEagleTexture()
{
	if (g_eagleTexture != 0)
	{
		glDeleteTextures(1, &g_eagleTexture);
		g_eagleTexture = 0;
	}

	glGenTextures(1, &g_eagleTexture);
	glBindTexture(GL_TEXTURE_2D, g_eagleTexture);
	g_eagleTexture = LoadTextureFromRgbBitmapFile("eagle.bmp");
	return true;
}


bool OnCreate(HWND hWnd)
{
	// �������������� OpenGL
	if (InitOpenGL(hWnd))
	{
		if (LoadEagleTexture())
		{
			return true;
		}		

	}
	return false;
}

void OnDestroy()
{
	glDeleteTextures(1, &g_eagleTexture);
	DestroyOpenGL();

}

void OnSize(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	glViewport(0, 0, width, height);
	SetProjection((double)width / (double)height);
}


void OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(wParam) 
	{
	case ID_GL_MODULATE:
		g_textureEnv = GL_MODULATE;
		break;
	case ID_GL_DECAL:
		g_textureEnv = GL_DECAL;
		break;
	case ID_GL_ADD:
		g_textureEnv = GL_ADD;
		break;
	case ID_GL_REPLACE:
		g_textureEnv = GL_REPLACE;
		break;
	case ID_NO_TEXTURE:
		g_textureEnv = 0;
		break;
	default:
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_COMMAND:
		OnCommand(wParam, lParam);
		break;
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
		return 1;	// ������ ���, ��� �������� ��� ����
		break;
	case WM_SIZE:
		OnSize(hWnd);
		break;
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_DESTROY:
		OnDestroy();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

