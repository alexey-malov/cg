#include "StdAfx.h"
#include "SkyBox.h"
#include "TextureLoader.h"


/*
����� ������������ ���������� ���������, ��� �������
��� ������ ���������� ���������� ��������� �� ������� ��������� 0-1,
������������ �������� ������� �� ��������� ����� ����������� �����������.
������ ����� ����������� � ������������ ������ OpenGL 1.1, ������������ �
Visual Studio, �� �������� � ���� ���������� � �������������� � ��������� �����
����� ������������ ������������
*/
#ifndef GL_CLAMP_TO_EDGE_EXT
#define GL_CLAMP_TO_EDGE_EXT 0x812F
#endif

CSkyBox::CSkyBox(
	 std::wstring const& leftTexture,
	 std::wstring const& rightTexture,
	 std::wstring const& downTexture,
	 std::wstring const& upTexture,
	 std::wstring const& backTexture,
	 std::wstring const& frontTexture
)
:m_initialized(false)
,m_leftTexture(leftTexture)
,m_rightTexture(rightTexture)
,m_downTexture(downTexture)
,m_upTexture(upTexture)
,m_backTexture(backTexture)
,m_frontTexture(frontTexture)
{
}

void CSkyBox::Draw()const
{
	if (!m_initialized)
	{
		CTextureLoader loader;
		// ������������� ����� "�������������" ���������� ���������,
		// �������������� ����������� ��������� ������ �� ������ ����
		loader.SetWrapMode(GL_CLAMP_TO_EDGE_EXT, GL_CLAMP_TO_EDGE_EXT);

		m_cube.SetTextures(
			loader.LoadTexture2D(m_leftTexture),
			loader.LoadTexture2D(m_rightTexture),
			loader.LoadTexture2D(m_downTexture),
			loader.LoadTexture2D(m_upTexture),
			loader.LoadTexture2D(m_backTexture),
			loader.LoadTexture2D(m_frontTexture)
			);
		m_initialized = true;
	}
	// ������������� ����������� ������ ������������ ����
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// ��������� ������� �������
	glPushMatrix();

	// ��������� ������ ������������� � 4 ������� ������� (�������� 12-14)
	// ���� ������� ���� 0, �� ������� ������ �������� ��������.
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
	modelView[12] = modelView[13] = modelView[14] = 0;
	glLoadMatrixf(modelView);

	// ������ ���
	m_cube.Draw();

	// ��������������� ������� �������
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}
