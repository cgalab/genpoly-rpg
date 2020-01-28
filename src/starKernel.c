/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

/* define MPOLYPOLY if you want to save the polygons as polygons and not
   as kernel points, undefine it if you don't */

/* SINCE THE CURRENT IMPLEMENTATION DOES NOT SUPPORT THE GENERATION 
   OF A POINT IN THE KERNEL, MPOLYPOLY must be defined! */
#define MPOLYPOLY /**/

#include "starKernel.h"
#include "eralloc.h"
#include "lineInfo.h"
#include "convexHull.h"
#include "basicLine.h"
#include "basicHeap.h"
#include "polyAVL.h"
#include "polyIncArray.h"
#include "starPoly.h"
#include <string.h>
#include <stdlib.h>

int SKcompPoints(caddr_t p1, caddr_t p2)
{
  return(compPoints(((t_isectPoint *)p1)->point, ((t_isectPoint *)p2)->point));  
}

int SKcompLines(t_line *line1, t_line *line2, double xcoord)
{
  int res;  
  t_point p1, p2;  

  p1.x = xcoord;  
  p2.x = xcoord;  
  BNsetYcoord(line1, &p1);  
  BNsetYcoord(line2, &p2);  

  if (deltaAbs(p1.y-p2.y))
    res = 0;  
  else if (p1.y < p2.y)
    res = 1;  
  else
    res = -1;  

  return(res);  
}

int SKqsortMinLines(const void *l1, const void *l2)
{
  t_line *line1, *line2;  
  int res;  
 
  line1 = (t_line *) l1;  
  line2 = (t_line *) l2;  

  /* handle vertical lines */
  if (BNisVertical(line1) && BNisVertical(line2))
    if (line1->a < line2->a)
      res = -1;  
    else if (line1->a == line2->a)
      res = 0;  
    else
      res = 1;  
  else if (BNisVertical(line1))
    res = -1;  
  else if (BNisVertical(line2))
    res = 1;  
  else
    res = (-SKcompLines(line1, line2, COMP_MIN));  
  return(res);  
}


int SKqsortMaxLines(const void *l1, const void *l2)
{
  t_line *line1, *line2;  
  int res;  

  line1 = (t_line *) l1;  
  line2 = (t_line *) l2;  
  /* handle vertical lines */
  if (BNisVertical(line1) && BNisVertical(line2))
    if (line1->a < line2->a)
      res = -1;  
    else if (line1->a == line2->a)
      res = 0;  
    else
      res = 1;  
  else if (BNisVertical(line1))
    res = 1;  
  else if (BNisVertical(line2))
    res = -1;  
  else
    res = (-SKcompLines(line1, line2, COMP_MAX));  
  return(res);  
}

t_line *SKinitLines(t_pointArray *pArray)
{
  t_line *lineArray;  
  int nrOfPoints, nrOfLines;  

  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfLines = ((nrOfPoints*nrOfPoints)-nrOfPoints)/2;  

  /* create the array */
  lineArray = (t_line *) ercalloc(nrOfLines, sizeof(t_line));  
  
  return(lineArray);  
}

void SKcreateLines(t_pointArray *pArray, t_line *minSortArray, 
		   t_line *maxSortArray)
{
  int nrOfPoints, nrOfLines;  
  int count, count2;  
  t_line *curLine;  

  /* create all the lines */
  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfLines = ((nrOfPoints*nrOfPoints)-nrOfPoints)/2;  

  curLine = minSortArray;  

  for (count=1;  count < nrOfPoints;  count++)
    {
      for (count2=count+1;  count2<=nrOfPoints;  count2++)
	{
	  BNcreatePLine(curLine, count, count2, pArray);  
	  curLine++;  
	}
    }

  /* copy minSortArray to maxSortArray */
  /*  bcopy(minSortArray, maxSortArray, nrOfLines*sizeof(t_line));  */
  memcpy(maxSortArray, minSortArray, nrOfLines*sizeof(t_line));   

  /* sort both arrays */
  qsort(minSortArray, nrOfLines, sizeof(t_line), SKqsortMinLines);  
  qsort(maxSortArray, nrOfLines, sizeof(t_line), SKqsortMaxLines);  
  
}

