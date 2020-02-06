/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "M2optPolyII.h"
#include "eralloc.h"
#include "edgeIncArray.h"
#include "binArray.h"
#include "pointArray.h"
#include "linkPoly.h"
#include "geomHash.h"
#include "nodeList.h"
#include <stdio.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#ifdef DO_STATS
static unsigned long optIICount;  
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

int idComp(const void *a, const void *b)
{
   if (*((int*)a) < *((int*)b))
      return -1;
   else if (*((int*)a) > *((int*)b))
      return  1;
   else {
      return  0;
   }
}

inline void incrIndex(int *ind, int nrOfPoints)
{
   if (*ind == nrOfPoints) *ind = 1;
   else                    ++(*ind);

   return;
}


void handleReversal(t_polygon *aPolygon, int start, int end)
{
   int nrOfPoints;
   int *pntIDs = (int*) NULL;
   int nrOfPntIDs = 0;
   int ind, cntr = 0, mid;

   nrOfPoints = BPsizeOf(aPolygon);  
  
   /*                                                                         */
   /* allocate space for the point ids                                        */
   /*                                                                         */
   if (start > end)  nrOfPntIDs = end - start + 1 + nrOfPoints;
   else              nrOfPntIDs = end - start + 1;
   pntIDs = (int *) ercalloc(nrOfPntIDs, sizeof(int));

   /*                                                                         */
   /* obtain point ids                                                        */
   /*                                                                         */
   if (start > end) {
      for (ind = start;  ind <= nrOfPoints;  ++ind) {
         pntIDs[cntr] =  BPgetPIndex(aPolygon, ind);
         ++cntr;
      }
      mid = 1;
   }
   else {
      mid = start;
   }
   for (ind = mid;  ind <= end;  ++ind) {
      pntIDs[cntr] =  BPgetPIndex(aPolygon, ind);
      ++cntr;
   }

   /*
   if (cntr != nrOfPntIDs) {
      printf("handleReversal: numbers don't match!\n");
      exit(1);
   }
   int i;
   for (i = 0;  i < nrOfPntIDs;  ++i) 
      printf("pntIDs[%2d] = %2d\n", i, pntIDs[i]);
   */
   
   /*                                                                         */
   /* sort point ids. recall: a sorted order of the point ids corresponds to  */
   /* a lexicographical order of the points.                                  */
   /*                                                                         */
   qsort(pntIDs, nrOfPntIDs, sizeof(int), idComp);

   /*
   printf("after sorting\n");
   for (i = 0;  i < nrOfPntIDs;  ++i) 
      printf("pntIDs[%2d] = %2d\n", i, pntIDs[i]);
   */

   /*                                                                         */
   /* restore point ids                                                       */
   /*                                                                         */
   cntr = 0;
   if (start > end) {
      for (ind = start;  ind <= nrOfPoints;  ++ind) {
         BPsetPIndex(aPolygon, ind, pntIDs[cntr]);
         ++cntr;
      }
      mid = 1;
   }
   else {
      mid = start;
   }
   for (ind = mid;  ind <= end;  ++ind) {
      BPsetPIndex(aPolygon, ind, pntIDs[cntr]);
      ++cntr;
   }

   erfree(pntIDs);
 
   return;
}


