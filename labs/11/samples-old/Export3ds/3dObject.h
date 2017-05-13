// 3dObject.h: interface for the C3dObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__2E94D933_CCC1_4617_97C4_EFCFC3B29030__INCLUDED_)
#define AFX_3DOBJECT_H__2E94D933_CCC1_4617_97C4_EFCFC3B29030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

class C3dObject  
{
public:
	CString m_Name;	// object name
	CArray <CMesh, CMesh const&> m_MeshArray;
	CArray <CTexture, CTexture const&> m_TextureArray;
	CArray <CMaterial, CMaterial const&> m_MaterialArray;
	BoundingBox m_BoundingBox;
public:
	void SetDistance(float scale);
	void CenterToBoundingBox();
	void DrawVertexNormals();
	BOOL Save(CString FileName);
	void DrawBoundingBoxes();
	CMaterial& GetMaterial(int MaterialId);
	int FindMaterial(CString MaterialName);
	CTexture& GetTexture(int TextureId);
	int LoadTexture(CString TextureName);
	CString m_DirectoryName;
//	CString GetDirectoryName();
	void Destroy();
	void Draw();
	C3dObject();
	virtual ~C3dObject();

	BOOL Load3ds(LPCTSTR FileName);

protected:
	BOOL SaveMeshes(CFile &file);
	BOOL SaveMaterials(CFile &file);
	BOOL LoadMeshes(database3ds *pDataBase);
	BOOL LoadMaterials(database3ds *pDataBase);
};

#endif // !defined(AFX_3DOBJECT_H__2E94D933_CCC1_4617_97C4_EFCFC3B29030__INCLUDED_)
