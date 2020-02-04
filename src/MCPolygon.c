/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MCPolygon.h"
#include "pointList.h"
#include "pointOps.h"
#include "pointArray.h"
#include "pointList.h"
#include "triangArray.h"
#include "convexHull.h"
#include "basicLine.h"
#include "basicIntArray.h"
#include "basicIo.h"
#include "ipeWrite.h"
#include "eralloc.h"

//smoothing
#include "M2optPolyII.h"
#include "smoothPoly.h"

typedef struct {
   int first, last;  
   t_intArray points;  
   int choosen;  
} t_ear;  

typedef struct {
   t_ear *ears;  
   int pos, length;  
} t_earArray;  

t_polygon *polygons = NULL;  
static t_convexHull convexHull;  
static int pos = 0;  
int pcount = 0;  

char *charIsects;  
int checkcount;  

/***************************************************************************/
/* ear stuff */

void initEars(t_earArray *a, int length)
{
   a->ears = (t_ear*)eralloc(sizeof(t_ear) * length);  
   a->pos = 0;  
   a->length = length;  
}

void addEar(t_earArray *a, t_ear ear)
{
   a->ears[a->pos++] = ear;  
}

/****************************************************************************/
/* basic checks */

//TODO: division by zero check
int intersect(t_line *la, t_line *lb)
{
   double k, h;  
   t_point a, b, c, d;  
   a = la->point1;  
   b = la->point2;  
   c = lb->point1;  
   d = lb->point2;  
   
   k = ((d.x - c.x)*(a.y - c.y) - (d.y - c.y)*(a.x - c.x))/
      ((d.y - c.y)*(b.x - a.x) - (d.x - c.x)*(b.y - a.y));  
   
   h = ((b.x - a.x)*(a.y - c.y) - (b.y - a.y)*(a.x - c.x))/
      ((d.y - c.y)*(b.x - a.x) - (d.x - c.x)*(b.y - a.y));  
   
   return (k >= 0) && (k <= 1) && (h >= 0) && (h <= 1);  
}

/* whether the point p is left of the line directed from a to b or not*/
int isLeftOf(t_point a, t_point b, t_point p)
{
   return (((p.y - a.y)*(b.x - a.x) - (b.y - a.y)*(p.x - a.x)) > 0);  
}

int binom(int n, int k)
{
   double z, ne;  
   int i;  
   
   ne = 1;  z = 1;  
   for (i = n;  i > k;  i--)
      z = z * i;  
   
   for (i = 2;  i <= n - k;  i++)
      ne = ne * i;  
   
   return (int) (z / ne);  
}

/****************************************************************************/
/* the real stuff */

int intersectEar(int i, int k, t_ear *ear, t_polygon *aPolygon, t_pointArray *pArray) // i < k
{
   t_point curPoint, bPoint, aPoint, pPoint;  
   t_line diag, nextEdge;  
   int index, inPoly, l;  
   int isect, tmp, curEarLength;  
   
   if (k - i <= 1) return 1;  
   
   curEarLength = IAsizeOf(&ear->points);  
   
   if ((tmp = charIsects[(i-1)*curEarLength + (k-1)])) //did we already check this diag?
      return tmp - 1;  
   
   checkcount++;  
   
   BNcreatePLine(&diag, 
                 BPgetPIndex(aPolygon, IAgetInt(&ear->points, i)), 
                 BPgetPIndex(aPolygon, IAgetInt(&ear->points, k)), 
                 pArray);  
   
   
   /* check if it's in the polygon */
   index = IAgetInt(&ear->points, i);  
   if (index == 1) {
      bPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, BPsizeOf(aPolygon)));  
      curPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, 1));  
      aPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, 2));  
   }
   else if (index == BPsizeOf(aPolygon)) {
      bPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, index - 1));  
      curPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, index));  
      aPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, 1));  
   }
   else {
      bPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, index - 1));  
      curPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, index));  
      aPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, index + 1));  
   }
   
   pPoint = PAgetPoint(pArray, BPgetPIndex(aPolygon, IAgetInt(&ear->points, k)));  
   
   inPoly = FALSE;  
   if (isLeftOf(bPoint, curPoint, aPoint)) {
      if (isLeftOf(bPoint, curPoint, pPoint) && isLeftOf(curPoint, aPoint, pPoint))
         inPoly = TRUE;  
   }
   else {
      if (isLeftOf(bPoint, curPoint, pPoint) || isLeftOf(curPoint, aPoint, pPoint))
         inPoly = TRUE;  
   }
   
   if (!inPoly) {
      /* check if the current diag intersects with any of the other edges */
      isect = FALSE;  
      for (l=1;  l<=curEarLength-1;  l++) {
         /* don't check edges connected to the diag */
         if ((l != i) &&
             (l + 1 != i) &&
             (l != k) &&
             (l + 1 != k)) {
            BNcreatePLine(&nextEdge, 
                          BPgetPIndex(aPolygon, IAgetInt(&ear->points, l)), 
                          BPgetPIndex(aPolygon, IAgetInt(&ear->points, l+1)), 
                          pArray);  
            
            if (intersect(&diag, &nextEdge)) {
               isect = TRUE;  
               break;  
            }
         }
      }
      
      if (!isect) {
         /* we've found one */
         charIsects[(i-1)*curEarLength + (k-1)] = 1;  
         return FALSE;  
         
      }
   }
   
   charIsects[(i-1)*curEarLength + (k-1)] = 2;  
   return TRUE;  
}


