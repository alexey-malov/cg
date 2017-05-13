#include "StdAfx.h"
#include "MyApplication.h"
#include "Frame.h"
#include "Vector3.h"

// ������ �������� ������, ������� ������ ����������� � ���� ���������
const double CMyApplication::FRUSTUM_SIZE = 2;
// ���������� �� ������ �� ����� ��������
const double CMyApplication::DISTANCE_TO_ORIGIN = 2;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_windowWidth(width)
,m_windowHeight(height)
,m_leftButtonPressed(false)
,m_mouseX(0)
,m_mouseY(0)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	// ��������� ����� ������ �� �������� ���������� �� ������ ���������
	// ����� ��� Z
	glTranslatef(0, 0, -DISTANCE_TO_ORIGIN);

	// ������ ������ �����
	glLineWidth(2);

	// � ���� ������� ������ �����
	glClearColor(1, 1, 1, 1);

	// �������� ���� ������� ��� �������� ��������� ����� � ������������
	glEnable(GL_DEPTH_TEST);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������� ������������ ����� � ������ ���
	CFrame frame;
	frame.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	// �������, ��� ������ ������� ������� ����� FRUSTUM_SIZE
	// (�� ���������� �� ������� ��������� ���������)
	double frustumHeight = FRUSTUM_SIZE;
	// ������ ������� ������� �������������� �������� ����������� ������ ����
	// (���� ���� - ���� ������� ��������� � ��������)
	double frustumWidth = frustumHeight * aspect;

	// ���� ������ ������� ������� ���������� ������, ��� FRUSTUM_SIZE, 
	// �� ������������ ������� ������� �������
	if (frustumWidth < FRUSTUM_SIZE && (aspect != 0))
	{
		frustumWidth = FRUSTUM_SIZE;
		frustumHeight = frustumWidth / aspect;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(
		-frustumWidth * 0.5, frustumWidth * 0.5,	// left, right
		-frustumHeight * 0.5, frustumHeight * 0.5,	// top, bottom
		FRUSTUM_SIZE * 0.5, FRUSTUM_SIZE * 20		// znear, zfar
		);
	glMatrixMode(GL_MODELVIEW);
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	// ������� �� ����� ������ ����
	if (button == GLUT_LEFT_BUTTON)
	{
		// ��������� ��������� ����� ������ ����
		m_leftButtonPressed = (state == GLUT_DOWN);
		// ��������� ���������� ����
		m_mouseX = x;
		m_mouseY = y;
	}
}

void CMyApplication::OnMotion(int x, int y)
{
	// ���� ������ ����� ������ ����
	if (m_leftButtonPressed)
	{
		// ��������� �������� ������� ����
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		// ��������� ���� �������� ������ ���� Y � X ��� ������� ���������
		// �� �������� ���� �� ���� X � Y
		GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
		GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;

		RotateCamera(rotateX, rotateY);

		// ��������� ������� ���������� ����
		m_mouseX = x;
		m_mouseY = y;

		// ���������� ����������� ����
		PostRedisplay();
	}
}

// ������� ������ ������ ������ �������� �� �������� ����
void CMyApplication::RotateCamera(GLfloat rotateX, GLfloat rotateY)
{
	// ��������� ������� �������� ������� �������������-����
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0]);

	// ��������� ����������� ������������ ���� ������ � 3� ������������
	// ��� ������������ ����� ������� �������������-����
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);

	// ������������ ������ ���� x � y ������
	glRotatef(rotateX, xAxis.x, xAxis.y, xAxis.z);
	glRotatef(rotateY, yAxis.x, yAxis.y, yAxis.z);

	// � ���� ��������� ������ ����� ��������� �����������, �������,
	// ������������ ����� ������ �������� ��������
	// ��� �� ����������� ����� ������ ����������� ������� �������������-����
	// �������� �� ����������������
	NormalizeModelViewMatrix();
}

// ���������� ���������������� ������� �������������-����
void CMyApplication::NormalizeModelViewMatrix(void)
{
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

	/*
	���������������� - ���������� ������������ ���� � ��������� ����� (������������)
	� �������� ������������������ (���������������)
	������� ����� ����� ��� ������ ������������ ������������ ����
	� ���������� ������������
	*/
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	xAxis.Normalize();	
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);
	yAxis.Normalize();

	// ��� Z ��������� ����� ��������� ������������ X � Y
	// Z ����� ��������������� ��������� �������� X � Y
	CVector3f zAxis = Cross(xAxis, yAxis);
	// � ����� ��������� �����
	zAxis.Normalize();
	// �� �� ����� ����������� � ����� x � y
	xAxis = Cross(yAxis, zAxis);
	xAxis.Normalize();
	yAxis = Cross(zAxis, xAxis);
	yAxis.Normalize();

	// ��������� ������� ������������ ���� ������� � ������
	modelView[0] = xAxis.x; modelView[4] = xAxis.y, modelView[8] = xAxis.z;
	modelView[1] = yAxis.x; modelView[5] = yAxis.y, modelView[9] = yAxis.z;
	modelView[2] = zAxis.x; modelView[6] = zAxis.y, modelView[10] = zAxis.z;

	// � ��������� ������� ������������-����
	glLoadMatrixf(modelView);
}
