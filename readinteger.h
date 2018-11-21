#include <inttypes.h>
#include <stdio.h>

#define INT int32_t
#define MAXINT INT32_MAX

//
//  read an positive integer from the file file
// return INT32_MAX if eof
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
