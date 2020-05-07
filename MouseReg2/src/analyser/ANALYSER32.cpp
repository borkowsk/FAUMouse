const char* program_title="\n"
"===============================================================================\n"
" MOUSE PARADIGM ANALYSER VERSION 2004c (3.51) , compilation "__DATE__"\n"
"-------------------------------------------------------------------------------\n";
const char* autors=
"\tProgrammed by Wojciech Borkowski under direction of \n"
"\t         Andrzej Nowak & Robin Vallacher\n"
"\tPlease see the licence.htm file or http:\\\www.iss.uw.edu.pl\~borkowsk\n"
"===============================================================================\n"
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
#include <sys/types.h> 
#include <sys/stat.h>
#include "platform.hpp"
#include "iter_dir.hpp"
extern "C" void StartAboutDialog();

#define DEFAULT_BAD_VALUE  (-99)
#define BRAKMARK	(-FLT_MAX)
#define DECPOSIT	(3)
#define EPSILPOW    double( -(DECPOSIT+1) )

/* User interface tu module */
char InputFileNames[256]="\0";//"exper.dat";
char CurrOutFileName[256]="\0";//"exper.dat";

char Description[1024]="\"Mouse paradigm program - dump file\"";
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
double FixCenterX=-1;						//FOR FIX TARGET MANUALLY
double FixCenterY=-1;						//FOR FIX TARGET MANUALLY
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

double TargCenterX=-1;					
double TargCenterY=-1;	

int LoadWarsawInput(FILE *file)
{					
    char bufor[512];
    long i;
    double LocalMax=-9999;
    double delta=0,deltaDelty=0,deltaT;
    table_position=0;//PUSTA!
    
    //printf("\nLOADING.\n");
    
    fgets(Description,sizeof(Description)-1,file);
    Description[strlen(Description)-1]='\0';
    fgets(bufor,sizeof(bufor),file);
    int skip=0;

    while(strlen(bufor)==0 || 
        bufor[0]=='#' || 
        (strncmp("X\tY",bufor,strlen("X\tY"))!=0 &&		//Stary format 	naglowka
        strncmp("\"X\"",bufor,strlen("\"X\""))!=0	)	//Nowy format	naglowka
        )						//Pusta linia lub jeszcze komentarz lub inny naglowek
    {
        skip++;//printf("Skipped line: %s\n",bufor);
        fgets(bufor,sizeof(bufor),file);
        if(feof(file))
        {
            fprintf(stderr,"Invalid input file\n");
            return -1;
        }
    }
    
    if(skip!=0)
        printf("First %d lines skipped\n",skip);

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
        
        //Sprawdzanie srodka ekranu albo zgadywanie jesli nie jest ustawiony
        if(i==0)
        {                     
            if(FixCenterX!=-1)
            {
                TargCenterX=FixCenterX;
            }
            else
            {
                if(TargCenterX==-1)
                {
                    TargCenterX=X;
                    printf("Supposed X of target as %g  !!!\n",TargCenterX);
                }
                else
                    if(TargCenterX!=X)
                    {
                        fprintf(stderr,"\aFind X of target is %d, but expected is %g\n",X,TargCenterX);
                        fprintf(stderr,"\a\a\aDATA FROM DIFERENT SCREEN RESOLUTION CAN'T BE MIXED!\n");
                        return -2;
                    }
            }

            if(FixCenterY!=-1)
            {
                TargCenterY=FixCenterY;
            }
            else
            {
                if(TargCenterY==-1)
                {
                    TargCenterY=Y;
                    printf("Supposed Y of target as %g  !!!\n",TargCenterY);
                }
                else
                    if(TargCenterY!=Y)
                    {
                        fprintf(stderr,"\aFind Y of target is %d, but expected is %g\n",Y,TargCenterY);
                        fprintf(stderr,"\a\a\aDATA FROM DIFERENT SCREEN RESOLUTION CANT BE MIXED!\n");
                        return -2;
                    }
            }
        }


        //Obliczenie odleglosci
        Distance=sqr(TargCenterX-X)+sqr(TargCenterY-Y);
        
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

int LoadFileAction(const char* FileName)
{
    FILE *file;
    if((file=fopen(FileName,"rt"))==NULL)
    {
        fprintf(stderr,"File: %s\n",FileName);
        perror("Can't read file! \7\7\7");
        exit(1);
    }
    
    if(USAIFORMAT==1)
    {
        if(LoadFAUInput(file)!=0)
        {
            fprintf(stderr,"File format error!!!(OLD FAU INPUT SYNTAX)\n");
            return(2);
        }
    }
    else
    {
        if(LoadWarsawInput(file)!=0)
        {
            fprintf(stderr,"File format error!!!(NEW INPUT SYNTAX)\n");
            return(2);
        }
    }
    
    strcat(Description,FileName);	
    fclose(file);
    return 0;
}


void SaveRawFileAction(const char* FileName,long skip)
//ZAPISYWANIE PLIKU Z SUROWYM FORMATEM (opcja -r czyli "RAW")
{
    FILE *file;
    
    if((file=fopen(FileName,"wt"))==NULL)
    {
        fprintf(stderr,"File: %s\n",FileName);
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
            perror(FileName);
            exit(errno);
        }
    }
    fclose(file);//TU PLIK JEST JUZ GOTOWY
}


