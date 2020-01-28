/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIntArray.h"
#include "pointArray.h"
#ifndef __BASICPOLY_H_
#define __BASICPOLY_H_

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

typedef t_intArray t_polygon;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void BPinit(t_polygon *aPolygon, int nrOfPoints);  

void BPrandomPerm(t_polygon *aPolygon);  

void BPswap(t_polygon *aPolygon, int index1, int index2);  

void BPnormalize(t_polygon *aPolygon, t_pointArray *pArray);  

int BPisCCW(t_polygon *aPolygon, t_pointArray *pArray);  

int BPisSimple(t_polygon *aPolygon, t_pointArray *pArray);  

int BPgetPIndex(t_polygon *aPolygon, int index);  

void BPsetPIndex(t_polygon *aPolygon, int index, int newValue);  

int BPsizeOf(t_polygon *aPolygon);  

void BPcopyPoly(t_polygon *sourcePolygon, t_polygon *destPolygon);  

void BPfree(t_polygon *aPolygon);  

int BPwriteToFile(t_polygon *aPolygon, FILE *outputFile);  

int BPwriteFile(t_polygon *aPolygon, t_pointArray *pArray, char *fileName);  

int BPreadFile(t_polygon *aPolygon, t_pointArray *pArray, char *fileName);  

int BPreadFromFile(t_polygon *aPolygon, int nrOfPoints, FILE *inputFile);  

#endif
