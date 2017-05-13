#include "StdAfx.h"
#include "Planet.h"
#include "TextureLoader.h"

CPlanet::CPlanet(std::wstring const& textureName, float radius, float slices, float stacks)
:m_sphere(radius, slices, stacks)
,m_textureName(textureName)
,m_rotationAngle(0)
,m_inclinationAngle(0)
,m_rotationSpeed(0)
{
	m_material.SetDiffuse(1, 1, 1);
	m_material.SetShininess(50);
	m_material.SetSpecular(1, 1, 1);
}

// �������� ���� �������� ������� ������ ����� ���
void CPlanet::Animate(float timeDelta)
{
	m_rotationAngle = fmodf(m_rotationAngle + m_rotationSpeed * timeDelta, 360);
}

void CPlanet::SetInclinationAngle(float inclinationAngle)
{
	m_inclinationAngle = inclinationAngle;
}

void CPlanet::SetRotationSpeed(float rotationSpeed)
{
	m_rotationSpeed = rotationSpeed;
}

void CPlanet::Draw()const
{
	if (!m_texture)
	{
		// ��������� �������� ��� ������ ������ ������ Draw()
		CTextureLoader loader;
		m_texture.Attach(loader.LoadTexture2D(m_textureName));
	}
	// ������������ �������� � �������� �������
	glEnable(GL_TEXTURE_2D);
	m_texture.Bind();
	m_material.Activate();

	// ��������� ������� �������
	// (�������������� ������ � �������� modelview)
	// � ������������ �������
	// (������� ������ ��� ��������, � ����� ��������� ���)
	glPushMatrix();
	glRotatef(m_inclinationAngle, 1, 0, 0);
	glRotatef(m_rotationAngle, 0, 0, 1);

	// ������ �����
	m_sphere.Draw();

	// ��������������� ������� �������
	glPopMatrix();
}
