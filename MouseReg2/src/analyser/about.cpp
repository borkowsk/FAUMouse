#include <windows.h> 
//#include "symshwin.h"
#include "about_rc.h"
#include <time.h>
#include <stdio.h>

HWND MyHwnd=NULL; //Mozna ustawic jesli jest juz inne okno

const ILE_SEC=120; //Ile sekund czeka na OK

#ifdef DEMO_VERSION
class time_checker
{
	
	tm *newtime;
	time_t aclock;
	
public:

	time_checker()
	{
		
		time( &aclock );                 /* Get time in seconds */
		
		newtime = localtime( &aclock );  /* Convert time to struct */
		/* tm form */
		if(newtime->tm_mon>=4 && newtime->tm_year>102)
			abort();

		/* Print local time as a string */
		//printf( "The current date and time are: %s", asctime( newtime ) );
		
	}
	
	
} TheChecker;
#endif

#define X( _p_ )     ((_p_)^'&')

static char WB_Copyright[]=
//!W!o!j!c!i!e!c!h! !T!.! !B!o!r!k!o!w!s!k!i! !f!r!o!m! !W!a!r!s!a!w! !U!n!i!v!e!r!s!i!t!y!
{
X(' '),
//X('P'),X('r'),X('o'),X('g'),X('r'),X('a'),X('m'),X('e'),X('d'),X(' '),
X('b'),X('y'),X(' '),
X('W'), X('o'), X('j'), X('c'), X('i'), X('e'), X('c'), X('h'), 
X(' '), X('T'), X('.'), X(' '), 
X('B'), X('o'), X('r'), X('k'), X('o'), X('w'), X('s'), X('k'), X('i'), 
//X(' '), X('f'), X('r'), X('o'), X('m'), X(' '), 
//X('W'), X('a'), X('r'), X('s'), X('a'), X('w'), X(' '), 
//X('U'), X('n'), X('i'), X('v'), X('e'), X('r'), X('s'), X('i'), X('t'), X('y'),
0
};
static char blebleble[]=
"\0\1\0\0\0\rtr0\0\1\0\1\0\0\0iisysiiyucgueihuhewijfcijjfcremcoiisteiiomiewiitpicimtmiimatitikii\0\0\0"
"0\08757812312432sqwe45115665525322345765434960916246378900887465765874582372657626526";
static char numer[4]="100";//FAKTYCZNY NUMER LICENCJI
static char bleblebleble[]=
"\0\0\1\1\0\rtr0\0\1\0\1\0\0\0iisysiiyucgueefuihfuihfuhfiuhefhuifhuhgfhyurgfughicimtmiimatitikii\0\0\0"
"0\08757812312432sqwe45115665525322345765434960911234127468357650259256826852656223226";
static char MOUSE_NO_LOGO[]="MOUSE_NO_LOGO";
static char INNE[]="MOUSE_TURTLE\0SCANDIX_C\nEKONET_CPP\nEKONET_EMU";
static char blebleblebleble[]=
"0\08757812312432sqwe45115665525322345765434960911234127468357650259256826852656223226"
"\0\0\1\1\0\rtr0\0\1\0\1\0\0\0iisysiiyucgueefuihfuihfuhfiuhefhuifhuhgfhyurgfughicimtmiimatitikii\0\0\0";
static char wnid[]="Windows & Copyright of EKONET WAS 1.0";
#undef X

extern "C"
BOOL CALLBACK AutorsDialog(HWND  hDlg,UINT   message,WPARAM   wParam,LPARAM   lParam);

class _type_force_call_about_dialog_on_start //Moze byc tylko jeden na aplikacje!!!
{
public:
	HWND Dialog;	//Uchwyt dialogu
	int  Timer;   //Uchwyt zegara
	//KONSTRUKTOR
	_type_force_call_about_dialog_on_start():Dialog(NULL),Timer(NULL)
	{
       // Start();
	}

