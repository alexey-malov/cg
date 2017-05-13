#include "StdAfx.h"
#include "MyApplication.h"
#include "Texture.h"
#include "Vector3.h"

// ���������� �� ������� ��������� ��������� ������������� ������
const double CMyApplication::ZNEAR = 0.1;
// ���������� �� ������ ��������� ��������� ������������� ������
const double CMyApplication::ZFAR = 200;
// ���� ������ �� ���������
const double CMyApplication::FIELD_OF_VIEW = 70;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_earth(L"earth.png")
,m_rotationController(width, height)
,m_light(CVector3f(0, 10, 0))
,m_skyBox(
	L"Galaxy_LT.png",
	L"Galaxy_RT.png",
	L"Galaxy_DN.png",
	L"Galaxy_UP.png",
	L"Galaxy_BK.png",
	L"Galaxy_FT.png"
	)
,m_polygonMode(GL_FILL)
{
	m_light.SetDiffuseIntensity(1, 1, 1);
	m_light.SetSpecularIntensity(1, 1, 1);

	m_earth.SetRotationSpeed(30);
	m_earth.SetInclinationAngle(23.5);
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	gluLookAt(0, 2, 1, 0, 0, 0, 0, 0, 1);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �������� ����� ��������� ��������
	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);

	DrawSkyBox();
	DrawEarth();
}

void CMyApplication::DrawEarth()const
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	m_light.SetLight(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	m_earth.Draw();
}

void CMyApplication::DrawSkyBox()const
{
	glDisable(GL_LIGHTING);
	// �������� ����� ���������� ������, �.�. 
	// �� ��������� ������ ���� � ����� ��� ��������� �������
	glDisable(GL_CULL_FACE);
	m_skyBox.Draw();
}

void CMyApplication::OnIdle()
{
	m_animationController.Tick();
	m_earth.Animate(m_animationController.GetTimeDelta() * 0.001);
	PostRedisplay();
	Sleep(10);
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

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	// ������� �� ����� ������ ����
	m_rotationController.OnMouse(button, state, x, y);
}

void CMyApplication::OnMotion(int x, int y)
{
	// ���� ������ ����� ������ ����
	if (m_rotationController.LeftButtonIsPressed())
	{
		m_rotationController.OnMotion(x, y);

		// ���������� ����������� ����
		PostRedisplay();
	}
}

void CMyApplication::OnKeyboard(unsigned char key, int /*x*/, int /*y*/)
{
	if (key == ' ')
	{
		if (m_polygonMode == GL_FILL)
		{
			m_polygonMode = GL_LINE;
		}
		else if (m_polygonMode == GL_LINE)
		{
			m_polygonMode = GL_POINT;
		}
		else // polygon mode == GL_POINT
		{
			m_polygonMode = GL_FILL;
		}
		PostRedisplay();
	}
}