int collinearitiesResolved(t_polygon *aPolygon, t_pointArray *pArray)
{
   int nrOfPoints, ind1, ind2, ind3;
   int i1, i2, i3;
   t_point p1, p2, p3;
   int keep_going = TRUE;
   int cr = FALSE;
   int start = -1;
   int collinear = FALSE;
   int reversal = FALSE;

   nrOfPoints = BPsizeOf(aPolygon);  
   ind1 = 1;  
   ind2 = 2;
   ind3 = 2;
   i1   = BPgetPIndex(aPolygon, ind1);
   p1   = PAgetPoint(pArray, i1);
   i3   = BPgetPIndex(aPolygon, ind2);
   p3   = PAgetPoint(pArray, i3);
   while (keep_going  ||  collinear) {
      i2   = i3;
      p2   = p3;
      incrIndex(&ind3, nrOfPoints);
      i3   = BPgetPIndex(aPolygon, ind3);
      p3   = PAgetPoint(pArray, i3);
      /*
      printf("checking (%f,%f)=%d relative to\n", p3.x, p3.y, i3);
      printf("(%f,%f)=%d and (%f,%f)=%d\n", p1.x, p1.y, i1, p2.x, p2.y, i2); 
      */
      if (0 == isOnLine(p1, p2, p3)) {
         collinear = TRUE;
         if (start == -1) start = ind1;
         //printf("%d, %d, %d are collinear\n", i1, i2, i3);
         if (((i1 < i3)  &&  (i3 < i2))  ||
             ((i1 > i3)  &&  (i3 > i2))  ||
             ((i2 < i1)  &&  (i1 < i3))  ||
             ((i2 > i1)  &&  (i1 > i3))) {
             reversal = TRUE;
         }
      }
      else {
         if (collinear) {
            if (reversal) {
               handleReversal(aPolygon, start, ind2);
               reversal = FALSE;
               cr = TRUE;
            }
            collinear = FALSE;
            start = -1;
         }
      }

      incrIndex(&ind1, nrOfPoints);
      if (ind1 == 1) keep_going = FALSE;
      incrIndex(&ind2, nrOfPoints);
      p1 = p2;
      i1 = i2;
   }
      
   return cr;
}



int eIndex(int pIndex1, int pIndex2, int nrOfPoints)
{
   int index;  
   int i1, i2;  
   
   i1 = MIN(pIndex1, pIndex2);  
   i2 = MAX(pIndex1, pIndex2);  
   
   index = (2*nrOfPoints - i1)*(i1 - 1)/2 + i2 - i1;  
   
   return(index);  
}

#ifdef DO_STATS
void MR2optPolyII()
{
   optIICount = 0;  
}


void MG2optPolyII(unsigned long *tries)
{
   *tries = optIICount;  
}

void MR2optPolyIII()
{
   MR2optPolyII();  
}


void MG2optPolyIII(unsigned long *tries)
{
   MG2optPolyII(tries);  
}

#endif


void delAllIsectsII(int edge, t_pointArray *pArray, 
                    t_edgeIArray *edgeArray, t_binArray *isectArray, 
                    t_intArray *isectCount, int nrOfPoints, int *isects, 
                    int mode)
{
   int count, index;  
   int index11, index12, index21, index22;  
   
   /* delete all the intersections marked for the edge */
   for (count=1;  count<=nrOfPoints;  count++)
      if (count != edge) {
         if (mode)	{
            index = eIndex(edge, count, nrOfPoints);  
            if (BAgetBool(isectArray, index)) {
               BAsetBool(isectArray, index, FALSE);  
               IAdec(isectCount, MIN(edge, count));  
               (*isects)--;  
            }
         }
         else {
            EAgetEdge(edgeArray, edge, &index11, &index12);  
            EAgetEdge(edgeArray, count, &index21, &index22);  
            
            if (PAisectSegments(pArray, index11, index12, index21, index22)) {
               IAdec(isectCount, MIN(edge, count));  
               (*isects)--;  
            }
         }
      }
   
   IAsetInt(isectCount, edge, 0);  
}



