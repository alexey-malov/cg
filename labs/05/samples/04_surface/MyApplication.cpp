#include "StdAfx.h"
#include "MyApplication.h"
#include "SincSurface.h"
#include "DirectLight.h"
#include "Material.h"

const float M_PI = 3.14159265358979323846f;
const double CMyApplication::FOV = 60;
const double CMyApplication::ZNEAR = 0.1;
const double CMyApplication::ZFAR = 50;
const float CMyApplication::ROTATION_SPEED = 20;



CMyApplication::CMyApplication(const char *title, int width, int height)
:CGLApplication(title, width, height)
,m_surface(50, 50, -10, 10, -10, 10)
,m_frame(3)
,m_rotationAngle(0)
,m_polygonMode(GL_FILL)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	m_lastTick = GetTickCount();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void CMyApplication::OnIdle()
{
	// ���� ���������� ����� �������� - �� ��������� �����
	PostRedisplay();
}

void CMyApplication::OnMouse(int button, int state, int /*x*/, int /*y*/)
{
	// ����������� ������ ������������ ��������� 
	// �� ������� ����� ������ ����
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		m_polygonMode = 
			(m_polygonMode == GL_FILL) ? GL_LINE :
			(m_polygonMode == GL_LINE) ? GL_POINT : GL_FILL;
	}
}

void CMyApplication::SetupLightsAndMaterials()
{
	// �������� ��������� � �������� ����� �0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ������ ��������� ������������� ��������� �����
	CDirectLight light(CVector3f(2, 2, 2));
	light.SetSpecularIntensity(1, 1, 1);
	light.SetDiffuseIntensity(1, 1, 1);
	light.SetLight(GL_LIGHT0);

	// ������ ��������� ���������
	CMaterial material;
	material.SetDiffuse(0.8f, 0.8f, 0.f);
	material.SetSpecular(0.3f, 0.3f, 0.3f);
	material.SetShininess(50);
	material.Activate();
}

void CMyApplication::SetupCamera()
{
	DWORD currentTick = GetTickCount();

	float delta = (currentTick - m_lastTick) * 0.001;
	m_lastTick = currentTick;

	// ��������� ���� �������� ������
	m_rotationAngle = fmodf(m_rotationAngle + ROTATION_SPEED * delta, 360);

	// ������������� ������
	glLoadIdentity();
	gluLookAt(
		12, 12, 8, 
		0, 0, 0, 
		0, 0, 1);

	// ������� ������ ������ ��� Z
	glRotatef(m_rotationAngle, 0, 0, 1);
}

void CMyApplication::OnDisplay(void)
{
	// ������� ����� ����� � ����� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupCamera();
	SetupLightsAndMaterials();

	// �������� ���� ������� ��� �������� ��������� ����� � ������������
	// ����� ���������� 3D-�����������
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT, m_polygonMode);
	glPointSize(3);
	m_surface.Draw();

	// ��������� ��������� � ���� ������� ����� ���������� ������������� ������
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	m_frame.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	// ����������� ���� ���������
	glViewport(0, 0, width, height);

	// ������ ������� ������� ������������� � ����������� �� ���������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double aspect = double(width) / double(height);
	gluPerspective(FOV, aspect, ZNEAR, ZFAR);

	// ������ ������� �������� ������� �������������-����
	glMatrixMode(GL_MODELVIEW);
}
