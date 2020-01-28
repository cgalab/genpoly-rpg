/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MgrowingPoly.h"
#include "incConvexHull.h"
#include "linkPoly.h"
#include "intIncArray.h"
#include "sortPoly.h"
#include "basicAVL.h"
#include "basicGeom.h"
#include "eralloc.h"

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

static t_pointArray *cArray;  
#ifdef DO_STATS
unsigned long growingCount;  
#endif

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
void MRgrowingPoly()
{
  growingCount = 0;  
}


void MGgrowingPoly(unsigned long *tries)
{
  *tries = growingCount;  
}
#endif



int MdelComp(caddr_t elem1, caddr_t elem2)
{
  int res;  
  t_McompEdge *e1, *e2;  

  e1 = (t_McompEdge *)elem1;  
  e2 = (t_McompEdge *)elem2;  

  if (e1->dest == e2->dest)
    res = 0;  
  else
    res = isOnLine(PAgetPoint(cArray, e2->orig), PAgetPoint(cArray, e2->dest), 
		     PAgetPoint(cArray, e1->dest));  
  
  return(res);  
}

int MinsComp(caddr_t elem1, caddr_t elem2)
{
  int res;  
  t_McompEdge *e1, *e2;  

  e1 = (t_McompEdge *)elem1;  
  e2 = (t_McompEdge *)elem2;  

  if (isOnLine(PAgetPoint(cArray, e2->orig), PAgetPoint(cArray, e2->dest), 
	       PAgetPoint(cArray, e1->orig)) == -1)
    {
      /* e1 is smaller than e2 */
      res = -1;  
      e2->visible = FALSE;  
    }
  else
    {
      res = 1;  
      e1->visible = FALSE;  
    }

  return(res);  
}


void MgrowingPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		  FILE *outFile)
{
  int count, count2, auxCount, noPointInside, polyCount;  
  int addPIndex, indexCount, leftBridge, rightBridge;  
  int nrOfPoints;  
  int curPIndex, oldPIndex;  
  t_intArray usedArray;  
  t_incCHull iHull;  
  t_linkPoly lPoly;  
  t_iIncArray visPoints, edgeArray;  
  t_AVLtree edgeTree;  
  t_McompEdge *curEdge;  
  t_McompEdge delEdge;  

  nrOfPoints = PAnrOfPoints(pArray);  

  if (nrOfPoints >= 3)
    {
      /* init the array of used points */
      IAinit(&usedArray, nrOfPoints);  
      

      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	{
	  /* init the linkPoly */
	  LPinit(&lPoly, nrOfPoints);  
	  
	  /* set the array for comparisons */
	  cArray = pArray;  

	  for (count=1;  count <= nrOfPoints;  count++)
	    {
	      IAsetInt(&usedArray, count, 0);  
	    }
      
	  /* select 2 start points */
	  /* init the convex hull with the two points */
	  IHinit(&iHull, nrOfPoints);  
	  addPIndex = randomInt(1, nrOfPoints);  
	  IHinsert(&iHull, addPIndex, addPIndex, addPIndex);  
	  LPaddAfter(&lPoly, addPIndex, addPIndex);  
	  IAsetInt(&usedArray, addPIndex, nrOfPoints);  
      
	  leftBridge = addPIndex;  
      
	  addPIndex = 
	    ((leftBridge + randomInt(0, nrOfPoints-2)) % nrOfPoints)+1;  
	  IHinsert(&iHull, addPIndex, leftBridge, leftBridge);  
	  LPaddAfter(&lPoly, leftBridge, addPIndex);   
	  IAsetInt(&usedArray, addPIndex, nrOfPoints);  
      
	  /* add the rest (=n-2) points */
	  for (count=2;  count < nrOfPoints;  count++)
	    {
	      auxCount = 0;  

	      do{ 
		/* choose one point at random */
		indexCount = randomInt(1, nrOfPoints-count-auxCount);  
	    
		do{
		  addPIndex = (addPIndex % nrOfPoints)+1;  
	      
		  if (IAgetInt(&usedArray, addPIndex) < count)
		    indexCount --;  
	      
		} while (indexCount > 0);  
	    
		auxCount++;  
		IAsetInt(&usedArray, addPIndex, count);  
	    
		/* get the supporting edges for the point */
		IHgetBridges(&iHull, addPIndex, &leftBridge, &rightBridge, pArray);  
	    
		/* check wether all the other points lie outside the 
		   triangle left_edge, newpoint, right_edge */

		count2 = 1;  
		noPointInside = TRUE;  
	    
		do {
		  if ((IAgetInt(&usedArray, count2) <= count) &&
		      (count2 != addPIndex))
		    noPointInside = ((isOnLine(PAgetPoint(pArray, addPIndex), 
					       PAgetPoint(pArray, leftBridge), 
					       PAgetPoint(pArray, count2)) == 1) ||
				     (isOnLine(PAgetPoint(pArray, rightBridge), 
					       PAgetPoint(pArray, addPIndex), 
					       PAgetPoint(pArray, count2)) == 1) ||
				     (isOnLine(PAgetPoint(pArray, leftBridge), 
					       PAgetPoint(pArray, rightBridge), 
					       PAgetPoint(pArray, count2)) == 1));  
	      
		  count2++;  
		} while (noPointInside && (count2 <= nrOfPoints));  
		
#ifdef DO_STATS
	  	growingCount++;  
#endif
	      } while (!noPointInside);  
	  
	      /* now we have found a point s.t. no other point lies 
		 within the convex hull */
	  
	      /* sort all the points appearing in the polygon 
		 between the left and the right supporting edge */
	      IIinitArray(&visPoints, nrOfPoints);  
	      IIaddInt(&visPoints, rightBridge);  
	      curPIndex = leftBridge;  
	  
	      while (curPIndex != rightBridge)
		{
		  IIaddInt(&visPoints, curPIndex);  
		  curPIndex = LPgetNext(&lPoly, curPIndex);  
		}
	    
	      IIfreezeArray(&visPoints);  
	      sortSelPoints(pArray, &visPoints, addPIndex);  
	      /* take the sorted array of points: if p_{ij} and 
		 p_{ij+1} are an edge of the polygon, save 
		 this edge */
	      ATinitTree(&edgeTree);  
	      IIinitArray(&edgeArray, nrOfPoints);  
	  
	      for (count2 = IIsizeOf(&visPoints);  count2 > 0;  count2--)
		{
		  if (((curEdge=(t_McompEdge *)ATviewMin(&edgeTree)) != NULL) &&
		      (curEdge->visible) &&
		      (curEdge->dest == IIgetInt(&visPoints, count2)))
		    IIaddInt(&edgeArray, curEdge->orig);  
	      
		  delEdge.dest = IIgetInt(&visPoints, count2);  
	      
		  ATdelElem(&edgeTree, (caddr_t)&delEdge, MdelComp, erfree);  
	      
		  if (isOnLine(PAgetPoint(pArray, addPIndex), 
			       PAgetPoint(pArray, IIgetInt(&visPoints, count2)), 
			       PAgetPoint(pArray, LPgetNext(&lPoly, 
							   IIgetInt(&visPoints, 
								    count2))))==-1)
		    {
		      curEdge = (t_McompEdge *) eralloc(sizeof(t_McompEdge));  
		      curEdge->visible = TRUE;  
		      curEdge->orig = IIgetInt(&visPoints, count2);  
		      curEdge->dest = LPgetNext(&lPoly, curEdge->orig);  
		      ATaddElem(&edgeTree, (caddr_t)curEdge, MinsComp);  
		    }
		}
	    
	      ATfreeTElem(&edgeTree, erfree);  

	      /* from all the edges saved, take one at random */
	      oldPIndex = IIgetInt(&edgeArray, randomInt(1, IIsizeOf(&edgeArray)));  

	      /* replace edge p_{ij}, p_{ij+1} with edges 
		 p_{ij}, newpoint and newpoint, p_{ij+1} */
	      LPaddAfter(&lPoly, oldPIndex, addPIndex);  
	      IAsetInt(&usedArray, addPIndex, nrOfPoints);  

	      /* free both temporary arrays */
	      IIfreeArray(&visPoints);  
	      IIfreeArray(&edgeArray);  
	  
	  
	      /* update the convex hull */
	      IHinsert(&iHull, addPIndex, leftBridge, rightBridge);  
	    }
	
	
	  BPfree(aPolygon);  
	  LPtoPoly(&lPoly, aPolygon);  
	  IHfreeHull(&iHull);  
	  LPfree(&lPoly);  

	  BPnormalize(aPolygon, pArray);  
	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	}
      IAfree(&usedArray);  
    }
}

