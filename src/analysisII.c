/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "analysisII.h"
#include "basicDoubleArray.h"
#include "basicGeom.h"
#include "convexHull.h"
#include "basicIo.h"
#include <math.h>
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
static int ANnrOfSinu;  
t_intArray ANsinuArr;  
static double ANangle, ANslope, ANlen;  
static t_doubleArray ANangleArr, ANslopeArr, ANlenArr;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void resetAnalysis(t_pointArray *pArray, int lenRes, int slopeRes, 
		   int angleRes, int nrOfPolygons)
{
  resetAnalysisII(lenRes, slopeRes, angleRes, PAnrOfPoints(pArray), 
		  nrOfPolygons);  
}

void resetSinuosityII(int nrOfPoints, int nrOfPolygons)
{
  int count;  

  ANnrOfSinu = nrOfPolygons;  

  IAinit(&ANsinuArr, nrOfPoints);  

  for (count=1;  count<=nrOfPoints;  count++)
    IAsetInt(&ANsinuArr, count, 0);  
}

void resetAnalysisII(int lenRes, int slopeRes, int angleRes, 
		   int nrOfPoints, int nrOfPolygons)
{
  int count;  

  resetSinuosityII(nrOfPoints, nrOfPolygons);  

  ANangle = 0.0;  
  ANslope = 0.0;  
  ANlen = 0.0;  
  DAinit(&ANangleArr, angleRes);  
  DAinit(&ANlenArr, lenRes);  
  DAinit(&ANslopeArr, slopeRes);  

  for (count=1;  count<=angleRes;  count++)
    DAsetDouble(&ANangleArr, count, 0.0);  

  for (count=1;  count<=lenRes;  count++)
    DAsetDouble(&ANlenArr, count, 0.0);  

  for (count=1;  count<=slopeRes;  count++)
    DAsetDouble(&ANslopeArr, count, 0.0);  

}

void sinuWrite(char *fName)
{
  FILE *outFile;  
  int nrOfElems;  
  int count, res;  

  res = FALSE;  

  if ((outFile = FopenWrite(fName)) != NULL)
    {
      nrOfElems = IAsizeOf(&ANsinuArr);  
      
      for (count=1;  count<=nrOfElems;  count++)
	{
	  fprintf(outFile, "%d %f\n", count, 
		  ((double)IAgetInt(&ANsinuArr, count))/((double)ANnrOfSinu));  
	}
      res = TRUE;  
      Fclose(outFile);  
    }
      
}

int resWriteII(char *fName, t_doubleArray *resArray)
{
  FILE *outFile;  
  int nrOfElems;  
  int count, res;  

  res = FALSE;  
  nrOfElems = DAsizeOf(resArray);  

  if ((outFile = FopenWrite(fName)) != NULL)
    {
      for (count=1;  count<=DAsizeOf(resArray);  count++)
	{
	  fprintf(outFile, "%d %f\n", count, DAgetDouble(resArray, count));  
	}
      res = TRUE;  
      Fclose(outFile);  
    }
      
  return(res);  
}

void writeSinuosityII(char *fSinu)
{
  sinuWrite(fSinu);  

  IAfree(&ANsinuArr);  
}

void writeAnalysis(char *fLen, char *fSlope, char *fAngle, char *fSinu)
{
  writeSinuosityII(fSinu);  

  resWriteII(fLen, &ANlenArr);  
  resWriteII(fAngle, &ANangleArr);  
  resWriteII(fSlope, &ANslopeArr);  
  DAfree(&ANangleArr);  
  DAfree(&ANlenArr);  
  DAfree(&ANslopeArr);  
}


