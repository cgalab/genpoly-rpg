/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __BASICINTARRAY_H_
#define __BASICINTARRAY_H_


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
  int nrOfInts;  
  int *array;  
} t_intArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void IAinit(t_intArray *anIntArray, int nrOfInts);  

int IAgetInt(t_intArray *anIntArray, int index);  

void IAsetInt(t_intArray *anIntArray, int index, int newValue);  

void IAinc(t_intArray *anIntArray, int index);  

void IAdec(t_intArray *anIntArray, int index);  

int IAsizeOf(t_intArray *anIntArray);  

void IAcopyArray(t_intArray *sourceArray, t_intArray *destArray);  

void IAfree(t_intArray *anIntArray);  

void IArealloc(t_intArray *anIntArray, int newNrOfInts);  

#endif
