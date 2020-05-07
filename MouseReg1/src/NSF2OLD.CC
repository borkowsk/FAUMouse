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
char Description[512]="\"Mouse program for Robin Vallacher - dump file\"";
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
"\nCONVERTER FOR DATA OBTAINED WITHIN MOUSE PARADIGM.\n"
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
fgets(Description,sizeof(Description)-1,file);
Description[strlen(Description)-1]='\0';
fgets(bufor,sizeof(bufor),file);
for(i=0;!feof(file);i++)
	{
	double pTime;
	double Distance;
	int X,Y,click;
	fscanf(file,"%d",     &X);
	fscanf(file,"%d",     &Y);
	fscanf(file,"%le",    &pTime);
	fscanf(file,"%d",     &click);
	Distance=sqr(centerX-X)+sqr(centerY-Y);
	if(Distance>0)
		Distance=sqrt(Distance);// For all non 0 values
		else
		if(Distance<0)
			{
			fprintf(stderr,"Bad distance. Set properly center coordinates (X,Y options).\n");
			exit(1);
			}
	if(Distance>LocalMax)
		LocalMax=Distance;
//	pTime;click;Distance; // Maybe 0 or great than 0
	fprintf(out,"%7d,%7d,%7d,%10ld\n",Y,X,int(Distance),(long int)(pTime*100));
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
	printf("USAGE:\n\t %s infile [outfile] [ options ]\n\n",argv[0]);
	printf("When outfile is optional. If do`t exist, program\n");
	printf("would change extension to .olf \n");
	printf("Options are:\n");
	printf("[-XN] - target position X (default 320)\n");
	printf("[-YN] - target position Y (default 240)\n");
	printf("[-DN] - fix max distance\n");
	printf("[-skN] - skip first N records\n");
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
		ZmienEXT(OutFileName,"olf");
		first_opt=2;
		}
for(int i=first_opt;argv[i]!=NULL;i++) /* Parse parameters */
		switch(argv[i][1]){
		case 'n':comment=0;break;
		case 'b':badval=atol(argv[i]+3);break;
		case 's':skipval=atol(argv[i]+3);break;
		case 'X':centerX=atol(argv[i]+3);break;
		case 'Y':centerY=atol(argv[i]+3);break;
		case 'D':FixMaxDistance=atol(argv[i]+3)+1;break;
		}
ConvertFileAction();
return 0;
}