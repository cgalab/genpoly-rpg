/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"

#ifndef __ISECTARRAY_H_
#define __ISECTARRAY_H_

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

typedef struct {
  int size;  
  int nrOfPoints;  
  char *array;  
} t_isectArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void ISinit(t_isectArray *iArray, t_pointArray *pArray, int nrOfLines);  

int IScheckIsect(t_isectArray *iArray, int s1pIndex1, int s1pIndex2, 
		  int s2pIndex1, int s2pIndex2);  

void ISfree(t_isectArray *iArray);  

#endif
