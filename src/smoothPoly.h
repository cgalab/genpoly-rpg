/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointList.h"
#include "pointArray.h"
#include "basicPoly.h"

#ifndef __SMOOTHPOLY_H_
#define __SMOOTHPOLY_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#define POINT_DISTANCE    0.25  /* distance from the original point, 
				   must be < 0.5 */

#define MAX_POINTS       50000  /* The smooth operartion doubles the
				   number of points, we will not work
				   on more than 50, 000 points */

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

void smoothPoly(t_pointList *pList, t_pointArray *pArray, 
		t_polygon *aPolygon);  

#endif
