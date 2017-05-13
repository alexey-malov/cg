#if !defined(AFX_OBJECTPROPERTIESDIALOG_H__D80800CF_73AF_45D7_91F3_DB4309255267__INCLUDED_)
#define AFX_OBJECTPROPERTIESDIALOG_H__D80800CF_73AF_45D7_91F3_DB4309255267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectPropertiesDialog.h : header file
//

#include "3dObject.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectPropertiesDialog dialog

class CObjectPropertiesDialog : public CDialog
{
// Construction
public:
	CObjectPropertiesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectPropertiesDialog)
	enum { IDD = IDD_OBJECT_PROPERTIES_DIALOG };
	CStatic	m_BlockColor;
	CComboBox	m_MeshList;
	UINT	m_VertexNumber;
	UINT	m_FragmentsNumber;
	UINT	m_MeshNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectPropertiesDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_MeshIndexSelected;

	// Generated message map functions
	//{{AFX_MSG(CObjectPropertiesDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void BuildMeshList();
	C3dObject	*m_p3dObject;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTPROPERTIESDIALOG_H__D80800CF_73AF_45D7_91F3_DB4309255267__INCLUDED_)