int DumpGroup(FILE* file,long start,long krok,bool breakline=true)
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
    if(breakline)
        printf("Time group from %5d to %5d; distance from %.*lf to %.*lf\n",start,end-1,DECPOSIT,LocMinDist,DECPOSIT,LocMaxDist);
    
    if(comment!=0)
        fprintf(file,"%4ld-%4ld\t",start,end-1);
            
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
        return errno;
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
             return errno;   
        }
        
        //DODATKOWA DELTA t * KROK
        if(comment!=0)
            fprintf(file,"[%lg/%lg]\t",fabs(RegTab[start].time-RegTab[start+1].time)*krok,alltime);
        
        //CZASY POSTOJU
        for(int l=0;l<=DIST_CLASS;l++)
            if(EOF==fprintf(file,"%lg\t",DistSteady[l]))
            {
                return errno;
            }
            
        //Czy to koniec lini, czy nastepna grupa bedzie w tej samej
        if(breakline)
            fprintf(file,"\n");

        return 0;
}

void OldStyleHeader(FILE* file,double epsilon,long skip)
{
    int j;

    if(skip==0)
        fprintf(file,"%s [no skipped records] REST TIME: V<%g pix/s\n",Description,epsilon);
    else
        fprintf(file,"%s [skipped records from 0 to %ld]\n",Description,skip-1);
    
    fprintf(file,"\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t\"%s\"\t",
        "G R O U P"," M.DIST  ","   SD    ","M.V(pix/s)","   SD    ","M.A(pix/s^2)","   SD   ");
    
    for(j=0;j<DIST_CLASS;j++)
        fprintf(file,"\"%s%u\"\t\"%s%u\"\t\"%s\"\t\"%s%u\"\t\"%s\"\t\"%s%u\"\t\"%s\"\t",
        "N:sub",j+1,"M.DIST",j+1,"   SD   ","M.V(pix/s)",j+1,"   SD   ","M.A(pix/s^2)",j+1,"   SD  ");      
    
    fprintf(file,"\"WHOLE TIME\"\t\"REST TIME\"\t");
    
    for(j=0;j<DIST_CLASS;j++)
    {
        fprintf(file,"\"REST TIME %u\"\t",j+1);
    }
    
    fprintf(file,"\n");
}

