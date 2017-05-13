#pragma once

#include "Buffer.h"
#include "Vector3.h"
#include "Shaders.h"

struct Vertex
{
	CVector3f startPos;
	CVector3f endPos;
};

class CMorphShape : public boost::noncopyable
{
public:
	CMorphShape(size_t numberOfVertices, float maxRadius, float minRadius);

	// �������� ������ � ��������� � ������������ �� ����������
	void SetProgram(GLuint program, GLuint phaseLocation, GLuint endPosLocation);

	// ������ ����
	void Draw(float phase)const;
private:
	// ������������� ������
	void Create()const;

	// ���������� ������ � ������
	size_t const m_numberOfVertices;

	// ����������� � ������������ ������ ���������
	float const m_maxRadius;
	float const m_minRadius;

	// ��������� ����� � ��������� �� ����������� � ��� �������
	mutable bool m_initialized;
	mutable CVertexBuffer m_vertexBuffer;
	mutable GLubyte const * m_pStartPosArray;
	mutable GLubyte const * m_pEndPosArray;

	CProgramHandle m_program;
	GLuint m_phaseLocation;
	GLuint m_endPosLocation;
};
