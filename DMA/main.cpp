#include <iostream>
#include <systemc.h> 
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm.h"
#include "DMA.h"
#include "DRAM.h"
#include "inputSRAM.h"
#include "weightSRAM.h"


#define SC_INCLUDE_DYNAMIC_PROCESSES

using namespace sc_core;
using namespace sc_dt;
using namespace std;

std::string int2str(int k){
  std::stringstream ss;
  ss<<k;
  return ss.str(); }

int sc_main(int argc, char* argv[])
{
  sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                           sc_core::SC_DO_NOTHING );
  

  DMA *dma; //initiator
  DRAM *dram;   //target

//signal
  sc_clock clock("clock", 5);

  sc_signal<bool>  clk;
  sc_signal<bool>  rst;
  sc_signal<bool>  write;
  sc_signal<bool>  read;
  sc_signal<sc_uint<ADDR_LENGTH> > s_addr;
  sc_signal<sc_uint<ADDR_LENGTH> > t_addr;
  sc_signal<sc_uint<32> > length;
  sc_signal<sc_uint<32> > input_l;
  sc_signal<sc_uint<32> > width;
  sc_signal<sc_uint<32> > high;
  sc_signal<sc_uint<DATA_LENGTH> > data_out;
  sc_signal<bool> sram_mode;
  sc_signal<bool> done;
  sc_signal<bool> busy;

  sc_signal<bool> sram_read;
  sc_signal<bool> sram_done1;
  sc_signal<bool> sram_done2;
  sc_signal<sc_uint<32> > sram_length;
  sc_signal<bool> sram_write1; // to inputsram
  sc_signal<sc_uint<32> > databus1[7];
  sc_signal<bool> mode;
  sc_signal<bool> vaild;
  sc_signal<sc_uint<32> > temp[8][7];


  sc_signal<bool> sram_write2; // to weightsram
  sc_signal<sc_uint<32> > databus2[9];
  sc_signal<bool> vaild2;
  sc_signal<sc_uint<32> > temp2[8][7];
  sc_signal<sc_uint<32> > w_bank_addr[8];

  

  //uint32_t memory[10];

  dma = new DMA("DMA");
  dram = new DRAM("dram");
  inputSRAM *a = new inputSRAM("inputSRAM");
  weightSRAM *b = new weightSRAM("weightSRAM");

  dma->clk(clock);
  dma->rst(rst);
  dma->read_in(write);
  dma->write_back(read);
  dma->source_address(s_addr);
  dma->target_address(t_addr);
  dma->data_length(length);
  dma->input_width(input_l);
  dma->tile_width(width);
  dma->tile_high(high);
  dma->sram_mode(sram_mode);
  dma->socket.bind(dram->socket);

  dma->done(done);
  dma->busy(busy);

  dma->isram_done(sram_done1);
  dma->wsram_done(sram_done2);
  dma->input_SRAM_write(sram_write1);
  dma->to_SRAM_length(sram_length);
  for (int i = 0; i < 7 ; i++){
    dma->to_SRAM_data[i](databus1[i]);
  }
  dma->to_SRAM_mode(mode);
  dma->weight_SRAM_write(sram_write2);
  for (int i = 0; i < 9; i++){
    dma->weight_data[i](databus2[i]);
  }
  

  a->clk(clock);
  a->rst(rst);
  a->pe_read(sram_read);
  a->dma_write(sram_write1);
  a->mode(mode);
  for (int i = 0; i < 7 ; i++){
    a->data[i](databus1[i]);
  }

  a->length(sram_length);
  for (int i = 0; i < 8 ; i++){
    for (int j = 0 ; j < 7; j++) {
      a->data_to_pe[i][j](temp[i][j]);
    }
  }
  a->vaild(vaild);
  a->done(sram_done1);

  b->clk(clock);
  b->rst(rst);
  b->pe_read(sram_read);
  for (int i = 0; i < 8 ; i++){
    b->weight_bank_addr[i](w_bank_addr[i]);
  }
  b->dma_write(sram_write2);
  b->mode(mode);
  for (int i = 0; i < 9 ; i++){
    b->data[i](databus2[i]);
  }
  b->length(sram_length);
  for (int i = 0; i < 8 ; i++){
    for (int j = 0 ; j < 3; j++) {
      b->data_to_pe[i][j](temp2[i][j]);
    }
  }
  b->vaild(vaild2);
  b->done(sram_done2);



  sc_trace_file *tf = sc_create_vcd_trace_file("result");
  sc_trace(tf,clock,"clk");
  sc_trace(tf,dma->input_SRAM_write,"dma->input_SRAM_write");
  sc_trace(tf,dma->read_in,"read_in");
  sc_trace(tf,dma->state,"dma->state");
  sc_trace(tf,dma->target,"dma->target");
  sc_trace(tf,a->done,"inputSRAM->done");
  sc_trace(tf,a->data_f,"inputSRAM->bank_f");
  sc_trace(tf,dma->source,"source");
  sc_trace(tf,dma->source_base,"source_base");
  sc_trace(tf,a->dma_write,"inputSRAM->dram_write");
  sc_trace(tf,a->mode,"mode");
  sc_trace(tf,dma->isram_done,"done");
  sc_trace(tf,dma->tile_count,"tile_count");
  sc_trace(tf,dma->input_size,"input_size");
  sc_trace(tf,dma->tile_width,"tile_width");
  sc_trace(tf,dma->tile_high,"tile_high");
  sc_trace(tf,dma->length,"length");
  for (int i = 0; i < 7 ; i++){
    sc_trace(tf,a->data[i],"inputSRAM->data["+int2str(i)+"]");
  }
  for (int i = 0; i < 7 ; i++){
    sc_trace(tf,dma->to_SRAM_data[i],"dma->to_sram_data["+int2str(i)+"]");
  }
  for (int i = 0; i < 7 ; i++){
    sc_trace(tf,dma->weight_data[i],"dma->weight_data["+int2str(i)+"]");
  }
  // for (int i = 0; i < 9 ; i++){
  //   sc_trace(tf,b->data[i],"weightSRAM->data["+int2str(i)+"]");
  // }
  // sc_trace(tf,dram->peq_cb().ptr,"dram.peq_cb.ptr");
  // sc_trace(tf,irpt_clear,"irpt_clear");
  // sc_trace(tf,re_data,"re_data");
  // sc_trace(tf,m_data,"m_data");   
  // sc_trace(tf,m_address,"m_address");
  // sc_trace(tf,rw,"rw");


