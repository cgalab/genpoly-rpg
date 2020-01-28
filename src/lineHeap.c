/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "lineInfo.h"
#include "lineHeap.h"

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

void LHinitHeap(t_lineHeap *aHeap, int minMax)
{
  BHinitHeap(&(aHeap->heap));  
  aHeap->minMax = minMax;  
}

void LHaddElem(t_lineHeap *aHeap, t_lInfoP elem)
{
  if (aHeap->minMax == HMAX)
    BHaddElem(&(aHeap->heap), (caddr_t) elem, LIcompMaxLines);  
  else
    BHaddElem(&(aHeap->heap), (caddr_t) elem, LIcompMinLines);  
}

t_lInfoP LHviewTop(t_lineHeap *aHeap)
{
  return((t_lInfoP) BHviewTop(&(aHeap->heap)));  
}

t_lInfoP LHgetTop(t_lineHeap *aHeap)
{
  if (aHeap->minMax == HMAX)
    return((t_lInfoP) BHgetTop(&(aHeap->heap), LIcompMaxLines));  
  else
    return((t_lInfoP) BHgetTop(&(aHeap->heap), LIcompMinLines));  
}

int LHisEmpty(t_lineHeap *aHeap)
{
  return(BHisEmpty(&(aHeap->heap)));  
}

void LHfreeHeap(t_lineHeap *aHeap)
{
  BHfreeHeap(&(aHeap->heap));  
}





