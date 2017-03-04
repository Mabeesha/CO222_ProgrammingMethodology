/*
	CO222 Project
	E/14/158 Gihan Chanaka Jayatilaka (gihanchanaka@gmail.com)
	
	CLOCK FUCTIONALITY
	Multi color digital clock
	Fixed color analog clock

	COMPILING
	Compile with -lm flag

	USING THE PROGRAM
	run ./progName -H or ./progName -help to get help.

	CODE USED
	CO222 Lab 04 skeleton.c file was used 

*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RESET		0
#define BRIGHT 		1
#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7
#define TRANSPARENT 9


int*** processedPrintData;
char* colors[]={"black","red","green","yellow","blue","magenta","cyan","white"};
char progName[100];



/*struct tm {
   int tm_sec;         seconds,  range 0 to 59        
   int tm_min;         minutes, range 0 to 59         
   int tm_hour;        hours, range 0 to 23           
   int tm_mday;        y of the month, range 1 to 31  
   int tm_mon;         month, range 0 to 11             
   int tm_year;        The number of years since 1900   
   int tm_wday;        day of the week, range 0 to 6    
   int tm_yday;        day in the year, range 0 to 365  
   int tm_isdst;       daylight saving time             	
};

*/


void refreshScreen();
int** loadRawPrintData();
void preparePrintData();
int** makeTimeFrameToPrint(int hour,int min,int sec);
void printBlock(int color);
void textcolor(int attr, int foregroundColor, int backgroundColor);
void printLetter(char c,int color);
void displayTime(int color);
void illuminate(int color);
void dontIlluminate();
int stringCompare(char* a, char* b);
int indexOf(char* string,  char** stringArray);
int isInArray(char* string,  char** stringArray);
void toLowerCase(char* ar);
int len(char* ar);
void copyStringFromTo(char* from,char* to,int len);
void analogClock();
void printSquare(int color);
void printHelp();



int main(int argC,char* argv[]){
	copyStringFromTo(argv[0],progName,len(argv[0]));
	if(argC==1)	displayTime(WHITE);
	else{
		if(stringCompare(argv[1],"-C") || stringCompare(argv[1],"-color")){
			if(argC==3){

				char inputColor[len(argv[2])];
				copyStringFromTo(argv[2],inputColor,len(argv[2]));
				toLowerCase(inputColor);
				if(isInArray(inputColor,colors)){
					displayTime(indexOf(inputColor,colors));
				}
				else{
					fprintf(stderr," %s :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",argv[2]);				
				}
			}
			else{
				if(argC>3) fprintf(stderr,"Too many arguments.\nType %s -H or clock -help to get help\n",progName);
				else{
					fprintf(stderr,"%s :option requires an argument -- 'C'\n",progName);				
					fprintf(stderr,"usage : %s -C [black|red|green|yellow|blue|magenta|cyan|white]\n",progName);

				}
			}

		}
		else if(stringCompare(argv[1],"-A") || stringCompare(argv[1],"-analog")) analogClock();
		else if(stringCompare(argv[1],"-H") || stringCompare(argv[1],"-help")) printHelp();
		else fprintf(stderr,"Invalid input\n Type %s -H or %s -help for the help about the program\n",progName,progName);
	}
	return 0;
}

void printHelp(){
	printf("usage :\n%s -C [black|red|green|yellow|blue|magenta|cyan|white]\n",progName);
	printf("or %s -color [black|red|green|yellow|blue|magenta|cyan|white]\n",progName);
	printf("%s -A or %s -analog to get the analog clock\n",progName,progName);
	printf("%s -H or %s -help for the help about the program\n",progName,progName);
}


