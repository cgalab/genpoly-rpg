/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __BASICVOIDARRAY_H_
#define __BASICVOIDARRAY_H_

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
  int nrOfVoids;  
  void **array;  
//	void *array;  
} t_voidArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void VAinit(t_voidArray *anVoidArray, int nrOfVoids);  

void *VAgetVoid(t_voidArray *anVoidArray, int index);  

void VAsetVoid(t_voidArray *anVoidArray, int index, void *newValue);  

int VAsizeOf(t_voidArray *anVoidArray);  

void VAcopyArray(t_voidArray *sourceArray, t_voidArray *destArray);  

void VAfree(t_voidArray *anVoidArray);  

void VArealloc(t_voidArray *anVoidArray, int newNrOfVoids);  

#endif
