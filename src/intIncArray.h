/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIntArray.h"

#ifndef __INTINCARRAY_H_
#define __INTINCARRAY_H_


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
  t_intArray array;  
} t_iIncArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void IIinitArray(t_iIncArray *anArray, int initialSize);  

int IIgetInt(t_iIncArray *anArray, int index);  

void IIsetInt(t_iIncArray *anArray, int index, int newValue);  

void IIaddInt(t_iIncArray *anArray, int newInt);  

int IIsizeOf(t_iIncArray *anArray);  

void IIfreeArray(t_iIncArray *anArray);  

/* reallocate to current size */
void IIfreezeArray(t_iIncArray *anArray);  

t_intArray *IIgetArray(t_iIncArray *anArray);  

void IIsortArray(t_iIncArray *anArray);  

#endif
