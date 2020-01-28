/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __CALCPOLY_H_
#define __CALCPOLY_H_

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/

enum t_calcType {
  CP_IDLE, 
  CP_POINTS, 
  CP_CIRCLEPOINTS, 
  CP_CLUSTERPOINTS, 
  CP_SMOOTH, 
  CP_BOUNCE, 
  CP_XMONO, 
  CP_ARRANGE_STAR, 
  CP_FAST_STAR, 
  CP_STAR, 
  CP_GROWING, 
  CP_GROWING_II, 
  CP_2OPT, 
  CP_2OPT_II, 
  CP_2OPT_III, 
  CP_SPACE_PART, 
  CP_REJECT, 
  CP_SEARCH, 
  CP_TRIANGLE, 
  CP_HOLES
};  


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
void CPsetState(enum t_calcType state, int parameter);  

void CPdoCalc();  

#endif
