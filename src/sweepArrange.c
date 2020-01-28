/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicHeap.h"
#include "basicStack.h"
#include "lineHeap.h"
#include "lineInfo.h"
#include "convexHull.h"
#include "sweepArrange.h"
#include "starPoly.h"
#include "pointIncArray.h"
#include "eralloc.h"
#include <stdio.h>
#include <stdlib.h>

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

int compEvents(caddr_t event1, caddr_t event2)
{
  t_event *ev1, *ev2;  

  ev1 = (t_event *) event1;  
  ev2 = (t_event *) event2;  
  return(compPoints(ev1->point, ev2->point));  
}


void hullEvents(t_pointArray *pArray, t_convexHull *cHull, t_heap *eventHeap)
{
  t_polyLine pOnHull;  
  t_event *curEvent;  
  int count;  
  
  /* create events for convex hull */
  pOnHull = CHgetPoints(cHull);  
  for (count = 2;  count <= BLsizeOf(&pOnHull);  count++)
    {
      curEvent = (t_event *) eralloc(sizeof(t_event));  

      curEvent->eventType = HULL_UPDATE;  
      curEvent->pIndex = BLgetPIndex(&pOnHull, count);  
      curEvent->point = PAgetPoint(pArray, curEvent->pIndex);  
      
      BHaddElem(eventHeap, (caddr_t)curEvent, compEvents);  
    }
}


t_lInfoP *createLines(t_pointArray *pArray, int *nrOfLines, t_heap *eventHeap)
{
  t_lInfoP *lineArray, *curALine, *vertALine;  
  t_lInfoP curLine;  
  t_event *curEvent;  
  int nrOfPoints, vertical;  
  int count, count2;  

  nrOfPoints = PAnrOfPoints(pArray);  
  *nrOfLines = ((nrOfPoints*nrOfPoints)-nrOfPoints)/2;  

  lineArray = (t_lInfoP *) ercalloc((*nrOfLines), sizeof(t_lInfoP));  
  curALine = lineArray;  
  vertALine = (lineArray+(*nrOfLines)-1);  
  vertical = 0;  
  
  for (count=1;  count < nrOfPoints;  count++)
    {
      for (count2=count+1;  count2<=nrOfPoints;  count2++)
	{
	  curLine = (t_lInfoP) eralloc(sizeof(t_lInfo));  
	  curLine->lastTouched = 0;  
	  curLine->curEdge = NULL;  
	  BNcreatePLine(&(curLine->line), count, count2, pArray);  

	  /* special treatment for vertical lines */
	  if (BNisVertical(&(curLine->line)))
	    {
	      vertical++;  
	      curEvent = (t_event *) eralloc(sizeof(t_event));  

	      curEvent->eventType = VERTICAL;  
	      curEvent->point = PAgetPoint(pArray, count);  
	      curEvent->line1 = curLine;  
	      /* if we have a vertical line, make sure we encounter 
		 this event before other events with the same x-coordinate */

	      curEvent->point.y = X_MIN - DELTA;  
	      
	      BHaddElem(eventHeap, (caddr_t)curEvent, compEvents);  	      
	      
	      /* save vertical lines at the end of the array, so that 
		 we can deallocate them properly! */
	      *vertALine = curLine;  
	      vertALine--;  
	    }
	  else  
	    {
	      /* we have a normal line, add it to the array, that is 
		 increment the counter so we will not overwrite it */
	      *curALine = curLine;  
	      curALine++;  
	    }
	}
    }
  (*nrOfLines) -= vertical;  
  return(lineArray);  
}

void lineEvents(t_pointArray *pArray, t_lInfoP *theLines, int nrOfLines, 
		t_convexHull *cHull, t_heap *eventHeap)
{
  int count, swapDone;  
  t_lInfoP auxLine;  
  t_point isectPoint;  
  t_event *curEvent;  
  

  do{
    swapDone = FALSE;  
    for (count=0;  count<nrOfLines-1;  count++)
      {
	if (LIcompMaxLines(*(caddr_t *)(theLines+count), 
			  *(caddr_t *)(theLines+count+1)) == -1)
	  {
	    auxLine = *(theLines+count);  
	    *(theLines+count) = *(theLines+count+1);  
	    *(theLines+count+1) = auxLine;  
	    swapDone = TRUE;  
	      
	    if (BNisectLines(&((*(theLines+count))->line), 
			     &((*(theLines+count+1))->line), 
			     &isectPoint) && 
		CHisInBox(cHull, &isectPoint))
	      {
		curEvent = (t_event *) eralloc(sizeof(t_event));  

		curEvent->eventType = NORMAL;  
		curEvent->point = isectPoint;  

		curEvent->line1 = *(theLines+count);  
		curEvent->line2 = *(theLines+count+1);  
		BHaddElem(eventHeap, (caddr_t)curEvent, compEvents);  	      
	      } 
	  }
      }
  } while (swapDone);  
}

