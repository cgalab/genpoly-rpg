/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicGeom.h"
#include "basicDefs.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

t_point BGdefaultPoint = {X_OUT, Y_OUT};  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
int randomInt(int min, int max)
{
  int result;  

  result = min +((int) (((double) (max-min+1))*((double) lrand48())/
                  ((double) RANDOM_MAX)));  

  if (result > max)
    result = max;  
  else if (result < min)
    result = min;  

  return(result);  
}

double randomDouble()
{
  double result;  

  result = ((double)lrand48())/((double) RANDOM_MAX);  

  return(result);  
}


unsigned long randomULong(unsigned long min, unsigned long max)
{
  unsigned long result;  

  result = min +((unsigned long) (((double) (max-min+1))*((double) lrand48())/
                  ((double) RANDOM_MAX)));  

  if (result > max)
    result = max;  
  else if (result < min)
    result = min;  

  return(result);  
}

t_point randomPoint()
{
  double randomX, randomY;  
  t_point randomPoint;  
  
  randomX = ((double)lrand48())/((double) RANDOM_MAX);  
  randomY = ((double)lrand48())/((double) RANDOM_MAX);  
  
  /* transform to [XZ_MIN, X_MAX]x[Y_MIN, Y_MAX] */
  randomPoint.x = X_MIN + (randomX * (X_MAX-X_MIN));  
  randomPoint.y = Y_MIN + (randomY * (Y_MAX-Y_MIN));  
  randomPoint.x_ori = randomPoint.x;
  randomPoint.y_ori = randomPoint.y;

  return(randomPoint);  
}

void normVec(t_point *vector)
{
  double length;  
  
  length = sqrt((vector->x*vector->x)+(vector->y*vector->y));  

  vector->x /= length;  
  vector->y /= length;  
}

int validPoint(t_point aPoint)
{
  int res;  
  
  res = ((aPoint.x >= X_MIN) && (aPoint.x <= X_MAX) &&
         (aPoint.y >= Y_MIN) && (aPoint.y <= Y_MAX));  

  return(res);  
}

int equalPoints(t_point point1, t_point point2)
/* this procedure tests if point 1 = point2 and returns the result */
{
  int res;  

  res = ((point1.x == point2.x) && (point1.y == point2.y));  

  return(res);  
}


double lineLength(t_point linePoint1, t_point linePoint2)
{
  double length;  

  length = (linePoint1.x-linePoint2.x)*(linePoint1.x-linePoint2.x) +
           (linePoint1.y-linePoint2.y)*(linePoint1.y-linePoint2.y);  

  return(length);  
}



int compPoints(t_point point1, t_point point2)
/* this procedure tests if point 1 < point2 and returns the result */
{
  int res;  

  res = ((point1.x < point2.x) || ((point1.x == point2.x) &&
                                   (point1.y < point2.y)));  

  return(res);  
}

void compSwapPoints(t_point *point1, t_point *point2)
/* compSwapPoints swaps point1 and point2 if !compPoints(point1, point2), 
   i.e. point2 < point1 */
{
  if (!compPoints(*point1, *point2))
    swapPoints(point1, point2);  
}

void swapPoints(t_point *point1, t_point *point2)
{
  t_point auxPoint;  

  auxPoint.x = point1->x;  
  auxPoint.y = point1->y;  
  point1->x = point2->x;  
  point1->y = point2->y;  
  point2->x = auxPoint.x;  
  point2->y = auxPoint.y;  
}

double calcArea(t_point point1, t_point point2, t_point point3)
{
  /* for details of the calculation, refer to O'Rourke, Comp Geometry 
     in C, pp.  19-20 */
  double twiceArea;  

  /*  twiceArrea = point1.x * point2.y - point1.y * point2.x +
      point1.y * point3.x - point1.x * point3.y +
      point2.x * point3.y - point3.x * point2.y;   */

  twiceArea = (point3.y - point1.y)*(point2.x - point1.x) -
    (point3.x - point1.x)*(point2.y - point1.y);  


  if (deltaAbs(twiceArea))  /* test if result lt delta */
    twiceArea = 0.0;  
  
  return(twiceArea);  
}

int isOnLine(t_point linePoint1, t_point linePoint2, 
             t_point aPoint)
{
  int res;  

  if (compPoints(linePoint1, linePoint2))
    res = isOnOrderedLine(linePoint1, linePoint2, aPoint);  
  else
    res = -isOnOrderedLine(linePoint2, linePoint1, aPoint);  
  return(res);  
}

int isOnOrderedLine(t_point linePoint1, t_point linePoint2, t_point aPoint)
{
  int res;  
  double area;  

  if (equalPoints(linePoint1, linePoint2))  {
    ERerror(ERR_LINE);  
  }

  area = calcArea(linePoint1, linePoint2, aPoint);  

  if (area > DELTA)
    res = 1;  
  else if (area < -DELTA)
    res = -1;  
  else 
    res = 0;  

  return(res);  
}
  


