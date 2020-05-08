// MessageFont.h : main header file for the MESSAGEFONT application
//

#if !defined(AFX_MESSAGEFONT_H__AE87A1E4_9C49_4378_99C4_2451B04A4AEB__INCLUDED_)
#define AFX_MESSAGEFONT_H__AE87A1E4_9C49_4378_99C4_2451B04A4AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMessageFontApp:
// See MessageFont.cpp for the implementation of this class
//

class CMessageFontApp : public CWinApp
{
public:
	CMessageFontApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageFontApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMessageFontApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEFONT_H__AE87A1E4_9C49_4378_99C4_2451B04A4AEB__INCLUDED_)
