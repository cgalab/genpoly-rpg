/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "incConvexHull.h"

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

void IHinit(t_incCHull *iHull, int nrOfPoints)
{
  LPinit(&(iHull->next), nrOfPoints);  
  LPinit(&(iHull->prev), nrOfPoints);  
}

void IHgetBridges(t_incCHull *iHull, int newPIndex, int *leftBridge, 
		 int *rightBridge, t_pointArray *pArray)
{
  int first, curIndex;  

  first = LPgetFirst(&(iHull->next));  

  (*leftBridge) = first;  
  (*rightBridge) = first;  
  
  curIndex = LPgetNext(&(iHull->next), first);  
  
  while (curIndex != first)
    {
      if (isOnLine(PAgetPoint(pArray, newPIndex), 
		   PAgetPoint(pArray, (*rightBridge)), 
		   PAgetPoint(pArray, curIndex)) == -1)
	(*rightBridge) = curIndex;  
      else if (isOnLine(PAgetPoint(pArray, newPIndex), 
			PAgetPoint(pArray, (*leftBridge)), 
			PAgetPoint(pArray, curIndex)) == 1)
	(*leftBridge) = curIndex;  

      curIndex = LPgetNext(&(iHull->next), curIndex);  
    }
}

void IHupdateBridges(t_incCHull *iHull, int newPIndex, int *leftBridge, 
		 int *rightBridge, t_pointArray *pArray)
{
  /* first, update the left bridge */
  while (isOnLine(PAgetPoint(pArray, newPIndex), 
		  PAgetPoint(pArray, (*leftBridge)), 
		  PAgetPoint(pArray, 
			     LPgetNext(&(iHull->next), (*leftBridge)))) != -1)
    {
      (*leftBridge) = LPgetNext(&(iHull->next), (*leftBridge));  
    }
  
  /* now update the right bridge */
  while (isOnLine(PAgetPoint(pArray, newPIndex), 
		  PAgetPoint(pArray, (*rightBridge)), 
		  PAgetPoint(pArray, 
			     LPgetNext(&(iHull->prev), (*rightBridge)))) != 1)
    {
      (*rightBridge) = LPgetNext(&(iHull->prev), (*rightBridge));  
    }
}

void IHinsert(t_incCHull *iHull, int newPIndex, int leftBridge, 
	     int rightBridge)
{
  LPaddBetween(&(iHull->next), leftBridge, rightBridge, newPIndex);  
  LPaddBetween(&(iHull->prev), rightBridge, leftBridge, newPIndex);  
}

void IHfreeHull(t_incCHull *iHull)
{
  LPfree(&(iHull->next));  
  LPfree(&(iHull->prev));  
}
