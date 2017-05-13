#include "StdAfx.h"
#include "MyApplication.h"
#include "Frame.h"
#include "Vector3.h"

// Размер видимого объема, которые должен поместиться в порт просмотра
const double CMyApplication::FRUSTUM_SIZE = 2;
// Расстояние от камеры до точки вращения
const double CMyApplication::DISTANCE_TO_ORIGIN = 2;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height)
,m_windowWidth(width)
,m_windowHeight(height)
,m_leftButtonPressed(false)
,m_mouseX(0)
,m_mouseY(0)
{
}

CMyApplication::~CMyApplication(void)
{
}

void CMyApplication::OnInit()
{
	// Переносим точку камеры на заданное расстояние от начала координат
	// вдоль оси Z
	glTranslatef(0, 0, -DISTANCE_TO_ORIGIN);

	// Задаем ширину линий
	glLineWidth(2);

	// И цвет очистки буфера цвета
	glClearColor(1, 1, 1, 1);

	// Включаем тест глубины для удаления невидимых линий и поверхностей
	glEnable(GL_DEPTH_TEST);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Создаем координатный фрейм и рисуем его
	CFrame frame;
	frame.Draw();
}

void CMyApplication::OnReshape(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	glViewport(0, 0, width, height);

	// Вычисляем соотношение сторон клиентской области окна
	double aspect = double(width) / double(height);

	// Считаем, что высота видимой области равна FRUSTUM_SIZE
	// (на расстоянии до ближней плоскости отсечения)
	double frustumHeight = FRUSTUM_SIZE;
	// Ширина видимой области рассчитывается согласно соотношению сторон окна
	// (шире окно - шире область видимости и наоборот)
	double frustumWidth = frustumHeight * aspect;

	// Если ширина видимой области получилась меньше, чем FRUSTUM_SIZE, 
	// то корректируем размеры видимой области
	if (frustumWidth < FRUSTUM_SIZE && (aspect != 0))
	{
		frustumWidth = FRUSTUM_SIZE;
		frustumHeight = frustumWidth / aspect;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(
		-frustumWidth * 0.5, frustumWidth * 0.5,	// left, right
		-frustumHeight * 0.5, frustumHeight * 0.5,	// top, bottom
		FRUSTUM_SIZE * 0.5, FRUSTUM_SIZE * 20		// znear, zfar
		);
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
