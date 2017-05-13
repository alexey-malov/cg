// gl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Graphics.h"
#include "Utils.h"
#include "MapTile.h"

#define MAX_LOADSTRING 100
#define M_PI 3.1415927f

// Global Variables:
HINSTANCE hInst;								// current instance
const TCHAR WINDOW_CLASS_NAME[] = "gl";				// window class name
const TCHAR WINDOW_TITLE[] = "Terrain - part 3";				// The title bar text


// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawScene();


MapTile g_tile;

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
	gluPerspective(60, aspect, 2, 200);
	glMatrixMode(GL_MODELVIEW);
}


DWORD lastTick = GetTickCount();
float g_delta = 0;

void ShiftTime()
{
	// ������������ �����, ��������� � ������� ���������� ������������� �����
	DWORD currentTick = GetTickCount();
	g_delta = (currentTick - lastTick) * 0.001f;
	lastTick = currentTick;
}

float g_cameraAngle = 0;
void RotateCamera()
{
	g_cameraAngle += 50 * g_delta;
	if (g_cameraAngle > 360)
	{
		g_cameraAngle -= 360;
	}
	glRotatef(g_cameraAngle, 0, 0, 1);
}

void SetCamera()
{
	glLoadIdentity();
	gluLookAt(
		0, -80, 50,	// ������� �����������
		0, 0, 0,	// �����, �� ������� �������
		0, 0, 1);	// ����������� ������� "����" � 3�-������������
}

GLenum g_polygonMode = GL_FILL;
bool g_textureEnabled = true;

void DrawScene()
{
	// ������ ������� ����� ���������
	ClearBuffers(0, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT, g_polygonMode);
	if (g_textureEnabled)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	
	ShiftTime();
	SetCamera();
	RotateCamera();

	g_tile.Draw();

	// ����������� �������� - ���������� ����� ��������� � 
	// ������ ����������� � �������� ������ �������
	EndDrawing();
}


void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	
	DrawScene();

	EndPaint(hWnd, &ps);
}


bool InitMapTile()
{
	GLfloat heightMap[VERTEX_TILE_SIZE * VERTEX_TILE_SIZE];
	BYTE heightFile[VERTEX_TILE_SIZE * VERTEX_TILE_SIZE];
	FILE *pFile = fopen("height.raw", "rb");
	if (!pFile)
	{
		return false;
	}
	fread(&heightFile, 1, sizeof(heightFile), pFile);
	fclose(pFile);

	static const float HEIGHT_SCALE = 50 / 255.0f;
	static const float HEIGHT_OFFSET = HEIGHT_SCALE / 2;

	for (int i = 0; i < sizeof(heightFile); ++i)
	{
		heightMap[i] = heightFile[i] * HEIGHT_SCALE + HEIGHT_OFFSET;
	}	

	GLuint tex = LoadTextureFromRgbBitmapFile("tex.bmp");
	if (!tex)
	{
		return false;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	g_tile.Load(-VERTEX_TILE_SIZE / 2, -VERTEX_TILE_SIZE / 2, VERTEX_TILE_SIZE, VERTEX_TILE_SIZE, 1, &heightMap[0], tex);

	return true;
}

void DeleteTileMap()
{
	g_tile.Destroy();
}

bool OnCreate(HWND hWnd)
{
	// �������������� OpenGL
	if (InitOpenGL(hWnd))
	{
		if (InitMapTile())
		{
			return true;
		}
	}
	return false;
}

void OnDestroy(HWND hWnd)
{
	DeleteTileMap();
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
	switch(wParam) 
	{
	case ID_POLYGONMODE_LINES:
		g_polygonMode = GL_LINE;
		break;
	case ID_POLYGONMODE_POINTS:
		g_polygonMode = GL_POINT;
		break;
	case ID_POLYGONMODE_SOLID:
		g_polygonMode = GL_FILL;
		break;
	case ID_TEXTURE_ENABLE:
		g_textureEnabled = true;
		break;
	case ID_TEXTURE_DISABLE:
		g_textureEnabled = false;
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

