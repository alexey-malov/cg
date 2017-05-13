#include "StdAfx.h"
#include "MyApplication.h"

const float M_PI = 3.14159265358979323846f;



CMyApplication::CMyApplication(const char *title)
:CGLApplication(title)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	// ������ ���� ������� ������ �����
	glClearColor(1, 1, 0, 1);
}

void CMyApplication::OnDisplay(void)
{
	// ������� ����� ����� � ����� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ����������� ���� ������
	glColor3f(0, 0, 1);

	// ������ ����������� ������
	FillEllipse(150, 120, 100, 90);
}

void CMyApplication::OnReshape(int width, int height)
{
	// ����������� ���� ���������
	glViewport(0, 0, width, height);

	// ������ ������� ������� ������������� � ����������� �� ���������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);

	// ������ ������� �������� ������� �������������-����
	glMatrixMode(GL_MODELVIEW);
}

// ������ ����������� ������
void CMyApplication::FillEllipse(float xCenter, float yCenter, float rx, float ry, int points)
{
	const float step = 2 * M_PI / points;

	// ������ ����������� � ���� "�����" �� �������������
	glBegin(GL_TRIANGLE_FAN);
	// ��������� ����� ����� ������������� � ������ �������
	glVertex2d(xCenter, yCenter);
	// ��������� ����� - ���������� �� ��� �������
	for (float angle = 0; angle <= 2 * M_PI; angle += step)
	{
		float a = (fabsf(angle - 2 * M_PI) < 1e-5) ? 0 : angle;
		const float dx = rx * cosf(a);
		const float dy = ry * sinf(a);
		glVertex2f(dx + xCenter, dy + yCenter);
	}
	glEnd();
}
