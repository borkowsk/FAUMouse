#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef __MSDOS__
#include <dos.h>
#endif
#ifdef unix
#define __HUGE__
#define huge
#define far
#endif

/* User interface tu module */
char FileName[128]="nul";//"exper.dat";
char OutFileName[128]="nul";//"exper.dat";
char Description[512]="\"Mouse program for Robin Vallacher - dump file\"";
const char* comment="*";

struct RegRec{
	double time;
	float  distance;
	int click;
	unsigned to16;
	};

#if defined( __HUGE__ ) || defined ( __LARGE__ )
#define MAX_DATA_POINTS (10L*60L*30L)
RegRec huge RegTabMem[MAX_DATA_POINTS];
RegRec huge* RegTab=RegTabMem;
#else
RegRec far RegTab[4095];
#endif

unsigned long position;
int DIST_CLASS=2;
double centerX=640/2,centerY=480/2;/* ASSUMED VGA SCREEN DIMENSIONS !!! */
double MAX_DIST=sqrt(320*320L+240*240L)+1;
double FixMaxDistance=0;

double sqr(double s)/* Wygodny kwadrat */
{
return s*s;
}

double Sigma(double sum,double sumSqr,unsigned N)
{
return sqrt(   ( sumSqr- (sum*sum)/N )  /  (N-1)  );
}

struct dist_class
{ double sum,sumSqr;
  unsigned N;

  dist_class(){sum=sumSqr=N=0;}
  void Register(double Distance);
  double Mean()
  {
  if(N>=1)
	return sum/N;
	else
	return -99;
  }
  double Sigma()
  {
  if(N>=2)
	return ::Sigma(sum,sumSqr,N);
	else
	return -99;
  }
};

void dist_class::Register(double Distance)
{
 sum+=Distance;
 sumSqr+=sqr(Distance);
 N++;
}

void LoadFileAction()
{
FILE *file;
char bufor[512];
long i;
double LocalMax=0;

if((file=fopen(FileName,"rt"))==NULL)
	{
	perror("Fatal! Can't read data! \7\7\7");
	return;
	}
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
	Distance=sqrt(Distance);
	if(Distance>LocalMax)
		LocalMax=Distance;
	RegTab[i].time=pTime;
	RegTab[i].click=click;
	RegTab[i].distance=Distance;
	}
fclose(file);
#ifdef __MSDOS__
i--; /* \r\n secwention make error-behavior of feof() */
#endif
position=i;
if(FixMaxDistance!=0)
	MAX_DIST=FixMaxDistance;
	else
	MAX_DIST=LocalMax+1/* Drobny dodatek zeby zeby distance/MAX <1 */;
}