	void Start()
	{
		const char* pom=NULL;
		if(	(pom=getenv(MOUSE_NO_LOGO))==NULL || strcmp(pom,"YES")!=0	)
		{
			LPTSTR pom=MAKEINTRESOURCE(IDD_DIALOG_ABOUT);
			HINSTANCE hMainInstance=GetModuleHandle(NULL);//???MainInstance - where???
			
			Dialog=CreateDialog(
				hMainInstance,//, //APLICATION INSTANCE
				pom,
				MyHwnd,//0,//hDlg, //OWNER WINDOW
				AutorsDialog);

			ShowWindow(Dialog,SW_SHOW);//SW_SHOWNOACTIVATE???
		}
	}
	//DESTRUKTOR NA WYPADEK GDYBY USER NIE ZAMKNAL
	~_type_force_call_about_dialog_on_start()
	{
		if(Timer) 
            KillTimer(NULL, Timer); 
		if(Dialog)
			DestroyWindow(Dialog);
		Dialog=NULL;
		Timer=NULL;
	}

} _force_call_about_dialog;

extern "C"
void StartAboutDialog()
{
	//_force_call_about_dialog.Start();
    //ProcessMessages();
    LPTSTR pom=MAKEINTRESOURCE(IDD_DIALOG_ABOUT);
	HINSTANCE hMainInstance=GetModuleHandle(NULL);//???MainInstance - where???			
	int	Dialog=DialogBox(
				hMainInstance,//, //APLICATION INSTANCE
				pom,
				MyHwnd,//0,//hDlg, //OWNER WINDOW
				AutorsDialog);
}

extern "C"
BOOL CALLBACK AutorsDialog(HWND  hDlg,UINT   message,WPARAM   wParam,LPARAM   lParam) 
{ 
    //static HWND hOK;    /* handle of OK push button */ 
	
    switch(message) 
    { 
	case WM_INITDIALOG: 
		 _force_call_about_dialog.Timer = SetTimer(hDlg, 1, 1000*ILE_SEC , NULL); 
		/* Retrieve the application name from the .RC file. */ 
		//LoadString(hMainInstance, idsAppName, szAppName, APPNAMEBUFFERLEN); 
		/* Retrieve the .INI (or registry) filename. */ 
		//LoadString(hMainInstance, idsIniFile, szIniFile,    MAXFILELEN); 

		return TRUE; 
		
	case WM_ACTIVATE: 
		{
			WORD fActive = LOWORD(wParam);           // activation flag 
			BOOL fMinimized = (BOOL) HIWORD(wParam); // minimized flag 
			HWND hwndPrevious = (HWND) lParam;       // window handle 

			SetWindowPos(  
				_force_call_about_dialog.Dialog,// handle to window
				HWND_TOPMOST,  // placement-order handle
				0,                 // horizontal position
				0,                 // vertical position  
				0,                // width
				0,                // height
				SWP_NOMOVE | SWP_NOSIZE //UINT uFlags window-positioning flags
				);


		}
		return FALSE;
	case WM_COMMAND: 
		{
			unsigned int Lo=LOWORD(wParam);
			unsigned int Hi=HIWORD(wParam);
            switch(Lo) 
            { 
			case 2://End
			case IDOK:
				//	case ID_OK: 
				//EndDialog(hDlg, LOWORD(wParam) == IDOK); 
				DestroyWindow(hDlg);
				KillTimer(NULL,_force_call_about_dialog.Timer);
				_force_call_about_dialog.Timer=NULL;
				_force_call_about_dialog.Dialog=NULL;
                return TRUE; 
            }
			return FALSE;
		}

	case WM_TIMER:
		{
//#ifdef NDEBUG
			DestroyWindow(hDlg);
			KillTimer(NULL,_force_call_about_dialog.Timer);
			_force_call_about_dialog.Timer=NULL;
			_force_call_about_dialog.Dialog=NULL; 
//#endif
		}	
		return TRUE; 

	default:
		return FALSE;
    }  
} 