void SAdeallocLines(t_lInfoP *theLines, int nrOfPoints)
{
  int nrOfLines, count;  

  nrOfLines = ((nrOfPoints*nrOfPoints) - nrOfPoints)/2 ;  
  for (count = 0;  count < nrOfLines;  count++)
    erfree((*(theLines+count)));  
    
  erfree(theLines);  
}

void makeArrange(t_arrange *anArrange, t_pointArray *pArray, 
		 t_lInfoP *theLines, int nrOfLines, t_convexHull *cHull, 
		 t_heap *eventHeap)
{
  t_polyLine pOnHull;  
  t_event *curEvent, *nextEvent;  
  t_line topLine, bottomLine;  /* for convex hull */
  t_line lastLine;  
  t_lineInfo lineInfo;  
  t_lInfoP curLInfo, auxLInfo;  
  t_lineHeap inHeap, outHeap;  
  t_point isectPoint;  
  t_edgeInfo *curEdge, *lastEdge, *firstIn, *lastIn, *firstOut, *lastOut;  
  t_face *rFace, *firstFace;  
  int bottomIndex, topIndex;  
  int sameEvent, sameLine, count, stepCounter;  
  int first, beforeTop, afterBottom;  

  firstFace = NULL;  
  /* get the points on the convex hull */
  pOnHull = CHgetPoints(cHull);  
  /* init the first two lines on the convex hull */
  topIndex = BLsizeOf(&pOnHull);  
  BNcreatePLine(&topLine, BLgetPIndex(&pOnHull, 1), 
	       BLgetPIndex(&pOnHull, topIndex), pArray);  

  bottomIndex = 2;  
  BNcreatePLine(&bottomLine, BLgetPIndex(&pOnHull, 1), 
	       BLgetPIndex(&pOnHull, bottomIndex), pArray);  
   
  stepCounter = 1;  
  /* do the sweep, create quadEdge data */

  /* set the variable for first = TRUE => generate outside! */
  first = TRUE;  

  curEdge = NULL;  

  while (!BHisEmpty(eventHeap))
    {
      sameEvent = TRUE;  

      /* create the heaps for incoming and outgoing edges */
      LHinitHeap(&inHeap, HMIN);  
      LHinitHeap(&outHeap, HMAX);  

      while (sameEvent)
	{
	  curEvent = (t_event *) BHgetTop(eventHeap, compEvents);  
	  switch(curEvent->eventType)
	    {
	    case HULL_UPDATE:
	      /* update the pointers for the convex hull */
	      if (topIndex != bottomIndex) /* have we reached t. last point? */
		{
		  if (equalPoints(PAgetPoint(pArray, curEvent->pIndex), 
				  BNgetPoint2(&topLine)))
		    {
		      topIndex--;  
		      BNcreatePLine(&topLine, curEvent->pIndex, 
				    BLgetPIndex(&pOnHull, topIndex), pArray);  
		    }
		  else if (equalPoints(PAgetPoint(pArray, curEvent->pIndex), 
				       BNgetPoint2(&bottomLine)))
		    {
		      bottomIndex++;  
		      BNcreatePLine(&bottomLine, curEvent->pIndex, 
				   BLgetPIndex(&pOnHull, bottomIndex), pArray);  
		    }
		}
	      break;  

	    case VERTICAL: 
	      /* for a vertical line , create all the intersection 
		 points */

	      for (count=0;  count<nrOfLines;  count++)
		if (BNisectLines(&(curEvent->line1->line), 
				  &((*(theLines+count))->line), 
				  &isectPoint) &&
		    (BNisOnSide(&topLine, &isectPoint) != 1) &&
		    (BNisOnSide(&bottomLine, &isectPoint) != -1))
		  /* do we have intersection and does the intersection
		     point lie within the convex hull?? */
		  {
		    nextEvent = (t_event *) eralloc(sizeof(t_event));  
		    
		    nextEvent->eventType = NORMAL;  
		    nextEvent->point = isectPoint;  
		    
		    nextEvent->line1 = curEvent->line1;  
		    nextEvent->line2 = *(theLines+count);  
		    BHaddElem(eventHeap, (caddr_t)nextEvent, compEvents);  
		  }		  
	      break;  
	    case NORMAL:
	      /* we have a normal point: add the current intersecting lines 
		 to top & bottom  heap */
	      isectPoint = curEvent->point;  
	      if ((BNisOnSide(&topLine, &isectPoint) != 1) &&
		  (BNisOnSide(&bottomLine, &isectPoint) != -1))
		{
		  /* point lies within convex Hull 
		     => add it's lines to both stacks */
		  if (curEvent->line1->lastTouched < stepCounter)
		    {
		      LHaddElem(&inHeap, curEvent->line1);  
		      LHaddElem(&outHeap, curEvent->line1);  
		      curEvent->line1->lastTouched = stepCounter;  
		    }
		  if (curEvent->line2->lastTouched < stepCounter)
		    {
		      LHaddElem(&inHeap, curEvent->line2);  
		      LHaddElem(&outHeap, curEvent->line2);  
		      curEvent->line2->lastTouched = stepCounter;  
		    }
		}
	      
	      break;  
	    default:
	      break;  
	    }
	  
	    
	  /* has the next event the same coordinates? => if 
	    yes, process together */
	  sameEvent = ((!BHisEmpty(eventHeap)) && 
		       equalPoints(((t_event *)BHviewTop(eventHeap))->point, 
				   curEvent->point));  
	  erfree(curEvent);  
	}
      
      /* reset the first edge;  we need it for the last new edge! */
      lastEdge = NULL;  
      curLInfo = NULL;  
      firstIn = NULL;  
      firstOut = NULL;  

      /* let's do all the incoming edges, we only have incoming edges if we
	 have a valid intersection point! */
      /* CHECK for parallel lines, else we create loops! */
      while (!LHisEmpty(&inHeap))
	{
	  curLInfo = LHgetTop(&inHeap);  
	  if (curLInfo->curEdge != NULL)
	    {	   
	      if (firstIn == NULL)
		{
		  firstIn = curLInfo->curEdge;  
		}
	      
	      QEsetOrig(curLInfo->curEdge, &isectPoint);  

	      /* set the next pointer! */
	      if ((lastEdge != NULL) && (lastEdge != curLInfo->curEdge))
		{
		  /* edges around the current point! */
		  curLInfo->curEdge->next = lastEdge;  
		  
		  /* edges for the face on the right side! */
		  QEright(lastEdge)->next = QEleft(curLInfo->curEdge);  
		}
	      
	      /* move the pointer for the last edge */
	      lastEdge = curLInfo->curEdge;  
	    }
	}
      /* set the last of the incoming edges */
      lastIn = lastEdge;  

      /* if this is the first point we proceed, we need to generate 
	 the outside face, i.e. the plane our arrangement lies
	 within! */
      if (first)
	{
	  first = FALSE;  
	  firstFace = (t_face *) eralloc(sizeof(t_face));  
	  firstFace->visited = -1;  /* special enumeration for */
	                           /* the first edge */
	}

      /* if we are not on the bottom of the convex hull, we surely 
	 are after the bottom line */
      afterBottom = !BNisOnLine(&bottomLine, &isectPoint);  
      
      /* we surely are before the top! */
      beforeTop = TRUE;  

      /* the "correct" previous edge is firstIn! */
      curEdge = firstIn;  

      /* set lastLine to some idiotic values */
      BNcreateDummy(&lastLine);  

      /* now let's do the outgoing edges */
      while (!LHisEmpty(&outHeap))
	{
	  if (!afterBottom)
	    {
	      /* just preview the top element, 
		 handle it like all other lines */
	      auxLInfo = LHviewTop(&outHeap);  
	      /* we are inisde the CH if the current line is 
		 the same as the bottom line */
	      afterBottom = BNisSame(&(auxLInfo->line), &bottomLine);  
	    }
	    
	  sameLine = FALSE;  
	  lastEdge = curEdge;  
    
	  do {
	    /* while we have parallel lines don't do anything
	       except remembering the current edge */
	    curLInfo = LHgetTop(&outHeap);  

	    /* check whether we have the same line */
	    sameLine = BNisSame(&(curLInfo->line), &(lastLine));  
	    
	    if ((lastEdge != NULL) && (sameLine))
	      {
		/* we only want to do this if we are sure we have parallel
		   edges */
		curLInfo->curEdge = QEsymEdge(lastEdge);  
		QElineInfoInit(&lineInfo, BNgetPIndex1(&(curLInfo->line)), 
			       BNgetPIndex2(&(curLInfo->line)));  
		QEaddLineInfo(curLInfo->curEdge, lineInfo);  
	      }
	  } while ((!LHisEmpty(&outHeap)) && sameLine);  
	  
	  
	  if (afterBottom && beforeTop)
	    {
	      if (!sameLine)
		{
	          
		  /* was the last line we processed parallel to 
		     one already processed? if yes, do nothing! */
		  
		  if (lastEdge == NULL)
		    /* we had no incoming edges => link to outside */
		    rFace = firstFace;  
	          else
		    {
		      if (QElFace(lastEdge) == NULL)
			{
			  /* last edge was outgoing edge => create
			     new face */
			  rFace = (t_face *) eralloc(sizeof(t_face));  
			  rFace->visited = 0;  
			  
			  QEsetLFace(lastEdge, rFace);  
			}
		      /* current edge's right face = left face of last edge! */
		      rFace = QElFace(lastEdge);  
		    }

		  /* create current edge */
		  curEdge = QEnewEdge();  
		    
		  /* set origin, and right face */
		  QEsetOrig(curEdge, &isectPoint);  
		  QEsetRFace(curEdge, rFace);  
		  
		  /* set line information */
		  QElineInfoInit(&lineInfo, BNgetPIndex1(&(curLInfo->line)), 
				 BNgetPIndex2(&(curLInfo->line)));  
		  QEaddLineInfo(curEdge, lineInfo);  
		  
		  /* make link to next edge and for right face */
		  if (lastEdge  != NULL)
		    {
		      /* lastEdge == NULL means we had NO incoming edges
			 => deal with special case! */
		      lastEdge->next = curEdge;  
		      QEright(curEdge)->next = QEleft(lastEdge);  
		    }
		    
		  /* remember first outgoing edge */
		  if (firstOut == NULL)
		    firstOut = curEdge;  
		  
		  /* store symmetric edge of current edge with line 
		     for completion */
		  curLInfo->curEdge = QEsymEdge(curEdge);  
		  /* check whether we have reached the top line of the convex 
		     Hull */
		  beforeTop = (beforeTop && 
			       (!BNisSame(&(curLInfo->line), &topLine)));  
		  /* save last line Info, we need this for checking whether 
		     two lines are the same */
		  lastLine = curLInfo->line;  	      
		}   
	    }	  
	  else
	    {	      
	      /* check whether we have reached the top line of the convex 
		 Hull */
	      beforeTop = beforeTop && (!BNisSame(&(curLInfo->line), &topLine));  
	    }
	}
	
      /* curEdge is the last outgoing edge */
      lastOut = curEdge;  
      
      /* check whether we had incoming edges, else use first outgoing edge
	 we do not need to check wehther we had no outgoing edges, 
	 beacause in this case lastOut = firstIn! */
      if ((lastIn == NULL) && (firstOut != NULL))
	lastIn = firstOut;  
	

      /* did we have any edges at all? If yes, make missing links! */
      if (lastOut != NULL)
	{
	  /* link the outgoing edge to the first incoming */	
	  lastOut->next = lastIn;  
	  
	  /* set the left face of the last segment, make link for right
	     face */
	  QEsetLFace(lastOut, QErFace(lastIn));  
	  QEright(lastIn)->next = QEleft(lastOut);       
	}
	  
      /* increase the stepcounter;  we use it for guaranteeing that 
	 each edge will be added exactly one time to the heap */
      stepCounter++;  

      /* free the current heaps */
      LHfreeHeap(&inHeap);  
      LHfreeHeap(&outHeap);  
    }

    anArrange->firstEdge = curEdge;  
}


