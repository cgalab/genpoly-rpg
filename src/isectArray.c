/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "isectArray.h"
#include "eralloc.h"
#include <stdio.h>

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

/* we use 8 bits/byte and 1byte/char, because this is the standard
   situation! */

int ISbits[8] = { 
  1<<0, 
  1<<1, 
  1<<2, 
  1<<3, 
  1<<4, 
  1<<5, 
  1<<6, 
  1<<7 };  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void ISgetIndex(int s1pIndex1, int s1pIndex2, int s2pIndex1, int s2pIndex2, 
		int nrOfPoints, int *index, int *offset)
{
  int s1pi1, s1pi2, s2pi1, s2pi2;  
  int maIndex, miIndex;  /* major and minor index */
  int nrOfLines;  
  int location;  

  if ((MIN(s1pIndex1, s1pIndex2) < MIN(s2pIndex1, s2pIndex2)) ||
      ((MIN(s1pIndex1, s1pIndex2) == MIN(s2pIndex1, s2pIndex2)) &&
       (MAX(s1pIndex1, s1pIndex2) < MAX(s2pIndex1, s2pIndex2))))
    {
      s1pi1 = MIN(s1pIndex1, s1pIndex2);  
      s1pi2 = MAX(s1pIndex1, s1pIndex2);  
      s2pi1 = MIN(s2pIndex1, s2pIndex2);  
      s2pi2 = MAX(s2pIndex1, s2pIndex2);  
    }
  else
    {
      s2pi1 = MIN(s1pIndex1, s1pIndex2);  
      s2pi2 = MAX(s1pIndex1, s1pIndex2);  
      s1pi1 = MIN(s2pIndex1, s2pIndex2);  
      s1pi2 = MAX(s2pIndex1, s2pIndex2);  
    }

  nrOfLines = ((nrOfPoints*nrOfPoints) - nrOfPoints)/2;  
  miIndex = (2*nrOfPoints - s1pi1)*(s1pi1 - 1)/2 + s1pi2 - s1pi1;  
  maIndex = (2*nrOfPoints - s2pi1)*(s2pi1 - 1)/2 + s2pi2 - s2pi1;  
  
  location = (2*nrOfLines - miIndex)*(miIndex - 1)/2 + maIndex - miIndex - 1;  

  *index = location / 8;  
  *offset = location % 8;  
}

void ISmark(t_isectArray *iArray, int s1pIndex1, int s1pIndex2, 
	    int s2pIndex1, int s2pIndex2)
{
  int index, offset;  
  /* mark those 2 edges as intersecting */
  ISgetIndex(s1pIndex1, s1pIndex2, s2pIndex1, s2pIndex2, iArray->nrOfPoints, 
	     &index, &offset);  
  
  *((iArray->array)+index) |= ISbits[offset];  
}



void ISinit(t_isectArray *iArray, t_pointArray *pArray, int nrOfLines)
{
  int nrOfIsects;  
  int found, count1, count2, count3, count4;  
  
  /* set the number of points */
  iArray->nrOfPoints = PAnrOfPoints(pArray);  

  /* calculate the memory size we need to store all the information */
  nrOfIsects = ((nrOfLines*nrOfLines)-nrOfLines)/2;  
  iArray->size = nrOfIsects / 8;  
  if ((nrOfIsects % 8) != 0)
    iArray->size++;  

  iArray->array = (char *) ercalloc(iArray->size, sizeof(char));  
  
  for (count1=0;  count1<(iArray->size);  count1++)
    *((iArray->array)+count1) = (char) 0x00;  

  /* loop over all lines, check for intersections */
  for (count1=1;  count1<iArray->nrOfPoints;  count1++)
    for (count2=count1+1;  count2<=iArray->nrOfPoints;  count2++)
      {
	found = FALSE;  
	for (count3=count1;  count3<iArray->nrOfPoints;  count3++)
	  for (count4=count3+1;  count4<=iArray->nrOfPoints;  count4++)
	    {
	      if (found && PAisectSegments(pArray, count1, count2, count3, 
					   count4))
		ISmark(iArray, count1, count2, count3, count4);  
	      else if (!found)
		found = ((count1 == count3) && (count2 == count4));  
	    }
      }
}



int IScheckIsect(t_isectArray *iArray, int s1pIndex1, int s1pIndex2, 
		  int s2pIndex1, int s2pIndex2)
{
  int index, offset;  
  int result;  

  /* mark those 2 edges as intersecting */
  ISgetIndex(s1pIndex1, s1pIndex2, s2pIndex1, s2pIndex2, iArray->nrOfPoints, 
	     &index, &offset);  
  
  /* is the bit set? */
  result = ((*((iArray->array)+index) & ISbits[offset]) != 0);  

  return(result);  
}

void ISfree(t_isectArray *iArray)
{
  if (iArray->size > 0)
    {
      erfree(iArray->array);  
      iArray->size = 0;  
      iArray->nrOfPoints = 0;  
    }
}


