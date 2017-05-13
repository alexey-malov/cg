// Material.cpp: implementation of the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Export3ds.h"
#include "Material.h"
#include "3dObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaterial::CMaterial()
{
	m_TextureIndex = -1;
	m_Name = "";
}

CMaterial::~CMaterial()
{
	Destroy();
}

BOOL CMaterial::ReadMaterial3ds(const material3ds &material)
{
	Destroy();

	m_Name = material.name;
	m_TextureName = material.texture.map.name;
	if (m_TextureName.IsEmpty())
	{
		m_TextureIndex = -1;
	}
	else
	{
		m_TextureIndex = m_pParentObject->LoadTexture(m_TextureName);
	}
	
	m_Scale.s = material.texture.map.uscale;
	m_Scale.t = material.texture.map.vscale;
	m_Offset.s = material.texture.map.uoffset;
	m_Offset.t = material.texture.map.voffset;
	
	m_Specular = vec4(material.specular.r,
					  material.specular.g,
					  material.specular.b,
					  1.0f);
	m_Diffuse = vec4(material.diffuse.r,
					 material.diffuse.g,
					 material.diffuse.b,
					 1.0f);
	m_Ambient = vec4(material.ambient.r,
					 material.ambient.g,
					 material.ambient.b,
					 1.0f);
	m_Emission = vec4(material.selfillumpct, 
					  material.selfillumpct,
					  material.selfillumpct,
					  1.0f);
	m_Shininess = material.shininess;
	
	return TRUE;
}

void CMaterial::SetParent(C3dObject *pParent)
{
	m_pParentObject = pParent;
}

void CMaterial::Destroy()
{
	m_TextureIndex = -1;
	m_TextureName = "";
	m_Name = "";
}

void CMaterial::Bind()
{
	const float max_diffuse[] = {1,1,1,1};
	if (m_TextureIndex>=0)
	{
		m_pParentObject->GetTexture(m_TextureIndex).Bind();
		glEnable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT, GL_AMBIENT, max_diffuse);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, max_diffuse);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Ambient.vec_array);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Diffuse.vec_array);
		glDisable(GL_TEXTURE_2D);
	}
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_Specular.vec_array);
	glMaterialf(GL_FRONT, GL_SHININESS, 128*m_Shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_Emission.vec_array);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(m_Scale.s, m_Scale.t, 1);
	glTranslatef(m_Offset.s, m_Offset.t,0);
	glMatrixMode(GL_MODELVIEW);
}

CString CMaterial::GetName()
{
	return m_Name;
}

BOOL CMaterial::Save(CFile &file)
{
	// save material name
	WRITE_STR(file, m_Name);		//string
	// save texture file name
	WRITE_STR(file, m_TextureName);	//string
	// save scale
	WRITE_BIN(file, m_Scale);		//vec2
	// save offset
	WRITE_BIN(file, m_Offset);		//vec2
	// save ambient color
	WRITE_BIN(file, m_Ambient);		//vec4
	// save diffuse color
	WRITE_BIN(file, m_Diffuse);		//vec4
	// save specular color
	WRITE_BIN(file, m_Specular);	//vec4
	// save emission color
	WRITE_BIN(file, m_Emission);	//vec4
	// save Shininess
	WRITE_BIN(file, m_Shininess);	//scalar
	return TRUE;
}