//write data to memory

  // inpu SRAM Test
  // for (int i =0 ; i <10000000 ; i++){
  //   dram->mem[i] = i+1;
  // }
  // sc_start(10, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  
  // s_addr = 0;
  // t_addr = 0x50000000;
  // write = 1;
  // input_l= 416;
  // width = 6;
  // high = 7;
  // length = 1680;
  // sram_mode = 1;
  // sc_start(5, SC_NS);
  // write = 0;
  // sc_start(2000, SC_NS);

  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // cout << "DONE:" <<  done << endl;

  
  // for (int k =0 ; k < 8; k++){
  //   cout << " bank" << k <<endl;
  //   for (int i =0 ; i < 7; i++){
  //     for (int j = 0 ; j < 6 ; j ++)
  //       cout << a->data_sram[k][i][j] << " ";
  //     cout << endl;
  //   }
  //   cout<<"*****************" << endl;
  // }
 
  // for (int k =0 ; k < 8; k++){
  //   cout << " bank" << k <<endl;
  //   for (int i =0 ; i < 7; i++){
  //     for (int j = 6 ; j < 12 ; j ++)
  //       cout << a->data_sram[k][i][j] << " ";
  //     cout << endl;
  //   }
  //   cout<<"*****************" << endl;
  // }

  // weight SRAM Test
  for (int i =0 ; i <1000 ; i++){
    dram->mem[i] = i+1;
  }
  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  
  s_addr = 0;
  t_addr = 0x51000000;
  write = 1;
  input_l= 0;
  width = 6;
  high = 7;
  length = 432;
  sram_mode = 1;
  sc_start(5, SC_NS);
  write = 0;
  sc_start(1000, SC_NS);

  cout << sc_time_stamp() << "  -----------------------------" << endl;
  cout << "DONE:" <<  done << endl;

  
  for (int k =0 ; k < 8; k++){
    cout << " bank" << k <<endl;
    for (int i =0 ; i < 3; i++){
      for (int j = 0 ; j < 6 ; j ++)
        cout << b->weight_sram[k][i][j] << " ";
      cout << endl;
    }
    cout<<"*****************" << endl;
  }
 
  // for (int k =0 ; k < 8; k++){
  //   cout << " bank" << k <<endl;
  //   for (int i =0 ; i < 7; i++){
  //     for (int j = 6 ; j < 12 ; j ++)
  //       cout << b->weight_sram[k][i][j] << " ";
  //     cout << endl;
  //   }
  //   cout<<"*****************" << endl;
  // }

  
  return 0;
}
