
//   COMMENTS:
//    Message dispatch table -
//      For every message to be handled by the main window procedure
//      place the message number and handler function pointer in
//      rgmsd (the message dispatch table).  Place the prototype
//      for the function in globals.h and the definition of the
//      function in the appropriate module.
//    Globals.h Contains the definitions of the structures and dispatch.c
//      contains the functions that use these structures.
//


#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <time.h>
#ifdef WIN16
#include <string.h>
#include "win16ext.h"           // required only for win16 applications
#endif
#include <mmsystem.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "dib.h"
#ifdef __cplusplus
}
#endif

#define  USES_config_resources
#define  USES_ifstream
#define  USES_STRING
#define  USES_IO
#define  USES_ERRNO
#include "uses_wb.hpp"

#include "globals.h"            // prototypes specific to this application
#include "resource.h"

#pragma pack( push, before_declare )
#pragma pack(2)
struct RegRec{
	double time;
	short int click;
	short int  X,Y;
	};
#pragma pack( pop, before_declare )

// Global variables. Program parameters
extern LPSTR CommandLine;
static RegRec* RegTab=NULL;
static size_t  position=0;
static char soundfile[256];
static char targetfile[256];
static char outputfile[256];
static char infofile[256];
static char prompt[1024];
static char description[1024];
static UINT AppendOutput=0;
static UINT TimerStep=100;
static UINT click=0;
static UINT score=0;
static UINT maxtime=0;
static  INT mousex=0;
static  INT mousey=0;
static  INT mouseb=0;
static  int ScreenX=0;
static  int ScreenY=0;
static COLORREF WindowBackground=0xff0000;

// Global variables. Handlers, counters etc

static UINT idTimer=0;     //  timer ID
static HANDLE dib=NULL;	   // Bacground bitmap handle	
static  INT mciOpen=0;          
static int  nTimerCount = 0;        //  current timer count
static int  nScoreCount = 0;        //  current score count
#define TIMERID ((UINT) 't')

//extern wb_about(char*);
#include "autor.h"

void UpdateStatusLine(HWND hwnd)
{
	char bigbufor[1024];
	char bufor[18];
   wsprintf(
            bigbufor,
            "%s x=%d y=%d buttons=%d clock=%d ms %s%s",
			prompt,
			mousex,
			mousey,
			mouseb,
            nTimerCount,
			score?"score=":"",
			score?ltoa(nScoreCount,bufor,10):""
        );

   SetWindowText(hwnd,bigbufor);
}


static char MouseText[48];         //  mouse state
static char ButtonText[48];        //  mouse-button state
static char KeyboardText[48];      //  keyboard state
static char CharacterText[48];     //  latest character
static char ScrollText[48];        //  scroll status
/*
static char TimerText[1024];         //  timer state
static RECT rectMouse;
static RECT rectButton;
static RECT rectKeyboard;
static RECT rectCharacter;
static RECT rectScroll;
static RECT rectTimer;
*/

//LRESULT MsgErasebkgnd(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);


// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND,        MsgCommand},
    {WM_CREATE,         MsgCreate},
    {WM_DESTROY,        MsgDestroy},
    {WM_MOUSEMOVE,      MsgMouseMove},
    {WM_LBUTTONDOWN,    MsgLButtonDown},
    {WM_LBUTTONUP,      MsgLButtonUp},
    {WM_LBUTTONDBLCLK,  MsgLButtonDoubleClick},
    {WM_RBUTTONDOWN,    MsgRButtonDown},
    {WM_RBUTTONUP,      MsgRButtonUp},
    {WM_RBUTTONDBLCLK,  MsgRButtonDoubleClick},
 //   {WM_KEYDOWN,        MsgKeyDown},
 //   {WM_KEYUP,          MsgKeyUp},
 //   {WM_CHAR,           MsgChar},
    {WM_TIMER,          MsgTimer},
//    {WM_HSCROLL,        MsgScroll},
//    {WM_VSCROLL,        MsgScroll},	
//	{WM_ERASEBKGND, MsgErasebkgnd},
    {WM_PAINT,          MsgPaint}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages
//
//  PARAMETERS:
//    hwnd - The window handle
//    uMessage - WM_COMMAND (unused)
//    GET_WM_COMMAND_ID(wparam,lparam) - The command number
//
//  RETURN VALUE:
//    Depends on the command.
//
//  COMMENTS:
//
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    LRESULT lRet = 0;

    // Message packing of wparam and lparam have changed for Win32,
    // so use the GET_WM_COMMAND macro to unpack the commnad

    switch (GET_WM_COMMAND_ID(wparam,lparam))
    {
        //
        // **TODO** Add cases here for application specific command messages.
        //

        case IDM_EXIT:
            DestroyWindow (hwnd);
            break;

        default:
            lRet = DefWindowProc(hwnd, uMessage, wparam, lparam);
    }

    return lRet;
}

//Function to Display info file
void InfoFileBox(HWND hwnd,char* infofile)
{
char* text=	NULL;
// "a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z\n"
//"b\nc\nd\ne\nf\ng\nh\n\i\nj\nk\nl\nm\nn\no\np\nq\nr\ns\nt\nu\nv\nw\nx\ny\nz\n";
size_t size=0;
FILE* file=fopen(infofile,"rb");
if(file==NULL)
	goto LERROR;
fseek(file,0,SEEK_END);
size=ftell(file);
fseek(file,0,SEEK_SET);
text=new char[size+2];
if(text==NULL)
	goto LERROR;
fread(text,1,size,file);
text[size]='\0';
fclose(file);
MessageBox(0,text,"Read information!",MB_ICONINFORMATION);
delete text;
return;
LERROR:
{
char bufor[1024];
sprintf(bufor,"\"%s\" on file \"%s\" ",infofile,strerror(errno));
MessageBox(0,bufor,"Error reading information file",MB_ICONERROR);
}
if(file) fclose(file);
if(text) delete text;
SendMessage(hwnd,WM_CLOSE,0,0);
}

int PlayInfoSound(HWND hwnd,char* soundfile)
{
DWORD ret;
#ifdef _DEBUG
return 0;
#endif
char bufor[1024]="\0";	
char lpszReturnString[128]="\0";
char lpszErrorString[128]="\0";
sprintf(bufor,"open %s type waveaudio alias finch",soundfile);
ret=mciSendString(bufor,lpszReturnString, sizeof(lpszReturnString)-1, NULL);
if(ret) goto LERROR;
ret=mciSendString("set finch time format samples", lpszReturnString, 
				sizeof(lpszReturnString)-1, NULL);
if(ret) goto LERROR;
ret=mciSendString("play finch", lpszReturnString, 
				sizeof(lpszReturnString)-1, NULL);
if(ret) goto LERROR;
return 1;
LERROR:
if(!mciGetErrorString(ret,lpszErrorString, sizeof(lpszErrorString)-1))
			*lpszErrorString='\0';
sprintf(bufor,"\"%s\":%s on file \"%s\" ", lpszReturnString,lpszErrorString,soundfile);
return 0;
}

int AllocTable(HWND hwnd)
{
//32 bity musza byc!
assert(sizeof(size_t)>=4);
maxtime*=1000;
size_t size=(maxtime/TimerStep)+10;//z rezerwa
RegTab=new RegRec[size];
if(RegTab!=NULL)
	return 1;
LERROR:
MessageBox(0,strerror(errno),"Error saving data",MB_ICONERROR);
SendMessage(hwnd,WM_CLOSE,0,0);
return 0;
}