void analogClock(){
	int screenRows=30,screenCols=30,row,col;
	time_t rawtime;
	struct tm *tt;
	int** ar;
	int sec=0;
	while(1){
		usleep(1);
		time(&rawtime);
		tt=localtime(&rawtime);
		
		if(sec!=tt->tm_sec){
			sec=tt->tm_sec;
			int** toPrint=(int**)calloc(screenRows,sizeof(int*));
			for(row=0;row<screenRows;row++){
				toPrint[row]=(int*)calloc(screenCols,sizeof(int));
			}

			float radius=(1.0*screenRows)/2;
			float centerRow=(1.0*screenRows)/2;
			float centerCol=(1.0*screenCols)/2;
			//make outer circle
			float ang=0;
			float radiusFraction=0.95;

			while(ang<=360){
				double y=centerRow-((radius*radiusFraction)*sin(M_PI*ang/180));
				double x=centerCol+((radius*radiusFraction)*cos(M_PI*ang/180));
				toPrint[(int)round(y)][(int)round(x)]=1;
				ang+=0.1;
			}

			//Make center
			toPrint[(int)centerRow][(int)centerCol]=3;

			//Make markings
			radiusFraction=0.95;
			for(ang=0;ang<360;ang+=30){
				double y=centerRow-((radius*radiusFraction)*sin(M_PI*ang/180));
				double x=centerCol+((radius*radiusFraction)*cos(M_PI*ang/180));
				toPrint[(int)round(y)][(int)round(x)]=2;
			}
			

			//SECOND
			radiusFraction=0;
			while(radiusFraction<0.8){
				float secAngle=2*M_PI*sec/60;
				double yy=centerRow-((radius*radiusFraction)*cos(secAngle));
				double xx=centerCol+((radius*radiusFraction)*sin(secAngle));
				toPrint[(int)round(yy)][(int)round(xx)]=4;
				radiusFraction+=0.01;
			}

			//MIN
			radiusFraction=0;
			while(radiusFraction<0.6){
				float minAngle=2*M_PI*(tt->tm_min*1.0/60);
				double yy=centerRow-((radius*radiusFraction)*cos(minAngle));
				double xx=centerCol+((radius*radiusFraction)*sin(minAngle));
				toPrint[(int)round(yy)][(int)round(xx)]=5;
				radiusFraction+=0.03;
			}

			//HOUR
			radiusFraction=0;
			while(radiusFraction<0.4){
				float hrAngle=2*M_PI*((tt->tm_hour%12)/12.0);
				double yy=centerRow-((radius*radiusFraction)*cos(hrAngle));
				double xx=centerCol+((radius*radiusFraction)*sin(hrAngle));
				toPrint[(int)round(yy)][(int)round(xx)]=6;
				radiusFraction+=0.01;
			}


			printf("%c[2J",0x1B);
			printf("%c[H\n",0x1B);

			for(row=0;row<screenRows;row++){
				for(col=0;col<screenCols;col++){
					if(toPrint[row][col]==1)printSquare(YELLOW);
					else if(toPrint[row][col]==2)printSquare(WHITE);
					else if(toPrint[row][col]==3)printSquare(BLUE);
					else if(toPrint[row][col]==4)printSquare(RED);
					else if(toPrint[row][col]==5)printSquare(GREEN);
					else if(toPrint[row][col]==6)printSquare(CYAN);
					else printSquare(TRANSPARENT);
				}
				printf("\n");
			}
			usleep(900);

		}
	}
}





int len(char* ar){
	int i=0;
	while(ar[i]!='\0')i++;
	return i;
}

void copyStringFromTo(char* from,char* to,int len){
	int i;
	for(i=0;i<len;i++)to[i]=from[i];
	to[len]='\0';
}

void toLowerCase(char* ar){
	int i=0;
	while(ar[i]!='\0'){
		if(ar[i]>='A' && ar[i]<='Z'){
			int x=ar[i]; //To avoid possible overflows
			x-='A';
			x+='a';
			ar[i]=(char)x;
		}
		i++;
	}
}

void displayTime(int color){
	preparePrintData();
	time_t rawtime;
	struct tm *tt;
	int** ar;
	int sec=0;
	while(1){
		usleep(1);
		time(&rawtime);
		tt=localtime(&rawtime);
		
		if(sec!=tt->tm_sec){
			sec=tt->tm_sec;
			ar=makeTimeFrameToPrint(tt->tm_hour,tt->tm_min,tt->tm_sec);
			printf("%c[2J",0x1B);
			printf("%c[H\n",0x1B);

			int r,c;
			for(r=0;r<5;r++){
				dontIlluminate();
				for(c=0;c<70;c++){

					if(ar[r][c])illuminate(color);
					else dontIlluminate();

				}
				printf("\n");

			}
			printf("\n");
			textcolor(RESET,color,TRANSPARENT);
			printf("%c[22C%d-%02d-%02d\n",0x1B,tt->tm_year+1900,tt->tm_mon+1,tt->tm_mday);
			
			textcolor(RESET,WHITE,9);
			printf("\n");
			usleep(990);

		}
	}
}




