#include "StdAfx.h"
#include "MyApplication.h"
#include "Matrix4.h"

const double CMyApplication::FIELD_OF_VIEW = 60;
const double CMyApplication::ZNEAR = 1;
const double CMyApplication::ZFAR = 10;
const double CMyApplication::X_ROTATION_SPEED = 43;
const double CMyApplication::Y_ROTATION_SPEED = -37;
const double CMyApplication::Z_ROTATION_SPEED = 29;

const GLuint CMyApplication::FRAME_BUFFER_WIDTH = 800;
const GLuint CMyApplication::FRAME_BUFFER_HEIGHT = 600;


CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_frameBuffer(0)
,m_depthBuffer(0)
,m_windowWidth(width)
,m_windowHeight(height)
,m_rotationController(width, height)
{
	// ��������� ���������� �������� � ������ ���������� ����������
	AddEventListener(&m_rotationController);

	// � �� �� �����, ����� ������� ������� �� ����������� ��������
	m_rotationController.AddEventListener(this);
}

CMyApplication::~CMyApplication()
{
	// ������� ����� ������� ���������������� ������ �����
	if (m_depthBuffer)
	{
		glDeleteRenderbuffersEXT(1, &m_depthBuffer);
	}

	// ������� ��������������� ����� �����
	if (m_frameBuffer)
	{
		glDeleteFramebuffersEXT(1, &m_frameBuffer);
	}
}

void CMyApplication::OnRotationControllerUpdate()
{
	// �������������� ����� ��� ���������� 
	// ��������� ����������� ��������
	PostRedisplay();
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);
	
	// ��������� ������� ������ � ������� �������������-����
	// ���������������� ������ �����
	m_modelViewFrameBuffer.LoadLookAtRH(
		0, 0, 5,
		0, 0, 0,
		0, 1, 0
		);

	// �������������� ������� ������������� ���� ��������� ������ �����
	CMatrix4d modelView;
	modelView.LoadLookAtRH(
		0, 0, 4,
		0, 0, 0,
		0, 1, 0
		);
	// � ��������� �� � ���������� ��������
	m_rotationController.SetModelViewMatrix(modelView);

	// �������������� ��������������� ����� �����
	InitFrameBuffer();
}

void CMyApplication::OnDisplay()
{
	// ������ ������ � ��������
	DrawObjectToTexture();

	// ������������� � ��������� ������ �����
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// ������� �������� ����� �����
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ��������� ������� ������������� � ������� ����
	SetupViewportAndProjectionMatrix(
		m_windowWidth, m_windowHeight, 
		FIELD_OF_VIEW, ZNEAR, ZFAR);

	// ��������� ������� �������������-����, ������� ��
	// � ����������� ��������
	glLoadMatrixd(m_rotationController.GetModelViewMatrix());

	// �������� ��������, ������������ � �������� ������ �����
	// �� ��������������� ������ �����
	glEnable(GL_TEXTURE_2D);
	m_colorBufferTexture.Bind();

	// ������ �������������
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(-2, -1.5);

		glTexCoord2f(1, 0);
		glVertex2f(2, -1.5);

		glTexCoord2f(1, 1);
		glVertex2f(2, 1.5);

		glTexCoord2f(0, 1);
		glVertex2f(-2, 1.5);
	}
	glEnd();
}

void CMyApplication::DrawObjectToTexture()const
{
	// ������������� � ����������� ������� � ��������������� 0
	// � ��������� ��������� ��������
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// ������ ��������������� ����� ��������
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer);

	// ������� ��������� � ��� ������ ����� � �������
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ����������� ������� ������������� ��� ����������
	// �� ��������������� ����� �����
	SetupViewportAndProjectionMatrix(
		FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 
		FIELD_OF_VIEW, 
		ZNEAR, ZFAR);

	// ��������� ������� ������������� ���� ��� 
	// ��������� �� ��������������� �����
	glLoadMatrixd(m_modelViewFrameBuffer);

	// ������ ��� �� ��������������� �����
	glutWireTorus(0.2, 2, 20, 50);

	// ������������� � ��������, �������������� � ������ �����
	// ���������������� ������ �����
	m_colorBufferTexture.Bind();
	// � ���������� mip-������
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}

