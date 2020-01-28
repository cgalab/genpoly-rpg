/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "linkPoly.h"

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

void LPinit(t_linkPoly *aLinkPoly, int nrOfPoints)
{
  IAinit(&(aLinkPoly->array), nrOfPoints);  
  aLinkPoly->startPoint = -1;  
}


void LPmarkIndex(t_linkPoly *aLinkPoly, int pIndex, int value)
{
  IAsetInt(&(aLinkPoly->array), pIndex, value);  
}


int LPgetNext(t_linkPoly *aLinkPoly, int pIndex)
{
  return(IAgetInt(&(aLinkPoly->array), pIndex));  
}


void LPsetNext(t_linkPoly *aLinkPoly, int pIndex, int nextPIndex)
{
  LPmarkIndex(aLinkPoly, pIndex, nextPIndex);  
  aLinkPoly->startPoint = pIndex;  
}


/* insert newPIndex after pIndex into the polygon */
void LPaddAfter(t_linkPoly *aLinkPoly, int pIndex, int newPIndex)
{
  LPsetNext(aLinkPoly, newPIndex, LPgetNext(aLinkPoly, pIndex));  
  LPsetNext(aLinkPoly, pIndex, newPIndex);  
  aLinkPoly->startPoint = newPIndex;  
}

/* add newPIndex to the polygon, such that the edges (origPIndex, newPIndex)
   and (newPIndex, destPIndex) are edges of the polygon */
void LPaddBetween(t_linkPoly *aLinkPoly, int origPIndex, int destPIndex, 
		  int newPIndex)
{
  LPsetNext(aLinkPoly, newPIndex, destPIndex);  
  LPsetNext(aLinkPoly, origPIndex, newPIndex);  
  aLinkPoly->startPoint = newPIndex;  
}

int LPsizeOf(t_linkPoly *aLinkPoly)
{
  return(IAsizeOf(&(aLinkPoly->array)));  
}

void LPfree(t_linkPoly *aLinkPoly)
{
  IAfree(&(aLinkPoly->array));  
} 

int LPgetFirst(t_linkPoly *aLinkPoly)
{
  return(aLinkPoly->startPoint);  
}

void LPtoPoly(t_linkPoly *aLinkPoly, t_polygon *aPolygon)
{
  int curIndex;  
  int count;  

  BPinit(aPolygon, LPsizeOf(aLinkPoly));  

  if (LPsizeOf(aLinkPoly) > 0)
    {
      curIndex = 1;  
      
      /* search the smallest index that is part of the actual polygon
       */ 
      while (LPgetNext(aLinkPoly, curIndex) == -1)
	curIndex++;  
      
      for (count = 1;  count <= BPsizeOf(aPolygon);  count++)
	{
	  BPsetPIndex(aPolygon, count, curIndex);  
	  curIndex = LPgetNext(aLinkPoly, curIndex);  
	}
    }
}


void LPfromPoly(t_linkPoly *aLinkPoly, t_polygon *aPolygon)
{
  int count;  

  LPinit(aLinkPoly, BPsizeOf(aPolygon));  
  for (count = 1;  count < BPsizeOf(aPolygon);  count++)
    {
      LPsetNext(aLinkPoly, BPgetPIndex(aPolygon, count), 
		BPgetPIndex(aPolygon, count+1));  
    }  
  LPsetNext(aLinkPoly, BPgetPIndex(aPolygon, BPsizeOf(aPolygon)), 
		BPgetPIndex(aPolygon, 1));  
}


void LPcopyPoly(t_linkPoly *sourcePoly, t_linkPoly *destPoly)
{
  IAcopyArray(&(sourcePoly->array), &(destPoly->array));  
  destPoly->startPoint = sourcePoly->startPoint;  
}




