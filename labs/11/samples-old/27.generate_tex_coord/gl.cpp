// gl.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Graphics.h"
#include "Utils.h"
#include "ParticleSystem.h"

#define MAX_LOADSTRING 100
#define M_PI 3.1415927f

// Global Variables:
HINSTANCE hInst;								// current instance
const TCHAR WINDOW_CLASS_NAME[] = "gl";				// window class name
const TCHAR WINDOW_TITLE[] = "Spherical texture coordinate generation";				// The title bar text


// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawScene();
void DrawCube();

bool g_useClipPlane = true;
bool g_useStencil = true;
bool g_drawReflection = true;
bool g_objectVisible = true;
bool g_mirrorVisible = true;

GLuint g_texture;

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


float angleX = 0;
float angleY = 0;
float angleZ = 0;

DWORD lastTick = GetTickCount();
float g_delta = 0;

void ShiftTime()
{
	// ������������ �����, ��������� � ������� ���������� ������������� �����
	DWORD currentTick = GetTickCount();
	g_delta = (currentTick - lastTick) * 0.001f;
	lastTick = currentTick;
}

void SetLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	const float diffuseColor[4] = {0.8f, 0.8f, 0.8f, 1};
	const float ambientColor[4] = {0.8f, 0.8f, 0.8f, 1};
	const float specularColor[4] = {1.2f, 1.2f, 1.2f, 1};
	const float lightDirection[4] = {0.0f, 0.0f, 1.0f, 0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
	
}

void SetCubeMaterial()
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	float specular[4] = {0.7f, 0.7f, 0.7f, 1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
}


float g_cameraY = 0;
void AnimateCamera()
{
	float time = g_delta;
	
	glRotatef(g_cameraY, 0, 1, 0);
	
	g_cameraY += 30 * time;
	if (g_cameraY >= 360)
	{
		g_cameraY -= 360;
	}
}

float g_cubeAcc = -3.5f;
float g_cubeSpeed = 3.0f;
float g_cubeY = 1.0;

void RotateCube()
{
	float time = g_delta;

	g_cubeSpeed += time * g_cubeAcc;
	g_cubeY += g_cubeSpeed * time;
	if (g_cubeY < -0.5f)
	{
		g_cubeY -= (g_cubeSpeed * time);
		g_cubeSpeed = -g_cubeSpeed;
	}

	if (g_cubeSpeed > 5)
	{
		g_cubeSpeed = 5;
	}

	angleX += 45 * time;
	if (angleX >= 360)
	{
		angleX -= 360;
	}
	angleY += 59 * time;
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

void TransformCube()
{
	glTranslatef(0, g_cubeY, 0);
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);

}

