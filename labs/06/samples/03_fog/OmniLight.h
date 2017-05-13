#pragma once

#include "Vector3.h"
#include "Light.h"

class COmniLight : public CLight
{
public:
	COmniLight(CVector3f const& position = CVector3f(0, 0, 0));

	void SetPosition(CVector3f const& position);
	void SetLight(GLenum light)const;
	void SetQuadraticAttenuation(GLfloat quadraticAttenuation);
	void SetLinearAttenuation(GLfloat linearAttenuation);
	void SetConstantAttenuation(GLfloat constantAttenuation);
private:
	CVector3f m_position;
	GLfloat m_quadraticAttenuation;
	GLfloat m_linearAttenuation;
	GLfloat m_constantAttenuation;
};
