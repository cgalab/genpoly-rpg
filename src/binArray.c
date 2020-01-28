/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "binArray.h"
#include "eralloc.h"
#include "basicDefs.h"

/* NOTE that we assume that sizeof(char) == 1 !!!                   */

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

unsigned char mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

int Cindex(int elem)
{
  return((elem-1)/8);  
}

int Coffset(int elem)
{
  return((elem-1)%8);  
}


void BAinit(t_binArray *aBinArray, int nrOfInts)
{
  unsigned char *curVal;  
  int count;  

  if (nrOfInts > 0)
  {
    aBinArray->arrSize = (nrOfInts/8);  

    if ((nrOfInts % 8) != 0)
      aBinArray->arrSize++;  
        
    aBinArray->size = nrOfInts;  
    aBinArray->array = (unsigned char *)
      ercalloc(aBinArray->arrSize, sizeof(unsigned char));  

    curVal = aBinArray->array;  

    for (count=1;  count<=aBinArray->arrSize;  count++)
    {
      *curVal = 0;  
      curVal++;  
    }
  }
  else
    {
      aBinArray->arrSize = 0;  
      aBinArray->size = 0;  
    }
}

int BAgetBool(t_binArray *aBinArray, int index)
{
  int result = FALSE;  
  int ind, off;  

  if ((index <= aBinArray->size) && (1 <= index))
    {
      ind = Cindex(index);  
      off = Coffset(index);  
      result = ((*(aBinArray->array+ind) & mask[off]) != 0);  
    }

  return(result);  
}

void BAsetBool(t_binArray *aBinArray, int index, int newValue)
{
  int ind, off;  

  if ((index <= aBinArray->size) && (1 <= index))
  {
    ind = Cindex(index);  
    off = Coffset(index);  
    
    if (newValue)
      *(aBinArray->array+ind) = (*(aBinArray->array+ind) | mask[off]);  
    else
      *(aBinArray->array+ind) = (*(aBinArray->array+ind) & (~mask[off]));  
  }
}


int BAsizeOf(t_binArray *aBinArray)
{
  return(aBinArray->size);  
}

void BAfree(t_binArray *aBinArray)
{
  if (aBinArray->arrSize > 0)
    erfree(aBinArray->array);  

  aBinArray->arrSize = 0;  
  aBinArray->size = 0;  
}


