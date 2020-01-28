/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __TRIANGARRAY_H_
#define __TRIANGARRAY_H_


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
  int vertex[3];  
} t_triangle;  

typedef struct{
  int nrOfTriangs;  
  int curNrOfTriangs;  
  int growthSize;  
  t_triangle *array;  
} t_triangArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void TRinitEmptyArray(t_triangArray *anArray);  

void TRinitArray(t_triangArray *anArray, int nrOfTriangs);  

t_triangle TRgetTriang(t_triangArray *anArray, int index);  

void TRsetTriang(t_triangArray *anArray, int index, t_triangle newTriang);  

int TRnrOfTriangs(t_triangArray *anArray);  

void TRfreeArray(t_triangArray *anArray);  

void TRreallocArray(t_triangArray *anArray, int newNrOfTriangs);  

void TRfreezeArray(t_triangArray *anArray);  

void TRcopyArray(t_triangArray *sourceArray, t_triangArray *destArray);  

void TRaddTriang(t_triangArray *anArray, t_triangle newTriang);  

#endif
