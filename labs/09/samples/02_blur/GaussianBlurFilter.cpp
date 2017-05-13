#include "StdAfx.h"
#include "GaussianBlurFilter.h"

CGaussianBlurFilter::CGaussianBlurFilter(void)
:m_generateMipmaps(false)
,m_width(0)
,m_height(0)
,m_stepLocation(-1)
,m_imageLocation(-1)
,m_coefficientsLocation(-1)
,m_filterRadius(0)
{
}

GLuint CGaussianBlurFilter::Blur(
	GLuint texture, 
	GLfloat sigma, 
	int width, int height, 
	bool generateMipmaps)const
{
	// ��������� ���������� ���������������� ������ �����
	PrepareFrameBuffer(width, height, generateMipmaps);

	assert(m_program);
	assert(m_frameBuffer);

	// ��������� �������� � ���������������� ������ �����
	m_frameBuffer.Bind();

	// ������������� ������� ����, � ����� ������� 
	// ������������� � �������������-����
	SetupViewportAndMatrices();

	// ���������� ��������� ���������
	ActivateProgram(sigma);

	// ��������� �������� �� �����������
	BlurHorizontal(texture);
	// ����� ��������� �������� ������������� ����������� �� ���������
	BlurVertical();

	// ��������� ������������� ���������������� ������ �����
	CFrameBuffer::Bind(GL_FRAMEBUFFER_EXT, 0);
	// � ����� ��������� ���������.
	glUseProgram(0);

	// ��������������� ��������� �������� ����� � ������ �������������
	RestoreViewportAndMatrices();

	// ���������� mip-������ ��� �������������� �������� �
	// ������������ � ������������ ������������ ������������
	if (generateMipmaps)
	{
		m_colorBuffer1.Bind();
		glGenerateMipmapEXT(GL_TEXTURE_2D);
	}
	return m_colorBuffer1;
}

void CGaussianBlurFilter::BlurHorizontal(GLuint texture)const
{
	// ��������� �������� � ����������� ����������� �������
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	// ������ ��� ������� �������� �� ��������
	glUniform2f(m_stepLocation, 1.0f / m_width, 0);

	// ������������ 0 ������� ������� �������� m_colorBuffer0 
	// � �������� ������ ����� �� ��������������� ������ �����
	CFrameBuffer::SetTexture2D(
		GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D,
		m_colorBuffer0,
		0);

	assert(
		CFrameBuffer::CheckStatus(GL_FRAMEBUFFER_EXT) == 
		GL_FRAMEBUFFER_COMPLETE_EXT);

	DrawRectangle();
}

void CGaussianBlurFilter::BlurVertical()const
{
	m_colorBuffer0.Bind();
	glUniform2f(m_stepLocation, 0, 1.0f / m_height);

	// ������������ 0 ������� ������� �������� m_colorBuffer1
	// � �������� ������ ����� �� ��������������� ������ �����
	m_frameBuffer.SetTexture2D(
		GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D,
		m_colorBuffer1,
		0);

	assert(
		CFrameBuffer::CheckStatus(GL_FRAMEBUFFER_EXT) == 
		GL_FRAMEBUFFER_COMPLETE_EXT);

	DrawRectangle();
}

void CGaussianBlurFilter::ActivateProgram(float sigma)const
{
	glUseProgram(m_program);
	glUniform1i(m_imageLocation, 0);
	std::vector<float> gauss = CalcGauss(m_filterRadius, sigma);
	glUniform1fv(m_coefficientsLocation, gauss.size(), &gauss[0]);
}

void CGaussianBlurFilter::SetupViewportAndMatrices()const
{
	// ��������� ��������� �������� ����� � ����� ���������
	glPushAttrib(GL_VIEWPORT_BIT);
	// � ������ ������� ���� �������� �� ��������������� ����� �����
	glViewport(0, 0, m_width, m_height);

	// ��������� ������� �������������-���� � ����� ������
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// � ��������� ��������� ������� ������������� ����
	glLoadIdentity();

	// ��������� ������� ������������� � ����� ������
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	// � ������ ����� ������� �������������
	glLoadIdentity();
	glOrtho(0, m_width, m_height, 0, -1, 1);
}

void CGaussianBlurFilter::RestoreViewportAndMatrices()const
{
	// ��������������� ������� �������������
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// ��������������� ������� �������������-����
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// ��������������� ������� ����
	glPopAttrib();
}

void CGaussianBlurFilter::DrawRectangle()const
{
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1);
		glVertex2f(0, 0);
		glTexCoord2f(0, 0);
		glVertex2f(0, m_height);
		glTexCoord2f(1, 0);
		glVertex2f(m_width, m_height);
		glTexCoord2f(1, 1);
		glVertex2f(m_width, 0);
	}
	glEnd();
}

