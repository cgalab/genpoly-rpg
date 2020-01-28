/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h" 

#ifndef __M2OPTPOLYII_H_
#define __M2OPTPOLYII_H_

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
void MR2optPolyII();  

void MG2optPolyII(unsigned long *tries);  

void MR2optPolyIII();  

void MG2optPolyIII(unsigned long *tries);  
#endif



void MnewDo2optMoves(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPoints, 
		     int mode);  

void M2optPolyII(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
	       FILE *outFile);  

void M2optPolyIII(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
	       FILE *outFile);  








#endif
