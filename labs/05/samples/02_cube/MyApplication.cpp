#include "StdAfx.h"
#include "MyApplication.h"

// Угол обзора по вертикали 
const double CMyApplication::FIELD_OF_VIEW = 60;
// Размер стороны куба
const double CMyApplication::CUBE_SIZE = 1;


const double CMyApplication::Z_NEAR = 0.1;
const double CMyApplication::Z_FAR = 10;


CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
{
	m_cube.SetSideColor(CCube::CS_NEGATIVE_X, 255, 0, 0);
	m_cube.SetSideColor(CCube::CS_POSITIVE_X, 0, 255, 0);
	m_cube.SetSideColor(CCube::CS_NEGATIVE_Y, 0, 0, 255);
	m_cube.SetSideColor(CCube::CS_POSITIVE_Y, 255, 255, 0);
	m_cube.SetSideColor(CCube::CS_NEGATIVE_Z, 0, 255, 255);
	m_cube.SetSideColor(CCube::CS_POSITIVE_Z, 255, 0, 255);
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	// Включаем режим отбраковки граней
	glEnable(GL_CULL_FACE);
	// Отбраковываться будут нелицевые стороны граней
	glCullFace(GL_BACK);
	// Сторона примитива считается лицевой, если при ее рисовании
	// обход верших осуществляется против часовой стрелки
	glFrontFace(GL_CCW);

	// Включаем тест глубины для удаления невидимых линий и поверхностей
	glEnable(GL_DEPTH_TEST);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Инициализируем матрицу моделирования вида
	glLoadIdentity();
	// и умножаем ее на матрицу просмотра из точки (2, 2, 2) в точку (0, 0, 0)
	// с вертикальным вектором (0, 1, 0)
	gluLookAt(
		2, 2, 2, 
		0, 0, 0,
		0, 1, 0
		);

	m_cube.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// Вычисляем соотношение сторон клиентской области окна
	double aspect = double(width) / double(height);

	// Задаем матрицу проецирования
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Матрица перспективного преобразования с параметрами:
	//	Угол обзора по вертикали: FIELD_OF_VIEW градусов
	//	Отношение ширины к высоте видимой областие: aspect
	//	Расстояние до ближней и дальней плоскостей отсечения видимого объема:
	//		zNear и zFar соответственно
	gluPerspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}