/** Modified algorithm with random trying of diagonal */
void MCPolygon2(t_polygon *aPolygon, t_pointArray *pArray, int hcount)
{
   int i, j, k, l;  
   int curEarLength;  
   int first, last, lastk = 0, current, size, tmp2;  
   int a, b;  
   int outerCount;  
   int selectedHoles, visitedHoles;  
   char *charPoly;  
   int checkmax;  
   t_polyLine hullPoints;  
   t_earArray ears;  
   t_ear currentEar;  
   t_intArray currentPoints;
   
   //printf("\n%d holes to be generated\n", hcount);  
   
   MCclear();  
   
   /* Calculate the Convex Hull for finding the "ears"  */
   CHcalcHull(&convexHull, pArray);  
   hullPoints = CHgetPoints(&convexHull);  
   
   /*************/
   /* find ears */
   /*************/
   initEars(&ears, BLsizeOf(&hullPoints));  
   /* first: find convexHull points on aPolygon */
   first = 0;  
   last = 0;  
   
   for (i = 1;   i <= BLsizeOf(&hullPoints) + 1;   i++) {
      if (i == BLsizeOf(&hullPoints) + 1)
         current = BLgetPIndex(&hullPoints, 1);  
      else
         current = BLgetPIndex(&hullPoints, i);  
      
      for (j = last + 1;   j <= BPsizeOf(aPolygon) + first;   j++) {
         if (j > BPsizeOf(aPolygon))
            k = j - BPsizeOf(aPolygon);  
         else
            k = j;  
         
         if (BPgetPIndex(aPolygon, k) == current) {
            if (i != 1) {
               size = j - last + 1;  
               if (size > 4) {
                  currentEar.first = lastk;  
                  currentEar.last = k;  
                  currentEar.choosen = FALSE;  
                  
                  IAinit(&currentPoints, size);  
                  
                  /* check if we wrapped around */
                  if (k < lastk) {
                     for (l=1;  l<=BPsizeOf(aPolygon) - lastk + 1;  l++)
                        IAsetInt(&currentPoints, l, lastk + l - 1);  
                     for (l=1;  l<=k;  l++)
                        IAsetInt(&currentPoints, 
                                 l + BPsizeOf(aPolygon) - lastk + 1, l);  
                  }
                  else
                     for (l=1;  l<=size;  l++)
                        IAsetInt(&currentPoints, l, lastk + l - 1);  
                  
                  currentEar.points = currentPoints;  
                  addEar(&ears, currentEar);  
               }
            }
            else
               first = j;  
            
            last = j;  
            lastk = k;  
            break;  
         }
      }
   }
   
   /* init the characteristic polygon array for constructing the outer 
      polygon */
   charPoly = (char*) eralloc(BPsizeOf(aPolygon));  
   for (i=0;  i<BPsizeOf(aPolygon);  i++)
      charPoly[i] = 1;  
   
   pcount = 1;  
   outerCount = BPsizeOf(aPolygon);  
   
   /*************************************************************/
   /* randomly select ears, randomly try diags, create polygons */
   /*************************************************************/
   polygons = (t_polygon*)ercalloc(ears.pos + 1, sizeof(t_polygon));  
   selectedHoles = 0;  
   visitedHoles = 0;  
   while ((selectedHoles < hcount) && (visitedHoles < ears.pos)) {
      j = randomInt(0, ears.pos - 1);  
      while (ears.ears[j].choosen)
         if (++j > ears.pos - 1)
            j = 0;  
      
      ears.ears[j].choosen = TRUE;  
      visitedHoles++;  
      
      curEarLength = IAsizeOf(&ears.ears[j].points);  
      /* printf("ear: %d length: %d\n", j, curEarLength);  */
      
      checkcount = 0;  
      
      charIsects = (char*) eralloc(curEarLength*curEarLength);  
      memset(charIsects, 0, curEarLength*curEarLength);  
      
      /*
        main part
      */
      
      //printf("start: %d\n", curEarLength);  
      checkmax = binom(curEarLength, 2) - (curEarLength - 1);  
      //printf("end\n");  
      
      while (1) {
         i = randomInt(2, curEarLength - 1);  
         k = randomInt(2, curEarLength - 1);  
         
         a = MIN(i, k);  
         b = MAX(i, k);  
         
         tmp2 = intersectEar(a, b, &ears.ears[j], aPolygon, pArray);  
         
         if (!intersectEar(a - 1, b + 1, &ears.ears[j], aPolygon, pArray) && 
             !tmp2) {
            // to ensure that all diags get checked (for checkcount)
            break;  
         }
         
         if (checkcount >= checkmax) {
            a = -1;  
            break;  
         }
      }
      
      erfree(charIsects);  
      
      
      /* Create the polygons for the holes */
      if (a == -1)
         continue;  
      
      selectedHoles++;  
      
      /* printf("selected %d, %d, %d, %d cut\n", a, b, a -1, b + 1);  */
      
      /* create the hole */
      BPinit(&polygons[pcount], b - a + 1);  
      for (i = a;  i <= b;  i++) {
         current = IAgetInt(&ears.ears[j].points, i);  
         charPoly[current - 1] = 0;  
         outerCount--;  
         BPsetPIndex(&polygons[pcount], i - a + 1, 
                     BPgetPIndex(aPolygon, current));  
      }
      pcount++;  
      IAfree(&ears.ears[j].points);  
   }
   
   /* create the outer boundary */
   current = 1;  
   BPinit(&polygons[0], outerCount);  
   for (i=1;  i<=BPsizeOf(aPolygon);  i++)  {
      if (charPoly[i-1]) {
         BPsetPIndex(&polygons[0], current++, BPgetPIndex(aPolygon, i));  
      }
   }
   
   erfree(charPoly);  
   
   /*
     if ((pcount - 1) < hcount) {
     sprintf(error, "\n  created only %d instead of %d holes  \n", 
     pcount - 1, hcount);  
     }
     printf("holes generated: %d\n", pcount - 1);  
   */
}