void DrawMirror(bool visible)
{
	Vector3d v0(-3, 0, -3);
	Vector3d v1(-3, 0, 3);
	Vector3d v2(3, 0, 3);
	Vector3d v3(3, 0, -3);

	if (visible)
	{
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDepthMask(GL_FALSE);
	}
	glColor4d(0.5, 0.5, 0.8, 0.7);
	glBegin(GL_QUADS);
	{
		v0.DeclareOpenGLVertex();
		
		v1.DeclareOpenGLVertex();
		
		v2.DeclareOpenGLVertex();
		
		v3.DeclareOpenGLVertex();
	}
	glEnd();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

ParticleSystem particleSystem(2000);

void MirrorSceneTranspose(const float *sourceMatrix, const Vector3d &mirrorNormal, const Vector3d &mirrorPoint, float *destinationMatrix)
{
	Vector3d xAxis(sourceMatrix[0], sourceMatrix[4], sourceMatrix[8]);
	Vector3d yAxis(sourceMatrix[1], sourceMatrix[5], sourceMatrix[9]);
	Vector3d zAxis(sourceMatrix[2], sourceMatrix[6], sourceMatrix[10]);
	Vector3d basis(-sourceMatrix[12], -sourceMatrix[12], -sourceMatrix[13]);
	
	Vector3d mirrorToBasis = basis - mirrorPoint;
	
	xAxis -= mirrorNormal.Project(xAxis) * 2; //xAxis.Project(mirrorNormal);
	yAxis -= mirrorNormal.Project(yAxis) * 2; //xAxis.Project(mirrorNormal);
	zAxis -= mirrorNormal.Project(zAxis) * 2; //xAxis.Project(mirrorNormal);
	basis -= mirrorNormal.Project(mirrorToBasis) * 2;
	
	xAxis.StoreVector(&destinationMatrix[0]);
	yAxis.StoreVector(&destinationMatrix[4]);
	zAxis.StoreVector(&destinationMatrix[8]);
	basis.StorePoint(&destinationMatrix[12]);
}


void BuildMirrorMatrix(const float *sourceMatrix, const Vector3d &mirrorNormal, const Vector3d &mirrorPoint, float *destinationMatrix)
{
	Vector3d xAxis(&sourceMatrix[0]);
	Vector3d yAxis(&sourceMatrix[4]);
	Vector3d zAxis(&sourceMatrix[8]);
	Vector3d origin(&sourceMatrix[12]);
	
	Vector3d mirrorToOrigin = origin - mirrorPoint;
	
	xAxis -= mirrorNormal.Project(xAxis) * 2;
	yAxis -= mirrorNormal.Project(yAxis) * 2;
	zAxis -= mirrorNormal.Project(zAxis) * 2;
	origin -= mirrorNormal.Project(mirrorToOrigin) * 2;
	
	xAxis.StoreVector(&destinationMatrix[0]);
	yAxis.StoreVector(&destinationMatrix[4]);
	zAxis.StoreVector(&destinationMatrix[8]);
	origin.StorePoint(&destinationMatrix[12]);
}

/*
void InverseTransposeMatrix(const float *mat, float *rv)
{
	rv[0] = mat[0];
	rv[1] = mat[4];
	rv[2] = mat[8];
	rv[3] = mat[3];
	rv[4] = mat[1];
	rv[5] = mat[5];
	rv[6] = mat[9];
	rv[7] = mat[7];
	rv[8] = mat[2];
	rv[9] = mat[6];
	rv[10] = mat[10];
	rv[11] = mat[11];

	rv[12] = -(mat[12]*rv[0] + mat[13]*rv[4] + mat[14]*rv[8]); //-mat[12] - (mat[0] * mat[0]  );
	rv[13] = -(mat[12]*rv[1] + mat[13]*rv[5] + mat[14]*rv[9]);	//-mat[13];
	rv[14] = -(mat[12]*rv[2] + mat[13]*rv[6] + mat[14]*rv[10]);	//-mat[14];

	rv[15] = mat[15];

}
*/

Vector3d TransformVector(const float *mat, Vector3d const & vec)
{
	return Vector3d(
		mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z,
		mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z,
		mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z
		);
}

Vector3d TransformPoint(const float *mat, Vector3d const & pnt)
{
	return Vector3d(
		mat[0] * pnt.x + mat[4] * pnt.y + mat[8] * pnt.z + mat[12],
		mat[1] * pnt.x + mat[5] * pnt.y + mat[9] * pnt.z + mat[13],
		mat[2] * pnt.x + mat[6] * pnt.y + mat[10] * pnt.z + mat[14]
		);
}

void SetCamera()
{
	glLoadIdentity();
	gluLookAt(
		0, 2, 5,	// ������� �����������
		0, 0, 0,	// �����, �� ������� �������
		0, 2, 0);	// ����������� ������� "����" � 3�-������������
}



void DrawScene()
{
	glClearStencil(0);
	// ������ ������� ����� ���������
	ClearBuffers(0, 0, 0, 1);

	// ������ ������ � �������� �����


	ShiftTime();

	SetCubeMaterial();

	SetCamera();
	AnimateCamera();
	RotateCube();
	float cameraMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraMatrix);
	float mirrorMatrix[16];
	{
		Vector3d mirrorDirection(0, 1, 0);
		Vector3d mirrorPosition(0, 0, 0);

		mirrorDirection = TransformVector(cameraMatrix, mirrorDirection);
		mirrorPosition = TransformPoint(cameraMatrix, mirrorPosition);

		BuildMirrorMatrix(cameraMatrix, mirrorDirection, mirrorPosition, mirrorMatrix);
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// ������ ������� � ������ ��������� (�� ����������� ����� ����� � �������)
	if (g_useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 255);
		DrawMirror(false);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glDisable(GL_STENCIL_TEST);
	}


	// ������ ���������
	glFrontFace(GL_CW);
	if (g_drawReflection)
	{
		if (g_useStencil)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_EQUAL, 1, 255);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		
		if (g_useClipPlane)
		{
			glEnable(GL_CLIP_PLANE0);
			double clipPlane[4] = {0, -1, 0, 0};
			glClipPlane(GL_CLIP_PLANE0, clipPlane);
		}
		glLoadMatrixf(mirrorMatrix);
		SetLights();
		TransformCube();
		DrawCube();
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_STENCIL_TEST);
	}
	glFrontFace(GL_CCW);

	{
		glLoadMatrixf(cameraMatrix);
	
		if (g_mirrorVisible)
		{
			glDisable(GL_LIGHTING);		
			DrawMirror(true);
		}

		if (g_objectVisible)
		{
			SetLights();
			TransformCube();
			DrawCube();
		}
		glDisable(GL_CLIP_PLANE0);
	}




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
	if (InitOpenGL(hWnd, 32, 16, 8))
	{
		if ((g_texture = LoadTextureFromRgbBitmapFile("floor.bmp")))
		{
			return true;
		}
	}
	return false;
}

