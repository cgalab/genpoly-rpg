/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointArray.h"
#include "basicDefs.h"
#include "basicGeom.h"
#include "basicIo.h"
#include "eralloc.h"
#include <string.h>
#include <stdlib.h>
#include <float.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
int PAvalidPoint(t_pointArray *anArray, t_point aPoint, int anIndex)
{
   int res = TRUE;  
   int count;  
   
   if (!validPoint(aPoint))
      res = FALSE;  
   else  {
      count = 0;  
      anIndex --;  /* We count from 0, not from 1, therefore we need to 
                      decrement anIndex! */
      
      while (res && (count < anArray->nrOfPoints))  {
         res = ((!deltaVal(((anArray->array+count)->x)-aPoint.x)) ||
                (!deltaVal(((anArray->array+count)->y)-aPoint.y)) ||
                (anIndex == count));  
         count++;  
      }
   }
   
   return(res);  
}

void PAinitEmptyArray(t_pointArray *anArray)
{
   PAinitArray(anArray, 0);  
}

void PAinitArray(t_pointArray *anArray, int nrOfPoints)
{
   anArray->nrOfPoints = nrOfPoints;  
   
   if (anArray->nrOfPoints > 0)  {
      anArray->array = (t_point *)
         ercalloc(anArray->nrOfPoints, sizeof(t_point));  
   }
}


int PAwriteToFile(t_pointArray *anArray, FILE *outputFile)
{
   unsigned int nrOfPoints = 0;  
   int count;  
   t_point curPoint;  
   
   if ((nrOfPoints = PAnrOfPoints(anArray)) > 0)  {
      
      fprintf(outputFile, "%f %f %f %f\n", X_MIN, X_MAX, Y_MIN, Y_MAX);  
      
      fprintf(outputFile, "%u\n", nrOfPoints);  
      
      for (count=1;  count<=nrOfPoints;  count++) {
         curPoint = PAgetPoint(anArray, count);  
         fprintf(outputFile, "%f %f\n", curPoint.x, curPoint.y);  
      }
      
   }
   return(TRUE);  
}


int PAreadFromFile(t_pointArray *anArray, FILE *inputFile)
{
   double xmin, xmax, ymin, ymax;
   double scale = 1.0, xscale = 1.0, yscale = 1.0, shift = 8192.0;  
   double x, y;  
   unsigned int nrOfPoints;  
   t_point *current;
   int count = 0;  

   if (EOF == fscanf(inputFile, "%u", &nrOfPoints))  return FALSE;
   
   PAfreeArray(anArray);  
   PAinitArray(anArray, nrOfPoints);   
   
   if (EOF == fscanf(inputFile, "%lf %lf", &x, &y))  return FALSE;
   current = anArray->array+count;
   current->x = current->x_ori = x;  
   current->y = current->y_ori = y;  
   xmin = xmax = x;
   ymin = ymax = y;

   for (count = 1;  count < nrOfPoints;  count++)  {
      if (EOF == fscanf(inputFile, "%lf %lf", &x, &y))  return FALSE;
      current = anArray->array+count;
      current->x = current->x_ori = x;  
      current->y = current->y_ori = y;  
      if      (x < xmin) xmin = x;
      else if (x > xmax) xmax = x;
      if      (y < ymin) ymin = y;
      else if (y > ymax) ymax = y;
   }

   xmin -= DELTA;
   ymin -= DELTA;
   xmax += DELTA;
   ymax += DELTA;

   if ((xmin < X_MIN)  ||  (xmax > X_MAX)  ||  
       (ymin < Y_MIN)  ||  (ymax > Y_MAX)) {
      xscale = (X_MAX-X_MIN)/(xmax-xmin);  
      yscale = (Y_MAX-Y_MIN)/(ymax-ymin);  
      if (xscale <= yscale) scale = xscale;
      else                  scale = yscale; 

      while (shift > scale)  shift /= 2.0;  
      scale = shift;

      for (count = 0;  count < nrOfPoints;  count++)  {
         current = anArray->array+count;
         current->x = (current->x - xmin) * scale + X_MIN;  
         current->y = (current->y - ymin) * scale + Y_MIN;  
      }
   }
   
   return TRUE;
}