int SaveDataAndCleanup(HWND hwnd)
{
char bufor[512];
size_t i=0;
FILE *file;
//printf("%s  %2.4gs. ",danker,GetCounter());
if(RegTab==NULL || position==0)
	{
	sprintf(bufor,"%s","Nothing to save!");
	goto LERROR;
	}

if(AppendOutput)
	{
	if((file=fopen(outputfile,"at"))==NULL)
		{
		sprintf(bufor,"Fatal! Can't store data! %s",strerror(errno));
		goto LERROR;
		}
	}
	else
	{
	if((file=fopen(outputfile,"wt"))==NULL)
		{
		sprintf(bufor,"Fatal! Can't store data! %s",strerror(errno));
		goto LERROR;
		}
	}

struct tm *newtime;
time_t aclock;
time( &aclock );                 /* Get time in seconds */
newtime = localtime( &aclock );  /* Convert time to struct */
                                    /* tm form */
 /* Print local time as a string */
fprintf(file,"#EXP\t%s",asctime( newtime ) );
fprintf(file,"%s\n",description);
fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\n","X","Y","TIME","CLICK");

for(i=0;i<position;i++)
	fprintf(file,"%d\t%d\t%g\t%d\n",
		RegTab[i].X,
		RegTab[i].Y,
		RegTab[i].time,
		RegTab[i].click);

fclose(file);
delete RegTab;
return 1;
LERROR:
MessageBox(0,bufor,"Error saving data",MB_ICONERROR);
return 0;
}


//
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Set the timer for five-second intervals
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_CREATE      (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{ 
	char lpszReturnString[128]="\0";
	int marker=0xabef;
	char* emptypar="";  
	HDC		wDc=GetDC(hwnd);
	ScreenX=GetDeviceCaps(wDc,HORZRES);
	ScreenY=GetDeviceCaps(wDc,VERTRES);
	ReleaseDC(hwnd,wDc);

	ifstream config_input(CommandLine,ios::nocreate);
	if(config_input.is_open()==0)
		{
		sprintf(lpszReturnString,"Can't open parameters file \"%s\" ",CommandLine);
		goto L_ERROR;
		}
		else
		{
		config_resources params(config_input,1,&emptypar);//Nie bierze lini polecen!
		config_input.close();
		if(strcmp("Yes", params("logo") )==0)
				wb_about("Windows Mouse Registrator");
		
		if((WindowBackground=strtoul( params("background"),NULL,0 ))==0)
						WindowBackground=0x0f0f0f;
		if((TimerStep=strtoul( params("step"),NULL,0 ))<50)
			{
			sprintf(lpszReturnString,"The step value  %ld is too small.",TimerStep);
			assert(marker==0xabef);
			goto L_ERROR;
			}
		if((maxtime=strtoul( params("maxtime"),NULL,0 ))*1000<TimerStep)
			{
			sprintf(lpszReturnString,"The maximum time value  %ld is too small.",TimerStep);
			assert(marker==0xabef);
			goto L_ERROR;
			}
		strncpy(soundfile,params("sound"),sizeof(soundfile));
		strncpy(targetfile,params("target"),sizeof(targetfile));
		strncpy(outputfile,params("output"),sizeof(outputfile));
		strncpy(infofile,params("information"),sizeof(infofile));
		strncpy(description,params("description"),sizeof(description));
		if(access(outputfile,2)==-1 && errno==EACCES)
			{
			sprintf(lpszReturnString,"File %s has't write permission",outputfile);
			assert(marker==0xabef);
			goto L_ERROR;
			}
		strncpy(prompt,params("prompttext"),sizeof(prompt));
		click=strcmp("Yes",params("click"))==0;
		score=strcmp("Yes",params("score"))==0;
		AppendOutput=strcmp("Yes",params("append"))==0;

		//Prepare window for experiment
		//------------------------------------	
		SetWindowText(hwnd,prompt);
		//Display info text

		if(!AllocTable(hwnd))
				return 0;

		if(strlen(infofile)>0)
			InfoFileBox(hwnd,infofile);
		
		if(strlen(targetfile)>0)
			{
			dib=OpenDIB(targetfile);
			if(dib==NULL)
					{
					sprintf(lpszReturnString,"Can't open bitmap file \"%s\" ",targetfile);
					assert(marker==0xabef);
					goto L_ERROR;
					}
			}

		//Start MCI sound
		if(strlen(soundfile)>0)
			mciOpen=PlayInfoSound(hwnd,soundfile);		
	
		// Set the timer for   TimerStep intervals
		mousex=ScreenX/2;
		mousey=ScreenY/2;
		RegTab[0].X=mousex;
		RegTab[0].Y=mousey;
		RegTab[0].time=RegTab[0].click=0;
		position=1;
		idTimer =  SetTimer(hwnd, TIMERID, TimerStep, NULL);
		SetCursorPos(mousex,mousey);
		}

	return 0;

L_ERROR:
MessageBox(0,lpszReturnString,"Error reading parameters",MB_ICONERROR);
DestroyWindow(hwnd);
return 0;
}

//
//  FUNCTION: MsgPaint(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Draw the strings for current messages.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_PAINT (Unused)
//    wparam    - Extra data (Unused)
//    lparam    - Extra data (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
	BITMAPINFOHEADER info;
    RECT rect;
    HDC hdc = BeginPaint(hwnd, &ps);
	HANDLE brush=CreateSolidBrush(WindowBackground);
 
	SetBkColor(hdc,WindowBackground);
    //MessageBeep(MB_ICONQUESTION);
	FillRect (hdc,&ps.rcPaint,brush ); 
	DeleteObject(brush);

	if(dib==NULL)
			{
			Ellipse(hdc,ScreenX/2-ScreenX/4,
						ScreenY/2-ScreenY/4,
						ScreenX/2+ScreenX/4,
						ScreenY/2+ScreenY/4);
			}
			else
			{
			DibInfo(dib,&info);
			
			/*
			if(rc.bottom<info.biHeight)
					rc.bottom=info.biHeight;
			if(rc.right<info.biWidth)
					rc.right=info.biWidth;
			small_rc.top=(rand()/(double)RAND_MAX)*(rc.bottom-info.biHeight)-15;
			small_rc.left=(rand()/(double)RAND_MAX)*(rc.right-info.biWidth)-15;
			small_rc.right=small_rc.left+info.biWidth+30;
			small_rc.bottom=small_rc.top+info.biHeight+30;	
			*/
         
		    DibBlt(hdc,ScreenX/2-info.biWidth/2,
						ScreenY/2-info.biHeight/2,
				//small_rc.left+15,small_rc.top+15,
				info.biWidth,info.biHeight,
				dib,0,0,SRCINVERT //Use only on error!
				);
			}
			
	/*
    if (IntersectRect(&rect, &rectMouse, &ps.rcPaint))
        TextOut(
            hdc,
            rectMouse.left, rectMouse.top,
            MouseText, strlen(MouseText)
        );
	*/
    EndPaint(hwnd, &ps);

    return 0;
}



