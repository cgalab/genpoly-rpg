/*****************************************************************************/
/*                                                                           */
/*                       Random Polygon Generator (RPG)                      */
/*                                                                           */
/* Copyright (C)  T. Auer, M. Gschwandtner, M. Heimlich, M. Held   1994-2020 */
/*                                                                           */
/*****************************************************************************/

#include "basicAVL.h"
#include "eralloc.h"
#include "basicDefs.h"
#include "basicGeom.h"

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

struct t_treeNode *LeftBalanceInsert(struct t_treeNode *aTree, int *heightMod);  

struct t_treeNode *RightBalanceInsert(struct t_treeNode *aTree, int *heightMod);  

struct t_treeNode *LeftBalanceDelete(struct t_treeNode *aTree, int *heightMod);  

struct t_treeNode *RightBalanceDelete(struct t_treeNode *aTree, int *heightMod);  

struct t_treeNode *SingleLeftRotation(struct t_treeNode *aTree, 
				      int *heightMod);  

struct t_treeNode *SingleRightRotation(struct t_treeNode *aTree, 
				       int *heightMod);  

struct t_treeNode *DoubleLeftRightRotation(struct t_treeNode *aTree);  

struct t_treeNode *DoubleRightLeftRotation(struct t_treeNode *aTree);  


void ATinitTree(t_AVLtree *aTree)
{
  aTree->root =  NULL;  
}

void ATrecFreeElem(struct t_treeNode *aTree, void delProc(void *))
{
  if (aTree != NULL)
    {
      if (aTree->elem != NULL)
	delProc(aTree->elem);  

      ATrecFreeElem(aTree->left, delProc);  
      ATrecFreeElem(aTree->right, delProc);  
      erfree(aTree);  
    }
}

void ATfreeTElem(t_AVLtree *aTree, void delProc(void *))
{
  ATrecFreeElem(aTree->root, delProc);  
  aTree->root = NULL;  
}

void ATrecFree(struct t_treeNode *aTree)
{
  if (aTree != NULL)
    {
      ATrecFree(aTree->left);  
      ATrecFree(aTree->right);  
      erfree(aTree);  
    }
}

void ATfreeTree(t_AVLtree *aTree)
{
  ATrecFree(aTree->root);  
  aTree->root = NULL;  
}


int ATisEmpty(t_AVLtree *aTree)
{
  return((aTree->root) == NULL);  
}

caddr_t ATrecViewMin(struct t_treeNode *aTree)
{
  caddr_t res;  

  if (aTree == NULL)
    res = NULL;  
  else if (aTree->left == NULL)
    res = aTree->elem;  
  else
    res = ATrecViewMin(aTree->left);  

  return(res);  
}

caddr_t ATrecViewMax(struct t_treeNode *aTree)
{
  caddr_t res;  

  if (aTree == NULL)
    res = NULL;  
  else if (aTree->right == NULL)
    res = aTree->elem;  
  else
    res = ATrecViewMax(aTree->right);  

  return(res);  
}

caddr_t ATviewMin(t_AVLtree *aTree)
{
  return(ATrecViewMin(aTree->root));  
}

caddr_t ATviewMax(t_AVLtree *aTree)
{
  return(ATrecViewMax(aTree->root));  
}

caddr_t ATrecSearch(struct t_treeNode *aTree, caddr_t elem, 
		    int compProc(caddr_t, caddr_t))
{
  caddr_t res;  
  int compRes;  

  if (aTree != NULL)
    {
      compRes = compProc(elem, aTree->elem);  
      if (compRes == 0)
	res = aTree->elem;  
      else if (compRes == -1)
	res = ATrecSearch(aTree->left, elem, compProc);  
      else
	res = ATrecSearch(aTree->right, elem, compProc);  
    }
  else
    res = NULL;  
  
  return(res);  
}

caddr_t ATsearch(t_AVLtree *aTree, caddr_t elem, int compProc(caddr_t, caddr_t))
{
  return(ATrecSearch(aTree->root, elem, compProc));  
}

  
struct t_treeNode *Insert(struct t_treeNode *aTree, caddr_t elem, int *heightMod, 
	    int compProc(caddr_t, caddr_t), int *res)
{
  int compRes;  

  if (aTree == NULL) 
    {
      aTree = (struct t_treeNode *)eralloc(sizeof(struct t_treeNode));  
      (*heightMod) = TRUE;  

      aTree->left = NULL;  
      aTree->right = NULL;  
      aTree->balance = AT_BAL;  
      aTree->elem = elem;  
      
      (*res) = TRUE;  
    }
  else
    {
      compRes = (compProc(elem, aTree->elem));  
      if (compRes == -1)
	{
	  /* insert in left subtree */
	  aTree->left = Insert(aTree->left, elem, heightMod, compProc, res);  
	  if (*heightMod)
	    aTree = LeftBalanceInsert(aTree, heightMod);  
	}
      else if (compRes == 1)
	{
	  /* insert in right subtree */
	  aTree->right = Insert(aTree->right, elem, heightMod, compProc, res);  
	  if  (*heightMod)  
            aTree = RightBalanceInsert(aTree, heightMod);  
	}     
      else
	{
	  /* element exists already, so do nothing */
	  (*heightMod) = FALSE;  
	  (*res) = FALSE;  
	}      
    }

  return(aTree);  
}