void CGaussianBlurFilter::PrepareFrameBuffer(int width, int height, bool generateMipmaps)const
{
	// ����������, ����� ������� ������ ����� ���������
	// � �������� ��� ����������
	bool createFrameBuffer = !m_frameBuffer;

	bool createTexture = 
		createFrameBuffer || 
		!m_colorBuffer0 || 
		!m_colorBuffer1;

	bool updateTextureImage = 
		createTexture || 
		(m_width != width) || 
		(m_height != height);

	bool updateMipmaps = 
		(m_generateMipmaps != generateMipmaps) || 
		updateTextureImage || 
		createTexture;

	m_generateMipmaps = generateMipmaps;

	// Prepare frame buffer
	if (createFrameBuffer)
	{
		m_frameBuffer.Create();
	}

	// �������������� ������ �����
	if (createTexture)
	{
		m_colorBuffer0.Create();
		m_colorBuffer1.Create();
	}

	m_width = width;
	m_height = height;

	// �������� �������� 0 ������ �����
	if (updateTextureImage || updateMipmaps)
	{
		m_colorBuffer0.Bind();
	}

	if (updateTextureImage)
	{
		// ��� ������� ������ ����� ���������� ����� �������������� � �������
		// ���������� �������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// ��������� 0 ������� ����������� ����������� �����������
		m_colorBuffer0.TexImage(
			0, GL_RGBA, 
			width, height, 
			0, GL_RGBA, 
			GL_UNSIGNED_BYTE, NULL);
	}

	// �������� �������� 1 ������ �����
	if (updateTextureImage || updateMipmaps)
	{
		m_colorBuffer1.Bind();
	}

	if (updateTextureImage)
	{
		// ��������� 0 ������� ����������� ����������� �����������
		m_colorBuffer1.TexImage(
			0, GL_RGBA, 
			m_width, m_height, 
			0, GL_RGBA, 
			GL_UNSIGNED_BYTE, NULL);
	}

	if (updateMipmaps)
	{
		// ��� ������� ������ ������ ����� ���������� ����� ��������������
		// �������� ��������� generateMipmaps
		if (generateMipmaps)
		{
			glTexParameteri(
				GL_TEXTURE_2D, 
				GL_TEXTURE_MIN_FILTER, 
				GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(
				GL_TEXTURE_2D, 
				GL_TEXTURE_MAG_FILTER, 
				GL_LINEAR);
			glGenerateMipmapEXT(GL_TEXTURE_2D);
		}
		else
		{
			glTexParameteri(
				GL_TEXTURE_2D, 
				GL_TEXTURE_MIN_FILTER, 
				GL_NEAREST);
			glTexParameteri(
				GL_TEXTURE_2D, 
				GL_TEXTURE_MAG_FILTER, 
				GL_NEAREST);
		}
	}
}

void CGaussianBlurFilter::SetProgram(
	GLuint program, 
	GLint stepLocation, 
	GLint imageLocation, 
	GLint coefficientsLocation, 
	size_t filterRadius)
{
	assert(program);
	assert(stepLocation >= 0);
	assert(imageLocation >= 0);
	assert(coefficientsLocation >= 0);
	assert(filterRadius > 0 && filterRadius < 20);

	m_program = program;
	m_stepLocation = stepLocation;
	m_imageLocation = imageLocation;
	m_coefficientsLocation = coefficientsLocation;
	m_filterRadius = filterRadius;
}

std::vector<float> CGaussianBlurFilter::CalcGauss(size_t size, float sigma)
{
	std::vector<float> gauss(size);

	// ��������� ������������ �������� �������������
	const float k1 = 1.0f / sqrtf(2 * M_PI * sigma * sigma);
	const float k2 = 1.0f / (2 * sigma * sigma);

	float sum = 0;

	for (size_t i = 0; i < size; ++i)
	{
		// ��������� ��������� ����������� � ������ �����
		float k = k1 * expf(-k2 * (i * i));
		gauss[i] = k;

		sum += k;

		// ��������� �������������� ������� ������������� 
		// ������������ ��� x=0
		// (����� ������������� �� ���������� ���������� �������������
		// ������� � ��������� ��������� ���������� �� ���� �����������)
		if (i > 0)
		{
			sum += k;
		}
	}

	// ��������� ������������ ������������� �������
	sum = 1.0f / sum;
	for (size_t i = 0; i < size; ++i)
	{
		gauss[i] *= sum;
	}

	return gauss;
}