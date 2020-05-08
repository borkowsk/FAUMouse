// MessageFontDlg.h : header file
//

#if !defined(AFX_MESSAGEFONTDLG_H__8CD39DED_DFB0_4CEA_988B_DA2480CCD20B__INCLUDED_)
#define AFX_MESSAGEFONTDLG_H__8CD39DED_DFB0_4CEA_988B_DA2480CCD20B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMessageFontDlg dialog

class CMessageFontDlg : public CDialog
{
// Construction
public:
	CMessageFontDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageFontDlg)
	enum { IDD = IDD_MESSAGEFONT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageFontDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMessageFontDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonMessage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEFONTDLG_H__8CD39DED_DFB0_4CEA_988B_DA2480CCD20B__INCLUDED_)