void preparePrintData(){
	/*
	[01][02][03]
	[04][05][06]
	[07][08][09]
	[10][11][12]
	[13][14][15]

	number[n]={flag, otherElements}
	flag= 0 if the blocks to be light are given, 1 if the other blocks are given. (Just added to make the input easier)
	*/
	int rawPrintData[11][10]={
		{1,5,8,11},
		{0,3,6,9,12,15},
		{1,4,5,11,12},
		{1,4,5,10,11},
		{1,2,5,10,11,13,14},
		{1,5,6,10,11},
		{1,5,6,11},
		{1,4,5,7,8,10,11,13,14},
		{1,5,11},
		{1,5,10,11},
		{0,5,11} //The colon
	};


	processedPrintData=(int***) calloc(11,sizeof(int**));
	int charNo;
	for(charNo=0;charNo<11;charNo++){
		processedPrintData[charNo]=(int**)calloc(3,sizeof(int*));
		int col;
		for(col=0;col<3;col++){
			processedPrintData[charNo][col]=(int*)calloc(5,sizeof(int));
		}
	}
	for (charNo=0;charNo<11;charNo++){
		if(rawPrintData[charNo][0]==1){
			int row,col;
			for(col=0;col<3;col++){
				for(row=0;row<5;row++){
					processedPrintData[charNo][col][row]=1;
				}
			}
				
		}
		int i=1;
		while(rawPrintData[charNo][i]!='\0' || rawPrintData[charNo][i]!=0){
			processedPrintData[charNo][ (rawPrintData[charNo][i]-1)%3 ][(rawPrintData[charNo][i]-1)/3]=(rawPrintData[charNo][0]==0);	
			i++;
		}
	}
}

int** makeTimeFrameToPrint(int hour,int min,int sec){
	int** toPrint=(int**) calloc(5,sizeof(int*));
	int row,col;
	for(row=0;row<5;row++){
		toPrint[row]=(int*) calloc(100,sizeof(int));
	}
	int tempHMS[]={hour/10,hour%10,min/10,min%10,sec/10,sec%10};

	int i,j;
	int colToStart=0;
	for(i=0;i<6;i++){
		for(row=0;row<5;row++){
			for(col=0;col<3;col++){
				for(j=0;j<2;j++){
					toPrint[row][colToStart+2*col+j]=processedPrintData[tempHMS[i]][col][row];
				}
			}
		}
		colToStart+=6;

		if(i%2==0)colToStart++;

		if(i%2==1 && i<5){
			for(row=0;row<5;row++){
				for(col=0;col<3;col++){
					for(j=0;j<2;j++){
						toPrint[row][colToStart+2*col+j]=processedPrintData[10][col][row];
					}
				}
			}
			colToStart+=6;
		}
	}
	return toPrint;
}

void printBlock(int color){
	textcolor(BRIGHT,BLACK,color);
	printf(" ");
	textcolor(RESET, WHITE, BLACK);	
}


void printSquare(int color){
	printBlock(color);
	printBlock(color);
}
void illuminate(int color){
	printBlock(color);
}


void dontIlluminate(){
	printBlock(TRANSPARENT);
}

void textcolor(int attr, int foregroundColor, int backgroundColor){	
	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, foregroundColor + 30, backgroundColor + 40);
	printf("%s", command);
}


int stringCompare(char* a, char* b){
	int lenA=0,lenB=0,i;
	while(a[lenA]!='\0') lenA++;
	while(b[lenB]!='\0') lenB++;
	if(lenA!=lenB) return 0;
	for(i=0;i<lenA;i++) if(a[i]!=b[i]) return 0;
	return 1;
}


int indexOf(char* string,  char** stringArray){
	int i;
	for(i=0;i<sizeof(stringArray);i++)if(stringCompare(string,stringArray[i])) return i;
	return -1;
}


int isInArray(char* string,  char** stringArray){
	return indexOf(string,stringArray)!=-1;
}