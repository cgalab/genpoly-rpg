/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "starPoly.h"
#include "sortPoly.h"
#include "basicLine.h"
#include <stdio.h>
#include <stdlib.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/


void SPinit(t_starPoly *aStarPoly, int nrOfPoints)
{
  BPinit(&(aStarPoly->polygon), nrOfPoints);  
  IAinit(&(aStarPoly->next), nrOfPoints);  
}

void SPcreatePoly(t_starPoly *aStarPoly, t_point centerPoint, 
                  t_pointArray *pArray)
{
   int count;  
   int lastPoint;  
   
   /* create the star-shaped polygon */
   sortPoints(pArray, &(aStarPoly->polygon), centerPoint);  
   
   /* create the next-array */  
   lastPoint = BPgetPIndex(&(aStarPoly->polygon), 1);  
   for (count = BPsizeOf(&(aStarPoly->polygon));   count > 0;   count--) {
      IAsetInt(&(aStarPoly->next), BPgetPIndex(&(aStarPoly->polygon), count), 
               lastPoint);  
      lastPoint = BPgetPIndex(&(aStarPoly->polygon), count);  
   }
}

int SPgetPIndex(t_starPoly *aStarPoly, int index)
{
  return(BPgetPIndex(&(aStarPoly->polygon), index));  
}

int SPgetNextPIndex(t_starPoly *aStarPoly, int index)
{
  return(IAgetInt(&(aStarPoly->next), index));  
}

void SPswapPoints(t_starPoly *aStarPoly, int index1, int index2)
{
  int auxPIndex, nIndex1, nIndex2;  
  int size;  
  
  auxPIndex = SPgetPIndex(aStarPoly, index1);  
  BPsetPIndex(&(aStarPoly->polygon), index1, SPgetPIndex(aStarPoly, index2));  
  BPsetPIndex(&(aStarPoly->polygon), index2, auxPIndex);  

  size = SPsizeOf(aStarPoly);  

  /* update the next information  for the points that are before 
     the points we swapped */
  nIndex1 = ((index1 - 2) % size) + 1;  
  nIndex2 = ((index2 - 2) % size) + 1;  
  IAsetInt(&(aStarPoly->next), SPgetPIndex(aStarPoly, nIndex1), 
	   SPgetPIndex(aStarPoly, index1));  
  IAsetInt(&(aStarPoly->next), SPgetPIndex(aStarPoly, nIndex1), 
	   SPgetPIndex(aStarPoly, index1));  

  /* update the next information for the points we swapped */
  nIndex1 = (index1 % size)+1;  
  nIndex2 = (index2 % size)+1;  
  IAsetInt(&(aStarPoly->next), SPgetPIndex(aStarPoly, index1), 
	   SPgetPIndex(aStarPoly, nIndex1));  
  IAsetInt(&(aStarPoly->next), SPgetPIndex(aStarPoly, index2), 
	   SPgetPIndex(aStarPoly, nIndex2));  
}

int SPsizeOf(t_starPoly *aStarPoly)
{
  return(BPsizeOf(&(aStarPoly->polygon)));  
}

void SPcopyPoly(t_starPoly *sourceStarPoly, t_starPoly *destStarPoly)
{
  BPcopyPoly(&(sourceStarPoly->polygon), &(destStarPoly->polygon));  
  IAcopyArray(&(sourceStarPoly->next), &(destStarPoly->next));  
}

int SPisPolyEdge(t_starPoly *aStarPoly, int pIndex1, int pIndex2)
{
  int res;  

  res = ((IAgetInt(&(aStarPoly->next), pIndex1) == pIndex2) ||
	 (IAgetInt(&(aStarPoly->next), pIndex2) == pIndex1));  

  return(res);  
}

t_polygon *SPgetPoly(t_starPoly *aStarPoly)
{
  return(&(aStarPoly->polygon));  
}


void SPfree(t_starPoly *aStarPoly)
{
  BPfree(&(aStarPoly->polygon));  
  IAfree(&(aStarPoly->next));  
} 


t_point SPpInKernel(t_starPoly *aStarPoly, t_pointArray *pArray)
{
  printf("THIS METHOD HAS NOT BEEN IMPLEMENTED!\n");  
  exit(1);  
}

