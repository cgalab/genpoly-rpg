/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MsearchPoly.h"
#include "isectArray.h"
#include "convexHull.h"
#include "forcedEdge.h"
#include "edgeIncArray.h"

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#ifdef DO_STATS
unsigned long searchCount;  
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
void MRsearchPoly()
{
  searchCount = 0;  
}


void MGsearchPoly(unsigned long *tries)
{
  *tries = searchCount;  
}
#endif


int edgeIndex(int pIndex1, int pIndex2, int nrOfPoints)
{
  int index;  
  int i1, i2;  
  
  i1 = MIN(pIndex1, pIndex2);  
  i2 = MAX(pIndex1, pIndex2);  
  
  index = (2*nrOfPoints - i1)*(i1 - 1)/2 + i2 - i1;  
  
  return(index);  
}

int isFree(t_intArray *edgeMarker, int pIndex1, int pIndex2, int nrOfPoints)
{
  int index, result;  

  result = (pIndex1 != pIndex2);  

  index = edgeIndex(pIndex1, pIndex2, nrOfPoints);  

  /* return whether we used an edge */
  result = result && ((IAgetInt(edgeMarker, index) > nrOfPoints));  
  
  return(result);  
}

int markSingle(t_intArray *edgeMarker, t_intArray *pointEdges, 
	       t_edgeIArray *markedEdges, int pIndex1, int pIndex2, int curStep)
{
  int index;  

  if (pIndex1 != pIndex2)
    {
      index = edgeIndex(pIndex1, pIndex2, IAsizeOf(pointEdges));  
      if (IAgetInt(edgeMarker, index) > curStep)
	{
	  IAsetInt(edgeMarker, index, curStep);  
	  IAdec(pointEdges, pIndex1);  
	  IAdec(pointEdges, pIndex2);  
	  EAaddEdge(markedEdges, pIndex1, pIndex2);  
	}
    }
  return(TRUE);  
}


int rule1(t_intArray *pointEdges, t_intArray *usedPoints)
{
  /* rule 1: no vertex except the current or the end vertex
     may have liberty 1, that is no unused point may have 
     liberty 1 */
  int count;  
  int result;  

  result = TRUE;  

  count=2;  
  while (result && (count <= IAsizeOf(pointEdges)))
    {
      if (!IAgetInt(usedPoints, count))
	result = (IAgetInt(pointEdges, count) > 1);  

      count++;  
    }

  return(result);  
  
}

int rule2(t_intArray *pointEdges, t_intArray *usedPoints, 
	  t_intArray *edgeMarker, int curPoint)
{
  /* rule 2: only one vertex adjacent to the current vertex may 
     have liberty 2 */
  int count;  
  int firstFound;  
  int result;  

  result =TRUE;  
  firstFound = FALSE;  
  count = 1;  

  while (result &&(count <= IAsizeOf(pointEdges)))
    {
      if ((!IAgetInt(usedPoints, count)) &&
	  (IAgetInt(pointEdges, count) == 2) &&
	  isFree(edgeMarker, count, curPoint, IAsizeOf(pointEdges)))
	{
	  if (firstFound)
	    result = FALSE;  
	  else
	    firstFound = TRUE;  
	}
      count++;  
    }
  
  return(result);  
}

/* we will not need rule 3, because backplanning is automatically
   enforced */

int rule4(t_intArray *edgeMarker, t_intArray *pointEdges, t_intArray *usedPoints, 
	  t_forcedEdges *fEdge, t_isectArray *iArray, t_edgeIArray *markedEdges, 
	  t_edgeIArray *forcedEdges, int curStep)
{
  /* rule 4: if a vertex is adjacent to 2 vertices which have 
     degree 2, remove all other edges from that vertex. */
  int count, count2;  
  int result = TRUE;  

  /* loop over all points */
  for (count=1;  count<=IAsizeOf(pointEdges);  count++)
    /* check only edges we did not use already */
    if (!IAgetInt(usedPoints, count))
      {
	/* check whether this point has only 2 liberties, 
	   if yes, force both edges! */
	if (IAgetInt(pointEdges, count) == 2)
	  {
	    /* we have only 2 edges for this point, force them both! */
	    for (count2=1;  count2<=IAsizeOf(pointEdges);  count2++) 
	      if ((count2 != count) && (!IAgetInt(pointEdges, count2)) &&
		  isFree(edgeMarker, count, count2, IAsizeOf(pointEdges)))
		{
		  if (!isForced(fEdge, count, count2))
		    markIsects(edgeMarker, pointEdges, markedEdges, iArray, 
			       count, count2, 
			       curStep);  
		  result = result && forceEdge(fEdge, forcedEdges, count, 
					       count2, curStep);  
		}
	  }
      }

  /* for all points check, whether they have 2 forced edges, 
     if yes, mark all other edges! */
  for (count=1;  count<=IAsizeOf(pointEdges);  count++)
    if ((nrOfForced(fEdge, count) == 2) && (!IAgetInt(usedPoints, count)))
      for (count2=1;  count2<=IAsizeOf(pointEdges);  count2++)
	{
	  if ((count2!=count) && (!isForced(fEdge, count, count2)))
	    markSingle(edgeMarker, pointEdges, markedEdges, count, count2, curStep);  
	}
  return(result);  
}


