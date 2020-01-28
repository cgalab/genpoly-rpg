/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "smoothPoly.h"
#include "M2optPolyII.h"
#include "basicDefs.h"

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

void smoothPoly(t_pointList *aList, t_pointArray *pArray, 
                t_polygon *aPolygon)
{
   int nrOfPoints, count;  
   t_pointArray auxArray;  
   t_polygon tempPoly;  
   t_point orig, dest, newPoint, vect;  
   
   nrOfPoints = PAnrOfPoints(pArray);  
   
   if ((nrOfPoints >= 3) && (nrOfPoints <= MAX_POINTS) && 
       (BPsizeOf(aPolygon) != 0))  {
      
      /* save the old array of points and create the new one */
      PAinitArray(&auxArray, nrOfPoints);  
      PAcopyArray(pArray, &auxArray);  
      PAfreeArray(pArray);  
      PAinitArray(pArray, 2*nrOfPoints);  

      dest = PAgetPoint(&auxArray, BPgetPIndex(aPolygon, nrOfPoints));  

      for (count = 1;   count <= nrOfPoints;   count++) {
         orig = dest;  
         dest = PAgetPoint(&auxArray, BPgetPIndex(aPolygon, count));  
         vect.x     = POINT_DISTANCE * (dest.x - orig.x);  
         vect.y     = POINT_DISTANCE * (dest.y - orig.y);  
         vect.x_ori = POINT_DISTANCE * (dest.x_ori - orig.x_ori);  
         vect.y_ori = POINT_DISTANCE * (dest.y_ori - orig.y_ori);  
         
         /* first point */
         newPoint.x     = orig.x + vect.x;  
         newPoint.y     = orig.y + vect.y;  
         newPoint.x_ori = orig.x_ori + vect.x_ori;  
         newPoint.y_ori = orig.y_ori + vect.y_ori;  
         PAsetPoint(pArray, (2*(count-1))+1, newPoint);  
         
         /* second point */
         newPoint.x     = dest.x - vect.x;  
         newPoint.y     = dest.y - vect.y;  
         newPoint.x_ori = dest.x_ori - vect.x_ori;  
         newPoint.y_ori = dest.y_ori - vect.y_ori;  
         PAsetPoint(pArray, 2*count, newPoint);  
      }

      /* now create the new polygon */
      BPfree(aPolygon);  
      BPinit(aPolygon, 2*nrOfPoints);  
      
      for (count = 1;   count <= (2*nrOfPoints);   count++)
         BPsetPIndex(aPolygon, count, count);     
      
      PAfreeArray(&auxArray);  
      
      /* resort the polygon */
      PAinitArray(&auxArray, 2*nrOfPoints);  
      PAcopyArray(pArray, &auxArray);  
      BPinit(&tempPoly, 2*nrOfPoints);  
      BPcopyPoly(aPolygon, &tempPoly);  
      
      PAsortArray(pArray);  
      
      PAcreateList(pArray, aList);  
      
      for (count = 1;   count <= (2*nrOfPoints);   count++) {
         BPsetPIndex(aPolygon, count, 
                     PAgetPIndex(pArray, 
                                 PAgetPoint(&auxArray, 
                                            BPgetPIndex(&tempPoly, count))));  
      }
      
      PAfreeArray(&auxArray);  
      BPfree(&tempPoly);  
      
      MnewDo2optMoves(aPolygon, pArray, 2*nrOfPoints, TRUE);  
   }
}



