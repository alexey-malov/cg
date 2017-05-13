// 01_hello_worldView.h : interface of the CMy01_hello_worldView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_01_HELLO_WORLDVIEW_H__92B95A2B_0559_489D_BEB1_E149A0F3B63B__INCLUDED_)
#define AFX_01_HELLO_WORLDVIEW_H__92B95A2B_0559_489D_BEB1_E149A0F3B63B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMy01_hello_worldView : public CWindowImpl<CMy01_hello_worldView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CMy01_hello_worldView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_01_HELLO_WORLDVIEW_H__92B95A2B_0559_489D_BEB1_E149A0F3B63B__INCLUDED_)
