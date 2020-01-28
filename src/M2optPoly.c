/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "M2optPoly.h"
#include "eralloc.h"
#include "linkPoly.h"

/*****************************************************************************/
/*                                                                           */
/* Constants and the like                                                    */
/*                                                                           */
/*****************************************************************************/

#ifdef DO_STATS
static unsigned long optCount;  
#endif


/*****************************************************************************/
/*                                                                           */
/* Data Types                                                                */
/*                                                                           */
/*****************************************************************************/



/*****************************************************************************/
/*                                                                           */
/* Procedures and functions                                                  */
/*                                                                           */
/*****************************************************************************/
#ifdef DO_STATS
void MR2optPoly()
{
  optCount = 0;  
}


void MG2optPoly(unsigned long *tries)
{
  *tries = optCount;  
}
#endif


t_isectInfo **getElem(t_isectInfo **isectArray, int pIndex1, int pIndex2, 
		     int nrOfPoints)
{
  int location, i1, i2;  

  i1 = MIN(pIndex1, pIndex2);  
  i2 = MAX(pIndex1, pIndex2);  
  
  location = (2*nrOfPoints - i1)*(i1 - 1)/2 + i2 - i1 - 1;  
  return((isectArray+location));  
}

void isectAdd(t_isectInfo **isectArray, t_isectList *isectList, 
	     t_isectInfo *newIsect, int nrOfPoints)
{
  t_isectInfo **pointList;  

  /* add to first list */
  pointList = getElem(isectArray, newIsect->edge[0].pIndex1, 
		      newIsect->edge[0].pIndex2, nrOfPoints);  
  newIsect->edge[0].next = (*pointList);  
  newIsect->edge[0].prev = NULL;  

  if ((*pointList) != NULL)
    {
      if (((*pointList)->edge[0].pIndex1 == newIsect->edge[0].pIndex1) &&
	  ((*pointList)->edge[0].pIndex2 == newIsect->edge[0].pIndex2))
	(*pointList)->edge[0].prev = newIsect;  
      else
	(*pointList)->edge[1].prev = newIsect;  
    }

  (*pointList) = newIsect;  

  /* add to second list */
  pointList = getElem(isectArray, newIsect->edge[1].pIndex1, 
		      newIsect->edge[1].pIndex2, nrOfPoints);  
  newIsect->edge[1].next = (*pointList);  
  newIsect->edge[1].prev = NULL;  
  
  if ((*pointList) !=NULL)
    {
      if (((*pointList)->edge[0].pIndex1 == newIsect->edge[1].pIndex1) &&
	  ((*pointList)->edge[0].pIndex2 == newIsect->edge[1].pIndex2))
	(*pointList)->edge[0].prev = newIsect;  
      else
	(*pointList)->edge[1].prev = newIsect;  
    }
    
  (*pointList) = newIsect;  
    
    
  /* add to global list, add element as head of the list */
  newIsect->prev = NULL;  
  newIsect->next = isectList->first;  
  if (isectList->first != NULL)
    isectList->first->prev = newIsect;  
  
  /* make the new element the new head */
  isectList->first = newIsect;  
  isectList->nrOfElems++;  
}

