/***************************************************************************************************************************************************
*
*                                 This program outputs the common ancestor for any two people 
*(The speed of this program can be greatly decreased by writing on file a big bunch of data from one command instead of how it is done here)
*                                 
****************************************************************************************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define INT int32_t
#define MAXINT INT32_MAX
#define MAXPOP 1000000
#define MAXGEN 1000
#define MAXDEGREE 10
#define pow(two,MAXDEGREE) 1024

typedef struct  
{	INT mum;
	INT dad;
} structsomeone;

struct structrelative
{	INT listrelative[pow(two,MAXDEGREE)];
	INT numrelative;
	INT highestgen; 
};
//
//     get the number of people or generation generation of the output of GeneEvolve located in the path path 
//	
INT getmaxpeople(char path[], INT generation)
{	FILE *fp; 
    INT count = 0;  // Line counter (result) 
    char filename[100]; 
    char c;  // To store a character read from file 
    strcpy(filename,path);
	strcat (filename, ".info.pop1.gen");
	char number[100];
    sprintf(number, "%d", generation);
	strcat(filename,number);
	strcat(filename,".txt");
    // Open the file 
    fp = fopen(filename, "r"); 
    // Check if file exists 
    if (fp == NULL) 
    {   printf("Could not open file %s", filename); 
        return 0; 
    }; 
    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
    {   if (c == '\n') // Increment count if this character is newline 
        count = count + 1; 
	}
    // Close the file 
    fclose(fp); 
	return (count-1);	
}
//
//  read an positive integer from the file file
//	
INT readinteger(FILE * file)
{	INT ID=0;
	char carac;
	do 
	{	carac=getc(file);
		if (carac!=EOF && carac>47 && carac<58) ID=ID*10+carac-48;
	}	while (carac!=EOF && carac>47 && carac<58);
	if (carac==EOF) ID=MAXINT;
	return(ID); 	
}
//
//  Read the file of generation generation of the output of GeneEvolve located in the path path to fill poeple with the ID of each individual, the ID of their mum
//  then the ID of their dad
//	People id filled with one ID per individual which is the ID in the output of GeneEvolve + the number of people in previous generations
//		
INT readfile(char path[],INT generation,structsomeone people[],INT maxpeople[])
{	FILE * file;
	char filename[100]; 
	strcpy (filename,path);
	strcat(filename,".info.pop1.gen");
	char number[100];
    sprintf(number, "%d", generation);
	strcat(filename,number);
	strcat(filename,".txt");
    if ((file = fopen(filename, "r")) == NULL) return (1);
    printf("file %s opened\n",filename);
	char carac;
    do {carac=getc(file);} while (carac!='\n' && carac!= EOF);
	INT ID=0;
	do 
	{	ID=readinteger(file);
		printf("ID %d,",ID);
		if (ID<MAXINT)
		{	ID                         = ID - 1 + maxpeople[generation];
			people[ID].dad             = readinteger(file) - 1 + maxpeople[generation-1];
			//printf("ID %d,",people[ID+maxpeople[generation]].dad);
			people[ID].mum             = readinteger(file) - 1 + maxpeople[generation-1];
			//printf("ID %d,",people[ID+maxpeople[generation]].mum);
			people[people[ID].dad].dad = readinteger(file) - 1 + maxpeople[generation-2];
			//printf("ID %d,",people[people[ID+maxpeople[generation]].dad + maxpeople[generation-1]].dad);
			people[people[ID].dad].mum = readinteger(file) - 1 + maxpeople[generation-2];
			//printf("ID %d,",people[people[ID+maxpeople[generation]].dad + maxpeople[generation-1]].mum);
			people[people[ID].mum].dad = readinteger(file) - 1 + maxpeople[generation-2];
			//printf("ID %d,",people[people[ID+maxpeople[generation]].mum + maxpeople[generation-1]].dad);
			people[people[ID].mum].mum = readinteger(file) - 1 + maxpeople[generation-2];
			//printf("ID %d,\n",people[people[ID+maxpeople[generation]].mum + maxpeople[generation-1]].mum);
			do carac=getc(file); while (carac!='\n' && carac!=EOF);
		};
	} while (ID<MAXINT);
	printf("\n");	
    fclose (file);
}
//
//  add ancestor to the relative
//		
INT addtorelative(INT ancestor,struct structrelative *ptrelative,INT gen,INT maxpeople[])
{	ptrelative->listrelative[ptrelative->numrelative]=ancestor;
	ptrelative->numrelative++;
	if (ptrelative->highestgen<gen) ptrelative->highestgen=gen;
	//printf("ID %d is a relative at generation %d\n",ancestor+1-maxpeople[gen],gen); 
}
//
//  store each individual, the ID of their mum then the ID of their dad
//		
structsomeone people[MAXPOP];
//
//  Search for relative of indiv1 and indiv2
//		
INT compare(INT indiv1,INT indiv2,INT mingeneration,INT currentgeneration,struct structrelative *ptrelative,INT maxpeople[])
{	//printf(" %p \n", ptrelative);
	if (people[indiv1].mum==people[indiv2].mum)
	{	addtorelative(people[indiv1].mum,ptrelative,currentgeneration,maxpeople);
		if (people[indiv1].dad==people[indiv2].dad)
		{	addtorelative(people[indiv1].dad,ptrelative,currentgeneration,maxpeople);
			if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].mum,people[indiv1].dad,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		} else 
		{ 	if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].dad,people[indiv2].dad,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].dad,people[indiv2].mum,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].dad,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		};
	} else
	{ 	if (people[indiv1].dad==people[indiv2].dad)
		{	addtorelative(people[indiv1].dad,ptrelative,currentgeneration,maxpeople);
			if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].mum,people[indiv2].mum,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].dad,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].dad,people[indiv2].mum,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		} else 
		{ 	if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].dad,people[indiv2].dad,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].dad,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].dad,people[indiv2].mum,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].mum,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		};
	};
	return (0);
}
//
//  Get the generation of an ID from people
//		
INT getgeneration(INT ID,INT maxpeople[])
{	INT gen=0;
	INT IDn=ID; 
	while(IDn-maxpeople[gen+1]>-1)
	{	gen++;
	};
	return (gen);
}
//
//  Print relative between indiv1 and indiv2
//		
INT printrelative(INT indiv1,INT indiv2,struct structrelative *ptrelative,INT maxpeople[],INT lastgeneration)
{	INT relat;
	printf("There is/are %d relatives\n",ptrelative->numrelative );
	if (ptrelative->numrelative>0) for(relat=0;relat<ptrelative->numrelative;relat++)
	{	INT gen=getgeneration(ptrelative->listrelative[relat],maxpeople);
		INT ID=ptrelative->listrelative[relat]+1-maxpeople[gen]; 
		printf("ID %d is a relative at generation %d\n",ID,gen);
	};
	if (ptrelative->numrelative>0) printf("The most recent relative is at a degree of %d\n",lastgeneration - ptrelative->highestgen);
	return (0);
}
//
//  Create the file relative.txt
//		
INT initsave(char path[])
{	FILE * file;
	char filename[100]; 
	strcpy (filename,path);
	strcat(filename,".relatives.txt");
    if ((file = fopen(filename, "w+")) == NULL) return 1; 
    fprintf(file,"ID1 ID2 #of_relatives #closest_degree\n");
	fflush(file);
    fclose (file);
}
//
//  store in relative.txt the relative between indiv1 and indiv2
//		
INT saverelative(char path[],INT indiv1,INT indiv2,INT nbrelative,INT lowestdegree,INT maxpeople[])
{	FILE * file;
	char filename[100]; 
	strcpy (filename,path);
	strcat(filename,".relatives.txt");
    if ((file = fopen(filename, "a")) == NULL) return 1;
    fprintf(file,"%d %d %d %d\n",indiv1+1-maxpeople[getgeneration(indiv1,maxpeople)],
				indiv2+1-maxpeople[getgeneration(indiv2,maxpeople)],
				nbrelative,
				lowestdegree);
	fclose (file);
}
//
//  main program
//	
int main(int argc, char *argv[])
{	printf("Start...\n");
	//
	printf("Reading arguments...\n");
	//
	INT maxdegree;
	if (argc<3)
    {   printf("argument degree of relatness missing. To run the program the arguments --max-degree X where X is the highest degree of relatness\
			need to be added. Default is 7\n");
		maxdegree=7;
	} else 
	{	maxdegree = atoi (argv[2]);
	};
	printf("Max degree is %d\n",maxdegree);
	INT lastgeneration;
    if (argc<5)
	{   printf("argument generation missing. To run the program the arguments --generation X where X is the generation to analyse need to be added\
		. Default is 28\n");
		lastgeneration=28;
	} else 
	{	lastgeneration = atoi (argv[4]);
	};
	printf("Last generation is %d\n",lastgeneration);
	char path[100];
	if (argc<7)
	{   printf("The path of the run is missing. Default is .\n");
	} else 
	{	strcpy(path,argv[6]);
	};
	printf("The path is %s\n",path);
	//
	// maxpeople store the number of people in previous generations, the following lines fill maypeople 
	//
	INT maxpeople[MAXGEN];
	maxpeople[0]=0;
	INT generation;
	for(generation=1;generation<lastgeneration+2;generation++)
	{	maxpeople[generation]=getmaxpeople(path,generation-1)+maxpeople[generation-1];
		printf("Population size in generation %d is %d\n",generation-1,maxpeople[generation]-maxpeople[generation-1]);
	};
	//
	// people store for each eprson and each generation the ID of the mum and the ID of the dad, the following lines fill people 
	//
	INT degree=maxdegree;
	generation=lastgeneration;
	while (degree>0)
	{	if (generation>0) readfile(path,generation,people,maxpeople);
		degree=(degree>1)?degree-2:0;
		generation=(generation>1)?generation-2:0;
	};
	//
	// initsave creat the file relative.txt
	//
	initsave(path);
	//
	// the following loop fill the file relative.txt by considering all pair of individual in the final generation
	//
	INT indiv1;
	for(indiv1=maxpeople[lastgeneration];indiv1<maxpeople[lastgeneration+1];indiv1++)
	{	printf("Compare ID %d and the rest of the world \n",indiv1+1-maxpeople[lastgeneration]);
		INT indiv2;
		for(indiv2=1+indiv1;indiv2<maxpeople[lastgeneration+1] ;indiv2++) 
		{	struct structrelative relative;
			relative.numrelative=0;
			relative.highestgen=0;
			struct structrelative *ptrelative=&relative;
		//	printf("and ID %d:\n ",indiv2+1-maxpeople[lastgeneration]);
			compare(indiv1,indiv2, lastgeneration-maxdegree,lastgeneration-1,ptrelative,maxpeople);
			if (ptrelative->numrelative)
			{//	printf("Print relative between ID %d and %d \n",indiv1+1-maxpeople[lastgeneration],indiv2+1-maxpeople[lastgeneration]);
			//	printrelative(indiv1,indiv2,ptrelative,maxpeople,lastgeneration);
				saverelative(path,indiv1,indiv2,ptrelative->numrelative,lastgeneration - ptrelative->highestgen,maxpeople);
			};
		};
		
	};
	return (0);
} 