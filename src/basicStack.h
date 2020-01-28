/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include <sys/types.h>
#include <stdlib.h>

#ifndef __BASICSTACK_H_
#define __BASICSTACK_H_

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

struct t_stackElem {
  caddr_t elem;  
  struct t_stackElem *next;  
};  

typedef struct {
  struct t_stackElem *top;  
} t_stack;  

/********************************************************************/
/*                                                                  */
/* Procedures and functions                                         */
/*                                                                  */
/********************************************************************/

void BSinitStack(t_stack *aStack);  

void BSpushElem(t_stack *aStack, caddr_t elem);  

caddr_t BSviewTop(t_stack *aStack);  

caddr_t BSpopElem(t_stack *aStack);  

int BSisEmpty(t_stack *aStack);  

void BSfreeStack(t_stack *aStack);  

#endif




