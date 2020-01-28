/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MstarPoly.h"
#include "sweepArrange.h"
#include "starPoly.h"
#include "convexHull.h"
#include "starKernel.h" 
#include "pointIncArray.h"
#include "polyIncArray.h"

/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/
#ifdef DO_STATS
unsigned long fastCount;  
#endif


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
#ifdef DO_STATS
void MRfastStarPoly()
{
  fastCount = 0;  
}


void MGfastStarPoly(unsigned long *tries)
{
  *tries = fastCount;  
}
#endif



void MstarPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
	       FILE *outFile)
{
  int nrOfPoints, kernelIndex, count;  
  t_arrange anArrange;  
  t_pointArray kernelPoints;  
  t_starPoly starPoly;  
  

  if ((nrOfPoints = PAnrOfPoints(pArray)) >= 3)
  {
    /* create the arrangement */
    SAcreate(&anArrange, pArray);  

    /* get all kernels from the arrangement */
    SAkernels(&kernelPoints, &anArrange, pArray);  

    /* free the arrangement */
    SAfree(&anArrange);  

    for (count=1;  count<=nrOfPolys;  count++)
      {
	/* choose one kernel */
	kernelIndex = randomInt(1, PAnrOfPoints(&kernelPoints));  
	
	/* create polygon around kernel */
	SPinit(&starPoly, PAnrOfPoints(pArray));  
	SPcreatePoly(&starPoly, PAgetPoint(&kernelPoints, kernelIndex), pArray);    
	/* copy polygon to output polygon */   
	BPfree(aPolygon);  
	BPinit(aPolygon, SPsizeOf(&starPoly));  
	BPcopyPoly(SPgetPoly(&starPoly), aPolygon);  
	SPfree(&starPoly);  

	BPnormalize(aPolygon, pArray);  
	if (outFile != NULL)
	  BPwriteToFile(aPolygon, outFile);  
      }

    /* free array of kernel points & star-shaped polygon */
    PAfreeArray(&kernelPoints);  

  }
}


void MfastStarPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
                   FILE *outFile)
{
   int nrOfPoints, count;  
   t_convexHull cHull;  
   t_point rPoint;  
   t_starPoly starPoly;  
   
   if ((nrOfPoints = PAnrOfPoints(pArray)) >= 3) {
      /* create the convex hull */
      CHcalcHull(&cHull, pArray);  
      
      for (count = 1;   count <= nrOfPolys;   count++) {
         /* select a random point within CH */
         do {
            rPoint = randomPoint();  
#ifdef DO_STATS
            fastCount++;  
#endif
         } while (!CHisInHull(&cHull, &rPoint, pArray));  
      
         /* create polygon around kernel */
         SPinit(&starPoly, PAnrOfPoints(pArray));  
         SPcreatePoly(&starPoly, rPoint, pArray);    
		 
         /* copy polygon to output polygon */   
         BPfree(aPolygon);  
         BPinit(aPolygon, SPsizeOf(&starPoly));  
         BPcopyPoly(SPgetPoly(&starPoly), aPolygon);  
         SPfree(&starPoly);  
         
         if (outFile != NULL)
            BPwriteToFile(aPolygon, outFile);  
      }
      /* free star-shaped polygon, convex hull */
      CHfreeHull(&cHull);  
   }
}



void MnewStarPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		  FILE *outFile)
{
  t_line *minLines, *maxLines;  
  t_yIncArray kernelPolys;  
  t_polygon *curPoly;  
  int polyIndex, count;  
  
  if (PAnrOfPoints(pArray) >= 3)
    {

      /* first, create all the lines */
      minLines = SKinitLines(pArray);  
      maxLines = SKinitLines(pArray);  

      SKcreateLines(pArray, minLines, maxLines);  
  
      /* generate all the kernels */
      SKcreatePolys(minLines, maxLines, &kernelPolys, pArray);  

      for (count=1;  count<=nrOfPolys;  count++)
	{
	  /* choose one polygon */
	  polyIndex = randomInt(1, YInrOfPolys(&kernelPolys));  

	  /* get the polygon */
	  curPoly = YIgetPoly(&kernelPolys, polyIndex);  
	 
	  /* copy polygon to output polygon */   
	  BPfree(aPolygon);  
	  BPinit(aPolygon, BPsizeOf(curPoly));  
	  BPcopyPoly(curPoly, aPolygon);  

	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);   
	}

      /* free kernel points, the star polygon */
      YIfreeArray(&kernelPolys);  

      /* free the lines */
      SKfreeLines(minLines);  
      SKfreeLines(maxLines);  
    }
}












