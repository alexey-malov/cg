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
	// ��������� �������� ���������� z
	double r = sqrt(x * x + y * y);
	double z = Sinc(r);

	// "���������� �����" ���������� ��������� 
	// ��� ���������� �����������������
	double delta = 1e-6;

	// ��������� �������� ������� � ����� x, y, z 
	// (������ ������, ��� ������ ���� ����� ���� � ������, 
	// ������������� �����������)
	float f = F(x, y, z);

	// ��������� �������������� ������� ����������� ������� F �� dx, dy � dz
	// �� �������� �������������� ����� ����������� ������� 
	// ������� � ����������� � ����� (x, y, z)
	double dfdx = -(F(x + delta, y, z) - f) / delta;
	double dfdy = -(F(x, y + delta, z) - f) / delta;
	double dfdz = 1; // ����������� ������� -F(x,y,z)=-(f(x,y)-z) �� z

	// ��������, �������� ����� ������ �������������
	double invLen = 1 / sqrt(dfdx * dfdx + dfdy * dfdy + dfdz * dfdz);

	// ��������� ���������
	Vertex result = 
	{
		// ���������� �������
		float(x), float(y), float(z),
		// �������� ������ ������� � ��������� �����
		float(dfdx * invLen), float(dfdy * invLen), float(dfdz * invLen)
	};
	return result;
}
