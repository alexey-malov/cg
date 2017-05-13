#include "StdAfx.h"
#include "MyApplication.h"
#include "DirectLight.h"

// ���� ������ �� ��������� 
const double CMyApplication::FIELD_OF_VIEW = 60;
// ������ ������� ����
const double CMyApplication::CUBE_SIZE = 1;


const double CMyApplication::Z_NEAR = 0.1;
const double CMyApplication::Z_FAR = 10;


CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_windowWidth(width)
,m_windowHeight(height)
,m_leftButtonPressed(false)
,m_mouseX(0)
,m_mouseY(0)
{
	m_cube.SetSideColor(CCube::CS_NEGATIVE_X, 255, 0, 0);
	m_cube.SetSideColor(CCube::CS_POSITIVE_X, 0, 255, 0);
	m_cube.SetSideColor(CCube::CS_NEGATIVE_Y, 0, 0, 255);
	m_cube.SetSideColor(CCube::CS_POSITIVE_Y, 255, 255, 0);
	m_cube.SetSideColor(CCube::CS_NEGATIVE_Z, 0, 255, 255);
	m_cube.SetSideColor(CCube::CS_POSITIVE_Z, 255, 0, 255);
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	// ������� � �������������� �������� �����
	CDirectLight light(CVector3f(1, 1, 3));	// ����������� �� �������� �����
	light.SetDiffuseIntensity(0.5f, 0.5f, 0.5f); // ������������� ��������� ������������
	light.SetAmbientIntensity(0.2f, 0.2f, 0.2f); // ������������� ������� ������������
	light.SetSpecularIntensity(0.3f, 0.3f, 0.3f);// ������������� ���������� ������������
	light.SetLight(GL_LIGHT0);	// �������� ��������� ��������� ����� � OpenGL

	// �������� ��������� � �������� ����� �0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ��������� ����� ������ �� �������� ���������� �� ������ ���������
	// ����� ��� Z
	gluLookAt(
		2, 2, 2, 
		0, 0, 0,
		0, 1, 0
		);

	// �������� ����� ���������� ������
	glEnable(GL_CULL_FACE);
	// ��������������� ����� ��������� ������� ������
	glCullFace(GL_BACK);
	// ������� ��������� ��������� �������, ���� ��� �� ���������
	// ����� ������ �������������� ������ ������� �������
	glFrontFace(GL_CCW);

	// �������� ���� ������� ��� �������� ��������� ����� � ������������
	glEnable(GL_DEPTH_TEST);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �������� �����, ����������� ������ ��������� 
	// ����������� �������� ����� �������
	glEnable(GL_COLOR_MATERIAL);
	// ���� ������� ����� ���������� ��������� � ������� 
	// ������������ ����� ���������
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// ������ ����������� ����������� ��������� ���������
	GLfloat specularColor[4] = {0.3, 0.3, 0.3, 1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);

	// ������ ������� ����������� ��������� ���������
	glMaterialf(GL_FRONT, GL_SHININESS, 20);

	m_cube.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
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