void isectDel(t_isectInfo **isectArray, t_isectList *isectList, 
	     t_isectInfo *delIsect, int nrOfPoints)
{
  t_isectInfo *auxIsect;  
  t_isectInfo **pointList;  

  /* delete from first list */
  pointList = getElem(isectArray, delIsect->edge[0].pIndex1, 
		      delIsect->edge[0].pIndex2, nrOfPoints);  
  if (delIsect->edge[0].prev != NULL)
    {
      auxIsect = delIsect->edge[0].prev;  
      if (auxIsect->edge[0].next == delIsect)
	auxIsect->edge[0].next = delIsect->edge[0].next;  
      else
	auxIsect->edge[1].next = delIsect->edge[0].next;  
    }
  else
    (*pointList) = delIsect->edge[0].next;  
  
  if (delIsect->edge[0].next != NULL)
    {
      auxIsect = delIsect->edge[0].next;  
      if (auxIsect->edge[0].prev == delIsect)
	auxIsect->edge[0].prev = delIsect->edge[0].prev;  
      else
	auxIsect->edge[1].prev = delIsect->edge[0].prev;  
    }
  

  /* delete from second list */
  pointList = getElem(isectArray, delIsect->edge[1].pIndex1, 
		      delIsect->edge[1].pIndex2, nrOfPoints);  
  if (delIsect->edge[1].prev != NULL)
    {
      auxIsect = delIsect->edge[1].prev;  
      if ((auxIsect->edge[0].pIndex1 == delIsect->edge[1].pIndex1) &&
	  (auxIsect->edge[0].pIndex2 == delIsect->edge[1].pIndex2))
	auxIsect->edge[0].next = delIsect->edge[1].next;  
      else
	auxIsect->edge[1].next = delIsect->edge[1].next;  
    }
  else
    (*pointList) = delIsect->edge[1].next;  
  
  if (delIsect->edge[1].next != NULL)
    {
      auxIsect = delIsect->edge[1].next;  
      if ((auxIsect->edge[0].pIndex1 == delIsect->edge[1].pIndex1) &&
	  (auxIsect->edge[0].pIndex2 == delIsect->edge[1].pIndex2))
	auxIsect->edge[0].prev = delIsect->edge[1].prev;  
      else
	auxIsect->edge[1].prev = delIsect->edge[1].prev;  
    }
  

  /* delete from global list */
  if (delIsect->prev != NULL)
    delIsect->prev->next = delIsect->next;  
  else /* we are at the head of the list => new head needed */
    isectList->first = delIsect->next;  
  if (delIsect->next != NULL)
    delIsect->next->prev = delIsect->prev;  

  isectList->nrOfElems--;  
}
 

void createIsect(int edge1PIndex1, int edge1PIndex2, 
		 int edge2PIndex1, int edge2PIndex2, 
		 t_isectInfo **isectArray, t_isectList *isectList, 
		 int nrOfPoints)
{
  t_isectInfo *isectInfo;  
  int e1p1, e1p2, e2p1, e2p2;  

  e1p1 = MIN(edge1PIndex1, edge1PIndex2);  
  e1p2 = MAX(edge1PIndex1, edge1PIndex2);  
  e2p1 = MIN(edge2PIndex1, edge2PIndex2);  
  e2p2 = MAX(edge2PIndex1, edge2PIndex2);  
  
  isectInfo = (t_isectInfo *) eralloc(sizeof(t_isectInfo));  
  if ((e1p1 < e2p1) || ((e1p1 == e2p1) && (e1p2 > e2p2)))
    {
      isectInfo->edge[0].pIndex1 = e1p1;  
      isectInfo->edge[0].pIndex2 = e1p2;  
      isectInfo->edge[1].pIndex1 = e2p1;  
      isectInfo->edge[1].pIndex2 = e2p2;  
    }
  else
    {     
      isectInfo->edge[0].pIndex1 = e2p1;  
      isectInfo->edge[0].pIndex2 = e2p2;  
      isectInfo->edge[1].pIndex1 = e1p1;  
      isectInfo->edge[1].pIndex2 = e1p2;  
    }


  isectAdd(isectArray, isectList, isectInfo, nrOfPoints);  
}


void delAllIsect(int pIndex1, int pIndex2, t_isectInfo **isectArray, 
		 t_isectList *isectList, int nrOfPoints)
{
  t_isectInfo *curIsect;  
  
  while ((curIsect=*(getElem(isectArray, pIndex1, pIndex2, nrOfPoints))) != NULL)
    {
      isectDel(isectArray, isectList, curIsect, nrOfPoints);  
      erfree(curIsect);  
    }
}


int isectEdges(t_pointArray *pArray, int edge1PIndex1, int edge1PIndex2, 
		int edge2PIndex1, int edge2PIndex2)
{
  int res;  

  res = PAisectSegments(pArray, edge1PIndex1, edge1PIndex2, edge2PIndex1, 
			edge2PIndex2);  

  return(res);  
}