int ATaddElem(t_AVLtree *aTree, caddr_t elem, int compProc(caddr_t, caddr_t))
{
  int heightMod;  
  int res;  

  aTree->root = Insert(aTree->root, elem, &(heightMod), compProc, &res);  

  return(res);  
}


struct t_treeNode *LeftBalanceInsert(struct t_treeNode *aTree, int *heightMod)
{
  switch (aTree->balance) 
    {
    case AT_RIGHT:
      aTree->balance = AT_BAL;  
      (*heightMod) = FALSE;  
      break;  
 
    case AT_BAL: 
      aTree->balance = AT_LEFT;  
      break;  
      
    case AT_LEFT:
      if  (aTree->left->balance == AT_LEFT)         
	aTree = SingleLeftRotation(aTree, heightMod);  
      else
	aTree = DoubleLeftRightRotation(aTree);  
   
      (*heightMod) = FALSE;  
      break;  

    default:
      break;  
    }

  return(aTree);  
}


struct t_treeNode *RightBalanceInsert(struct t_treeNode *aTree, int *heightMod)
{
  switch(aTree->balance)
    { 
    case AT_LEFT: 
      aTree->balance = AT_BAL;  
      (*heightMod) = FALSE;  
      break;  

    case AT_BAL: 
      aTree->balance = AT_RIGHT;  
      break;  

    case AT_RIGHT: 
      if  (aTree->right->balance == AT_RIGHT)
	aTree = SingleRightRotation(aTree, heightMod);  
      else                            
	aTree = DoubleRightLeftRotation(aTree);  
    
      (*heightMod) = FALSE;  
      break;  

    default:
      break;  
    }          

  return(aTree);  
}


struct t_treeNode *KeepSorted(struct t_treeNode *aTree, caddr_t *elem, 
			      int *heightMod)
{
  struct t_treeNode *auxNode;  
  
  if  (aTree->left == NULL)
    {
      (* elem) = aTree->elem;        
      auxNode = aTree;  
      aTree = aTree->right;  
      erfree(auxNode);  
      (*heightMod) = TRUE;  
    }
  else
    {
      aTree->left = KeepSorted(aTree->left, elem, heightMod);  
      if  (*heightMod)                    
	aTree = RightBalanceDelete(aTree, heightMod);  
    }
  return(aTree);  
}


struct t_treeNode *Delete(struct t_treeNode *aTree, caddr_t elem, int *heightMod, 
			  int compProc(caddr_t, caddr_t), void delProc(void *))
{
  struct t_treeNode *auxNode;  
  caddr_t auxElem;  

  int compRes;  
  
  if (aTree != NULL)
    {
      compRes = compProc(elem, aTree->elem);  
      
      if (compRes == -1)
	{
	  aTree->left = Delete(aTree->left, elem, heightMod, compProc, delProc);  
	  if  (*heightMod)
	    aTree = RightBalanceDelete(aTree, heightMod);  
	}
      else if (compRes == 1)
	{
	  aTree->right = Delete(aTree->right, elem, heightMod, compProc, delProc);  
     
	  if (*heightMod)                      
	    aTree = LeftBalanceDelete(aTree, heightMod);  
	}    
      else
	{
	  delProc(aTree->elem);  

	  if (aTree->left == NULL)
	    {
	      auxNode = aTree;   
	      aTree = aTree->right;   
	      erfree(auxNode);  
	      (*heightMod) = TRUE;  
	    }             
          else if (aTree->right == NULL)
	    {
	      auxNode = aTree;   
	      aTree = aTree->left;    
	      erfree(auxNode);  
	      (*heightMod) = TRUE;  
	    }
          else
	    {
	      aTree->right = KeepSorted(aTree->right, &(auxElem), heightMod);  
	      aTree->elem = auxElem;  
	      if (*heightMod)
		aTree = LeftBalanceDelete(aTree, heightMod) ;  
	    }
	}
    }
  else
    (*heightMod) = FALSE;  

  return(aTree);    
}
      

