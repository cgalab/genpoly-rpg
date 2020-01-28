/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "lineInfo.h"

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

int LIcompLines(t_lInfoP line1, t_lInfoP line2, double xcoord)
{
  int res;  
  t_point p1, p2;  

  p1.x = xcoord;  
  p2.x = xcoord;  
  BNsetYcoord(&(line1->line), &p1);  
  BNsetYcoord(&(line2->line), &p2);  

  if (deltaAbs(p1.y-p2.y))
    res = 0;  
  else if (p1.y < p2.y)
    res = 1;  
  else
    res = -1;  

  return(res);  
}

int LIqsortMinLines(const void *l1, const void *l2)
{
  caddr_t *auxl1, *auxl2;  

  auxl1 = (caddr_t *)l1;  
  auxl2 = (caddr_t *)l2;  
  return(-LIcompMinLines(*auxl1, *auxl2));  
}


int LIcompMinLines(caddr_t l1, caddr_t l2)
{
  t_lInfoP line1, line2;  
  int res;  
 
  line1 = (t_lInfoP) l1;  
  line2 = (t_lInfoP) l2;  

  /* handle vertical lines */
  if (BNisVertical(&(line1->line)) && BNisVertical(&(line2->line)))
    if (line1->line.a < line2->line.a)
      res = -1;  
    else if (line1->line.a == line2->line.a)
      res = 0;  
    else
      res = -1;  
  else if (BNisVertical(&(line1->line)))
    res = 1;  
  else if (BNisVertical(&(line2->line)))
    res = -1;  
  else
    res = LIcompLines(line1, line2, COMP_MIN);  
  return(res);  
}


int LIcompMaxLines(caddr_t l1, caddr_t l2)
{
  t_lInfoP line1, line2;  
  int res;  

  line1 = (t_lInfoP) l1;  
  line2 = (t_lInfoP) l2;  
  /* handle vertical lines */
  if (BNisVertical(&(line1->line)) && BNisVertical(&(line2->line)))
    if (line1->line.a < line2->line.a)
      res = -1;  
    else if (line1->line.a == line2->line.a)
      res = 0;  
    else
      res = -1;  
  else if (BNisVertical(&(line1->line)))
    res = -1;  
  else if (BNisVertical(&(line2->line)))
    res = 1;  
  else
    res = LIcompLines(line1, line2, COMP_MAX);  
  return(res);  
}





