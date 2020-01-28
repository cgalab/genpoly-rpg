/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointList.h"
#include "pointArray.h"
#include "basicIo.h"
#include "basicDefs.h"
#include "basicGeom.h"
#include <stdio.h>
#include <stdlib.h>
#include "eralloc.h"

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

static int checkDistance = 1;    /* switch for PLaddPoint */

void PLsetCheck(int c)
{
  checkDistance = c;  
}

int wList(t_pointList *aList, FILE *outputFile);  

int rList(t_pointList *aList, FILE *inputFile);  


void PLinitList(t_pointList *aList)
{
  /* We assume that the list aList has already memory allocated */

  aList->head = (struct t_pointListElem *)
                eralloc(sizeof(struct t_pointListElem));  
  aList->tail = (struct t_pointListElem *)
                eralloc(sizeof(struct t_pointListElem));  

  aList->head->next = aList->tail;  
  aList->head->prev = NULL;  
  aList->tail->next = NULL;  
  aList->tail->prev = aList->head;  

  /* For searching in the list: initialize head, tail with
  values beyond the extrema! */
  aList->head->datum.x = - X_MAX;  
  aList->head->datum.y = - Y_MAX;  
  aList->tail->datum.x = 2 * X_MAX;  
  aList->tail->datum.y = 2 * Y_MAX;  

  /* the Current Element is head => as soon as we add an element, 
  we have to change this! */
  aList->current=aList->head;  

}


int PLaddPoint(t_pointList *aList, t_point aPoint)
{
   struct t_pointListElem *auxElem, *insertElem, *newElem;  
   int insert = FALSE;  /* if insert = FALSE we found an element with
                           a distance < delta, or the point is not
                           within [0, 1]^2 */
   
   if (validPoint(aPoint)) {
      
      insert = TRUE;  
      
      if (checkDistance) {
         /* NO insertion before the head-element => start search with the next
            element */
         auxElem = aList->head->next;  
         
         while ((auxElem != aList->tail) && 
                compPoints(auxElem->datum, aPoint) && 
                insert) {
            if (deltaVal(auxElem->datum.x-aPoint.x) &&
                deltaVal(auxElem->datum.y-aPoint.y))
               insert = FALSE;  
            
            auxElem = auxElem->next;  
         }
         insertElem = auxElem; /* set a pointer to the element where we want */
                               /* to insert */
         
         /* check the following elements whether the distance is less
            than delta */
         while (insert && (auxElem != aList->tail) &&
                deltaVal(auxElem->datum.x-aPoint.x)) {
            if (deltaVal(auxElem->datum.y-aPoint.y))
               insert = FALSE;  
            
            auxElem = auxElem->next;  
         }
      }
      else
         insertElem = aList->tail;  
      
      /* We have to guarantee that we will not have two points with the same
         coordinates, but we may have a point with the same coordiantes as
         the tail element */
      if (insert) {
         newElem = (struct t_pointListElem *)
            eralloc(sizeof(struct t_pointListElem));  
         newElem->datum = aPoint;  
         newElem->prev = insertElem->prev;  
         newElem->prev->next =   newElem;  
         newElem->next = insertElem;  
         newElem->next->prev =   newElem;  
      }
      
      /* If this was the first insertion, we have to update current */
      if (aList->current ==  aList->head) PLresetList(aList);  
   }
   
   return(insert);  
}


int PLaddRandomPoints(t_pointList *pList, int nrOfPoints)
{
   t_pointArray pArray;  
   int count;  
   
   if (nrOfPoints > MAX_RAND) {
      /* init the array */
      PAinitArray(&pArray, nrOfPoints);  
      
      /* fill the array with random points */
      for (count=1;  count<=nrOfPoints;  count++)
         PAsetPoint(&pArray, count, randomPoint());  
      
      /* now sort the array */
      PAsortArray(&pArray);  
      
      /* add the points to the list */
      for (count=PAnrOfPoints(&pArray);  count >= 1;  count--) {
         if (PLaddPoint(pList, PAgetPoint(&pArray, count)))
            nrOfPoints--;  
      }
      
      /* free the array */
      PAfreeArray(&pArray);  
   }
   
   /* add the missing points by adding one after one */
   while (nrOfPoints > 0) {
      do
         {}
      while (!PLaddPoint(pList, randomPoint()));  
      
      nrOfPoints--;  
   }

   return(TRUE);  
}


