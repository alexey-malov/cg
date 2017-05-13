#pragma once

template <class T>
class CVector3
{
public:
	T x, y, z;

	CVector3(void)
		:x(0), y(0), z(0)
	{
	}

	CVector3(T x0, T y0, T z0)
		:x(x0), y(y0), z(z0)
	{
	}

	T GetLength()const
	{
		return static_cast<T>(sqrt(x * x + y * y + z * z));
	}

	// Ќормализуем
	void Normalize()
	{
		float invLength = 1 / GetLength();
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}

};


// ¬ычисл€ем векторное произведение a и b
template <class T>
CVector3<T> const Cross(CVector3<T> const & a, CVector3<T> const & b)
{
	/*
	¬екторное произведение векторов A и B равно определителю матрицы:
	      |i  j  k |
	AxB = |Ax Ay Az|
	      |Bx By Bz|
	*/
	return CVector3<T>(
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z, 
		a.x * b.y - a.y * b.x );

}

typedef CVector3<float> CVector3f;
typedef CVector3<double> CVector3d;