void NewStyleHeader(FILE* file,unsigned TIME_CLASS)
{
    int i,j;

    //if(skip==0)
    //Naglowki danych ogolnych
    fprintf(file,"CASE\tSkipped_records\tRest_eps.\tTarget_X\tTarget_Y\t");
    
    //Naglowki dla calosci czasu   
    fprintf(file,"RECORDS\tMDIST\tSD_MDIST\tM.VEL(pix/s)\tSD_MVEL\tM.ACC(pix/s^2)\tSD_MACC\t");
    
    for(j=0;j<DIST_CLASS;j++)
        fprintf(file,"d%u\tMDIST_d%u\tSD_MDIST_d%u\tM.VEL_d%u(pix/s)\tSD_MVEL_d%u\tM.ACC_d%u(pix/s^2)\tSD_MACC_d%u\t",j+1,j+1,j+1,j+1,j+1,j+1,j+1);      
    
    fprintf(file,"WHOLE_TIME\tREST_TIME\t");
    
    for(j=0;j<DIST_CLASS;j++)
    {
        fprintf(file,"REST_TIME_d%u\t",j+1);
    }

    //Naglowki dla fragmentow czasu
    if(TIME_CLASS>=2)
    for(i=0;i<TIME_CLASS;i++)
    {
        fprintf(file,"RECORDS_t%u\tMDIST_t%u\tSD_MDIST_t%u\tM.VEL_t%u(pix/s)\tSD_MVEL_t%u\tM.ACC_t%u(pix/s^2)\tSD_MACC_t%u\t",i+1,i+1,i+1,i+1,i+1,i+1,i+1);
        
        for(j=0;j<DIST_CLASS;j++)
            fprintf(file,"t%u_d%u\tMDIST_t%u_d%u\tSD_MDIST_t%u_d%u\tM.VEL_t%u_d%u(pix/s)\tSD_MVEL_t%u_d%u\tM.ACC_t%u_d%u(pix/s^2)\tSD_MACC_t%u_d%u\t",i+1,j+1,i+1,j+1,i+1,j+1,i+1,j+1,i+1,j+1,i+1,j+1,i+1,j+1);      
        
        fprintf(file,"WHOLE_TIME_d%u\tREST_TIME_d%u\t",i+1,i+1);
        
        for(j=0;j<DIST_CLASS;j++)
        {
            fprintf(file,"REST_TIME_t%u_d%u\t",i+1,j+1);
        }
    }

    fprintf(file,"\n");
 
}

void WriteCaseInfo(FILE* file,const char* subject,double epsilon,long skip)
{
    //Wartosci ogolne
    fprintf(file,"%s\t%u\t%g\t%g\t%g\t",subject,skip,epsilon,TargCenterX,TargCenterY);
}

void PutExtractedData(FILE* file,long groups,long skip,long krok,bool multi)
{
    DumpGroup(file,skip,groups*krok,multi);//CALOSC
    
    if(groups>=2)	//ODCINKI CZASOWE
        for(long i=0;i<groups;i++)
            DumpGroup(file,skip+i*krok,krok,multi);

    if(!multi)
        fprintf(file,"\t%s\n",Description);
    else
        fprintf(file,"\n");
}

void MakeExtractFile(const char* FileName,long skip,long groups,int multi)
//WYPISYWANIE STATYSTYK GRUP DO PLIKU, GRUPA POD GRUPA 
{
    long krok=-1;
    FILE *file;
    
    if((file=fopen(FileName,"wt"))==NULL)
    {
        perror("Fatal! Can't store data! \7\7\7");
        return;
    }
    
    if(skip<=1)
        skip=2;
    krok=(table_position-skip)/groups;
    skip+=(table_position-skip)-groups*krok;
    
    double epsilon=pow(10,EPSILPOW);//REST: V<%g pix/s
    
    if(multi)
        printf("Mouse rest when V < %g pix/s\n",epsilon);
    
    if(comment!=0)//NAGLOWKI 
    {
       if(multi)
            OldStyleHeader(file,epsilon,skip);
       else
            NewStyleHeader(file,groups);
    }
    
    printf("Records from 0 to %d will be skipped. %d records will be used.\n",skip-1,table_position-skip);
    
    errno=0;
    //FAKTYCZNE OBLICZENIA I ZAPISYWANIE
    if(!multi)
        WriteCaseInfo(file,FileName,epsilon,skip);

    PutExtractedData(file,groups,skip,krok,multi);

    fclose(file);
}

