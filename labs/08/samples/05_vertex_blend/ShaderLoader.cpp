#include "StdAfx.h"
#include "ShaderLoader.h"
#include "Shaders.h"

CShaderLoader::CShaderLoader(void)
{
}

CShaderLoader::~CShaderLoader(void)
{
}


GLuint CShaderLoader::LoadShader(
	GLenum shaderType, 
	std::istream & stream, 
	GLuint shaderId)
{
	// �������� ������ �� ��������� ������ � ��������� �����
	std::stringstream strStream;
	strStream << stream.rdbuf();

	// �������� ������, ���������� �������� ��� �������
	std::string shaderSource = strStream.str();

	CShader shader(shaderId);
	// ������� ����� ������ 
	// (���� � ��� ��� ������� ������� ������������� �������)
	if (!shader)
	{
		shader.Create(shaderType);
	}
	shader.SetSource(shaderSource.c_str());

	// ���������� ������������� ���������� �������
	return shader.Detach();
}


GLuint CShaderLoader::LoadShader(GLenum shaderType, const char * fileName, GLuint shaderId)
{
	std::ifstream stream(fileName);
	if (!stream.is_open())
	{
		throw std::runtime_error("Can't open shader file");
	}
	return LoadShader(shaderType, stream, shaderId);
}


GLuint CShaderLoader::LoadShader(GLenum shaderType, const wchar_t * fileName, GLuint shaderId)
{
	std::ifstream stream(fileName);
	if (!stream.is_open())
	{
		throw std::runtime_error("Can't open shader file");
	}
	return LoadShader(shaderType, stream, shaderId);
}