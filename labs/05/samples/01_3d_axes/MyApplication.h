#pragma once
#include "glapplication.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char * title, int width, int height);
	~CMyApplication(void);
protected:
	virtual void OnInit();
	virtual void OnDisplay();
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);
private:
	// ¬ращаем камеру вокруг начала кординат на заданный угол
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY);
	// ѕроизводим ортонормирование матрицы моделировани€-вида
	static void NormalizeModelViewMatrix(void);

	// ‘лаг, свидетельствующий о состо€нии левой кнопки мыши
	bool m_leftButtonPressed;

	// —тарые координаты курсора мыши
	int	m_mouseX;
	int m_mouseY;

	// –азмеры окна
	int m_windowWidth;
	int m_windowHeight;

	// –азмер видимой области (по вертикали и горизонтали)
	static const double FRUSTUM_SIZE;

	// –ассто€ние от камеры до центра координатной оси
	static const double DISTANCE_TO_ORIGIN;
};
