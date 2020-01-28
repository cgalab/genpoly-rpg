/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MtriangPoly.h"
#include "eralloc.h"
#include "triangArray.h"
#include "triangAVL.h"
#include "indexAVL.h"
#include "linkPoly.h"
#include "qhull.h"

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

char qh_version[]= "version 2.4 97/4/2";  

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

void edgeUpdate(t_triangAVL *edgeTree, int pIndex1, int pIndex2, int oldIndex, 
		int newIndex)
{
  t_triangEdge *curEdge;  
  
  /* find the edge */
  curEdge = TAsearch(edgeTree, pIndex1, pIndex2);  

  /* check which information corrresponds to the old triangle information */
  if (curEdge->triangIndex1 == oldIndex)
    /* update the information */
    curEdge->triangIndex1 = newIndex;  
  else if (curEdge->triangIndex2 == oldIndex)
    /* update the information */
    curEdge->triangIndex2 = newIndex;  
}


void edgeFlips(t_pointArray *pArray, t_triangAVL *edgeTree, 
	       t_triangArray *tArray)
{
  int count;  
  t_triangle triang1, triang2;  
  int tIndex1, tIndex2;  
  int oldP1, oldP2;  
  int newP1, newP2;  
  t_triangEdge *curEdge;  

  for (count=1;  count<PAnrOfPoints(pArray)*FLIP_FACTOR;  count++)
    {
      /* first, pick a random edge */
      curEdge = TArandomGet(edgeTree);  
      oldP1 = curEdge->pIndex1;  
      oldP2 = curEdge->pIndex2;  
      tIndex1 = curEdge->triangIndex1;  
      tIndex2 = curEdge->triangIndex2;  
      
      /* do we have a triangle at the border? */
      /* have we already fliped this edge? */
      if ((tIndex1 != -1) && (tIndex2 != -1) && curEdge->marker)
	{
	  /* get the triangles adjacent to this edge */
	  triang1 = TRgetTriang(tArray, tIndex1);  
	  triang2 = TRgetTriang(tArray, tIndex2);  

	  /* find the vertex which is not part of the edge in the
	     first triangle */
	  if ((triang1.vertex[0] != oldP1) && (triang1.vertex[0] != oldP2))
	    newP1 = triang1.vertex[0];  
	  else if ((triang1.vertex[1] != oldP1) && 
		   (triang1.vertex[1] != oldP2))
	    newP1 = triang1.vertex[1];  
	  else 
	    newP1 = triang1.vertex[2];  

	  /* find the vertex which is not part of the edge in the
	     second triangle */
	  if ((triang2.vertex[0] != oldP1) && (triang2.vertex[0] != oldP2))
	    newP2 = triang2.vertex[0];  
	  else if ((triang2.vertex[1] != oldP1) && 
		   (triang2.vertex[1] != oldP2))
	    newP2 = triang2.vertex[1];  
	  else 
	    newP2 = triang2.vertex[2];  

	  /* next, check whether a flip is possible. */
	  if (PAisOnLine(pArray, newP1, newP2, oldP1)*
	      PAisOnLine(pArray, newP1, newP2, oldP2) == -1)
	    {
	      /* replace the edge */
	      TAdelElem(edgeTree, oldP1, oldP2);  
	      TAaddElem(edgeTree, newP1, newP2, tIndex1, FALSE);  
	      TAaddElem(edgeTree, newP1, newP2, tIndex2, FALSE);  
	      
	      /* replace the original triangles */
	      triang1.vertex[0] = newP1;  
	      triang1.vertex[1] = newP2;  
	      triang1.vertex[2] = oldP1;  
	      
	      triang2.vertex[0] = newP1;  
	      triang2.vertex[1] = newP2;  
	      triang2.vertex[2] = oldP2;  
	      
	      /* modify the triangle array */
	      TRsetTriang(tArray, tIndex1, triang1);  
	      TRsetTriang(tArray, tIndex2, triang2);  
	      
	      /* update information for the remaining four the edges */
	      edgeUpdate(edgeTree, newP1, oldP1, tIndex2, tIndex1);  
	      edgeUpdate(edgeTree, newP2, oldP1, tIndex2, tIndex1);  
	      edgeUpdate(edgeTree, newP1, oldP2, tIndex1, tIndex2);  
	      edgeUpdate(edgeTree, newP2, oldP2, tIndex1, tIndex2);  
	    }
	}
    }
}


