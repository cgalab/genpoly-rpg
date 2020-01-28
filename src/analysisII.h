/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "basicPoly.h"
#include "basicDoubleArray.h"

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
void resetAnalysis(t_pointArray *pArray, int lenRes, int slopeRes, 
		   int angleRes, int nrOfPolygons);  

void resetSinuosityII(int nrOfPoints, int nrOfPolygons);  

void resetAnalysisII(int lenRes, int slopeRes, int angleRes, 
		   int nrOfPoints, int nrOfPolygons);  

void writeSinuosityII(char *fSinu);  

void writeAnalysis(char *fLen, char *fSlope, char *fAngle, char *fSinu);  

void angleDistII(t_pointArray *pArray, t_polygon *aPolygon);  

void lengthDistII(t_pointArray *pArray, t_polygon *aPolygon);  

void slopeDistII(t_pointArray *pArray, t_polygon *aPolygon);  

void sinuosityII(t_pointArray *pArray, t_polygon *aPolygon);  











