/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIo.h"
#include "basicDefs.h"
#include <stdio.h>
#include <string.h>

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/


FILE *FopenRead(char *fileName)
{
   FILE *aFile;  
   aFile = fopen(fileName, "r");  
   return(aFile);  
}


FILE *FopenWrite(char *fileName)
{
   FILE *aFile;  
   if ((fileName != NULL) && (strlen(fileName) > 0))
      aFile = fopen(fileName, "w");  
   else
      aFile = stdout;  
   
   return(aFile);  
}


void Fheader(FILE *aFile)
{
   fprintf(aFile, "################################################\n");  
   fprintf(aFile, "#                                              #\n");  
   fprintf(aFile, "#    Data file for random polygon generator    #\n");  
   fprintf(aFile, "#                                              #\n");  
   fprintf(aFile, "################################################\n");  
}

void FreadLine(FILE *aFile, char *aLine, int lineSize)
{
   char *s;  
   do {
      s = fgets(aLine, lineSize, aFile);  
   } while ((*aLine == '#')  && (s != (char*)NULL));  
}


int Fclose(FILE *aFile)
{
   if (aFile != stdout)
      return(fclose(aFile));  
   else
      return(TRUE);  
}