void SaveFileAction(long skip)
{
FILE *file;
double delta=0,olddelta=0,deltaDelty=0;
double olddistance=RegTab[0].distance;

if(skip<=0)
	skip=1;
if((file=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7\7");
	return;
	}
	//else errno=0;
if(strcmp(comment,"-nc")!=0)
	{
	fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
	fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t%s\n","TIME","CLIK","DISTANCE","V(pix/sec)","A(pix/sec^2)");
	}
for(long i=skip;i<position;i++)
	{/* Calculations */
	double  distance=RegTab[i].distance;
	double deltaT=fabs(RegTab[i].time-RegTab[i-1].time);
	if(deltaT==0)
		{
		fprintf(stderr,"WARNING! DeltaT=0 (time)rec %ld & %ld\n",i-1,i);
		continue;
		}
	delta=fabs(olddistance-distance)/deltaT;
	deltaDelty=(delta-olddelta)/deltaT;

	olddistance=distance;
	olddelta=delta;
	if(EOF==fprintf(file,"%lf\t%3d\t%lf\t%lf\t%lf\n",
		RegTab[i].time,RegTab[i].click,distance,delta,deltaDelty))
			{
			perror(OutFileName);
			exit(errno);
			}
	}
fclose(file);
}


void DumpGroup(FILE* file,long start,long krok)
{
const MXCLAS=12;
dist_class DistClasses[MXCLAS];
double     DistSteady[MXCLAS];
dist_class VeloClasses[MXCLAS];
dist_class AcceClasses[MXCLAS];

double olddelta=0,olddistance=0;
for(int k=0;k<MXCLAS;k++)
		DistSteady[k]=0;
if(start>0)
	olddistance=RegTab[start-1].distance;
	else
	{
	olddistance=RegTab[0].distance;
	start=1;
	}
fprintf(file,"(%4ld-%4ld)\t",start,start+krok-1);
for(long i=start;i<start+krok;i++)
	{/* Calculations */
	double  distance=RegTab[i].distance;
	double deltaT=fabs(RegTab[i].time-RegTab[i-1].time);
	if(deltaT==0)
		{
		fprintf(stderr,"WARNING! DeltaT=0 (time)rec %ld & %ld(is skipped)\n",i-1,i);
		continue;
		}
	double  delta=fabs(olddistance-distance)/deltaT;
	double  deltaDelty=fabs(delta-olddelta)/deltaT;
	olddelta=delta;
	olddistance=distance;
	DistClasses[0].Register(distance);
	if(delta==0)/* Gdy predkosc 0 */
		DistSteady[0]+=deltaT;
	VeloClasses[0].Register(delta);
	AcceClasses[0].Register(deltaDelty);

	/* Musi dzielic na DIST_CLASS i nie wiecej */
	unsigned GroupIndex=(distance/MAX_DIST)*(DIST_CLASS)+1;

	DistClasses[GroupIndex].Register(distance);
	if(delta==0)/* Gdy predkosc 0 */
		DistSteady[GroupIndex]+=deltaT;
	VeloClasses[GroupIndex].Register(delta);
	AcceClasses[GroupIndex].Register(deltaDelty);
	}

if(EOF==fprintf(file,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",
	    DistClasses[0].Mean(),DistClasses[0].Sigma(),
	    VeloClasses[0].Mean(),VeloClasses[0].Sigma(),
	    AcceClasses[0].Mean(),AcceClasses[0].Sigma()))
		{
		perror(OutFileName);
		exit(errno);
		}
for(int j=1;j<=DIST_CLASS;j++)
	if(EOF==fprintf(file,"(%4u)\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t",
	    DistClasses[j].N,
	    DistClasses[j].Mean(),DistClasses[j].Sigma(),
	    VeloClasses[j].Mean(),VeloClasses[j].Sigma(),
	    AcceClasses[j].Mean(),AcceClasses[j].Sigma()))
		{
		perror(OutFileName);
		exit(errno);
		}
fprintf(file,"[%lf]\t",fabs(RegTab[start].time-RegTab[start+1].time)*krok);
for(int l=0;l<=DIST_CLASS;l++)
	if(EOF==fprintf(file,"%lf\t",DistSteady[l]))
		{
		perror(OutFileName);
		exit(errno);
		}
fprintf(file,"\n");
}

void MakeExtractFile(long skip,long groups)
{
FILE *file;
if((file=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7\7");
	return;
	}
long krok=(position-skip)/groups;
skip+=(position-skip)-groups*krok;
if(strcmp(comment,"-nc")!=0)
	{
	fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
	fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
		"G R O U P"," M.DIST  ","   SD    ","M.V(pix/s)","   SD    ","M.A(pix/s^2)","   SD   ");
	for(int j=0;j<DIST_CLASS;j++)
		fprintf(file,"\"%s%u\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
		"SUB:",j+1,"{ M.DIST","   SD   ","M.V(pix/s)","   SD   ","M.A(pix/s^2)","   SD  }");
	fprintf(file,"\"WHOLE TIME\"\t\"REST TIME\"\t\"<-REST TIME FOR DISTANCE GROUPS->\"\t\n");
	}
errno=0;
for(long i=0;i<groups;i++)
	DumpGroup(file,skip+i*krok,krok);
fclose(file);
}

main(int argc,char* argv[])
{
long skipval=0;
int groups=0;
int extract=1;
if(argc<3)
	{
	fprintf(stderr,"USAGE: %s infile outfile [ options ]\n\n",argv[0]);
	fprintf(stderr,"When options are:\n");
	fprintf(stderr,"[-nc] - optional \"non comment\" swich\n");
	fprintf(stderr,"[-XN] - target position X (default 320)\n");
	fprintf(stderr,"[-YN] - target position Y (default 240)\n");
	fprintf(stderr,"[-DN] - fix max distance\n");
	fprintf(stderr,"[-skN] - skip first N records\n");
	fprintf(stderr,"[-exN ] - extract to N time groups\n");
	fprintf(stderr,"[-cN] - number of distance groups (default 2)\n");
	exit(1);
	}
	else
	{
	strcpy(FileName,argv[1]);
	strcpy(OutFileName,argv[2]);
	for(int i=3;argv[i]!=NULL;i++) /* Parse parameters */
		switch(argv[i][1]){
		case 'n':comment="-nc";break;
		case 's':skipval=atol(argv[i]+3);break;
		case 'e':extract=1;groups=atoi(argv[i]+3);break;
		case 'X':centerX=atol(argv[i]+3);break;
		case 'Y':centerY=atol(argv[i]+3);break;
		case 'D':FixMaxDistance=atol(argv[i]+3)+1;break;
		case 'c':DIST_CLASS=atol(argv[i]+3);break;
		}
	printf("\nI AM WORKING!\n");
	}
LoadFileAction();
if(extract==1)
	MakeExtractFile(skipval,groups);
	else
	SaveFileAction(skipval);
return 0;
}

