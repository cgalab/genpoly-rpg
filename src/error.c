/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "error.h"
#include <stdio.h>
#include <stdlib.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void ERerror(int errNumber)
{
   switch(errNumber)  {
   case ERR_MEM:
      fprintf(stderr, "ERROR ALLOCATING MEMORY\n");  
      break;  
   case ERR_ARGS:    
      fprintf(stderr, "INVALID OR INCOMPLETE COMMAND LINE OPTION(s)\n\n");  
      fprintf(stderr, "Valid options are:\n\n");  
      fprintf(stderr, 
              " --random <number>:  generates <number> random points within");  
      fprintf(stderr, " the unit square.\n\n");  
      fprintf(stderr, 
              " --cluster <number>: generates <number> clustered points within");  
      fprintf(stderr, " the unit circle.\n\n");  
      fprintf(stderr, 
              " --circle <number>:  generates <number> random points within");  
      fprintf(stderr, " the unit circle.\n\n");  
      fprintf(stderr, 
              " --seed <number>:    initializes the random number generator");  
      fprintf(stderr, " with <number>\n\n");  
      fprintf(stderr, 
              " --algo <algo>:      specifies the algorithm used to ");  
      fprintf(stderr, "generate the polygon.\n");  
      fprintf(stderr, 
              "                     Valid parameters are:\n");  
      fprintf(stderr, 
              //              "                         \"xmono\", \"arrange\", \"star\", ");  
              "                         \"xmono\", \"star\", ");  
      //      fprintf(stderr, "\"star\", \"growth\", \n");  
      fprintf(stderr, "\"growth\", \"2opt\", \"space\", \n");  
      fprintf(stderr, 
              "                         \"triangle\".\n\n");  
      fprintf(stderr, 
              " --smooth <number>:  calls the smoothing routine");  
      fprintf(stderr, " <number> times.\n\n");   
      fprintf(stderr, 
              " --holes <number>:   generate <number> holes.\n\n");  
      fprintf(stderr, 
              " --format <format>:  specifies the format of the output ");  
      fprintf(stderr, "file. Valid are:\n");  
      fprintf(stderr, 
              "                         \"ipe\" and ");  
      fprintf(stderr, "\"line\".\n\n");  
      fprintf(stderr, 
              " --output <file>:    directs the output to file <file>.\n\n");  
      fprintf(stderr, 
              " --input <file>:     the input is read from <file>.\n\n");  
      /*
      fprintf(stderr, 
              " --analysis <angle> <slope> <length>:\n");  
      fprintf(stderr, 
              "                     analyzes the generated polygons.");  
      fprintf(stderr, 
              " The parameters\n");  
      fprintf(stderr, 
              "                     specify the number of classes.\n\n");  
      */
      fprintf(stderr, 
              " --count <number>:   specifies the number of polygons to ");  
      fprintf(stderr, "generate.\n\n");  
      break;  
   case ERR_INTOVER:
      fprintf(stderr, "OVERFLOW WHEN COUNTING POLYGONS\n");  
      break;  
   case ERR_LINE:
      fprintf(stderr, "LINE WHERE POINTS ARE IDENTICAL\n");  
      break;  
   default:
      break;  
   }
   
   exit(1);  
}












