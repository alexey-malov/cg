#pragma once
#include "surface.h"

class CSincSurface :
	public CSurface
{
public:
	CSincSurface(int columns, int rows, float xMin, float xMax, float yMin, float yMax);
protected:
	// перегружаем метод вычисления вершины сетки
	virtual Vertex CalculateVertex(double x, double y)const;
private:
	// функция sinc=sin(x)/x
	static double Sinc(double x);
	// Представление поверхности виде функции F(x,y,z)=f(x, y) - z
	static double F(double x, double y, double z);
};
