// ObjectPropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Export3ds.h"
#include "ObjectPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertiesDialog dialog


CObjectPropertiesDialog::CObjectPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectPropertiesDialog::IDD, pParent),
	m_MeshIndexSelected(-1)
{
	//{{AFX_DATA_INIT(CObjectPropertiesDialog)
	m_VertexNumber = 0;
	m_FragmentsNumber = 0;
	m_MeshNumber = 0;
	//}}AFX_DATA_INIT
}


void CObjectPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectPropertiesDialog)
	DDX_Control(pDX, IDC_BLOCK_COLOR, m_BlockColor);
	DDX_Control(pDX, IDC_MESH_LIST, m_MeshList);
	DDX_Text(pDX, IDC_VERTEX_NUMBER, m_VertexNumber);
	DDX_Text(pDX, IDC_FRAGMENT_NUMBER, m_FragmentsNumber);
	DDX_Text(pDX, IDC_MESH_NUMBER, m_MeshNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectPropertiesDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertiesDialog message handlers

BOOL CObjectPropertiesDialog::OnInitDialog() 
{
	// TODO: Add extra initialization here
	ASSERT(m_p3dObject);

	m_MeshNumber = m_p3dObject->m_MeshArray.GetSize();
	if (m_MeshNumber)
	{
		m_MeshIndexSelected = 0;
	}

	CDialog::OnInitDialog();

	BuildMeshList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CObjectPropertiesDialog::BuildMeshList()
{
	for (UINT mesh=0; mesh < m_MeshNumber; mesh++)
	{
		int index = m_MeshList.AddString((LPCTSTR)m_p3dObject->m_MeshArray[mesh].m_Name);
		m_MeshList.SetItemData(index, mesh);
	}
}
