/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicPoly.h"
#include "basicDefs.h"
#include "basicIo.h"
#include "linkPoly.h"
#include <string.h>

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

void BPinit(t_polygon *aPolygon, int nrOfPoints)
{
  if (nrOfPoints > 0)
    IAinit((t_intArray*)aPolygon, nrOfPoints);  
  else
    aPolygon->nrOfInts = 0;  
}

void BPrandomPerm(t_polygon *aPolygon)
{
  int count, swapIndex;  

  for (count=1;  count<=BPsizeOf(aPolygon);  count++)
    BPsetPIndex(aPolygon, count, count);  
    
  for (count=1;  count<BPsizeOf(aPolygon);  count++)
    {
      swapIndex = randomInt(count, BPsizeOf(aPolygon));  
      BPswap(aPolygon, count, swapIndex);  
    }
}

void BPswap(t_polygon *aPolygon, int index1, int index2)
{
  int auxPIndex;  

  if ((index1 != index2) && (MIN(index1, index2) > 0) &&
      (MAX(index1, index2) <= BPsizeOf(aPolygon)))
    {
      auxPIndex = BPgetPIndex(aPolygon, index1);  
      BPsetPIndex(aPolygon, index1, BPgetPIndex(aPolygon, index2));  
      BPsetPIndex(aPolygon, index2, auxPIndex);  
    }
}

int BPisCCW(t_polygon *aPolygon, t_pointArray *pArray)
{
  int result = TRUE;  
  
  /* check whether we have cw or ccw order */
  if (PAisOnLine(pArray, BPgetPIndex(aPolygon, BPsizeOf(aPolygon)), 1, 
		 BPgetPIndex(aPolygon, 2)) == -1)
    result = FALSE;  
  
  return(result);  
}

void BPnormalize(t_polygon *aPolygon, t_pointArray *pArray)
{
  int count;  
  t_linkPoly lPoly;  
  
  if (BPsizeOf(aPolygon) > 0)
      {
	/* first, let's check whether the first point has index 1 */
	if (BPgetPIndex(aPolygon, 1) != 1)
	  {
	    /* copy to a link polygon and back! */
	    LPfromPoly(&lPoly, aPolygon);  
	    BPfree(aPolygon);  
	    LPtoPoly(&lPoly, aPolygon);  
	    LPfree(&lPoly);  
	  }

	/* check whether we have cw or ccw order */
	if (!BPisCCW(aPolygon, pArray))
	  {
	    /* the polygon is in cw order, change that to ccw order! */
	    for (count=2;  count<=((BPsizeOf(aPolygon)+1)/2);  count++)
	      BPswap(aPolygon, count, (BPsizeOf(aPolygon)+2-count));  
	  }
      }
}
     
int BPisSimple(t_polygon *aPolygon, t_pointArray *pArray)
{
  int simplePoly, nrOfPoints, count, count2;  
  int e2PIndex2;  

  /* now we have a polygon, check whether it is simple! */
  simplePoly = TRUE;  
  
  nrOfPoints = BPsizeOf(aPolygon);  
  
  count = 1;  
  while (simplePoly && (count < nrOfPoints))
    {
      count2 = count+1;  
      while (simplePoly && (count2 <= nrOfPoints))
	{
	  e2PIndex2 = (count2%nrOfPoints)+1;  
	  simplePoly = 
	    (!PAisectSegments(pArray, BPgetPIndex(aPolygon, count), 
			      BPgetPIndex(aPolygon, count+1), 
			      BPgetPIndex(aPolygon, count2), 
			      BPgetPIndex(aPolygon, e2PIndex2)));  
	  count2++;  
	}
      count++;  
    }

  return(simplePoly);  
}

int BPgetPIndex(t_polygon *aPolygon, int index)
{
  return(IAgetInt((t_intArray*)aPolygon, index));  
}


void BPsetPIndex(t_polygon *aPolygon, int index, int newValue)
{
  IAsetInt((t_intArray*)aPolygon, index, newValue);  
}


