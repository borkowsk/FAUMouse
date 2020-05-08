// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   init.c
//
//  PURPOSE:   Performs application and instance specific initialization.
//
//  FUNCTIONS:
//    InitApplication() - Initializes window data and registers window.
//    InitInstance() - Saves instance handle and creates main window.
//
//  COMMENTS:
// 

#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "resource.h"

HINSTANCE WB_Instance=0;                // current instance
HINSTANCE hInst=0;
HWND    WB_Hwnd=0;					// current main

char szAppName[9];              // The name of this application
char szTitle[40];               // The title bar text

static int ScreenX;
static int ScreenY;

//
//  FUNCTION: InitApplication(HINSTANCE)
//
//  PURPOSE: Initializes window data and registers window class.
//
//  PARAMETERS:
//    hInstance - The handle to the instance of this application that
//          is currently being executed.
//
//  RETURN VALUE:
//    TRUE - Success
//    FALSE - Initialization failed
//
//  COMMENTS:
//
//    This function is called at initialization time only if no other
//    instances of the application are running.  This function performs
//    initialization tasks that can be done once for any number of running
//    instances.
//
//    In this case, we initialize a window class by filling out a data
//    structure of type WNDCLASS and calling the Windows RegisterClass()
//    function.  Since all instances of this application use the same
//    window class, we only need to do this when the first instance is
//    initialized.
//

BOOL InitApplication(HINSTANCE hInstance)
{
    #ifdef WIN16
    WNDCLASS  wc;
    #else
    WNDCLASSEX wc;
    #endif

    // Load the application name and description strings.
	WB_Instance=hInstance;
    int testrc=LoadString(hInstance, IDS_APPNAME, szAppName, sizeof(szAppName));
    LoadString(hInstance, IDS_DESCRIPTION, szTitle, sizeof(szTitle));
    char* IconStr=MAKEINTRESOURCE(IDI_APPICON);
    // Fill in window class structure with parameters that describe the
    // main window.

    #ifndef WIN16
    wc.cbSize        = sizeof(WNDCLASSEX);
	wc.hIconSm       = (HICON)LoadImage(hInstance,		// Load small icon image
	                             IconStr,
	                             IMAGE_ICON,
	                             16, 16,
	                             0);
    #endif
    wc.style         = CS_HREDRAW | CS_VREDRAW;// Class style(s).
    wc.lpfnWndProc   = (WNDPROC)WndProc;        // Window Procedure
    wc.cbClsExtra    = 0;                       // No per-class extra data.
    wc.cbWndExtra    = 0;                       // No per-window extra data.
    wc.hInstance     = hInstance;               // Owner of this class
    wc.hIcon         = LoadIcon(hInstance, IconStr); // Icon name from .RC
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Cursor
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default color
    wc.lpszMenuName  = NULL;//szAppName;               // Menu name from .RC
    wc.lpszClassName = szAppName;               // Name to register as

    // Register the window class and return FALSE if unsuccesful.

#ifdef WIN16
    if (!RegisterClass(&wc))
    {
        goto F_ERROR ;
    }
    
#else 

    if ( !RegisterClassEx(&wc) )
    {
        if ( !RegisterClass((LPWNDCLASS)&wc.style) )

           goto F_ERROR ;
    }

#endif

    //
    // **TODO** Call module specific application initialization functions here.
    //
	

    return TRUE;

	F_ERROR:

	MessageBox(0,"Can't register the window class","Initialisation error",MB_ICONERROR);
	
	return FALSE;

}


//
//  FUNCTION:  InitInstance(HINSTANCE, int)
//
//  PURPOSE:  Saves instance handle and creates main window.
//
//  PARAMTERS:
//    hInstance - The handle to the instance of this application that
//          is currently being executed.
//    nCmdShow - Specifies how the main window is to be diplayed.
//
//  RETURN VALUE:
//    TRUE - Success
//    FALSE - Initialization failed
//
//  COMMENTS:
//    This function is called at initialization time for every instance of
//    this application.  This function performs initialization tasks that
//    cannot be shared by multiple instances.
//
//    In this case, we save the instance handle in a static variable and
//    create and display the main program window.
//
#ifdef _DEBUG
#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW | WS_MAXIMIZE)
#else
#define WINDOW_STYLE	(WS_OVERLAPPED | WS_MAXIMIZE) 				
#endif

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   
	HWND    hwnd; // Main window handle.

	
	// Save the instance handle in static variable, which will be used in
    // many subsequence calls from this application to Windows.

    hInst = hInstance; // Store instance handle in our global variable

    // Create a main window for this application instance.
    hwnd = CreateWindow(szAppName,           // See RegisterClass() call.
						szTitle,             // Text for window title bar.
						WINDOW_STYLE,
                        0, 0,    
                        ScreenX, ScreenY,   
						//CW_USEDEFAULT,0,
                        NULL,                // Overlapped has no parent.
                        NULL,                // Use the window class menu.
                        hInstance,           
                        NULL);               

    // If window could not be created, return "failure"
    if (!hwnd)
        goto F_ERROR;

	WB_Hwnd=hwnd;/*Save to global varaiable */

    //
    // **TODO** Call module specific instance initialization functions here.
    //

    // **INPUT** Initialize the input module.
    if (!InitInput(hwnd))
    {
        goto F_ERROR;
    }

    // Make the window visible; update its client area; and return "success"
    ShowWindow(hwnd,SW_MAXIMIZE); // Show the window
    UpdateWindow(hwnd);         // Sends WM_PAINT message
 
    return TRUE;                // We succeeded...

F_ERROR:
	MessageBox(0,"Can't open window","Initialisation error",MB_ICONERROR);
	return FALSE;
}
