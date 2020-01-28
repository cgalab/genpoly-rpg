/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicHeap.h"
#include "lineInfo.h"

#ifndef __LINEHEAP_H_
#define __LINEHEAP_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#define HMIN          1
#define HMAX          2


/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

typedef struct {
  t_heap heap;  
  int minMax;  /* use LIcompMinLines or LIcompMaxLines? */
} t_lineHeap;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void LHinitHeap(t_lineHeap *aHeap, int minMax);  

void LHaddElem(t_lineHeap *aHeap, t_lInfoP elem);  

t_lInfoP LHviewTop(t_lineHeap *aHeap);  

t_lInfoP LHgetTop(t_lineHeap *aHeap);  

int LHisEmpty(t_lineHeap *aHeap);  

void LHfreeHeap(t_lineHeap *aHeap);  

#endif

