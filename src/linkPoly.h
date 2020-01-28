/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicIntArray.h"
#include "basicPoly.h"
#ifndef __LINKPOLY_H_
#define __LINKPOLY_H_


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
  t_intArray array;  
  int startPoint;  
} t_linkPoly;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void LPinit(t_linkPoly *aLinkPoly, int nrOfPoints);  

void LPmarkIndex(t_linkPoly *aLinkPoly, int pIndex, int value);  

int LPgetNext(t_linkPoly *aLinkPoly, int pIndex);  

void LPsetNext(t_linkPoly *aLinkPoly, int pIndex, int nextPIndex);  

int LPgetFirst(t_linkPoly *aLinkPoly);  

/* insert newPIndex after pIndex into the polygon */
void LPaddAfter(t_linkPoly *aLinkPoly, int pIndex, int newPIndex);  

/* add newPIndex to the polygon, such that the edges (origPIndex, newPIndex)
   and (newPIndex, destPIndex) are edges of the polygon */
void LPaddBetween(t_linkPoly *aLinkPoly, int origPIndex, int destPIndex, 
		  int newPIndex);  

int LPsizeOf(t_linkPoly *aLinkPoly);  

void LPcopyPoly(t_linkPoly *sourcePolygon, t_linkPoly *destPolygon);  

void LPfree(t_linkPoly *aLinkPoly);  

void LPtoPoly(t_linkPoly *aLinkPoly, t_polygon *aPolygon);  

void LPfromPoly(t_linkPoly *aLinkPoly, t_polygon *aPolygon);  

#endif

