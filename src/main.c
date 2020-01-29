/*****************************************************************************/
/*                                                                           */
/*                      Random Polygon Generator (RPG)                       */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* E-Mail:      held@cs.sbg.ac.at                                            */
/* Snail Mail:  Martin Held                                                  */
/*              Universitaet Salzburg                                        */
/*              FB Computerwissenschaften                                    */
/*              Computational Geometry and Applications Lab                  */
/*              A-5020 Salzburg, Austria                                     */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* This program is free software; you may redistribute it and/or modify it   */
/* under the terms of the GNU General Public License as published by the     */
/* Free Software Foundation; either version 3 of the License, or (at your    */
/* option) any later version.                                                */
/*                                                                           */
/* Please note that this program includes an old version of qhull for        */
/* computing convex hulls. The source code for qhull is provided only for    */
/* convenience purposes but is not covered by the license regulations for    */
/* the rest of RPG. Please see http://www.qhull.org/ for license regulations */
/* and for the most recent version of the qhull code.                        */
/*                                                                           */
/*****************************************************************************/

/* get standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#include "pointOps.h"
#include "polyOps.h"
#include "eralloc.h"
#include "basicInits.h"
#include "argEval.h"
#include "calcPoly.h"
#include "outWrite.h"
#include "analysisII.h"


void PrintHeader(void)
{
   printf("\n");  
   printf("**************************************************************************\n");  
   printf("*                                                                        *\n");  
   printf("*                     RandomPolygonGenerator: RPG 3.0                    *\n");  
   printf("*                                                                        *\n");  
   printf("*   T. Auer, M. Gschwandtner, M. Heimlich, M. Held       (C) 1994-2020   *\n");  
   printf("*                                                                        *\n");  
   printf("* Univ. Salzburg, FB Computerwissenschaften,                             *\n");  
   printf("* Computational Geometry and Applications Lab                            *\n");  
   printf("* A-5020 Salzburg, Austria                            held@cs.sbg.ac.at  *\n");  
   printf("*                                                                        *\n");  
   printf("**************************************************************************\n");  

   return;  
}


double get_current_rtime(void) {
  struct rusage usage;
  if (getrusage(RUSAGE_SELF, &usage) < 0) {
    fprintf(stderr, "getrusage() failed: %s\n", strerror(errno));
    exit(1);
  }
  return usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec/1e6;
}
long get_maxrss(void) {
  struct rusage usage;
  if (getrusage(RUSAGE_SELF, &usage) < 0) {
    fprintf(stderr, "getrusage() failed: %s\n", strerror(errno));
    exit(1);
  }
  return usage.ru_maxrss;
}


int main(int argc, char *argv[])
{
   int format, nrOfPoints, nrOfPolys, seed, smooth;  
   int analysis, sinuosity, lenRes, angleRes, slopeRes, count, cluster;  
   int status_fd;
   int auxParam, nholes;  
   t_pointArray pArray;  
   enum t_calcType algo;  
   char inFile[255], outFile[255], lenFile[255], angleFile[255];
   char slopeFile[255], sinuFile[255], linkFile[255], outFName[255];  
   
   PrintHeader();  

   /*                                                                        */
   /* basic initializations                                                  */
   /*                                                                        */
   BIinit();  
   erinit();  
   
   /*                                                                        */
   /* parse command-line arguments                                           */
   /*                                                                        */
   if (!AEeval(argc, argv, &nrOfPoints, &nrOfPolys, &format, &seed, &smooth, 
               &algo, inFile, outFName, &analysis, &sinuosity, &angleRes, 
               &lenRes, &slopeRes, &auxParam, &cluster, &nholes, &status_fd)) {
      exit(1);  
   }

   /*                                                                        */
   /* initialize the structure for the points                                */
   /*                                                                        */
   POinit();  
   YOinit();  
   
   /*                                                                        */
   /* read input file it it was specified                                    */
   /*                                                                        */
   if (strlen(inFile) > 0)
      POloadPoints(inFile);  
   
   /*                                                                        */
   /* initialize random-number generator                                     */
   /*                                                                        */
   if (seed >= 0)
      srand48(seed);  
   else
      srand48(1);  
   
   /*                                                                        */
   /* generate the specified number of random points                         */
   /*                                                                        */
   if (cluster > 0)
      POgenerateClusterPoints(nrOfPoints);  
   else if (nrOfPoints > 0)
      POgenerateRandomPoints(nrOfPoints);  
   else if (nrOfPoints < 0) {
      nrOfPoints = -nrOfPoints;  
      POgenerateRandomCirclePoints(nrOfPoints);  
   }
   
   /*                                                                        */
   /* initialize the analysis process                                        */
   /*                                                                        */
   /* this is here for legacy reasons, but it currently is not functional!   */
   /*                                                                        */
   if (analysis)  {
      resetAnalysisII(lenRes, slopeRes, angleRes, nrOfPoints, nrOfPolys);  
      /* resetLink(nrOfPoints, nrOfPolys);  */
   }
   else if (sinuosity) {
      resetSinuosityII(nrOfPoints, nrOfPolys);  
   }

   /*                                                                        */
   /* generate the polygon(s)                                                */
   /*                                                                        */
   PAlistInitArray(&pArray, POgetPointList());  
   double rtime_started = get_current_rtime();
   for (count = 1;   count <= nrOfPolys;   count++) {
      /*                                                                     */
      /* restore the initial point set                                       */
      /*                                                                     */
      PAcreateList(&pArray, POgetPointList());  
      
      CPsetState(algo, auxParam);  
      CPdoCalc();  
      
      /*                                                                     */
      /* smooth the polygon if requested                                     */
      /*                                                                     */
      while (smooth > 0) {
         CPsetState(CP_SMOOTH, RPG_UNDEFINED);  
         CPdoCalc();  
         smooth--;  
      }
      
      /*                                                                     */
      /* compute holes if requested                                          */
      /*                                                                     */
      if (nholes > 0) {
         CPsetState(CP_HOLES, nholes);  
         CPdoCalc();  
      }
      
      if ((!analysis) && (!sinuosity)) {
         /*                                                                  */
         /* write the polygon(s) to a file                                   */
         /*                                                                  */
         if (strlen(outFName) > 0) {
            if (nrOfPolys > 1) {
               if (format == IPE)        
                  sprintf(outFile, "%s-%03i.ipe", outFName, count);  
               else if (format == LINE)  
                  sprintf(outFile, "%s-%03i.line", outFName, count);  
            }
            else {
               if (format == IPE)        
                  sprintf(outFile, "%s.ipe",  outFName);  
               else if (format == LINE)  
                  sprintf(outFile, "%s.line",  outFName);  
            }
         }
         else
            outFile[0] = 0;  

         OWwrite(outFile, format);  
      }
      else if (analysis)
         YOanalysisII();  
      else if (sinuosity)
         YOsinuosityII();  
   }
   double rtime_ended = get_current_rtime();
   long rmem = get_maxrss();
   if (status_fd >= 0) {
      FILE *status = fdopen(status_fd, "a");
      if (!status) {
         fprintf(stderr, "Cannot open status FD %d: %s\n", status_fd, strerror(errno));
         exit(-1);
      }

      fprintf(status, "[STATUS] CPUTIME: %.6lf\n", rtime_ended - rtime_started);
      fprintf(status, "[STATUS] MAXRSS: %ld\n", rmem);
   }

   PAfreeArray(&pArray);  
   
   /*                                                                        */
   /* carry out an analysis of characteristics of the polygon generated      */
   /*                                                                        */
   /* this is here for legacy reasons, but currently is not functional!      */
   /*                                                                        */
   if (analysis) {
      if ((strlen(outFile) == 0) || (strcasecmp(outFile, "none") == 0))
         strcpy(outFile, "analysis");  
      
      strcpy(lenFile, outFile);  
      strcpy(angleFile, outFile);  
      strcpy(slopeFile, outFile);  
      strcpy(sinuFile, outFile);  
      strcpy(linkFile, outFile);  
      strcat(angleFile, ".ang");  
      strcat(slopeFile, ".slp");  
      strcat(lenFile, ".len");  
      strcat(sinuFile, ".sin");  
      strcat(linkFile, ".lnk");  
      
      writeAnalysis(lenFile, slopeFile, angleFile, sinuFile);  
      /* writeLink(linkFile);  */
   }
   else if (sinuosity) {
      if ((strlen(outFile) == 0) || (strcasecmp(outFile, "none") == 0))
         strcpy(outFile, "analysis");  
      
      strcpy(sinuFile, outFile);  
      strcat(sinuFile, ".sin");  
      
      writeSinuosityII(sinuFile);  
   }

   erend();  

   exit(0);  
}
