#define USES_SYMSHELL
#define USES_STRING
#define USES_ERRNO
#define USES_SIGNAL
#include "resource.h"
#include "uses_wb.hpp"
#include "wb_posix.h"

extern "C" 
{
extern char* SymShellProd="ISS Mouse Registrator Runner. Compiled "__DATE__ __TIME__;
void symshell_about();
int WB_error_enter_before_clean=0;
}

int main(int argc,char* argv[])
{
int status;
mouse_activity(0);
set_background(255);
shell_setup("Windows runner for mouse registrator",argc,argv);
init_plot(3,3,40,1);
for(int i=1;i<argc;i++)
	{
	const char* n_args[3];
	n_args[0]="mousereg.exe";
	n_args[1]=argv[i];
	n_args[2]=NULL;
	print(1,1,"Starting %32.32s",argv[i]);
	flush_plot();
	pid_t child=(pid_t)spawnvp(_P_NOWAITO,"mousereg.exe",n_args); 
	if(child==pid_t(-1))
		{
		printc(1,1,255,55,
			"%s: on spawn \"mousereg.exe\"",strerror(errno));
		flush_plot();
		sleep(4);// -POSIX sleep function -> 4 sec
		continue;
		}
	print(1,1,"Processing %30.30s",argv[i]);
	flush_plot();
	//waitpid -POSIX function
	int ret;
	while((ret=waitpid(child,&status,WNOHANG))!=child)
	  {
	  //int test;//for debuging	
      
	  if(ret==-1)	
		{
		printc(1,1,255,55,
			"%s: on wait \"mousereg.exe\"",strerror(errno));
		flush_plot();
		sleep(4);// -POSIX sleep function -> 4 sec
		break;//while. Nie ma procesu.
		}
	  
	  if(input_ready())
		switch(/*test=*/get_char()){
		case EOF:	
				kill(child,SIGTERM/*SIGABRT*/);//other signal not supported
				goto EXIT;
		case IDM_NCONTINUE:
				goto EXIT;
		case IDM_ABOUT:
				symshell_about();break;
		default:
			clear_screen();
			print(1,1,"Processing %30.30s",argv[i]);
			flush_plot();
		}//switch
	  }
	}

EXIT:
close_plot();
return 0;
}