void untangleII(int edge1, int edge2, t_edgeIArray *edgeArray, 
                t_binArray *isectArray, t_intArray *isectCount, 
                t_linkPoly *lPoly, t_pointArray *pArray, int nrOfPoints, 
                int *isects, int mode)
{
   int newNext, nextPIndex, nextPIndex2, oldPIndex;  
   int count, count2;  
   int dest1, dest2, orig1, orig2;  
   int index1, index2;  
   int mustSwap;  
   int edge1PIndex1, edge1PIndex2, edge2PIndex1, edge2PIndex2;  
   
   /* get both origins, both destinations */
   EAgetEdge(edgeArray, edge1, &edge1PIndex1, &edge1PIndex2);  
   EAgetEdge(edgeArray, edge2, &edge2PIndex1, &edge2PIndex2);  
   
   
   /* edge 1 */
   if (LPgetNext(lPoly, edge1PIndex1) == edge1PIndex2) {
      orig1 = edge1PIndex1;  
      dest1 = edge1PIndex2;  
   }
   else {
      orig1 = edge1PIndex2;  
      dest1 = edge1PIndex1;  
   }
   
   
   /* edge 2 */
   if (LPgetNext(lPoly, edge2PIndex1) == edge2PIndex2) {
      orig2 = edge2PIndex1;  
      dest2 = edge2PIndex2;  
   }
   else {
      orig2 = edge2PIndex2;  
      dest2 = edge2PIndex1;  
   }
   
   /* first case: we have collinear points where one segment 
      lies completely within the other */
   /* we know that e1p1 <= e1p2, e1p1 <= e2p1, e2p1 < e2p2 
      and e1p2 < e1p1 (this is necessary for intersection!) 
      so all we have to check for case one is whether 
      e2p2 < e1p2 */
   if ((PAisOnLine(pArray, orig1, dest1, orig2) == 0) && 
       (PAisOnLine(pArray, orig1, dest1, dest2) == 0) &&
       (edge2PIndex2 <= edge1PIndex2)) {
      /* special case: both edges share an endpoint */
      if (orig1 == dest2) {
         /* first, find the point that lies before o2 */
         nextPIndex = dest1;  
         while (LPgetNext(lPoly, nextPIndex) != orig2)
            nextPIndex = LPgetNext(lPoly, nextPIndex);  
         
         /* find the edge in the array*/
         count = 0;  
         do {
            count++;  
            EAgetEdge(edgeArray, count, &index1, &index2);  
         } while (((index1 != orig2) || (index2 != nextPIndex)) &&
                  ((index1 != nextPIndex) || (index2 != orig2)));  
         
         
         LPsetNext(lPoly, nextPIndex, orig1);  
         LPsetNext(lPoly, orig1, orig2);  
         LPsetNext(lPoly, orig2, dest1);  
         
         EAsetEdge2(edgeArray, pArray, edge1, nextPIndex, orig1);  
         
         EAsetEdge2(edgeArray, pArray, edge2, orig1, orig2);  
         
         EAsetEdge2(edgeArray, pArray, count, orig2, dest1);  
         
      }
      else if (dest1 == orig2) {
         nextPIndex = LPgetNext(lPoly, dest2);  
         /* find the edge in the array*/
         
         count = 0;  
         do {
            count++;  
            EAgetEdge(edgeArray, count, &index1, &index2);  
         } while (((index1 != dest2) || (index2 != nextPIndex)) &&
                  ((index1 != nextPIndex) || (index2 != dest2)));  
         
         
         LPsetNext(lPoly, dest1, nextPIndex);  
         LPsetNext(lPoly, orig1, dest2);  
         LPsetNext(lPoly, dest2, dest1);  
         
         EAsetEdge2(edgeArray, pArray, edge1, dest1, nextPIndex);  
         
         EAsetEdge2(edgeArray, pArray, edge2, orig1, dest2);  
         
         EAsetEdge2(edgeArray, pArray, count, dest2, dest1);  
      }
      else {
         /* first, relink the edge that lies within the other */
         /* first, find the point that lies before o2 */
         nextPIndex = dest1;  
         while (LPgetNext(lPoly, nextPIndex) != orig2)
            nextPIndex = LPgetNext(lPoly, nextPIndex);  
         
         count = 0;  
         do {
            count++;  
            EAgetEdge(edgeArray, count, &index1, &index2);  
         } while (((index1 != orig2) || (index2 != nextPIndex)) &&
                  ((index1 != nextPIndex) || (index2 != orig2)));  
         
         
         nextPIndex2 = LPgetNext(lPoly, dest2);  
         count2 = 0;  
         do {
            count++;  
            EAgetEdge(edgeArray, count, &index1, &index2);  
         } while (((index1 != dest2) || (index2 != nextPIndex2)) &&
                  ((index1 != nextPIndex2) || (index2 != dest2)));  
         
         LPsetNext(lPoly, nextPIndex, nextPIndex);  
         EAsetEdge2(edgeArray, pArray, edge1, nextPIndex, nextPIndex2);  
         /* relink orig1, dest1, orig2, dest2 */
         if ((orig1 < dest1) == (orig2 < dest2)) {
            /* we have o1 => o2 => d2 => d1 */
            LPsetNext(lPoly, orig1, orig2);  
            EAsetEdge2(edgeArray, pArray, edge2, orig1, orig2);  
            
            LPsetNext(lPoly, orig2, dest2);  
            EAsetEdge2(edgeArray, pArray, count, orig2, dest2);  
            
            LPsetNext(lPoly, dest2, dest1);  
            EAsetEdge2(edgeArray, pArray, count2, dest2, dest1);  
         }
         else {
            /* we have o1 => d2 => o2 => d1 */
            LPsetNext(lPoly, orig1, dest2);  
            EAsetEdge2(edgeArray, pArray, edge2, orig1, dest2);  
            
            LPsetNext(lPoly, dest2, orig2);  
            EAsetEdge2(edgeArray, pArray, count, dest2, orig2);  
            
            LPsetNext(lPoly, orig2, dest1);  
            EAsetEdge2(edgeArray, pArray, count2, orig2, dest1);  
         }
      }
   }
   /* other case: link origin with origin, destination with destination */
   else  {
      /* check whether Index 1 lies within d2->o1, 
         if yes, swap edge 1 and 2! 
         Otherwise, we would loose the counterclockwise orientation on 
         our polygon! */
      nextPIndex = dest2;  
      oldPIndex = LPgetNext(lPoly, orig1);  
      mustSwap = FALSE;  
      
      while ((nextPIndex != oldPIndex) && !mustSwap) {
         mustSwap = (nextPIndex == 1);  
         nextPIndex = LPgetNext(lPoly, nextPIndex);  
      }
      
      /* point 1 lies on the chain from d2->o1, so we must invert the 
         other half of the polygon! => swap edge1 and edge2 */
      if (mustSwap)	{
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
      EAsetEdge2(edgeArray, pArray, edge1, dest2, dest1);  
      
      EAsetEdge2(edgeArray, pArray, edge2, orig2, orig1);  
   }
   
}



void checkAndUntangle(  t_edgeIArray *edgeArray, t_binArray *isectArray, 
                        t_intArray *isectCount, t_linkPoly *lPoly, 
                        t_pointArray *pArray, int nrOfPoints, 
                        int count, int count2) 
{   
   int index11, index12, index21, index22;  
   
   if (count != count2) {
      EAgetEdge(edgeArray, count, &index11, &index12);  
      EAgetEdge(edgeArray, count2, &index21, &index22);  
      
      
      /* If the lines intersect, we have to untangle them */
      if (PAisectSegments(pArray, index11, index12, index21, index22)) {
         if ((index11 != index21) && (index11 != index22) && 
             (index12 != index21) && (index12 != index22)) {
            untangleII(count, count2, edgeArray, isectArray, isectCount, 
                       lPoly, pArray, nrOfPoints, 0, FALSE);  
         }
      }
   }
}



void new2optPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
                 FILE *outFile, int mode)
{
   int nrOfPoints, polyCount;  
   int restart = 0;
   
   /* get the number of points */
   nrOfPoints = PAnrOfPoints(pArray);  
   
   if (nrOfPoints >= 3)  {
      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++) {
         /* init the link Polygon */
         BPfree(aPolygon);  
         BPinit(aPolygon, nrOfPoints);  
         BPrandomPerm(aPolygon);  
         
         /* now we have a polygon, let's make it simple! */
         do {
            MnewDo2optMoves(aPolygon, pArray, nrOfPoints, mode);  
            restart = collinearitiesResolved(aPolygon, pArray);
            //if (restart) printf("collinearities resolved; need to rerun 2opt!\n");
         } while (restart);

         if (outFile != NULL)
            BPwriteToFile(aPolygon, outFile);  
         
         /*
         if (!BPisSimple(aPolygon, pArray))  {
            printf("ERROR! POLYGON NOT SIMPLE!\n");  
            //exit(1);  
         }
         */
      }
   }
}




