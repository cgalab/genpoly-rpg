/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h" 

#ifndef __MREJECTPOLY_H_
#define __MREJECTPOLY_H_

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
#ifdef DO_STATS
void MRrejectPoly();  

void MGrejectPoly(unsigned long *tries);  
#endif

void MrejectPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		 FILE *outFile);  

#endif
