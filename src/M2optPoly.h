/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h" 

#ifndef __M2OPTPOLY_H_
#define __M2OPTPOLY_H_

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

typedef struct t_isectInfoS t_isectInfo;  
typedef t_isectInfo *t_isectInfoP;  

struct t_singleEdge{
  t_isectInfo *next, *prev;  
  /* We assume that always pIndex1 < pIndex2! */
  int pIndex1, pIndex2;  
};  

struct t_isectInfoS{
  struct t_singleEdge edge[2];  
  t_isectInfo *prev, *next;  
};  

typedef struct{
  t_isectInfo *first;  
  int nrOfElems;  
} t_isectList;  
  
/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

#ifdef DO_STATS
void MR2optPoly();  

void MG2optPoly(unsigned long *tries);  
#endif

void M2optPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
	       FILE *outFile);  

void Mdo2optMoves(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPoints);  

#endif