void SKcreateAll(t_point isectPoint, t_starPoly *sPoly1, t_starPoly *sPoly2, 
		 t_starPoly *sPoly3, t_starPoly *sPoly4, t_line *curLine, 
		 t_pointArray *pArray, t_convexHull *cHull)
{
  int count;  
  int swapDone;  
  int pI1, pI2;  
  t_point p1, p2;  

  SPinit(sPoly1, PAnrOfPoints(pArray));  
  SPinit(sPoly2, PAnrOfPoints(pArray));  
  SPinit(sPoly3, PAnrOfPoints(pArray));  
  SPinit(sPoly4, PAnrOfPoints(pArray));  
  SPcreatePoly(sPoly1, isectPoint, pArray);  
  SPcopyPoly(sPoly1, sPoly2);  
  SPcopyPoly(sPoly1, sPoly3);  
  SPcopyPoly(sPoly1, sPoly4);  
  
  /* check the colinear points */
  /* i.e. do a bubble sorts of all the points that lie 
     on the current edge */
  /* We do NOT need to check point 1, since it always has to be 
     on the first position! */
  /* NOTE! We must NOT swap points that lie on the convex hull! */
  do {
    swapDone = FALSE;  
    
    for (count=2;  count < PAnrOfPoints(pArray);  count++)
      {
	/* check if both points lie on the current line, 
	   and if we have to swap them */
	pI1 = SPgetPIndex(sPoly1, count);  
	pI2 = SPgetPIndex(sPoly1, count+1);  
	p1 = PAgetPoint(pArray, pI1);  
	p2 = PAgetPoint(pArray, pI2);  
	
	if (BNisOnLine(curLine, &p1) &&
	   BNisOnLine(curLine, &p2) &&
	   (pI1 < pI2) && 
	   (!CHisOnHull(cHull, pI1) || !CHisOnHull(cHull, pI2)))
	  {
	    swapDone = TRUE;  
	    SPswapPoints(sPoly1, count, count+1);  
	  }
      }
  } while (swapDone);  
  
  /* polygon 2 */
  do {
    swapDone = FALSE;  
    
    for (count=2;  count < PAnrOfPoints(pArray);  count++)
      {
	/* check if both points lie on the current line, 
	   and if we have to swap them */
	pI1 = SPgetPIndex(sPoly2, count);  
	pI2 = SPgetPIndex(sPoly2, count+1);  
	p1 = PAgetPoint(pArray, pI1);  
	p2 = PAgetPoint(pArray, pI2);  
	
	if (BNisOnLine(curLine, &p1) &&
	   BNisOnLine(curLine, &p2) &&
	   (pI1 > pI2) && 
	   (!CHisOnHull(cHull, pI1) || !CHisOnHull(cHull, pI2)))
	  {
	    swapDone = TRUE;  
	    SPswapPoints(sPoly2, count, count+1);  
	  }
      }
  } while (swapDone);  
  /* polygon 3 */
  do {
    swapDone = FALSE;  
    
    for (count=2;  count < PAnrOfPoints(pArray);  count++)
      {
	/* check if both points lie on the current line, 
	   and if we have to swap them */
	pI1 = SPgetPIndex(sPoly3, count);  
	pI2 = SPgetPIndex(sPoly3, count+1);  
	p1 = PAgetPoint(pArray, pI1);  
	p2 = PAgetPoint(pArray, pI2);  
	
	if (BNisOnLine(curLine, &p1) &&
	   BNisOnLine(curLine, &p2) &&
	   (((pI1 > pI2) &&
	     compPoints(p1, isectPoint) &&
	     compPoints(p2, isectPoint)) ||
	    ((pI1 < pI2) &&
	     compPoints(isectPoint, p1) &&
	     compPoints(isectPoint, p2))) && 
	   (!CHisOnHull(cHull, pI1) || !CHisOnHull(cHull, pI2)))
	   {
	     swapDone = TRUE;  
	     SPswapPoints(sPoly3, count, count+1);  
	   }
      }
  } while (swapDone);  

  /* polygon 4 */
  do {
    swapDone = FALSE;  
    
    for (count=2;  count < PAnrOfPoints(pArray);  count++)
      {
	/* check if both points lie on the current line, 
	   and if we have to swap them */
	pI1 = SPgetPIndex(sPoly4, count);  
	pI2 = SPgetPIndex(sPoly4, count+1);  
	p1 = PAgetPoint(pArray, pI1);  
	p2 = PAgetPoint(pArray, pI2);  
	
	if (BNisOnLine(curLine, &p1) &&
	   BNisOnLine(curLine, &p2) &&
	   (((pI1 < pI2) &&
	     compPoints(p1, isectPoint) &&
	     compPoints(p2, isectPoint)) ||
	    ((pI1 > pI2) &&
	     compPoints(isectPoint, p1) &&
	     compPoints(isectPoint, p2))) && 
	   (!CHisOnHull(cHull, pI1) || !CHisOnHull(cHull, pI2)))
	  {
	    swapDone = TRUE;  
	    SPswapPoints(sPoly4, count, count+1);  
	  }
      }
  } while (swapDone);  
  
}


