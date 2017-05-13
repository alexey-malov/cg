#include "StdAfx.h"
#include "SincSurface.h"

CSincSurface::CSincSurface(int columns, int rows, float xMin, float xMax, float yMin, float yMax)
:CSurface(columns, rows, xMin, xMax, yMin, yMax)
{
}

double CSincSurface::Sinc(double x)
{
	return (fabs(x) < 1e-7) ? 1 : sin(x) / x;
}

double CSincSurface::F(double x, double y, double z)
{
	double r = sqrt(x * x + y * y);
	double f = Sinc(r);
	return f - z;
}

Vertex CSincSurface::CalculateVertex(double x, double y)const
{
	// вычисляем значение координаты z
	double r = sqrt(x * x + y * y);
	double z = Sinc(r);

	// "бесконечно малое" приращение аргумента 
	// для численного дифференцирования
	double delta = 1e-6;

	// Вычисляем значение функции в точке x, y, z 
	// (строго говоря, оно должно быть равно нулю в точках, 
	// принадлежащих поверхности)
	float f = F(x, y, z);

	// вычисляем приблизительно частные производные функции F по dx, dy и dz
	// их значения приблизительно равны координатам вектора 
	// нормали к поверхности а точке (x, y, z)
	double dfdx = -(F(x + delta, y, z) - f) / delta;
	double dfdy = -(F(x, y + delta, z) - f) / delta;
	double dfdz = 1; // производная фукнции -F(x,y,z)=-(f(x,y)-z) по z

	// величина, обратная длине векора антиградиента
	double invLen = 1 / sqrt(dfdx * dfdx + dfdy * dfdy + dfdz * dfdz);

	// формируем результат
	Vertex result = 
	{
		// координаты вершины
		float(x), float(y), float(z),
		// приводим вектор нормали к единичной длине
		float(dfdx * invLen), float(dfdy * invLen), float(dfdz * invLen)
	};
	return result;
}
