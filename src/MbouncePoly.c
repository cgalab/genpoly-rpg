/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MbouncePoly.h"
#include "basicGeom.h"
#include "pointArray.h"
#include "sortPoly.h"

/* define the following constant for rather moving the points 
   instead of generating new ones */
#ifdef BOUNCE_POINTS
/* #undef BOUNCE_POINTS */
#endif
/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

#ifdef DO_STATS
static unsigned long bounceCount;  
#endif

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
#ifdef DO_STATS
void MRbouncePoly()
{
  bounceCount = 0;  
}


void MGbouncePoly(unsigned long *tries, double *radius)
{
  *tries = bounceCount;  
  *radius = MOVE_RADIUS;  
}
#endif



int checkNeighbours(t_pointArray *pArray, t_polygon *aPolygon, int pIndex, 
                    t_point newPoint)
{
  int nrOfPoints;  
  int res, count, curIndex, prevIndex, nextIndex;  
  t_point prevPoint, nextPoint;  
  t_point linePoint1, linePoint2;  

  nrOfPoints = PAnrOfPoints(pArray);  
  
  curIndex = BPgetPIndex(aPolygon, pIndex);  
  prevIndex = ((nrOfPoints+pIndex-2)%nrOfPoints)+1;  
  nextIndex = ((pIndex)%nrOfPoints)+1;  
  prevPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, prevIndex));  
  nextPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, nextIndex));  

  count = 0;  
  res = TRUE;  

  while ((count < nrOfPoints-2) && res)
  {
    linePoint1 = PAgetPoint(pArray, 
                   BPgetPIndex(aPolygon, ((pIndex+count)%nrOfPoints)+1));  
    linePoint2 = PAgetPoint(pArray, 
                   BPgetPIndex(aPolygon, ((pIndex+count+1)%nrOfPoints)+1));  
    res = ((!isectSegments(newPoint, prevPoint, linePoint1, linePoint2)) &&
          (!isectSegments(newPoint, nextPoint, linePoint1, linePoint2)));  
    count++;  
  }

  return(res);  
}



void MbouncePoly(t_pointList *aList, t_polygon *aPolygon, 
		 t_pointArray *pArray, int nrOfMoves)
{
  int count, polyIndex, curIndex;  
#ifdef BOUNCE_POINTS
  t_point oldPoint;  
  t_point moveVec;  
  double randRadius;  
#endif
  t_point newPoint;  
  t_polygon tempPoly;  
  t_pointArray tempArray;  
  
  double randomX, randomY;  

  BPfree(aPolygon);  

  if (PAnrOfPoints(pArray) >= 3)
    {	
      sortPFirst(pArray, aPolygon);  
	
      for (count=0;  count < nrOfMoves;  count++)
	{
	  for (polyIndex=1;  polyIndex <= BPsizeOf(aPolygon);  polyIndex++)
	    {
	      curIndex = BPgetPIndex(aPolygon, polyIndex);  
#ifdef BOUNCE_POINTS
	      oldPoint = PAgetPoint(pArray, curIndex);  
#endif
		
	      do {
#ifdef BOUNCE_POINTS
		moveVec.x = randomDouble() - 0.5;  
		moveVec.y = randomDouble() - 0.5;  

		/* normalize the vector and multiply it with radius! */
		normVec(&moveVec);  
		randRadius = randomDouble() * MOVE_RADIUS;  
		moveVec.x *= randRadius;  
		moveVec.y *= randRadius;  

		newPoint.x = oldPoint.x + (moveVec.x*(X_MAX-X_MIN));  
		newPoint.y = oldPoint.y + (moveVec.y*(Y_MAX-Y_MIN));  
#else
		randomX = randomDouble();  
		randomY = randomDouble();  
		newPoint.x = X_MIN + (randomX*(X_MAX-X_MIN));  
		newPoint.y = Y_MIN + (randomY*(Y_MAX-Y_MIN));  
#endif
#ifdef DO_STATS
		bounceCount++;  
#endif
	      } while (!PAvalidPoint(pArray, newPoint, polyIndex) ||
		       !checkNeighbours(pArray, aPolygon, polyIndex, newPoint));  
	      PAsetPoint(pArray, curIndex, newPoint);  
	    }
	}
      
      PAinitArray(&tempArray, PAnrOfPoints(pArray));  
      PAcopyArray(pArray, &tempArray);  
      BPinit(&tempPoly, BPsizeOf(aPolygon));  
      BPcopyPoly(aPolygon, &tempPoly);  

      PAsortArray(pArray);  

      PAcreateList(pArray, aList);  
      for (count=1;  count<=BPsizeOf(aPolygon);  count++)
	{
	  BPsetPIndex(aPolygon, count, 
		      PAgetPIndex(pArray, 
				  PAgetPoint(&tempArray, 
					     BPgetPIndex(&tempPoly, count))));  
	}

      PAfreeArray(&tempArray);  
      BPfree(&tempPoly);  

      BPnormalize(aPolygon, pArray);  
    }
  
}

