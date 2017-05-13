#include "StdAfx.h"
#include "MyApplication.h"
#include "TextureLoader.h"

// ���������� �� ������� ��������� ��������� ������������� ������
const double CMyApplication::ZNEAR = 0.5;
// ���������� �� ������ ��������� ��������� ������������� ������
const double CMyApplication::ZFAR = 10;
// ���� ������ �� ���������
const double CMyApplication::FIELD_OF_VIEW = 60;

const float CMyApplication::ROTATION_SPEED = 40;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_rotationAngle(0)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	glewInit();
	// ��������� ������� ���������� GL_ARB_multitexture
	if (!GLEW_ARB_multitexture)
	{
		throw std::runtime_error("GL_ARB_multitexture extension is not supported");
	}

	CTextureLoader textureLoader;
	m_car.Attach(textureLoader.LoadTexture2D(L"car.jpg"));
	m_smile.Attach(textureLoader.LoadTexture2D(L"smile.png"));

	textureLoader.SetWrapMode(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	m_sun.Attach(textureLoader.LoadTexture2D(L"sun.png"));

	m_medved.Attach(textureLoader.LoadTexture2D(L"medved.png"));

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, -2, 4, 0, 0, 0, 0, 1, 0);

	/*
	������ �������� ��� ����������� ����� �0 
	*/
	glActiveTexture(GL_TEXTURE0);
	m_car.Bind();

	// �������� ��������������� ������ � ������ ���������� �����
	// � ������ �������������
	{
		glActiveTexture(GL_TEXTURE0);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();

		glTranslatef(-1.5f, 1.5f, 0);
		glColor3f(0.5, 1, 1);

		DrawRectangle();

		glPopMatrix();
	}

	// �������� ��������������� � ������ � ������ ���������� ������
	// � ������ �������������
	// ������ ���������� ���� �������� � ������ GL_DECAL
	{
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		m_medved.Bind();
		glPushMatrix();
		glTranslatef(+1.5f, 1.5f, 0);
		DrawRectangle();
		glPopMatrix();
	}

	// �������� ��������������� � ����� ���������� ������
	// � ������ �������������
	// ������ ���������� ���� �������� � ������ GL_MODULATE
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		m_smile.Bind();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(-1.5f, -1.0f, 0);
		DrawRectangle();
		glPopMatrix();
	}

	/*
	������ �������������, �������� � ������ �� ���������� ������
	������������� ���������� ���������
	*/
	{
		glActiveTexture(GL_TEXTURE1);
		m_sun.Bind();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// ��������� ������� ModelView � ����� ������ � ���������
		// ������������� ����� ������� 1.5, -1.0
		glPushMatrix();
		glTranslatef(1.5f, -1.0f, 0);

		// ���������� ���������� ���� �1 � ������ ��� ���� �������
		// ������������� ���������� ���������
		glActiveTexture(GL_TEXTURE1);
		glMatrixMode(GL_TEXTURE);
		// ��������� ������� ������� � ����� ������
		// (� ������ ���������� ����� - ���� ���� ������)
		glPushMatrix();	
		glLoadIdentity();
		// ������� ������ ��� Z, ���������� ����� ����� (0.5, 0.5, 0) 
		glTranslatef(0.5, 0.5, 0);
		glRotatef(m_rotationAngle, 0, 0, 1);
		glTranslatef(-0.5, -0.5, 0);

		DrawRectangle();

		// ��������������� �������
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

void CMyApplication::OnIdle()
{
	m_animationController.Tick();

	m_rotationAngle += m_animationController.GetTimeDelta() * 0.001 * ROTATION_SPEED;
	m_rotationAngle = fmodf(m_rotationAngle, 360);

	Sleep(10);
	PostRedisplay();
}

void CMyApplication::DrawRectangle()const
{
	/*
	������ �������������, �������� ��� ������ �� ��� ������ ���������� ����������
	Y
	|
	0---|---3
	|   |   |
	|   |   |
	-----+------X
	|   |   |
	|   |   |
	1---|---2
	|
	*/
	glBegin(GL_QUADS);
	{
		// ������� ����� ����
		glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
		glMultiTexCoord2f(GL_TEXTURE1, 0, 0);
		glVertex3f(-1, 1, 0);

		// ������ ����� ����
		glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
		glMultiTexCoord2f(GL_TEXTURE1, 0, 1);
		glVertex3f(-1, -1, 0);

		// ������ ������ ����
		glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
		glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
		glVertex3f(1, -1, 0);

		// ������� ������ ����
		glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
		glMultiTexCoord2f(GL_TEXTURE1, 1, 0);
		glVertex3f(1, 1, 0);
	}	
	glEnd();
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
}
