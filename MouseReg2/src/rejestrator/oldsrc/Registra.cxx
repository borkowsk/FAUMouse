#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#define huge //stiupid keyword

struct RegRec{
	double time;
	short int click;
	short int  X,Y;
	};

#define MAX_DATA_POINTS (10L*60L*30L)
RegRec huge* RegTab=NULL;/*[MAX_DATA_POINTS];*/

int score_enabled=0;
int score_changed=1;
long score=0;

unsigned long position=0xffffffffUL;

extern Interface MyMouse;
extern double  dlimit;   /* Users setting limit */
double GetCounter();	 /* Counter in sec.hun */
int click_delay=0;       /* Bezwladnosc przycisku myszy */

void TimerUserAction()
{
extern void EndTime();

if(position> MAX_DATA_POINTS  )
		EndTime();
		else
		{
		RegRec far* rec=(RegRec far*)(RegTab+position);
		int pom_click;
		click_delay--;
		rec->time=GetCounter();
		rec->click=pom_click=MyMouse.CheckClick();
		rec->X=MyMouse.GetMosX();
		rec->Y=MyMouse.GetMosY();
		position++;
		if(score_enabled && click_delay<=0)
			{
			if( pom_click & 1 ) score++;
			if( pom_click & 2 ) score--;
			score_changed=1;
			click_delay=CL_DELAY;
			}
		if(pom_click>0 && click)/* STOP AFTER CLICK */
				EndTime();
		}
}

void TimerUserStartAction()
{
//RegRec huge* RegTab=NULL;/*[MAX_DATA_POINTS];*/
RegTab=(RegRec huge*)farcalloc(MAX_DATA_POINTS,sizeof(RegRec));
if(!RegTab)
	{
	perror("Not enought memory for registration table. System status ");
	exit(1);
	}
CONTINUE_FLAG=1;
position=0;
TimerUserAction(); //Moment 0
printf("%s ",prompt);
}

void TimerUserEndAction()
{
CONTINUE_FLAG=0; /* End of experiment */
}

void RegistrationAtExitAction()
{
FILE *file;
printf("%s  %2.4gs. ",danker,GetCounter());
if(RegTab==NULL || position==0xffffffffUL)
	{
	perror("Nothing to save! \7\7 System status");
	return;
	}
if((file=fopen(FileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7 System status");
	return;
	}
fprintf(file,"%s\n",Description);
fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\n","X","Y","TIME","CLICK");
for(long i=0;i<position;i++)
	fprintf(file,"%d\t%d\t%g\t%d\n",
		RegTab[i].X,
		RegTab[i].Y,
		RegTab[i].time,
		RegTab[i].click);
fclose(file);
farfree(RegTab);
delay(1000);
}

#pragma exit RegistrationAtExitAction   255

