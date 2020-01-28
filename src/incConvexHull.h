/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "linkPoly.h"

#ifndef __INCCONVEXHULL_H_
#define __INCCONVEXHULL_H_

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
  t_linkPoly next;  
  t_linkPoly prev;  
} t_incCHull;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void IHinit(t_incCHull *iHull, int nrOfPoints);  

void IHgetBridges(t_incCHull *iHull, int newPIndex, int *leftBridge, 
		 int *rightBridge, t_pointArray *pArray);  

void IHupdateBridges(t_incCHull *iHull, int newPIndex, int *leftBridge, 
		 int *rightBridge, t_pointArray *pArray);  

void IHinsert(t_incCHull *iHull, int newPIndex, int leftBridge, 
	     int rightBridge);  

void IHfreeHull(t_incCHull *iHull);  

#endif
