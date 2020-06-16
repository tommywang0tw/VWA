/** @file mm.h
 *  @brief Function prototypes for the mm module.
 *
 *  This contains memory manager to manage the transcation
 *
 *  @author HarveyZeng (caslab)
 *  
 *  @bug No known bugs.
 */
#ifndef MM_H
#define MM_H

#include "top.h"

// **************************************************************************************
// User-defined memory manager, which maintains a pool of transactions
// **************************************************************************************

class mm: public tlm::tlm_mm_interface
{
  typedef tlm::tlm_generic_payload gp_t;

public:
  mm() : free_list(0), empties(0)
  #ifdef DEBUG
  , count(0)
  #endif
  {}

  gp_t* allocate();
  void  free(gp_t* trans);

private:
  struct access
  {
    gp_t* trans;
    access* next;
    access* prev;
  };

  access* free_list;
  access* empties;

  #ifdef DEBUG
  int     count;
  #endif
};
#endif