void createAllIsects(t_pointArray *pArray, t_linkPoly *lPoly, int orig, 
		     t_isectInfo **isectArray, t_isectList *isectList, 
		     int nrOfPoints)
/* NOTE: egde (orig, dest) must be an edge of the polygon  */
{
  int curPIndex;  

  curPIndex = LPgetNext(lPoly, orig);  
  
  do {
    /* does the given edge intersect with the current edge? */
    if (isectEdges(pArray, orig, LPgetNext(lPoly, orig), 
		   curPIndex, LPgetNext(lPoly, curPIndex)))
      createIsect(orig, LPgetNext(lPoly, orig), 
		  curPIndex, LPgetNext(lPoly, curPIndex), 
		  isectArray, isectList, nrOfPoints);  

    /* move to next edge */
    curPIndex = LPgetNext(lPoly, curPIndex);  
  } while (curPIndex != orig);  
}

void untangle(int edge1PIndex1, int edge1PIndex2, int edge2PIndex1, 
	      int edge2PIndex2, t_pointArray *pArray, t_linkPoly *lPoly, 
	      t_isectInfo **isectArray, t_isectList *isectList, int nrOfPoints)
{
  int newNext, nextPIndex, oldPIndex;  
  int dest1, dest2, orig1, orig2;  
  int mustSwap;  

  /* get both origins, both destinations */
  /* edge 1 */
  if (LPgetNext(lPoly, edge1PIndex1) == edge1PIndex2)
    {
      orig1 = edge1PIndex1;  
      dest1 = edge1PIndex2;  
    }
  else
    {
      orig1 = edge1PIndex2;  
      dest1 = edge1PIndex1;  
    }


  /* edge 2 */
  if (LPgetNext(lPoly, edge2PIndex1) == edge2PIndex2)
    {
      orig2 = edge2PIndex1;  
      dest2 = edge2PIndex2;  
    }
  else
    {
      orig2 = edge2PIndex2;  
      dest2 = edge2PIndex1;  
    }

  /* first case: we have colinear points where one segment 
     lies completely within the other */
  /* we know that e1p1 <= e1p2, e1p1 <= e2p1, e2p1 < e2p2 
     and e1p2 < e1p1 (this is necessary for intersection!) 
     so all we have to check for case one is whether 
     e2p2 < e1p2 */
  if ((PAisOnLine(pArray, orig1, dest1, orig2) == 0) && 
     (PAisOnLine(pArray, orig1, dest1, dest2) == 0) &&
     (edge2PIndex2 <= edge1PIndex2))
    {
      /* special case: both edges share an endpoint */
      if (orig1 == dest2)
	{
	  /* first, find the point that lies before o2 */
	  nextPIndex = dest1;  
	  while (LPgetNext(lPoly, nextPIndex) != orig2)
	    nextPIndex = LPgetNext(lPoly, nextPIndex);  

	  delAllIsect(nextPIndex, orig2, isectArray, isectList, 
		      nrOfPoints);  

	  LPsetNext(lPoly, nextPIndex, orig1);  
	  LPsetNext(lPoly, orig1, orig2);  
	  LPsetNext(lPoly, orig2, dest1);  
	  createAllIsects(pArray, lPoly, orig1, isectArray, isectList, 
			  nrOfPoints);  
	  createAllIsects(pArray, lPoly, orig2, isectArray, isectList, 
			  nrOfPoints);  
	  createAllIsects(pArray, lPoly, nextPIndex, isectArray, isectList, 
			  nrOfPoints);  

	  
	}
      else if (dest1 == orig2)
	{
	  delAllIsect(dest2, LPgetNext(lPoly, dest2), isectArray, isectList, 
		       nrOfPoints);  
	  LPsetNext(lPoly, dest1, LPgetNext(lPoly, dest2));  
	  LPsetNext(lPoly, orig1, dest2);  
	  LPsetNext(lPoly, dest2, dest1);  
	  createAllIsects(pArray, lPoly, orig1, isectArray, isectList, 
			  nrOfPoints);  
	  createAllIsects(pArray, lPoly, dest2, isectArray, isectList, 
			  nrOfPoints);  
	  createAllIsects(pArray, lPoly, dest1, isectArray, isectList, 
			  nrOfPoints);  
	  
	}
      else
	{
	  /* first, relink the edge that lies within the other */
	  /* first, find the point that lies before o2 */
	  nextPIndex = dest1;  
	  while (LPgetNext(lPoly, nextPIndex) != orig2)
	    nextPIndex = LPgetNext(lPoly, nextPIndex);  

	  delAllIsect(nextPIndex, orig2, isectArray, isectList, 
		      nrOfPoints);  
	  delAllIsect(dest2, LPgetNext(lPoly, dest2), isectArray, isectList, 
		      nrOfPoints);  
	  LPsetNext(lPoly, nextPIndex, LPgetNext(lPoly, dest2));  
	  createAllIsects(pArray, lPoly, nextPIndex, isectArray, isectList, 
			  nrOfPoints);  
	  /* relink orig1, dest1, orig2, dest2 */
	  if ((orig1 < dest1) == (orig2 < dest2))
	    /* we have o1 => o2 => d2 => d1 */
	    {
	      LPsetNext(lPoly, orig1, orig2);  
	      LPsetNext(lPoly, orig2, dest2);  
	      LPsetNext(lPoly, dest2, dest1);  
	    }
	  else
	    /* we have o1 => d2 => o2 => d1 */
	    {
	      LPsetNext(lPoly, orig1, dest2);  
	      LPsetNext(lPoly, dest2, orig2);  
	      LPsetNext(lPoly, orig2, dest1);  
	    }

	  createAllIsects(pArray, lPoly, orig1, isectArray, isectList, 
			  nrOfPoints);  
	  createAllIsects(pArray, lPoly, orig2, isectArray, isectList, 
			  nrOfPoints);  
	  createAllIsects(pArray, lPoly, dest2, isectArray, isectList, 
			  nrOfPoints);  
	}
    }
  /* other case: link origin with origin, destination with destination */
  else
    {
      /* check whether Index 1 lies within d2->o1, 
	 if yes, swap edge 1 and 2! 
	 Otherwise, we would loose the counterclockwise orientation on 
	 our polygon! */
      nextPIndex = dest2;  
      oldPIndex = LPgetNext(lPoly, orig1);  
      mustSwap = FALSE;  

      while ((nextPIndex != oldPIndex) && !mustSwap)
	{
	  mustSwap = (nextPIndex == 1);  
	  nextPIndex = LPgetNext(lPoly, nextPIndex);  
	}

      /* point 1 lies on the chain from d2->o1, so we must invert the 
	 other half of the polygon! => swap edge1 and edge2 */
      if (mustSwap)
	{
	  oldPIndex = orig1;  
	  orig1 = orig2;  
	  orig2 = oldPIndex;  
	  oldPIndex = dest1;  
	  dest1 = dest2;  
	  dest2 = oldPIndex;  
	}

      /* revert orientation in the chain from d2->o1 */
      newNext = LPgetNext(lPoly, dest2);  
      nextPIndex = dest2;  

      do {
	/* proceed to the next point */
	oldPIndex = nextPIndex;  
	nextPIndex = newNext;  

	/* save the next point */
	newNext = LPgetNext(lPoly, nextPIndex);  

	/* make the current edge */
	LPsetNext(lPoly, nextPIndex, oldPIndex);  
      } while (nextPIndex != orig1);  

      /* make both new edges */
      LPsetNext(lPoly, dest2, dest1);  
      LPsetNext(lPoly, orig2, orig1);  

      /* create all the intersections for the 2 new edges */
      createAllIsects(pArray, lPoly, dest2, isectArray, isectList, 
		      nrOfPoints);  
      createAllIsects(pArray, lPoly, orig2, isectArray, isectList, 
		      nrOfPoints);  
    }

}

