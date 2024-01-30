#ifndef __GRAPH_L9_B2_A0_C1_H__
#define __GRAPH_L9_B2_A0_C1_H__

#include <adf.h>
#include "para_L9.h"
#include <stdio.h>
using namespace adf;

template <int COL_OFFSET, int ROW_OFFSET>
class simpleGraph_k7_L9_B2_A0_C1: public graph {
  public:
    kernel mm[L9_NUM_KERNEL];
    port<input> in[L9_NUM_KERNEL];
    port<output> out;
    
    simpleGraph_k7_L9_B2_A0_C1() {
      
      mm[0] = kernel::create(mm7_kernel0_L9_B0_A0_C1);
      source(mm[0]) = "mm7_kernel0_L9_B0_A0_C1.cc";
      mm[1] = kernel::create(mm7_kernel2_L9_B1_A0_C1);
      source(mm[1]) = "mm7_kernel2_L9_B1_A0_C1.cc";

      for (int i=0; i<L9_NUM_KERNEL;i++){
        runtime<ratio>(mm[i]) = 0.9;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L9_NUM_KERNEL;i++){
        if ((ROW_OFFSET%2)==0){
          location<buffer>(mm[i].in[0]) =
          { address(COL_OFFSET+i, ROW_OFFSET+1, 0x5000), 
            address(COL_OFFSET+i, ROW_OFFSET+1, 0x7000)};
          location<stack>(mm[i]) = address(COL_OFFSET+i, ROW_OFFSET, 0x0000);
        }
        else{
          location<buffer>(mm[i].in[0]) =
          { address(COL_OFFSET-i, ROW_OFFSET-1, 0x5000), 
            address(COL_OFFSET-i, ROW_OFFSET-1, 0x7000)};
          location<stack>(mm[i]) = address(COL_OFFSET-i, ROW_OFFSET, 0x0000);
        }
      }

      for (int i=0; i<L9_NUM_KERNEL;i++){
        connect<window<L9_h1*L9_w1>>(in[i], mm[i].in[0]);
      }

      

      for (int i=0; i<L9_NUM_KERNEL;i++){
        if(i==L9_NUM_KERNEL-1){
          connect<stream>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[1]);
        }
      }
      

    };
    
};

#endif /**********__GRAPH_H__**********/