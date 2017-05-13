// Mesh.h: interface for the CMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH_H__78D5AFFF_1365_492F_A08D_25EB237F0E56__INCLUDED_)
#define AFX_MESH_H__78D5AFFF_1365_492F_A08D_25EB237F0E56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ��������� ������� ����
struct VertexDesc
{
	vec3	Coord;
	vec3	Normal;
	vec2	TexCoord;
};

struct FaceDesc
{
	USHORT	VertexIndices[3];	// ������ ������ �����
	UINT	SmoothGroup;
	UINT	Flag;
};

// ������ ���������� � ��������� ����
struct MeshFragment
{
	CString MaterialName;	// ��� ���������
	int		MaterialId;		// ������ ���������
	CArray	<FaceDesc, FaceDesc const &> FaceArray;	// ������ ������
};

// axis oriented bounding box
struct BoundingBox
{
	vec3	MinCoords;
	vec3	MaxCoords;
public:
	void Draw();
};

class C3dObject;

class CMesh  
{
protected:
	C3dObject *m_pParentObject;	// ��������� �� ��������
	CArray <VertexDesc, VertexDesc const &> m_VertexArray;		// ������ ������
	CArray <MeshFragment, MeshFragment const& > m_SubMeshArray;	// ������ ��������
	void DrawFragment(int Fragment);
public:
	BoundingBox m_BoundingBox;

	void DrawVertexNormals(float NormalLength=1.0f);
	BOOL Save(CFile &file);
	CString	m_Name;	// ��� ����
	void DrawBoundingBox();
	void SetParent(C3dObject *pParent);
	
	void Draw();
	void Destroy();
	BOOL ReadMesh3ds(mesh3ds const& Mesh);
	CMesh();
	virtual ~CMesh();

};

#endif // !defined(AFX_MESH_H__78D5AFFF_1365_492F_A08D_25EB237F0E56__INCLUDED_)
