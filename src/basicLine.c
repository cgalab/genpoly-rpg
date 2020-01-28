/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicLine.h"
#include "basicDefs.h"

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void BNcreatePLine(t_line *aLine, int pIndex1, int pIndex2, t_pointArray *pArray)
{
  t_point p1, p2;  
  
  p1 = PAgetPoint(pArray, MIN(pIndex1, pIndex2));  
  p2 = PAgetPoint(pArray, MAX(pIndex1, pIndex2));  

  BNcreateLine(aLine, p1, p2);  

  aLine->pIndex1 = MIN(pIndex1, pIndex2);  
  aLine->pIndex2 = MAX(pIndex1, pIndex2);  
}


void BNcreateLine(t_line *aLine, t_point point1, t_point point2)
{
  t_point nVec;  /* normal vector */

  if (compPoints(point1, point2))
    {
      aLine->point1 = point1;  
      aLine->point2 = point2;  
    }
  else
    {
      aLine->point1 = point2;  
      aLine->point2 = point1;  
    }

  aLine->pIndex1 = -1;  
  aLine->pIndex2 = -1;  

  nVec.x = (point1.y - point2.y);  
  nVec.y = (point2.x - point1.x);  

  normVec(&nVec);  

  aLine->a = nVec.x;  
  aLine->b = nVec.y;  
  aLine->c = - ((nVec.x*point1.x) + (nVec.y*point1.y));  
}


void BNcreateDummy(t_line *aLine)
{
  aLine->point1 = BGdefaultPoint;  
  aLine->point2 = BGdefaultPoint;  

  aLine->a = 0.0;  
  aLine->b = 0.0;  
  aLine->c = 1.0;  
}


t_line BNminLine(t_line *line1, t_line *line2)
{
  t_line result;  

  if (compPoints(line1->point1, line2->point1) || 
      (equalPoints(line1->point1, line2->point1) && 
       compPoints(line1->point2, line2->point2)))
    result = *line1;  
  else
    result = *line2;  

  return(result);  
}

t_line BNmaxLine(t_line *line1, t_line *line2)
{
  t_line result;  

  if (compPoints(line1->point1, line2->point1) || 
      (equalPoints(line1->point1, line2->point1) && 
       compPoints(line1->point2, line2->point2)))
    result = *line2;  
  else
    result = *line1;  

  return(result);  
}


int BNisectLines(t_line *line1, t_line *line2, t_point *isectPoint)
{
   t_line l1, l2;  
   t_point vec1, auxPoint1, auxPoint2;  
   int res = TRUE;  
   int side;  
   double scal1, lscal, rscal;  
   double div;  
   
   /* first we have to sort our lines such that the two lines are always
      processed in the same order */
   l1 = BNminLine(line1, line2);  
   l2 = BNmaxLine(line1, line2);  
   
   /* special cases: lines share one or more points
      we assume that the two lines are not identical, 
      
      Since we know that l1 < l2, l1.p1 == l2.p2 is not possible! */
   
   /* for default, we set the intersection point to 
      BGdefaultPoint */
   (*isectPoint) = BGdefaultPoint;  
   
   
   /* check whether any of the two points are identical */ 
   /* special treatment for these cases, because it's cheap */
   if (BNisSame(&l1, &l2))
      res = FALSE;  
   else if (equalPoints(l1.point1, l2.point1)) 
      *isectPoint = l1.point1;  
   else if (equalPoints(l1.point2, l2.point1) ||
            equalPoints(l1.point2, l2.point2))
      *isectPoint = l1.point2;  
   /* check whether any point is lying on the other line */
   else if (BNisOnLine(&l2, &l1.point1))
      *isectPoint = l1.point1;  
   else if (BNisOnLine(&l2, &l1.point2))
      *isectPoint = l1.point2;  
   else if (BNisOnLine(&l1, &l2.point1))
      *isectPoint = l2.point1;  
   else if (BNisOnLine(&l1, &l2.point2))
      *isectPoint = l2.point2;  
   else   {
      /* Now for the common case that we have to calculate the 
         intersection point */
      
      /* calculate vector point 2 - point 1 */
      vec1.x = l1.point2.x - l1.point1.x;  
      vec1.y = l1.point2.y - l1.point1.y;  
      
      /* normVec(&vec1);  */
      
      div = ((l2.a * vec1.x) + (l2.b * vec1.y));  
      
      if (fabs(div) < PARDELTA)	{
         /* lines are nearly parallel */
         res = FALSE;  
      }
      else  {
         scal1 =  - ((l2.c + (l2.a*l1.point1.x) + (l2.b*l1.point1.y))/div);  
         isectPoint->x = l1.point1.x + scal1*vec1.x;  
         isectPoint->y = l1.point1.y + scal1*vec1.y;  
         
         /* check whether this point lies on line 2 */
         if (!BNisOnLine(&l2, isectPoint))  {
            /* we have to move the point in a way that it lies 
               on line2 */
            
            lscal = scal1;  
            rscal = scal1;  
            /* we have to distinct whether A lies on the left or 
               on the right side of l2 */
            side = BNisOnSide(&l2, &l1.point1);  
            
            auxPoint1 = *isectPoint;  
            auxPoint2 = *isectPoint;  
            if (side == 1)  {
               while (BNisOnSide(&l2, &auxPoint1) == -1)  {
                  lscal *= 1.0/FACTOR;  
                  auxPoint1.x = l1.point1.x + lscal*vec1.x;  
                  auxPoint1.y = l1.point1.y + lscal*vec1.y;  
               }
               
               while (BNisOnSide(&l2, &auxPoint2) == 1) {
                  rscal *= FACTOR;  
                  auxPoint2.x = l1.point1.x + rscal*vec1.x;  
                  auxPoint2.y = l1.point1.y + rscal*vec1.y;  
               }
            }
            else  {
               while (BNisOnSide(&l2, &auxPoint1) == -1) {
                  lscal *= FACTOR;  
                  auxPoint1.x = l1.point1.x + lscal*vec1.x;  
                  auxPoint1.y = l1.point1.y + lscal*vec1.y;  
               }
               
               while (BNisOnSide(&l2, &auxPoint2) == 1) {
                  rscal *= 1/FACTOR;  
                  auxPoint2.x = l1.point1.x + rscal*vec1.x;  
                  auxPoint2.y = l1.point1.y + rscal*vec1.y;  
               }
            }
            
            /* Now we have that lscal is left of l2 and 
               rscal is right of l2 */
            while ((!BNisOnLine(&l2, &auxPoint1)) &&
                   (!BNisOnLine(&l2, &auxPoint2))) {
               scal1 = (lscal+rscal)/2.0;  
               isectPoint->x = l1.point1.x +scal1*vec1.x;  
               isectPoint->y = l1.point1.y +scal1*vec1.y;  
               if (BNisOnSide(&l2, isectPoint) == 1) {
                  lscal = scal1;  
                  auxPoint1 = *isectPoint;  
               }
               else {
                  rscal = scal1;  
                  auxPoint2 = *isectPoint;  
               }
            }
            if (BNisOnLine(&l2, &auxPoint1))
               *isectPoint = auxPoint1;  
            else
               *isectPoint = auxPoint2;  
         }
	  }
	  
   }
   
   return(res);  
}