void MCclear()
{
   int i;  
   
   for (i=0;   i<pcount;   i++)
      BPfree(&polygons[pcount]);  
   
   if (pcount)
      erfree(polygons);  
   
   pcount = 0;  
   pos = 0;  
}

void MCreset()
{
   pos = 0;  
}

t_polygon* MCgetNextPolygon()
{
   if (pos < pcount)
      return &polygons[pos++];  
   
   return NULL;  
}

int MCsavePoly(char* filename, t_pointArray *pArray)
{
   int success = FALSE;  
   unsigned int nrOfPolys = pcount;  
   int i;  
   FILE *outputFile;  
   
   if ((outputFile = FopenWrite(filename)) != NULL) {
      Fheader(outputFile);  
      
      success = TRUE;  
      if (polygons) {
         PAwriteToFile(pArray, outputFile);  
         
         fprintf(outputFile, "# number of polygons\n");  
         fprintf(outputFile, "%u\n", nrOfPolys);  
         
         for (i=0;  i<pcount;  i++)
            BPwriteToFile(&polygons[i], outputFile);  
      }
      
      Fclose(outputFile);  
   }
   return(success);  
}

void MCwriteVoronoi(char *fileName, t_pointArray *pArray)
{
   int count, i;  
   t_point min, max, curPoint;  
   FILE *vFile;  
   t_polygon *aPolygon;  
   
   if (polygons) {
      if ((vFile = FopenWrite(fileName)) != NULL) {
         /* get bounding box */
         PAgetMinMax(pArray, &min, &max);  
         
         /* begin the file */
         fprintf(vFile, "%f %f %f %f\n", min.x, min.y, max.x, 
                 max.y);  
         fprintf(vFile, "%d\n", pcount);  
         for (i=0;  i<pcount;  i++) {
            aPolygon = &polygons[i];  
            fprintf(vFile, "%u\n", BPsizeOf(aPolygon));  
            fprintf(vFile, "1\n");  
            
            /* write all the points */
            for (count=1;  count <= BPsizeOf(aPolygon);  count++) {
               curPoint =  PAgetPoint(pArray, BPgetPIndex(aPolygon, count));  
               fprintf(vFile, "0\n");  
               fprintf(vFile, "%20.15f %20.15f 0.0 0.0\n", curPoint.x, curPoint.y);  
            }
         }
         
         /* close the line */
         Fclose(vFile);  
      }
   }
}