int BPsizeOf(t_polygon *aPolygon)
{
  int res;  

  res = IAsizeOf((t_intArray*)aPolygon);  
  return(res);  
}

void BPcopyPoly(t_polygon *sourcePolygon, t_polygon *destPolygon)
{
  IAcopyArray((t_intArray*)sourcePolygon, (t_intArray*)destPolygon);  
}


void BPfree(t_polygon *aPolygon)
{
   if (aPolygon->nrOfInts >0)
      IAfree((t_intArray*)aPolygon);  
}


int BPwriteToFile(t_polygon *aPolygon, FILE *outputFile)
{
   unsigned int nrOfPoints = 0;  
   int count;  
   
   if ((nrOfPoints = BPsizeOf(aPolygon)) > 0)  {
      fprintf(outputFile, "POLYGON\n");  
      
      for (count=1;  count<=nrOfPoints;  count++)  {
         fprintf(outputFile, "%u\n", BPgetPIndex(aPolygon, count));  
      }
      
   }
   
   fflush(outputFile);  
   
   return(TRUE);  
}



int BPreadFromFile(t_polygon *aPolygon, int nrOfPoints, FILE *inputFile)
{
  int curPIndex, count;  
  char aLine[256];  
  
  do{
    FreadLine(inputFile, aLine, sizeof(aLine));  
  } while (!strcasecmp(aLine, "POLYGON"));  

  for (count=1;  count<=nrOfPoints;  count++)  {
      FreadLine(inputFile, aLine, sizeof(aLine));  
      sscanf(aLine, "%u", &curPIndex);  

      BPsetPIndex(aPolygon, count, curPIndex);  
    }

  return(TRUE);  
}


int BPwriteFile(t_polygon *aPolygon, t_pointArray *pArray, char *fileName)
{
   int success = FALSE;  
   unsigned int nrOfPolys = 1;  
   FILE *outputFile;  
   
   if ((outputFile = FopenWrite(fileName)) != NULL)  {
      Fheader(outputFile);  
      
      success = TRUE;  
      if (BPsizeOf(aPolygon) > 0) {
         PAwriteToFile(pArray, outputFile);  
         
         fprintf(outputFile, "# number of polygons\n");  
         fprintf(outputFile, "%u\n", nrOfPolys);  
         
         BPwriteToFile(aPolygon, outputFile);  
      }
      
      Fclose(outputFile);  
   }
   return(success);  
}


int BPreadFile(t_polygon *aPolygon, t_pointArray *pArray, char *fileName)
{
   int success = FALSE;  
   char aLine[256];  
   t_pointArray auxArray;  
   t_polygon auxPoly;  
   int count;  
   FILE *inputFile;  
   
   if ((inputFile = FopenRead(fileName)) != NULL)  {
      success = TRUE;  
      PAreadFromFile(pArray, inputFile);  
      
      /* read the number of polygons, but ignore it! */
      FreadLine(inputFile, aLine, sizeof(aLine));  
      
      BPfree(aPolygon);  
      BPinit(aPolygon, PAnrOfPoints(pArray));  
      BPreadFromFile(aPolygon, PAnrOfPoints(pArray), inputFile);  
      
      Fclose(inputFile);  
      
      /* resort the array and move vertex indices */
      PAinitArray(&auxArray, PAnrOfPoints(pArray));  
      PAcopyArray(pArray, &auxArray);  
      BPinit(&auxPoly, BPsizeOf(aPolygon));  
      BPcopyPoly(aPolygon, &auxPoly);  
      
      PAsortArray(pArray);  
      
      for (count=1;  count<=BPsizeOf(aPolygon);  count++) {
         BPsetPIndex(aPolygon, count, 
                     PAgetPIndex(pArray, 
                                 PAgetPoint(&auxArray, 
                                            BPgetPIndex(&auxPoly, count))));  
      }
      
      BPfree(&auxPoly);  
      PAfreeArray(&auxArray);  
      BPnormalize(aPolygon, pArray);  
   }
   return(success);  
}
