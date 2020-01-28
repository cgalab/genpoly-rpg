/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "basicPoly.h"

#ifndef __MSTARPOLY_H_
#define __MSTARPOLY_H_

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
void MRfastStarPoly();  

void MGfastStarPoly(unsigned long *tries);  
#endif


void MstarPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
	       FILE *outFile);  

void MfastStarPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		   FILE *outFile);  

void MnewStarPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		  FILE *outFile);  

#endif
