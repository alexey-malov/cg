#pragma once

#include "Sphere.h"
#include "Material.h"
#include "Texture.h"

class CPlanet
{
public:
	CPlanet(
		std::wstring const& textureName, 
		float radius = 1, float slices = 50, 
		float stacks = 25);

	// ��������� ������� (�������� ������ ����� ���)
	void Animate(float timeDelta);

	// ������ �������
	void Draw()const;

	// ���� ������� ��� ��������
	void SetInclinationAngle(float inclinationAngle);

	// �������� �������� ������ ����� ���
	void SetRotationSpeed(float rotationSpeed);
private:
	CSphere m_sphere;		// �����, ���������������� ����������� �������
	CMaterial m_material;	// �������� �������
	float m_rotationAngle;	// ���� �������� ������ ������ ���
	float m_inclinationAngle; // ���� ������� ��� ��������
	float m_rotationSpeed;	// �������� �������� ������ ����������� ���
	mutable CTexture2D m_texture;  // �������� �������
	std::wstring m_textureName;	// ��� ����� � ���������
};
