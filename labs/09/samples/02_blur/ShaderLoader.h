#pragma once

class CShaderLoader
{
public:
	CShaderLoader(void);
	~CShaderLoader(void);

	// ��������� �������� ��������� ���� ������� �� �����
	GLuint LoadShader(
		GLenum shaderType, 
		const wchar_t * fileName, 
		GLuint shaderId = 0);
	GLuint LoadShader(
		GLenum shaderType, 
		const char * fileName, 
		GLuint shaderId = 0);

	// ��������� �������� ��������� ���� ������� �� �������� ������
	GLuint LoadShader(
		GLenum shaderType, 
		std::istream & stream, 
		GLuint shaderId = 0);
};