void SAcreate(t_arrange *anArrange, t_pointArray *pArray)
{
  t_convexHull cHull;  
  t_heap eventHeap;  
  t_lInfoP *theLines;  
  int nrOfLines;  

  /* first, calculate the convex hull */
  CHcalcHull(&cHull, pArray);  

  /* init the event heap */
  BHinitHeap(&eventHeap);  

  /* create events for convex hull */
  hullEvents(pArray, &cHull, &eventHeap);  

  /* create the lines & events for vertical lines */
  theLines = createLines(pArray, &nrOfLines, &eventHeap);  

  /* sort the lines at the minimum point */
  qsort(theLines, nrOfLines, sizeof(t_lInfoP), LIqsortMinLines);  
  
  /* do bubble sort for maximum point, each swap generates an 
     event for the sweep. We will do a bubble sort here because
     each swap means that the 2 lines intersect and thus we 
     have to clavulate the intersection point */
  lineEvents(pArray, theLines, nrOfLines, &cHull, &eventHeap);  

  /* create the arrangement */
  makeArrange(anArrange, pArray, theLines, nrOfLines, &cHull, &eventHeap);  

  /* free the lines we used for generating the arrangement */
  SAdeallocLines(theLines, PAnrOfPoints(pArray));  
 
  /* free the convex hull */
  CHfreeHull(&cHull);  

}

