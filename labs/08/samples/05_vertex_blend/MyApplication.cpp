#include "StdAfx.h"
#include "MyApplication.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"
#include "ProgramInfo.h"


const int CMyApplication::NUMBER_OF_VERTICES = 360;

const float CMyApplication::SHAPE_MAX_RADIUS = 1.0f;
const float CMyApplication::SHAPE_MIN_RADIUS = 0.2f;

const float CMyApplication::ANIMATION_PERIOD = 2.0f;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_morphShape(NUMBER_OF_VERTICES, SHAPE_MAX_RADIUS, SHAPE_MIN_RADIUS)
,m_phase(0)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
	glColor3f(0, 0, 0);
	
	glLoadIdentity();
	InitShaders();

	// �������� ������������ uniform � attribute-����������
	GLint phaseLocation = m_program.GetUniformLocation("phase");
	GLint vertex2Location = m_program.GetAttribLocation("endPosition");;
	// �������� ���������� � ��������� � �� ���������� ������
	m_morphShape.SetProgram(m_program, phaseLocation, vertex2Location);
}

void CMyApplication::InitShaders()
{
	// ��������� �������
	CShaderLoader loader;
	CShader vertexShader =
		loader.LoadShader(GL_VERTEX_SHADER, L"vertex_blend.vsh");

	// ������� ��������� � ������������ � ��� �������
	m_program.Create();
	m_program.AttachShader(vertexShader);

	// ����������� �������
	CShaderCompiler compiler;
	compiler.CompileShader(vertexShader);
	compiler.CheckStatus();

	// ��������� ���������
	CProgramLinker linker;
	linker.LinkProgram(m_program);
	linker.CheckStatus();

	// ������� ���������� � ���������
	CProgramInfo programInfo(m_program);
	programInfo.Print(std::cout);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_morphShape.Draw((sinf(m_phase) + 1) /2) ;
}

void CMyApplication::OnIdle()
{
	m_animationController.Tick();

	float delta = m_animationController.GetTimeDelta() * 0.001f;

	m_phase = fmodf(
		float(m_phase + delta * 2 * M_PI / ANIMATION_PERIOD), 
		float(2 * M_PI)
		);

	PostRedisplay();
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-SHAPE_MAX_RADIUS * aspect, SHAPE_MAX_RADIUS * aspect, -SHAPE_MAX_RADIUS, SHAPE_MAX_RADIUS, -1, +1);
	glMatrixMode(GL_MODELVIEW);
}

