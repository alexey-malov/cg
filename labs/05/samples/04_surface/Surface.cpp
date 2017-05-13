#include "StdAfx.h"
#include "Surface.h"

CSurface::CSurface(int columns, int rows, float xMin, float xMax, float yMin, float yMax)
:m_rows(rows)
,m_columns(columns)
,m_xMin(xMin)
,m_xMax(xMax)
,m_yMin(yMin)
,m_yMax(yMax)
,m_displayList(0)
{
}

void CSurface::Draw()const
{
	// ��� ������ ��������� � ������ ������� ������� ������� 
	// ��������� ����������� � ���������� ������
	if (m_displayList == 0)
	{
		m_displayList = glGenLists(1);
		glNewList(m_displayList, GL_COMPILE);

		// ��������� ��� ����� �����
		const float dy = (m_yMax - m_yMin) / (m_rows - 1);
		const float dx = (m_xMax - m_xMin) / (m_columns - 1);

		float y = m_yMin;
		// ��������� �� ������� �����
		for (int row = 0; row < m_rows - 1; ++row, y += dy)
		{
			// ������ ������ ����� ��������������� ���� ����� �� �������������
			glBegin(GL_TRIANGLE_STRIP);
			float x = m_xMin;

			// ��������� �� �������� ������� ������
			for (int column = 0; column <= m_columns; ++column, x += dx)
			{
				// ��������� ��������� ������� � ����� ���� �������� ������
				// ����� �� �������������
				Vertex v0 = CalculateVertex(x, y + dy);
				Vertex v1 = CalculateVertex(x, y);

				// ������ ������� � ���������� ������� �� ������ �������
				glNormal3f(v0.nx, v0.ny, v0.nz);
				glVertex3f(v0.x, v0.y, v0.z);

				// ������ ������� � ���������� ������� �� �������� �������
				glNormal3f(v1.nx, v1.ny, v1.nz);
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}

		glEndList();
	}

	// ������� ����� ���������� ���������� ������ ������ ���������
	// �����
	glCallList(m_displayList);
}

Vertex CSurface::CalculateVertex(double x, double y)const
{
	Vertex result = 
	{
		float(x), float(y), 0, 
		0, 0, 1
	};
	return result;
}

// �� ������ ������� ���������� ������ ��� ���������� ���������� ������
CSurface::~CSurface(void)
{
	if (m_displayList != 0)
	{
		glDeleteLists(m_displayList, 1);
	}
}
