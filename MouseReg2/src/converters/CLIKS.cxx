#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <values.h>
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
char OutFileName[128]="nul";//"exper.dat";
char Description[512]="\"Mouse program for Robin Vallacher - dump file\"";
const char* comment="*";
long badval=DEFAULT_BAD_VALUE;

struct RegRec{
	double time;
	float  distance;
	int click;
	unsigned to16;
	};

#if defined( __HUGE__ ) || defined ( __LARGE__ )
#define MAX_DATA_POINTS (10L*60L*30L)
RegRec huge  RegTabMem[MAX_DATA_POINTS];
RegRec huge* RegTab=RegTabMem;
#else
RegRec far RegTab[4095];
#endif

unsigned long table_position;
int DIST_CLASS=2;
double centerX=640/2,centerY=480/2;/* ASSUMED VGA SCREEN DIMENSIONS !!! */
double MAX_DIST=sqrt(320*320L+240*240L)+1;
double FixMaxDistance=0;

double Sigma(double sum,double sumSqr,unsigned N);
inline double sqr(double s)/* Wygodny kwadrat */
{
return s*s;
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
	return badval;
  }
  double Sigma()
  {
  if(N>=2)
	return ::Sigma(sum,sumSqr,N);
	else
	return badval;
  }
};

void dist_class::Register(double Distance)
{
 sum+=Distance;
 sumSqr+=sqr(Distance);
 N++;
}

double Sigma(double sum,double sumSqr,unsigned N)
{
const double dokladnosc=0.001;// Wielokrotne dodawanie kwadratow gubi dokladnosc
//double pom=  (  sumSqr/N - sqr(sum/N)  );
double pom=  ( sumSqr - sqr(sum)/N  )/  (N-1) ;
if(pom<-dokladnosc)
	return badval;
if(pom>0)
	return sqrt( pom );
	else
	return 0;
}

void LoadFileAction()
{
FILE *file;
char bufor[512];
long i;
double LocalMax=-9999;

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
	if(Distance>0)
		Distance=sqrt(Distance);
		else
		Distance=badval;
	if(Distance>LocalMax)
		LocalMax=Distance;
	RegTab[i].time=pTime;
	RegTab[i].click=click;
	RegTab[i].distance=Distance;
	}
fclose(file);
#ifdef __MSDOS__
i--; /* \r\n sequention make error-behavior of feof() */
#endif
table_position=i;
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
for(long i=skip;i<table_position;i++)
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
long  end=start+krok;
long    i=0;
dist_class DistClasses[MXCLAS][2];
double     DistSteady[MXCLAS];
double LocMinDist=MAXINT; // Najmniejsza odleglosc od trrget w grupie - do ustawienia
double LocMaxDist=0;      // Najwieksza odleglosc od target w grupie - do ustawienia

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
for(i=start;i<end;i++)
    {/* MIN/MAX searching */
    double  distance=RegTab[i].distance;
    if(distance>LocMaxDist)
	LocMaxDist=distance;
    if(distance<LocMinDist)
	LocMinDist=distance;
    }
fprintf(file,"{%4ld-%4ld}\t",start,end-1);
for(i=start;i<end;i++)
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
	if( RegTab[i].click!=0 )
		{
		if(RegTab[i].click & 1 )
			DistClasses[0][1].Register(distance);
		if(RegTab[i].click & 2 )
			DistClasses[0][2].Register(distance);
		if(RegTab[i].click & 4 )
			fprintf(stderr,"Invalid click value in record %ld\n",i);
//			DistClasses[0][0].Register(distance);
		}

	if(delta==0)/* Gdy predkosc 0 */
		DistSteady[0]+=deltaT;

	/* Musi dzielic na DIST_CLASS i nie wiecej */

	unsigned GroupIndex=(LocMaxDist>LocMinDist?
			((distance-LocMinDist)/
			(LocMaxDist-LocMinDist))
			*(DIST_CLASS)+1
			: 1);

	if(delta==0)/* Gdy predkosc 0 */
		DistSteady[GroupIndex]+=deltaT;

	if( RegTab[i].click!=0 )
		{
		if(RegTab[i].click & 1 )
			DistClasses[GroupIndex][1].Register(distance);
		if(RegTab[i].click & 2 )
			DistClasses[GroupIndex][2].Register(distance);
//		if(RegTab[i].click & 4 )
//			DistClasses[0][0].Register(distance);
		}

	}

