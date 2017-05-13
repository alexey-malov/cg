#pragma once

#include "Vector3.h"

class CDirectLight
{
public:
	CDirectLight(CVector3f const& lightDirection = CVector3f(0, 0, 1));

	void SetDirection(CVector3f const& direction);
	void SetDiffuseIntensity(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);
	void SetAmbientIntensity(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);
	void SetSpecularIntensity(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);

	void SetLight(GLenum light);
private:
	CVector3f m_direction;
	GLfloat m_diffuse[4];
	GLfloat m_ambient[4];
	GLfloat m_specular[4];
};
