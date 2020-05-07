const char* VERSION=
"\n"
"===========================================================\n"
" ANALYSER FOR DATA OBTAINED WITHIN MOUSE PARADIGM.\n"
"-----------------------------------------------------------\n"
"Programmed by Wojciech Borkowski under direction of \n"
"         Andrzej Nowak & Robin Vallacher\n"
"(VERSION 3.20a, M$ Windows port, compilation "__DATE__")\n"
"===========================================================\n"
"\n";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <values.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include "platform.hpp"
#define DEFAULT_BAD_VALUE  (-99)

/* User interface tu module */
char FileName[128]="nul";//"exper.dat";
char OutFileName[128]="nul";//"exper.dat";
char Description[1024]="\"Mouse program for Robin Vallacher - dump file\"";
int  comment=1;
long badval=DEFAULT_BAD_VALUE;

/* REKORD DANYCH I DEFINICJA TABLICY REKORDOW - O SZTYWNYM ROZMIARZE!!! */
/************************************************************************/
struct RegRec{
	double	time;
	float	distance;
	double	speed;
	double	accel;
	int		click;
	//unsigned to16;
	};

//ZMIENNE GLOWNEJ TABLICY REKORDOW
//////////////////////////////////////////
#define MAX_DATA_POINTS (10L*60L*30L*2)	//NAJWIEKSZA MOZLIWA ILOSC REKORDOW
RegRec huge  RegTabMem[MAX_DATA_POINTS];
RegRec huge* RegTab=RegTabMem;
unsigned long table_position;

// INNE PARAMETRY 
//////////////////////////////////
int USAIFORMAT=0;						//Czy wczytuje format FAU
int DIST_CLASS=2;						//ILE KLAS ODLEGLOSCI
double centerX=640/2,centerY=480/2;		//DEFAULT VALUE ASSUMED VGA SCREEN DIMENSIONS !!! 
double MAX_DIST=sqrt(320*320L+240*240L)+1; //ustawiane ale chyba nie uzywane
double FixMaxDistance=0;				   //do ustawiania MAX_DIST

/* FUNKCJE */
double Sigma(double sum,double sumSqr,unsigned N);
inline double sqr(double s)/* Wygodny kwadrat */
{
return s*s;
}

struct stat_class
{
  double sum,sumSqr;
  unsigned N;

  stat_class()
  {
	  sum=sumSqr=N=0;
  }
  
