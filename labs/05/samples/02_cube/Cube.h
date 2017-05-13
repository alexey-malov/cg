#pragma once

class CCube
{
public:
	enum CubeSide
	{
		CS_NEGATIVE_X,
		CS_POSITIVE_X,
		CS_NEGATIVE_Y,
		CS_POSITIVE_Y,
		CS_NEGATIVE_Z,
		CS_POSITIVE_Z,

		CS_MIN_CUBE_SIDE_INDEX = CS_NEGATIVE_X,
		CS_MAX_CUBE_SIDE_INDEX = CS_POSITIVE_Z,
	};
public:
	// ������� ��� � �������� ������ �������
	CCube(float size = 1);
	// ������ ���
	void Draw()const;
	// ������ ���� ������� ����
	void SetSideColor(CubeSide side, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255);
private:
	float m_size;

	// ����� ������ ����
	GLubyte m_sideColors[6][4];
};
