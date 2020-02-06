/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "convexHull.h"
#include "basicGeom.h"

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

/* calculate the convex hull of the points given in pArray */
void CHcalcHull(t_convexHull *aHull, t_pointArray *pArray)
{
   t_polygon sortedPoly;  
   t_polyLine pOnHull;  
   t_point curPoint;  
   int count;  
   
   BPinit(&sortedPoly, 0);  
   sortPFirst(pArray, &sortedPoly);  
   
   /* init convex hull with max. number of points + 1, since 
      the last point will appear twice on the hull while running 
      the algorithm */
   BLinit(&pOnHull, PAnrOfPoints(pArray)+1);  
   
   /* set the first 2 points */
   BLaddPIndex(&pOnHull, BPgetPIndex(&sortedPoly, BPsizeOf(&sortedPoly)));  
   BLaddPIndex(&pOnHull, BPgetPIndex(&sortedPoly, 1));  
   
   count=2;  
   while (count <= BPsizeOf(&sortedPoly)) {
      if (PAisOnLine(pArray, 
                     BLgetPIndex(&pOnHull, BLsizeOf(&pOnHull)-1), 
                     BLgetPIndex(&pOnHull, BLsizeOf(&pOnHull)), 
                     BPgetPIndex(&sortedPoly, count)) >= 0)	{
         BLaddPIndex(&pOnHull, BPgetPIndex(&sortedPoly, count));  
         count++;  
      }
      else {
         BLdelLastIndex(&pOnHull); 
      }
   }
   
   /* free sortedPoly! */
   BPfree(&sortedPoly);   
   
   /* We have the last point twice on the hull, so just delete it once! */
   BLdelLastIndex(&pOnHull);  
   
   BLinit(&(aHull->pointsOnHull), BLsizeOf(&pOnHull));  
   
   curPoint = PAgetPoint(pArray, BLgetPIndex(&pOnHull, 1));  
   aHull->min = curPoint;  
   aHull->max = curPoint;  
   
   for (count=2;  count <= BLsizeOf(&pOnHull);  count++)   {
      BLaddPIndex(&(aHull->pointsOnHull), BLgetPIndex(&pOnHull, count));  
      curPoint = PAgetPoint(pArray, BLgetPIndex(&pOnHull, count));  
      aHull->min.x = MIN(aHull->min.x, curPoint.x);  
      aHull->min.y = MIN(aHull->min.y, curPoint.y);  
      aHull->max.x = MAX(aHull->max.x, curPoint.x);  
      aHull->max.y = MAX(aHull->max.y, curPoint.y);  
   }

   BLaddPIndex(&(aHull->pointsOnHull), BLgetPIndex(&pOnHull, 1));  
   
   BLfree(&pOnHull);  
}

void CHfreeHull(t_convexHull *aHull)
{
   BLfree(&(aHull->pointsOnHull));  
}


int CHisOnHull(t_convexHull *aHull, int pIndex)
{
   int count, result;  
   /* we will use the straightforward approach of checking whether the 
      point in question is one of the points definig the hull, 
      instead of the (theoretically) faster approach used in CHisInHull */
   
   count = 0;  
   result = FALSE;  
   
   while ((count < BLsizeOf(&(aHull->pointsOnHull))) && (!result))  {
      count ++;  
      result = (BLgetPIndex(&(aHull->pointsOnHull), count) == pIndex);  
   }
   
   return(result);  
}


/* answer TRUE if queryPoint lies within/on convexHull, 
   FALSE if not */
int CHisInBox(t_convexHull *aHull, t_point *queryPoint)
{
   return((queryPoint->x >= aHull->min.x) && (queryPoint->y >= aHull->min.y) &&
          (queryPoint->x <= aHull->max.x) && (queryPoint->y <= aHull->max.y));  
}

t_polyLine CHgetPoints(t_convexHull *aHull)
{
   return(aHull->pointsOnHull);  
}

/* answer TRUE if queryPoint lies within/on convexHull, 
   FALSE if not */