void MnewDo2optMoves(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPoints, 
                     int mode)
{
   int index;  
   int count, count2, dirty, dsect;  
   int oldIndex;  
   t_intArray isectCount;  
   t_linkPoly lPoly;  
   t_binArray isectArray;  
   t_edgeIArray edgeArray;  
   nodeList *lines;  
   listNode *li1, *li2, *li1next, *li2next;  
   
   LPfromPoly(&lPoly, aPolygon);  
   
   /* convert the polygon to an edgeIncArray */
   EAinitArray2(&edgeArray, nrOfPoints);  
   
   oldIndex = nrOfPoints;  
   for (index = 1;  index <= nrOfPoints;  index++) {
      EAaddEdge2(&edgeArray, pArray, BPgetPIndex(aPolygon, oldIndex), 
                 BPgetPIndex(aPolygon, index));  
      oldIndex = index;  
   }
   
   /* find the intersection according to randomIndex, */
   
   dirty = EAhasDirtySectors(&edgeArray);  
   
   do {
      dsect=randomInt(0, dirty-1);  
      lines = EAgetLinesOfDirtySector(&edgeArray, dsect);  
      li1 = NLGetIter(lines);  
      EAcleanSector(&edgeArray, dsect);  
      while (li1 != NULL ) {
         li1next = li2 = li1->next;  
         
         if (EAvalidateEdge(&edgeArray, 
                            ((lineElement *)li1->data)->lc->lineNumber+1, 
                            ((lineElement *)li1->data)->p1, 
                            ((lineElement *)li1->data)->p2) == 0) {
            //debug//									printf ("   Delete First: %i\n", ((lineElement *)li1->data)->lc->lineNumber +1);  				
            
            lineElement_del((lineElement *)li1->data);  
            NLDelByListNode(lines, li1);  
         }
         else {
            //debug//								printf ("   First: %i\n", ((lineElement *)li1->data)->lc->lineNumber +1);  				
            while (li2 != NULL) {
               li2next = li2->next;  
               
               /* check whether the Edge is out of date */				
               if (EAvalidateEdge(&edgeArray, 
                                  ((lineElement *)li2->data)->lc->lineNumber+1,
                                  ((lineElement *)li2->data)->p1, 
                                  ((lineElement *)li2->data)->p2)==0) {
                  //debug//   							  printf ("  Delete Second: %i\n", ((lineElement *)li2->data)->lc->lineNumber +1);  				
                  if (li1next == li2) {
                     //If we deleted the Upcoming First_Element we have to set it to the next
                     li1next = li2next;  
                  }
                  lineElement_del((lineElement *)li2->data);  
                  NLDelByListNode(lines, li2);  
                  
                  li2=li2next;  
               }
               else {
                  //debug// 									printf ("  Second: %i\n", ((lineElement *)li2->data)->lc->lineNumber +1);  				
                  count = ((lineElement *)li1->data)->lc->lineNumber +1;  
                  count2 = ((lineElement *)li2->data)->lc->lineNumber +1;  
                  checkAndUntangle(&edgeArray, &isectArray, &isectCount, 
                                   &lPoly, pArray, 
                                   nrOfPoints, count, count2);  
                  li2 = li2->next;  
               }
            }
         }
         li1 = li1next;  
      }
      dirty = EAhasDirtySectors(&edgeArray);  
   } while (dirty > 0);  
   
   /* DEBUG
      printf ("Generated lineElements: %i\n", maxInUse());  
      printf ("Consumed Memory: %ikB\n", (maxInUse() *sizeof(struct lineElement))/1024);  
      printf ("Used lineElements: %i\n", maxUsedEL());  
      
      printf ("Generated nodes: %i\n", NLGetMaxNodes());  
      printf ("Consumed Memory: %ikB\n", (NLGetMaxNodes() *sizeof(struct listNode))/1024);  	
   */
   
   BPfree(aPolygon);  
   LPtoPoly(&lPoly, aPolygon);  
   oldIndex = nrOfPoints;  
   LPfree(&lPoly);  
   BPnormalize(aPolygon, pArray);  
   EAfreeArray2(&edgeArray);  
}


void M2optPolyII(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
                 FILE *outFile)
{
   new2optPoly(aPolygon, pArray, nrOfPolys, outFile, FALSE);  
}


void M2optPolyIII(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
                  FILE *outFile)
{
   new2optPoly(aPolygon, pArray, nrOfPolys, outFile, FALSE);  
}

