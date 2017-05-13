#include "StdAfx.h"
#include "MyApplication.h"

// ���� ������ �� ��������� 
const double CMyApplication::FIELD_OF_VIEW = 60;
// ������ ������� ����
const double CMyApplication::CUBE_SIZE = 1;


const double CMyApplication::Z_NEAR = 0.1;
const double CMyApplication::Z_FAR = 10;


CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
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


	// �������������� ������� ������������� ����
	glLoadIdentity();
	// � �������� �� �� ������� ��������� �� ����� (2, 2, 2) � ����� (0, 0, 0)
	// � ������������ �������� (0, 1, 0)
	gluLookAt(
		2, 2, 2, 
		0, 0, 0,
		0, 1, 0
		);

	m_cube.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	// ������ ������� �������������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// ������� �������������� �������������� � �����������:
	//	���� ������ �� ���������: FIELD_OF_VIEW ��������
	//	��������� ������ � ������ ������� ��������: aspect
	//	���������� �� ������� � ������� ���������� ��������� �������� ������:
	//		zNear � zFar ��������������
	gluPerspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}