void angleDistII(t_pointArray *pArray, t_polygon *aPolygon)
{
  int nrOfPoints, count, curVal;  
  int nrOfClasses;  
  t_point orig, dest;  
  t_intArray hitArray;  
  double step, angle, lastAngle, curAngle;  
  double curRes, newRes;  

  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfClasses = DAsizeOf(&ANangleArr);  

  if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0))
    {
      step = 360.0 / (double)nrOfClasses;  

      IAinit(&hitArray, nrOfClasses);  
      for (count=1;  count<=nrOfClasses;  count++)
	IAsetInt(&hitArray, count, 0);  

      orig = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints-1));  
      dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  

      angle = atan2((dest.y-orig.y), (dest.x-orig.x))*180.0*M_1_PI;  
      
      for (count=1;  count<=nrOfPoints;  count++)
	{
	  lastAngle = 180.0 + angle;  

	  orig = dest;  
	  dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, count));  

	  angle = atan2((dest.y-orig.y), (dest.x-orig.x))*180.0*M_1_PI;  

	  curAngle = lastAngle-angle;  
     
	  while (curAngle < 0.0)
	    curAngle = curAngle + 360.0;  
	  while (curAngle >= 360.0)
	    curAngle = curAngle - 360.0;  

	  curVal = (int) (curAngle/step)+1;  
	
	  if (curVal < 1)
	    {  
	      curVal = 1;  
	      printf("ERROR\n");  
	      exit(1);  
	    }
	  if (curVal > nrOfClasses)
	    {
	      curVal = nrOfClasses;  
	      printf("ERROR\n");  
	      exit(1);  
	    }
	  IAinc(&hitArray, curVal);  
	}
      
      for (count=1;  count<=nrOfClasses;  count++)
	{
	  curRes = DAgetDouble(&ANangleArr, count);  
	  newRes = ((curRes*ANangle)+
		    ((double)IAgetInt(&hitArray, count)/
		     (double)nrOfPoints)) / (ANangle+1);  
	  DAsetDouble(&ANangleArr, count, newRes);  
	}
      ANangle++;  

      IAfree(&hitArray);  
    }
}


void lengthDistII(t_pointArray *pArray, t_polygon *aPolygon)
{
  int nrOfPoints, count, count2, curVal, hullSize;  
  int nrOfClasses;  
  t_point point1, orig, dest;  
  t_intArray hitArray;  
  t_convexHull cHull;  
  t_polyLine pOnHull;  
  double step, maxLen, len;  
  double curRes, newRes;  

  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfClasses = DAsizeOf(&ANlenArr);  

  if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0))
    {

      /* compute the maximum length */
      /* first, we need to compute the convex hull */
      CHcalcHull(&cHull, pArray);  
      
      /* now get the maximum distance */
      maxLen = 0.0;  
      pOnHull = CHgetPoints(&cHull);  
      hullSize = BLsizeOf(&pOnHull);  

      for (count=1;  count<hullSize;  count++)
	{
	  point1 = PAgetPoint(pArray, BLgetPIndex(&pOnHull, count));  
	  for (count2=count+1;  count2<=hullSize;  count2++)
	    {
	      len = lineLength(point1, PAgetPoint(pArray, 
						 BLgetPIndex(&pOnHull, 
							     count2)));  
	      if (len > maxLen)
		maxLen = len;  
	    }
	}
      
      step = maxLen / (double)nrOfClasses;  

      IAinit(&hitArray, nrOfClasses);  
      for (count=1;  count<=nrOfClasses;  count++)
	IAsetInt(&hitArray, count, 0);  

      dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  
      for (count=1;  count<=nrOfPoints;  count++)
	{
	  orig = dest;  
	  dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, count));  

	  len = lineLength(orig, dest);  
	  if (deltaAbs(len-maxLen))
	    curVal = nrOfClasses;  
	  else
	    curVal = (int) (len/step)+1;  
	
	  if (curVal < 1)
	    {  
	      curVal = 1;  
	      printf("ERROR\n");  
	      exit(1);  
	    }
	  if (curVal > nrOfClasses)
	    {
	      curVal = nrOfClasses;  
	      printf("ERROR\n");  
	      exit(1);  
	    }
	  IAinc(&hitArray, curVal);  
	}

      for (count=1;  count<=nrOfClasses;  count++)
	{
	  curRes = DAgetDouble(&ANlenArr, count);  
	  newRes = ((curRes*ANlen)+
		    ((double)IAgetInt(&hitArray, count)/
		     (double)nrOfPoints)) / (ANlen+1);  
	  DAsetDouble(&ANlenArr, count, newRes);  
	}
      ANlen++;  

      IAfree(&hitArray);  
      CHfreeHull(&cHull);  
    }
}