int isectSegments(t_point line1point1, t_point line1point2, 
		  t_point line2point1, t_point line2point2)
{
  int result;  
   double xmin2, xmax1, ymin1, ymin2, ymax1, ymax2;  

  /* we want both line segments ordered in a way that p1 < p2 */
  compSwapPoints(&line1point1, &line1point2);  
  compSwapPoints(&line2point1, &line2point2);  

  /* We always want to have l1p1<l2p1 or (l1p1=l2p1 and l1p2<l2p2) */
  /* That guarantees that for any two line segments we will always 
     have the same order of the 4 points when doing the test on
     intersection */
  if (!(compPoints(line1point1, line2point1) || 
                  (equalPoints(line1point1, line2point1) &&
                   compPoints(line1point2, line2point2)))) {
    swapPoints(&line1point1, &line2point1);  
    swapPoints(&line1point2, &line2point2);  
  }

  /* now that we have assured that the points will always be in the 
     same order, we can do the testing. first we do a bounding-box
     test. we can take the x minima and maxima directly, since our 
      points are ordered */
   xmax1 = line1point2.x;  
   xmin2 = line2point1.x;  
   
   /* now compute the y minima and maxima */
   if (line1point1.y < line1point2.y)  {
      ymin1 = line1point1.y;  
      ymax1 = line1point2.y;  
   }
   else  {
      ymin1 = line1point2.y;  
      ymax1 = line1point1.y;  
   }
   
   if (line2point1.y < line2point2.y)   {
      ymin2 = line2point1.y;  
      ymax2 = line2point2.y;  
   }
   else  {
      ymin2 = line2point2.y;  
      ymax2 = line2point1.y;  
   }
   
   /* test whether the bounding boxes overlap! */
   /* from our ordering, we know that xmin1 < xmax2 */
   if ((xmax1 < xmin2) ||
       (ymax2 < ymin1) || (ymax1 < ymin2))    {
      result = FALSE;  
   }
   else {
      result = isectOrderedSegments(line1point1, line1point2, 
                                    line2point1, line2point2);  
   }
  return(result);  
}


int isectOrderedSegments(t_point line1point1, t_point line1point2, 
                         t_point line2point1, t_point line2point2)
{
   int posL1P1, posL1P2, posL2P1, posL2P2;  
   int res1, res2;  
   int result;  

   //printf("isectOrderedSegments():\n");
   //printf("(%f,%f))-->(%f,%f)\n", line1point1.x, line1point1.y, line1point2.x, line1point2.y);
   //printf("(%f,%f))-->(%f,%f)\n", line2point1.x, line2point1.y, line2point2.x, line2point2.y);
   /* determine where the points of line1 lie with respect to line2 */
   posL1P1 = isOnLine(line2point1, line2point2, line1point1);  
   posL1P2 = isOnLine(line2point1, line2point2, line1point2);  
   
   if ((res1 = posL1P1*posL1P2) == 1)  {
      result = FALSE;  
   }
   else	{
      /* determine where the points of line2 lie with respect to line1 */
      posL2P1 = isOnLine(line1point1, line1point2, line2point1);  
      posL2P2 = isOnLine(line1point1, line1point2, line2point2);  
      
      /* test whether either a and b lie on one side of (c, d) or vice versa */
      if ((res2 = posL2P1*posL2P2) == 1)
         /* lines do not intersect */
         result = FALSE;  
      
      /* we check whether we have intersection */
      else if ((res1 + res2) == -2)
         /* we have intersection */
         result = TRUE;  
      
      /* test for collinearity */
      else if ((posL1P1 + 2*posL1P2 + 4*posL2P1 + 8*posL2P2) == 0)
         /* We have collinearity */
         /* since we have l1p1 < l1p2, l2p1 < l2p2 and further
            l1p1 <= l2p1, all we have to check is whether l2p1 < l1p2, 
            if yes, the lines overlap! */
         /* we ignore collinearities for the moment, though! */
         //result = compPoints(line2point1, line1point2);  
         result = 0;
      
      else
      /* One point lies on the other line, e.g. a on (c, d). If
         a!=c and a!=d we have intersection */
         result = (!equalPoints(line1point1, line2point1) &&
                   !equalPoints(line1point1, line2point2) &&
                   !equalPoints(line1point2, line2point1) &&
                   !equalPoints(line1point2, line2point2));  
   }
   
   return(result);  
}


int testDelta(double compVal, double delta)
{
  int result;  

  if (delta > 0.0)
     result = (fabs(compVal) < delta*MAX(X_MAX, Y_MAX));  
  else
    result = (fabs(compVal) == 0.0);  

  return(result);  
}

/* test for less than delta (compare with constant DELTA) */
int deltaAbs(double compVal)
{
   //return(testDelta(compVal, DELTA));  
   return (fabs(compVal) <= DELTA);  
}

int deltaVal(double compVal)
{
   //return(testDelta(compVal, BGdelta));  
   return (fabs(compVal) <= DELTA);  
}
      

int signum(double x) 
{
   return ((x > 0) ? 1 : ((x < 0) ? -1 : 0));
}