int PAreadFile(t_pointArray *anArray, char *fileName)
{
   int success = FALSE;  
   FILE *inputFile;  
   
   if ((inputFile = FopenRead(fileName)) != NULL) {
      success = PAreadFromFile(anArray, inputFile);  
      Fclose(inputFile);  
   }
   
   return(success);  
}


void PAlistInitArray(t_pointArray *anArray, t_pointList *aList)
{
   t_point *curElem;  
   int count;  
   
   PAinitArray(anArray, PLcountPoints(aList));  
   
   PLresetList(aList);  
   curElem = anArray->array;  
   
   count =0;  
   while (PLgetCurrentPoint(aList, curElem))  {
      count++;  
      curElem++;  
   }
}


void PAcopyArray(t_pointArray *sourceArray, t_pointArray *destArray)
{
   /*  bcopy(sourceArray->array, destArray->array, 
       sourceArray->nrOfPoints*sizeof(t_point));  */
   memcpy(destArray->array, sourceArray->array, 
          sourceArray->nrOfPoints*sizeof(t_point));  
}

int PAcompPoints(const void *elem1, const void *elem2)
{
   /* NOTE: this function returns -1 if elem1 < elem2 */
   int res;  
   t_point *point1, *point2;  
   point1 = (t_point *) elem1;  
   point2 = (t_point *) elem2;  
   
   if (equalPoints(*point1, *point2))
      res = 0;  
   else if (compPoints(*point1, *point2))
      res = -1;  
   else
      res = 1;  
   
   return(res);  
}

void PAsortArray(t_pointArray *anArray)
{
   qsort(anArray->array, PAnrOfPoints(anArray), sizeof(t_point), PAcompPoints);  
}


int PAcreateList(t_pointArray *anArray, t_pointList *aList)
{
   int count;  
   
   PLcleanList(aList);  
   
   for (count=PAnrOfPoints(anArray);  count >= 1;  count--) {
      PLaddPoint(aList, PAgetPoint(anArray, count));  
   }
   
   PLresetList(aList);  
   return(TRUE);  
}



t_point PAgetPoint(t_pointArray *anArray, int index)
{
   t_point *getElem;  
   
   if ((index <= anArray->nrOfPoints) && (1 <= index))
      getElem = (anArray->array+index-1);  
   else
      getElem = &BGdefaultPoint;  
   
   return(*getElem);  
}

int PAisInArray(t_pointArray *anArray, t_point aPoint)
{
   return(PAgetPIndex(anArray, aPoint) != -1);  
}

int PAgetPIndex(t_pointArray *anArray, t_point aPoint)
{
   int leftMarker, rightMarker, curIndex;  
   int result;  
   t_point curPoint;  
   
   leftMarker = 1;  
   rightMarker = anArray->nrOfPoints;  
   
   do {
      curIndex = (leftMarker+rightMarker)/2;  
      curPoint=PAgetPoint(anArray, curIndex);  
      if (compPoints(aPoint, curPoint))
         rightMarker = curIndex-1;  
      else
         leftMarker = curIndex+1;  
   }
   while (!equalPoints(aPoint, curPoint) && (leftMarker<=rightMarker));  
   
   if (equalPoints(aPoint, curPoint))
      result = curIndex;  
   else
      result = -1;  
   
   return(result);  
}


void PAsetPoint(t_pointArray *anArray, int index, t_point newPoint)
{
   if ((index <= anArray->nrOfPoints) && (1 <= index))
      *(anArray->array+index-1) = newPoint;  
}


int PAnrOfPoints(t_pointArray *anArray)
{
   return(anArray->nrOfPoints);  
}

