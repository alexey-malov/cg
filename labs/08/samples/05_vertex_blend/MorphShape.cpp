#include "StdAfx.h"
#include "MorphShape.h"


CMorphShape::CMorphShape(size_t numberOfVertices, float maxRadius, float minRadius)
:m_pStartPosArray(NULL)
,m_pEndPosArray(NULL)
,m_numberOfVertices(numberOfVertices)
,m_maxRadius(maxRadius)
,m_minRadius(minRadius)
,m_initialized(false)
{
}

void CMorphShape::Create()const
{
	// �������������� ������ � ����������� � ��������
	std::vector<Vertex> vertices(m_numberOfVertices);
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		Vertex & v = vertices[i];
		// ��������� ��������� ���������� �������
		float angle = static_cast<float>(2 * M_PI * i / m_numberOfVertices);
		float ca = cosf(angle);
		float sa = sinf(angle);

		v.startPos = 
			CVector3f(m_minRadius * ca, m_minRadius * sa, 0);

		// ��������� �������� ���������� �������
		float r2 = m_minRadius + 
			(m_maxRadius - m_minRadius) * (cosf(6 * angle) + 1) / 2;

		v.endPos = 
			CVector3f(r2 * ca, r2 * sa, 0);
	}
	// ������� � ��������� ������� ����� ������
	m_vertexBuffer.Create();
	m_vertexBuffer.BufferData(
		sizeof(Vertex) * m_numberOfVertices,
		&vertices[0],
		GL_STATIC_DRAW
		);

	// ���������� ������ �������� ��������� � �������� ��������� ������
	const GLubyte * pBufferPointer = 
		static_cast<const GLubyte*>(m_vertexBuffer.GetBufferPointer());

	m_pStartPosArray = pBufferPointer + offsetof(Vertex, startPos);
	m_pEndPosArray = pBufferPointer + offsetof(Vertex, endPos);

	// ��������, ��� ������ ��� ������������������
	m_initialized = true;
}

void CMorphShape::SetProgram(
	GLuint program, 
	GLuint phaseLocation, 
	GLuint endPosLocation)
{
	m_program = program;
	m_phaseLocation = phaseLocation;
	m_endPosLocation = endPosLocation;
}

void CMorphShape::Draw(float phase)const
{
	if (!m_program)
	{
		throw std::logic_error("Shader program was not specified");
	}

	if (!m_initialized)
	{
		Create();
	}

	// ���������� ��������� ���������
	glUseProgram(m_program);

	// �������������� uniform-����������
	glUniform1f(m_phaseLocation, phase);

	// ��������� �������� ���������� ������
	m_vertexBuffer.Bind();

	// �������������� ������ �������� ������
	glVertexPointer(
		3, GL_FLOAT, sizeof(Vertex), m_pStartPosArray);
	glVertexAttribPointer(
		m_endPosLocation, 3, GL_FLOAT,
		GL_FALSE, sizeof(Vertex), m_pEndPosArray);

	// ��������� ������������� �������� ������ � ���������
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(m_endPosLocation);

	// ������ ������
	glDrawArrays(GL_LINE_LOOP, 0, m_numberOfVertices);

	// ��������� ������������� �������� ������ � ���������
	glDisableVertexAttribArray(m_endPosLocation);
	glDisableClientState(GL_VERTEX_ARRAY);

	// ��������� ���������
	glUseProgram(0);
}