void ExtractToOneLine(FILE* file,const char* CaseName,long skip,long groups)
{
    if(skip<=1)
        skip=2;

    long krok=(table_position-skip)/groups;
    skip+=(table_position-skip)-groups*krok;
    
    double epsilon=pow(10,EPSILPOW);//REST: V<%g pix/s
    
    printf("Records from 0 to %d will be skipped. %d records will be used.\n",skip-1,table_position-skip);
    
    errno=0;
    WriteCaseInfo(file,CaseName,epsilon,skip);
    PutExtractedData(file,groups,skip,krok,false/*NOT MULTI*/);
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

//KOLEJNE PARAMETRY OBROBKI - DEFAULTY MUSZA BYC WIDOCZNE WE write_help()
long skipval=0;
int groups=3;
int extract=1;
int multi=0;  

void write_help(const char* progname)
{
    printf("%s",program_title);
    printf("USAGE:\n\t %s infile [outfile] [ options ]\n\n",progname);
    printf("where infile may include '*' and '?' or it may be a directory name,\n");
    printf("and outfile is optional.\n");
    printf("Options may include the following:\n");
    
    printf("[-m ] - make old-style multi line output .\t(%s)\n",(multi==1?"YES":"NO"));
    printf("[-r ] - make raw output file for dynamic plots.\t(%s)\n",(extract==0?"YES":"NO"));        
    printf("\tThis produce timeseries file for detailed study of a particular case.\n");
    printf("[-XN] - force target position X diferent than stored in input file\n");
    printf("[-YN] - force target position Y diferent than stored in input filen"); 
    printf("[-skN] - skip first N records \t(default is %ld)\n",skipval);
    printf("[-exN] - extract N time groups \t(default is %d)\n",groups);
    printf("[-cN] - extract N distance groups \t(default is %d)\n",DIST_CLASS);        
    printf("[-pN] - output precision in digits after decimal points (%u)\n",DECPOSIT);	
    printf("[-bN] - bad value marker \t(default is %ld)\n",badval);	
    printf("[-DN] - fix max distance \t(default is %g)\n",FixMaxDistance);	
    printf("[-nc] - optional \"non comment\" switch which\n");
    printf("\tremove texts and GROUP and WHOLE TIME column \t(%s)\n",(comment?"YES":"NO"));
    printf("[-fau] - input in old FAU format (not from MOUSEREG) (%s)\n",(USAIFORMAT==0?"NO":"YES"));   
    printf("%s","\t'N' means proper value of paramater.\n\t In (), the default values are provided.");
    StartAboutDialog();   
    getchar();
}

void read_options(int first_opt,int argc,char* argv[])
{
    for(int i=first_opt;argv[i]!=NULL;i++) /* Parse parameters */
        switch(argv[i][1]){
        case 'f':USAIFORMAT=1;
            printf("* Old FAU input file format will be expect\n");
            break;
        case 'n':comment=0;
            printf("* Comment was set to %d.\n",comment);
            break;
        case 'b':badval=atol(argv[i]+2);
            printf("* Bad value marker was set to %ld.\n",badval);
            break;
        case 's':skipval=atol(argv[i]+3);
            printf("* Skip counter was set to %ld.\n",skipval);
            break;
        case 'r':extract=0;
            printf("* Raw output mode was enabled.\n");
            break;
        case 'm':multi=1;
            printf("* Multi line output was enabled.\n");
            break;
        case 'e':extract=1;groups=atoi(argv[i]+3);
            printf("* Mode was set to grouping. Number of groups is %d.\n",groups);
            break;
        case 'X':FixCenterX=atol(argv[i]+2);
            printf("* Target X was set to %g.\n",FixCenterX);
            break;
        case 'Y':FixCenterY=atol(argv[i]+2);
            printf("* Target Y was set to %g.\n",FixCenterY);
            break;
        case 'D':FixMaxDistance=atol(argv[i]+2)+1;
            printf("* Max distance was fixed on %g.\n",FixMaxDistance);
            break;
        case 'c':DIST_CLASS=atol(argv[i]+2);
            printf("* Distance class was set to %d.\n",DIST_CLASS);
            break;
    }    
}

int main(int argc,char* argv[])
{
    int first_opt=2;
      
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    char path_buffer[_MAX_PATH+1];
    
        
    if(argc<2 || strcmp(argv[1],"-h")==0 )
    {
        write_help(argv[0]);
        return 1;
    }
    else
    {       
        printf("%s",program_title);
        printf("%s",autors);     
        printf("Data array ready for %u records. Size of record = %uB\nSize of table = %uB .\n",MAX_DATA_POINTS,sizeof(RegRec),sizeof(RegTabMem));//RegRec huge  RegTabMem[MAX_DATA_POINTS];

        strcpy(InputFileNames,argv[1]);
        printf("Data from %s\n",InputFileNames);
                    
        if(strchr(InputFileNames,'*')==0 && strchr(InputFileNames,'?')==0)
        {

            struct _stat buf;
            int result=_stat(InputFileNames, &buf );// Get data associated with path
            if( result != 0 )
            {
                perror( "Problem getting information about input path" );
                goto READ_ERROR;
            }
            if(buf.st_mode & _S_IFDIR)//to jest katalog
                strcat(InputFileNames,"\\*.out");//Dodajemy ze chodzi o pliki out
        }
       
        if(argv[2]!=NULL && argv[2][0]!='-') // Jest nazwa pliku wyjsciowego
        {
            strcpy(CurrOutFileName,argv[2]);
            printf("Output to %s\n",CurrOutFileName);
            first_opt=3;
        }                               

        read_options(first_opt,argc,argv);
    }
      
    //DECYZJA JAKI SPOSOB OBRABIANIA     
    _fullpath(path_buffer,InputFileNames,_MAX_PATH);
    _splitpath(path_buffer, drive, dir, fname, ext );

    if(strchr(fname,'*')!=NULL || strchr(fname,'?')!=NULL || (*fname=='\0' && *ext=='\0'))
    {       
        char name_buffer[_MAX_FNAME+_MAX_EXT+1];
        sprintf(path_buffer,"%s%s",drive,dir);
        sprintf(name_buffer,"%s%s",fname,ext);        
        directory_iterator currdir(path_buffer,(name_buffer[0]=='\0'?"*.out":name_buffer));
        
        if(!currdir.IsOK())
        {
            fprintf(stderr,"Wrong input parameter: %s%s\n",path_buffer,name_buffer);
            return 1;
        }  

        if(strchr(CurrOutFileName,'*')!=NULL)//Sprawdzanie czy user chce miec wiele plikow - jak w starej wersji
        {            
            //OUTPUT DO WIELU PLIKOW
            for(;currdir!=0;currdir++)
            {                
                sprintf(InputFileNames,"%s%s",currdir.get_name(),*currdir);     
                strcpy(CurrOutFileName,InputFileNames);
                ZmienEXT(CurrOutFileName,"dat"); //Uproszczenie zamiast podkladania gwiazdki
                printf("Processing file %s to %s\n",InputFileNames,CurrOutFileName);

                if(LoadFileAction(InputFileNames)!=0)
                    continue;
                
                if(extract==1)
                    MakeExtractFile(CurrOutFileName,skipval,groups,multi);
                else
                    SaveRawFileAction(CurrOutFileName,skipval);
            }            
        }
        else
        {            
            //OUTPUT DO JEDNEGO PLIKU 
            if((multi==1) || (extract==0) )
            {
                fprintf(stderr,"Option '[m]ulti' and '[r]aw' are incompatible with one file output.\n"
                               " Use \"*.dat\" as output name.\n");
                goto READ_ERROR;
            }
            
            if(*CurrOutFileName=='\0')
            {                
                sprintf(CurrOutFileName,"%s%sallout.dat",drive,dir);
                printf("Output to %s\n",CurrOutFileName);
            }
            
            FILE* file=NULL;
            if((file=fopen(CurrOutFileName,"wt"))==NULL)
            {
                fprintf(stderr,"Can't store data! \7\7\7\n");
                perror(CurrOutFileName);
                goto READ_ERROR;
            }
            
            NewStyleHeader(file,groups);

            for(;currdir!=0;currdir++)
            {
                sprintf(InputFileNames,"%s%s",currdir.get_name(),*currdir);     
                printf("Processing file %s to %s\n",InputFileNames,CurrOutFileName);
                
                if(LoadFileAction(InputFileNames)!=0)
                    continue;

                ExtractToOneLine(file,InputFileNames,skipval,groups);
            }
            
            fclose(file);
        }        
    }
    else
    {   
        if(*CurrOutFileName=='\0')        // Brak nazwy pliku wyjsciowego. Katalog - "S:\MOUSE PARADIGM 2004\DANE\ "
        {	       
            strcpy(CurrOutFileName,InputFileNames);
            ZmienEXT(CurrOutFileName,"dat");
            printf("Output to %s\n",CurrOutFileName);
        }
        
        if(LoadFileAction(InputFileNames)!=0)
        {
            goto READ_ERROR;
        }
        
        if(extract==1)
            MakeExtractFile(CurrOutFileName,skipval,groups,multi);
        else
            SaveRawFileAction(CurrOutFileName,skipval);
    }
    
    printf("\t D O N E !\n<PRESS ENTER>");
    getchar();
    return 0;
READ_ERROR:
    fprintf(stderr,"\t SOME SERIOUS ERRORS OCCURED\n<PRESS ENTER>");
    getchar();
    return -1;
}