/*
t_point SPpInKernel(t_starPoly *aStarPoly, t_pointArray *pArray)
{
  t_point p1, p2, v1, v2;  
  t_point check1, check2;  
  t_point left, right, isectPoint;  
  t_line symLine, curLine;  
  int oldPIndex, newPIndex, count;  

  {* point 1 must be a convex vertex of the polygon, since p1 is 
     the point with minimum x-coordinate! *}
  left = PAgetPoint(pArray, 1);  

  {* get both neighbours of point 1 *}
  p1 = PAgetPoint(pArray, SPgetPIndex(aStarPoly, 2));   
  p2 = PAgetPoint(pArray, SPgetPIndex(aStarPoly, PAnrOfPoints(pArray)));  
 
  {* check for all other lines whether point 1 lies on the line!
     => if yes, use the new line as border! *}
  for (count=2;  count<SPsizeOf(aStarPoly);  count++)
    {
      check1 = PAgetPoint(pArray, SPgetPIndex(aStarPoly, count));  
      check2 = PAgetPoint(pArray, SPgetPIndex(aStarPoly, count+1));  

      if (isOnLine(check1, check2, left) == 0)
	{
	  {* these points are colinear, move p1 and p2 if necessary! *}
	  if (isOnLine(check1, check2, p1) == -1)
	    p1 = check1;  
	  if (isOnLine(check1, check2, p2) == -1)
	    p2 = check1;  
	}
    }

  if (isOnLine(left, p1, p2) == 1)
    {      
      
      {* calculate both vectors emerging from p1 *}
      v1.x  = p1.x - left.x;  
      v1.y  = p1.y - left.y;  
      v2.x  = p2.x - left.x;  
      v2.y  = p2.y - left.y;  
      
      {* normalize the vectors *}
      normVec(&v1);  
      normVec(&v2);  
      
      {* add both vectors for getting the symmetric axis *}
      v1.x += v2.x;  
      v1.y += v2.y;  
      
      {* generate a point distant enough from the origin *}
      v1.x *= MAX(1.0, MAX(X_MAX, Y_MAX));  
      v1.y *= MAX(1.0, MAX(X_MAX, Y_MAX));  
      
      right.x = left.x + v1.x;  
      right.y = left.y + v1.y;  
      {* we know that left < right, based on the fact that left is the point 
	 with minimum x-coordinate *}
      
      {* start with point 2 in the polygon *}
      oldPIndex = SPgetNextPIndex(aStarPoly, 1);  
      
      {* save the 2 ortiginal points for intersection calculation *}
      BNcreateLine(&symLine, left, right);  
      
      {* check all edges whether they move our points *}
      for (count=2;  count < SPsizeOf(aStarPoly);  count++)
	{
	  newPIndex = SPgetNextPIndex(aStarPoly, oldPIndex);  
	  
	  if (isOnLine(PAgetPoint(pArray, oldPIndex), 
		       PAgetPoint(pArray, newPIndex), left) == -1)
	    {
	      BNcreatePLine(&curLine, oldPIndex, newPIndex, pArray);  
	      if (BNisectLines(&curLine, &symLine, &isectPoint))
		left = isectPoint;  
	    }
	    
	    if (isOnLine(PAgetPoint(pArray, oldPIndex), 
			 PAgetPoint(pArray, newPIndex), right) == -1)
	      {
		BNcreatePLine(&curLine, oldPIndex, newPIndex, pArray);  
		if (BNisectLines(&curLine, &symLine, &isectPoint))
		  right = isectPoint;  
	      }
	      
	      
	    oldPIndex = newPIndex;  
	}

      {* if we are in the kernel, left < right must hold! *}
      {* further we require that p2 lies left of the line (left, p1)! *}
      if (compPoints(left, right))
	{
	  isectPoint.x = (left.x+right.x)/2.0;  
	  isectPoint.y = (left.y+right.y)/2.0;  
	}
      else
	isectPoint = BGdefaultPoint;  

    }
  else
    isectPoint = BGdefaultPoint;  

  return(isectPoint);   
}
*/




