; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CObjectPropertiesDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Export3ds.h"
LastPage=0

ClassCount=5
Class1=CExport3dsApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class2=CChildView
Resource3=IDR_MAINFRAME (English (U.S.))
Resource4=IDD_ABOUTBOX (English (U.S.))
Class5=CObjectPropertiesDialog
Resource5=IDD_OBJECT_PROPERTIES_DIALOG

[CLS:CExport3dsApp]
Type=0
HeaderFile=Export3ds.h
ImplementationFile=Export3ds.cpp
Filter=N

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=W
BaseClass=CWnd 
VirtualFilter=WC
LastObject=ID_VIEW_LIGHTING

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_VIEW_BACKGROUNDCOLORCHANGE




[CLS:CAboutDlg]
Type=0
HeaderFile=Export3ds.cpp
ImplementationFile=Export3ds.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_LOAD_MODEL
Command2=ID_FILE_SAVE_MODEL
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_VIEW_OBJECT
Command11=ID_VIEW_BOUNDINGBOXES
Command12=ID_VIEW_NORMALS
Command13=ID_VIEW_LIGHTING
Command14=ID_VIEW_OBJECTPROPERTIES
Command15=ID_VIEW_BACKGROUNDCOLORCHANGE
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command4=ID_FILE_PRINT
Command5=ID_APP_ABOUT
CommandCount=5

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_VIEW_BOUNDINGBOXES
Command2=ID_EDIT_COPY
Command3=ID_VIEW_BACKGROUNDCOLORCHANGE
Command4=ID_VIEW_NORMALS
Command5=ID_FILE_LOAD_MODEL
Command6=ID_FILE_SAVE_MODEL
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_OBJECT_PROPERTIES_DIALOG]
Type=1
Class=CObjectPropertiesDialog
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_MESH_NUMBER,edit,1350633602
Control4=IDC_STATIC,static,1342308352
Control5=IDC_MESH_LIST,combobox,1344340226
Control6=IDC_STATIC,static,1342308352
Control7=IDC_FRAGMENT_NUMBER,edit,1350633602
Control8=IDC_VERTEX_NUMBER,edit,1350633602
Control9=IDC_STATIC,static,1342308352
Control10=IDC_FRAGMENTS_LIST,listbox,1352745219
Control11=IDC_BLOCK_COLOR,static,1342308352

[CLS:CObjectPropertiesDialog]
Type=0
HeaderFile=ObjectPropertiesDialog.h
ImplementationFile=ObjectPropertiesDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COLOR_RECT
VirtualFilter=dWC

