/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "edgeIncArray.h"
#include "basicIntArray.h"

#ifndef __FORCEDEDGE_H_
#define __FORCEDEDGE_H_

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
  t_intArray pointForced;  
  t_intArray forcedEdges;  
} t_forcedEdges;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void initFEdge(t_forcedEdges *fEdge, int nrOfPoints);  

void freeFEdge(t_forcedEdges *fEdge);  

/* force the edge if possible, return false if the 
   there exist already 2 forced edges for either 
   pIndex1 or pIndex2 */
int forceEdge(t_forcedEdges *fEdge, t_edgeIArray *curForced, int pIndex1, 
	      int pIndex2, int curStep);  

/* check whether there exists a forced edge from orig
   to any other edge than used, if yes, return that edge */
int forcedEdge(t_forcedEdges *fEdge, int orig, t_intArray *usedPoints);  

int isForced(t_forcedEdges *fEdge, int pIndex1, int pIndex2);  

int nrOfForced(t_forcedEdges *fEdge, int pIndex);  

int unforce(t_forcedEdges *fEdge, t_edgeIArray *curForced, int curStep);  

#endif
