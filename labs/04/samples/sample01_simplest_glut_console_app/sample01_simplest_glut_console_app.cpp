// sample01_simplest_glut_console_app.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


const float M_PI = 3.14159265358979323846f;
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGTH = 600;
const char WINDOW_TITLE[] = "My first OpenGL application";

void OnDrawScene();
void OnResizeWindow(int width, int height);
int g_windowWidth = WINDOW_WIDTH;
int g_windowHeight = WINDOW_HEIGTH;

int main(int argc, char* argv[])
{
	// �������������� ���������� GLUT
	glutInit(&argc, argv);

	/*
	�������������� ������ ������ �����, ������������ OpenGL:
		- 32-������ ����� �����,
		- ������� ������ �������
		- ������� �����������
	*/
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// ������ ��������� ������� ����
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);

	glutCreateWindow(WINDOW_TITLE);

	// ������ ������� ����������� �����
	glutDisplayFunc(&OnDrawScene);

	// ������ ���������� ������� ��������� �������� ����
	glutReshapeFunc(&OnResizeWindow);

	// ��������� ������� ���� ���������
	glutMainLoop();

	// ���� ��, � ���������, ��� �� ��������,
	// �.�. glutMainLoop() �� ���������� ���������� � ��� �������� ����
	// ����� �������� ������� exit(), ������� ��������� ������ ����������
	return 0;
}

// ������ ������������ ������
void Draw5PointStar(float xCenter, float yCenter, float radius)
{
	static const float STEP = M_PI * 4 / 5;

	// �������� ����� ������ ���������� (��������� ������� �����)
	glBegin(GL_LINE_LOOP);

	float angle = -M_PI / 2;

	// ��������� ��������� ������ ����� �����, ������������� �� ����������
	// � ������, � ������: -90, 54, 198, 342, 486 (126) ��������
	for (int i = 0; i < 5; ++i, angle += STEP)
	{
		float x = xCenter + radius * cosf(angle);
		float y = yCenter + radius * sinf(angle);

		// ���� ���������������� ��������� ����� glColor3f,
		// ������ ������� ������ ����� ����� ���� ����
		// glColor3f(i / 5.0f, 1.0f - i / 10.0f, (i + 2) / 7.0f); 

		// ������� glVertex2f ��������� � ������� ������ ����������
		// �����, ������� �� ��������� z = 0
		// ������� 2f � �������� ������� ����������, ��� �������� 2 ����������
		// x � y ���� GLfloat
		glVertex2f(x, y);
	}

	// ����������� ������ ����������
	glEnd();
}

void OnDrawScene()
{
	/*
	������ ���� ������� ������ ����� (������)
	*/
	glClearColor(1, 1, 0, 0);

	// ������� ����� ����� � ����� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ ������� ���� ������� �����
	// ������� 3f � �������� ������� ����������, ��� ����� ������� 
	// 3 ������������ ����� (r, g, b) � ������� float (�� 0 �� 1).
	glColor3f(0, 0, 1);

	// ������ ������������ ������
	Draw5PointStar(300, 200, 120);

	// ���������� ������� � ��������� ������ ����� �������
	glutSwapBuffers();
}

// ������ ������� ���������� ��� ��������� ������������� �������� ����
// � ����� ��������������� ����� �������� ����
void OnResizeWindow(int width, int height)
{
	g_windowWidth = width;
	g_windowHeight = height;

	/*
	������ ��������� � ������� �������� ����� (����� ���������) � ������ �����
	����� ���������� �� ����� �������������� �� ��� �������
	*/
	glViewport(0, 0, width, height);

	// ������ ������� �������� OpenGL ������� �������������
	glMatrixMode(GL_PROJECTION);

	// ��������� � ��� ��������� �������
	glLoadIdentity();

	/*
	� �������� �� �� ������� ���������������� ������������� �����,
	��� ������ �������� ���� ��������� ����� ��������������� 
	����� (0, 0), ������� ������� - (width, height),
	� ����� ����� �������� � ���� ������, ������������� �� ������� � ���������
	�� -1 (�����) �� +1 (�������)
	*/
	glOrtho(0, width, height, 0, -1, 1);

	// ������ ������� �������� ������� �������������-����
	glMatrixMode(GL_MODELVIEW);
}