void ATdelElem(t_AVLtree *aTree, caddr_t elem, int compProc(caddr_t, caddr_t), 
	       void delProc(void *))
{
  int heightMod = FALSE;  

  aTree->root = Delete(aTree->root, elem, &(heightMod), compProc, delProc);  
}


struct t_treeNode *LeftBalanceDelete(struct t_treeNode *aTree, int *heightMod)
{
  switch(aTree->balance)
    {   
    case AT_RIGHT:
      aTree->balance = AT_BAL;  
      break;  

    case AT_BAL:
      aTree->balance = AT_LEFT;  
      (*heightMod) = FALSE;  
      break;  

    case AT_LEFT: 
      if ((aTree->left->balance == AT_BAL) ||
	  (aTree->left->balance == AT_LEFT))
	aTree = SingleLeftRotation(aTree, heightMod);  
      else
	aTree = DoubleLeftRightRotation(aTree);  
      break;  

    default:
      break;  
    }
  
  return(aTree);  
}
	

struct t_treeNode *RightBalanceDelete(struct t_treeNode *aTree, int *heightMod)
{
  switch(aTree->balance)
    {
    case AT_LEFT:
      aTree->balance = AT_BAL;  
      break;  
      
    case AT_BAL:
      aTree->balance = AT_RIGHT;  
      (*heightMod) = FALSE;  
      break;  

    case AT_RIGHT: 
      if  ((aTree->right->balance == AT_BAL) ||
	   (aTree->right->balance == AT_RIGHT))
	aTree = SingleRightRotation(aTree, heightMod);  
      else                            
	aTree = DoubleRightLeftRotation(aTree);  
      break;  

    default:
      break;  
    }
  
  return(aTree);  
}


struct t_treeNode *SingleLeftRotation(struct t_treeNode *aTree, 
				      int *heightMod)
{
  struct t_treeNode *left;  

  left = aTree->left;  
  aTree->left = left->right;  
  left->right = aTree;  
  
  if  (left->balance == AT_LEFT)
    {
      aTree->balance     = AT_BAL;  
      left->balance = AT_BAL;  
    }
  else
    {
      aTree->balance = AT_LEFT;  
      left->balance =  AT_RIGHT;  
      (*heightMod) = FALSE;  
    }

  aTree = left;  
  return(aTree);  
}


struct t_treeNode *SingleRightRotation(struct t_treeNode *aTree, 
				       int *heightMod)
{
  struct t_treeNode *right;  

  right = aTree->right;  
  aTree->right = right->left;  
  right->left = aTree;  

  if (right->balance == AT_RIGHT)
    {
      aTree->balance = AT_BAL;  
      right->balance = AT_BAL;  
    }
  else
    {
      aTree->balance = AT_RIGHT;  
      right->balance = AT_LEFT;  
      (*heightMod) = FALSE;   
    }

  aTree = right;  
  return(aTree);  
}


struct t_treeNode *DoubleLeftRightRotation(struct t_treeNode *aTree)
{
  struct t_treeNode *left, *lright;  

  left = aTree->left;  
  lright = left->right;  
  left->right = lright->left;  
  lright->left = left;  
  aTree->left = lright->right;  
  lright->right = aTree;  

  if  (lright->balance == AT_LEFT)
    aTree->balance = AT_RIGHT;  
  else
    aTree->balance = AT_BAL;  

  if  (lright->balance == AT_RIGHT)
    left->balance = AT_LEFT;  
  else
    left->balance = AT_BAL;  
    
  aTree = lright;  
  aTree->balance = AT_BAL;   

  return(aTree);  
}


struct t_treeNode *DoubleRightLeftRotation(struct t_treeNode *aTree)
{
  struct t_treeNode *rleft, *right;  

  right = aTree->right;  
  rleft = right->left;  
  right->left  = rleft->right;  
  rleft->right = right;  
  aTree->right = rleft->left;  
  rleft->left  = aTree;  

  if (rleft->balance == AT_RIGHT)
    aTree->balance = AT_LEFT;  
  else
    aTree->balance = AT_BAL;  

    
  if  (rleft->balance == AT_LEFT)
    right->balance = AT_RIGHT;  
  else
    right->balance = AT_BAL;  
  
  aTree = rleft;  
  aTree->balance = AT_BAL;                                 

  return(aTree);  
}


caddr_t randomGet(struct t_treeNode *aTree)
{
  double seed;  
  caddr_t result;  

  seed = randomDouble();  

  if ((seed < SPLIT_BASE) && (aTree->right != NULL))
    result = randomGet(aTree->right);  
  else if ((seed < 2*SPLIT_BASE) && (aTree->left != NULL))
    result = randomGet(aTree->left);  
  else
    result = aTree->elem;  

  return result;  
}


caddr_t ATrandomGet(t_AVLtree *aTree)
{
  return(randomGet(aTree->root));  
}





