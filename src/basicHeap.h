/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <sys/types.h>
#include <stdlib.h>

#ifndef __BASICHEAP_H_
#define __BASICHEAP_H_

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

struct t_heapElem {
  caddr_t elem;  
  struct t_heapElem *left, *right;  
  int pathLength;  
};  

typedef struct {
  struct t_heapElem *top;  
} t_heap;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void BHinitHeap(t_heap *aHeap);  

void BHaddElem(t_heap *aHeap, caddr_t elem, int compProc(caddr_t, caddr_t));  

caddr_t BHviewTop(t_heap *aHeap);  

caddr_t BHgetTop(t_heap *aHeap, int compProc(caddr_t, caddr_t));  

int BHisEmpty(t_heap *aHeap);  

void BHfreeHeap(t_heap *aHeap);  

#endif
