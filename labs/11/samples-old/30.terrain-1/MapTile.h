// MapTile.h: interface for the MapTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPTILE_H__08423551_D9BE_4B22_A4F9_0F5B7E09EDFA__INCLUDED_)
#define AFX_MAPTILE_H__08423551_D9BE_4B22_A4F9_0F5B7E09EDFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TILE_SIZE 64
#define VERTEX_TILE_SIZE (TILE_SIZE + 1)

struct MapTile  
{
	// ����� �����
	GLfloat	heightMap[VERTEX_TILE_SIZE * VERTEX_TILE_SIZE];

	int	startX, startY;	// ���������� ������ �����
	int numRows, numColumns;
	int mapStep;

	// ����� ����������� ������ ��� ��������� �����
	GLuint	displayList;

	void Load(int x, int y, int columns, int rows, int step, GLfloat const* mapData)
	{
		startX = x;
		startY = y;
		numRows = columns;
		numColumns = rows;
		mapStep = step;
		displayList = 0;
		memcpy(heightMap, mapData, numRows * numColumns * sizeof(GLfloat));
	}

	void Upload()
	{
		displayList = glGenLists(1);

		glNewList(displayList, GL_COMPILE);
		{
			int y0 = startY;
			for (int row = 0; row < numRows - 1; ++row , y0 += mapStep)
			{
				int x0 = startX;
				glBegin(GL_TRIANGLE_STRIP);
				for (int column = 0; column < numColumns; ++column, x0 += mapStep)
				{
					glVertex3f((float)x0, (float)(y0 + mapStep), heightMap[(row + 1) * numColumns + column]);
					glVertex3f((float)x0, (float)y0, heightMap[row * numColumns + column]);
				}
				glEnd();
			}
		}
		glEndList();

	}

	void Draw()
	{
		if (!displayList)
		{
			Upload();
		}
		glCallList(displayList);
	}

	void Destroy()
	{
		if (displayList != 0)
		{
			glDeleteLists(displayList, 1);
		}
	}
};


#endif // !defined(AFX_MAPTILE_H__08423551_D9BE_4B22_A4F9_0F5B7E09EDFA__INCLUDED_)