int PLaddRandomCirclePoints(t_pointList *aList, int nrOfPoints)
{
   t_point rPoint;  
   t_pointArray pArray;  
   int count;  
   double x, y, length;  
   
   if (nrOfPoints > MAX_RAND) {
      /* init the array */
      PAinitArray(&pArray, nrOfPoints);  
      
      /* fill the array with random points */
      for (count=1;  count<=nrOfPoints;  count++) {
         do {
            rPoint = randomPoint();  
            x = rPoint.x - 0.5;  
            y = rPoint.y - 0.5;  
            length = x*x + y*y;  
         }
         while (length > 0.25);  
         
         PAsetPoint(&pArray, count, rPoint);  
      }
      
      /* now sort the array */
      PAsortArray(&pArray);  
      
      /* add the points to the list */
      for (count=PAnrOfPoints(&pArray);  count >= 1;  count--) {
         if (PLaddPoint(aList, PAgetPoint(&pArray, count)))
            nrOfPoints--;  
      }
      
      /* free the array */
      PAfreeArray(&pArray);  
   }
   
   /* add the missing points by adding one after one */
   while (nrOfPoints > 0) {
      do {
         rPoint = randomPoint();  
         x = rPoint.x - 0.5;  
         y = rPoint.y - 0.5;  
         length = x*x + y*y;  
      }
      while ((length > 0.25) || (!PLaddPoint(aList, rPoint)));  
      
      nrOfPoints--;  
   }
   
   return(TRUE);  
}

int PLdelPoint(t_pointList *aList, t_point aPoint)
{
   struct t_pointListElem *auxElem;  
   
   auxElem = aList->head;  
   
   while ((auxElem->next != aList->tail) && 
          compPoints(auxElem->datum, aPoint)) {
      auxElem = auxElem->next;  
   }
   
   if ((auxElem != aList->tail) && equalPoints(auxElem->datum, aPoint)) {
      auxElem->prev->next = auxElem->next;  
      auxElem->next->prev = auxElem->prev;  
      if (aList->current == auxElem)
         aList->current = auxElem->next;  
      erfree(auxElem);  
   }

   return(TRUE);  
}

int PLdelEpsilonPoints(t_pointList *aList, t_point aPoint, 
                       double epsilon)
{
   struct t_pointListElem *auxElem, *delElem;  
   double dx, dy;  
   
   auxElem = aList->head;  
   
   while ((auxElem->next != aList->tail) &&
          (auxElem->datum.x < (aPoint.x-epsilon))) {
      auxElem = auxElem->next;  
   }
   while ((auxElem != aList->tail) &&
          auxElem->datum.x <= (aPoint.x+epsilon)) {
      dx = (aPoint.x - auxElem->datum.x)*(aPoint.x - auxElem->datum.x);  
      dy = (aPoint.y - auxElem->datum.y)*(aPoint.y - auxElem->datum.y);  
      if ((dx + dy) <= (epsilon*epsilon)) {
         auxElem->prev->next = auxElem->next;  
         auxElem->next->prev = auxElem->prev;  
         if (aList->current == auxElem)
            aList->current = auxElem->next;  
         delElem = auxElem;  
         auxElem = auxElem->next;  
         erfree(delElem);  
      }
      else {
         auxElem = auxElem->next;  
      }
   }

   return(TRUE);  
}


int PLgetCurrentPoint(t_pointList *aList, t_point *aPoint)
{
   int returnValue = FALSE;  
   
   if ((aList->current != aList->head) &&
       (aList->current != aList->tail)) {
      *aPoint = aList->current->datum;  
      aList->current = aList->current->next;  
      returnValue = TRUE;  
   }

   return(returnValue);  
}


int PLresetList(t_pointList *aList)
{
  if (aList->head->next == aList->tail)
    aList->current = aList->head;  
  else
    aList->current = aList->head->next;  

  return(TRUE);  
}

int PLcleanList(t_pointList *aList)
{
  aList->current=aList->head->next;  

  while (aList->current != aList->tail) {
     aList->current = aList->current->next;  
     erfree(aList->current->prev);  
  }

  aList->head->next = aList->tail;  
  aList->tail->prev = aList->head;  

  PLresetList(aList);  

  return(TRUE);  
}

void PLgetMinMax(t_pointList *aList, t_point *minPoint, t_point *maxPoint)
{
   struct t_pointListElem *auxElem;  
   
   auxElem = aList->head->next;  
   
   if (aList->head->next == aList->tail) {
      /* we have an emtpy list, set the minimum and maximum to
         X_MIN, Y_MIN and X_MAX, Y_MAX */
      minPoint->x = X_MIN;  
      minPoint->y = Y_MIN;  
      maxPoint->x = X_MAX;  
      maxPoint->y = Y_MAX;  
   }
   else  {
      /* we have at leats one element, set minimum and
         maximum x coordinate (the points are sorted according
         to their x-ccordinate! */
      minPoint->x = aList->head->next->datum.x;  
      maxPoint->x = aList->tail->prev->datum.x;  
      
      /* initiate y-coordinates with extreme values */
      minPoint->y = Y_MAX;  
      maxPoint->y = Y_MIN;  
      
      auxElem = aList->head->next;  
      while (auxElem != aList->tail)  {
         if (auxElem->datum.y < minPoint->y)
            minPoint->y = auxElem->datum.y;  
         if (auxElem->datum.y > maxPoint->y)
            maxPoint->y = auxElem->datum.y;  
         
         auxElem = auxElem->next;  
      }
   }
}

