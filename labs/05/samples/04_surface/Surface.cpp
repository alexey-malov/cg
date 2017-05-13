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
	// ѕри первом обращении к данной функции запишем команды 
	// рисовани€ поверхности в дисплейный список
	if (m_displayList == 0)
	{
		m_displayList = glGenLists(1);
		glNewList(m_displayList, GL_COMPILE);

		// вычисл€ем шаг узлов сетки
		const float dy = (m_yMax - m_yMin) / (m_rows - 1);
		const float dx = (m_xMax - m_xMin) / (m_columns - 1);

		float y = m_yMin;
		// пробегаем по строкам сетки
		for (int row = 0; row < m_rows - 1; ++row, y += dy)
		{
			// каждой строке будет соответствовать сво€ лента из треугольников
			glBegin(GL_TRIANGLE_STRIP);
			float x = m_xMin;

			// пробегаем по столбцам текущей строки
			for (int column = 0; column <= m_columns; ++column, x += dx)
			{
				// вычисл€ем параметры вершины в узлах пары соседних вершин
				// ленты из треугольников
				Vertex v0 = CalculateVertex(x, y + dy);
				Vertex v1 = CalculateVertex(x, y);

				// задаем нормаль и координаты вершины на четной позиции
				glNormal3f(v0.nx, v0.ny, v0.nz);
				glVertex3f(v0.x, v0.y, v0.z);

				// задаем нормаль и координаты вершины на нечетной позиции
				glNormal3f(v1.nx, v1.ny, v1.nz);
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}

		glEndList();
	}

	// ¬ызовем ранее записанный дисплейный список команд рисовани€
	// сетки
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

// Ќе забыли удалить дисплейный список при разрушении экземпл€ра класса
CSurface::~CSurface(void)
{
	if (m_displayList != 0)
	{
		glDeleteLists(m_displayList, 1);
	}
}
