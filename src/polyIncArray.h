/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "polyArray.h"

#ifndef __POLYINCARRAY_H_
#define __POLYINCARRAY_H_


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
  int curNrOfPolys;  /* current size */
  int growthSize;   /* add to current size for reallocing */ 
  t_polyArray array;  
} t_yIncArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void YIinitArray(t_yIncArray *anArray, int initialSize);  

t_polygon *YIgetPoly(t_yIncArray *anArray, int index);  

void YIaddPoly(t_yIncArray *anArray, t_polygon *newPoly);  

int YInrOfPolys(t_yIncArray *anArray);  

void YIfreeArray(t_yIncArray *anArray);  

/* reallocate to current size */
void YIfreezeArray(t_yIncArray *anArray);  

t_polygon *YIgetArray(t_yIncArray *anArray);  

#endif
