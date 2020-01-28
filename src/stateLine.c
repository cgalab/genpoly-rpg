/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "stateLine.h"
#include <string.h>

/********************************************************************/
/*                                                                  */
/* Constants and the like                                           */
/*                                                                  */
/********************************************************************/
static enum t_stateInfo state;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void SLsetState(enum t_stateInfo stateInfo)
{
  state = stateInfo;  
  //MFdisplayState();  
}


void SLgetStateString(char *stateString)
{
  switch(state)
  {
  case ST_OK:
    strcpy(stateString, "Normal.");  
    break;  

  case ST_FLE_N_FND:
    strcpy(stateString, "File not found.");  
    break;  

  case ST_INV_OP:
    strcpy(stateString, "Operation not possible.");  
    break;  

  case ST_ADD_P:
    strcpy(stateString, 
           "LEFT button to add point, RIGHT button to end adding.");  
    break;  

  case ST_DEL_P:
    strcpy(stateString, 
           "LEFT button to delete point, RIGHT button to end deleting.");  
    break;  

  case ST_SMOOTH:
    strcpy(stateString, 
           "Smoothing polygon...");  
    break;  

  case ST_POINTS:
    strcpy(stateString, 
           "Computing random points (square)...");  
    break;  

  case ST_CIRCLEPOINTS:
    strcpy(stateString, 
           "Computing random points (circle)...");  
    break;  

  case ST_XMONO:
    strcpy(stateString, 
           "Computing x-monotone polygon...");  
    break;  

  case ST_BOUNCE:
    strcpy(stateString, 
           "Computing Bouncing Vertices...");  
    break;  

  case ST_STAR:
    strcpy(stateString, 
           "Computing star-shaped polygon...");  
    break;  

  case ST_GROW:
    strcpy(stateString, 
           "Computing Steady Growth...");  
    break;  

  case ST_GROW_II:
    strcpy(stateString, 
           "Computing Steady Growth II...");  
    break;  

  case ST_2OPT:
    strcpy(stateString, 
           "Computing 2-opt Moves...");  
    break;  

  case ST_2OPT_II:
    strcpy(stateString, 
           "Computing 2-opt Moves II...");  
    break;  

  case ST_2OPT_III:
    strcpy(stateString, 
           "Computing 2-opt Moves III...");  
    break;  

  case ST_SPACE_PART:
    strcpy(stateString, 
           "Computing Space Partitioning...");  
    break;  

  case ST_REJECT:
    strcpy(stateString, 
           "Computing Permute & Rejet...");  
    break;  

  case ST_SEARCH:
    strcpy(stateString, 
           "Computing Incremental Construction...");  
    break;  

  case ST_TRIANGLE:
    strcpy(stateString, 
           "Computing Triangle Select...");  
    break;  

  case ST_ZOOM1:
    strcpy(stateString, 
           "Select FIRST point for zoom window.");  
    break;  

  case ST_ZOOM2:
    strcpy(stateString, 
           "Select SECOND point for zoom window.");  
    break;  

  default:
    strcpy(stateString, "Unknown state.");  
    break;  
  }
}






