// Mesh.cpp: implementation of the CMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Export3ds.h"
#include "Mesh.h"
#include "3dObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMesh::CMesh()
{

}

CMesh::~CMesh()
{
	Destroy();
}

BOOL CMesh::ReadMesh3ds(mesh3ds const& Mesh)
{
	int vert;
	int face;
	int mat;
	// уничтожаем старье
	Destroy();

	m_Name = Mesh.name;
	m_VertexArray.SetSize(Mesh.nvertices);	// устанавливаем размер массива вершин
	m_SubMeshArray.SetSize(Mesh.nmats+1);		// устанавливаем размер массива субматериалов

	if (!(Mesh.nvertices&&Mesh.nfaces))
	{
		return TRUE;
	}
	// set bounding box
	m_BoundingBox.MaxCoords = vec3(Mesh.vertexarray[0].x, Mesh.vertexarray[0].y, Mesh.vertexarray[0].z);
	m_BoundingBox.MinCoords = m_BoundingBox.MaxCoords;

	// заполняем массив вершин
	for (vert=0; vert<m_VertexArray.GetSize(); vert++)
	{
		m_VertexArray[vert].Coord = (float*)&(Mesh.vertexarray[vert]);
		m_VertexArray[vert].Normal = vec3_null;
		if (vert<Mesh.ntextverts)
		{
			m_VertexArray[vert].TexCoord = (float*)&(Mesh.textarray[vert]);
		}
		else
		{
			m_VertexArray[vert].TexCoord = vec2(0,0);
		}

		// adjust bounds
		for (int i=0; i<3; i++)
		{
			if (m_BoundingBox.MaxCoords[i]<m_VertexArray[vert].Coord[i])
				m_BoundingBox.MaxCoords[i] = m_VertexArray[vert].Coord[i];

			if (m_BoundingBox.MinCoords[i]>m_VertexArray[vert].Coord[i])
				m_BoundingBox.MinCoords[i] = m_VertexArray[vert].Coord[i];
		}
	}

	CByteArray AssigendFacesArray;
	AssigendFacesArray.SetSize(Mesh.nfaces);
	ZeroMemory(AssigendFacesArray.GetData(),Mesh.nfaces);
	int nAssignedFaces = 0;

	// заполняем массив субмешей
	for (mat=0; mat<m_SubMeshArray.GetSize()-1; mat++)
	{
		// set face count
		int nfaces = Mesh.matarray[mat].nfaces;
		m_SubMeshArray[mat].FaceArray.SetSize(nfaces);

		// set material name
		m_SubMeshArray[mat].MaterialName = Mesh.matarray[mat].name;
		m_SubMeshArray[mat].MaterialId = m_pParentObject->FindMaterial(m_SubMeshArray[mat].MaterialName);
		
		for (face=0; face<nfaces; face++)
		{
			int FaceId = Mesh.matarray[mat].faceindex[face];
			if (!AssigendFacesArray[FaceId])
			{
				AssigendFacesArray[FaceId] = TRUE;
				nAssignedFaces++;
			}
			
			int faceVerts[3];

			faceVerts[0] = Mesh.facearray[FaceId].v1;
			faceVerts[1] = Mesh.facearray[FaceId].v2;
			faceVerts[2] = Mesh.facearray[FaceId].v3;

			// заносим вершины грани
			for (vert=0; vert<3; vert++)
			{
				m_SubMeshArray[mat].FaceArray[face].VertexIndices[vert] = faceVerts[vert];
			}

			m_SubMeshArray[mat].FaceArray[face].Flag = Mesh.facearray[FaceId].flag;
			if (Mesh.smootharray)
			{
				m_SubMeshArray[mat].FaceArray[face].SmoothGroup = Mesh.smootharray[FaceId];
			}
			else
			{
				m_SubMeshArray[mat].FaceArray[face].SmoothGroup = 0;
			}
		}
	}

	m_SubMeshArray[Mesh.nmats].MaterialId = -1;
	m_SubMeshArray[Mesh.nmats].MaterialName = "";
	m_SubMeshArray[Mesh.nmats].FaceArray.SetSize(Mesh.nfaces-nAssignedFaces);

	int CurrentNotAssignedFace = 0;
	for (face=0; face<Mesh.nfaces; face++)
	{
		if (!AssigendFacesArray[face])
		{
			// у этой грани нет материала, поэтому добавляем ее.
			FaceDesc &FaceRef = m_SubMeshArray[Mesh.nmats].FaceArray[CurrentNotAssignedFace];
			FaceRef.Flag = Mesh.facearray[face].flag;
			FaceRef.VertexIndices[0] = Mesh.facearray[face].v1;
			FaceRef.VertexIndices[1] = Mesh.facearray[face].v2;
			FaceRef.VertexIndices[2] = Mesh.facearray[face].v3;
			FaceRef.SmoothGroup = Mesh.smootharray[face];
			CurrentNotAssignedFace++;
		}
	}

	// строим Face LookUp (ищем одинаковые вершины меша)
	CArray <int, int> VertexLookup;
	VertexLookup.SetSize(m_VertexArray.GetSize());
	for (vert=0; vert<m_VertexArray.GetSize(); vert++)
	{
		for (int vl=0; vl<vert; vl++)
		{
			if (m_VertexArray[vert].Coord==m_VertexArray[vl].Coord)
			{
				// такая вершина уже есть
				break;
			}
		}
		VertexLookup[vert] = vl;
	}

	// суммируем нормали фейсов для общих вершин
	for (mat=0; mat<m_SubMeshArray.GetSize(); mat++)
	{
		for (face=0; face<m_SubMeshArray[mat].FaceArray.GetSize(); face++)
		{
			FaceDesc &FaceRef = m_SubMeshArray[mat].FaceArray[face];
			// считаем нормаль к фейсу
			vec3 v0(m_VertexArray[FaceRef.VertexIndices[0]].Coord);
			vec3 v1(m_VertexArray[FaceRef.VertexIndices[1]].Coord);
			vec3 v2(m_VertexArray[FaceRef.VertexIndices[2]].Coord);
			vec3 FaceNormal;
			v1-=v0;
			v2-=v0;
			cross(FaceNormal,v1,v2);
			normalize(FaceNormal);
			m_VertexArray[VertexLookup[FaceRef.VertexIndices[0]]].Normal+=FaceNormal;
			m_VertexArray[VertexLookup[FaceRef.VertexIndices[1]]].Normal+=FaceNormal;
			m_VertexArray[VertexLookup[FaceRef.VertexIndices[2]]].Normal+=FaceNormal;
		}
	}

	// Дублируем нормали у вершин, с одинаковыми координатам
	for (vert=0; vert<m_VertexArray.GetSize(); vert++)
	{
		m_VertexArray[vert].Normal = m_VertexArray[VertexLookup[vert]].Normal;
	}
	

	// normalize vertex normals
	for (vert=0; vert<m_VertexArray.GetSize(); vert++)
	{
		normalize(m_VertexArray[vert].Normal);
	}
	
	return TRUE;
}

