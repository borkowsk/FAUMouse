#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <values.h>
#include <assert.h>
#include <errno.h>
#ifdef __MSDOS__
#include <dos.h>
#ifdef __HUGE__
#define huge
#define far
#endif
#endif
#ifdef unix
#define __HUGE__
#define huge
#define far
#endif
#define DEFAULT_BAD_VALUE  (-99)
/* User interface tu module */
char FileName[128]="nul";//"exper.dat";
char OutFileName[128]="nul";//"exper.olf";
char Description[512]="\"Mouse paradigm data file - converted from olf[ile]\"";
int  comment=1;
long badval=DEFAULT_BAD_VALUE;
double centerX=640/2,centerY=480/2;/* ASSUMED VGA SCREEN DIMENSIONS !!! */
double MAX_DIST=sqrt(320*320L+240*240L)+1;
double FixMaxDistance=0;

inline double sqr(double s)/* Wygodny kwadrat */
{
return s*s;
}

void ZmienEXT(char* Name,const char* ext)
/* Podmienia rozszerzenie nazwy. Zaklada ze starczy miejsca! */
{
char* pos=strrchr(Name,'.');
printf(
"\nCUT DISTANCES FROM OLD DATA OBTAINED WITHIN MOUSE PARADIGM.\n"
"Programmed by Wojciech Borkowski under direction of \n"
"Andrzej Nowak & Robin Vallacher (VERSION %s)\n",__DATE__);

if(pos==NULL)
	{
	pos=Name+strlen(Name);
	*pos='.';
	}
pos++; // Pozycja za kropka
strcpy(pos,ext);
}


void ConvertFileAction()
{
FILE *file,*out;
char bufor[512];
long i;
double LocalMax=-9999;

if((file=fopen(FileName,"rt"))==NULL)
	{
	perror("Fatal! Can't read data! \7\7\7");
	return;
	}
if((out=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't open output file! \7\7\7");
	return;
	}
printf("WAIT-CONVERSION IN PROGRESS\n");
for(i=0;!feof(file);i++)
	{
	double pTime;
	double Distance;
	int X,Y,click;
	fscanf(file,"%03d, %03d, %lf, %lf\n",&Y,&X,&Distance,&pTime);
	//fscanf(file,"%d",     &X);
	//fscanf(file,"%d",     &Y);
	//fscanf(file,"%le",    &pTime);
	//fscanf(file,"%d",     &click);
	fprintf(out,"%g\n",Distance);
	}
fclose(file);
fclose(out);
#ifdef __MSDOS__
i--; /* \r\n sequention make error-behavior of feof() */
#endif
if(FixMaxDistance!=0)
	MAX_DIST=FixMaxDistance;
	else
	MAX_DIST=LocalMax+1/* Drobny dodatek zeby zeby distance/MAX <1 */;
}


int main(int argc,char* argv[])
{
long skipval=0;
int first_opt=2;
if(argc<2)
	{
	printf("USAGE:\n\t %s infile [outfile] \n\n",argv[0]);
	printf("When outfile is optional. If do`t exist, program\n");
	printf("would change extension to .dst \n");
	return 1;
	}
strcpy(FileName,argv[1]);
if(argv[2]!=NULL && argv[2][0]!='-') // Jest nazwa pliku wyjsciowego
		{
		strcpy(OutFileName,argv[2]);
		first_opt=3;
		}
		else        // Brak nazwy pliku wyjsciowego
		{	    // treba zbudowac
		strcpy(OutFileName,argv[1]);
		ZmienEXT(OutFileName,"dst");
		first_opt=2;
		}
/*
for(int i=first_opt;argv[i]!=NULL;i++) /* Parse parameters */
		switch(argv[i][1]){
		case 'n':comment=0;break;
		case 'b':badval=atol(argv[i]+3);break;
		case 's':skipval=atol(argv[i]+3);break;
		case 'X':centerX=atol(argv[i]+3);break;
		case 'Y':centerY=atol(argv[i]+3);break;
		case 'D':FixMaxDistance=atol(argv[i]+3)+1;break;
		}
*/
ConvertFileAction();
return 0;
}