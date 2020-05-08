// MessageFontDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MessageFont.h"
#include "MessageFontDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CBTMessageBox( LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );
int AFXAPI CBTMessageBox( UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT) -1 );

	HHOOK hHook;	//	must be always != NULL
	CFont gl_Font;
	CStatic gl_Static;
//	Callback_HOOK_function:
//		system calls this function before activating, creating, 
//		destroying, minimizing, maximizing, moving, or sizing a window
LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	HWND  hParentWnd, hChildWnd;    // msgbox is "child"
	RECT  rParent, rChild, rDesktop;

	// notification that a window is about to be activated
	// window handle is wParam
	if (nCode == HCBT_ACTIVATE)
	{
		// set window handles
		hParentWnd = GetForegroundWindow();
		hChildWnd  = (HWND)wParam;

		if(	(hParentWnd != 0) &&
			(hChildWnd != 0) &&
			(GetWindowRect(GetDesktopWindow(), &rDesktop) != 0) &&
			(GetWindowRect(hParentWnd, &rParent) != 0) &&
			(GetWindowRect(hChildWnd, &rChild) != 0))
		{
			if(gl_Font.m_hObject)
				gl_Font.DeleteObject();
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
			lf.lfHeight = 8+rand()%16;             // request a random-pixel-height font
			strcpy(lf.lfFaceName, "Arial");        // request a face name "Arial"
			VERIFY(gl_Font.CreateFontIndirect(&lf));  // create the font

			HWND hCh = ::GetWindow(hChildWnd, GW_CHILD);
			while(hCh)
			{
				TCHAR szClassName[16];
				if(::GetClassName(hCh, szClassName, 16) && _tcsicmp(szClassName, _T("Static")) == 0)
				{
					//	change font for all static controls
					CStatic* pStatic = (CStatic*)CWnd::FromHandle(hCh);
					pStatic->SetFont(&gl_Font);
					//CDC* dc=pStatic->GetDC();
					//CSize sc=GetTextExtent(LPCTSTR lpszString, int nCount);
					//... SPRAWDZENIE ROZMIARU TEKSTU
				}
				hCh = ::GetNextWindow(hCh, GW_HWNDNEXT);
			}
		}

		// exit CBT hook
		//
		UnhookWindowsHookEx(hHook); //!!!!
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	// otherwise, continue with any possible chained hooks
	else CallNextHookEx(hHook, nCode, wParam, lParam);
	return 0;
}

int CBTMessageBox( LPCTSTR lpszText, UINT nType/* = MB_OK*/, UINT nIDHelp/* = 0 */)
{
	//	08.04.2003 15:50:07		Victor
	//	do the hook to change button titles
	hHook = SetWindowsHookEx(WH_CBT, &CBTProc, 0, GetCurrentThreadId());
	return AfxMessageBox(lpszText, nType, nIDHelp);
}

int AFXAPI CBTMessageBox( UINT nIDPrompt, UINT nType/* = MB_OK*/, UINT nIDHelp/* = (UINT) -1*/ )
{
	//	08.04.2003 15:50:07		Victor
	//	do the hook to change button titles
	hHook = SetWindowsHookEx(WH_CBT, &CBTProc, 0, GetCurrentThreadId());
	return AfxMessageBox(nIDPrompt, nType, nIDHelp);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageFontDlg dialog

CMessageFontDlg::CMessageFontDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageFontDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageFontDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMessageFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageFontDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMessageFontDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageFontDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MESSAGE, OnButtonMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageFontDlg message handlers

BOOL CMessageFontDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMessageFontDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMessageFontDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMessageFontDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMessageFontDlg::OnButtonMessage() 
{
	CBTMessageBox	(_T("HOOK Font - different size test"));
}
