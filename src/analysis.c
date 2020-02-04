/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "analysis.h"
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

static int writeFile = FALSE;  

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
int aGetFile()
{
   return writeFile;  
}

void aToggleFile()
{
   writeFile = !writeFile;  
}

int resWrite(char *fName, t_intArray *resArray, int nrOfElems)
{
   FILE *outFile = NULL;  
   int count, res;  
   
   res = FALSE;  
   
   if (writeFile && ((outFile = FopenWrite(fName)) == NULL))
      return res;  
   
   for (count=1;  count<=IAsizeOf(resArray);  count++) {
      if (writeFile)
         fprintf(outFile, "%d %f\n", count, ((double)IAgetInt(resArray, count))/((double)nrOfElems));  
   }
   res = TRUE;  
   if (writeFile && outFile != NULL)
      Fclose(outFile);  
   
   return(res);  
}


void angleDist(t_pointArray *pArray, t_polygon *aPolygon, int nrOfClasses, 
               char *fName)
{
   int nrOfPoints, count, curVal;  
   t_point orig, dest;  
   t_intArray hitArray;  
   double step, angle, lastAngle, curAngle;  
   
   nrOfPoints = PAnrOfPoints(pArray);  
   
   if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0)) {
      step = 360.0 / (double)nrOfClasses;  
      
      IAinit(&hitArray, nrOfClasses);  
      for (count=1;  count<=nrOfClasses;  count++)
         IAsetInt(&hitArray, count, 0);  
      
      orig = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints-1));  
      dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  
      
      angle = atan2((dest.y-orig.y), (dest.x-orig.x))*180.0*M_1_PI;  
      
      for (count=1;  count<=nrOfPoints;  count++) {
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
         
         if (curVal < 1) {  
            curVal = 1;  
            printf("ERROR\n");  
            exit(1);  
         }
         if (curVal > nrOfClasses) {
            curVal = nrOfClasses;  
            printf("ERROR\n");  
            exit(1);  
         }
         IAinc(&hitArray, curVal);  
      }
      
      resWrite(fName, &hitArray, nrOfPoints);  
      
      IAfree(&hitArray);  
   }
}


void lengthDist(t_pointArray *pArray, t_polygon *aPolygon, int nrOfClasses, 
                char *fName)
{
   int nrOfPoints, count, count2, curVal, hullSize;  
   t_point point1, orig, dest;  
   t_intArray hitArray;  
   t_convexHull cHull;  
   t_polyLine pOnHull;  
   double step, maxLen, len;  
   
   nrOfPoints = PAnrOfPoints(pArray);  
   
   if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0)) {
      
      /* compute the maximum length */
      /* first, we need to compute the convex hull */
      CHcalcHull(&cHull, pArray);  
      
      /* now get the maximum distance */
      maxLen = 0.0;  
      pOnHull = CHgetPoints(&cHull);  
      hullSize = BLsizeOf(&pOnHull);  
      
      for (count=1;  count<hullSize;  count++) {
         point1 = PAgetPoint(pArray, BLgetPIndex(&pOnHull, count));  
         for (count2=count+1;  count2<=hullSize;  count2++) {
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
      for (count=1;  count<=nrOfPoints;  count++) {
         orig = dest;  
         dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, count));  
         
         len = lineLength(orig, dest);  
         if (deltaAbs(len-maxLen))
            curVal = nrOfClasses;  
         else
            curVal = (int) (len/step)+1;  
         
         if (curVal < 1) {  
            curVal = 1;  
            printf("ERROR\n");  
            exit(1);  
         }
         if (curVal > nrOfClasses) {
            curVal = nrOfClasses;  
            printf("ERROR\n");  
            exit(1);  
         }
         IAinc(&hitArray, curVal);  
      }
      
      resWrite(fName, &hitArray, nrOfPoints);  
      
      IAfree(&hitArray);  
      CHfreeHull(&cHull);  
   }
}


void slopeDist(t_pointArray *pArray, t_polygon *aPolygon, int nrOfClasses, 
               char *fName)
{
   int nrOfPoints, count, curVal;  
   t_point orig, dest;  
   t_intArray hitArray;  
   double step, angle;  
   
   nrOfPoints = PAnrOfPoints(pArray);  
   
   if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0)) {
      step = 360.0 / (double)nrOfClasses;  
      
      IAinit(&hitArray, nrOfClasses);  
      for (count=1;  count<=nrOfClasses;  count++)
         IAsetInt(&hitArray, count, 0);  
      
      dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  
      for (count=1;  count<=nrOfPoints;  count++) {
         orig = dest;  
         dest = PAgetPoint(pArray, BPgetPIndex(aPolygon, count));  
         
         angle = atan2((dest.y-orig.y), (dest.x-orig.x))*180.0*M_1_PI;  
         if (angle < 0.0)
            angle = angle + 360.0;  
         
         curVal = (int) (angle/step)+1;  
         
         if (curVal < 1) {  
            curVal = 1;  
            printf("ERROR\n");  
            exit(1);  
         }
         if (curVal > nrOfClasses) {
            curVal = nrOfClasses;  
            printf("ERROR\n");  
            exit(1);  
         }
         IAinc(&hitArray, curVal);  
      }
      
      resWrite(fName, &hitArray, nrOfPoints);  
      
      IAfree(&hitArray);  
   }
}


int pLiesTo(t_point ref, t_point test)
{
   int result = 1;  
   
   if (test.x == ref.x)
      result = 0;  
   else if (test.x > ref.x)
      result = -1;  
   
   return(result);  
}


void sinuosity(t_pointArray *pArray, t_polygon *aPolygon, char *fName)
{
   int nrOfPoints, count;  
   t_point cur, prev, next;  
   int prevTurn, sinu, prevDouble;  
   int curTurn;  
   FILE *outFile;  
   int side1, side2;  
   
   nrOfPoints = PAnrOfPoints(pArray);  
   prevTurn = 0;  
   sinu = 0;  
   prevDouble = 0;  
   
   if ((nrOfPoints >= 3) && (BPsizeOf(aPolygon) != 0)) {
      prev = PAgetPoint(pArray, BPgetPIndex(aPolygon, nrOfPoints));  
      cur = PAgetPoint(pArray, BPgetPIndex(aPolygon, 1));  
      next = PAgetPoint(pArray, BPgetPIndex(aPolygon, 2));  
      side1 = pLiesTo(cur, prev);  
      
      for (count=1;  count<=nrOfPoints;  count++) {
         side2 = pLiesTo(cur, next);  
         /* test whether we have a turn */
         if ((side1 * side2) != -1) {
            curTurn = isOnLine(prev, cur, next);  
            /* compare with prevoius turn */
            /* if the turns are identical, test if we are in the same 
               "arc", if not, increase sinu & start a new one. */
            if (curTurn != 0) {
               if ((curTurn == prevTurn) && (curTurn != prevDouble)) {
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

      if (writeFile && (outFile = FopenWrite(fName)) != NULL) {
         fprintf(outFile, "%d\n", sinu);  
         Fclose(outFile);  
      }
   }
}
  



