/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h" 

#ifndef __MGROWINGPOLY_H_
#define __MGROWINGPOLY_H_

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
  int orig;  
  int dest;  
  int visible;   /* has there been an edge in fronty of this edge? */
} t_McompEdge;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
#ifdef DO_STATS
void MRgrowingPoly();  

void MGgrowingPoly(unsigned long *tries);  
#endif



void MgrowingPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		  FILE *outFile);  

void MgrowingPolyII(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		  FILE *outFile);  

#endif
