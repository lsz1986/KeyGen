// YHZK.h : main header file for the OWNDRAW application
//

#if !defined(AFX_OWNDRAW_H__EA62C9D0_DB6D_4BFF_8A06_1079F5DA0629__INCLUDED_)
#define AFX_OWNDRAW_H__EA62C9D0_DB6D_4BFF_8A06_1079F5DA0629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CYHZKApp:
// See YHZK.cpp for the implementation of this class
//

class CYHZKApp : public CWinApp
{
public:
	CYHZKApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYHZKApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CYHZKApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNDRAW_H__EA62C9D0_DB6D_4BFF_8A06_1079F5DA0629__INCLUDED_)
