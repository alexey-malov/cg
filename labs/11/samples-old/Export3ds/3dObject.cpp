// 3dObject.cpp: implementation of the C3dObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Export3ds.h"
#include "3dObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3dObject::C3dObject()
{

}

C3dObject::~C3dObject()
{

}

BOOL C3dObject::Load3ds(LPCTSTR FileName)
{
//	return TRUE;
	file3ds *pFile;
	database3ds	*pDataBase=NULL;

	Destroy();

	pFile = OpenFile3ds(FileName, "rb");
	InitDatabase3ds(&pDataBase);
	CreateDatabase3ds(pFile, pDataBase);

	// Load materials and textures
	if (!LoadMaterials(pDataBase))
	{
		Destroy();
		return FALSE;
	}
	
	// Load meshes
	if (!LoadMeshes(pDataBase))
	{
		Destroy();
		return FALSE;
	}


	ReleaseDatabase3ds(&pDataBase);
	CloseFile3ds(pFile);
	CloseAllFiles3ds();
	return TRUE;
}

void C3dObject::Draw()
{
	for (int mesh=0;mesh<m_MeshArray.GetSize();mesh++)
	{
		m_MeshArray[mesh].Draw();
	}
}

// грузим материалы
BOOL C3dObject::LoadMaterials(database3ds *pDataBase)
{
	int mat;
	int nMaterials = GetMaterialCount3ds(pDataBase);

	if (!nMaterials)
		return TRUE;
	m_MaterialArray.SetSize(nMaterials);
	for (mat=0; mat<nMaterials; mat++)
	{
		material3ds *pMaterial = NULL;
		GetMaterialByIndex3ds(pDataBase, mat, &pMaterial);
		m_MaterialArray[mat].SetParent(this);
		m_MaterialArray[mat].ReadMaterial3ds(*pMaterial);
		ReleaseMaterial3ds(&pMaterial);
	}
	return TRUE;
}

void C3dObject::Destroy()
{
	m_MeshArray.RemoveAll();
	m_TextureArray.RemoveAll();
	m_MaterialArray.RemoveAll();

	m_Name = "";
}

// грузим меши
BOOL C3dObject::LoadMeshes(database3ds *pDataBase)
{
	int mesh;
	int nMeshes = GetMeshCount3ds(pDataBase);

	if (!nMeshes)
		return FALSE;

	m_MeshArray.SetSize(nMeshes);

	for (mesh=0; mesh<nMeshes; mesh++)
	{
		mesh3ds *pMesh = NULL;

		GetMeshByIndex3ds(pDataBase, mesh, &pMesh);

		m_MeshArray[mesh].SetParent(this);
		m_MeshArray[mesh].ReadMesh3ds(*pMesh);
		
		RelMeshObj3ds(&pMesh);
	}

	m_BoundingBox = m_MeshArray[0].m_BoundingBox;
	// Set Bounding box
	for (mesh=1; mesh<nMeshes; mesh++)
	{
		// adjust bounds
		for (int i=0; i<3; i++)
		{
			if (m_BoundingBox.MaxCoords[i]<m_MeshArray[mesh].m_BoundingBox.MaxCoords[i])
				m_BoundingBox.MaxCoords[i]=m_MeshArray[mesh].m_BoundingBox.MaxCoords[i];

			if (m_BoundingBox.MinCoords[i]>m_MeshArray[mesh].m_BoundingBox.MinCoords[i])
				m_BoundingBox.MinCoords[i]=m_MeshArray[mesh].m_BoundingBox.MinCoords[i];
		}

	}


	return TRUE;
}


// возвращает индекс текстуры с данным именем
// попутно загружает данную текстуру, если нет текстур
// в случае ошибки возвращает -1
int C3dObject::LoadTexture(CString TextureName)
{
	int tex;
	for (tex=0; tex<m_TextureArray.GetSize(); tex++)
	{
		if (m_TextureArray[tex].GetName()==TextureName)
		{
			return tex;
		}
	}
	CTexture texture;
	tex = m_TextureArray.Add(texture);

	if (!m_TextureArray[tex].LoadBitmap(m_DirectoryName+TextureName))
	{
		return -1;
	}

	return tex;
}

CTexture& C3dObject::GetTexture(int TextureId)
{
	return m_TextureArray[TextureId];
}

int C3dObject::FindMaterial(CString MaterialName)
{
	int mat;
	for (mat=0; mat<m_MaterialArray.GetSize(); mat++)
	{
		if (m_MaterialArray[mat].GetName()==MaterialName)
		{
			return mat;
		}
	}
	return -1;
}

CMaterial& C3dObject::GetMaterial(int MaterialId)
{
	return m_MaterialArray[MaterialId];
}

void C3dObject::DrawBoundingBoxes()
{
	for (int mesh=0;mesh<m_MeshArray.GetSize();mesh++)
	{
		//m_MeshArray[mesh].DrawBoundingBox();
	}
	m_BoundingBox.Draw();
}

BOOL C3dObject::Save(CString FileName)
{
	CFile file;

	if (!file.Open((LPCTSTR)FileName, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

	// save materials
	if (!SaveMaterials(file))
	{
		file.Close();
		return FALSE;
	}

	// save meshes
	if (!SaveMeshes(file))
	{
		file.Close();
		return FALSE;
	}

	file.Close();
	return TRUE;
}

BOOL C3dObject::SaveMaterials(CFile &file)
{
	// save material number
	int nMaterials = m_MaterialArray.GetSize();
	file.Write(&nMaterials, sizeof (nMaterials));

	for (int mat=0; mat<nMaterials; mat++)
	{
		m_MaterialArray[mat].Save(file);
	}
	return TRUE;
}

BOOL C3dObject::SaveMeshes(CFile &file)
{
	// save mesh numebr
	WRITE_SIZE(file, m_MeshArray);
	// save each mesh
	for (int mesh=0; mesh<m_MeshArray.GetSize(); mesh++)
	{
		m_MeshArray[mesh].Save(file);
	}
	return TRUE;
}

void C3dObject::DrawVertexNormals()
{
	for (int mesh=0;mesh<m_MeshArray.GetSize();mesh++)
	{
		m_MeshArray[mesh].DrawVertexNormals();
	}

}

void C3dObject::CenterToBoundingBox()
{
	vec3 center;
	center=m_BoundingBox.MinCoords;
	center+=m_BoundingBox.MaxCoords;
	center*=0.5f;
	glTranslatef(-center.x,-center.y,-center.z);
}

void C3dObject::SetDistance(float scale)
{
	// считаем половину диагонали бокса
	vec3 radius=m_BoundingBox.MaxCoords;
	radius-=m_BoundingBox.MinCoords;
	float r = nv_norm(radius)*0.5f;
	glTranslatef(0,0,-scale*r);
}
