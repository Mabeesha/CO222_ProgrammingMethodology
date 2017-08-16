/*
	CO222 Project 2
	2017-May
	Gihan Chanaka Jayatilaka E/14/158
	gihanchanaka@gmail.com


	Compiling instructions: NONE.
	Tested on: gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)


	Fav line: (320) return toReturn;

	JUSTIFICATIONS:
	1.Why the linked list was not sorted?
	Here we need only several (M) of the highest frequency words.
	That takes only M*N order of operations.
	Sorting takes at least N*ln(N).
	Given that M is a smaller number compared to N,
	(Who wants a graph of a 100 words! The default was 10)
	skimming through the linked list M times was more efficient.

	2.initializing char arrays for fixed size 100:
	It was assumed that the maximum word size would be 100-1 characters.
	The FOURTH longest english word has only 45 letters and even with punctuations it will not reach 99.
	(There 3 other english words longer than 100 characters.)

	3.Creating blank list with two nodes:
	This was just to keep the code simple. These are removed by cleanTempDataFromList() function.
	Implementing the linked list without an initial node is possible but more complicated.
	Since we are first adding data and then getting the result, the approach used here works fine.
	But if it was needed to take results in the middle, this approach may give errors. 

	4. Returning a void** in inputHandle():
	The flag data and the input file list had to be returned out of the function.
	Since C "doesnot" allow returning two arrays of two types of data,
	those arrays' addresses were casted to coid* type and put into one void** and returned.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int totalWords=0;
int maxCountOfAWord=0;

int max(int a,int b){
	if(a>b) return a;
	return b;
}

int min(int a,int b){
	if(max(a,b)==a) return b;
	return a;
}

int isCapitalLetter(char c){
	return (c>='A' && c<='Z');
}

int isSimpleLetter(char c){
	return (c>='a' && c<='z');
}

int isLetter(char c){
	return isSimpleLetter(c) || isCapitalLetter(c);
}

int isNumber(char c){
	return (c<='9' && c>='0');
}
int isAlphaNumeric(char c){
	return isNumber(c)||isLetter(c);
}

char toSimple(char c){
	if(isCapitalLetter(c)) return c-'A'+'a';
	return c;
}

int stringLength(char* str){
	int ans=0;
	while(str[ans]!='\0')ans++;
	return ans;
}

char *stringToSimple(char* str){
	int i;
	for(i=0;i<stringLength(str);i++){
		str[i]=toSimple(str[i]);
	}
	return str;
}

char* preprocess(char *word){
	int i=0,j=0,len=stringLength(word);
	for(i=0;i<len;i++){
		if(isAlphaNumeric(word[i])){
			word[j++]=word[i];
		}
	}
	word[j]='\0';
	stringToSimple(word);
	return word;
}

int charArrayCompare(char *a, char *b){
	// Returns 0 if the strings match
	// Returns 1 if a is lexographically preceding !!!I dont know the spellings :D
	// Returns -1 if b is lexographically receding
	int cmp=strcmp(a,b);
	if(cmp<0)return -1;
	if(cmp>0)return 1;
	return cmp;
}

int wordMatch(char *a,char *b){
	return charArrayCompare(a,b)==0;
}

struct StringNode{
	char *word;
	int count;
	struct  StringNode *next;
};

typedef struct StringNode String;

int stringCompare(String *s1,String *s2){
	return(charArrayCompare(s1->word,s2->word));
}

String *createString(char *s){
	String *newString=malloc(sizeof(String));
	newString->word=s;
	newString->count=1;
	newString->next=NULL;
	return newString;
}

String *copyString(String *s){
	String *newString=malloc(sizeof(String));
	newString->word=s->word;
	newString->count=s->count;
	return newString;
}

int isEmptyString(String *s){
	return (s->word[0]=='\0');
}

void putToList(String* head,String *s){
	if(isEmptyString(s)) return ;
	if(head->next==NULL){
		head->next=s;
	}
	else if(stringCompare(s,head->next)==0){
		head->next->count++;
		free(s);
	}
	else{
		putToList(head->next,s);
	}
}

String *popMaxFromList(String *head){
	int max=0;
	String *maxWord=NULL;
	while(head!=NULL){
		if(max<(head->count)){
			maxWord=head;
			max=head->count;
		}
		head=head->next;
	}
	if(maxWord==NULL) return NULL;
	String *toReturn=copyString(maxWord);
	maxWord->count=0;
	return toReturn;
}

void printString(String *s){
	int i,len=stringLength(s->word);
	for(i=0;i<len;i++)printf("%c",s->word[i]);
}

String *popFirst(String *head){
	return head->next;
}


String *createBlankList(){
	String *head=createString("AA\0");
	return head;
}

String *cleanTempDataFromList(String *head){
	return popFirst(head);
}

void printListFull(String *head){
	while(head!=NULL){
		printf("%s %d\n",head->word,head->count );
		head=head->next;
	}
}

void drawBar_type1(float barWidth,int maxWordLength){
	int i;
	for(i=0;i<maxWordLength+1;i++)printf(" ");
	printf("│");
	for(i=0;i<barWidth;i++)printf("░");
	printf("\n");
}

void drawBar_type2(float barWidth,int maxWordLength,int count,char* word){
	int i,lenWord=stringLength(word);
	printf(" %s",word);
	for(i=0;i<(maxWordLength-lenWord);i++)printf(" ");
	printf("│");
	for(i=0;i<barWidth;i++)printf("░");
	printf("%.2f%%\n",100*count/(float)totalWords);
}
void drawBar_type3(int maxWordLength){
	int i;
	for(i=0;i<maxWordLength+1;i++)printf(" ");
	printf("│");
	printf("\n");
}

void drawBar(char* word, int maxWordLength,int count,int scaled){
	float percentage,barWidth;
	if(!scaled)percentage=100*count/(float)totalWords;
	else percentage=100*count/(float)maxCountOfAWord;
	barWidth=(int)((73-maxWordLength)*percentage/100);
	drawBar_type1(barWidth,maxWordLength);
	drawBar_type2(barWidth,maxWordLength,count,word);
	drawBar_type1(barWidth,maxWordLength);
	drawBar_type3(maxWordLength);
}


void printListFullOrdered(String *head,int maxWordLength,int scaled){
	String *next=popMaxFromList(head);
	maxCountOfAWord=next->count;
	do{
		drawBar(next->word,maxWordLength+1,next->count,scaled);
	}
	while((next=popMaxFromList(head))!=NULL);
}
void drawGraph(String *head,int lines,int scaled){
	String *tempList=createBlankList();
	int longestWordLength=0,i;
	for(i=0;i<lines;i++){
		String *s=popMaxFromList(head);
		longestWordLength=max(longestWordLength,stringLength(s->word));
		putToList(tempList,s);
	}
	tempList=cleanTempDataFromList(tempList);
	printListFullOrdered(tempList,longestWordLength,scaled);
	for(i=0;i<longestWordLength+2;i++)printf(" ");
	printf("└");
	for(i=0;i<80-longestWordLength-3;i++)printf("─");
	printf("\n");
}

int isDigit(char *s){
	int i,len=stringLength(s);
	for(i=0;i<len;i++)if(!isNumber(s[i]))return 0;
	return 1;
}

int listLength(String* head){
	int length=0;
	while(head!=NULL){
		length++;
		head=head->next;
	}
	return length;
}

void* inputHandle(int argCount,char** argVector){
	void** toReturn=(void **)malloc(sizeof(int*)+sizeof(FILE**));
	// controlData[0]= 1=OKAY TO GO
	// controlData[1]=No of files
	// controlData[2]=length
	// controlData[3]=scaled
	// controlData[4]= word=0,char=1
	int* controlData=(int*)malloc(sizeof(int)*10);
	controlData[2]=10;
	FILE** fileList=(FILE**)malloc(sizeof(FILE*)*100);

	toReturn[0]=(void*)controlData;
	toReturn[1]=(void*)fileList;



	int i;

	for(i=1;i<argCount;i++){
		if(argVector[i][0]=='-'){
			if(wordMatch(argVector[i],"-l")||wordMatch(argVector[i],"--length")){
				controlData[2]=atoi(argVector[i+1]);
				i++;
			}
			else if(wordMatch(argVector[i],"-s")||(wordMatch(argVector[i],"--scaled")||wordMatch(argVector[i],"-scaled"))){
				controlData[3]=1;
			}
			else if(wordMatch(argVector[i],"-w")||wordMatch(argVector[i],"-w")){
				if(wordMatch(argVector[i],"-c"))controlData[4]+=1;
				if(wordMatch(argVector[i],"-w"))controlData[4]+=2;

			}
			else{
				printf("usage: %s [-l --length] [-w | -c] [-s --scaled] filename1 filename2 ..\n", argVector[0]);
				controlData[0]=0;
			}
		}
		else{
			if(!isDigit(argVector[i])){
				FILE *thisFile=fopen(argVector[i],"r");
				if(thisFile!=NULL){
					controlData[0]=1;
					fileList[controlData[1]]=thisFile;
					controlData[1]++;
				}
				else{
					printf("Cannot open one or more given files\n");
					controlData[0]=0;
				}
			}
		}
	}
	if(controlData[1]==0){
		printf("No input files were given\n");
		printf("usage: %s [-l --length] [-w | -c] [-s --scaled] filename1 filename2 ..\n", argVector[0]);
	}

	if(controlData[4]>2){
		controlData[0]=0;
		printf("[-c] and [-w] cannot use together\n");
	}
	else{
		if(controlData[4]!=1)controlData[4]=0;
	}
	//Apparently you can return an integer array and a file array from a single function!!
	return toReturn;

}

String *countWords(void** inputCommands){
	String *head=createBlankList();
	int fileNo;
	for( fileNo=0;fileNo< ((int*)inputCommands[0])[1];fileNo++){
		char *readWord=(char*)malloc(100);
		while(fscanf(((FILE**)inputCommands[1])[fileNo],"%s ",readWord)>0){
			String *toAdd=createString(preprocess(readWord));
			if(!isEmptyString(toAdd)){
				putToList(head,toAdd);

				readWord=(char*)malloc(100);
				totalWords++;
			}

		}
	}
	head=cleanTempDataFromList(head);
	return head;
}

String *countLetters(void** inputCommands){
	String *head=createBlankList();
	int fileNo,i,len;
	for( fileNo=0;fileNo< ((int*)inputCommands[0])[1];fileNo++){
		char *readWord=(char*)malloc(100);
		while(fscanf(((FILE**)inputCommands[1])[fileNo],"%s ",readWord)>0){
			preprocess(readWord);
			len=stringLength(readWord);
			for(i=0;i<len;i++){
				char* letterToAdd=(char*)malloc(sizeof(char)*2);
				letterToAdd[0]=readWord[i];
				letterToAdd[1]='\0';
				putToList(head,createString(letterToAdd));
				totalWords++;
			} 
		}
	}
	head=cleanTempDataFromList(head);
	return head;
}



int main(int argCount,char* argVector[]){
	String *head=createBlankList();
	void** inputCommands=inputHandle(argCount,argVector);
	if(((int*)inputCommands[0])[0]){

		if(!((int*)inputCommands[0])[4])  head=countWords(inputCommands);
		else head=countLetters(inputCommands);
		drawGraph(head,min(listLength(head),((int*)inputCommands[0])[2])     ,((int*)inputCommands[0])[3]);
	}

	return 0;
}
