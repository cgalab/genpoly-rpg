/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "MCPolygon.h"

#include "polyOps.h"
#include "smoothPoly.h"
#include "MbouncePoly.h"
#include "MxmonoPoly.h"
#include "MstarPoly.h"
#include "MgrowingPoly.h"
#include "M2optPoly.h"
#include "M2optPolyII.h"
#include "MspPoly.h"
#include "MrejectPoly.h"
#include "MsearchPoly.h"
#include "MtriangPoly.h"
#include "basicPoly.h"
#include "pointArray.h"
#include "ipeWrite.h"
#include "stateLine.h"
#include "basicIo.h"
#include "analysis.h"
#include "analysisII.h"
#include "calcPoly.h"

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/


static t_polygon aPolygon;  
static t_pointArray pArray;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/
void YOinit()
{
   BPinit(&aPolygon, 0);  
   PAinitEmptyArray(&pArray);  
}


void YOfree()
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
}


void YOclearPoly()
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   BPinit(&aPolygon, 0);  
   PAinitEmptyArray(&pArray);  
   
   /**/
   MCclear();  
}

void YOipeWrite(FILE *ipeFile)
{
   int i;  
   
   if (pcount == 0) {
      IWwritePoly(ipeFile, &aPolygon, &pArray);  
   }
   else {
      for (i=0;   i<pcount;   i++)
         IWwritePoly(ipeFile, &polygons[i], &pArray);  
   }
}

void YOsmoothPoly(t_pointList *pList)
{
   smoothPoly(pList, &pArray, &aPolygon);  
}


void YObouncePoly(int nrOfMoves, t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MbouncePoly(pList, &aPolygon, &pArray, nrOfMoves);  
}

void YOxmonoPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MxmonoPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOstarPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MstarPoly(&aPolygon, &pArray, 1, NULL);  
   
   //  MFredrawFree();  
}

void YOnewStarPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MnewStarPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOfastStarPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MfastStarPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOgrowingPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MgrowingPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOgrowingPolyII(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MgrowingPolyII(&aPolygon, &pArray, 1, NULL);  
}

void YO2optPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   M2optPoly(&aPolygon, &pArray, 1, NULL);  
}

void YO2optPolyII(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   M2optPolyII(&aPolygon, &pArray, 1, NULL);  
}

void YO2optPolyIII(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   M2optPolyIII(&aPolygon, &pArray, 1, NULL);  
}

void YOspPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MspPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOrejectPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MrejectPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOsearchPoly(t_pointList *pList)
{
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MsearchPoly(&aPolygon, &pArray, 1, NULL);  
}

void YOtriangPoly(int nrOfVertices, t_pointList *pList)
{
   char msg[255];  
   
   BPfree(&aPolygon);  
   PAfreeArray(&pArray);  
   
   PAlistInitArray(&pArray, pList);  
   
   MtriangPoly(pList, &aPolygon, &pArray, nrOfVertices, 1, NULL);  
   
   /*
     if (PAnrOfPoints(&pArray) != nrOfVertices)  {
     sprintf(msg, "Polygon contains only %i instead\n of %i vertices (as specified).", PAnrOfPoints(&pArray), nrOfVertices);  
     
     fprintf(stderr, "%s", msg);  
     }
   */
}

void YOloadPoly(char *fileName, t_pointList *pList)
{
   if (fileName != NULL)  {
      BPfree(&aPolygon);  
      PAfreeArray(&pArray);  
      PLfreeList(pList);  
      PLinitList(pList);  
      
      PAinitArray(&pArray, 0);  
      
      if (!BPreadFile(&aPolygon, &pArray, fileName))  {
         SLsetState(ST_FLE_N_FND);  
      }
      else {
         PAsortArray(&pArray);  
         PAcreateList(&pArray, pList);  
      }
   }
}

void YOwritePoly(char *fileName)
{
   if (fileName != NULL)  {
      if (!BPwriteFile(&aPolygon, &pArray, fileName))
         SLsetState(ST_FLE_N_FND);  
   }
}



void YOwriteLine(char *fileName)
{
   int count, i;  
   t_point min, max, curPoint;  
   FILE *vFile;  
   t_polygon *myPolygon;  
   
   if (BPsizeOf(&aPolygon) > 0) {
      if ((vFile = FopenWrite(fileName)) != NULL) {
         if (pcount == 0) {
            fprintf(vFile, "%u\n", BPsizeOf(&aPolygon) + 1);  
            
            /* write all the points */
            for (count=1;   count <= BPsizeOf(&aPolygon);   count++)  {
               curPoint = PAgetPoint(&pArray, BPgetPIndex(&aPolygon, count));  
               fprintf(vFile, "%20.15f %20.15f\n", curPoint.x_ori, 
                       curPoint.y_ori);  
            }
            curPoint =  PAgetPoint(&pArray, BPgetPIndex(&aPolygon, 1));  
            fprintf(vFile, "%20.15f %20.15f\n", curPoint.x_ori, 
                    curPoint.y_ori);  
         }
         else {
            for (i = 0;   i < pcount;   i++) {
               myPolygon = &polygons[i];  
               fprintf(vFile, "%u\n", BPsizeOf(myPolygon) + 1);  
               /* write all the points */
               for (count=1;  count <= BPsizeOf(myPolygon);  count++) {
                  curPoint = PAgetPoint(&pArray, 
                                        BPgetPIndex(myPolygon, count));  
                  fprintf(vFile, "%20.15f %20.15f\n", 
                             curPoint.x_ori, curPoint.y_ori);  
               }
               curPoint =  PAgetPoint(&pArray, BPgetPIndex(myPolygon, 1));  
               fprintf(vFile, "%20.15f %20.15f\n", curPoint.x_ori, 
                       curPoint.y_ori);  
            }
         }
         Fclose(vFile);  
      }
   }
}

  
void YOlengthDist(char *fileName, int nrOfClasses)
{
   lengthDist(&pArray, &aPolygon, nrOfClasses, fileName);  
}

void YOslopeDist(char *fileName, int nrOfClasses)
{
   slopeDist(&pArray, &aPolygon, nrOfClasses, fileName);  
}

void YOangleDist(char *fileName, int nrOfClasses)
{
   angleDist(&pArray, &aPolygon, nrOfClasses, fileName);  
}

void YOsinuosity(char *fileName)
{
   sinuosity(&pArray, &aPolygon, fileName);  
}

/*
  void YOlinkDiam(char *fileName)
  {
  linkDiam(&pArray, &aPolygon, fileName);  
  }
*/

void YOsinuosityII()
{
   sinuosityII(&pArray, &aPolygon);  
}


void YOanalysisII()
{
   lengthDistII(&pArray, &aPolygon);  
   slopeDistII(&pArray, &aPolygon);  
   angleDistII(&pArray, &aPolygon);  
   sinuosityII(&pArray, &aPolygon);  
   /* linkDiamII(&pArray, &aPolygon);  */
}


void YOgetPoly(t_polygon **polygon, t_pointArray **points)
{
   *polygon = &aPolygon;  
   *points = &pArray;  
   /*
     PAinitArray(points, PAnrOfPoints(&pArray));  
     PAcopyArray(&pArray, points);  
     
     BPinit(polygon,  BPsizeOf(&aPolygon));  
     BPcopyPoly(&aPolygon, polygon);  
   */
}


void YOholesPoly(int nrOfHoles, t_pointList *pList)
{
   MCPolygon2(&aPolygon, &pArray, nrOfHoles);  
   
   //MspPoly(&aPolygon, &pArray, 1, NULL);   ??
}