void MgrowingPolyII(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		  FILE *outFile)
{
  int count, count2, noPointInside, polyCount;  
  int addPIndex, indexCount, leftBridge, rightBridge;  
  int nrOfPoints;  
  int curPIndex, oldPIndex;  
  t_intArray usedArray;  
  t_incCHull iHull;  
  t_linkPoly lPoly;  
  t_iIncArray visPoints, edgeArray;  
  t_AVLtree edgeTree;  
  t_McompEdge *curEdge;  
  t_McompEdge delEdge;  

  nrOfPoints = PAnrOfPoints(pArray);  

  if (nrOfPoints >= 3)
    {
      /* init the array of used points */
      IAinit(&usedArray, nrOfPoints);  
      

      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	{
	  /* init the linkPoly */
	  LPinit(&lPoly, nrOfPoints);  
	  
	  /* set the array for comparisons */
	  cArray = pArray;  

	  for (count=1;  count <= nrOfPoints;  count++)
	    {
	      IAsetInt(&usedArray, count, 0);  
	    }
      
	  /* select 2 start points */
	  /* init the convex hull with the two points */
	  IHinit(&iHull, nrOfPoints);  
	  addPIndex = randomInt(1, nrOfPoints);  
	  IHinsert(&iHull, addPIndex, addPIndex, addPIndex);  
	  LPaddAfter(&lPoly, addPIndex, addPIndex);  
	  IAsetInt(&usedArray, addPIndex, nrOfPoints);  
      
	  leftBridge = addPIndex;  
      
	  addPIndex = 
	    ((leftBridge + randomInt(0, nrOfPoints-2)) % nrOfPoints)+1;  
	  IHinsert(&iHull, addPIndex, leftBridge, leftBridge);  
	  LPaddAfter(&lPoly, leftBridge, addPIndex);   
	  IAsetInt(&usedArray, addPIndex, nrOfPoints);  
      
	  /* add the rest (=n-2) points */
	  for (count=2;  count < nrOfPoints;  count++)
	    {
	      /* choose one point at random */
	      indexCount = randomInt(1, nrOfPoints-count);  
	      
	      do{
		addPIndex = (addPIndex % nrOfPoints)+1;  
	      
		if (IAgetInt(&usedArray, addPIndex) < count)
		  indexCount --;  
		
	      } while (indexCount > 0);  
	      
	      /* get the supporting edges for the point */
	      IHgetBridges(&iHull, addPIndex, &leftBridge, &rightBridge, pArray);  
	      
	      /* check wether all the other points lie outside the 
		 triangle left_edge, newpoint, right_edge */

#ifdef DO_STATS
	      growingCount++;  
#endif
	      
	      count2 = 1;  
	      noPointInside = TRUE;  
	      
	      for (count2=1;  count2 <= nrOfPoints;  count2++)
		{
		  if ((IAgetInt(&usedArray, count2) <= count) &&
		      (count2 != addPIndex))
		    if ((isOnLine(PAgetPoint(pArray, addPIndex), 
				  PAgetPoint(pArray, leftBridge), 
				  PAgetPoint(pArray, count2)) != 1) &&
			(isOnLine(PAgetPoint(pArray, rightBridge), 
				  PAgetPoint(pArray, addPIndex), 
				  PAgetPoint(pArray, count2)) != 1) &&
			(isOnLine(PAgetPoint(pArray, leftBridge), 
				  PAgetPoint(pArray, rightBridge), 
				  PAgetPoint(pArray, count2)) != 1))
		      {
			addPIndex = count2;  
			IHupdateBridges(&iHull, addPIndex, &leftBridge, 
			   &rightBridge, pArray);  
#ifdef DO_STATS
			growingCount++;  
#endif
		      }
		}
	      
	      /* now we have found a point s.t. no other point lies 
		 within the convex hull */
	      
	      /* sort all the points appearing in the polygon 
		 between the left and the right supporting edge */
	      IIinitArray(&visPoints, nrOfPoints);  
	      IIaddInt(&visPoints, rightBridge);  
	      curPIndex = leftBridge;  
	      
	      while (curPIndex != rightBridge)
		{
		  IIaddInt(&visPoints, curPIndex);  
		  curPIndex = LPgetNext(&lPoly, curPIndex);  
		}
	    
	      IIfreezeArray(&visPoints);  
	      sortSelPoints(pArray, &visPoints, addPIndex);  
	      /* take the sorted array of points: if p_{ij} and 
		 p_{ij+1} are an edge of the polygon, save 
		 this edge */
	      ATinitTree(&edgeTree);  
	      IIinitArray(&edgeArray, nrOfPoints);  
	  
	      for (count2 = IIsizeOf(&visPoints);  count2 > 0;  count2--)
		{
		  if (((curEdge=(t_McompEdge *)ATviewMin(&edgeTree)) != NULL) &&
		      (curEdge->visible) &&
		      (curEdge->dest == IIgetInt(&visPoints, count2)))
		    IIaddInt(&edgeArray, curEdge->orig);  
	      
		  delEdge.dest = IIgetInt(&visPoints, count2);  
	      
		  ATdelElem(&edgeTree, (caddr_t)&delEdge, MdelComp, erfree);  
	      
		  if (isOnLine(PAgetPoint(pArray, addPIndex), 
			       PAgetPoint(pArray, IIgetInt(&visPoints, count2)), 
			       PAgetPoint(pArray, LPgetNext(&lPoly, 
							   IIgetInt(&visPoints, 
								    count2))))==-1)
		    {
		      curEdge = (t_McompEdge *) eralloc(sizeof(t_McompEdge));  
		      curEdge->visible = TRUE;  
		      curEdge->orig = IIgetInt(&visPoints, count2);  
		      curEdge->dest = LPgetNext(&lPoly, curEdge->orig);  
		      ATaddElem(&edgeTree, (caddr_t)curEdge, MinsComp);  
		    }
		}
	    
	      ATfreeTElem(&edgeTree, erfree);  

	      /* from all the edges saved, take one at random */
	      oldPIndex = IIgetInt(&edgeArray, randomInt(1, IIsizeOf(&edgeArray)));  

	      /* replace edge p_{ij}, p_{ij+1} with edges 
		 p_{ij}, newpoint and newpoint, p_{ij+1} */
	      LPaddAfter(&lPoly, oldPIndex, addPIndex);  
	      IAsetInt(&usedArray, addPIndex, nrOfPoints);  

	      /* free both temporary arrays */
	      IIfreeArray(&visPoints);  
	      IIfreeArray(&edgeArray);  
	  
	  
	      /* update the convex hull */
	      IHinsert(&iHull, addPIndex, leftBridge, rightBridge);  
	    }
	
	
	  BPfree(aPolygon);  
	  LPtoPoly(&lPoly, aPolygon);  
	  IHfreeHull(&iHull);  
	  LPfree(&lPoly);  

	  BPnormalize(aPolygon, pArray);  
	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	}
      IAfree(&usedArray);  
    }
}

