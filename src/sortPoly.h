/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "intIncArray.h"
#include "basicPoly.h"

#ifndef __SORTPOLY_H_
#define __SORTPOLY_H_

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



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

/* inits a polygon and sorts all points in clockwise order around the 
   first point */

void sortPoints(t_pointArray *pArray, t_polygon *aPolygon, t_point compPoint);  

void sortPFirst(t_pointArray *pArray, t_polygon *aPolygon);  

void sortSelPoints(t_pointArray *pArray, t_iIncArray *iArray, int compPIndex);  

#endif