int CHisInHull(t_convexHull *aHull, t_point *queryPoint, t_pointArray *pArray)
{
   int left, right, median;  
   int res = FALSE;  
   double length1, length2;  
   
   t_polyLine *pOnHull = &(aHull->pointsOnHull);  
   
   /* make a binary search for the wedge where p lies */
   
   right = 2;  
   left = BLsizeOf(pOnHull);  
   
   if ((isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                 PAgetPoint(pArray, BLgetPIndex(pOnHull, right)), 
                 *queryPoint) != -1) &&
       (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                 PAgetPoint(pArray, BLgetPIndex(pOnHull, left)), 
                 *queryPoint) != 1))  {
      /* check the special case that the point lies on the 
         first or on the last line! */
      /* special case: we have colinear points */
      if (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                   PAgetPoint(pArray, BLgetPIndex(pOnHull, right)), 
                   *queryPoint) == 0)
         left = right;  
      else if (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                        PAgetPoint(pArray, BLgetPIndex(pOnHull, left)), 
                        *queryPoint) == 0)
         right = left;  
      
      while (right < left-1) {
         /* special case: we have colinear points */
         if (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                      PAgetPoint(pArray, BLgetPIndex(pOnHull, right)), 
                      *queryPoint) == 0)
            left = right;  
         else if (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                           PAgetPoint(pArray, BLgetPIndex(pOnHull, left)), 
                           *queryPoint) == 0)
            right = left;  
         else   {
            median = (left+right)/2;  
            if (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                         PAgetPoint(pArray, BLgetPIndex(pOnHull, median)), 
                         *queryPoint) == 1)
               right = median;  
            else
               left = median;  
         }
      }
      
      if (left == right) {
         /* we have collinear points => check the distance */
         length1 = lineLength(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                              *queryPoint);  
         length2 = lineLength(PAgetPoint(pArray, BLgetPIndex(pOnHull, 1)), 
                              PAgetPoint(pArray, BLgetPIndex(pOnHull, left)));  
         
         res = ((length1 < length2) || deltaAbs(length1-length2));  
      }	 
      else	{
         /* check whether we left of the oriented edge left-right */
         res = (isOnLine(PAgetPoint(pArray, BLgetPIndex(pOnHull, right)), 
                         PAgetPoint(pArray, BLgetPIndex(pOnHull, left)), 
                         *queryPoint) != -1);  
      }
	
   }
   
   return(res);  
}

void CHisectHull(t_convexHull *cHull, t_line *aLine, t_pointArray *pArray, 
                 t_point *isectPoint1, t_point *isectPoint2)
{
   int origin, dest;  
   int origSide, destSide;  
   int first = TRUE;  
   int count;  
   t_point auxPoint;  
   t_polyLine pOnHull;  
   t_line auxLine;  
   
   pOnHull = CHgetPoints(cHull);  
   origin = BLgetPIndex(&pOnHull, BLsizeOf(&pOnHull));  
   auxPoint = PAgetPoint(pArray, origin);  
   origSide = BNisOnSide(aLine, &auxPoint);  
   
   for (count=1;  count <= BLsizeOf(&pOnHull);  count++)  {
      dest = BLgetPIndex(&pOnHull, count);  
      auxPoint = PAgetPoint(pArray, dest);  
      destSide = BNisOnSide(aLine, &auxPoint);  
      
      if (destSide == 0) {
         if (first)  {
            (*isectPoint1) = PAgetPoint(pArray, dest);  
            first = FALSE;  
         }
         else
            (*isectPoint2) = PAgetPoint(pArray, dest);  
      }
      else if ((destSide * origSide) == -1)	{
         BNcreatePLine(&auxLine, origin, dest, pArray);  
         
         if (first) {
            BNisectLines(&auxLine, aLine, isectPoint1);  
            first = FALSE;  
         }
         else
            BNisectLines(&auxLine, aLine, isectPoint2);  
         
      }
      
      origin = dest;  
      origSide = destSide;  
   }
   
   
   if (compPoints(*isectPoint2, *isectPoint1))
      swapPoints(isectPoint1, isectPoint2);  
}