for(int j=0;j<=DIST_CLASS;j++)
    if(EOF==fprintf(file,"(I :\t%4u\t%lf\t%lf\tII:\t%4u\t%lf\t%lf\t)",
	    DistClasses[j][1].N,
	    DistClasses[j][1].Mean(),
	    DistClasses[j][1].Sigma(),
	    DistClasses[j][2].N,
	    DistClasses[j][2].Mean(),
	    DistClasses[j][2].Sigma()
	    ) )
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
long i;
long krok=(table_position-skip)/groups;
FILE *file;
if((file=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7\7");
	return;
	}
skip+=(table_position-skip)-groups*krok;
if(strcmp(comment,"-nc")!=0)
    {
    if(skip==0)
    fprintf(file,"%s [no skipped records]\n",Description);
	else
	fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
	fprintf(file,"{  recs   }\t"
		     "A:BUTT\tNUM\tM.DIST\tSIGM\tBUTT\tNUM\tM.DIST\tSIGM\t");
	for(i=1;i<=DIST_CLASS;i++)
		fprintf(file,
		     "%d:BUTT\tNUM\tM.DIST\tSIGM\tBUTT\tNUM\tM.DIST\tSIGM\t",i);
	fprintf(file,"WHOLE TIME\tWHOLE REST\tREST TIME FOR %d GROUPS\n",DIST_CLASS);
    }
errno=0;
DumpGroup(file,skip,groups*krok);
if(groups>=2)
  for(i=0;i<groups;i++)
    DumpGroup(file,skip+i*krok,krok);
fclose(file);
}

void ZmienEXT(char* Name,const char* ext)
/* Podmienia rozszerzenie nazwy. Zaklada ze starczy miejsca! */
{
char* pos=strrchr(Name,'.');
if(pos==NULL)
	{
	pos=Name+strlen(Name);
	*pos='.';
	}
pos++; // Pozycja za kropka
strcpy(pos,ext);
}

main(int argc,char* argv[])
{
long skipval=0;
int groups=3;
int extract=1;
int first_opt=2;
if(argc<2)
	{
	printf("USAGE:\n\t %s infile [outfile] [ options ]\n\n",argv[0]);
	printf("When outfile is optional. If do`t exist, program\n");
	printf("would change extension to .dat \n");
	printf("Options are:\n");
        printf("[-r ] - make raw output file\n");
	printf("[-nc] - optional \"non comment\" swich\n");
	printf("[-bN] - bad value marker (default -99)\n");
	printf("[-XN] - target position X (default 320)\n");
	printf("[-YN] - target position Y (default 240)\n");
	printf("[-DN] - fix max distance\n");
	printf("[-skN] - skip first N records\n");
	printf("[-exN ] - extract to N time groups\n");
	printf("[-cN] - number of distance groups (default 2)\n");
	exit(1);
	}
	else
	{
	strcpy(FileName,argv[1]);
	if(argv[2]!=NULL && argv[2][0]!='-') // Jest nazwa pliku wyjsciowego
		{
		strcpy(OutFileName,argv[2]);
		first_opt=3;
		}
		else        // Brak nazwy pliku wyjsciowego
		{	    // treba zbudowac
		strcpy(OutFileName,argv[1]);
		ZmienEXT(OutFileName,"dat");
		first_opt=2;
		}
	for(int i=first_opt;argv[i]!=NULL;i++) /* Parse parameters */
		switch(argv[i][1]){
		case 'n':comment="-nc";break;
		case 'b':badval=atol(argv[i]+3);break;
		case 's':skipval=atol(argv[i]+3);break;
                case 'r':extract=0;break;
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

