#pragma once

#include "Shaders.h"
#include "FrameBuffer.h"
#include "Texture.h"

class CGaussianBlurFilter
{
public:
	CGaussianBlurFilter(void);

	// �������� ������� �������� � ���������, ����������� �������� ��������
	void SetProgram(
		GLuint program,				// ���������
		GLint stepLocation,			// ������������ ���������� step
		GLint imageLocation,		// ������������ ���������� image
		GLint coefficientsLocation, // ������������ ������� �������������
		size_t filterRadius);		// ������ �������

	// ��������� �������� ����������� � ���������� ��������
	// � ���������� ������������� ����������� ������� � �����������
	// ������������� ��������, ������������ � �������� ����������,
	// �������� ������ ������� ������
	GLuint Blur(
		GLuint texture,						// ���������� ������
		GLfloat sigma,						// �������������������� ����������
		int width, int height,				// ������� ��������
		bool generateMipmaps = true)const;	// ������������� ������������� mip-�������
private:
	// ��������� ������������ �������� �������
	static std::vector<float> CalcGauss(size_t size, float sigma);
	
	// �������������� ��������������� ����� ����� ��� 
	// ��������� ����������� ��������� �������
	void PrepareFrameBuffer(
		int width, 
		int height, 
		bool generateMipmaps = true)const;

	// ������ ���������������� �������������
	void DrawRectangle()const;

	// ��������� ��������� ������ � �������� �����
	void SetupViewportAndMatrices()const;

	// ��������������� ��������� �������� ����� � �������
	void RestoreViewportAndMatrices()const;

	// ������������ ��������� � ������������� �� uniform-����������
	void ActivateProgram(float sigma)const;

	// ��������� �������� �� �����������
	void BlurHorizontal(GLuint texture)const;

	// ��������� �������� �� ���������
	void BlurVertical()const;
	
	// ����������� ������
	CProgramHandle m_program;

	// ������������ ����������, �������� ��� ������� ������ �� ��������
	GLint m_stepLocation;

	// ������������ ����������, �������� ������������ ����������� �������
	GLint m_imageLocation;	

	// ������������ ������� ������������� �������
	GLint m_coefficientsLocation;

	// ������ �������
	size_t m_filterRadius;

	// ����, �������� ������������� ������������� mip-������� ��������
	mutable bool m_generateMipmaps;

	// ��������������� ����� �����
	mutable CFrameBuffer m_frameBuffer;
	// ��������, ������������ ��� �������� �����������, �������� �� �����������
	mutable CTexture2D m_colorBuffer0;
	// ��������, ������������ ��� �������� ���������� �����������
	mutable CTexture2D m_colorBuffer1;

	// ������ � ������ ��������
	mutable unsigned m_width;
	mutable unsigned m_height;
};