void markCell(t_edgeInfo *startEdge, t_stack *edgeStack, int markValue, 
	      t_starPoly *currentPoly)
{
  /* mark all the edges of the current cell, check for each edge 
     the other side, if it is not marked, check whether it belongs to
     the same kernel;  if yes, mark neighbour cell, else 
     push the edge */  

  t_edgeInfo *curEdge;  
  t_lineInfo lineInfo;  
  int sameCell;  
  int count;  

  /* first, mark left face of current edge */
  QElFace(startEdge)->visited = markValue;  
  
  curEdge = QElnextEdge(startEdge);  
  
  while (curEdge != startEdge)
    {
      /* we are walking around the left face, so check the right face! */
      if (QErFace(curEdge)->visited == 0)
	{
	  sameCell = TRUE;  
	  /* we need to process the right face of the current edge! */
	  /* check whether the right edge belongs to the same kernel */
	  for (count=1;  count <= QEgetNrOfLines(curEdge);  count++)
	    {
	      lineInfo = QEgetLineInfo(curEdge, count);  
	      sameCell = (sameCell && 
			  (!SPisPolyEdge(currentPoly, 
					 QElineInfoP1(&lineInfo), 
					 QElineInfoP2(&lineInfo))));  
	    }
	  if (sameCell)
	    /* the face on the right belongs to the same kernel */
	    markCell(QEsymEdge(curEdge), edgeStack, markValue, currentPoly);  
	  else   
	    /* the face on the right belongs to another kernel
	       => push edge on stack */
	    BSpushElem(edgeStack, (caddr_t)QEsymEdge(curEdge));  
	}
      /* take a look at the next edge/face */
      curEdge = QElnextEdge(curEdge);  
      
    }
}