void MCipeWrite(FILE *ipeFile, t_pointArray *pArray)
{
   int i;  
   
   if (polygons)
      for (i=0;   i<pcount;   i++) {
         IWwritePoly(ipeFile, &polygons[i], pArray);  
         fprintf(ipeFile, "\n");  
      }
}

void MCsmoothPoly(t_pointList *aList, t_pointArray *pArray)
{
   int nrOfPoints, allPointsCount, count, i, offset;  
   t_pointArray auxArray;  
   t_polygon *aPolygon;  
   t_point orig, dest, newPoint, vect;  
   

   if (polygons) {
      //pcount = 1;  
      //POclearPoints();  
      
      allPointsCount = PAnrOfPoints(pArray);  
      PAinitArray(&auxArray, allPointsCount);  
      PAcopyArray(pArray, &auxArray);  
      PAfreeArray(pArray);  
      PAinitArray(pArray, 2*allPointsCount);  
      offset = 0;  
      
      for (i=0;  i<pcount;  i++) {
         aPolygon = &polygons[i];  
         
         nrOfPoints = BPsizeOf(aPolygon);  
         
         if (nrOfPoints <= MAX_POINTS) {
            
            dest = PAgetPoint(&auxArray, BPgetPIndex(aPolygon, nrOfPoints));  
            
            for (count=1;  count<=nrOfPoints;  count++) {
               orig = dest;  
               dest = PAgetPoint(&auxArray, BPgetPIndex(aPolygon, count));  
               vect.x = POINT_DISTANCE * (dest.x - orig.x);  
               vect.y = POINT_DISTANCE * (dest.y - orig.y);  
               
               /* first point */
               newPoint.x = orig.x + vect.x;  
               newPoint.y = orig.y + vect.y;  
               PAsetPoint(pArray, (2*(count-1))+1 + offset, newPoint);  
               //POaddPoint(newPoint.x, newPoint.y);  
               
               /* second point */
               newPoint.x = dest.x - vect.x;  
               newPoint.y = dest.y - vect.y;  
               PAsetPoint(pArray, 2*count + offset, newPoint);  
               //POaddPoint(newPoint.x, newPoint.y);  
            }
            
            /* now create the new polygon */
            BPfree(aPolygon);  
            BPinit(aPolygon, 2*nrOfPoints);  
            
            for (count=1;  count<=(2*nrOfPoints);  count++)
               BPsetPIndex(aPolygon, count, count + offset);  
            
            
            //MnewDo2optMoves(aPolygon, pArray, 2*nrOfPoints, TRUE);  
         }
         offset = offset + nrOfPoints*2;  
      }
      
      PLsetCheck(FALSE);  
      PAcreateList(pArray, aList);  
      PLsetCheck(TRUE);  
      PAfreeArray(&auxArray);  
    //    MFredrawFree();  
   }
}
