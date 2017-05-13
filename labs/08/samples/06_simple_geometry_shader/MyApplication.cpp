#include "StdAfx.h"
#include "MyApplication.h"
#include "ShaderLoader.h"
#include "ShaderCompiler.h"
#include "ProgramLinker.h"
#include "TextureLoader.h"

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	InitShaders();
	InitTextures();
}

void CMyApplication::InitTextures()
{
	// ��������� ��������
	CTextureLoader loader;
	loader.BuildMipmaps(true);
	loader.SetMagFilter(GL_LINEAR);
	loader.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	m_texture = loader.LoadTexture2D(L"car.jpg");
}

void CMyApplication::InitShaders()
{
	// ��������� ��������� �������������� �������� �����������
	if (!GLEW_EXT_geometry_shader4)
	{
		throw std::runtime_error(
			"The OpenGL implementation does not support geometry shaders");
	}


	CShaderLoader loader;
	// ��������� �������
	m_vertexShader = 
		loader.LoadShader(GL_VERTEX_SHADER, "shaders\\vertex_shader.vsh");
	m_fragmentShader = 
		loader.LoadShader(GL_FRAGMENT_SHADER, "shaders\\fragment_shader.fsh");
	m_geometryShader = 
		loader.LoadShader(GL_GEOMETRY_SHADER, "shaders\\geometry_shader.gsh");

	// ������� ����������� ������ � ������������ ������� � ����
	m_program.Create();
	m_program.AttachShader(m_vertexShader);
	m_program.AttachShader(m_fragmentShader);
	m_program.AttachShader(m_geometryShader);

	// ����������� �������
	CShaderCompiler compiler;
	compiler.CompileShader(m_vertexShader);
	compiler.CompileShader(m_geometryShader);
	compiler.CompileShader(m_fragmentShader);
	compiler.CheckStatus();

	// ������ ��������� ��������� ���������
	// ��� ������� ����������: �����
	m_program.SetParameter(GL_GEOMETRY_INPUT_TYPE_ARB, GL_POINTS);
	// ���� �������� ����������: ����� �� ��������������
	m_program.SetParameter(GL_GEOMETRY_OUTPUT_TYPE_ARB, GL_TRIANGLE_STRIP);
	// ������������ ���������� ������, ����������� �������������� ��������
	// �� ���� �����. ��� ����� �� ���� ������������� ��� ����� 4
	m_program.SetParameter(GL_GEOMETRY_VERTICES_OUT_EXT, 4);

	// ��������� ��������� � ��������� �� ������
	CProgramLinker linker;
	linker.LinkProgram(m_program);
	linker.CheckStatus();

	// �������� ������������ uniform-����������, ������������ � 
	// ��������� ���������
	m_sizeLocation = m_program.GetUniformLocation("Size");
	m_textureMapLocation = m_program.GetUniformLocation("TextureMap");
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// ���������� ��������� ��������� � ������ �������� �� uniform-����������
	glUseProgram(m_program);
	if (m_sizeLocation >= 0)
	{
		// ������� ����� � ��������������� �����������
		glUniform2f(m_sizeLocation, 0.6f, 0.6f);
	}
	if (m_textureMapLocation >= 0)
	{
		// ������ ����� ����������� ������ 0 ��� ������������� ���������������
		// TextureMap
		glUniform1i(m_textureMapLocation, 0);
	}

	// ������������� � ����������� �������� � 0 ���������� ������
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// ������ ��� ����� � ���� ���������������� ���������������
	glBegin(GL_POINTS);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0.5, 0.5);
	glEnd();

	// ������������� �� ����������� ��������
	glUseProgram(0);
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);
}