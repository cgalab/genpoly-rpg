/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "argEval.h"
#include "basicDefs.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
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

int AEeval(int argc, char *argv[], int *nrOfPoints, int *nrOfPolys, 
           int *format, int *seed, int *smooth, enum t_calcType *algo, 
           char *inFile, char *outFile, int *analysis, int *sinuosity, 
           int *angRes, int *lenRes, int *slopeRes, int *auxParam, 
           int *cluster, int *nholes)
{
   int count;  
   int success = TRUE;  
   
   /* basic initializations */
   (*nrOfPoints) = 0;  
   (*nrOfPolys) = 1;  
   outFile[0] = 0;  
   inFile[0] = 0;  
   (*seed) = 1;  
   (*smooth) = 0;  
   (*format) = LINE;  
   (*analysis) = FALSE;  
   (*sinuosity) = FALSE;  
   (*algo) = CP_2OPT_II;  
   (*auxParam) = RPG_UNDEFINED;  
   (*cluster) = 0;  
   (*nholes) = 0;  

   count = 1;  
   while ((count < argc) && success)    {
      
      /*
      if (strcasecmp(argv[count], "--sinuosity") == 0)	{
         (*sinuosity) = TRUE;  
      }
      else 
      */
      if (strcasecmp(argv[count], "--smooth") == 0)	{
         /* smooth the polygon n times */
         count++;  
         if ((success = count < argc)) {
            (*smooth) = atoi(argv[count]);  
            success = ((*smooth) > 0);  
         }
      }
      else if (strcasecmp(argv[count], "--count") == 0) {
         /* specify the number of polygons to generate */
         count++;  
         if ((success = count < argc)) {
            (*nrOfPolys) = atoi(argv[count]);  
            success = ((*nrOfPolys) > 0);  
         }
      }
      else if (strcasecmp(argv[count], "--random") == 0) {
         /* generate random points */
         count++;  
         if ((success = count < argc)) {
            (*nrOfPoints) = atoi(argv[count]);  
            success = ((*nrOfPoints) > 0);  
         }
      }
      else if (strcasecmp(argv[count], "--cluster") == 0) {
         /* generate clustered random points */
         count++;  
         if ((success = count < argc)) {
            (*nrOfPoints) = atoi(argv[count]);  
            (*cluster) = (*nrOfPoints);  
            success = ((*nrOfPoints) > 0);  
         }
      }
      else if (strcasecmp(argv[count], "--circle") == 0) {
         /* generate random points */
         count++;  
         if ((success = count < argc)) {
            (*nrOfPoints) = -atoi(argv[count]);  
            success = ((*nrOfPoints) < 0);  
         }
      }
      else if (strcasecmp(argv[count], "--holes") == 0) {
         /* generate multiply-connected area */
         count++;  
         if ((success = count < argc)) {
            (*nholes) = atoi(argv[count]);  
            success = ((*nholes) > 0);  
         }
      }
      else if (strcasecmp(argv[count], "--seed") == 0) {
         /* generate random points */
         count++;  
         if ((success = count < argc)) {
            (*seed) = atoi(argv[count]);  
            success = ((*seed) >= 0);  
         }
      }
      /*
      else if (strcasecmp(argv[count], "--analysis") == 0) {
         (*analysis) =  TRUE;  
         // specify the analysis method
         count++;  
         if ((success = count < argc))  {
            (*angRes) = atoi(argv[count]);  
            success = ((*angRes) >= 0);  
            count++;  
         }
         if ((success = success && count < argc))  {
            (*slopeRes) = atoi(argv[count]);  
            success = ((*slopeRes) >= 0);  
            count++;  
         }
         if ((success = success && count < argc))  {
            (*lenRes) = atoi(argv[count]);  
            success = ((*lenRes) >= 0);  
         }
      }
      */
      else if (strcasecmp(argv[count], "--algo") == 0) {
         /* specify the algorithm */
         count++;  
         if ((success = count < argc)) {
            /*
            if (strcasecmp(argv[count], "bounce") == 0) {
               (*algo) = CP_BOUNCE;  
               count++;  
               if ((success = count < argc)) {
                  (*auxParam) = atoi(argv[count]);  
                  success = ((*auxParam) > 0);  
               }
            } 
            else 
            */
            if (strcasecmp(argv[count], "triangle") == 0)	{
               (*algo) = CP_TRIANGLE;  
               /*
               count++;  
               if ((success = count < argc)) {
                  (*auxParam) = atoi(argv[count]);  
                  success = ((*auxParam) > 0);  
               }
               */
            } 
            else if (strcasecmp(argv[count], "xmono") == 0)
               (*algo) = CP_XMONO;  
            //            else if (strcasecmp(argv[count], "arrange") == 0)
            //   (*algo) = CP_ARRANGE_STAR;  
            else if (strcasecmp(argv[count], "star") == 0)
               (*algo) = CP_FAST_STAR;  
            //            else if (strcasecmp(argv[count], "star") == 0)
            //   (*algo) = CP_STAR;  
            else if (strcasecmp(argv[count], "growth") == 0)
               (*algo) = CP_GROWING_II;  
            else if (strcasecmp(argv[count], "2opt") == 0)
               (*algo) = CP_2OPT_II;  
            else if (strcasecmp(argv[count], "space") == 0)
               (*algo) = CP_SPACE_PART;  
            //else if (strcasecmp(argv[count], "reject") == 0)
            //   (*algo) = CP_REJECT;  
            //else if (strcasecmp(argv[count], "search") == 0)
            //   (*algo) = CP_SEARCH;  
            else
               success = FALSE;  
         }
      }
      else if (strcasecmp(argv[count], "--format") == 0) {
         /* specify the format of the output */
         count++;  
         if ((success = count < argc)) {
            if (strcasecmp(argv[count], "ipe") == 0)
               (*format) = IPE;  
            else if (strcasecmp(argv[count], "line") == 0)
               (*format) = LINE;  	      
         }
      }
      else if (strcasecmp(argv[count], "--output") == 0) {
         /* generate random points */
         count++;  
         if ((success = count < argc)) {
            strcpy(outFile, argv[count]);  
         }
      }
      else if (strcasecmp(argv[count], "--input") == 0) {
         /* generate random points */
         count++;  
         if ((success = count < argc)) {
            strcpy(inFile, argv[count]);  
         }
      }
      else {
         success = FALSE;  
      }
      
      count++;  
      
   }
   
   if (!success)
      ERerror(ERR_ARGS);  
   
   return(success);  
}