void CMesh::Destroy()
{
	m_SubMeshArray.RemoveAll();
	m_VertexArray.RemoveAll();
}

void CMesh::Draw()
{
	int fragment;

/*	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof (VertexDesc), &(m_VertexArray.GetData()->Coord));
	glTexCoordPointer(3, GL_FLOAT, sizeof (VertexDesc), &(m_VertexArray.GetData()->TexCoord));
*/
	for (fragment=0; fragment<m_SubMeshArray.GetSize(); fragment++)
	{
		DrawFragment(fragment);
	}
	

/*	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	*/
}

void CMesh::DrawFragment(int Fragment)
{
	int face, vert;

	// bind fragment material
	if (m_SubMeshArray[Fragment].MaterialId>=0)
	{
		m_pParentObject->GetMaterial(m_SubMeshArray[Fragment].MaterialId).Bind();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	glColor3ub(255,255,255);

	glBegin(GL_TRIANGLES);
	for (face=0; face<m_SubMeshArray[Fragment].FaceArray.GetSize(); face++)
	{
		for (vert=0; vert<3; vert++)
		{
			int vertexId = m_SubMeshArray[Fragment].FaceArray[face].VertexIndices[vert];
			glTexCoord2fv(m_VertexArray[vertexId].TexCoord.vec_array);
			glNormal3fv(m_VertexArray[vertexId].Normal.vec_array);
			glVertex3fv(m_VertexArray[vertexId].Coord.vec_array);
		}
	}
	glEnd();
}

void CMesh::SetParent(C3dObject *pParent)
{
	m_pParentObject = pParent;
}

void CMesh::DrawBoundingBox()
{
	const int VertKoefs[6][4][3] = 
	{
		{{0,0,0},{0,1,0},{1,1,0},{1,0,0}},
		{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

		{{0,0,0},{0,0,1},{1,0,1},{1,0,0}},
		{{0,1,0},{0,1,1},{1,1,1},{1,1,0}},

		{{0,0,0},{0,0,1},{0,1,1},{0,1,0}},
		{{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
	};
	if (!m_VertexArray.GetSize())
	{
		return;
	}
	m_BoundingBox.Draw();
}

BOOL CMesh::Save(CFile &file)
{
	// write mesh name
	WRITE_STR(file, m_Name);
	// write bounding box
	WRITE_BIN(file, m_BoundingBox);
	// write vertex number
	WRITE_SIZE(file, m_VertexArray);
	// write vertex data
	WRITE_ARR(file, m_VertexArray);
	// write sub mesh number
	WRITE_SIZE(file, m_SubMeshArray);
	// write all sub_meshes
	for (int fragment=0; fragment<m_SubMeshArray.GetSize(); fragment++)
	{
		// write material name
		WRITE_STR(file, m_SubMeshArray[fragment].MaterialName);
		// write material index
		WRITE_BIN(file, m_SubMeshArray[fragment].MaterialId);
		// write number of faces in subset
		WRITE_SIZE(file, m_SubMeshArray[fragment].FaceArray);
	}
	
	for (fragment=0; fragment<m_SubMeshArray.GetSize(); fragment++)
	{
		// write all faces in subset
		for (int face=0; face<m_SubMeshArray[fragment].FaceArray.GetSize(); face++)
		{
			// write vertex indices
			WRITE_BIN(file, (m_SubMeshArray[fragment].FaceArray[face].VertexIndices));
		}
	}
	return TRUE;
}

void CMesh::DrawVertexNormals(float NormalLength)
{

	int vert;
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for (vert=0; vert<m_VertexArray.GetSize(); vert++)
	{
		vec3 normal_end(m_VertexArray[vert].Normal);
		normal_end*=NormalLength;
		normal_end+=m_VertexArray[vert].Coord;
		glVertex3fv(m_VertexArray[vert].Coord.vec_array);
		glVertex3fv(normal_end.vec_array);
	}
	glEnd();
}

void BoundingBox::Draw()
{
	const int VertKoefs[6][4][3] = 
	{
		{{0,0,0},{0,1,0},{1,1,0},{1,0,0}},
		{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},

		{{0,0,0},{0,0,1},{1,0,1},{1,0,0}},
		{{0,1,0},{0,1,1},{1,1,1},{1,1,0}},

		{{0,0,0},{0,0,1},{0,1,1},{0,1,0}},
		{{1,0,0},{1,0,1},{1,1,1},{1,1,0}},
	};
	GLboolean cf = glIsEnabled(GL_CULL_FACE);
	int pm[2];
	glGetIntegerv(GL_POLYGON_MODE, pm);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);
	for (int quad=0; quad<6; quad++)
	{
		for (int vert=0; vert<4; vert++)
		{
			vec3 CurVertex (VertKoefs[quad][vert][0]?MaxCoords.x:MinCoords.x,
							VertKoefs[quad][vert][1]?MaxCoords.y:MinCoords.y,
							VertKoefs[quad][vert][2]?MaxCoords.z:MinCoords.z);
			glVertex3fv(CurVertex.vec_array);
		}
	}
	glEnd();

	glPolygonMode(GL_FRONT, pm[0]);
	glPolygonMode(GL_BACK, pm[1]);
	if (cf)
	{
		glEnable(GL_CULL_FACE);
	}

}