void addTriangle(t_triangAVL *edgeTree, t_indexAVL *indexTree, int curIndex, 
		 int pIndex1, int pIndex2)
{
  t_triangEdge *curEdge;  
  
  curEdge = TAsearch(edgeTree, pIndex1, pIndex2);  
  
  if (curEdge->triangIndex1 != curIndex)
    ITaddElem(indexTree, curEdge->triangIndex1);  
  if ((curEdge->triangIndex2 != -1) && (curEdge->triangIndex2 != curIndex))
    ITaddElem(indexTree, curEdge->triangIndex2);  
}


int addIfFeasible(t_linkPoly *linkPoly, t_triangAVL *edgeTree, 
		  t_indexAVL *indexTree, t_triangle triangle, 
		  int curIndex, double threshold)
{
  int result = 0;  
  int v0, v1, v2;  

  v0 = triangle.vertex[0];  
  v1 = triangle.vertex[1];  
  v2 = triangle.vertex[2];  

  /* first, check for collapsing the triangle, i.e. replacing two 
     consectutive edges through a single edge */ 
  /* v0 -> v1 -> v2 */
  if ((LPgetNext(linkPoly, v0) == v1) && (LPgetNext(linkPoly, v1) == v2))  
    {
      if (drand48() > threshold)	
	{
	  result = -1;  
	  LPsetNext(linkPoly, v0, v2);  
	  LPmarkIndex(linkPoly, v1, RPG_USED);  
	  addTriangle(edgeTree, indexTree, curIndex, v0, v2);  
	}
    }
  /* v1 -> v2 -> v0 */
  else if ((LPgetNext(linkPoly, v1) == v2) && (LPgetNext(linkPoly, v2) == v0))
    {
      if (drand48() > threshold) 
	{
	  result = -1;  
	  LPsetNext(linkPoly, v1, v0);  
	  LPmarkIndex(linkPoly, v2, RPG_USED);  
	  addTriangle(edgeTree, indexTree, curIndex, v0, v2);  
	}
    }
  /* v2 -> v0 -> v1 */
  else if ((LPgetNext(linkPoly, v2) == v0) && (LPgetNext(linkPoly, v0) == v1))
    {
      if (drand48() > threshold) 
	{
	  result = -1;  
	  LPsetNext(linkPoly, v2, v1);  
	  LPmarkIndex(linkPoly, v0, RPG_USED);  
	  addTriangle(edgeTree, indexTree, curIndex, v2, v1);  
	}
    }
  /* v2 -> v1 -> v0 */
  else if ((LPgetNext(linkPoly, v2) == v1) && (LPgetNext(linkPoly, v1) == v0))
    {
      if (drand48() > threshold) 
	{
	  result = -1;  
	  LPsetNext(linkPoly, v2, v0);  
	  LPmarkIndex(linkPoly, v1, RPG_USED);  
	  addTriangle(edgeTree, indexTree, curIndex, v2, v0);  
	}
    }
  /* v1 -> v0 -> v2 */
  else if ((LPgetNext(linkPoly, v1) == v0) && (LPgetNext(linkPoly, v0) == v2))
    {
      if (drand48() > threshold) 
	{
	  result = -1;  
	  LPsetNext(linkPoly, v1, v2);  
	  LPmarkIndex(linkPoly, v0, RPG_USED);  
	  addTriangle(edgeTree, indexTree, curIndex, v1, v2);  
	}
    }
  /* v0 -> v2 -> v1 */
  else if ((LPgetNext(linkPoly, v0) == v2) && (LPgetNext(linkPoly, v2) == v1))
    {
      if (drand48() > threshold) 
	{
	  result = -1;  
	  LPsetNext(linkPoly, v0, v1);  
	  LPmarkIndex(linkPoly, v2, RPG_USED);  
	  addTriangle(edgeTree, indexTree, curIndex, v0, v1);  
	}
    }
  /* next check expanding: replace a single edge with two edges */
  /* v0 -> v1 */
  else if ((LPgetNext(linkPoly, v0) == v1) && 
	   (LPgetNext(linkPoly, v2) == RPG_UNDEFINED))
    {
      result = +1;  
      LPaddAfter(linkPoly, v0, v2);  
      addTriangle(edgeTree, indexTree, curIndex, v0, v2);  
      addTriangle(edgeTree, indexTree, curIndex, v1, v2);  
    }
  /* v1 -> v2 */
  else if ((LPgetNext(linkPoly, v1) == v2) && 
	   (LPgetNext(linkPoly, v0) == RPG_UNDEFINED))
    {
      result = +1;  
      LPaddAfter(linkPoly, v1, v0);  
      addTriangle(edgeTree, indexTree, curIndex, v1, v0);  
      addTriangle(edgeTree, indexTree, curIndex, v2, v0);  
    }
  /* v2 -> v0 */
  else if ((LPgetNext(linkPoly, v2) == v0) && 
	   (LPgetNext(linkPoly, v1) == RPG_UNDEFINED))
    {
      result = +1;  
      LPaddAfter(linkPoly, v2, v1);  
      addTriangle(edgeTree, indexTree, curIndex, v0, v1);  
      addTriangle(edgeTree, indexTree, curIndex, v2, v1);  
    }
  /* v0 -> v2 */
  else if ((LPgetNext(linkPoly, v0) == v2) && 
	   (LPgetNext(linkPoly, v1) == RPG_UNDEFINED))
    {
      result = +1;  
      LPaddAfter(linkPoly, v0, v1);  
      addTriangle(edgeTree, indexTree, curIndex, v0, v1);  
      addTriangle(edgeTree, indexTree, curIndex, v2, v1);  
    }
  /* v2 -> v1 */
  else if ((LPgetNext(linkPoly, v2) == v1) && 
	   (LPgetNext(linkPoly, v0) == RPG_UNDEFINED))
    {
      result = +1;  
      LPaddAfter(linkPoly, v2, v0);  
      addTriangle(edgeTree, indexTree, curIndex, v1, v0);  
      addTriangle(edgeTree, indexTree, curIndex, v2, v0);  
    }
  /* v1 -> v0 */
  else if ((LPgetNext(linkPoly, v1) == v0) && 
	   (LPgetNext(linkPoly, v2) == RPG_UNDEFINED))
    {
      result = +1;  
      LPaddAfter(linkPoly, v1, v2);  
      addTriangle(edgeTree, indexTree, curIndex, v1, v2);  
      addTriangle(edgeTree, indexTree, curIndex, v0, v2);  
    }
  
  return(result);  
}


