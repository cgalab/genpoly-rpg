/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __ULONGARRAY_H_
#define __ULONGARRAY_H_


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
  int nrOfULongs;  
  unsigned long *array;  
} t_uLongArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void ULinit(t_uLongArray *anuLongArray, int nrOfULongs);  

unsigned long ULgetULong(t_uLongArray *anArray, int index);  

void ULsetULong(t_uLongArray *anArray, int index, unsigned long newValue);  

void ULinc(t_uLongArray *anArray, int index);  

void ULdec(t_uLongArray *anArray, int index);  

int ULsizeOf(t_uLongArray *anArray);  

void ULcopyArray(t_uLongArray *sourceArray, t_uLongArray *destArray);  

void ULfree(t_uLongArray *anArray);  

void ULrealloc(t_uLongArray *anArray, int newNrOfULongs);  

#endif
