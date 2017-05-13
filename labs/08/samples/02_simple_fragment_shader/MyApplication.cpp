#include "StdAfx.h"
#include "MyApplication.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
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
	
	// ������ ��������� ������
	glLoadIdentity();
	gluLookAt(
		0, 0, 3, 
		0, 0, 0, 
		0, 1, 0);

	InitShaders();
}

void CMyApplication::InitShaders()
{
	// ������� ��������� ��������
	CShaderLoader loader;
	// � ��������� � ��� ������� ��������� � ����������� �������
	CShader vertexShader = 
		loader.LoadShader(GL_VERTEX_SHADER, "checker.vsh");
	CShader fragmentShader = 
		loader.LoadShader(GL_FRAGMENT_SHADER, "checker.fsh");

	Sleep(1);
	LARGE_INTEGER tick0;
	QueryPerformanceCounter(&tick0);

	// ������� ����������
	CShaderCompiler compiler;
	// � ��������� ���������� ��������
	compiler.CompileShader(vertexShader);
	compiler.CompileShader(fragmentShader);

	LARGE_INTEGER tick1;
	QueryPerformanceCounter(&tick1);

	std::cout << "Compilation duration: " << tick1.QuadPart - tick0.QuadPart << "\n";

	// ������� ��������� � ������������ � ��� �������
	m_program.Create();
	m_program.AttachShader(vertexShader);
	m_program.AttachShader(fragmentShader);

	LARGE_INTEGER tick2;
	QueryPerformanceCounter(&tick2);
	std::cout << "Attach shaders duration: " << tick2.QuadPart - tick1.QuadPart << "\n";

	// ��������� ��������� ������������������ ��������.
	// ���� ���� �� ���� �� �������� ��������������� � �������
	// ����� ��������� ����������
	compiler.CheckStatus();
	// ��� ���������, ������� ���������������� ��� �������

	LARGE_INTEGER tick3;
	QueryPerformanceCounter(&tick3);
	std::cout << "Compilation checking status duration: " << tick3.QuadPart - tick2.QuadPart << "\n";

	// ������� �����������,
	CProgramLinker linker;
	// ��������� ��������� � ��� �������
	linker.LinkProgram(m_program);

	LARGE_INTEGER tick4;
	QueryPerformanceCounter(&tick4);
	std::cout << "Link duration: " << tick4.QuadPart - tick3.QuadPart << "\n";

	// ��������� ��������� ���������������� ��������
	// ���� ��� ���������� �������� ������, ��
	// ����� ��������� ����������
	linker.CheckStatus();
	// ��� ���������

	LARGE_INTEGER tick5;
	QueryPerformanceCounter(&tick5);
	std::cout << "Link checking status duration: " << tick5.QuadPart - tick4.QuadPart << "\n";
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);

	// ������ �������
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(-0.8, -0.8);

		glTexCoord2f(4, 0);
		glVertex2f(0.8, -0.8);

		glTexCoord2f(4, 4);
		glVertex2f(0.8, 0.8);

		glTexCoord2f(0, 4);
		glVertex2f(-0.8, 0.8);
	}
	glEnd();

	glUseProgram(0);
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-aspect, +aspect, -1, 1, 0, 10);
	glMatrixMode(GL_MODELVIEW);
}

