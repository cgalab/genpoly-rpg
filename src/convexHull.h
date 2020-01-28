/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "sortPoly.h"
#include "basicPolyLine.h"
#include "basicLine.h"

#ifndef __CONVEXHULL_H_
#define __CONVEXHULL_H_

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

typedef struct
{
  t_polyLine pointsOnHull;  
  t_point min, max;  
} t_convexHull;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

/* calculate the convex hull of the points given in pArray */
void CHcalcHull(t_convexHull *aHull, t_pointArray *pArray);  

void CHfreeHull(t_convexHull *aHull);  

/* answer true if the query point is one of the points 
   currently defining the convex hull */
int CHisOnHull(t_convexHull *aHull, int pIndex);  

/* answer TRUE if queryPoint lies within/on convexHull, 
   FALSE if not */
int CHisInBox(t_convexHull *aHull, t_point *queryPoint);  

t_polyLine CHgetPoints(t_convexHull *aHull);  

int CHisInHull(t_convexHull *aHull, t_point *queryPoint, t_pointArray *pArray);  

void CHisectHull(t_convexHull *aHull, t_line *aLine, t_pointArray *pArray, 
		 t_point *isectPoint1, t_point *isectPoint2);  

#endif
