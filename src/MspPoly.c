/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MspPoly.h"
#include "linkPoly.h"
#include "basicLine.h"
#include "intIncArray.h"
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

void MrecPartition(t_linkPoly *lPoly, t_iIncArray *curPoints, 
                   t_pointArray *pArray, int entry, int exit)
{
   int found, auxCount, indexCount, curIndex;  
   int count, reverse;  
   int side, newPIndex, origPIndex;  
   double randomValue;  
   t_intArray usedArray;  
   t_iIncArray leftPoints, rightPoints;  
   t_point midPoint, newPoint, auxPoint, entryP, exitP;  
   t_line baseLine;  
   
   /* if we have only two or three points, link poly */
   if (IIsizeOf(curPoints) == 0)   {
      LPsetNext(lPoly, entry, exit);  
   }
   else if (IIsizeOf(curPoints) == 1)  {
      LPsetNext(lPoly, entry, IIgetInt(curPoints, 1));  
      LPsetNext(lPoly, IIgetInt(curPoints, 1), exit);  
   }
   else  {
      /* we have more points than three, 
         generate polygon by recursion */
      IIinitArray(&leftPoints, IIsizeOf(curPoints));  
      IIinitArray(&rightPoints, IIsizeOf(curPoints));  
      
      /* create the base line from entry to exit */
      BNcreatePLine(&baseLine, entry, exit, pArray);  
      
      
      /* init the array used */
      IAinit(&usedArray, IIsizeOf(curPoints));  
      for (count = 1;   count <= IIsizeOf(curPoints);   count++)
         IAsetInt(&usedArray, count, TRUE);  

      /* choose one point p at random, such that p is not collinear 
         with entry and exit */
      auxCount = IIsizeOf(curPoints);  
      curIndex = 1;  
      do {
         /* choose one point at random */
         indexCount = randomInt(1, auxCount);  
	
         do {
            curIndex = (curIndex % IIsizeOf(curPoints))+1;  
	  
            if (IAgetInt(&usedArray, curIndex))
               indexCount--;  
            
         } while (indexCount > 0);  

         /* we have found a point, check whether it is 
            collinear with the base line!*/
         IAsetInt(&usedArray, curIndex, FALSE);  
         newPIndex = IIgetInt(curPoints, curIndex);  
         newPoint = PAgetPoint(pArray, newPIndex);  
         found = !BNisOnLine(&baseLine, &newPoint);  
         auxCount--;  
      } while ((!found) && (auxCount > 0));  
      
      IAfree(&usedArray);  
      
      if (!found) {
         /* all points are colinear, just sort them! */
         IIsortArray(curPoints);  
         origPIndex = entry;  
         
         /* create the links in the correct order! */
         if (entry < exit)  {
            for (count = 1;   count <= IIsizeOf(curPoints);   count++) {
               newPIndex = IIgetInt(curPoints, count);  
               LPsetNext(lPoly, origPIndex, newPIndex);  
               origPIndex = newPIndex;  
            }
         }
         else  {
	      for (count = IIsizeOf(curPoints);   count >= 1;   count--) {
             newPIndex = IIgetInt(curPoints, count);  
             LPsetNext(lPoly, origPIndex, newPIndex);  
             origPIndex = newPIndex;  
          }
         }
         
         LPsetNext(lPoly, origPIndex, exit);  
      }
      else	{
         /* choose one line through p at random, s.t. entry and 
            exit lie on different sides! */
         entryP = PAgetPoint(pArray, entry);  
         exitP = PAgetPoint(pArray, exit);  
         randomValue = (1.0 - 2.0 * MINDIST)* randomDouble() + MINDIST ;  
         
         midPoint.x = entryP.x + randomValue * (exitP.x-entryP.x);  
         midPoint.y = entryP.y + randomValue * (exitP.y-entryP.y);  
         
         /* check whether entry lies left of the selected 
            line, if not, reverse the line! */
         reverse = (isOnLine(PAgetPoint(pArray, entry), 
                             PAgetPoint(pArray, exit), 
                             PAgetPoint(pArray, newPIndex)) == -1);  
         /* separate the points whether they lie left or right 
            of the line defined by the 2 points */
         origPIndex = newPIndex;  
         
         for (count =1;   count <= IIsizeOf(curPoints);   count++)  {
            if (IIgetInt(curPoints, count) != origPIndex) {
               if (reverse)
                  side = isOnLine(PAgetPoint(pArray, newPIndex), midPoint, 
                                  PAgetPoint(pArray, 
                                             IIgetInt(curPoints, count)));  
               else
                  side = isOnLine(midPoint, PAgetPoint(pArray, newPIndex), 
                                  PAgetPoint(pArray, 
                                             IIgetInt(curPoints, count)));  
               
               if (side == 0) {
                  /* current point is colinear, take the point 
                     that is further away from midPoint! */
                  newPoint = PAgetPoint(pArray, newPIndex);  
                  auxPoint = PAgetPoint(pArray, IIgetInt(curPoints, count));  
                  
                  if (BNdistance(&baseLine, &newPoint) <
                      BNdistance(&baseLine, &auxPoint)) {
                     IIaddInt(&leftPoints, newPIndex);  
                     newPIndex = IIgetInt(curPoints, count);  
                  }
                  else
                     IIaddInt(&leftPoints, IIgetInt(curPoints, count));  
               }
               else if (side == 1)
                  IIaddInt(&leftPoints, IIgetInt(curPoints, count));  
               else
                  IIaddInt(&rightPoints, IIgetInt(curPoints, count));  
            }
         }
         /* freeze left and right arrays */
         IIfreezeArray(&leftPoints);  
         IIfreezeArray(&rightPoints);  
         
         /* recursively draw the "left" polygon with 
            p1 as start and p2 as endpoint */
         MrecPartition(lPoly, &leftPoints, pArray, entry, newPIndex);  
         
         /* recursively draw the "right" polygon with 
            p2 as start and p1 as endpoint */
         MrecPartition(lPoly, &rightPoints, pArray, newPIndex, exit);  
         
         /* free the arrays */
         IIfreeArray(&leftPoints);  
         IIfreeArray(&rightPoints);  
      }
      /* free the arrays */
      IIfreeArray(&leftPoints);  
      IIfreeArray(&rightPoints);  
   }
}


