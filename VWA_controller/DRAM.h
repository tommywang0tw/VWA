/** @file DRAM.h
 *  @brief Function prototypes for the DRAM module.
 *
 *  This contains DRAM module
 *
 *  @Author: HarveyZeng (caslab), LanceHuang (caslab)
 *
 *  @bug No known bugs.
 */
#ifndef DRAM_H
#define DRAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ps_config.h"
#include "general_function.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"


//===input size===
#define input_WIDTH (18)
#define input_HEIGHT (18)
#define input_CHANNEL (16)
//===filter size===
#define filter_SIZE (3)
#define filter_CHANNEL (1)
//===output size===
#define output_WIDTH (16)
#define output_HEIGHT (16)



// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

DECLARE_EXTENDED_PHASE(internal_ph);
using namespace std;

struct DRAM: sc_module {
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_target_socket<DRAM> socket;

    SC_CTOR(DRAM)
        : socket("socket")
        , n_trans(0)
        , response_in_progress(false)
        , next_response_pending(0)
        , end_req_pending(0)
        , m_peq(this, &DRAM::peq_cb)
    {
        // Register callbacks for incoming interface method calls
        socket.register_nb_transport_fw(this, &DRAM::nb_transport_fw);
        dram_id = DRAM4_ID;
        row_index = 0xffffffff; // Initial value
/*
        for(int i=0; i<(MEM_SIZE/4); i++){
            mem[i] = i;
        }
*/


        //load input
        int *input = new int[input_WIDTH * input_HEIGHT * input_CHANNEL];
        int *weight = new int[filter_SIZE * filter_SIZE * input_CHANNEL * filter_CHANNEL];
        int tmp;

        ifstream fin("data.txt");
        for(int i = 0; i < (input_WIDTH * input_HEIGHT * input_CHANNEL); i++){
            fin >> tmp;
            input[i] = tmp;
        }
        
        //input covert to column major
        for(int k = 0; k < input_CHANNEL; k++){
            for (int i = 0; i < input_HEIGHT; i++) {
                for (int j = 0; j < input_WIDTH; j++) {
                    int index = j * input_HEIGHT + i;
                    mem[i * input_HEIGHT + j + (input_HEIGHT * input_WIDTH * k)] = input[index];
                }
            }
        }
        for(int i=0; i < 100; i++)
            cout << mem[i] << " ";
        //cout << "mem[18]: " << mem[18] << endl;


        for(int i = (input_WIDTH * input_HEIGHT * input_CHANNEL); i < (DRAM4_WEIGHT_BASE/4); i++){
            mem[i] = 0;
        }


        //load weight
        ifstream fweight("weight.txt");
        for(int i = 0; i < (filter_SIZE * filter_SIZE * input_CHANNEL * filter_CHANNEL); i++){
            fweight >> tmp;
            weight[i] = tmp;
            //cout << "index : " << i << " data : " << weight[i] << endl;
        }
        
        //weight covert to column major
        for(int a = 0; a < filter_CHANNEL; a++){
            for(int k = 0; k < input_CHANNEL; k++){
                for (int i = 0; i < filter_SIZE; i++) {
                    for (int j = 0; j < filter_SIZE; j++) {
                        int index = j * filter_SIZE + i;
                        mem[i * filter_SIZE + j + (k * filter_SIZE * filter_SIZE) + (a * input_CHANNEL * filter_SIZE * filter_SIZE) + DRAM4_WEIGHT_BASE/4] = weight[index + (k * filter_SIZE * filter_SIZE) + (a * input_CHANNEL * filter_SIZE * filter_SIZE)];
                        cout << "index : " << index << " weight[index] : " <<weight[index] << endl;
                        //cout << "mem[" << i * filter_SIZE + j + (k * filter_SIZE * filter_SIZE) + (a * input_CHANNEL * filter_SIZE * filter_SIZE) + DRAM4_WEIGHT_BASE/4 << "]: " << mem[i * filter_SIZE + j + (k * filter_SIZE * filter_SIZE) + (a * input_CHANNEL * filter_SIZE * filter_SIZE) + DRAM4_WEIGHT_BASE/4] << endl;
                    }
                }
            }
        }

        
    }

    // TLM-2 non-blocking transport method

    virtual tlm::tlm_sync_enum nb_transport_fw( tlm::tlm_generic_payload& trans,
            tlm::tlm_phase& phase, sc_time& delay );
    void peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase);
    tlm::tlm_sync_enum send_end_req(tlm::tlm_generic_payload& trans);
    void send_response(tlm::tlm_generic_payload& trans);
    int get_id();
    uint32_t dram_id;
    int   n_trans;
    bool  response_in_progress;
    tlm::tlm_generic_payload*  next_response_pending;
    tlm::tlm_generic_payload*  end_req_pending;
    tlm_utils::peq_with_cb_and_phase<DRAM> m_peq;
    uint32_t mem[MEM_SIZE/4]; // 256MB(8B * 0x2000000)

    // DRAM timing
    int calculate_delay(int addr);
    uint32_t row_hit_miss_latency(int addr);
    int get_row_index(int);
    void access(bool,uint32_t,unsigned int ,uint32_t*);
    int row_index;

};

#endif
