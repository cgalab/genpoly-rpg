/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "sortPoly.h"
#include "basicPoly.h"
#include <stdlib.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* point around which we will sort */
static t_point SPcompPoint;  
   
/* logic variable whether the compare point is the first point */
static int first;  

/* index of the point we want to be the first point */
static int indexOne;  

/* array of the points we want to sort */
static t_pointArray *SPpointArray;  

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

/* NOTE: this function is invoced by qsort, thus if elem1 < elem2, 
   -1 must be returned! */
int boComparePoints(const void *elem1, const void *elem2)
{
   int *index1, *index2;  
   int side1, side2;  
   int res;  
   double length1, length2;  
   t_point point1, point2;  
   t_point basePoint, firstPoint;  
   
   index1 = (int *) elem1;  
   index2 = (int *) elem2;  
   
   /* special treatment for Index 1: We always want the point with index 1
      to be the first point in our array! */
   if (*index1 == indexOne)
      /* index1 is 1, therefore index1 < index2 */
      res = -1;  
   else if (*index2 == indexOne)
      /* index2 is 1, therefore index1 > index2 */
      res = 1;  
   /* if not sorting around first point, compare lines only if they 
      lie on same side of line(compPoint, point1)! */
   else   {
      point1 = PAgetPoint(SPpointArray, *index1);  
      point2 = PAgetPoint(SPpointArray, *index2);  
      basePoint = SPcompPoint;  
      firstPoint = PAgetPoint(SPpointArray, indexOne);  
      
      /* we need special handling for points that are colinear with 
         the base point and the first point! */
      if (first ||
          (isOnLine(basePoint, firstPoint, point1) == 1) ||
          ((isOnLine(basePoint, firstPoint, point1) == 0) &&
           (compPoints(basePoint, firstPoint) == compPoints(basePoint, point1))))
         side1 = 1;  
      else
         side1 = -1;  
      
      if (first ||
          (isOnLine(basePoint, firstPoint, point2) == 1) ||
          ((isOnLine(basePoint, firstPoint, point2) == 0) &&
           (compPoints(basePoint, firstPoint) == compPoints(basePoint, point2))))
         side2 = 1;  
      else
         side2 = -1;  
      
      if (side1 == side2) {
         /* sort the points s.t. the resulting polygon is in ccw order */
         if (*index1 < *index2)
            res = -isOnLine(basePoint, point1, point2);  
         else
            res = isOnLine(basePoint, point2, point1);  
         
         /* if the three points are collinear, check the distance */
         if (res == 0) {
            length1 = lineLength(basePoint, point1);  
            length2 = lineLength(basePoint, point2);  
            
            if (deltaAbs(length1 - length2))
               res = 0;  
            else if (length1 < length2)
               res = -1;  
            else
               res = 1;  
         }
      }
      else if (side1 == -1)
         res = 1;  
      else
         res = -1;     	    
      /* if we did not sort around the first point, we need to invert the
         result in order to get a ccw polygon */
   }

   return(res);  
}


void sortPoints(t_pointArray *pArray, t_polygon *aPolygon, t_point compPoint)
{
  int count;  

  BPfree(aPolygon);  
  BPinit(aPolygon, PAnrOfPoints(pArray));  

  for (count=1;  count<=PAnrOfPoints(pArray);  count++) {
    BPsetPIndex(aPolygon, count, count);  
  }

  SPpointArray = pArray;  
  SPcompPoint = compPoint;  

  indexOne = 1;  
  
  first = equalPoints(compPoint, PAgetPoint(pArray, 1));  

  qsort(aPolygon->array, PAnrOfPoints(pArray), sizeof(int), boComparePoints);  
}

/* sort the points around first point */
void sortPFirst(t_pointArray *pArray, t_polygon *aPolygon)
{
  sortPoints(pArray, aPolygon, PAgetPoint(pArray, 1));  
}


void sortSelPoints(t_pointArray *pArray, t_iIncArray *iArray, int compPIndex)
{
  SPpointArray = pArray;  
  SPcompPoint = PAgetPoint(pArray, compPIndex);  
  
  first = FALSE;  
  
  indexOne= IIgetInt(iArray, 1);  

  qsort(IIgetArray(iArray)->array, IIsizeOf(iArray), sizeof(int), 
	boComparePoints);  
}








