// Material.h: interface for the CMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIAL_H__065BB485_858F_460D_B15D_D68DB53EF00A__INCLUDED_)
#define AFX_MATERIAL_H__065BB485_858F_460D_B15D_D68DB53EF00A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3dObject;

class CMaterial  
{
protected:
	C3dObject	*m_pParentObject;
	CString		m_Name;				// material name
	// texture parameters
	int			m_TextureIndex;		// texture index
	CString		m_TextureName;
	vec2		m_Scale;
	vec2		m_Offset;
	// material parameters
	vec4		m_Diffuse, m_Ambient, m_Specular, m_Emission;
	float		m_Shininess;
public:
	BOOL Save(CFile &file);
	CString GetName();
	void Bind();
	void Destroy();
	void SetParent(C3dObject *pParent);
	BOOL ReadMaterial3ds(material3ds const& material);
	CMaterial();
	virtual ~CMaterial();

};

#endif // !defined(AFX_MATERIAL_H__065BB485_858F_460D_B15D_D68DB53EF00A__INCLUDED_)
