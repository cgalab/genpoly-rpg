/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __BASICDOUBLEARRAY_H_
#define __BASICDOUBLEARRAY_H_


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
  int nrOfDoubles;  
  double *array;  
} t_doubleArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void DAinit(t_doubleArray *anDoubleArray, int nrOfDoubles);  

double DAgetDouble(t_doubleArray *anDoubleArray, int index);  

void DAsetDouble(t_doubleArray *anDoubleArray, int index, double newValue);  

int DAsizeOf(t_doubleArray *anDoubleArray);  

void DAcopyArray(t_doubleArray *sourceArray, t_doubleArray *destArray);  

void DAfree(t_doubleArray *anDoubleArray);  

void DArealloc(t_doubleArray *anDoubleArray, int newNrOfDoubles);  

#endif
