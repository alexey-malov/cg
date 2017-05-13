// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Export3ds.h"
#include "ChildView.h"
#include "OpenGL.h"
#include "ObjectPropertiesDialog.h"

#define ANIMATION_TIMER	1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView():
	m_IsViewObject(TRUE),
	m_IsViewBoundingBox(FALSE),
	m_IsViewNormals(FALSE),
	m_BackgroundColor(RGB(0,0,0)),
	m_IsLighting(TRUE)
{
	
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD_MODEL, OnFileLoadModel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_BOUNDINGBOXES, OnViewBoundingboxes)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOUNDINGBOXES, OnUpdateViewBoundingboxes)
	ON_COMMAND(ID_VIEW_OBJECT, OnViewObject)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECT, OnUpdateViewObject)
	ON_COMMAND(ID_VIEW_OBJECTPROPERTIES, OnViewObjectproperties)
	ON_COMMAND(ID_FILE_SAVE_MODEL, OnFileSavemodel)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_NORMALS, OnViewNormals)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NORMALS, OnUpdateViewNormals)
	ON_COMMAND(ID_VIEW_BACKGROUNDCOLORCHANGE, OnViewBackgroundcolorchange)
	ON_COMMAND(ID_VIEW_LIGHTING, OnViewLighting)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIGHTING, OnUpdateViewLighting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	DrawGL();
	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!InitGL())
		return -1;
	SetTimer(ANIMATION_TIMER, 20, 0);
	return 0;
}

BOOL CChildView::InitGL()
{
	m_hgldc = ::GetDC(m_hWnd);
	// setup pixel format
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_GENERIC_ACCELERATED | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cStencilBits = 8;
	pfd.cDepthBits = 24;
	
	int iPixelMode = ChoosePixelFormat(m_hgldc, &pfd);
	if (!iPixelMode) 
		return FALSE;
	if (!SetPixelFormat(m_hgldc, iPixelMode, &pfd))
		return FALSE;

	m_hglrc = wglCreateContext(m_hgldc);
	if (!m_hglrc)
		return FALSE;

	if (!wglMakeCurrent(m_hgldc, m_hglrc)) 
		return FALSE;

	if (!InitExtensions())
		return FALSE;

	
	return TRUE;
}

void CChildView::OnDestroy() 
{
	CWnd ::OnDestroy();

	KillTimer(ANIMATION_TIMER);
	// TODO: Add your message handler code here
	DestroyOpenGL();
}

void CChildView::DestroyOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hglrc);
}

void CChildView::DrawGL()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glClearColor(GetRValue(m_BackgroundColor)/255.0f, 
				GetGValue(m_BackgroundColor)/255.0f,
				GetBValue(m_BackgroundColor)/255.0f,
				1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,0, 0,0,-1, 0,1,0);

	DrawObject();

	glFlush();
	SwapBuffers(m_hgldc);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CWnd ::OnEraseBkgnd(pDC);
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	ResizeGL(rc.Width(), rc.Height());
}

void CChildView::ResizeGL(int cx, int cy)
{
	int matrixmode;
	glGetIntegerv(GL_MATRIX_MODE, &matrixmode);
	glMatrixMode(GL_PROJECTION);
	glViewport(0,0, cx,cy);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)cx/cy, 1, 1000);
	glMatrixMode(matrixmode);
}

void CChildView::OnFileLoadModel() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, NULL,
					NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					"3d studio files (*.3ds)|*.3ds", this);
	
	if (dlg.DoModal()==IDOK)
	{
		CString st = dlg.GetPathName();
		Object3d.m_DirectoryName = st.Left(st.GetLength()-dlg.GetFileName().GetLength());
		Object3d.Load3ds((LPCTSTR)st);
	}
	RedrawWindow();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	
	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	
	CWnd ::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnViewBoundingboxes() 
{
	// TODO: Add your command handler code here
	m_IsViewBoundingBox = !m_IsViewBoundingBox;
	RedrawWindow();
}

void CChildView::OnUpdateViewBoundingboxes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_IsViewBoundingBox);
}

void CChildView::DrawObject()
{

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	if (m_IsLighting)
	{
		glEnable(GL_LIGHTING);
	}
	glEnable(GL_LIGHT0);

	glPushMatrix();
		glLoadIdentity();
		const GLfloat LightPos[] = {0,0,1,0};
		const GLfloat LightDir[] = {0,0,-1};

		const GLfloat MatDif[] = {1,1,1,1};
		const GLfloat MatAmb[] = {1,1,1,1};
		const GLfloat MatSpec[] = {1,1,1,1};
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDir);
	glPopMatrix();

	Object3d.SetDistance(1.6f);
	glRotatef((GetTickCount()%3600)*0.1f, 1,0,1);
	Object3d.CenterToBoundingBox();

	if (m_IsViewObject)
	{
		Object3d.Draw();
	}
	glDisable(GL_LIGHTING);
	if (m_IsViewBoundingBox)
	{
		Object3d.DrawBoundingBoxes();
	}
	if (m_IsViewNormals)
	{
		glColor3ub(255, 0, 0);
		Object3d.DrawVertexNormals();
	}

}

void CChildView::OnViewObject() 
{
	// TODO: Add your command handler code here
	m_IsViewObject = !m_IsViewObject;
	RedrawWindow();
}

void CChildView::OnUpdateViewObject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_IsViewObject);
}

void CChildView::OnViewObjectproperties() 
{
	// TODO: Add your command handler code here
	CObjectPropertiesDialog PropertiesDialog;
	PropertiesDialog.m_p3dObject = &Object3d;
	PropertiesDialog.DoModal();
}

void CChildView::OnFileSavemodel() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE, "mdl", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Model files (*.mdl)|*.mdl");
	if (dlg.DoModal()==IDOK)
	{
		// save model
		Object3d.Save(dlg.GetPathName());
	}
}

void CChildView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	RedrawWindow();
	CWnd ::OnTimer(nIDEvent);
}

void CChildView::OnViewNormals() 
{
	// TODO: Add your command handler code here
	m_IsViewNormals = !m_IsViewNormals;
}

void CChildView::OnUpdateViewNormals(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_IsViewNormals);
}

void CChildView::OnViewBackgroundcolorchange() 
{
	// TODO: Add your command handler code here
	CColorDialog col(m_BackgroundColor, CC_ANYCOLOR|CC_FULLOPEN);
	if (col.DoModal()==IDOK)
	{
		m_BackgroundColor = col.GetColor();
	}
	RedrawWindow();
}

void CChildView::OnViewLighting() 
{
	// TODO: Add your command handler code here
	m_IsLighting = !m_IsLighting;
}

void CChildView::OnUpdateViewLighting(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_IsLighting);
}
