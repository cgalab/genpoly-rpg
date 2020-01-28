/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicGeom.h"
#include "pointList.h"
#include "pointArray.h"

#ifndef __POINTINCARRAY_H_
#define __POINTINCARRAY_H_


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
  int curNrOfPoints;  /* current size */
  int growthSize;   /* add to current size for reallocing */ 
  t_pointArray array;  
} t_pIncArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void PIinitArray(t_pIncArray *anArray, int initialSize);  

t_point PIgetPoint(t_pIncArray *anArray, int index);  

void PIaddPoint(t_pIncArray *anArray, t_point newPoint);  

int PInrOfPoints(t_pIncArray *anArray);  

void PIfreeArray(t_pIncArray *anArray);  

/* reallocate to current size */
void PIfreezeArray(t_pIncArray *anArray);  

t_pointArray *PIgetArray(t_pIncArray *anArray);  

#endif
