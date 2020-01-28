/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#ifndef __POLYARRAY_H_
#define __POLYARRAY_H_


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
  int nrOfPolys;  
  t_polygon *array;  
} t_polyArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void YAinitEmptyArray(t_polyArray *anArray);  

void YAinitArray(t_polyArray *anArray, int nrOfPolys);  

t_polygon *YAgetPoly(t_polyArray *anArray, int index);  

void YAsetPoly(t_polyArray *anArray, int index, t_polygon *newPoly);  

int YAnrOfPolys(t_polyArray *anArray);  

void YAfreeArray(t_polyArray *anArray);  

void YAreallocArray(t_polyArray *anArray, int newNrOfPolys);  

#endif
