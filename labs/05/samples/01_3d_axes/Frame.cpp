#include "StdAfx.h"
#include "Frame.h"

CFrame::CFrame(float size, float endSize)
:m_size(size)
,m_endSize(endSize)
{
}

void CFrame::Draw(void)const
{
	float arrowOffset = m_size * m_endSize;
	float arrowOffset2 = arrowOffset * 0.5;

	glBegin(GL_LINES);
	// ��� X
	{
		glColor3ub(255, 0, 0);

		// ������ ��� X
		glVertex3f(0, 0, 0);
		glVertex3f(m_size, 0, 0);
		// ������ ���������� ��� X
		glVertex3f(m_size, 0, 0);
		glVertex3f(m_size - arrowOffset, -arrowOffset2, 0);
		glVertex3f(m_size, 0, 0);
		glVertex3f(m_size - arrowOffset, +arrowOffset2, 0);
	}
	// ��� Y
	{
		glColor3ub(0, 255, 0);

		// ������ ��� Y
		glVertex3f(0, 0, 0);
		glVertex3f(0, m_size, 0);
		// ������ ���������� ��� Y
		glVertex3f(0, m_size, 0);
		glVertex3f(-arrowOffset2, m_size - arrowOffset, 0);
		glVertex3f(0, m_size, 0);
		glVertex3f(+arrowOffset2, m_size - arrowOffset, 0);
	}
	// ��� Z
	{
		glColor3ub(0, 0, 255);
		// ������ ��� Z
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, m_size);
		// ������ ���������� ��� Z
		glVertex3f(0, 0, m_size);
		glVertex3f(-arrowOffset2, 0, m_size - arrowOffset);
		glVertex3f(0, 0, m_size);
		glVertex3f(+arrowOffset2, 0, m_size - arrowOffset);
	}
	glEnd();
}