//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Kills the timer and posts the quit message.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_DESTROY (Unused)
//    wparam    - Extra data (Unused)
//    lparam    - Extra data (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	char	lpszReturnString[128];
	
	if(RegTab)
		SaveDataAndCleanup(hwnd);
  
    if(mciOpen)
		mciSendString("close finch", lpszReturnString, 
		sizeof(lpszReturnString)-1, NULL);

    if(dib)
		GlobalFree(dib);		//Free bacground bitmap

    KillTimer(hwnd, idTimer);       // Stops the timer

    PostQuitMessage(0);

	 return 0;
}

//
//  FUNCTION: MsgTimer(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display timer message and a current time.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_TIMER (Unused)
//    wparam    - The timer identifier
//    lparam    - NULL           (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgTimer(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if ( wparam == TIMERID ) 
		{
		nTimerCount += TimerStep;
		UpdateStatusLine(hwnd);
		RegRec* rec=(RegRec*)(RegTab+position);
		
		if(maxtime<nTimerCount)
				{
				SetWindowText(hwnd,prompt);
				SendMessage(hwnd,WM_CLOSE,0,0);
				return 0;
				}

		rec->time=nTimerCount/1000.0;
		rec->click=mouseb;
		rec->X=mousex;
		rec->Y=mousey;
		position++;

		//		InvalidateRect(hwnd, &rectTimer, TRUE);

		
		}

    return 0;
}



//
//  FUNCTION: MsgMouseMove(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display mouse move message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_MOUSEMOVE (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgMouseMove(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{

	mousex= LOWORD(lparam);
	mousey= HIWORD(lparam);
    wsprintf(
        MouseText,
        "WM_MOUSEMOVE: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
   // InvalidateRect(hwnd, &rectMouse, TRUE);

    return 0;
}


