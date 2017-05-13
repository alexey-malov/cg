#pragma once

#include "Vector3.h"

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
	CCube(float size = 1);
	void Draw()const;
	void DrawShadowVolume(CVector3f const& lightPosition, float extrusionFactor)const;
	void SetSideColor(CubeSide side, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255);
private:
	// ������ ������� ������� �������� ������
	void DrawShadowVolumeSides(CVector3f const& lightPosition, float extrusionFactor)const;

	// ������ ���� � ��� �������� ������
	void DrawShadowVolumeCaps(CVector3f const& lightPosition, float extrusionFactor)const;

	// ��������� ������������� ���������� � ������
	static void InitEdges();

	// ������ ����
	float m_size;

	// ����� ������ ����
	GLubyte m_sideColors[6][4];

	// ������ ������ ����
	static const float m_vertices[8][3];

	// ������ ������ ����
	static const unsigned short m_faces[6][4];
	static const size_t m_faceCount;

	// ���������� � ����� ������������� �����
	struct Edge
	{
		// ������� ������, ������������ �����
		short vStart, vEnd;

		// ������� ����������� � ����� ������
		// frontFaceNormal - ������� �����, � ������ ������� ������� 
		// ����� ������ � ������������������ vStart, vEnd
		// backFaceNormal - ������� �����, � ������ ������� �������
		// ����� ����� � ������������������ vEnd, vStart
		CVector3f frontFaceNormal;
		CVector3f backFaceNormal;

		// ���� �� ������ ��������� ����������� �����
		// (������������ ��� ����� ���������� � ������)
		bool backFaceNormalIsDefined;
	};

	// ������ ����� ����
	static std::vector<Edge> m_edges;
};
