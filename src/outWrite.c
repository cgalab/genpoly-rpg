/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "outWrite.h"
#include "argEval.h"
#include "ipeWrite.h"
#include "polyOps.h"
#include "pointOps.h"
#include <stdio.h>
#include <string.h>

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


void OWipe(char *outFile)
{
  FILE *ipeFile;  
  
  ipeFile = IWopenFile(outFile);  
  
  POipeWrite(ipeFile);  
  YOipeWrite(ipeFile);  
  
  IWcloseFile(ipeFile);  
}

void OWwrite(char *outFile, int format)
{
   if ((strlen(outFile) > 0)  &&  (strcasecmp(outFile, "none") != 0)) {
      if (format == IPE)        OWipe(outFile);  
      else if (format == LINE)  YOwriteLine(outFile);  
   }
}






