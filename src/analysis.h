/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "basicPoly.h"

#ifndef __ANALYSIS_H_
#define __ANALYSIS_H_

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
void angleDist(t_pointArray *pArray, t_polygon *aPolygon, int nrOfClasses, 
	   char *fName);  

void lengthDist(t_pointArray *pArray, t_polygon *aPolygon, int nrOfClasses, 
	   char *fName);  

void slopeDist(t_pointArray *pArray, t_polygon *aPolygon, int nrOfClasses, 
	   char *fName);  

void sinuosity(t_pointArray *pArray, t_polygon *aPolygon, char *fName);  

#endif