/* for a given x value, calculate y s.t. the point lies on the line, 
   return y, return TRUE, if such a point exists, else FALSE 
   (we need this for handling vertical lines!) */
int BNsetYcoord(t_line *aLine, t_point *aPoint)
{
  int res = FALSE;  

  if (!BNisVertical(aLine))  {
    aPoint->y = - ((aLine->a*aPoint->x)+aLine->c)/aLine->b;  
    res = BNisOnLine(aLine, aPoint);  
  }

  return(res);  
}

int BNsetXcoord(t_line *aLine, t_point *aPoint)
{
  int res = FALSE;  

  if (aLine->a != 0.0)  {
    aPoint->x = - ((aLine->b*aPoint->y)+aLine->c)/aLine->a;  
    res = BNisOnLine(aLine, aPoint);  
  }

  return(res);  
}


int BNisOnLine(t_line *aLine, t_point *aPoint)
{
  int res;  

  res = (BNisOnSide(aLine, aPoint) == 0);  
  return(res);   
}


/* left = 1, right = -1, on = 0 */
int BNisOnSide(t_line *aLine, t_point *aPoint)
{
  double dist;  
  int res;  

  dist = BNdistance(aLine, aPoint);  

  if (deltaAbs(dist))
    res = 0;  
  else
    res = SIGN(dist);  

  return(res);  
}


double BNdistance(t_line *aLine, t_point *aPoint)
{
  double dist;  

  dist = ((aLine->a * aPoint->x) + (aLine->b * aPoint->y) + aLine->c);  

  return(dist);  
}


int BNisVertical(t_line *aLine)
{
  return(aLine->b == 0.0);  
}

void BNcopyLine(t_line *from, t_line *to)
{
  *to = *from;  
}


t_point BNgetPoint1(t_line *aLine)
{
  return(aLine->point1);  
}

t_point BNgetPoint2(t_line *aLine)
{
  return(aLine->point2);  
}

void BNsetPIndex1(t_line *aLine, int newIndex)
{
  aLine->pIndex1 = newIndex;  
}

int BNgetPIndex1(t_line *aLine)
{
  return(aLine->pIndex1);  
}

int BNgetPIndex2(t_line *aLine)
{
  return(aLine->pIndex2);  
}


int BNisSame(t_line *line1, t_line *line2)
{
  return((equalPoints(line1->point1, line2->point1) &&
	  equalPoints(line1->point2, line2->point2))
	 ||
	 (deltaAbs(line1->a - line2->a) &&
	  deltaAbs(line1->b - line2->b) && 
	  deltaAbs(line1->c - line2->c)));  

}


