/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MrejectPoly.h"
#include "basicIntArray.h"

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

#ifdef DO_STATS
unsigned long rejectCount;  
#endif


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
#ifdef DO_STATS
void MRrejectPoly()
{
  rejectCount = 0;  
}


void MGrejectPoly(unsigned long *tries)
{
  *tries = rejectCount;  
}
#endif

void MrejectPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		 FILE *outFile)
{
  int count;  

  if (PAnrOfPoints(pArray) >= 3)
    {
      for (count=1;  count<=nrOfPolys;  count++)
	{
	  BPfree(aPolygon);  
	  BPinit(aPolygon, PAnrOfPoints(pArray));  
	  do {
	    /* choose a random polygon */
	    BPrandomPerm(aPolygon);  
#ifdef DO_STATS
	    rejectCount++;  
#endif
	    /* now we have a polygon, check whether it is simple! */	
	  }while (!BPisSimple(aPolygon, pArray));  
	  
	  BPnormalize(aPolygon, pArray);  

	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	}
    }
}