void CMyApplication::OnIdle()
{
	m_animationController.Tick();
	double delta = m_animationController.GetTimeDelta() * 0.001;

	// ������������ �������� �������, ���������� �� ��������������� 
	// ����� �����, ������ ���� X, Y � Z
	m_modelViewFrameBuffer.Rotate(delta * X_ROTATION_SPEED, 1, 0, 0);
	m_modelViewFrameBuffer.Rotate(delta * Y_ROTATION_SPEED, 0, 1, 0);
	m_modelViewFrameBuffer.Rotate(delta * Z_ROTATION_SPEED, 0, 0, 1);
	m_modelViewFrameBuffer.Normalize();

	// ����������� ����������� �����
	PostRedisplay();
}

void CMyApplication::InitFrameBuffer()
{
	// �������������� ��������, ������������ ��� �������� 
	// ������ ����� ���������������� ������ �����
	m_colorBufferTexture.Create();
	m_colorBufferTexture.Bind();
	glTexParameteri(
		GL_TEXTURE_2D, 
		GL_TEXTURE_MIN_FILTER, 
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(
		GL_TEXTURE_2D, 
		GL_TEXTURE_MAG_FILTER, 
		GL_LINEAR);

	// ������ ������� ������������ ���������� ��� ��������
	// (��� ��������� �� ������� OpenGL)
	if (GL_EXT_texture_filter_anisotropic)
	{
		GLint maxAnisotropy = 0;
		glGetIntegerv(
			GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, 
			&maxAnisotropy);
		glTexParameteri(
			GL_TEXTURE_2D, 
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 
			maxAnisotropy);
	}

	// ������� �������� ��������� �������, �� ������������� �� ����������
	// (�������� pData) ����� NULL
	m_colorBufferTexture.TexImage(
		0, 
		GL_RGBA, 
		FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 
		0, GL_RGBA, 
		GL_UNSIGNED_BYTE, NULL);
	// ������������� ���������� ������ ����������� ��� ��������
	// (������ ������� ����� � ������ ���������� GL_EXT_framebuffer_object)
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	// ���������� ��� ��� ������ �����
	glGenFramebuffersEXT(1, &m_frameBuffer);
	// ��������� �������� �� ���������������� ������ �����
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBuffer);
	// ������������ 0 ������� ����������� �������� � �������� ������ ����� �0
	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT,			// ��� ������ �����
		GL_COLOR_ATTACHMENT0_EXT,	// ����� �������������
		GL_TEXTURE_2D,				// ��� ��������
		m_colorBufferTexture,		// ���������� ������
		0);							// mip-�������

	// ���������� ��� ��� ������ ������������
	glGenRenderbuffersEXT(1, &m_depthBuffer);
	// ��������� �������� � ������ ������������
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer);
	// �������� � ������ ������������ ������ ��� �������� 
	// ����������� ������� �������� �������
	glRenderbufferStorageEXT(
		GL_RENDERBUFFER_EXT, 
		GL_DEPTH_COMPONENT, 
		FRAME_BUFFER_WIDTH, 
		FRAME_BUFFER_HEIGHT);
	// ������������ ����� ������������ � �������� ������ �������
	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT,			// ���� ������������� ������ �����
		GL_DEPTH_ATTACHMENT_EXT,	// ����� �������������
		GL_RENDERBUFFER_EXT,		// ���� ������������� ������ ������������
		m_depthBuffer);				// ����� ������������

	// ��������� ������ ������ �����
	GLenum frameBufferStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	// ����� ����� ������ ���� ������ 
	// (��������� ����������� ����� �����������)
	if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		throw std::logic_error("Framebuffer is incomplete");
	}

	// ������������� �� ������������� ��������� ������ �����
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CMyApplication::OnReshape(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}

void CMyApplication::SetupViewportAndProjectionMatrix(
	int width, int height, 
	double fov, double znear, double zfar)
{
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, znear, zfar);
	glMatrixMode(GL_MODELVIEW);
}

