/***************************************************************************************************************************************************
*
*                                 This program outputs the common ancestor for any two people 
*(The speed of this program can be greatly decreased by writing on file a big bunch of data from one command instead of how it is done here)
*                                 
****************************************************************************************************************************************************/
#include <time.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "readinteger.h"

#define MAXINT INT32_MAX
#define MAXPOP 1000000
#define MAXGEN 1000
#define INT int32_t
#define MAXDEGREE 10
#define pow(two,MAXDEGREE) 1024


typedef struct  
{	INT mum;
	INT dad;
} structsomeone;

struct structrelative
{	//INT listrelative[pow(two,MAXDEGREE)];
	INT numrelative[MAXPOP];
	INT highestgen[MAXPOP];
};

INT getmaxpeople(char path[], INT generation);
INT readinteger(FILE * file);
INT readfile(char path[],INT generation,structsomeone people[],INT maxpeople[]);
INT addtorelative(INT ancestor,INT indiv2,struct structrelative *ptrelative,INT gen,INT maxpeople[]);
INT compare(INT indiv1,INT indiv2,INT indiv2origin,INT mingeneration,INT currentgeneration,struct structrelative *ptrelative,INT maxpeople[]);
INT comparegenaway(INT indiv1,INT indiv2,INT indiv2origin,INT mingeneration,INT currentgeneration,struct structrelative *ptrelative,INT maxpeople[]);
INT getgeneration(INT ID,INT maxpeople[]);
INT printrelative(INT indiv1,INT indiv2,struct structrelative *ptrelative,INT maxpeople[],INT lastgeneration);
INT initsave(char path[],INT numgen);
INT saveallrelative(char path[],INT indiv1,INT lastgeneration,struct structrelative *ptrelative,INT maxpeople[],INT numgen,INT numgenpeople);



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
INT addtorelative(INT ancestor,INT indiv2,struct structrelative *ptrelative,INT gen,INT maxpeople[])
{	//ptrelative->listrelative[ptrelative->numrelative[indiv2]]=ancestor;
	ptrelative->numrelative[indiv2]++;
	if (ptrelative->highestgen[indiv2]<gen) ptrelative->highestgen[indiv2]=gen;
	/*printf("ID %d is a relative at generation %d, there are %d ancestor with individual %d, highest degree %d\n",
			ancestor+1-maxpeople[gen],
			gen,
			ptrelative->numrelative[indiv2],
			indiv2,
			ptrelative->highestgen[indiv2]); */
}
//
//  store each individual, the ID of their mum then the ID of their dad
//		
structsomeone people[MAXPOP];
//
//  Search for relative of indiv1 and indiv2
//		
INT compare(INT indiv1,INT indiv2,INT indiv2origin,INT mingeneration,INT currentgeneration,struct structrelative *ptrelative,INT maxpeople[])
{	//printf(" %p \n", ptrelative);
	if (people[indiv1].mum==people[indiv2].mum)
	{	addtorelative(people[indiv1].mum,indiv2origin,ptrelative,currentgeneration,maxpeople);
		if (people[indiv1].dad==people[indiv2].dad)
		{	addtorelative(people[indiv1].dad,indiv2origin,ptrelative,currentgeneration,maxpeople);
			if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].mum,people[indiv1].dad,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		} else 
		{ 	if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].dad,people[indiv2].dad,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].dad,people[indiv2].mum,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].dad,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		};
	} else
	{ 	if (people[indiv1].dad==people[indiv2].dad)
		{	addtorelative(people[indiv1].dad,indiv2origin,ptrelative,currentgeneration,maxpeople);
			if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].mum,people[indiv2].mum,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].dad,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].dad,people[indiv2].mum,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		} else 
		{ 	if (mingeneration<currentgeneration) 
			{	compare(people[indiv1].dad,people[indiv2].dad,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].dad,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].dad,people[indiv2].mum,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
				compare(people[indiv1].mum,people[indiv2].mum,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			};
		};
	};
	return (0);
}
//
//  Search for relative of indiv1 and indiv2 being with one generation away
//	
INT comparegenaway(INT indiv1,INT indiv2,INT indiv2origin,INT mingeneration,INT currentgeneration,struct structrelative *ptrelative,INT maxpeople[])
{	//printf(" %p \n", ptrelative);
	if (people[indiv1].mum==indiv2)
	{	addtorelative(indiv2,indiv2origin,ptrelative,currentgeneration,maxpeople);
		compare(people[indiv1].dad,indiv2,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
	} else 
	{	if (people[indiv1].dad==indiv2)
		{	addtorelative(indiv2,indiv2origin,ptrelative,currentgeneration,maxpeople);
			compare(people[indiv1].mum,indiv2,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
		} else 
		{	compare(people[indiv1].dad,indiv2,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);
			compare(people[indiv1].mum,indiv2,indiv2origin,mingeneration,currentgeneration-1,ptrelative,maxpeople);	
		};
	};
	return (0);
};
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
	printf("There is/are %d relatives\n",ptrelative->numrelative[indiv2] );
	if (ptrelative->numrelative[indiv2]>0) for(relat=0;relat<ptrelative->numrelative[indiv2];relat++)
	{	//INT gen=getgeneration(ptrelative->listrelative[relat],maxpeople);
		//INT ID=ptrelative->listrelative[relat]+1-maxpeople[gen]; 
		//printf("ID %d is a relative at generation %d\n",ID,gen);
	};
	if (ptrelative->numrelative[indiv2]>0) printf("The most recent relative is at a degree of %d\n",lastgeneration - ptrelative->highestgen[indiv2]);
	return (0);
}
//
//  Create the file relative.txt
//		
INT initsave(char path[],INT numgen)
{	FILE * file;
	char filename[100]; 
	strcpy (filename,path);
	strcat(filename,".relatives");
	char number[100];
    sprintf(number, "%d", numgen);
	strcat(filename,number);
	strcat(filename,".txt");
    if ((file = fopen(filename, "w+")) == NULL) return 1; 
    fprintf(file,"ID1.generation ID2.generation #of_relatives #closest_degree\n");
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
    fprintf(file,"%d %d %d %d %d\n",indiv1+1-maxpeople[getgeneration(indiv1,maxpeople)],
				indiv2+1-maxpeople[getgeneration(indiv2,maxpeople)],
				nbrelative,
				lowestdegree);
	fclose (file);
}

INT saveallrelative(char path[],INT indiv1,INT lastgeneration,struct structrelative *ptrelative,INT maxpeople[],INT numgen,INT numgenpeople)
{	FILE * file;
	char filename[100]; 
	strcpy (filename,path);
	strcat(filename,".relatives");
	char number[100];
    sprintf(number, "%d", numgen);
	strcat(filename,number);
	strcat(filename,".txt");
    if ((file = fopen(filename, "a")) == NULL) return 1;
    INT indiv2;
	for(indiv2=1+indiv1;indiv2<maxpeople[lastgeneration+1] ;indiv2++) 
	{	if (ptrelative->numrelative[indiv2]) fprintf(file,"%d.%d %d.%d %d %d\n",
				indiv1+1-maxpeople[getgeneration(indiv1,maxpeople)],
				getgeneration(indiv1,maxpeople),
				indiv2+1-maxpeople[getgeneration(indiv2,maxpeople)],
				getgeneration(indiv2,maxpeople),
				ptrelative->numrelative[indiv2],
				lastgeneration - ptrelative->highestgen[indiv2]);
	}
	if (numgenpeople>1)
	{	for(indiv2=maxpeople[lastgeneration-1];indiv2<maxpeople[lastgeneration] ;indiv2++) 
		{	if (ptrelative->numrelative[indiv2]) fprintf(file,"%d.%d %d.%d %d %d\n",
				indiv1+1-maxpeople[getgeneration(indiv1,maxpeople)],
				getgeneration(indiv1,maxpeople),
				indiv2+1-maxpeople[getgeneration(indiv2,maxpeople)],
				getgeneration(indiv2,maxpeople),
				ptrelative->numrelative[indiv2],
				lastgeneration - ptrelative->highestgen[indiv2]);
		};	
	};
	fclose (file);
}
//
//  main program
//	
int main(int argc, char *argv[])
{	 clock_t begin = clock();
	printf("Start...\n");
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
	{   printf("The path of the run is missing. Default is /rc_scratch/emsa1620/GeneEvolve/Output_files300/.\n");
	} else 
	{	strcpy(path,argv[6]);
	};
	printf("The path is %s\n",path);
	INT numgen;
	if (argc<9)
	{   printf("All pairs will be considered from $generation$ to $generation-numgen+1$. Default is $numgen=1$.\n");
		numgen=1;
	} else 
	{	numgen = atoi (argv[8]);
	};
	
	
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
	INT degree=maxdegree+numgen-1;
	generation=lastgeneration;
	while (degree>0)
	{	if (generation>0) readfile(path,generation,people,maxpeople);
		degree=(degree>1)?degree-2:0;
		generation=(generation>1)?generation-2:0;
	};
	if (numgen>1) readfile(path,lastgeneration-1,people,maxpeople);
	//
	// initsave creat the file relative.txt
	//
	initsave(path,numgen);
	//
	// the following loop fill the file relative.txt by considering all pair of individual in the final generation
	//
	INT indiv1;
	for(indiv1=maxpeople[lastgeneration];indiv1<maxpeople[lastgeneration+1];indiv1++)
	{	printf("Compare ID %d.%d and the rest of the world \n",indiv1+1-maxpeople[lastgeneration],lastgeneration );
		INT indiv2;
		struct structrelative relative;
		for(indiv2=0;indiv2<MAXPOP;indiv2++) 
		{	relative.numrelative[indiv2]=0;
			relative.highestgen[indiv2]=0;
		};
		struct structrelative *ptrelative=&relative;
		for(indiv2=1+indiv1;indiv2<maxpeople[lastgeneration+1] ;indiv2++) 
		{	//	printf("and ID %d:\n ",indiv2+1-maxpeople[lastgeneration]);
			compare(indiv1,indiv2,indiv2, lastgeneration-maxdegree,lastgeneration-1,ptrelative,maxpeople);
			/*if (ptrelative->numrelative)
			{//	printf("Print relative between ID %d and %d \n",indiv1+1-maxpeople[lastgeneration],indiv2+1-maxpeople[lastgeneration]);
			//	printrelative(indiv1,indiv2,ptrelative,maxpeople,lastgeneration);
			//	saverelative(path,indiv1,indiv2,ptrelative->numrelative,lastgeneration - ptrelative->highestgen,maxpeople);
			};*/
		};
		if (numgen>1)
		{	for(indiv2=maxpeople[lastgeneration-1];indiv2<maxpeople[lastgeneration] ;indiv2++) 
			{	comparegenaway(indiv1,indiv2,indiv2, lastgeneration-maxdegree,lastgeneration-1,ptrelative,maxpeople);
			};				
		};
		saveallrelative(path,indiv1,lastgeneration,ptrelative,maxpeople,numgen,numgen);
	};
	if (numgen>1)
	{	lastgeneration--;
		for(indiv1=maxpeople[lastgeneration];indiv1<maxpeople[lastgeneration+1];indiv1++)
		{	printf("Compare ID %d.%d and the rest of the world \n",indiv1+1-maxpeople[lastgeneration],lastgeneration );
			INT indiv2;
			struct structrelative relative;
			for(indiv2=0;indiv2<MAXPOP;indiv2++) 
			{	relative.numrelative[indiv2]=0;
				relative.highestgen[indiv2]=0;
			};
			struct structrelative *ptrelative=&relative;
			for(indiv2=1+indiv1;indiv2<maxpeople[lastgeneration+1] ;indiv2++) 
			{	//	printf("and ID %d:\n ",indiv2+1-maxpeople[lastgeneration]);
				compare(indiv1,indiv2,indiv2,lastgeneration-maxdegree,lastgeneration-1,ptrelative,maxpeople);
				/*if (ptrelative->numrelative)
				{	printf("Print relative between ID %d and %d \n",indiv1+1-maxpeople[lastgeneration],indiv2+1-maxpeople[lastgeneration]);
					printrelative(indiv1,indiv2,ptrelative,maxpeople,lastgeneration);
				//	saverelative(path,indiv1,indiv2,ptrelative->numrelative,lastgeneration - ptrelative->highestgen,maxpeople);
				};*/
			};
			saveallrelative(path,indiv1,lastgeneration,ptrelative,maxpeople,numgen,0);		
		};
	};
			
	clock_t end = clock();
	float elapsed_secs = (float)(end - begin) ;
    printf("\nTotal time:%f\n cpu click so %f seconds",elapsed_secs,elapsed_secs/CLOCKS_PER_SEC );
	return (0);
}
