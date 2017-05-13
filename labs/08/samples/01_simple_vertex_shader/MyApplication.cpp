#include "StdAfx.h"
#include "MyApplication.h"

// ���� ������ �� ���������
const double CMyApplication::FIELD_OF_VIEW = 60;

// ���������� �� ������� ��������� ���������� ������
const double CMyApplication::ZNEAR = 1;

// ���������� �� ������� ��������� ���������� ������
const double CMyApplication::ZFAR = 5;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_rotationController(width, height)
,m_program(0)
{
}

CMyApplication::~CMyApplication(void)
{
	if (m_program)
	{
		glDeleteProgram(m_program);
	}
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
	
	glLoadIdentity();
	gluLookAt(
		0, 0, 3, 
		0, 0, 0, 
		0, 1, 0);

	InitShaders();
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(m_program);

	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutSolidTorus(0.2, 0.8, 20, 40);

	glUseProgram(0);
}

void CMyApplication::InitShaders()
{
	// ������� ��������� ������
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	// ������� ���������
	m_program = glCreateProgram();

	// ������������ � ��� ������
	glAttachShader(m_program, vertexShader);

	// ��������� �������� ��� ������� � ������
	GLchar const shaderSource[] = 
		"void main()\n"
		"{\n"
		"	gl_Position = ftransform();\n"
		"	gl_FrontColor = (gl_Position + vec4(1.0)) * 0.5;\n"
		"}";
	GLchar const * pShaderSource = shaderSource;
	GLchar const ** ppShaderSource = &pShaderSource;
	// � ��������� �������� ��� �������
	glShaderSource(vertexShader, 1, ppShaderSource, NULL);

	// ����������� ������
	glCompileShader(vertexShader);

	// �������� ������ ������������������ �������
	GLint compileStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		//glDeleteShader(vertexShader);
		throw std::runtime_error("Cannot compile shader");
	}

	// ��������� ���������
	glLinkProgram(m_program);

	// ����������� ������ �������� ���������� ���������
	GLint linkStatus;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus != GL_TRUE)
	{
		glDeleteShader(vertexShader);
		glDeleteProgram(m_program);
		throw std::runtime_error("Cannot link program");
	}

	// ������ ����� �������� �� �������� ����� ���������� ���������
	// (� ���� ����� ������������� ��� � ���)
	// �� ����� ������ ����� ����, ��� ����� ���������� �� ���)
	glDeleteShader(vertexShader);

	// ����� ����, ��� ��������� ������������, ����� ����������� 
	// �� ��� ������, �.�. �� ��� ����� ���� �� ����� �� ����������
	glDetachShader(m_program, vertexShader);
	// ����� ������������ �� ���������, ������ ����� ������, �.�.
	// �� �������� ��� �� �������� ��� ������ glDeleteShader
}

void CMyApplication::OnReshape(int width, int height)
{
	m_rotationController.ResizeWindow(width, height);

	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	m_rotationController.OnMouse(button, state, x, y);
}

void CMyApplication::OnMotion(int x, int y)
{
	if (m_rotationController.LeftButtonIsPressed())
	{
		m_rotationController.OnMotion(x, y);
		PostRedisplay();
	}
}