int hullRule(int *leftGuard, int *rightGuard, int newPoint, t_convexHull *cHull)
{
  /* hullRule: guarantee that all the vertices on CH appear in the
     order they appear on the CH */
  int result;  
  t_polyLine pOnHull;  

  result = TRUE;  
  
  if (CHisOnHull(cHull, newPoint))
    {
      pOnHull = CHgetPoints(cHull);  

      if (((*leftGuard) != -1) && 
	 (BLgetPIndex(&pOnHull, (*leftGuard)) == newPoint))
	{
	  /* if we walk around in the left direction, disable right */
	  (*leftGuard)--;  
	  (*rightGuard) = -1;  
	}
      else if (((*rightGuard) != -1) && 
	      (BLgetPIndex(&pOnHull, (*rightGuard)) == newPoint))
	{
	  /* if we walk around in the right direction, disable left */
	  (*rightGuard)++;  
	  (*leftGuard) = -1;  
	}
      else
	result = FALSE;  
    }
    
  return(result);  
}


int ccwRule(t_pointArray *pArray, t_polygon *aPolygon, 
	    int newPoint, int *leftPoints)
{
  int count;  

  /* if the current point is the second point, 
     calculate leftPoints, else check whether to decrement it! */
  if (BPgetPIndex(aPolygon, 2) == newPoint)
    {
      (*leftPoints) = 0;  

      for (count=1;  count<=BPsizeOf(aPolygon);  count++)
	if ((count != 1) && (count != newPoint) &&
	    (PAisOnLine(pArray, BPgetPIndex(aPolygon, 1), 
			BPgetPIndex(aPolygon, 2), count) == 1))
	  (*leftPoints)++;  
    }
  
  /* guarantee that the polygon has ccw order */
  else if (PAisOnLine(pArray, BPgetPIndex(aPolygon, 1), BPgetPIndex(aPolygon, 2), 
		 newPoint) == 1)
    (*leftPoints)--;  
  
  return((*leftPoints) > 0);  
}





int markEdges(t_intArray *edgeMarker, t_intArray *pointEdges, 
	      t_edgeIArray *markedEdges, int pIndex, int curStep)
{
  int count;  

  for (count=1;  count<=IAsizeOf(pointEdges);  count++)
    if (count != pIndex)
      markSingle(edgeMarker, pointEdges, markedEdges, pIndex, count, curStep);  
  
  return(TRUE);  
}

int markIsects(t_intArray *edgeMarker, t_intArray *pointEdges, 
	       t_edgeIArray *markedEdges, t_isectArray *iArray, 
	       int pIndex1, int pIndex2, int curStep)
{
  int count1, count2;  
  int index;  
   
  for (count1=1;  count1 < IAsizeOf(pointEdges);  count1++)
    for (count2=count1+1;  count2<=IAsizeOf(pointEdges);  count2++)
      if (IScheckIsect(iArray, count1, count2, pIndex1, pIndex2))
	{
	  index = edgeIndex(count1, count2, IAsizeOf(pointEdges));  
	  if (IAgetInt(edgeMarker, index) > curStep)
	    {
	      IAsetInt(edgeMarker, index, curStep);  
	      IAdec(pointEdges, count1);  
	      IAdec(pointEdges, count2);  
	      EAaddEdge(markedEdges, count1, count2);  
	    }
	}
  return(TRUE);  
}

