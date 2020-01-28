/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h" 
#include "pointList.h"

#ifndef __MBOUNCEPOLY_H_
#define __MBOUNCEPOLY_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/* define an U_\epsilon in which the new vertex must lie */
#ifndef MOVE_RADIUS
#define MOVE_RADIUS 0.2
#endif

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

#ifdef DO_STATS
void MRbouncePoly();  

void MGbouncePoly(unsigned long *tries, double *radius);  
#endif



void MbouncePoly(t_pointList *aList, t_polygon *aPolygon, t_pointArray *pArray, 
		 int nrOfMoves);  

#endif