//
//  FUNCTION: MsgLButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display left mouse button down message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONDOWN (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgLButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 wsprintf(
		  ButtonText,
		  "WM_LBUTTONDOWN: %x, %d, %d",
		  wparam, LOWORD(lparam), HIWORD(lparam)
	 );
	// InvalidateRect(hwnd, &rectButton, TRUE);
	mouseb|=0x1;
	
	if(score)
		{
		nScoreCount++;
		UpdateStatusLine(hwnd);
		}

	if(click)
		{
		SetWindowText(hwnd,prompt);
		SendMessage(hwnd,WM_CLOSE,0,0);
		}
		
    return 0;
}


//
//  FUNCTION: MsgLButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display left mouse button up message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONUP (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgLButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 wsprintf(
		  ButtonText,
		  "WM_LBUTTONUP: %x, %d, %d",
		  wparam, LOWORD(lparam), HIWORD(lparam)
	 );
  //  InvalidateRect(hwnd, &rectButton, TRUE);
	mouseb&=~0x1;
    return 0;
}

//
//  FUNCTION: MsgLButtonDoubleClick(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display left mouse button double click message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_LBUTTONDOBLECLICK (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgLButtonDoubleClick
	 (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 wsprintf(
		  ButtonText,
		  "WM_LBUTTONDBLCLK: %x, %d, %d",
		  wparam, LOWORD(lparam), HIWORD(lparam)
    );
  //  InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgRButtonDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display right mouse button down message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_RBUTTONDOWN (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgRButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 wsprintf(
		  ButtonText,
		  "WM_RBUTTONDOWN: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
  //  InvalidateRect(hwnd, &rectButton, TRUE);
	mouseb|=0x2;
	
	if(score)
		{
		nScoreCount--;
		UpdateStatusLine(hwnd);
		}

	if(click)
		{
		SetWindowText(hwnd,prompt);
		SendMessage(hwnd,WM_CLOSE,0,0);
		}
    
	return 0;
}


//
//  FUNCTION: MsgRButtonUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display right mouse button up message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_RBUTTONUP (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgRButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 wsprintf(
		  ButtonText,
        "WM_RBUTTONUP: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
   // InvalidateRect(hwnd, &rectButton, TRUE);
	mouseb&=~0x2;
    return 0;
}


//
//  FUNCTION: MsgRButtonDoubleClick(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display right mouse button double click message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_RBUTTONDOUBLECLICK (Unused)
//    wparam    - Key flags
//    lparam    -
//      LOWORD - x position of cursor
//      HIWORD - y position of cursor
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgRButtonDoubleClick
	 (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 wsprintf(
        ButtonText,
        "WM_RBUTTONDBLCLK: %x, %d, %d",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
   // InvalidateRect(hwnd, &rectButton, TRUE);

    return 0;
}


//
//  FUNCTION: MsgKeyDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display key down message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_KEYDOWN (Unused)
//    wparam    - Virtual Key Code
//    lparam    - Key Data
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgKeyDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        KeyboardText,
        "WM_KEYDOWN: %x, %x, %x",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
   // InvalidateRect(hwnd, &rectKeyboard, TRUE);

    return 0;
}


//
//  FUNCTION: MsgKeyUp(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display key up message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_KEYUP (Unused)
//    wparam    - Virtual Key Code
//    lparam    - Key Data
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgKeyUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        KeyboardText,
        "WM_KEYUP: %x, %x, %x",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
   // InvalidateRect(hwnd, &rectKeyboard, TRUE);

    return 0;
}


//
//  FUNCTION: MsgChar(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display character recieved message and its parameters.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_CHAR (Unused)
//    wparam    - Character Code
//    lparam    - Key Data
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgChar(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    wsprintf(
        CharacterText,
        "WM_CHAR: %c, %x, %x",
        wparam, LOWORD(lparam), HIWORD(lparam)
    );
  //  InvalidateRect(hwnd, &rectCharacter, TRUE);

    return 0;
}


//
//  FUNCTION: MsgScroll(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Display scrollbar events and current position.
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - WM_VSCROLL or WM_HSCROLL
//    GET_WM_HSCROLL_CODE(wparam,lparam) - Type of scroll
//    GET_WM_HSCROLL_POS(wparam, lparam) - Position of scroll
//          Only valid for SB_THUMBPOSTION and SB_THUMBTRACK
//
//  RETURN VALUE:
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

