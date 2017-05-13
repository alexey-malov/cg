#include "StdAfx.h"
#include "MyApplication.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"
#include "TextureLoader.h"
#include "ProgramInfo.h"

// ���� ������ �� ���������
const double CMyApplication::FIELD_OF_VIEW = 60;

const double CMyApplication::ZNEAR = 1;

const double CMyApplication::ZFAR = 5;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_mainTextureSamplerLocation(-1)
,m_cloudsTextureSamplerLocation(-1)
,m_rotationController(width, height)
,m_sphere(1, 30, 20)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1);
	glColor3f(1, 1, 1);
	
	glLoadIdentity();
	gluLookAt(
		0, 0, 3, 
		0, 0, 0, 
		0, 1, 0);

	InitShaders();
	LoadTextures();
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	m_sphereTexture.Bind();

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	m_cloudsTexture.Bind();

	// ���������� ��������� ���������
	glUseProgram(m_program);

	// ������������� �������� uniform-���������� mainTexture
	// ��������� ���������� ���� sampler ������ ���� ����� ����������� �����,
	// � �������� ��� ���������
	glUniform1i(m_mainTextureSamplerLocation, 0);
	glUniform1i(m_cloudsTextureSamplerLocation, 1);

	m_sphere.Draw();

	glUseProgram(0);
}

void CMyApplication::LoadTextures()
{
	CTextureLoader loader;
	m_sphereTexture = loader.LoadTexture2D(L"earth.jpg");
	m_cloudsTexture = loader.LoadTexture2D(L"clouds.jpg");
}

void CMyApplication::InitShaders()
{
	// ��������� �������
	CShaderLoader loader;
	CShader vertexShader =
		loader.LoadShader(GL_VERTEX_SHADER, L"texture_mapping.vsh");
	CShader fragmentShader =
		loader.LoadShader(GL_FRAGMENT_SHADER, L"texture_mapping.fsh");

	// ������� ��������� � ������������ � ��� �������
	m_program.Create();
	m_program.AttachShader(vertexShader);
	m_program.AttachShader(fragmentShader);

	// ����������� �������
	CShaderCompiler compiler;
	compiler.CompileShader(vertexShader);
	compiler.CompileShader(fragmentShader);
	compiler.CheckStatus();

	// ��������� ���������
	CProgramLinker linker;
	linker.LinkProgram(m_program);
	linker.CheckStatus();

	// ������� ���������� � ���������
	CProgramInfo programInfo(m_program);
	programInfo.Print(std::cout);

	// �������� � ��������� ������������ uniform-���������� mainTexture
	m_mainTextureSamplerLocation = m_program.GetUniformLocation("mainTexture");
	m_cloudsTextureSamplerLocation = m_program.GetUniformLocation("cloudsTexture");
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