void SAkernels(t_pointArray *kernelPoints, t_arrange *anArrange, 
	       t_pointArray *pArray)
{
  t_edgeInfo *curEdge;  
  t_stack edgeStack;  
  t_point p1, p2, p3, pInKernel;  
  t_pIncArray kPoints;  
  t_starPoly curStarPoly;  
  int nrOfKernels;  


  /* we have found no kernel so far, thus set the counter to 0 */
  nrOfKernels = 0;  

  /* init an empty array for the kernels */
  PIinitArray(&kPoints, PAnrOfPoints(pArray));  

  /* init the stack, push the first edge (and|or) it's symmetric edge! */
  BSinitStack(&edgeStack);  
  curEdge = anArrange->firstEdge;  
  if (QElFace(curEdge)->visited == 0)
    BSpushElem(&edgeStack, (caddr_t)curEdge);  
  if (QElFace(QEsymEdge(curEdge))->visited == 0)
    BSpushElem(&edgeStack, (caddr_t)QEsymEdge(curEdge));  

  /* take edge from stack: if it's region has not been 
     marked already, number it, calculate a point within the 
     edge and mark the current cell */
  /* mark all the edges of the current cell */
  while (!BSisEmpty(&edgeStack))
    {
      /* get edge from stack */
      curEdge = (t_edgeInfo *) BSpopElem(&edgeStack);  
      
      /* check if left region has already been marked */
      if (QElFace(curEdge)->visited == 0)
	{
	  /* we found a new kernel */
	  nrOfKernels ++;  

	  /* calculate a point within the current cell */
	  /* NOTE: there is NO possibility that two consecutive 
	     edges lie on the same line! */
	  p1 = QEorig(curEdge);  
	  p2 = QEdest(curEdge);  
	  p3 = QEdest(QElnextEdge(curEdge));  
	  
	  pInKernel.x = (0.4*p1.x + 0.3*p2.x + 0.3*p3.x);   
	  pInKernel.y = (0.4*p1.y + 0.3*p2.y + 0.3*p3.y);   

	  /* add the current point to the list of points within the
	     kernel */
	  PIaddPoint(&kPoints, pInKernel);  
	     
	  /* calculate the current star-shaped polygon defined by the
	     kernel */

	  /* init an empty star-shaped polygon */
	  SPinit(&curStarPoly, PAnrOfPoints(pArray));  
  
	  SPcreatePoly(&curStarPoly, pInKernel, pArray);  
	  
	  /* mark current face and adjacent faces belonging to the same 
	     kernel */
	  markCell(curEdge, &edgeStack, nrOfKernels, &curStarPoly);  
	  
	  /* free the star-shaped polygon */
	  SPfree(&curStarPoly);  
	}
    }  

  /* we have found all the kernels, now freeze the kernel array */
  PIfreezeArray(&kPoints);  

  /* free the stack */
  BSfreeStack(&edgeStack);  

  /* copy to the parameter array */
  PAinitArray(kernelPoints, PInrOfPoints(&kPoints));  
  PAcopyArray(PIgetArray(&kPoints), kernelPoints);  
  PIfreeArray(&kPoints);  
}

