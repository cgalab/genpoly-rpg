/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __BINARRAY_H_
#define __BINARRAY_H_

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
  int size;  
  int arrSize;  
  unsigned char *array;  
} t_binArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void BAinit(t_binArray *aBinArray, int nrOfInts);  

int BAgetBool(t_binArray *aBinArray, int index);  

void BAsetBool(t_binArray *aBinArray, int index, int newValue);  

int BAsizeOf(t_binArray *aBinArray);  

void BAfree(t_binArray *aBinArray);  

#endif