#pragma argsused
LRESULT MsgScroll(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	 char *szDirection;
	 char *szType;
	 int  fnBar = (uMessage == WM_HSCROLL) ? SB_HORZ : SB_VERT;
	 int  nPos  = GetScrollPos(hwnd, fnBar);

	 szDirection = (uMessage == WM_HSCROLL) ? "WM_HSCROLL" : "WM_VSCROLL";

	 switch(GET_WM_HSCROLL_CODE(wparam,lparam))
	 {
		  case SB_LINEUP:
				szType = "SB_LINEUP";
				nPos = max(0, nPos-1);
				break;
		  case SB_LINEDOWN:
				szType = "SB_LINEDOWN";
				nPos = min(100, nPos+1);
				break;
		  case SB_PAGEUP:
				szType = "SB_PAGEUP";
				nPos = max(0, nPos-10);
				break;
		  case SB_PAGEDOWN:
				szType = "SB_PAGEDOWN";
				nPos = min(100, nPos+10);
				break;
		  case SB_THUMBPOSITION:
				szType = "SB_THUMBPOSITION";
				nPos = GET_WM_HSCROLL_POS(wparam, lparam);
				break;
		  case SB_THUMBTRACK:
				szType = "SB_THUMBTRACK";
				nPos = GET_WM_HSCROLL_POS(wparam, lparam);
				break;
		  case SB_ENDSCROLL:
				szType = "SB_ENDSCROLL";
				break;
		  default:
				szType = "unknown";
				break;
    }

    wsprintf(
         ScrollText, "%s: %s, %x",
        (LPSTR)szDirection, (LPSTR)szType, nPos
    );
 //   InvalidateRect(hwnd, &rectScroll, TRUE);

    if (GET_WM_HSCROLL_CODE(wparam,lparam) != SB_THUMBTRACK)
        SetScrollPos(hwnd, fnBar, nPos, TRUE);

    return 0;
}


//
//  FUNCTION: InitInput(HWND)
//
//  PURPOSE: Set up the rectangles for dispay of each type of message
//
//  PARAMETERS:
//    hwnd   - Window handle
//
//  RETURN VALUE:
//    Always returns TRUE - Sucess
//
//  COMMENTS:
//
//

BOOL InitInput(HWND hwnd)
{
	/*
    HDC hdc;
    TEXTMETRIC tm;
    RECT rect;
    int  dyLine;

    // Get a handle to the display context for the window in order
    //  to compute line height, screen width, and pixel count for 1/4 inch

    hdc = GetDC(hwnd);
    GetTextMetrics(hdc, &tm);
    dyLine = tm.tmExternalLeading + tm.tmHeight;

    rect.left   = GetDeviceCaps(hdc, LOGPIXELSX) / 4;   // 1/4 inch
    rect.right  = GetDeviceCaps(hdc, HORZRES);
    rect.top    = GetDeviceCaps(hdc, LOGPIXELSY) / 4;   // 1/4 inch
    ReleaseDC(hwnd, hdc);

    // Compute rectangle for mouse move events
    rect.bottom = rect.top + dyLine;
    rectMouse   = rect;

    // Compute rectangle for mouse button events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectButton = rect;

    // Compute rectangle for keyboard events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectKeyboard = rect;

    // Compute rectangle for character events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectCharacter = rect;

    // Compute rectangle for scroll events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectScroll = rect;

    // Compute rectangle for timer events
    rect.top += dyLine;
    rect.bottom += dyLine;
    rectTimer = rect;
*/

    return TRUE;
}
 
 
/*
#pragma argsused
LRESULT MsgErasebkgnd(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    
 RECT rc;
 //WM_ERASEBKGND 
 HDC hdc =  (HDC) wparam; // handle of device context 
 HANDLE brush=CreateSolidBrush(WindowBackground);
 
 GetClientRect (hwnd, &rc); 
 FillRect (hdc, &rc,brush ); 
 DeleteObject(brush);


 return 1;
}
*/