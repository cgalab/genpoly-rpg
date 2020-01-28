/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "pointOps.h"	
#include "polyOps.h"	
#include "pointList.h"	
#include "pointArray.h"	
#include "basicDefs.h"	
#include "stateLine.h"	
#include "ipeWrite.h"

#include <stdio.h>

/********************************************************************/
/*                                                                  */
/* Constants and global variables                                   */
/*                                                                  */
/********************************************************************/

static t_pointList aList;  

static enum t_POactionState POactionState = NONE;  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void POinit()
{
  POactionState = NONE;  
  PLinitList(&aList);  
}

void POfree()
{
   PLfreeList(&aList);  
}

t_pointList* POgetPointList()
{
   return(&aList);  
}

void POloadPoints(char *fileName)
{
  t_pointArray auxArray;  

  if (fileName != NULL) {
     POclearPoints();  
     
     PAinitArray(&auxArray, 0);  
     
     if (!PAreadFile(&auxArray, fileName))   {
        SLsetState(ST_FLE_N_FND);  
     }
     else   {
        PAsortArray(&auxArray);  
        PAcreateList(&auxArray, &aList);  
     }
     
     PAfreeArray(&auxArray);  
  }
}


void POdelPoint(double x, double y, double epsilon)
{
  t_point aPoint;  

  YOclearPoly();  
  PLdelEpsilonPoints(&aList, aPoint, epsilon);  
}

void POaddPoint(double x, double y)
{
   t_point aPoint;  
   
   YOclearPoly();  
   PLaddPoint(&aList, aPoint);  
}

void POclearPoints()
{
   YOclearPoly();  
   PLfreeList(&aList);  
   PLinitList(&aList);  
}

void POdoAction(double x, double y, double epsilon)
{
  switch(POactionState)
  {
  case ADD: 
    POaddPoint(x, y);  
    break;     
  
  case DELETE:
    POdelPoint(x, y, epsilon);  
    break;  

  default:
    break;  
  }
}

void POclearAction()
{
  POactionState = NONE;  
  SLsetState(ST_OK);  
}

void POsetActionAdd()
{
  POactionState = ADD;  
  SLsetState(ST_ADD_P);  
}

void POsetActionDelete()
{
  POactionState = DELETE;  
  SLsetState(ST_DEL_P);  
}

void POipeWrite(FILE *ipeFile)
{
  t_pointArray pArray;  

  PAlistInitArray(&pArray, &aList);  
  IWwritePoints(ipeFile, &pArray);  

  PAfreeArray(&pArray);  
}


void POgenerateRandomPoints(int nrOfPoints)
{
  YOclearPoly();  
  PLaddRandomPoints(&aList, nrOfPoints);  
  //  MFredrawFree();  
}

void POgenerateRandomCirclePoints(int nrOfPoints)
{
  YOclearPoly();  
  PLaddRandomCirclePoints(&aList, nrOfPoints);  
  //  MFredrawFree();  
}


void POgenerateClusterPoints(int nrOfPoints)
{
  YOclearPoly();  
  PLaddClusterPoints(&aList, nrOfPoints);  
  //  MFredrawFree();  
}









