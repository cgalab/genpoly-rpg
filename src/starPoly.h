/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIntArray.h"
#include "pointArray.h"
#include "basicPoly.h"

#ifndef __STARPOLY_H_
#define __STARPOLY_H_

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

typedef struct {
  t_polygon polygon;   /* the polygon */
  /* at next[i] we have the Index of the point that is following
     i in the polygon (in ccw order) */
  t_intArray next;    
} t_starPoly;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/


void SPinit(t_starPoly *aStarPoly, int nrOfPoints);  

void SPcreatePoly(t_starPoly *aStarPoly, t_point centerPoint, 
		  t_pointArray *pArray);  

int SPgetPIndex(t_starPoly *aStarPoly, int index);  

void SPswapPoints(t_starPoly *aStarPoly, int index1, int index2);  

int SPgetNextPIndex(t_starPoly *aStarPoly, int index);  

int SPsizeOf(t_starPoly *aStarPoly);  

void SPcopyPoly(t_starPoly *sourceStarPoly, t_starPoly *destStarPoly);  

int SPisPolyEdge(t_starPoly *aStarPoly, int pIndex1, int pIndex2);  

t_polygon *SPgetPoly(t_starPoly *aStarPoly);  

void SPfree(t_starPoly *aStarPoly);  

/* this routine is faulty! do not use it! */
t_point SPpInKernel(t_starPoly *aStarPoly, t_pointArray *pArray);  

#endif


