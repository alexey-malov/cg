// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__D53D235D_8BF8_4089_A0B4_A8342B625B7F__INCLUDED_)
#define AFX_CHILDVIEW_H__D53D235D_8BF8_4089_A0B4_A8342B625B7F__INCLUDED_

#include "3dObject.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoadModel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewBoundingboxes();
	afx_msg void OnUpdateViewBoundingboxes(CCmdUI* pCmdUI);
	afx_msg void OnViewObject();
	afx_msg void OnUpdateViewObject(CCmdUI* pCmdUI);
	afx_msg void OnViewObjectproperties();
	afx_msg void OnFileSavemodel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewNormals();
	afx_msg void OnUpdateViewNormals(CCmdUI* pCmdUI);
	afx_msg void OnViewBackgroundcolorchange();
	afx_msg void OnViewLighting();
	afx_msg void OnUpdateViewLighting(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	COLORREF m_BackgroundColor;
	BOOL m_IsViewObject;
	BOOL m_IsViewBoundingBox;
	BOOL m_IsViewNormals;
	BOOL m_IsLighting;
	C3dObject Object3d;
	void ResizeGL(int cx, int cy);
	void DrawGL();
	void DrawObject();
	void DestroyOpenGL();
	BOOL InitGL();
	HGLRC m_hglrc;
	HDC	m_hgldc;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__D53D235D_8BF8_4089_A0B4_A8342B625B7F__INCLUDED_)
