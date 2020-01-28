/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicVoidArray.h"

#ifndef __VOIDINCARRAY_H_
#define __VOIDINCARRAY_H_


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
  t_voidArray array;  
} t_vIncArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void VIinitArray(t_vIncArray *anArray, int initialSize);  

void *VIgetVoid(t_vIncArray *anArray, int index);  

void VIsetVoid(t_vIncArray *anArray, int index, void *newValue);  

void VIaddVoid(t_vIncArray *anArray, void *newVoid);  

int VIsizeOf(t_vIncArray *anArray);  

void VIfreeArray(t_vIncArray *anArray);  

/* reallocate to current size */
void VIfreezeArray(t_vIncArray *anArray);  

t_voidArray *VIgetArray(t_vIncArray *anArray);  

#endif
