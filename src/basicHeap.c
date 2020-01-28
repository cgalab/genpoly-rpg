/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicHeap.h"
#include "basicDefs.h"
#include "eralloc.h"

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void BHinitHeap(t_heap *aHeap)
{
  aHeap->top = NULL;  
}


struct t_heapElem *BHmerge(struct t_heapElem *heap1, struct t_heapElem *heap2, 
			   int compProc(caddr_t, caddr_t));  


struct t_heapElem *BHdoMerge(struct t_heapElem *heap1, struct t_heapElem *heap2, 
			     int compProc(caddr_t, caddr_t))
{
  struct t_heapElem *mHeap;  

  if (heap1->left == NULL)
    heap1->left = heap2;  
  else 
    {
      heap1->right = BHmerge(heap1->right, heap2, compProc);  
      if (heap1->left->pathLength < heap1->right->pathLength)
	{
	  mHeap = heap1->left;  
	  heap1->left = heap1->right;  
	  heap1->right = mHeap;  
	}
      
      heap1->pathLength = heap1->right->pathLength+1;  
    }
  return(heap1);  
}


struct t_heapElem *BHmerge(struct t_heapElem *heap1, struct t_heapElem *heap2, 
			   int compProc(caddr_t, caddr_t))
{
  struct t_heapElem *mHeap;  

  if (heap1 == NULL)
    mHeap = heap2;  
  else if (heap2 == NULL)
    mHeap = heap1;  
  else if (compProc(heap1->elem, heap2->elem) == 1)
    mHeap = BHdoMerge(heap1, heap2, compProc);  
  else
    mHeap = BHdoMerge(heap2, heap1, compProc);  

  return(mHeap);  
}


void BHaddElem(t_heap *aHeap, caddr_t elem, int compProc(caddr_t, caddr_t))
{
  struct t_heapElem *newElem;  

  newElem = (struct t_heapElem *) eralloc(sizeof(struct t_heapElem));  

  newElem->elem = elem;  
  newElem->right = NULL;  
  newElem->left = NULL;  
  newElem->pathLength = 0;  

  aHeap->top = BHmerge(newElem, aHeap->top, compProc);  
}


caddr_t BHviewTop(t_heap *aHeap)
{
  return(aHeap->top->elem);  
}


caddr_t BHgetTop(t_heap *aHeap, int compProc(caddr_t, caddr_t))
{
  struct t_heapElem *leftHeap, *rightHeap;  
  caddr_t elem;  

  elem = aHeap->top->elem;  
  leftHeap = aHeap->top->left;  
  rightHeap = aHeap->top->right;  

  erfree(aHeap->top);  

  aHeap->top = BHmerge(leftHeap, rightHeap, compProc);  

  return(elem);  
}


int BHisEmpty(t_heap *aHeap)
{
  return((aHeap->top) == NULL);  
}

void BHfreeIt(struct t_heapElem *elem)
{
  if (elem != NULL)
    {
      BHfreeIt(elem->left);  
      BHfreeIt(elem->right);  
      erfree(elem);  
    }
}

void BHfreeHeap(t_heap *aHeap)
{
  BHfreeIt(aHeap->top);  
}
  