void PAfreeArray(t_pointArray *anArray)
{
   if (anArray->nrOfPoints > 0)
      erfree(anArray->array);  
   
   anArray->nrOfPoints = 0;  
}

int PAisOnLine(t_pointArray *anArray, int indexLineP1, 
               int indexLineP2, int indexPoint)
{
   int res;  
   t_point lineP1, lineP2, aPoint;  
   
   lineP1 = PAgetPoint(anArray, indexLineP1);  
   lineP2 = PAgetPoint(anArray, indexLineP2);  
   aPoint = PAgetPoint(anArray, indexPoint);  
   
   if (indexLineP1 < indexLineP2)
      res = isOnOrderedLine(lineP1, lineP2, aPoint);  
   else
      res = -isOnOrderedLine(lineP2, lineP1, aPoint);  
   
   return(res);  
}

int PAisectSegments(t_pointArray *anArray, int indexl1p1, 
                    int indexl1p2, int indexl2p1, int indexl2p2)
{
   int result;  
   int minL1, maxL1, minL2, maxL2;  
   t_point l1p1, l1p2, l2p1, l2p2;  
   
   /* map the indices in a way that l1p1 <= l1p2 and l2p1 <= l2p2 */
   minL1 = MIN(indexl1p1, indexl1p2);  
   maxL1 = MAX(indexl1p1, indexl1p2);  
   minL2 = MIN(indexl2p1, indexl2p2);  
   maxL2 = MAX(indexl2p1, indexl2p2);  

   //printf("PAisectSegments():\n");
   //printf("(%d-->%d) vs (%d-->%d)\n", minL1, maxL1, minL2, maxL2);

   if (maxL1 <= minL2) return FALSE;
   if (maxL2 <= minL1) return FALSE;
   
   l1p1 = PAgetPoint(anArray, minL1);  
   l1p2 = PAgetPoint(anArray, maxL1);  
   l2p1 = PAgetPoint(anArray, minL2);  
   l2p2 = PAgetPoint(anArray, maxL2);  
   
   if ((minL1 < minL2) || ((minL1 == minL2)  && (maxL1 < maxL2)))
      result = isectOrderedSegments(l1p1, l1p2, l2p1, l2p2);  
   else
      result = isectOrderedSegments(l2p1, l2p2, l1p1, l1p2);  
   
   return(result);  
}

void PAreallocArray(t_pointArray *anArray, int newNrOfPoints)
{
   anArray->array = 
      (t_point *)errealloc(anArray->array, sizeof(t_point)*newNrOfPoints);  
   anArray->nrOfPoints = newNrOfPoints;  
}


void PAgetMinMax(t_pointArray *anArray, t_point *minPoint, t_point *maxPoint)
{
   int count;  
   
   if (PAnrOfPoints(anArray) == 0) {
      /* we have an emtpy list, set the minimum and maximum to 
         X_MIN, Y_MIN and X_MAX, Y_MAX */
      minPoint->x = X_MIN;  
      minPoint->y = Y_MIN;  
      maxPoint->x = X_MAX;  
      maxPoint->y = Y_MAX;  
   }
   else {
      /* we have at leats one element, set minimum and 
         maximum x coordinate (the points are sorted according
         to their x-ccordinate! */
      minPoint->x = PAgetPoint(anArray, 1).x;  
      maxPoint->x = PAgetPoint(anArray, PAnrOfPoints(anArray)).x;  
      
      /* initiate y-coordinates with extreme values */
      minPoint->y = Y_MAX;  
      maxPoint->y = Y_MIN;  
      
      for (count=1;  count<=PAnrOfPoints(anArray);  count++) {
         if (PAgetPoint(anArray, count).y < minPoint->y)
            minPoint->y = PAgetPoint(anArray, count).y;  
         if (PAgetPoint(anArray, count).y > maxPoint->y)
            maxPoint->y = PAgetPoint(anArray, count).y;  
      }
   }
}
