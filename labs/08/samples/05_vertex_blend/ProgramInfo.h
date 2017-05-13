#pragma once

#include "shaders.h"

class CProgramInfo
{
public:
	CProgramInfo(GLuint program);

	// ���������� �� �������������� ���� ��� ��������� �������������
	static std::string TypeToString(GLenum type);

	// ���������� ���������� uniform-���������� � ���������
	GLuint GetActiveUniforms()const;

	// ���������� ���������� attribute-���������� � ���������
	GLuint GetActiveAttributes()const;

	// ������� ���������� � �������� uniform-���������� � �����
	void PrintUniformInfo(GLuint index, std::ostream & stream)const;
	void PrintAttributeInfo(GLuint index, std::ostream & stream)const;

	void Print(std::ostream & stream)const;

private:
	CProgramInfo(CProgramInfo const&);
	CProgramInfo& operator=(CProgramInfo const&);

	CProgramHandle m_program;
};
