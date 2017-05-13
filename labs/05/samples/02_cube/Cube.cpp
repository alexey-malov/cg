#include "StdAfx.h"
#include "Cube.h"

CCube::CCube(float size)
:m_size(size)
{
	SetSideColor(CS_NEGATIVE_X, 255, 255, 255);
	SetSideColor(CS_POSITIVE_X, 255, 255, 255);
	SetSideColor(CS_NEGATIVE_Y, 255, 255, 255);
	SetSideColor(CS_POSITIVE_Y, 255, 255, 255);
	SetSideColor(CS_NEGATIVE_Z, 255, 255, 255);
	SetSideColor(CS_POSITIVE_Z, 255, 255, 255);
}

void CCube::Draw()const
{

	/*

	   Y
	   |
	   |
	   |
	   +---X
	  /
	 /
	Z
	   3----2
	  /    /|
	 /    / |
	7----6  |
	|  0 |  1
	|    | /
    |    |/
	4----5
	*/
	// ������ ��������� ������
	static const float vertices[8][3] = 
	{
		{-1, -1, -1},	// 0
		{+1, -1, -1},	// 1
		{+1, +1, -1},	// 2
		{-1, +1, -1},	// 3
		{-1, -1, +1},	// 4
		{+1, -1, +1},	// 5
		{+1, +1, +1},	// 6
		{-1, +1, +1},	// 7
	};

	// ������ ��������� ������ (� �������, ����������� � 
	// �������� ���������� �� � ������� ������)
	// ������� ������ ������ ������������� � ������� �� ������
	// ������ ������� ������� (���� �������� �� ����� �������)
	static const unsigned char faces[6][4] = 
	{
		{4, 7, 3, 0},	// ����� x<0
		{5, 1, 2, 6},	// ����� x>0
		{4, 0, 1, 5},	// ����� y<0
		{7, 6, 2, 3},	// ����� y>0
		{0, 3, 2, 1},	// ����� z<0
		{4, 5, 6, 7},	// ����� z>0
	};
	static size_t const faceCount = sizeof(faces) / sizeof(*faces);

	// ��������� ������� ������� �������������-���� � ����� ������
	// �.�. ��������� ������� ��� ����� �������������� ��� ������ glScale
	glPushMatrix();
	// ������ ��������������� ������ ������
	glScalef(m_size * 0.5f, m_size * 0.5f, m_size * 0.5f);

	glBegin(GL_QUADS);
	{
		for (size_t face = 0; face < faceCount; ++face)
		{
			// ������������� ���� �����
			glColor4ubv(m_sideColors[face]);

			// ������ ��������������� �����, ���������� �� �������
			for (size_t i = 0; i < 4; ++i)
			{
				size_t vertexIndex = faces[face][i];
				glVertex3fv(vertices[vertexIndex]);
			}
		}
	}
	glEnd();

	// ��������������� ������� ������������� ���� �� ����� ������
	glPopMatrix();
}


void CCube::SetSideColor(CubeSide side, GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
	if (side < CS_MIN_CUBE_SIDE_INDEX || side > CS_MAX_CUBE_SIDE_INDEX)
	{
		throw std::invalid_argument("Invalid cube side");
	}

	m_sideColors[side][0] = r;
	m_sideColors[side][1] = g;
	m_sideColors[side][2] = b;
	m_sideColors[side][3] = a;
}