void MtriangPoly(t_pointList *pList, t_polygon *aPolygon, t_pointArray *pArray, 
		 int nrOfVertices, int nrOfPolys, FILE *outFile)
{
  int exitcode;  
  int count, pcount, curIndex, polyCount;  
  int curNrOfVertices, maxNrOfVertices;  
  int nrOfPoints;  
  coordT *points;  
  char *options = "qhull d Qs Ft";  
  t_triangArray triangArray, auxTArray;  
  t_triangAVL edgeTree;  
  t_indexAVL indexTree;  
  t_triangle curTriang;  
  t_linkPoly linkPoly, maxPoly;  
  t_pointArray auxPoints;  
  double threshold;  

  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfVertices = nrOfPoints;  

  if ((nrOfVertices >= 3)  && (nrOfPoints > 3))
    {
      PAinitArray(&auxPoints, nrOfPoints);  
      PAcopyArray(pArray, &auxPoints);  
      
      TRinitEmptyArray(&triangArray);  

      /* first, triangulate the point set */

      /* I don't know how the following works, and as long as it does, I don't
	 especially care. If it doesn't, we are in deep trouble! :-)

	 The following was copied from the user frontend unix.c distributed 
	 with the quickhull algorithm 
     
	 We generate our datastructure by replacing printfs with access routines
	 for our data structures */
  
      qh_init_A (stdin, stdout, stderr, 0, NULL);  
      exitcode = setjmp (qh errexit);  
  
      if (!exitcode) {
	  /* allocate the memory for the points */
	  points = (double *)ercalloc(nrOfPoints, 2*sizeof(coordT));  
	  for (count=0;  count<nrOfPoints;  count++)   {
	      *(points+2*count) = PAgetPoint(&auxPoints, count+1).x;  
	      *(points+2*count+1) = PAgetPoint(&auxPoints, count+1).y;  
	    }

	  qh_initflags(options);  
	  qh PROJECTdelaunay = TRUE;  
	  qh_init_B(points, nrOfPoints, 2, FALSE);  
	  qh_qhull();  

	  qh_check_output();  
	  qh_produce_output(&triangArray);       /**/
	  /* qh_produce_output();  /**/
	  if (qh VERIFYoutput && !qh FORCEoutput && !qh STOPpoint && 
	      !qh STOPcone)
	    qh_check_points();  

	  qh_freeqhull( True);  
	  erfree(points);  

	  /* end of copy */
	  TRfreezeArray(&triangArray);  
	}

      /* init an emtpy maximum polygon */
      /* init a linked polygon */
      LPinit(&linkPoly, PAnrOfPoints(&auxPoints));  
      LPinit(&maxPoly, PAnrOfPoints(&auxPoints));  
      TRinitArray(&auxTArray, TRnrOfTriangs(&triangArray));  

      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	{
	  maxNrOfVertices = 0;  
	  
	  for (pcount=0;  pcount<DF_COUNT;  pcount++)
	    { 
	      /* copy the original triangArray */
	      TRcopyArray(&triangArray, &auxTArray);  

	      /* first, create the AVL tree containing all edges */
	      TAinitTree(&edgeTree);  
	      ITinitTree(&indexTree);  
	      
	      for (count=1;  count<=TRnrOfTriangs(&auxTArray);  count++)
		{
		  curTriang = TRgetTriang(&auxTArray, count);  
		  TAaddElem(&edgeTree, curTriang.vertex[0], 
			    curTriang.vertex[1], count, TRUE);  
		  TAaddElem(&edgeTree, curTriang.vertex[0], 
			    curTriang.vertex[2], count, TRUE);  
		  TAaddElem(&edgeTree, curTriang.vertex[1], 
			    curTriang.vertex[2], count, TRUE);  
		}
	       
      	      /* next, perform the edge flips */
	      edgeFlips(pArray, &edgeTree, &auxTArray);  

	      /* second, create the polygon by selecting the triangles */ 
	      /* init an empty polygon */
	      for (count=1;  count<=PAnrOfPoints(&auxPoints);  count++)
		LPmarkIndex(&linkPoly, count, RPG_UNDEFINED);  
	      
	      /* choose a start Polygon */
	      curIndex = randomInt(1, TRnrOfTriangs(&auxTArray));  
	      curTriang = TRgetTriang(&auxTArray, curIndex);  
	      LPsetNext(&linkPoly, curTriang.vertex[0], curTriang.vertex[1]);  
	      LPsetNext(&linkPoly, curTriang.vertex[1], curTriang.vertex[2]);  
	      LPsetNext(&linkPoly, curTriang.vertex[2], curTriang.vertex[0]);  
	      
	      if (maxNrOfVertices == 0)
		{
		  LPsetNext(&maxPoly, curTriang.vertex[0], 
			    curTriang.vertex[1]);  
		  LPsetNext(&maxPoly, curTriang.vertex[1], 
			    curTriang.vertex[2]);  
		  LPsetNext(&maxPoly, curTriang.vertex[2], 
			    curTriang.vertex[0]);  
		  maxNrOfVertices = 3;  
		}

	      /* add all adjoining triangles to the index tree */
	      addTriangle(&edgeTree, &indexTree, curIndex, curTriang.vertex[0], 
			  curTriang.vertex[1]);  
	      addTriangle(&edgeTree, &indexTree, curIndex, curTriang.vertex[0], 
			  curTriang.vertex[2]);  
	      addTriangle(&edgeTree, &indexTree, curIndex, curTriang.vertex[1], 
			  curTriang.vertex[2]);  
	      
	      curNrOfVertices = 3;  
	      /* now loop while the index tree is not empty and while
		 we have not reached the required number of points */
	      threshold = ((double)(nrOfPoints-nrOfVertices) / 
			   (double)nrOfPoints);  
	      
	      while ((curNrOfVertices < nrOfVertices) && 
		     !(ITisEmpty(&indexTree)))
		{
		  /* get a random triangle */
		  curIndex = ITrandomDelGet(&indexTree);  
		  curTriang = TRgetTriang(&auxTArray, curIndex);  
		  
		  /* add the triangle if feasible */
		  curNrOfVertices += 
		    addIfFeasible(&linkPoly, &edgeTree, &indexTree, curTriang, 
				  curIndex, threshold);  
		  
		  if (curNrOfVertices > maxNrOfVertices)
		    {
		      maxNrOfVertices = curNrOfVertices;  
		      LPcopyPoly(&linkPoly, &maxPoly);  
		    }
		}
	      TAfreeTElem(&edgeTree);  
	      ITfreeTElem(&indexTree);  
	    }
	  
	  /* all that is left is to transform the polygon to a normal
	     polygon */
	  curIndex = LPgetFirst(&maxPoly);  
	  BPfree(aPolygon);  
	  BPinit(aPolygon, maxNrOfVertices);  
	  PAfreeArray(pArray);  
	  PAinitArray(pArray, maxNrOfVertices);  
  
	  /* first, copy the points we need */
	  for (count=1;  count<=maxNrOfVertices;  count++)
	    {
	      PAsetPoint(pArray, count, PAgetPoint(&auxPoints, curIndex));  
	      curIndex = LPgetNext(&maxPoly, curIndex);  
	    }
	  PAsortArray(pArray);  
	  PAcreateList(pArray, pList);  

	  /* next, get the correct indices */
	  curIndex =  LPgetFirst(&maxPoly);  
	  for (count=1;  count<=maxNrOfVertices;  count++)
	    {
	      BPsetPIndex(aPolygon, count, 
			  PAgetPIndex(pArray, PAgetPoint(&auxPoints, curIndex)));  
	      curIndex = LPgetNext(&maxPoly, curIndex);  
	    }
	  /* normalize the polygon */
	  BPnormalize(aPolygon, pArray);  
	  
	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
 	}
  
      /* delete the AVL tree and the array */
      TRfreeArray(&triangArray);    		       
      TRfreeArray(&auxTArray);  
      LPfree(&linkPoly);  
      LPfree(&maxPoly);  
      PAfreeArray(&auxPoints);  
    }
  else if (nrOfPoints == 3)
    {
      
      BPfree(aPolygon);  
      BPinit(aPolygon, nrOfPoints);  
      BPsetPIndex(aPolygon, 1, 1);  
      BPsetPIndex(aPolygon, 2, 2);  
      BPsetPIndex(aPolygon, 3, 3);  
      BPnormalize(aPolygon, pArray);  
      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	if (outFile != NULL)
	  BPwriteToFile(aPolygon, outFile);  
    }
}
