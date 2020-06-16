/** @file mm.cpp
 *  @brief Function prototypes for the mm module.
 *
 *  This contains memory manager to manage the transcation
 *
 *  @author HarveyZeng (caslab)
 *  
 *  @bug No known bugs.
 */

#include "mm.h"


mm::gp_t* mm::allocate()
{

  gp_t* ptr;
  if (free_list)
  {
    ptr = free_list->trans;
    empties = free_list;
    free_list = free_list->next;
  }
  else
  {
    ptr = new gp_t(this);
  }
  return ptr;
}

void mm::free(gp_t* trans)
{

  if (!empties)
  {
    empties = new access;
    empties->next = free_list;
    empties->prev = 0;
    if (free_list)
      free_list->prev = empties;
  }
  free_list = empties;
  free_list->trans = trans;
  empties = free_list->prev;
}
