#include "StdAfx.h"
#include "Flower.h"

CFlower::CFlower(float minRadius, float maxRadius, int petals, COLORREF centerColor, COLORREF petalColor)
:m_minRadius(minRadius)
,m_maxRadius(maxRadius)
,m_petals(petals)
,m_centerColor(centerColor)
,m_petalColor(petalColor)
,m_displayList(0)
{
}

CFlower::CFlower(CFlower const& f)
:m_minRadius(f.m_minRadius)
,m_maxRadius(f.m_maxRadius)
,m_petals(f.m_petals)
,m_centerColor(f.m_centerColor)
,m_petalColor(f.m_petalColor)
,m_displayList(0)	// ���������� ������ � ����� ����� ������ ����
{
}

CFlower::~CFlower(void)
{
	if (m_displayList)
	{
		glDeleteLists(m_displayList, 1);
		m_displayList = 0;
	}
}

void CFlower::Draw()const
{
	// ���� ���������� ������ ��� �� ��� ������, �� ��� ������ ������� ���
	if (!m_displayList)
	{
		// ���������� ����� ���������� ������ ��� ������
		m_displayList = glGenLists(1);

		{
			glNewList(m_displayList, GL_COMPILE);

			static const float M_PI = 3.14159265358979323846f;

			// ������� ��� (��� ��������� ������� �������� 
			// ��� ����������� ��� ������� 20 ������)
			const float step = 2 * M_PI / (m_petals * 20);

			glBegin(GL_TRIANGLE_FAN);

			// ����������� ������� ����� ����� ���� centerColor
			// ������� 3ub ��������, ��� ������� ��������� 3 ��������� ���� unsigned byte
			glColor3ub(GetRValue(m_centerColor), GetGValue(m_centerColor), GetBValue(m_centerColor));
			glVertex2f(0, 0);

			// ������� �������� ����� ����� ���� petalColor
			glColor3ub(GetRValue(m_petalColor), GetGValue(m_petalColor), GetBValue(m_petalColor));

			for (float angle = 0; angle <= 2 * M_PI + step / 2; angle += step)
			{
				// � ���������� ���� ����������� sin(2 * PI) ����� ���������� �� sin(0)
				// ������� ���� angle ������� ������ � 2*PI, ������� ��� ������ 0
				// ��� ����� ��� ����, ����� ������ � ����� ������ ����� �������
				float a = (fabsf(angle - 2 * M_PI) < 1e-4) ? 0 : angle;

				// ��������� ������ ������������ ���������� ��� ������� ����
				float radius = 0.5f * (m_maxRadius - m_minRadius) * cosf(a * m_petals) + m_minRadius;
				float x = radius * cosf(a);
				float y = radius * sinf(a);

				// ������� ����� �������
				glVertex2f(x, y);
			}

			glEnd();
		}
		// ��������� ���������� ������
		glEndList();
	}

	// �������� ����� ��������� ���������� ������
	glCallList(m_displayList);
}
