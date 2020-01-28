/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "intIncArray.h"
#include "geomHash.h"
#include "nodeList.h"
#include "voidIncArray.h"
#include "pointArray.h"
#include "basicVoidArray.h"
#include "basicPoly.h"
#ifndef __EDGEINCARRAY_H_
#define __EDGEINCARRAY_H_


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

typedef struct t_edgeIArray{
  int gridSize;  
  t_iIncArray pIndex1;  /* store origin of edge */
  t_iIncArray pIndex2;  /* store destination of edge */
	
  struct hashSector *geomHash;  
  struct hashSector **dirtySectors;  
  
  int numDirty;  
  t_vIncArray lines;  
} t_edgeIArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void EAinitArray(t_edgeIArray *anArray, int initialSize);  

void EAinitArray2(t_edgeIArray *anArray, int initialSize);  

void EAgetEdge(t_edgeIArray *anArray, int index, int *pIndex1, int *pIndex2);  

void EAsetEdge(t_edgeIArray *anArray, int index, int pIndex1, int pIndex2);  

void EAsetEdge2(t_edgeIArray *anArray, t_pointArray *pArray, int index, int pIndex1, int pIndex2);  

void EAaddEdge(t_edgeIArray *anArray, int pIndex1, int pIndex2);  

void EAaddEdge2(t_edgeIArray *anArray, t_pointArray *pArray, int pIndex1, int pIndex2);  

nodeList *EAgetPossibleIsects(t_edgeIArray *anArray, int index);  

nodeList *EAgetLinesOfDirtySector(t_edgeIArray *anArray, int index);  

int EAisDirty(t_edgeIArray *anArray, int index);  

int EAhasDirtySectors(t_edgeIArray *anArray);  

void EAcleanSector(t_edgeIArray *anArray, int index);  

int EAnrOfEdges(t_edgeIArray *anArray);  

void EAfreeArray(t_edgeIArray *anArray);  

void EAfreeArray2(t_edgeIArray *anArray);  

int EAvalidateEdge(t_edgeIArray *anArray, int lineNum, int pIndex1, int pIndex2);  

/* reallocate to current size */
void EAfreezeArray(t_edgeIArray *anArray);  

#endif
