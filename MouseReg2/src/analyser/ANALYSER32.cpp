const char* VERSION=
"\n"
"===========================================================\n"
" ANALYSER FOR DATA OBTAINED WITHIN MOUSE PARADIGM.\n"
"-----------------------------------------------------------\n"
"Programmed by Wojciech Borkowski under direction of \n"
"         Andrzej Nowak & Robin Vallacher\n"
"(VERSION 3.20b, M$ Windows port, compilation "__DATE__")\n"
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
#include <float.h>
#include "platform.hpp"

#define DEFAULT_BAD_VALUE  (-99)
#define BRAKMARK	(-FLT_MAX)
#define DECPOSIT	(3)
#define EPSILPOW    double( -(DECPOSIT+1) )

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
	int		click;
	double	speed;
	double	accel;
	};

//ZMIENNE GLOWNEJ TABLICY REKORDOW
//////////////////////////////////////////
const size_t MAX_DATA_POINTS=(  10L*    //10 zapisow na sekunde
                                60L*    //60 sekund na minute
                                30L*    //30 minut to i tak za duzo
                                2       //ale po co jeszcze to ?????????
                                );	//NAJWIEKSZA MOZLIWA ILOSC REKORDOW

RegRec huge  RegTabMem[MAX_DATA_POINTS];
RegRec huge* RegTab=RegTabMem;
unsigned long table_position;

// INNE PARAMETRY 
//////////////////////////////////
int USAIFORMAT=0;						//Czy wczytuje format FAU
int DIST_CLASS=2;						//ILE KLAS ODLEGLOSCI
double centerX=-1;						//DETECT
double centerY=-1;						//DETECT
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
	  sum=sumSqr=N=0;//Zerowanie w konstruktorze!!!
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


