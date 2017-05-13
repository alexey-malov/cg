#include "StdAfx.h"
#include "MyApplication.h"
#include "Matrix4.h"

const double CMyApplication::FOV = 60;
const double CMyApplication::ZNEAR = 1;

// �������� ����������� ������
const float CMyApplication::EXTRUSION_FACTOR = 100;

CMyApplication::CMyApplication(const char * title, int width, int height)
:CGLApplication(title, width, height, true, true)
,m_rotationController(width, height)
,m_lightAnimationPhase(0)
,m_torusAnimationPhase(0)
{
	AddEventListener(&m_rotationController);
	m_rotationController.AddEventListener(this);

	m_light.SetAmbientIntensity(0.2f, 0.2f, 0.2f);
	m_light.SetDiffuseIntensity(0.8f, 0.8f, 0.8f);
	m_light.SetSpecularIntensity(0.4f, 0.4f, 0.4f);
	m_light.SetPosition(CVector3f(3, 3, 2));

	m_material.SetShininess(30);
	m_material.SetAmbient(0.2, 0.1, 0.3);
	m_material.SetDiffuse(0.5, 0.6, 0.7);
	m_material.SetSpecular(0.3, 0.3, 0.3);
}

void CMyApplication::OnInit()
{
	glClearColor(0.3, 0.3, 0.3, 1);
	glLoadIdentity();
	CMatrix4d modelView;
	modelView.LoadLookAtRH(
		0, 0, 10, 
		0, 0, 0, 
		0, 1, 0);
	m_rotationController.SetModelViewMatrix(modelView);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void CMyApplication::OnIdle()
{
	m_animationController.Tick();

	static const float LIGHT_ANIMATION_SPEED = 40.0f * M_PI / 180.0f;

	m_lightAnimationPhase = fmodf(
		m_lightAnimationPhase + 
		LIGHT_ANIMATION_SPEED * m_animationController.GetTimeDelta() * 0.001, 
		2 * M_PI);

	static const float LIGHT_ROTATION_RADIUS = 3;
	m_lightPosition.x = cosf(m_lightAnimationPhase) * LIGHT_ROTATION_RADIUS + 2;
	m_lightPosition.y = sinf(m_lightAnimationPhase) * LIGHT_ROTATION_RADIUS + 2;
	m_lightPosition.z = sin(m_lightAnimationPhase * 2) * LIGHT_ROTATION_RADIUS + 1;

	static const float TORUS_ANIMATION_SPEED = 56;
	m_torusAnimationPhase = fmodf(
		m_torusAnimationPhase + 
		TORUS_ANIMATION_SPEED * m_animationController.GetTimeDelta() * 0.001, 
		360);

	PostRedisplay();
}

void CMyApplication::DrawLightSource()const
{
	// ������ �������� �����
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(m_lightPosition.x, m_lightPosition.y, m_lightPosition.z);
	glutSolidSphere(0.1, 15, 10);
	glPopMatrix();
}

void CMyApplication::DrawScene(bool enableLighting)const
{
	// ������������ ��������
	m_material.Activate(GL_FRONT);

	// ������ ��������� ��������� �����
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	COmniLight light(m_light);
	light.SetPosition(m_lightPosition);
	if (!enableLighting)
	{
		// ��� ����������� ��������� �������� ����� 
		// ��������� ������ ������� ����
		light.SetDiffuseIntensity(0, 0, 0);
		light.SetSpecularIntensity(0, 0, 0);
	}
	light.SetLight(GL_LIGHT0);

	// ������ ���
	m_cube.Draw();

	// ������ ���� ���������� �����
	glPushMatrix();
	glTranslatef(-2, -8, -2);
	glRotatef(m_torusAnimationPhase, 1, 0, 0);
	glutSolidTorus(1.5, 4, 30, 60);
	glTranslatef(-4, 0, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(1.5, 4, 30, 60);
	glPopMatrix();
}

void CMyApplication::DrawShadowVolume()const
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0);

	// ������ �������� ������������ ���� �������������� ����������
	// � ���������� � ����������� �� ������� ���������� GL_EXT_stencil_wrap
	GLenum incrOp = GLEW_EXT_stencil_wrap ? GL_INCR_WRAP_EXT : GL_INCR;
	GLenum decrOp = GLEW_EXT_stencil_wrap ? GL_DECR_WRAP_EXT : GL_DECR;

	// ���� �� ��������� ���������� �������� ��� ������� ���������
	// ��� ������� � ��������� ������, � ����� ������������ ���
	// ����������/����������?
	if (GLEW_EXT_stencil_two_side && GL_EXT_stencil_wrap)
	{
		// ������� ����� ����� ��������� �� ���� ����

		// �������� ������������ ���� ���������
		glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

		// ��������� ��������� ������, �� ��������� ���� �������,
		// ����� ����������� �������� � ������ ���������
		glActiveStencilFaceEXT(GL_BACK);
		glStencilOp(GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);
		glStencilFunc(GL_ALWAYS, 0, ~0);

		// ��������� ������� ������, �� ��������� ���� �������,
		// ����� ��������� �������� � ������ ���������
		glActiveStencilFaceEXT(GL_FRONT);
		glStencilOp(GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);
		glStencilFunc(GL_ALWAYS, 0, ~0);

		// ������ ������� �����, �������� ����� ���������� ������
		glDisable(GL_CULL_FACE);
		m_cube.DrawShadowVolume(m_lightPosition, EXTRUSION_FACTOR);

		// �������� ����� ���������� ������
		glEnable(GL_CULL_FACE);
		// ��������� ������������ ���� ���������
		glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
	}
	else	// ��������� ���������� �� ��������������
	{
		// ������� ����� ����� ��������� � ��� �����

		// ������ ��������� �����, ���������� �������� � ������ ���������
		// ��� ������������� ����� �������
		glCullFace(GL_FRONT);
		glStencilOp(GL_KEEP, incrOp, GL_KEEP);
		m_cube.DrawShadowVolume(m_lightPosition, EXTRUSION_FACTOR);

		// ������ ������� �����, �������� �������� � ������ ���������
		// ��� ������������� ����� �������
		glCullFace(GL_BACK);
		glStencilOp(GL_KEEP, decrOp, GL_KEEP);
		m_cube.DrawShadowVolume(m_lightPosition, EXTRUSION_FACTOR);
	}
}

