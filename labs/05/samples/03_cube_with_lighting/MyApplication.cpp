#include "StdAfx.h"
#include "MyApplication.h"
#include "DirectLight.h"

// Угол обзора по вертикали 
const double CMyApplication::FIELD_OF_VIEW = 60;
// Размер стороны куба
const double CMyApplication::CUBE_SIZE = 1;


const double CMyApplication::Z_NEAR = 0.1;
const double CMyApplication::Z_FAR = 10;


CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_windowWidth(width)
,m_windowHeight(height)
,m_leftButtonPressed(false)
,m_mouseX(0)
,m_mouseY(0)
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
	// Создаем и инициализируем источник света
	CDirectLight light(CVector3f(1, 1, 3));	// направление на источник света
	light.SetDiffuseIntensity(0.5f, 0.5f, 0.5f); // интенсивность диффузной составляющей
	light.SetAmbientIntensity(0.2f, 0.2f, 0.2f); // интенсивность фоновой составляющей
	light.SetSpecularIntensity(0.3f, 0.3f, 0.3f);// интенсивность зеркальной составляющей
	light.SetLight(GL_LIGHT0);	// копируем параметры источника света в OpenGL

	// Включаем освещение и источник света №0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Переносим точку камеры на заданное расстояние от начала координат
	// вдоль оси Z
	gluLookAt(
		2, 2, 2, 
		0, 0, 0,
		0, 1, 0
		);

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

	// Включаем режим, управляющий цветом материала 
	// посредством текущего цвета вершины
	glEnable(GL_COLOR_MATERIAL);
	// Цвет вершины будет определять диффузную и фоновую 
	// составляющие цвета материала
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Задаем коэффициент зеркального отражения материала
	GLfloat specularColor[4] = {0.3, 0.3, 0.3, 1};
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);

	// Задаем степень зеркального отражения материала
	glMaterialf(GL_FRONT, GL_SHININESS, 20);

	m_cube.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	glViewport(0, 0, width, height);

	// Вычисляем соотношение сторон клиентской области окна
	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glMatrixMode(GL_MODELVIEW);
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	// Событие от левой кнопки мыши
	if (button == GLUT_LEFT_BUTTON)
	{
		// Сохраняем состояние левой кнопки мыши
		m_leftButtonPressed = (state == GLUT_DOWN);
		// Сохраняем координаты мыши
		m_mouseX = x;
		m_mouseY = y;
	}
}

void CMyApplication::OnMotion(int x, int y)
{
	// Если нажата левая кнопка мыши
	if (m_leftButtonPressed)
	{
		// Вычисляем смещение курсора мыши
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		// Вычисляем угол поворота вокруг осей Y и X как линейно зависящие
		// от смещения мыши по осям X и Y
		GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
		GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;

		RotateCamera(rotateX, rotateY);

		// Сохраняем текущие координаты мыши
		m_mouseX = x;
		m_mouseY = y;

		// Инициируем перерисовку окна
		PostRedisplay();
	}
}

// Вращаем камеру вокруг начала кординат на заданный угол
void CMyApplication::RotateCamera(GLfloat rotateX, GLfloat rotateY)
{
	// Извлекаем текущее значение матрицы моделирования-вида
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0]);

	// Извлекаем направления координатных осей камеры в 3д пространстве
	// как коэффициенты строк матрицы моделирования-вида
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);

	// Поворачиваем вокруг осей x и y камеры
	glRotatef(rotateX, xAxis.x, xAxis.y, xAxis.z);
	glRotatef(rotateY, yAxis.x, yAxis.y, yAxis.z);

	// В ходе умножения матриц могут возникать погрешности, которые,
	// накапливаясь могут сильно искажать картинку
	// Для их компенсации после каждой модификации матрицы моделирования-вида
	// проводим ее ортонормирование
	NormalizeModelViewMatrix();
}

// Производим ортонормирование матрицы моделирования-вида
void CMyApplication::NormalizeModelViewMatrix(void)
{
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

	/*
	Ортонормирование - приведение координатных осей к единичной длине (нормирование)
	и взаимной перпендикулярности (ортогонализация)
	Достичь этого можно при помощи нормализации координатных осей
	и векторного произведения
	*/
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	xAxis.Normalize();	
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);
	yAxis.Normalize();

	// Ось Z вычисляем через векторное произведение X и Y
	// Z будет перпендикулярна плоскости векторов X и Y
	CVector3f zAxis = Cross(xAxis, yAxis);
	// И иметь единичную длину
	zAxis.Normalize();
	// То же самое проделываем с осями x и y
	xAxis = Cross(yAxis, zAxis);
	xAxis.Normalize();
	yAxis = Cross(zAxis, xAxis);
	yAxis.Normalize();

	// Сохраняем вектора координатных осей обратно в массив
	modelView[0] = xAxis.x; modelView[4] = xAxis.y, modelView[8] = xAxis.z;
	modelView[1] = yAxis.x; modelView[5] = yAxis.y, modelView[9] = yAxis.z;
	modelView[2] = zAxis.x; modelView[6] = zAxis.y, modelView[10] = zAxis.z;

	// И загружаем матрицу моделирвания-вида
	glLoadMatrixf(modelView);
}
