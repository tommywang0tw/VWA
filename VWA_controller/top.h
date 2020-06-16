/** @file top.h
 *  @brief Function prototypes for the top module.
 *
 *  This contains the all the module and the combined socket
 *
 *  @author HarveyZeng (caslab)
 *  
 *  @bug No known bugs.
 */
#ifndef TOP_H
#define TOP_H
// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc.h>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "ps_config.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

#endif
