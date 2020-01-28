/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h"
#include "triangArray.h"
#include "triangAVL.h"
#include "indexAVL.h"
#include "linkPoly.h"

#ifndef __MTRIANGPOLY_H_
#define __MTRIANGPOLY_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#define FLIP_FACTOR    50

#define DF_COUNT       10
/********************************************************************/
/*                                                                  */
/* Data Types                                                       */
/*                                                                  */
/********************************************************************/

  
/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void edgeFlips(t_pointArray *pArray, t_triangAVL *edgeTree, 
	       t_triangArray *tArray);  

void addTriangle(t_triangAVL *edgeTree, t_indexAVL *indexTree, int curIndex, 
		 int pIndex1, int pIndex2);  

int addIfFeasible(t_linkPoly *linkPoly, t_triangAVL *edgeTree, 
		  t_indexAVL *indexTree, t_triangle triangle, 
		  int curIndex, double threshold);  

void MtriangPoly(t_pointList *pList, t_polygon *aPolygon, t_pointArray *pArray, 
		 int nrOfVertices, int nrOfPolys, FILE *outFile);  

#endif