void slopeDistII(t_pointArray *pArray, t_polygon *aPolygon)
{
  int nrOfPoints, count, curVal;  
  int nrOfClasses;  
  t_point orig, dest;  
  t_intArray hitArray;  
  double step, angle;  
  double curRes, newRes;  

  nrOfPoints = PAnrOfPoints(pArray);  
  nrOfClasses = DAsizeOf(&ANslopeArr);  

  if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0))
    {
      step = 360.0 / (double)nrOfClasses;  

      IAinit(&hitArray, nrOfClasses);  
      for (count=1;  count<=nrOfClasses;  count++)
	IAsetInt(&hitArray, count, 0);  

      dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  
      for (count=1;  count<=nrOfPoints;  count++)
	{
	  orig = dest;  
	  dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, count));  

	  angle = atan2((dest.y-orig.y), (dest.x-orig.x))*180.0*M_1_PI;  
	  if (angle < 0.0)
	    angle = angle + 360.0;  

	  curVal = (int) (angle/step)+1;  
	
	  if (curVal < 1)
	    {  
	      curVal = 1;  
	      printf("ERROR\n");  
	      exit(1);  
	    }
	  if (curVal > nrOfClasses)
	    {
	      curVal = nrOfClasses;  
	      printf("ERROR\n");  
	      exit(1);  
	    }
	  IAinc(&hitArray, curVal);  
	}
      for (count=1;  count<=nrOfClasses;  count++)
	{
	  curRes = DAgetDouble(&ANslopeArr, count);  
	  newRes = ((curRes*ANslope)+
		    ((double)IAgetInt(&hitArray, count)/
		     (double)nrOfPoints)) / (ANslope+1);  
	  DAsetDouble(&ANslopeArr, count, newRes);  
	}
      ANslope++;  

      IAfree(&hitArray);  
    }
}


int pLiesToII(t_point ref, t_point test)
{
  int result = 1;  

  if (test.x == ref.x)
    result = 0;  
  else if (test.x > ref.x)
    result = -1;  

  return(result);  
}


void sinuosityII(t_pointArray *pArray, t_polygon *aPolygon)
{
  int nrOfPoints, count;  
  t_point cur, prev, next;  
  int prevTurn, sinu, prevDouble;  
  int curTurn;  
  int side1, side2;  

  nrOfPoints = PAnrOfPoints(pArray);  
  prevTurn = 0;  
  sinu = 0;  
  prevDouble = 0;  

  if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0))
    {
      prev = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  
      cur = PAgetPoint(pArray, BPgetPIndex(aPolygon, 1));  
      next = PAgetPoint(pArray, BPgetPIndex(aPolygon, 2));  
      side1 = pLiesToII(cur, prev);  

      for (count=1;  count<=nrOfPoints;  count++)
	{
	  side2 = pLiesToII(cur, next);  
	  /* test whether we have a turn */
	  if ((side1 * side2) != -1)
	    {
 	      curTurn = isOnLine(prev, cur, next);  
	      /* compare with prevoius turn */
	      /* if the turns are identical, test if we are in the same 
		 "arc", if not, increase sinu & start a new one. */
	      if (curTurn != 0)
		{
		  if ((curTurn == prevTurn) && (curTurn != prevDouble))
		    {
		      sinu ++;  
		      prevDouble = curTurn;  
		    }
		  
		  /* save the current turn */
		  prevTurn = curTurn;  
		}
	    }
	  
	  /* proceed to the next point */
	  prev = cur;  
	  cur = next;  
	  next = PAgetPoint(pArray, 
			    BPgetPIndex(aPolygon, ((count+2)%nrOfPoints)));  
	  side1 = -side2;  
	}

      /* all polygons have at least sinuosity 1 */
      if (sinu == 0)
	sinu = 1;  

      IAinc(&ANsinuArr, sinu);  
    }
}
  






