// gl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Graphics.h"
#include "Utils.h"

#define MAX_LOADSTRING 100
#define M_PI 3.1415927f

// Global Variables:
HINSTANCE hInst;								// current instance
const TCHAR WINDOW_CLASS_NAME[] = "gl";				// window class name
const TCHAR WINDOW_TITLE[] = "Billboard";				// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawScene();

GLuint	g_billboardTexture = 0;

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
		0, 0, 3,	// ������� �����������
		0, 0, 0,	// �����, �� ������� �������
		0, 1, 0);	// ����������� ������� "����" � 3�-������������
}

float angleX = 0;
float angleY = 0;
float angleZ = 0;

DWORD lastTick = GetTickCount();

void Rotate()
{
//	glTranslatef(-0.5, 0, 0);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);

	// ������������ �����, ��������� � ������� ���������� ������������� �����
	DWORD currentTick = GetTickCount();
	float time = (currentTick - lastTick) * 0.001f;
	lastTick = currentTick;

	angleX += 60 * time;
	if (angleX >= 360)
	{
		angleX -= 360;
	}

	angleY += 46 * time;
	if (angleY >= 360)
	{
		angleY -= 360;
	}

	angleZ += 53 * time;
	if (angleZ >= 360)
	{
		angleZ -= 360;
	}

}

void DrawBillboard(Vector3d const& position, float sizeX, float sizeY)
{
	float modelview[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelview[0][0]);

	float sizeX2 = sizeX * 0.5f;
	float sizeY2 = sizeY * 0.5f;
	// ��������� ����������� ������������ ���� �� ������� �������������-���� 
	// � �������� jcb x � y �� �������� ������� ���������
	Vector3d xAxis(modelview[0][0]*sizeX2, modelview[1][0]*sizeX2, modelview[2][0]*sizeX2);
	Vector3d yAxis(modelview[0][1]*sizeY2, modelview[1][1]*sizeY2, modelview[2][1]*sizeY2);

	// ������� �������� ������� ������ ��������� ������������ ��� ������
	Vector3d p0(-xAxis.x + yAxis.x, -xAxis.y + yAxis.y, -xAxis.z + yAxis.z);
	Vector3d p1(-xAxis.x - yAxis.x, -xAxis.y - yAxis.y, -xAxis.z - yAxis.z);
	Vector3d p2(+xAxis.x - yAxis.x, +xAxis.y - yAxis.y, +xAxis.z - yAxis.z);
	Vector3d p3(+xAxis.x + yAxis.x, +xAxis.y + yAxis.y, +xAxis.z + yAxis.z);

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// ���������� �������� ������ ��������
	Vector2d t0(0, 0);
	Vector2d t1(0, 1);
	Vector2d t2(1, 1);
	Vector2d t3(1, 0);

	glBegin(GL_QUADS);
	{
		/*
		0-------3
		|		|
		|		|
		|		|
		|		|
		1-------2
		*/
		Vector3d v0 = position + p0;
		t0.DeclareTextureCoord();
		v0.DeclareOpenGLVertex();

		Vector3d v1 = position + p1;
		t1.DeclareTextureCoord();
		v1.DeclareOpenGLVertex();

		Vector3d v2 = position + p2;
		t2.DeclareTextureCoord();
		v2.DeclareOpenGLVertex();

		Vector3d v3 = position + p3;
		t3.DeclareTextureCoord();
		v3.DeclareOpenGLVertex();

		//glVertex3fv();
	}
	glEnd();
}

void DrawBasis()
{
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
	ClearBuffers(0, 0, 0, 1);

	// ������ ������ � �������� �����
	SetCamera();

	Rotate();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	DrawBillboard(Vector3d(0.5f, 0 , 0), 2, 1);
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

bool OnCreate(HWND hWnd)
{
	// �������������� OpenGL
	if (InitOpenGL(hWnd))
	{

		if ((g_billboardTexture = LoadTextureFromRgbBitmapFile("cat.bmp")) != 0)
		{
			return true;
		}
	}
	return false;
}

void OnDestroy(HWND hWnd)
{
	if (g_billboardTexture)
	{
		glDeleteTextures(1, &g_billboardTexture);
	}
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

void OnCommandMessage(WPARAM wParam, LPARAM lParam)
{
	/*
	switch(wParam)
	{
	
	default:
		break;
	}
	*/
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_COMMAND:
		OnCommandMessage(wParam, lParam);
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
		OnDestroy(hWnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

