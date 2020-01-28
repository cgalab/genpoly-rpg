/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "pointArray.h" 
#include "convexHull.h"
#include "isectArray.h"
#include "edgeIncArray.h"
#include "forcedEdge.h"

#ifndef __MSEARCHPOLY_H_
#define __MSEARCHPOLY_H_

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
  
/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
#ifdef DO_STATS
void MRsearchPoly();  

void MGsearchPoly(unsigned long *tries);  
#endif


void MsearchPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		 FILE *outFile);  

int rule1(t_intArray *pointEdges, t_intArray *usedPoints);  

int rule2(t_intArray *pointEdges, t_intArray *usedPoints, 
	  t_intArray *edgeMarker, int curPoint);  

int rule4(t_intArray *edgeMarker, t_intArray *pointEdges, t_intArray *usedPoints, 
	  t_forcedEdges *fEdge, t_isectArray *iArray, t_edgeIArray *markedEdges, 
	  t_edgeIArray *forcedEdges, int curStep);  

int hullRule(int *leftGuard, int *rightGuard, int newPoint, t_convexHull *cHull);  

int ccwRule(t_pointArray *pArray, t_polygon *aPolygon, 
	    int newPoint, int *leftPoints);  

int markEdges(t_intArray *edgeMarker, t_intArray *pointEdges, 
	      t_edgeIArray *markedEdges, int pIndex, int curStep);  

int markIsects(t_intArray *edgeMarker, t_intArray *pointEdges, 
	       t_edgeIArray *markedEdges, t_isectArray *iArray, 
	       int pIndex1, int pIndex2, int curStep);  

void freeEdges(t_intArray *edgeMarker, t_intArray *pointEdges, 
	       t_edgeIArray *markedEdges, int curStep);  

int isFree(t_intArray *edgeMarker, int pIndex1, int pIndex2, int nrOfPoints);  

int markSingle(t_intArray *edgeMarker, t_intArray *pointEdges, 
	       t_edgeIArray *markedEdges, int pIndex1, int pIndex2, int curStep);  

#endif







