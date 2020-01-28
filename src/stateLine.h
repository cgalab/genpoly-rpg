/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#ifndef __STATELINE_H_
#define __STATELINE_H_

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

enum t_stateInfo {
  ST_OK, 
  ST_POINTS, 
  ST_CIRCLEPOINTS, 
  ST_FLE_N_FND, 
  ST_INV_OP, 
  ST_ADD_P, 
  ST_DEL_P, 
  ST_SMOOTH, 
  ST_BOUNCE, 
  ST_XMONO, 
  ST_STAR, 
  ST_GROW, 
  ST_GROW_II, 
  ST_2OPT, 
  ST_2OPT_II, 
  ST_2OPT_III, 
  ST_SPACE_PART, 
  ST_REJECT, 
  ST_SEARCH, 
  ST_ZOOM1, 
  ST_ZOOM2, 
  ST_TRIANGLE
};  


/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void SLsetState(enum t_stateInfo stateInfo);  

//void SLgetStateString(char *stateString);  

#endif