void CMyApplication::DrawShadowVolumeEdges()const
{
	glDisable(GL_LIGHTING);
	glColor3f(1, 0, 0);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_cube.DrawShadowVolume(m_lightPosition, EXTRUSION_FACTOR);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);	
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glLoadMatrixd(m_rotationController.GetModelViewMatrix());

	// ������ ������� ����� � ���� �����
	DrawLightSource();

	// ������ ����� ��� ���������� �����
	DrawScene(false);

	// ������ ������� �����
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);
	DrawShadowVolume();

	// ������ ����� �� ���������� ���������� �����
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilFunc(GL_EQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDepthFunc(GL_EQUAL);
	DrawScene(true);
	glDepthFunc(GL_LESS);
	glDisable(GL_STENCIL_TEST);
	
	// ������ ����� ������ �������� ������
	DrawShadowVolumeEdges();
}

void CMyApplication::OnRotationControllerUpdate()
{
	PostRedisplay();
}

void CMyApplication::OnReshape(int width, int height)
{
	// ������ ���� ��������� �������� � ���������� ������� ����
	glViewport(0, 0, width, height);

	// ��������� ����������� ������ ����
	double aspect = double(width) / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// ������������� ������� �������������
	gluPerspective(FOV, aspect, ZNEAR, ZNEAR + 1);

	// �������� ������������ ������� �������������
	double projectionMatrix[4][4];
	glGetDoublev(GL_PROJECTION_MATRIX, &projectionMatrix[0][0]);

	// � ������������ �� ��� ������ ���������� ��������� �������
	// ��������� ���������
	projectionMatrix[2][2] = -1;
	projectionMatrix[3][2] = -2 * ZNEAR;

	// ��������� ���������������� ������� �������������
	glLoadMatrixd(&projectionMatrix[0][0]);

	glMatrixMode(GL_MODELVIEW);
}