void freeEdges(t_intArray *edgeMarker, t_intArray *pointEdges, 
	       t_edgeIArray *markedEdges, int curStep)
{
  int count;  
  int pIndex1, pIndex2;  
  int nrOfPoints;  
  int index;  
  
  EAfreezeArray(markedEdges);  
  nrOfPoints = IAsizeOf(pointEdges);  

  for (count=1;  count <= EAnrOfEdges(markedEdges);  count++)
    {
      EAgetEdge(markedEdges, count, &pIndex1, &pIndex2);  
      index = edgeIndex(pIndex1, pIndex2, nrOfPoints);  
      
      if (IAgetInt(edgeMarker, index) == curStep)
	{	
	  /* mark edge as unused */
	  IAsetInt(edgeMarker, index, nrOfPoints+1);  
	  
	  /* if the edge does not originate from point 1, 
	     increase the number of usable edges */
	    IAinc(pointEdges, pIndex1);  
	    IAinc(pointEdges, pIndex2);  
	}
    }
}


int recCreatePoly(t_polygon *aPolygon, t_isectArray *iArray, int nrOfLines, 
		  int curStep, int nrOfPoints, t_intArray *edgeMarker, 
		  t_intArray *pointEdges, t_forcedEdges *fEdge, 
		  t_intArray *usedPoints, int origLGuard, int origRGuard, 
		  t_convexHull *cHull, int origLPoints, t_pointArray *pArray)
{
  int completion;  
  int validResult;  
  int randomValue;  
  int lastPoint, newPoint;  
  int possibleEdges;  
  int leftGuard, rightGuard, leftPoints;  
  t_intArray curAvail;  
  t_edgeIArray markedEdges, forcedEdges;  
  
  lastPoint = BPgetPIndex(aPolygon, curStep-1);  

  validResult = TRUE;  

  IAinit(&curAvail, nrOfPoints);  
  EAinitArray(&markedEdges, nrOfPoints);  
  EAinitArray(&forcedEdges, nrOfPoints);  

  for (completion=1;  completion<=nrOfPoints;  completion++)
    if (isFree(edgeMarker, lastPoint, completion, nrOfPoints))
      IAsetInt(&curAvail, completion, TRUE);  
    else
      IAsetInt(&curAvail, completion, FALSE);  

  if (isFree(edgeMarker, lastPoint, 1, nrOfPoints))
    /* we may not choose the first point again! for all points 
	 used besides the first point, we have marked the edges as 
	 used! */
    possibleEdges = IAgetInt(pointEdges, lastPoint)-1;  
  else
    possibleEdges = IAgetInt(pointEdges, lastPoint);  
    
  do {
    /* choose a random value for the edge */
    
    if (possibleEdges > 0)
      {
	/* get the guards for the convex hull */
	leftGuard = origLGuard;  
	rightGuard = origRGuard;  
	leftPoints = origLPoints;  

	if (((newPoint = forcedEdge(fEdge, lastPoint, usedPoints)) != -1) &&
	   (curStep != 2))
	  {
	    /* if we have a forced edge, we must use this edge, 
	       so there is no sense in another iteration! */
	    validResult = isFree(edgeMarker, lastPoint, newPoint, nrOfPoints);  
	    completion = validResult;  
	  }	  
	else
	  {
	    validResult = TRUE;  
	    /* exit the loop when we found a completion or 
	       we added the last point */
	    completion = TRUE;  
	    randomValue = randomInt(1, possibleEdges);  	
	    
	    newPoint = 0;  
	    do {
	      newPoint ++;  
	      /* we require that the new point is different from the last 
		 and has not been used already! */
	      if ((newPoint != lastPoint) && 
		  (!IAgetInt(usedPoints, newPoint)) &&
		  IAgetInt(&curAvail, newPoint))
		randomValue--;  
	    } while (randomValue > 0);  
	  }

	/* choose one point */
	BPsetPIndex(aPolygon, curStep, newPoint);  
	IAsetInt(usedPoints, newPoint, TRUE);  
	
	/*	for (completion=1;  completion<curStep;  completion++)
		printf("  ");  
		printf("ADDING POINT %2u AT STEP %2u\n", newPoint, curStep);  */
	
	/* mark all the edges that cross with the edge 
	   (lastPoint, newPoint) */
	markIsects(edgeMarker, pointEdges, &markedEdges, iArray, lastPoint, 
		   newPoint, curStep);  
	
	/* mark the current edge as used in the current step! */
	markSingle(edgeMarker, pointEdges, &markedEdges, lastPoint, newPoint, 
		   curStep);  
	IAsetInt(&curAvail, newPoint, FALSE);  
	possibleEdges--;  
    
	/* mark all the edges with lastPoint, but only if lastPoint 
	   is not the startPoint */
	if (curStep != 2)
	  markEdges(edgeMarker, pointEdges, &markedEdges, lastPoint, curStep);  
	
	if (curStep == nrOfPoints)
	  {
	    /* we have reached the last point, so we have 
	       completion */
	    /* Do we have a valid result? => check whether the edge 
	       between the last point and point 1 is available! */
	    validResult = validResult && 
	      isFree(edgeMarker, BPgetPIndex(aPolygon, curStep), 1, nrOfPoints);  

	    /* unmark all the edges we have marked in the process */
	    freeEdges(edgeMarker, pointEdges, &markedEdges, curStep);  
	    IAsetInt(usedPoints, newPoint, FALSE);  
	  }
	else
	  {
	    completion = completion && hullRule(&leftGuard, &rightGuard, 
						newPoint, cHull);  
	    /* completion = completion && ccwRule(pArray, aPolygon, newPoint, 
	       &leftPoints);  */
	    completion = completion && rule1(pointEdges, usedPoints);  
	    completion = completion && rule2(pointEdges, usedPoints, 
					     edgeMarker, newPoint);  
	    completion = completion && rule4(edgeMarker, pointEdges, usedPoints, 
					     fEdge, iArray, &markedEdges, 
					     &forcedEdges, curStep);  
	    if (completion)
	      completion =  recCreatePoly(aPolygon, iArray, nrOfLines, curStep+1, 
					  nrOfPoints, edgeMarker, pointEdges, 
					  fEdge, usedPoints, leftGuard, 
					  rightGuard, cHull, leftPoints, pArray);  
      
	    /* unmark all the edges we have marked in the process */
	    freeEdges(edgeMarker, pointEdges, &markedEdges, curStep);  
	    unforce(fEdge, &forcedEdges, curStep);  
	    IAsetInt(usedPoints, newPoint, FALSE);  

	    /* check whether there exist another choice for newPoint, 
	       or if we already have found a solution! */
	    validResult = (completion || validResult);  
	  }
      }
    else
      validResult = FALSE;  
    

  } while ((!completion) && (validResult));  

  IAfree(&curAvail);  
  EAfreeArray(&markedEdges);  
  EAfreeArray(&forcedEdges);  
  
#ifdef DO_STATS
  if (!validResult)
    searchCount++;  
#endif

  return(validResult);  
}


void MsearchPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
		 FILE *outFile)
{
  int nrOfPoints, nrOfLines;  
  int count, polyCount;  
  t_intArray edgeMarker, pointEdges;  
  t_intArray usedPoints;  
  t_isectArray iArray;  
  t_convexHull cHull;  
  t_polyLine pOnHull;  
  t_forcedEdges fEdge;  
  
  nrOfPoints = PAnrOfPoints(pArray);  
  
  if (nrOfPoints >= 3)
    {
      /* calculate the number of edges */
      nrOfLines = ((nrOfPoints*nrOfPoints)-nrOfPoints)/2;  

      /* create an integer array where we may note all 
	 the lines we may use and an array for the actual number of
	 lines */
      IAinit(&edgeMarker, nrOfLines);  
      IAinit(&pointEdges, nrOfPoints);  
      IAinit(&usedPoints, nrOfPoints);  
      CHcalcHull(&cHull, pArray);  
      initFEdge(&fEdge, nrOfPoints);  

      /* init the information about possible edges */
      ISinit(&iArray, pArray, nrOfLines);  
      
      pOnHull = CHgetPoints(&cHull);  

      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	{
	  /* free and reinit the polygon */
	  BPfree(aPolygon);  
	  BPinit(aPolygon, nrOfPoints);  

	  /* for being on the safe side, init the flags for each
	     polygon separately */
	  for (count=1;  count<=nrOfPoints;  count++)
	    {
	      IAsetInt(&pointEdges, count, nrOfPoints-1);  
	      IAsetInt(&usedPoints, count, FALSE);  
	    }

	  /* let the point with index 1 be the first point in the polygon */
	  BPsetPIndex(aPolygon, 1, 1);  
	  IAsetInt(&usedPoints, 1, TRUE);  

	  for (count=1;  count<=nrOfLines;  count++)
	    IAsetInt(&edgeMarker, count, nrOfPoints+1);  

	  /* recursively create the polygon */
	  recCreatePoly(aPolygon, &iArray, nrOfLines, 2, nrOfPoints, &edgeMarker, 
			&pointEdges, &fEdge, &usedPoints, 
			BLsizeOf(&pOnHull), 2, &cHull, 0, pArray);  

	  BPnormalize(aPolygon, pArray);  
	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	}

      /* free all the used arrays */
      IAfree(&edgeMarker);  
      IAfree(&pointEdges);  
      IAfree(&usedPoints);  
      freeFEdge(&fEdge);  
      CHfreeHull(&cHull);  
      ISfree(&iArray);  
    }
}



