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
  sc_trace(tf,b->data_f,"weight->bank_f");
  for (int i = 0; i < 7 ; i++){
    sc_trace(tf,a->data[i],"inputSRAM->data["+int2str(i)+"]");
  }
  for (int i = 0; i < 9 ; i++){
    sc_trace(tf,b->data[i],"weightSRAM->data["+int2str(i)+"]");
  }
  // sc_trace(tf,dram->peq_cb().ptr,"dram.peq_cb.ptr");
  // sc_trace(tf,irpt_clear,"irpt_clear");
  // sc_trace(tf,re_data,"re_data");
  // sc_trace(tf,m_data,"m_data");   
  // sc_trace(tf,m_address,"m_address");
  // sc_trace(tf,rw,"rw");


//write data to memory
  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  for (int i =0 ; i < 80 ; i++){
    dram->mem[i] = i+1;
  }
  s_addr = 0;
  t_addr = 0x50000000;
  write = 1;
  length = 28;
  sram_mode = 0;
  sc_start(5, SC_NS);
  write = 0;
  sc_start(70, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // cout << " 1=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[0][i][j]<< " ";
  // }
  // cout << endl << " 2=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[1][i][j]<< " ";
  // }
  // cout << endl<< " 3=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[2][i][j]<< " ";
  // }
  // cout << endl;

  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr = 28;
  t_addr = 0x50000024;
  write = 1;
  length = 28;
  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  
  // cout << " 1=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[0][i][j]<< " ";
  // }
  // cout << endl << " 2=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[1][i][j]<< " ";
  // }
  // cout << endl<< " 3=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[2][i][j]<< " ";
  // }
  // cout << endl;

  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr =72;
  t_addr = 0x51000038;
  write = 1;
  length = 36;
  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // cout << " 1=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[0][i][j]<< " ";
  // }
  // cout << endl << " 2=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[1][i][j]<< " ";
  // }
  // cout << endl<< " 3=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[2][i][j]<< " ";
  // }
  // cout << endl;
  
  // sc_start(10, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // s_addr =108;
  // t_addr = 0x51000038;
  // write = 1;
  // length = 36;
  // sc_start(5, SC_NS);
  // write = 0;

  // sc_start(70, SC_NS);

  //  sc_start(10, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // s_addr =144;
  // t_addr = 0x51000038;
  // write = 1;
  // length = 36;
  // sc_start(5, SC_NS);
  // write = 0;

  // sc_start(70, SC_NS);

  // sc_start(10, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // s_addr =180;
  // t_addr = 0x51000038;
  // write = 1;
  // length = 36;
  // sc_start(5, SC_NS);
  // write = 0;

  // sc_start(70, SC_NS);
  
  // sc_start(10, SC_NS);
  // cout << sc_time_stamp() << "  -----------------------------" << endl;
  // s_addr =216;
  // t_addr = 0x51000038;
  // write = 1;
  // length = 36;
  // sc_start(5, SC_NS);
  // write = 0;

  // sc_start(70, SC_NS);

  // cout << " 1=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[0][i][j]<< " ";
  // }
  // cout << endl << " 2=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[1][i][j]<< " ";
  // }
  // cout << endl<< " 3=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[2][i][j]<< " ";
  // }
  // cout << endl;

  // cout << " 4=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[3][i][j]<< " ";
  // }
  // cout << endl << " 5=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[4][i][j]<< " ";
  // }
  // cout << endl<< " 6=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[5][i][j]<< " ";
  // }
  // cout << endl;

  // cout << " 1.1=>";
  // for (int i =0 ; i < 3; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << b->data_sram[0][i][j+3]<< " ";
  // }
  // cout << endl;

  // sram_read = 1 ;
  // sc_start(5, SC_NS);
  // for (int i =0 ; i < 8; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << temp2[i][j]<< " ";

  //   cout <<endl;
  // }

  // sc_start(5, SC_NS);
  // for (int i =0 ; i < 8; i++){
  //   for (int j = 0; j< 3; j++)
  //   cout << temp2[i][j]<< " ";

  //   cout <<endl;
  // }

  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr = 84;
  t_addr = 0x50000038;
  write = 1;
  length = 28;
  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);

  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr = 112;
  t_addr = 0x50000038;
  write = 1;
  length = 28;
  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);

  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr = 140;
  t_addr = 0x50000038;
  write = 1;
  length = 28;
  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);

  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr = 168;
  t_addr = 0x50000038;
  write = 1;
  length = 28;
  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);
  sc_start(10, SC_NS);
  cout << sc_time_stamp() << "  -----------------------------" << endl;
  s_addr = 196;
  t_addr = 0x50000038;
  write = 1;
  length = 28;

  sc_start(5, SC_NS);
  write = 0;

  sc_start(70, SC_NS);

  cout << " 1=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[0][i][0];
  }
  cout << endl << " 2=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[1][i][0];
  }
  cout << endl<< " 3=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[2][i][0];
  }
  cout << endl;
  cout << " 4=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[3][i][0];
  }
  cout << endl << " 5=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[4][i][0];
  }
  cout << endl<< " 6=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[5][i][0];
  }
  cout << endl;
  cout << endl<< " 1.1=>";
  for (int i =0 ; i < 7; i++){
    cout << a->data_sram[0][i][1];
  }
  cout << endl;
  cout << "DATABUS" << "............"<<endl;
  sram_read = 1;
  sc_start(5, SC_NS);
  for (int i =0 ; i < 8; i++){
    for (int j = 0; j< 7; j++)
    cout << temp[i][j]<< " ";

    cout <<endl;
  }

  sc_start(5, SC_NS);
  for (int i =0 ; i < 8; i++){
    for (int j = 0; j< 7; j++)
    cout << temp[i][j]<< " ";

    cout <<endl;
  }

  
  return 0;
}
