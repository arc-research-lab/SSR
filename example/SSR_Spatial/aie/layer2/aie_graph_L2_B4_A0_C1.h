#ifndef __GRAPH_L2_B4_A0_C1_H__
#define __GRAPH_L2_B4_A0_C1_H__

#include <adf.h>
#include "para_L2.h"
#include <stdio.h>
using namespace adf;


template <int COL_OFFSET, int ROW_OFFSET>
class simpleGraph_k0_L2_B4_A0_C1: public graph {
  private:
    kernel mm[L2_NUM_KERNEL];

  public:
    port<input> in[L2_NUM_KERNEL];
    port<input> weights_offset;
    port<output> out;
    
    simpleGraph_k0_L2_B4_A0_C1() {
      
      mm[0] = kernel::create(mm0_kernel0_L2_B0_A0_C1);
      source(mm[0]) = "mm0_kernel0_L2_B0_A0_C1.cc";
      mm[1] = kernel::create(mm0_kernel1_L2_B1_A0_C1);
      source(mm[1]) = "mm0_kernel1_L2_B1_A0_C1.cc";
      mm[2] = kernel::create(mm0_kernel1_L2_B2_A0_C1);
      source(mm[2]) = "mm0_kernel1_L2_B2_A0_C1.cc";
      mm[3] = kernel::create(mm0_kernel2_L2_B3_A0_C1);
      source(mm[3]) = "mm0_kernel2_L2_B3_A0_C1.cc";

      for (int i=0; i<L2_NUM_KERNEL;i++){
        runtime<ratio>(mm[i]) = 0.9;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L2_NUM_KERNEL;i++){
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


      for (int i=0; i<L2_NUM_KERNEL;i++){
        connect<window<L2_h1*L2_w1>>(in[i], mm[i].in[0]);
      }

      connect<parameter>(weights_offset, async(mm[0].in[1]));

      for (int i=0; i<L2_NUM_KERNEL;i++){
        if(i==L2_NUM_KERNEL-1){
          connect<stream>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[1]);
        }
      }
      

    };
    
};

#endif /**********__GRAPH_H__**********/