  void Register(double Distance)
  {
	sum+=Distance;
	sumSqr+=sqr(Distance);
	N++;
  }
  
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

double Sigma(double sum,double sumSqr,unsigned N)
{
const double dokladnosc=0.001;// Wielokrotne dodawanie kwadratow gubi dokladnosc
//double pom=  (  sumSqr/N - sqr(sum/N)  ); //Inny wariant - dla duzych N
double pom=  ( sumSqr - sqr(sum)/N  )/  (N-1) ;
if(pom<-dokladnosc)
	return badval;
if(pom>0)
	return sqrt( pom );
	else
	return 0;
}


void LoadWarsawInput(FILE *file)
{
char bufor[512];
long i;
double LocalMax=-9999;

fgets(Description,sizeof(Description)-1,file);
Description[strlen(Description)-1]='\0';
fgets(bufor,sizeof(bufor),file);
while(strlen(bufor)==0 || 
	  bufor[0]=='#' || 
	  (strncmp("X\tY",bufor,strlen("X\tY"))!=0 &&	//Stary format 	naglowka
	   strncmp("\"X\"",bufor,strlen("\"X\""))!=0	)	//Nowy format	naglowka
	  )//Pusta linia lub jeszcze komentarz lub inny nagowek
	{
	printf("Skipped line: %s\n",bufor);
	fgets(bufor,sizeof(bufor),file);	
	}
for(i=0;!feof(file);i++)
	{
	int nfie=0;
	double pTime;
	double Distance;
	int X,Y,click;
	if( (nfie=fscanf(file,"%d",     &X		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%d",     &Y		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%le",    &pTime	))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%d",     &click	))==EOF || nfie!=1) goto ERROR;
	nfie=fgetc(file);/* Powinno zjesc koniec lini! */
	if(nfie!='\n' && nfie!=EOF) 
					goto ERROR;
	nfie=fgetc(file);/* Sprawdza czy jest jeszcze jakis znak */
	if(nfie!=EOF) ungetc(nfie,file);

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
	RegTab[i].time=pTime;
	RegTab[i].click=click;
	RegTab[i].distance=Distance; // Maybe 0 or great than 0
	continue;
ERROR:
	fprintf(stderr,"Data format error at %d. record. LOAD BREAK!\n",i+1);
	break;
	}

//#ifdef __MSDOS__
//i--; /* \r\n sequention make error-behavior of feof() */
//#endif
table_position=i;
if(FixMaxDistance!=0)
	MAX_DIST=FixMaxDistance;
	else
	MAX_DIST=LocalMax+1/* Drobny dodatek zeby zeby distance/MAX <1 */;
}

int LoadStrTo(FILE* file,char* Str,char Sep)
{
	int ch=EOF;

	while((ch=getc(file))!=Sep && ch!=EOF )
	{
		*Str=ch;
		Str++;
	}
	*Str='\0';

	if(ch==EOF)
		return EOF;
	else
		return 1;
}


void LoadFAUInput(FILE *file)
{
char resztki[1024];
long i;
double LocalMax=-9999;

memset(Description,'\0',sizeof(Description));	
memset(Description,' ',30);		//20 spacji na poczatku zeby wpisac subject
fgets(&Description[21],sizeof(Description)-21,file);//Jedna linia naglowka kolumn
Description[strlen(Description)-1]='\0';	//Zakonczona 

//subj-cond,Quest,time,x,y,dist,speed,accel

for(i=0;!feof(file);i++)
	{
	int nfie=0;
	char		quest[128];
	char		subj[512];
	float			pTime;
	int			X,Y,click;
	float		Distance;
	float		Speed;
	float		Acceleration;

	if( (nfie=LoadStrTo(file,subj,','))==EOF || nfie!=1) goto ERROR;//Subject-condition
	if(	(nfie=LoadStrTo(file,quest,','))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%f,",    &pTime	))==EOF || nfie!=1) goto ERROR;
	if( (nfie=fscanf(file,"%d,",     &X		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%d,",     &Y		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%f,",   &Distance))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%f,",   &Speed	))==EOF || nfie!=1){ if(i==0) Speed=-DBL_MAX; else goto ERROR;}
	if(	(nfie=fscanf(file,"%f",&Acceleration))==EOF || nfie!=1){ if(i<2)  Acceleration=-DBL_MAX; else goto ERROR;}

	if( (nfie=LoadStrTo(file,resztki,'\n'))==EOF || nfie!=1) goto ERROR;//Reszta lini
	if( strlen(resztki)>=sizeof(resztki) ) goto ERROR;

	nfie=fgetc(file);/* Sprawdza czy jest jeszcze jakis znak do przeczytania */
	if(nfie!=EOF && nfie==subj[0]) 
			goto ERROR; /* Powinien byc koniec pliku lub to samo co w poprzedniej lini */
	if(nfie!=EOF) 
			ungetc(nfie,file);

	/*
	Distance=sqr(centerX-X)+sqr(centerY-Y);
	if(Distance>0)
		Distance=sqrt(Distance);// For all non 0 values
		else
		if(Distance<0)
			{
			fprintf(stderr,"Bad distance. Set properly center coordinates (X,Y options).\n");
			exit(1);
			}
	*/
	if(i>1 && Distance>LocalMax)
		LocalMax=Distance;

	RegTab[i].time=pTime;
	RegTab[i].click=click;
	RegTab[i].distance=Distance; // Maybe 0 or great than 0
	RegTab[i].speed=Speed;
	RegTab[i].accel=Acceleration;

	continue;
ERROR:
	fprintf(stderr,"Data format error at %d. record. LOAD BREAK!\n",i+1);
	break;
	}

//#ifdef __MSDOS__
//i--; /* \r\n sequention make error-behavior of feof() */
//#endif
table_position=i;
if(FixMaxDistance!=0)
	MAX_DIST=FixMaxDistance;
	else
	MAX_DIST=LocalMax+1/* Drobny dodatek zeby zeby distance/MAX <1 */;
}

void LoadFileAction()
{
	FILE *file;
	if((file=fopen(FileName,"rt"))==NULL)
	{
		fprintf(stderr,"File: %s\n",FileName);
		perror("Fatal! Can't read data! \7\7\7");
		return;
	}
	
	if(USAIFORMAT==1)
	{
		LoadFAUInput(file);
	}
	else
	{
		LoadWarsawInput(file);
	}
	
	fclose(file);
	strcat(Description,FileName);
}


void SaveFileAction(long skip)
//ZAPISYWANIE PLIKU Z SUROWYM FORMATEM (opcja -r czyli "RAW")
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
if(comment!=0)
	{
	fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
	fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t%s\n","  TIME  ","CLIK","DISTANCE","V(pix/sec)","A(pix/sec^2)");
	}

for(long i=skip;i<table_position;i++)	// Calculations - PETLA LICZENIA
	{
	double distance=RegTab[i].distance;	//AKTUALNA ODLEGLOSC
	double deltaT=fabs(RegTab[i].time-RegTab[i-1].time);//AKTUALNA ROZNICA CZASU
	assert(distance>=0);
	assert(deltaT>=0);

	if(deltaT==0)	//POWINNA BYC RACZEJ MNIEJSZA NIZ 1 ALE NIE O!!!
		{
		fprintf(stderr,"WARNING! DeltaT=0 (time)rec %ld & %ld\n",i-1,i);
		continue;
		}
	
	delta=fabs(olddistance-distance)/deltaT;	//PREDKOSC - ZMIANA ODLEGLOSCI W CZASIE 
	deltaDelty=(delta-olddelta)/deltaT;			//PRZYSPIESZENIE - ZMIANA PREDKOSCI W CZASIE

	olddistance=distance;	//ZAPAMIETANIE DLA NASTEPNEGO KROKU
	olddelta=delta;			//--------------//-----------------

	//WLASCIWY ZAPIS DO PLIKU WYJSCIOWEGO
	if(EOF==fprintf(file,"%8.3lf\t%3d\t%8.3lf\t%8.3lf\t%10.3lf\n",
		RegTab[i].time,RegTab[i].click,distance,delta,fabs(deltaDelty)))
			{
			perror(OutFileName);
			exit(errno);
			}
	}
	fclose(file);//TU PLIK JEST JUZ GOTOWY
}


void DumpGroup(FILE* file,long start,long krok)
//Funkcja liczaca dla grup i zapisujaca wyniki do pliku
{
const MXCLAS=12;
long  end=start+krok;
long    i=0;
stat_class DistClasses[MXCLAS];
double     DistSteady[MXCLAS];
stat_class VeloClasses[MXCLAS];
stat_class AcceClasses[MXCLAS];
double LocMinDist=INT_MAX; // Najmniejsza odleglosc od trrget w grupie - do ustawienia
double LocMaxDist=0;      // Najwieksza odleglosc od target w grupie - do ustawienia

double olddelta=0,olddistance=0;
for(int k=0;k<MXCLAS;k++)
		DistSteady[k]=0;

if(start>0)//SENSOWNE POPRZEDNIE WARTOSCI GDY POMIJA SIE POCZATKOWE REKORDY DANYCH
	olddistance=RegTab[start-1].distance;
	else
	{
	olddistance=RegTab[0].distance;
	start=1;
	}

for(i=start;i<end;i++)// MIN/MAX searching 
    {
    double  distance=RegTab[i].distance;
    if(distance>LocMaxDist)
		LocMaxDist=distance;
    if(distance<LocMinDist)
		LocMinDist=distance;
    }

if(comment!=0)
	fprintf(file,"{%4ld-%4ld}\t",start,end-1);

for(i=start;i<end;i++)// Calculations - WLASCIWE OBLICZENIA W PETLI DLA GRUP
	{
	double  distance=RegTab[i].distance;
	double	deltaT=fabs(RegTab[i].time-RegTab[i-1].time);

	if(deltaT==0)//ZAZWYCZAJ MNIEJSZE NIZ 1 ALE NIE 0!!!
		{
		fprintf(stderr,"WARNING! DeltaT=0 (time)rec %ld & %ld(is skipped)\n",i-1,i);
		continue;
		}
	
	double  delta=fabs(olddistance-distance)/deltaT;//PREDKOSC - ROZNICA ODLEGLOSCI W CZASIE
	double  deltaDelty=fabs(delta-olddelta)/deltaT;	//PRZYSPIESZENIE - ROZNICA PREDKOSCI W CZASIE
	
	olddelta=delta;	//ZAPAMIETANIE DLA NASTEPNEGO KROKU PETLI
	olddistance=distance;

	//REJESTRACJA STATYSTYK GLOBALNYCH
	DistClasses[0].Register(distance);
	if(delta==0)/* Gdy predkosc 0 */
		DistSteady[0]+=deltaT;
	VeloClasses[0].Register(delta);
	AcceClasses[0].Register(fabs(deltaDelty));

	//REJESTRACJA STATYSTYK GRUPOWYCH
	
    unsigned GroupIndex=(LocMaxDist>LocMinDist?
			((distance-LocMinDist)/
			(LocMaxDist-LocMinDist))
			*(DIST_CLASS)+1
			: 1);/* Musi dzielic na DIST_CLASS i nie wiecej */

	DistClasses[GroupIndex].Register(distance);
	if(delta==0)/* Gdy predkosc 0 */
		DistSteady[GroupIndex]+=deltaT;
	VeloClasses[GroupIndex].Register(delta);
	AcceClasses[GroupIndex].Register(fabs(deltaDelty));
	}

//ZAPISANIE  STATYSTYK GLOBALNYCH
if(EOF==fprintf(file,"%8.3lf\t%8.3lf\t%8.3lf\t%8.3lf\t%10.3lf\t%10.3lf\t",
	    DistClasses[0].Mean(),DistClasses[0].Sigma(),
	    VeloClasses[0].Mean(),VeloClasses[0].Sigma(),
	    AcceClasses[0].Mean(),AcceClasses[0].Sigma() ))
		{
			perror(OutFileName);
			exit(errno);
		}

//ZAPISANIE STATYSTYK GRUPOWYCH
for(int j=1;j<=DIST_CLASS;j++)
    if(EOF==fprintf(file," %5u\t%8.3lf\t%8.3lf\t%8.3lf\t%8.3lf\t%10.3lf\t%10.3lf\t",
	    DistClasses[j].N,
	    DistClasses[j].Mean(),DistClasses[j].Sigma(),
	    VeloClasses[j].Mean(),VeloClasses[j].Sigma(),
	    AcceClasses[j].Mean(),AcceClasses[j].Sigma() ))
		{
			perror(OutFileName);
			exit(errno);
		}

//DODATKOWA DELTA t * KROK
if(comment!=0)
	fprintf(file,"[%8.3lf]\t",fabs(RegTab[start].time-RegTab[start+1].time)*krok);

//CZASY POSTOJU
for(int l=0;l<=DIST_CLASS;l++)
	if(EOF==fprintf(file,"%8.3lf\t",DistSteady[l]))
		{
			perror(OutFileName);
			exit(errno);
		}
	
fprintf(file,"\n");
}

void MakeExtractFile(long skip,long groups)
//WYPISYWANIE STATYSTYK GRUP DO PLIKU 
{
long krok=(table_position-skip)/groups;
FILE *file;
if((file=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7\7");
	return;
	}
skip+=(table_position-skip)-groups*krok;

if(comment!=0)//NAGLOWKI 
    {
    if(skip==0)
    fprintf(file,"%s [no skipped records]\n",Description);
	else
	fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
	
	fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
		"G R O U P"," M.DIST  ","   SD    ","M.V(pix/s)","   SD    ","M.A(pix/s^2)","   SD   ");
	
	for(int j=0;j<DIST_CLASS;j++)
		fprintf(file,"\"%s%u\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
		"SUB:",j+1,"{ M.DIST","   SD   ","M.V(pix/s)","   SD   ","M.A(pix/s^2)","   SD  }");
	
	fprintf(file,"\"WHOLE TIME\"\t\"REST TIME\"\t\"<-REST TIME FOR DISTANCE GROUPS->\"\t\n");
    }
errno=0;

//FAKTYCZNE OBLICZENIA I ZAPISYWANIE
DumpGroup(file,skip,groups*krok);//CALOSC

if(groups>=2)	//ODCINKI CZASOWE
  for(long i=0;i<groups;i++)
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

int main(int argc,char* argv[])
{
long skipval=0;
int groups=3;
int extract=1;
int first_opt=2;

printf(VERSION);


if(argc<2)
	{
	printf("USAGE:\n\t %s infile [outfile] [ options ]\n\n",argv[0]);
	printf("When outfile is optional. If do`t exist, program\n");
	printf("would change extension to .dat \n");
	printf("Options are:\n");
	printf("[-fau]- input in FAU format.\n",(USAIFORMAT==0?"YES":"NO"));
    printf("[-r ] - make raw output file \t(%s)\n",(extract==0?"YES":"NO"));
	printf("[-nc] - optional \"non comment\" swich that\n");
	printf("\tremove text and GROUP and WHOLE TIME column \t(%s)\n",(comment?"YES":"NO"));
	printf("[-bN] - bad value marker \t(default %ld)\n",badval);
	printf("[-XN] - target position X \t(default %g)\n",centerX);
	printf("[-YN] - target position Y \t(default %g)\n",centerY);
	printf("[-skN] - skip first N records \t(default %ld)\n",skipval);
	printf("[-exN] - extract to N time groups \t(default %d)\n",groups);
	printf("[-cN] - number of distance groups \t(default %d)\n",DIST_CLASS);
	printf("[-DN] - fix max distance \t(default %g)\n",FixMaxDistance);
	return 1;
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
		case 'f':USAIFORMAT=1;
			printf("Loadind FAU input file\n");
			break;
		case 'n':comment=0;
			printf("Comment was set to %d.\n",comment);
			break;
		case 'b':badval=atol(argv[i]+2);
			printf("Bad value marker was set to %ld.\n",badval);
			break;
		case 's':skipval=atol(argv[i]+3);
			printf("Skip counter was set to %ld.\n",skipval);
			break;
        case 'r':extract=0;
			printf("Mode was set to raw.\n");
			break;
		case 'e':extract=1;groups=atoi(argv[i]+3);
			printf("Mode was set to grouping. Number of groups is %d.\n",groups);
			break;
		case 'X':centerX=atol(argv[i]+2);
			printf("Target X was set to %g.\n",centerX);
			break;
		case 'Y':centerY=atol(argv[i]+2);
			printf("Target Y was set to %g.\n",centerY);
			break;
		case 'D':FixMaxDistance=atol(argv[i]+2)+1;
			printf("Max distance was fixed on %g.\n",FixMaxDistance);
			break;
		case 'c':DIST_CLASS=atol(argv[i]+2);
			printf("Distance class was set to %d.\n",DIST_CLASS);
			break;
		}
	printf("\n\t W A I T !\n");
	}

LoadFileAction();

if(extract==1)
	MakeExtractFile(skipval,groups);
	else
	SaveFileAction(skipval);

printf("\t D O N E !\n");
return 0;
}

