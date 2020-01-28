/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicGeom.h"
#include "MxmonoPoly.h"
#include "basicPolyLine.h"
#include "basicPoly.h"
#include "uLongArray.h"
#include "pathTree.h"
#include "error.h"
#include <limits.h>

/********************************************************************/
/*                                                                  */
/* Implementation of the algorithm given by Zhu, Sundaram, Snoeyink, */
/*   and Mitchell, "Generating Random Polygons with Given Vertices" */
/*                                                                  */
/********************************************************************/
/* undefine the following if we want to generate number larger than 
   the generator's maximum */
#define CHECK_MAXRAND
#undef CHECK_MAXRAND

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
unsigned long checkNadd(unsigned long num1, unsigned long num2)
{
  unsigned long result = 0;  

#ifdef CHECK_MAXRAND
  if (((RANDOM_MAX - num2) > num1) && ((ULONG_MAX - num2) > num1))
#else
  if ((ULONG_MAX - num2) > num1)
#endif
    result = num1 + num2;  
  else
    ERerror(ERR_INTOVER);  

  return(result);  
}

void generateBottom(int startIndex, unsigned long randomValue, 
		    t_polyLine *topChain, t_polyLine *bottomChain, 
		    t_uLongArray *top, t_uLongArray *bottom, 
		    t_pointArray *pArray);  

void generateTop(int startIndex, unsigned long randomValue, 
		 t_polyLine *topChain, t_polyLine *bottomChain, 
		 t_uLongArray *top, t_uLongArray *bottom, 
		 t_pointArray *pArray)
{
  unsigned long partSum;  
  int topIndex, curIndex;  
  int count;  

  if (startIndex > 2)
  {
    partSum = 0;  
    topIndex = startIndex-1;  
    curIndex = startIndex-1;  

    do {
      curIndex--;  
      if (PAisOnLine(pArray, topIndex, startIndex, curIndex) == -1)
	{
	  topIndex = curIndex;  
        partSum += ULgetULong(bottom, curIndex+1);  
      } 
    }
    while (randomValue > partSum);  
    BLaddPIndex(bottomChain, curIndex);  
    for (count=startIndex-2;  count>curIndex;  count--)
      BLaddPIndex(topChain, count);  
    randomValue -= (partSum-ULgetULong(bottom, curIndex+1));  
    generateBottom(curIndex+1, randomValue, topChain, bottomChain, top, 
                   bottom, pArray);  

  }
}

void generateBottom(int startIndex, unsigned long randomValue, 
		    t_polyLine *topChain, t_polyLine *bottomChain, 
		    t_uLongArray *top, t_uLongArray *bottom, 
		    t_pointArray *pArray)
{
  unsigned long partSum;  
  int bottomIndex, curIndex;  
  int count;  

  if (startIndex > 2)
  {
    partSum = 0;  
    bottomIndex = startIndex-1;  
    curIndex = startIndex-1;  

    do {
      curIndex--;  
      if (PAisOnLine(pArray, bottomIndex, startIndex, curIndex) == 1)
      {
        bottomIndex = curIndex;  
        partSum += ULgetULong(top, curIndex+1);  
      } 
    }
    while (randomValue > partSum);  
    BLaddPIndex(topChain, curIndex);  
    for (count=startIndex-2;  count>curIndex;  count--)
      BLaddPIndex(bottomChain, count);  
    randomValue -= (partSum-ULgetULong(top, curIndex+1));  
    generateTop(curIndex+1, randomValue, topChain, bottomChain, top, 
                   bottom, pArray);  

  }
}

void MxmonoPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		FILE *outFile)
{
  t_uLongArray top, bottom;  /* TN and BN */
  t_pathTree topTree, bottomTree;  
  t_polyLine topChain, bottomChain;  
  
  int nrOfPoints;  
  unsigned long nrOfPolygons;  
  int count, curIndex;  
  unsigned long curTop, curBottom;  
  unsigned long randomValue;  
 
  if ((nrOfPoints = PAnrOfPoints(pArray)) >= 3)
    {
      /* first we have to count the polygons */

      /* create the arrays for BN and TN */
      ULinit(&top, nrOfPoints);  
      ULinit(&bottom, nrOfPoints);  

      /* init the second element of TN and BN */ 
      ULsetULong(&top, 2, 1);  
      ULsetULong(&bottom, 2, 1);  

      /* init the top and the bottom visibility trees */
      PTinitTree(&topTree);  
      PTinitTree(&bottomTree);  

      PTmakeIncTop(&topTree, pArray, 2);  
      PTmakeIncBottom(&bottomTree, pArray, 2);  
   
      for (count=3;  count<=nrOfPoints;  count++)
	{
	  curBottom = 0;  
    
	  PTmakeIncTop(&topTree, pArray, count);  
	  while ((curIndex = PTgetNextIndex(&topTree)) != -1)
	    {
	      if (curIndex != (count-1))
		curBottom = checkNadd(curBottom, ULgetULong(&top, curIndex+1));  
	    }
	  ULsetULong(&bottom, count, curBottom);  
	  
	  curTop = 0;  

	  PTmakeIncBottom(&bottomTree, pArray, count);  
	  while ((curIndex = PTgetNextIndex(&bottomTree)) != -1)
	    {
	      if (curIndex != (count-1))
		curTop = checkNadd(curTop, ULgetULong(&bottom, curIndex+1));  
	    }
	  ULsetULong(&top, count, curTop);  
     	}

      PTfreeTree(&topTree);  
      PTfreeTree(&bottomTree);  

      nrOfPolygons = ULgetULong(&top, nrOfPoints) +
	ULgetULong(&bottom, nrOfPoints);  

      for (count=1;  count<=nrOfPolys;  count++)
	{
	  /* Now that we have the number of xmonotone polygons, we may 
	     select one */
	  randomValue = randomULong(1, nrOfPolygons);  

	  BLinit(&topChain, nrOfPoints);  
	  BLinit(&bottomChain, nrOfPoints);  

	  BLaddPIndex(&topChain, nrOfPoints);  
	  BLaddPIndex(&bottomChain, nrOfPoints);  

	  if (randomValue <= ULgetULong(&top, nrOfPoints))
	    {
	      BLaddPIndex(&topChain, nrOfPoints-1);  
	      generateTop(nrOfPoints, randomValue, &topChain, &bottomChain, 
			  &top, &bottom, pArray);  
	    }
	  else
	    {
	      randomValue -= ULgetULong(&top, nrOfPoints);  
	      BLaddPIndex(&bottomChain, nrOfPoints-1);  
	      generateBottom(nrOfPoints, randomValue, &topChain, &bottomChain, 
			     &top, &bottom, pArray);  
	    }

	  if (BLgetPIndex(&topChain, BLsizeOf(&topChain)) != 1)
	    BLaddPIndex(&topChain, 1);  
	  if (BLgetPIndex(&bottomChain, BLsizeOf(&bottomChain)) != 1)
	    BLaddPIndex(&bottomChain, 1);  

	  curIndex = 1;  

	  BPfree(aPolygon);  
	  BPinit(aPolygon, nrOfPoints);  

	  /* read bottom chain first, because we want to have 
	     the polygon in CCW order */
	  for (count=BLsizeOf(&bottomChain);  count>0;  count--)
	    {
	      BPsetPIndex(aPolygon, curIndex, BLgetPIndex(&bottomChain, count));  
	      curIndex++;  
	    }
	  for (count=2;  count<BLsizeOf(&topChain);  count++)
	    {
	      BPsetPIndex(aPolygon, curIndex, BLgetPIndex(&topChain, count));  
	      curIndex++;  
	    }
    
	  BLfree(&topChain);  
	  BLfree(&bottomChain);     

	  BPnormalize(aPolygon, pArray);  
	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	}
      ULfree(&top);  
      ULfree(&bottom);  
    }
}