void OnDestroy(HWND hWnd)
{
	glDeleteTextures(1, &g_texture);
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
	case ID_CLIPPLANE_OFF:
		g_useClipPlane = false;
		break;
	case ID_CLIPPLANE_ON:
		g_useClipPlane = true;
		break;
	case ID_STENCIL_ON:
		g_useStencil = true;
		break;
	case ID_STENCIL_OFF:
		g_useStencil = false;
		break;
	case ID_REFLECTION_INVISIBLE:
		g_drawReflection = false;
		break;
	case ID_REFLECTION_VISIBLE:
		g_drawReflection = true;
		break;
	case ID_OBJECT_INVISIBLE:
		g_objectVisible = false;
		break;
	case ID_OBJECT_VISIBLE:
		g_objectVisible = true;
		break;
	case ID_MIRROR_VISIBLE:
		g_mirrorVisible = true;
		break;
	case ID_MIRROR_INVISIBLE:
		g_mirrorVisible = false;
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


void DrawCube()
{

	static const Vector3d cubeVertices[8] =
	{
		Vector3d(-1, -1, -1),	// 0
		Vector3d(-1, -1, +1),	// 1
		Vector3d(+1, -1, -1),	// 2	
		Vector3d(+1, -1, +1),	// 3
		Vector3d(-1, +1, -1),	// 4
		Vector3d(-1, +1, +1),	// 5
		Vector3d(+1, +1, -1),	// 6
		Vector3d(+1, +1, +1),	// 7
	};
	
	//*
	//���� ������������������ ��� ������, �� ��� ����� �������
	static const QuadFace cubeFaces[6] = 
	{
		{2, 3, 1, 0, {1, 0, 0, 1}},
		{7, 3, 2, 6, {0, 1, 0, 1}},
		{5, 1, 3, 7, {0, 0, 1, 1}},
		{4, 0, 1, 5, {1, 1, 0, 1}},
		{6, 2, 0, 4, {0, 1, 1, 1}},
		{5, 7, 6, 4, {1, 0, 1, 1}},
	};
	//*/
	
	/*
	static const QuadFace cubeFaces[6] = 
	{
		{2, 3, 1, 0, {0.4f, 0.4f, 0.4f, 1}},
		{7, 3, 2, 6, {0.4f, 0.4f, 0.4f, 1}},
		{5, 1, 3, 7, {0.4f, 0.4f, 0.4f, 1}},
		{4, 0, 1, 5, {0.4f, 0.4f, 0.4f, 1}},
		{6, 2, 0, 4, {0.4f, 0.4f, 0.4f, 1}},
		{5, 7, 6, 4, {0.4f, 0.4f, 0.4f, 1}},
	};

  //*/
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);


	static const int numFaces = sizeof(cubeFaces) / sizeof(QuadFace);
	glBegin(GL_QUADS);
	{
		for (int faceIndex = 0; faceIndex < numFaces; ++faceIndex)
		{
			QuadFace const& face = cubeFaces[faceIndex];
			face.faceColor.SetOpenGLColor();
			Vector3d const& v0 = cubeVertices[face.v0];
			Vector3d const& v1 = cubeVertices[face.v1];
			Vector3d const& v2 = cubeVertices[face.v2];
			Vector3d const& v3 = cubeVertices[face.v3];
			Vector3d normal;
			CalculateTriangleNormal(v0, v1, v2, &normal);
			//normal.DeclareOpenGLNormal();
			Vector3d n0, n1, n2, n3;
			n0 = (v0*0.15f + normal); n0.Normalize();
			n1 = (v1*0.15f + normal); n1.Normalize();
			n2 = (v2*0.15f + normal); n2.Normalize();
			n3 = (v3*0.15f + normal); n3.Normalize();

			n0.DeclareOpenGLNormal();
			v0.DeclareOpenGLVertex();

			n1.DeclareOpenGLNormal();
			v1.DeclareOpenGLVertex();

			n2.DeclareOpenGLNormal();
			v2.DeclareOpenGLVertex();

			n3.DeclareOpenGLNormal();
			v3.DeclareOpenGLVertex();
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	

}
