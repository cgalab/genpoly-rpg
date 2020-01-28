/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicGeom.h"
#include "pointList.h"

#ifndef __POINTARRAY_H_
#define __POINTARRAY_H_


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

typedef struct{
  int nrOfPoints;  
  t_point *array;  
} t_pointArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
int PAvalidPoint(t_pointArray *anArray, t_point aPoint, int anIndex);  

void PAinitEmptyArray(t_pointArray *anArray);  

void PAcopyArray(t_pointArray *sourceArray, t_pointArray *destArray);  

int PAcreateList(t_pointArray *anArray, t_pointList *aList);  

void PAinitArray(t_pointArray *anArray, int nrOfPoints);  

int PAwriteToFile(t_pointArray *anArray, FILE *outputFile);  

int PAreadFile(t_pointArray *anArray, char *fileName);  

int PAreadFromFile(t_pointArray *anArray, FILE *inputFile);  

void PAlistInitArray(t_pointArray *anArray, t_pointList *aList);  

t_point PAgetPoint(t_pointArray *anArray, int index);  

int PAgetPIndex(t_pointArray *anArray, t_point aPoint);  

int PAisInArray(t_pointArray *anArray, t_point aPoint);  

void PAsetPoint(t_pointArray *anArray, int index, t_point newPoint);  

int PAnrOfPoints(t_pointArray *anArray);  

void PAfreeArray(t_pointArray *anArray);  

int PAisOnLine(t_pointArray *anArray, int indexLineP1, 
               int indexLineP2, int indexPoint);  

int PAisectSegments(t_pointArray *anArray, int indexL1P1, 
                 int indexL1P2, int indexL2P1, int indexL2P2);  

/* IMPORTANT! Normally an array will always be sorted, but for 
   the "bouncing polygons" we have to sort the array since we
   modified our points! */

void PAsortArray(t_pointArray *anArray);  

/* for the star-shaped polygons we need a method for growing an array */

void PAreallocArray(t_pointArray *anArray, int newNrOfPoints);  

void PAgetMinMax(t_pointArray *anArray, t_point *minPoint, t_point *maxPoint);  

#endif