int PLfreeList(t_pointList *aList)
{
  aList->current=aList->head;  

  while (aList->current != aList->tail)
  {
    aList->current = aList->current->next;  
    erfree(aList->current->prev);  
  }

  erfree(aList->current);  

  return(TRUE);  
}


int PLcountPoints(t_pointList *aList)
{
  int nrOfPoints = 0;  
  struct t_pointListElem *auxElem;  

  auxElem = aList->head->next;  

  while (auxElem != aList->tail)
  {
    nrOfPoints ++;  
    auxElem = auxElem->next;  
  }
  return(nrOfPoints);  
}



#define CLUSTER_SIZE 100
#define PERCENT      20  /* must be less than 100 ! */
#define EMPTY        0
#define FULL         1

int PLaddClusterPoints(t_pointList *aList, int nrOfPoints)
{
   t_point rPoint;  
   t_pointArray pArray;  
   int count, i, j, k, number;  
   double x, y, length;  
   short int grid[CLUSTER_SIZE][CLUSTER_SIZE];  
   double xCluster[CLUSTER_SIZE+1];  
   double yCluster[CLUSTER_SIZE+1];  
   int currSize;  
   struct {int i, j;  }
   cluster[CLUSTER_SIZE * CLUSTER_SIZE * PERCENT / 100 + 1];  
   
   /* we set up up a regular rectangular grid of size  currSize x currSize */
   currSize = sqrt((double)nrOfPoints);  
   if (currSize < 6) currSize = 6;  
   else if (currSize > CLUSTER_SIZE) currSize = CLUSTER_SIZE;  
   
   /* store the x- and y-coordinates of the grid-points */
   length = 1 / (double)currSize;  
   for (i = 0;   i < currSize;  ++i) {
      xCluster[i] = length * i;  
      yCluster[i] = length * i;  
   }
   xCluster[currSize] = 1.0;  
   yCluster[currSize] = 1.0;  
   
   /* mark all grid cells as empty */
   for (i = 0;   i < currSize;   ++i) {
      for (j = 0;   j < currSize;   ++j) {
         grid[i][j] = EMPTY;  
      }
   }
   
   /* mark PERCENT-many grid cells as to be filled */
   number = currSize * currSize * PERCENT / 100 + 1;  
   count  = 0;  
   while (count < number)  {
      i = lrand48() % currSize;  
      j = lrand48() % currSize;  
      grid[i][j] = FULL;  
      ++count;  
   }
   /* make sure that at least one cell inside circle is marked */
   grid[currSize / 2][currSize / 2] = FULL;  
   
   /* copy the marked grid cells into a cluster array */
   count = 0;  
   for (i = 0;   i < currSize;   ++i) {
      for (j = 0;   j < currSize;   ++j) {
         if (grid[i][j] == FULL) {
            /* test if grid center is inside circle with radius 0.5 and
               center (0.5, 0.5) */
            x = (xCluster[i] + xCluster[i+1]) / 2.0 - 0.5;  
            y = (yCluster[j] + yCluster[j+1]) / 2.0 - 0.5;  
            if ((x * x + y * y) <= 0.25) {
               cluster[count].i = i;  
               cluster[count].j = j;  
               ++count;  
            }
         }
      }
   }
   currSize = count;  
   
   /* generate random points */
   if (nrOfPoints > MAX_RAND) {
      /* init the array */
      PAinitArray(&pArray, nrOfPoints);  
      
      /* fill the array with random points within a disk */
      count = 0;  
      while (count <= nrOfPoints) {
         k = lrand48() % currSize;  
         rPoint = randomPoint();  
         rPoint.x = rPoint.x_ori = xCluster[cluster[k].i] + rPoint.x * length;
         rPoint.y = rPoint.y_ori = yCluster[cluster[k].j] + rPoint.y * length;
         PAsetPoint(&pArray, count, rPoint);  
         ++count;  
      }
      
      /* now sort the array */
      PAsortArray(&pArray);  
      
      /* add the points to the list */
      for (count=PAnrOfPoints(&pArray);  count >= 1;  count--)  {
         if (PLaddPoint(aList, PAgetPoint(&pArray, count)))
            nrOfPoints--;  
      }

      /* free the array */
      PAfreeArray(&pArray);  
   }

   /* add the missing points by adding one after one */
   while (nrOfPoints > 0) {
      do {
         k = lrand48() % currSize;  
         rPoint = randomPoint();  
         rPoint.x = rPoint.x_ori = xCluster[cluster[k].i] + rPoint.x * length;
         rPoint.y = rPoint.y_ori = yCluster[cluster[k].j] + rPoint.y * length;
      }
      while (!PLaddPoint(aList, rPoint));  

      nrOfPoints--;  
   }

   return(TRUE);  
}

