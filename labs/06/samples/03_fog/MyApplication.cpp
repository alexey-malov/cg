#include "StdAfx.h"
#include "MyApplication.h"
#include "Vector3.h"

// ���������� �� ������� ��������� ��������� ������������� ������
const double CMyApplication::ZNEAR = 0.1;
// ���������� �� ������ ��������� ��������� ������������� ������
const double CMyApplication::ZFAR = 200;
// ���� ������ �� ���������
const double CMyApplication::FIELD_OF_VIEW = 70;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_light(CVector3f(10, 10, 10))
,m_rotationController(width, height)
,m_fogEnabled(true)
{
	m_light.SetDiffuseIntensity(1, 1, 1, 1);
	m_light.SetAmbientIntensity(0.2f, 0.2f, 0.2f, 1);
	m_light.SetSpecularIntensity(1, 1, 1, 1);

	m_teapotMaterial.SetDiffuse(0.8f, 0.6f, 0.8f);
	m_teapotMaterial.SetAmbient(0.4f, 0.3f, 0.4f);
	m_teapotMaterial.SetSpecular(1, 1, 1);
	m_teapotMaterial.SetShininess(30);
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 0.5, 1);

	gluLookAt(3, 0, 0, 0, 0, 0, 0, 0, 1);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �������� ���� ��������� �����
	if (m_fogEnabled)
	{
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}

	// ������ ����� ������
	glFogi(GL_FOG_MODE, GL_EXP2);

	// ������ ���� ������
	float fogColor[4] = {1, 1, 0.5, 1};
	glFogfv(GL_FOG_COLOR, fogColor);

	// � ��� ���������
	glFogf(GL_FOG_DENSITY, 0.4);

	// �������� ��������� � ������ ������
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	m_light.SetLight(GL_LIGHT0);
	m_teapotMaterial.Activate();

	// ������ ������
	glutSolidTeapot(1);
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	m_rotationController.ResizeWindow(width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
}

void CMyApplication::OnMotion(int x, int y)
{
	if (m_rotationController.LeftButtonIsPressed())
	{
		m_rotationController.OnMotion(x, y);
		PostRedisplay();
	}
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	m_rotationController.OnMouse(button, state, x, y);
}

void CMyApplication::OnKeyboard(unsigned char key, int /*x*/, int /*y*/)
{
	if (key == ' ')
	{
		m_fogEnabled = !m_fogEnabled;
		PostRedisplay();
	}
}
