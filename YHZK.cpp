// YHZK.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "YHZK.h"
#include "YHZKDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYHZKApp

BEGIN_MESSAGE_MAP(CYHZKApp, CWinApp)
	//{{AFX_MSG_MAP(CYHZKApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYHZKApp construction

CYHZKApp::CYHZKApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CYHZKApp object

CYHZKApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CYHZKApp initialization

BOOL CYHZKApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CYHZKDlg dlg;
	m_pMainWnd = &dlg;

	SetDialogBkColor(TITLE_COLOR,TEXT_COLOR);

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
//	GetDlgItem(IDC_BTN_LOG_IN)->ShowWindow(FALSE);
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
