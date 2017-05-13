#pragma once
#include "glapplication.h"
#include "Cube.h"

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
	// Вращаем камеру вокруг начала кординат на заданный угол
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY);
	// Производим ортонормирование матрицы моделирования-вида
	static void NormalizeModelViewMatrix(void);
private:
	bool m_leftButtonPressed;
	int	m_mouseX;
	int m_mouseY;
	int m_windowWidth;
	int m_windowHeight;

	CCube m_cube;

	// Угол обзора по вертикали 
	static const double FIELD_OF_VIEW;

	static const double Z_NEAR;
	static const double Z_FAR;

	// Размер стороны куба
	static const double CUBE_SIZE;

};