void M2optPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
	       FILE *outFile)
{
  int nrOfPoints, polyCount;  
  
  /* get the number of points */
  nrOfPoints = PAnrOfPoints(pArray);  

  if (nrOfPoints >= 3)
    {
      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	{
	  /* init the link Polygon */
	  BPfree(aPolygon);  
	  BPinit(aPolygon, nrOfPoints);  
	  BPrandomPerm(aPolygon);  
	  
	  /* now we have a polygon, let's make it simple! */
	  Mdo2optMoves(aPolygon, pArray, nrOfPoints);  

	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	  
	  if (!BPisSimple(aPolygon, pArray))
	    {
	      printf("ERROR! POLYGON NOT SIMPLE!\n");  
	      exit(1);  
	    }

	}
    }
}


void Mdo2optMoves(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPoints)
{
  int nrOfEdges;  
  int curPIndex, auxPIndex;  
  int e1p1, e1p2, e2p1, e2p2;  
  int count, count2, randomIndex;  
  t_linkPoly lPoly;  
  t_isectInfo **isectArray, *curIsect;  
  t_isectList isectList;  

  /* create a linked Polygon */
  LPfromPoly(&lPoly, aPolygon);  
  
  /* init the list of all intersections, init the array */
  isectList.nrOfElems = 0;  
  isectList.first = NULL;  
  
  nrOfEdges = ((nrOfPoints*nrOfPoints)-nrOfPoints)/2;  
  isectArray = 
    (t_isectInfo **) ercalloc(nrOfEdges, sizeof(t_isectInfo *));  
  
  for (count=0;  count<nrOfEdges;  count++)
    *(isectArray+count) = NULL;  
  
  /* first, calculate all the intersections */
  curPIndex = 1;  
  for (count=1;  count<nrOfPoints;  count++)
    {
      auxPIndex = curPIndex;  
      for (count2=count;  count2 < nrOfPoints;  count2++)
	{
	  auxPIndex = LPgetNext(&lPoly, auxPIndex);  
	  if (isectEdges(pArray, curPIndex, LPgetNext(&lPoly, curPIndex), 
			 auxPIndex, LPgetNext(&lPoly, auxPIndex)))
	    
	    createIsect(curPIndex, LPgetNext(&lPoly, curPIndex), 
			auxPIndex, LPgetNext(&lPoly, auxPIndex), 
			isectArray, &isectList, nrOfPoints);  
	}
      curPIndex = LPgetNext(&lPoly, curPIndex);  	  	  
    }
  
  
  /* now process all the intersections */
  while (isectList.nrOfElems > 0)
    {
#ifdef DO_STATS
      optCount++;  
#endif
      /* choose one intersection at random */
      curIsect = isectList.first;  
      randomIndex = randomInt(1, isectList.nrOfElems);  
      while (randomIndex > 1)
	{
	  curIsect = curIsect->next;  
	  randomIndex--;  
	}
      
      e1p1 = curIsect->edge[0].pIndex1;  
      e1p2 = curIsect->edge[0].pIndex2;  
      e2p1 = curIsect->edge[1].pIndex1;  
      e2p2 = curIsect->edge[1].pIndex2;  
      /* delete all the intersections defined by 
	 both edges */
      delAllIsect(e1p1, e1p2, isectArray, &isectList, nrOfPoints);  
      delAllIsect(e2p1, e2p2, isectArray, &isectList, nrOfPoints);  
      
      /* untangle the intersection */
      untangle(e1p1, e1p2, e2p1, e2p2, pArray, &lPoly, isectArray, 
	       &isectList, nrOfPoints);  
      
    }
  
  
  erfree(isectArray);  	
  BPfree(aPolygon);  
  LPtoPoly(&lPoly, aPolygon);  
  LPfree(&lPoly);  

  BPnormalize(aPolygon, pArray);  	
}