int LoadWarsawInput(FILE *file)
{
char bufor[512];
long i;
double LocalMax=-9999;
double delta=0,deltaDelty=0,deltaT;


//printf("\nLOADING.\n");

fgets(Description,sizeof(Description)-1,file);
Description[strlen(Description)-1]='\0';
fgets(bufor,sizeof(bufor),file);
while(strlen(bufor)==0 || 
	  bufor[0]=='#' || 
	  (strncmp("X\tY",bufor,strlen("X\tY"))!=0 &&		//Stary format 	naglowka
	   strncmp("\"X\"",bufor,strlen("\"X\""))!=0	)	//Nowy format	naglowka
	  )						//Pusta linia lub jeszcze komentarz lub inny naglowek
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

	//Wczytywanie rekordu
	if( (nfie=fscanf(file,"%d",     &X		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%d",     &Y		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%le",    &pTime	))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%d",     &click	))==EOF || nfie!=1) goto ERROR;
	nfie=fgetc(file);/* Powinno zjesc koniec lini! */
	if(nfie!='\n' && nfie!=EOF) 
					goto ERROR;
	nfie=fgetc(file);/* Sprawdza czy jest jeszcze jakis znak */
	if(nfie!=EOF) ungetc(nfie,file);

	//Zgadywanie srodka ekranu jesli nie jest ustawiony
	if(centerX==-1 && i==0)
	{
		centerX=X;
		printf("Supposed X of target as %g  !!!\n",centerX);
	}
	if(centerY==-1 && i==0)
	{
		centerY=Y;
		printf("Supposed Y of target as %g  !!!\n",centerY);
	}

	//Obliczenie odleglosci
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

	assert(Distance>=0);

	//Zapamietanie w tablicy dla procedur wyjsciowych
	RegTab[i].time=pTime;
	RegTab[i].click=click;
	RegTab[i].distance=Distance; // Maybe 0 or great than 0 - AKTUALNA ODLEGLOSC

	if(i>0) //Juz mozna liczyc
	{
		deltaT=fabs(RegTab[i].time-RegTab[i-1].time);//AKTUALNA ROZNICA CZASU
		
		assert(deltaT>=0);
		
		if(deltaT==0)	//POWINNA BYC RACZEJ MNIEJSZA NIZ 1 ALE NIE O!!!
		{
			fprintf(stderr,"WARNING! DeltaT=0 (time)rec %ld & %ld\n",i-1,i);
			continue;
		}
		
		delta=fabs(RegTab[i-1].distance-Distance)/deltaT;	//PREDKOSC - ZMIANA ODLEGLOSCI W CZASIE 
		RegTab[i].speed=delta;		//Reszta zapamietywania w tablicy

		deltaDelty=(delta-RegTab[i-1].speed)/deltaT;		//PRZYSPIESZENIE - ZMIANA PREDKOSCI W CZASIE
		if(i>1)
			RegTab[i].accel=deltaDelty;	//Reszta zapamietywania w tablicy
			else
			RegTab[i].accel=BRAKMARK;	//Dla 0 i 1 rekordu nie da sie policzyc
		
	}
	else
	{
		RegTab[i].speed=BRAKMARK;
		RegTab[i].accel=BRAKMARK;
	}

	continue;
ERROR:
	fprintf(stderr,"Data format error at %d. record. LOAD BREAK!\n",i+1);
	return -1;
	}

//#ifdef __MSDOS__
//i--; /* \r\n sequention make error-behavior of feof() */
//#endif
table_position=i;
if(FixMaxDistance!=0)
	MAX_DIST=FixMaxDistance;
	else
	MAX_DIST=LocalMax+1/* Drobny dodatek zeby zeby distance/MAX <1 */;
return 0;
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


int LoadFAUInput(FILE *file)
{
char		resztki[1024];
char		quest[128];
char		subj[512];
long		i;
double		LocalMax=-9999;

memset(Description,'\0',sizeof(Description));	
memset(Description,' ',30);		//20 spacji na poczatku zeby wpisac subject
fgets(&Description[21],sizeof(Description)-21,file);//Jedna linia naglowka kolumn
Description[strlen(Description)-1]='\0';	//Zakonczona 

//subj-cond,Quest,time,x,y,dist,speed,accel

for(i=0;!feof(file);i++)
	{
	int			nfie=0;
	float		pTime;
	int			X=0;
	int			Y=0;
	int			click=0;
	float		Distance;
	float		Speed;
	float		Acceleration;

	if( (nfie=LoadStrTo(file,subj,','))==EOF || nfie!=1) goto ERROR;//Subject-condition
	if(	(nfie=LoadStrTo(file,quest,','))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%f,",    &pTime	))==EOF || nfie!=1) goto ERROR;
	if( (nfie=fscanf(file,"%d,",     &X		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%d,",     &Y		))==EOF || nfie!=1) goto ERROR;
	if(	(nfie=fscanf(file,"%f,",   &Distance))==EOF || nfie!=1) goto ERROR;
	if( Distance<0 )
	{
		fprintf(stderr,"Negative distance %g .\n",Distance);
		goto ERROR;
	}
	if(	(nfie=fscanf(file,"%f,",   &Speed	))==EOF || nfie!=1){ if(i==0) Speed=BRAKMARK; else goto ERROR;}
	if(	(nfie=fscanf(file,"%f",&Acceleration))==EOF || nfie!=1){ if(i<2)  Acceleration=BRAKMARK; else goto ERROR;}
	if( (nfie=LoadStrTo(file,resztki,'\n'))==EOF || nfie!=1) goto ERROR;//Reszta lini
	
	if( strlen(resztki)>=sizeof(resztki) ) 
				goto ERROR;

	nfie=fgetc(file);/* Sprawdza czy jest jeszcze jakis znak do przeczytania */
	if(nfie!=EOF && nfie!=subj[0]) 
				goto ERROR; /* Powinien byc koniec pliku lub to samo co w poprzedniej lini */
	
	if(nfie!=EOF) 
			ungetc(nfie,file);

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
	return -1;	
	}

//#ifdef __MSDOS__
//i--; /* \r\n sequention make error-behavior of feof() */
//#endif
memcpy(Description,subj,strlen(subj));
memcpy(Description+strlen(subj)+1,quest,strlen(quest));
table_position=i;
if(FixMaxDistance!=0)
	MAX_DIST=FixMaxDistance;
	else
	MAX_DIST=LocalMax+1/* Drobny dodatek zeby zeby distance/MAX <1 */;
return 0;
}

void LoadFileAction()
{
	FILE *file;
	if((file=fopen(FileName,"rt"))==NULL)
	{
		fprintf(stderr,"File: %s\n",FileName);
		perror("Fatal! Can't read data! \7\7\7");
		exit(1);
	}
	
	if(USAIFORMAT==1)
	{
		if(LoadFAUInput(file)!=0)
		{
			fprintf(stderr,"File format (FAU) error!!!\n CAN'T CONTINUE.");
			exit(2);
		}
	}
	else
	{
		if(LoadWarsawInput(file)!=0)
		{
			fprintf(stderr,"File format (WARSAW) error!!!\n CAN'T CONTINUE.");
			exit(2);
		}
	}

	strcat(Description,FileName);	
	fclose(file);
}


void SaveFileAction(long skip)
//ZAPISYWANIE PLIKU Z SUROWYM FORMATEM (opcja -r czyli "RAW")
{
FILE *file;

if((file=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7\7");
	return;
	}
	//else errno=0;
if(comment!=0)
	{
	if(skip>0)
		fprintf(file," %s [skipped records from 0 to %ld]\n",Description,skip-1);
		else
		fprintf(file," %s [all records translated]\n",Description);
	fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\n","  TIME  ","CLIK","DISTANCE","V(pix/sec)","A(pix/sec^2)");
	}

printf("Records from 0 to %d will be skipped. %d records will be used.\n",skip-1,table_position-skip);

for(long i=skip;i<table_position;i++)	// Calculations - PETLA LICZENIA
	{
	assert(RegTab[i].distance>=0);
	//WLASCIWY ZAPIS DO PLIKU WYJSCIOWEGO
	if(EOF==fprintf(file,"%8.3lf\t%3d\t%8.*lf\t%8.*lf\t%10.*lf\n",
			RegTab[i].time,
			RegTab[i].click,
			DECPOSIT,RegTab[i].distance,
			DECPOSIT,(RegTab[i].speed!=BRAKMARK?fabs(RegTab[i].speed):badval),
			DECPOSIT,(RegTab[i].accel!=BRAKMARK?fabs(RegTab[i].accel):badval)
			)
		)
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
double     DistSteady[MXCLAS];
stat_class DistClasses[MXCLAS];
stat_class VeloClasses[MXCLAS];
stat_class AcceClasses[MXCLAS];

double LocMinDist=INT_MAX; // Najmniejsza odleglosc od trrget w grupie - do ustawienia
double LocMaxDist=0;      // Najwieksza odleglosc od target w grupie - do ustawienia

for(int k=0;k<MXCLAS;k++)
		DistSteady[k]=0.0;	//Musi byc wyzerowane bo nie ma tu zerowania w konstruktorze

//if(start<=1)
//	start=2;
assert(start>=2);//Conajmniej bo dwa pierwsze rekordy maja braki w "speed" i "accel"

for(i=start;i<end;i++)// MIN/MAX searching 
    {
    double  distance=RegTab[i].distance;
	assert(distance>=0);
    if(distance>LocMaxDist)
		LocMaxDist=distance;
    if(distance<LocMinDist)
		LocMinDist=distance;
    }

double alltime=0;//Konrolne zliczanie calego czasu
double epsilon=pow(10,EPSILPOW);//REST: V<%g pix/s
printf("Time group from %5d to %5d; distance from %.*lf to %.*lf\n",start,end-1,DECPOSIT,LocMinDist,DECPOSIT,LocMaxDist);

if(comment!=0)
	fprintf(file,"{%4ld-%4ld}\t",start,end-1);



for(i=start;i<end;i++)// Calculations - WLASCIWE OBLICZENIA W PETLI DLA GRUP
	{
	double  distance=RegTab[i].distance;
	double	deltaT=fabs(RegTab[i].time-RegTab[i-1].time);
	double  delta=fabs(RegTab[i].speed);		//PREDKOSC - ROZNICA ODLEGLOSCI W CZASIE
	double  deltaDelty=fabs(RegTab[i].accel);	//PRZYSPIESZENIE - ROZNICA PREDKOSCI W CZASIE
	
	if(deltaT==0)//ZAZWYCZAJ MNIEJSZE NIZ 1 ALE NIE 0!!!
		{
		fprintf(stderr,"WARNING! DeltaT=0 (time)rec %ld & %ld(is skipped)\n",i-1,i);
		continue;
		}

	//olddelta=delta;	//ZAPAMIETANIE DLA NASTEPNEGO KROKU PETLI
	//olddistance=distance;

	//REJESTRACJA STATYSTYK GLOBALNYCH
	DistClasses[0].Register(distance);
	VeloClasses[0].Register(delta);
	AcceClasses[0].Register(fabs(deltaDelty));
	
	//printf("%e %g\n",delta,delta);
	alltime+=deltaT;
	if(-epsilon<delta && delta<epsilon)/* Gdy predkosc +-=0 */
	{
		//printf("%e %10.*lf\n",delta,DECPOSIT,delta);
		DistSteady[0]+=deltaT;
	}

	//REJESTRACJA DLA STATYSTYK GRUPOWYCH
	if(LocMaxDist>LocMinDist)//O ile wogole jest zroznicowanie
	{
		unsigned GroupIndex=
			(
			(distance-LocMinDist)
			/
			(LocMaxDist-LocMinDist)
			)
			*DIST_CLASS+1;
		
		if(GroupIndex>DIST_CLASS)//Wypadl MAX
		{
			GroupIndex=DIST_CLASS;
		}

		assert(0<GroupIndex && GroupIndex<=DIST_CLASS);

		DistClasses[GroupIndex].Register(distance);
		VeloClasses[GroupIndex].Register(delta);
		AcceClasses[GroupIndex].Register(fabs(deltaDelty));
		
		if(-epsilon<delta && delta<epsilon)/* Gdy predkosc +-=0 */
		{
			DistSteady[GroupIndex]+=deltaT;
		}
	}

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
    if(EOF==fprintf(file," %5u\t%8.*lf\t%8.*lf\t%8.*lf\t%8.*lf\t%10.*lf\t%10.*lf\t",
	    DistClasses[j].N,
	    DECPOSIT,DistClasses[j].Mean(),
		DECPOSIT,DistClasses[j].Sigma(),
	    DECPOSIT,VeloClasses[j].Mean(),
		DECPOSIT,VeloClasses[j].Sigma(),
	    DECPOSIT,AcceClasses[j].Mean(),
		DECPOSIT,AcceClasses[j].Sigma() ))
		{
			perror(OutFileName);
			exit(errno);
		}

//DODATKOWA DELTA t * KROK
if(comment!=0)
	fprintf(file,"[%lg/%lg]\t",fabs(RegTab[start].time-RegTab[start+1].time)*krok,alltime);

//CZASY POSTOJU
for(int l=0;l<=DIST_CLASS;l++)
	if(EOF==fprintf(file,"%lg\t",DistSteady[l]))
		{
			perror(OutFileName);
			exit(errno);
		}
	
fprintf(file,"\n");
}

void MakeExtractFile(long skip,long groups)
//WYPISYWANIE STATYSTYK GRUP DO PLIKU 
{
long krok=-1;
FILE *file;

if((file=fopen(OutFileName,"wt"))==NULL)
	{
	perror("Fatal! Can't store data! \7\7\7");
	return;
	}

if(skip<=1)
		skip=2;
krok=(table_position-skip)/groups;
skip+=(table_position-skip)-groups*krok;

double epsilon=pow(10,EPSILPOW);//REST: V<%g pix/s
printf("Mouse rest when V<%g pix/s\n",epsilon);
if(comment!=0)//NAGLOWKI 
    {
    if(skip==0)
		fprintf(file,"%s [no skipped records] REST TIME: V<%g pix/s\n",Description,epsilon);
	else
	fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
	
	fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
		"G R O U P"," M.DIST  ","   SD    ","M.V(pix/s)","   SD    ","M.A(pix/s^2)","   SD   ");
	
	for(int j=0;j<DIST_CLASS;j++)
		fprintf(file,"\"%s%u\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
		"N:sub",j+1,"{ M.DIST","   SD   ","M.V(pix/s)","   SD   ","M.A(pix/s^2)","   SD  }");
	
	fprintf(file,"\"WHOLE TIME\"\t\"REST TIME\"\t\"<-REST TIME FOR DISTANCE GROUPS->\"\t\n");
    }

printf("Records from 0 to %d will be skipped. %d records will be used.\n",skip-1,table_position-skip);

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
printf("Data array ready for %u records. Size of record = %uB . Size of table = %uB .\n",MAX_DATA_POINTS,sizeof(RegRec),sizeof(RegTabMem));//RegRec huge  RegTabMem[MAX_DATA_POINTS];


if(argc<2 || strcmp(argv[1],"-h")==0 )
	{
	printf("USAGE:\n\t %s infile [outfile] [ options ]\n\n",argv[0]);
	printf("where outfile is optional. If it does not exist, the program\n");
	printf("make name from infile adding extension .dat \n");
	printf("Options are:\n");
	printf("[-fau] - input in FAU format (not from MOUSEREG) (%s)\n",(USAIFORMAT==0?"NO":"YES"));    
    printf("[-r ] - make raw output file in reverse of normal output.\t(%s)\n",(extract==0?"YES":"NO"));
    printf("\tThis is timeseries file for detailed study of a particular case.\n");
	printf("[-XN] - target position X \n");
	printf("[-YN] - target position Y \n");
    printf("\tIt is very i m p o r t a n t  to provide a proper value!!!\n");
	printf("[-skN] - skip first N records \t(default %ld)\n",skipval);
	printf("[-exN] - extract to N time groups \t(default %d)\n",groups);
	printf("[-cN] - number of distance groups \t(default %d)\n",DIST_CLASS);
	printf("[-DN] - fix max distance \t(default %g)\n",FixMaxDistance);	
    printf("[-pN] - output precision in digits after decimal points (%u)\n",DECPOSIT);	
    printf("[-bN] - bad value marker \t(default is %ld)\n",badval);	
    printf("[-nc] - optional \"non comment\" switch which\n");
	printf("\tremove texts and GROUP and WHOLE TIME column \t(%s)\n",(comment?"YES":"NO"));
    printf("%s","\t'N' means proper value of paramater.\n\t In (), the default values are provided.\n");

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

