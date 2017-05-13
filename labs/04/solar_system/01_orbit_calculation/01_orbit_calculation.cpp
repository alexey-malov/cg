// 01_orbit_calculation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainApp.h"
#include "KeplerMotionLaws.h"


CMainApp app("Orbit calculation", 800, 600);

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	using namespace boost::math::tools;

	const double M = 203.9192621 * M_PI / 180;
	const double e = 0.2;
	double a = 1;
	KeplerEquationFunctorDouble fn(M, e);

	boost::uintmax_t maxIter = 20;


	double E;
	double result = 0;
	DWORD tick = GetTickCount();
	for (int i = 0; i < 100; ++i)
	{
		E = SolveKeplerEquation(M, e, maxIter); //halley_iterate(fn, M, M - e, M + e, 10, maxIter);
		result += ((i & 1) * 2 - 1) + E;
	}
	std::cout << GetTickCount() - tick << "\n";

	double tg_v_2 = sqrt((1 + e) / (1 - e)) * tan(E * 0.5);
	double v = atan(tg_v_2) * 2;
	double r = a * (1 - e * cos(E));

	
	std::cout << (E * 180 / M_PI) << "\n" << (std::tr1::get<0>(KeplerEquationFunctorDouble(M, e)(E)) * M_PI / 180) << "\n" << maxIter << "\n";
	std::cout << "v:" << (v * 180 / M_PI) << "\n";
	std::cout << "r:" << r << "\n";

	CEllipticOrbit<double> orbit(1, 0.5);
	E = orbit.EccentricityAnomaly(0.15);
	*/

	app.MainLoop();
	
	return 0;
}