void markPush(t_edgeInfo *edge, t_stack *freeStack)
{
  if (edge->visited == 0)
    {
      /* mark current edge as visited */
      edge->visited = -10;   
      QEsymEdge(edge)->visited = -10;   
      QErotEdge(edge)->visited = -10;   
      QEirotEdge(edge)->visited = -10;  

      /* push current edge on stack */
      BSpushElem(freeStack, (caddr_t)QEbaseAddr(edge));  
    }
}

void SAfree(t_arrange *anArrange)
{
  t_stack edgeStack;  
  t_stack freeEStack, freeFStack;  
  t_edgeInfo *curEdge, *nextEdge;  

  caddr_t curElem;  

  /* init both stacks */
  BSinitStack(&edgeStack);  
  BSinitStack(&freeFStack);  
  BSinitStack(&freeEStack);  

  /* put first edge on the stack */
  BSpushElem(&edgeStack, (caddr_t)anArrange->firstEdge);  
  BSpushElem(&edgeStack, (caddr_t)QEsymEdge(anArrange->firstEdge));  
  
  /* put all elements on the stack */
  while (!BSisEmpty(&edgeStack))
    {
      curEdge = (t_edgeInfo *)BSpopElem(&edgeStack);  
      markPush(curEdge, &freeEStack);  

      
      if (QElFace(curEdge)->visited != -10)
	{
	  /* did we already visit the left face? if not, walk around! */

	  /* mark left face as visited */
	  QElFace(curEdge)->visited = -10;  
	  BSpushElem(&freeFStack, (caddr_t)QElFace(curEdge));  
       
	  nextEdge = QElnextEdge(curEdge);  
	  while (nextEdge != curEdge)
	    {
	      /* we walk around the left face, so take a look at the right
		 face */
	      if (QErFace(nextEdge)->visited != -10)
		/* we have to walk around the right face => push edge! */
		BSpushElem(&edgeStack, (caddr_t)QEsymEdge(nextEdge));  
	      
	      /* mark and push current edge */
	      markPush(nextEdge, &freeEStack);  
	      
	      /* procees to next edge */
	      nextEdge = QElnextEdge(nextEdge);  
	    }
	}
    }

  /* free all the faces on the stack */
  while (!BSisEmpty(&freeFStack))
    {
      curElem = BSpopElem(&freeFStack);  
      erfree(curElem);  
    }
    
  /* free all the edges on the stack */
  while (!BSisEmpty(&freeEStack))
    {
      curElem = BSpopElem(&freeEStack);  
      QEfreeEdge((t_edgeInfo *)curElem);  
    }
    
  /* free both stacks */
  BSfreeStack(&edgeStack);  
  BSfreeStack(&freeEStack);  
  BSfreeStack(&freeFStack);  

  
}
