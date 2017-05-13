#include "StdAfx.h"
#include "MyApplication.h"

// Расстояние до ближней плоскости отсечения отображаемого объема
const double CMyApplication::ZNEAR = 0.5;
// Расстояние до дальей плоскости отсечения отображаемого объема
const double CMyApplication::ZFAR = 10;
// Угол обзора по вертикали
const double CMyApplication::FIELD_OF_VIEW = 60;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(
		1, -2, 0.2, // Положение глаза наблюдателя
		0, 0, 0,	// Точка, в которую направлена камера
		0, 0, 1		// Вектор, служащий для определения вектора "вверх"
		);

	// Рисуем красный квадрат
	glColor3f(1, 0, 0);
	glRectf(-1, -1, 1, 1);

	// Рисуем синий квадрат в плоскости Z=0, предварительно задав для него 
	// трансформацию повоота вокруг оси Y и а затем переноса на 0.2 вдоль оси Z
	glTranslatef(0, 0, 0.2);
	glRotatef(60, 0, 1, 0);

	glColor3f(0, 0, 1);
	glRectf(-0.5, -0.5, +0.5, +0.5);
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// Вычисляем соотношение сторон клиентской области окна
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
}