void SKcreatePolys(t_line *minLines, t_line *maxLines, 
		   t_yIncArray *kernelPolys, t_pointArray *pArray)
{
  int nrOfPoints, nrOfLines;  
  int count, count2, found;  
  int first, isInside;  
  t_convexHull cHull;  
  t_polyAVL polyTree;  
  t_point minPoint, maxPoint;  
  t_point isectPoint;  
  t_isectPoint *isectPointP;  
  t_point oldPoint, newPoint;  
  t_starPoly sPoly1, sPoly2, sPoly3, sPoly4;  
  t_heap isectPHeap;  

  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfLines = ((nrOfPoints*nrOfPoints)-nrOfPoints)/2;  

  /* create the convex hull */
  CHcalcHull(&cHull, pArray);  

  /* initialize the AVL tree */
  
#ifdef MPOLYPOLY
  YVinitTree(&polyTree);  
#else
  SVinitTree(&polyTree);  
#endif

  /* initialize all 4 star-shaped polygons */
  SPinit(&sPoly1, 0);  
  SPinit(&sPoly2, 0);  
  SPinit(&sPoly3, 0);  
  SPinit(&sPoly4, 0);  

  /* set the start index to 0, because we want to start with 
     the first line of maxLines */

  /* loop over all lines, calculate all the intersections */
  for (count=0;  count < nrOfLines-1;  count++)
    {
      /* get the intersection between the convex hull and
	 the current line */
      CHisectHull(&cHull, (minLines+count), pArray, &minPoint, &maxPoint);  

      /* init the heap */
      BHinitHeap(&isectPHeap);  
      /* add the intersections with the convex hull to the heap */
      isectPointP = (t_isectPoint *) eralloc(sizeof(t_isectPoint));  
      isectPointP->point = minPoint;  
      isectPointP->pIndex1 = -2;  
      isectPointP->pIndex2 = -2;  
      BHaddElem(&isectPHeap, (caddr_t)isectPointP, SKcompPoints);  
      isectPointP = (t_isectPoint *) eralloc(sizeof(t_isectPoint));  
      isectPointP->point = maxPoint;  
      isectPointP->pIndex1 = -2;  
      isectPointP->pIndex2 = -2;  
      BHaddElem(&isectPHeap, (caddr_t)isectPointP, SKcompPoints);  
      
      found = FALSE;  
      count2 = 0;  

      /* calculate all the intersections */
      while (!found)
	{
	  if ((BNgetPIndex1(minLines+count) == BNgetPIndex1(maxLines+count2))&&
	      (BNgetPIndex2(minLines+count) == BNgetPIndex2(maxLines+count2)))
	    {
	      found = TRUE;  
	      BNsetPIndex1((maxLines+count2), -2);  
	    }
	  else if (BNgetPIndex1(maxLines+count2) != -2)
	    {
	      if (BNisectLines((minLines+count), 
			       (maxLines+count2), &isectPoint) && 
		  compPoints(minPoint, isectPoint) && 
		  compPoints(isectPoint, maxPoint))
		{
		  isectPointP = (t_isectPoint *) eralloc(sizeof(t_isectPoint));  
		  isectPointP->pIndex1 = BNgetPIndex1(maxLines+count2);  
		  isectPointP->pIndex2 = BNgetPIndex2(maxLines+count2);  
		  isectPointP->point = isectPoint;  
		 
		  BHaddElem(&isectPHeap, (caddr_t)isectPointP, SKcompPoints);  
		}
	    }
	  count2 ++;  
	}

      first = TRUE;  
      /* proceed all the intersections */	
      while (!BHisEmpty(&isectPHeap))
	{
	  isInside = TRUE;  
	  while (!BHisEmpty(&isectPHeap) && isInside)
	    {
	      /* get the next point from the stack, guarantee that 
		 oldPoint is different from the point we get now */
	      isectPointP = (t_isectPoint *) BHgetTop(&isectPHeap, 
						      SKcompPoints);  
	      oldPoint = isectPointP->point;  
	      if (first)
		isInside = FALSE;  
	      else
		{
		  first = FALSE;  
		  isInside = (!SPisPolyEdge(&sPoly1, isectPointP->pIndex1, 
					    isectPointP->pIndex2) &&
			      !SPisPolyEdge(&sPoly2, isectPointP->pIndex1, 
					    isectPointP->pIndex2) &&
			      !SPisPolyEdge(&sPoly3, isectPointP->pIndex1, 
					    isectPointP->pIndex2) &&
			      !SPisPolyEdge(&sPoly4, isectPointP->pIndex1, 
					    isectPointP->pIndex2));  
		}
	      erfree(isectPointP);  
	  
	    }
	
	  newPoint = oldPoint;  
	  
	  while (!BHisEmpty(&isectPHeap) && equalPoints(newPoint, oldPoint))
	    {	 
	      isectPointP = (t_isectPoint *) BHgetTop(&isectPHeap, 
						      SKcompPoints);  
	      oldPoint = isectPointP->point;  
	      erfree(isectPointP);  
	    }
	   
	  if (!equalPoints(newPoint, oldPoint))
	    {
	      /* we got two distinct points, now calculate 
		 the center point of them and create the 
		 star-shaped polygon */
	      isectPoint.x = (newPoint.x + oldPoint.x) / 2.0;  
	      isectPoint.y = (newPoint.y + oldPoint.y) / 2.0;  
	      
	      /* we have to guarantee that we will not use 
		 a point that is in our point set, otherwise
		 we will not get a meaningfull polygon */
   	      if (!PAisInArray(pArray, isectPoint))
		{
		  SPfree(&sPoly1);  
		  SPfree(&sPoly2);  
		  SPfree(&sPoly3);  
		  SPfree(&sPoly4);  

		  SKcreateAll(isectPoint, &sPoly1, &sPoly2, &sPoly3, &sPoly4, 
			      (minLines+count), pArray, &cHull);  
		  
		  /* save both polygons */
		  
#ifdef MPOLYPOLY
		  YVaddStarElem(&polyTree, &sPoly1);  
		  if (YVcompProc((caddr_t)&sPoly1, (caddr_t)&sPoly2) != 0)
		    YVaddStarElem(&polyTree, &sPoly2);  
		  if (YVcompProc((caddr_t)&sPoly1, (caddr_t)&sPoly3) != 0)
		    YVaddStarElem(&polyTree, &sPoly3);  
		  if (YVcompProc((caddr_t)&sPoly1, (caddr_t)&sPoly4) != 0)
		    YVaddStarElem(&polyTree, &sPoly4);  
#else
		  SVaddElem(&polyTree, &sPoly1, pArray);  
		  if (NVcompProc((caddr_t)&sPoly1, (caddr_t)&sPoly2))
		    SVaddElem(&polyTree, &sPoly2, pArray);  
		  if (NVcompProc((caddr_t)&sPoly1, (caddr_t)&sPoly3))
		    SVaddElem(&polyTree, &sPoly3, pArray);  
		  if (NVcompProc((caddr_t)&sPoly1, (caddr_t)&sPoly4))
		    SVaddElem(&polyTree, &sPoly4, pArray);  
#endif

		  
		}
	    }
	  
	}
      BHfreeHeap(&isectPHeap);  
	      
    }
    
  /* free all 4 polygons */
  SPfree(&sPoly1);  
  SPfree(&sPoly2);  
  SPfree(&sPoly3);  
  SPfree(&sPoly4);  
    
  /* free the convex hull */
  CHfreeHull(&cHull);  
    
  /* convert the AVL tree to an array */
  YIinitArray(kernelPolys, PAnrOfPoints(pArray));  
  
#ifdef MPOLYPOLY
  YVtoArray(&polyTree, kernelPolys, pArray);  
#else
  THIS HAS TO BE ADAPTED!;  
  SVtoArray(&polyTree, pInKernel);  
#endif

  YIfreezeArray(kernelPolys);  
  
}

void SKwritePolys(FILE *outFile, t_line *minLines, t_line *maxLines, 
		    t_pointArray *pArray)
{
  t_yIncArray kernelPolys;  
  int count;  

  SKcreatePolys(minLines, maxLines, &kernelPolys, pArray);  
  
  for (count=1;  count<=YInrOfPolys(&kernelPolys);  count++)
    BPwriteToFile(YIgetPoly(&kernelPolys, count), outFile);  
}

void SKfreeLines(t_line *lineArray)
{
  erfree(lineArray);  
}