void MspPoly(t_polygon *aPolygon, t_pointArray *pArray, int nrOfPolys, 
             FILE *outFile)
{
  int nrOfPoints, polyCount;  
  int pIndex1, pIndex2, auxIndex;  
  int count, side;  
  t_iIncArray leftPoints, rightPoints;  
  t_linkPoly lPoly;  

  nrOfPoints = PAnrOfPoints(pArray);  
  
  if (nrOfPoints >= 3)
    {
      for (polyCount=1;  polyCount<=nrOfPolys;  polyCount++)
	{
	  /* init the linkPoly */
	  LPinit(&lPoly, nrOfPoints);  

	  /* choose the first 2 points */
	  pIndex1 = randomInt(1, nrOfPoints);  
	  pIndex2 = ((pIndex1 + randomInt(0, nrOfPoints-2)) % nrOfPoints)+1;  

	  /* if pIndex1 > pIndex2, swap the points */
	  if (pIndex1 > pIndex2)
	    {
	      auxIndex = pIndex1;  
	      pIndex1 = pIndex2;  
	      pIndex2 = auxIndex;  
	    }
	
	  /* initialize the arrays for the points on the left and the right 
	     sides of line (pIndex1, pIndex2) */
	  IIinitArray(&leftPoints, nrOfPoints);  
	  IIinitArray(&rightPoints, nrOfPoints);  
	
	  /* separate the points whether they lie left or right 
	     of the line defined by the 2 points */
	  for (count=1;  count <= nrOfPoints;  count++)
	    {
	      if ((count != pIndex1) && (count != pIndex2))
		{
		  side = PAisOnLine(pArray, pIndex1, pIndex2, count);  

		  if (side == 0)
		    {
		      /* if points are colinear with first 2 points, 
			 take the outmost points */
		      if (count < pIndex1)
			/* we do not need to add p1 to the array 
			   just now, because since count < pIndex1, 
			   we will proceed pIndex1 later on! */
			pIndex1 = count;  
		      else if (count > pIndex2)
			{
			  /* we already would have proceeded pIndex2, 
			     so add it to the left side! */
			  IIaddInt(&leftPoints, pIndex2);  
			  pIndex2 = count;  
			}
		      else
			IIaddInt(&leftPoints, count);  
		    }
		  else if (side == 1)
		    IIaddInt(&leftPoints, count);  
		  else
		    IIaddInt(&rightPoints, count);  
		}
	    }
	  /* freeze left and right arrays */
	  IIfreezeArray(&leftPoints);  
	  IIfreezeArray(&rightPoints);  

	  /* recursively draw the "left" polygon with 
	     p1 as start and p2 as endpoint */
	  /* MrecPartition(&lPoly, &leftPoints, pArray, pIndex1, pIndex2);  */
	  MrecPartition(&lPoly, &rightPoints, pArray, pIndex1, pIndex2);  
      
	  /* recursively draw the "right" polygon with 
	     p2 as start and p1 as endpoint */
	  /* MrecPartition(&lPoly, &rightPoints, pArray, pIndex2, pIndex1);  */
	  MrecPartition(&lPoly, &leftPoints, pArray, pIndex2, pIndex1);  

	  /* free the arrays for the left and the right sides */
	  IIfreeArray(&leftPoints);  
	  IIfreeArray(&rightPoints);  
      	
	  /* create a normal polygon from the link polygon */
	  BPfree(aPolygon);  
	  LPtoPoly(&lPoly, aPolygon);  
	  LPfree(&lPoly);  
	  
	  BPnormalize(aPolygon, pArray);  
	  if (outFile != NULL)
	    BPwriteToFile(aPolygon, outFile);  
	}
    }
}

