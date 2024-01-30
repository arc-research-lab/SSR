#ifndef __GRAPH_L5_B6_A0_C0_H__
#define __GRAPH_L5_B6_A0_C0_H__

#include <adf.h>
#include "para_L5.h"
#include <stdio.h>
using namespace adf;


template <int COL_OFFSET, int ROW_OFFSET>
class simpleGraph_k0_L5_B6_A0_C0: public graph {
  private:
    kernel mm[L5_NUM_KERNEL];

  public:
    port<input> in[L5_NUM_KERNEL];
    port<input> weights_offset;
    port<output> out;
    
    simpleGraph_k0_L5_B6_A0_C0() {
      
      mm[0] = kernel::create(mm0_kernel0_L5_B0_A0_C0);
      source(mm[0]) = "mm0_kernel0_L5_B0_A0_C0.cc";
      mm[1] = kernel::create(mm0_kernel1_L5_B1_A0_C0);
      source(mm[1]) = "mm0_kernel1_L5_B1_A0_C0.cc";
      mm[2] = kernel::create(mm0_kernel1_L5_B2_A0_C0);
      source(mm[2]) = "mm0_kernel1_L5_B2_A0_C0.cc";
      mm[3] = kernel::create(mm0_kernel1_L5_B3_A0_C0);
      source(mm[3]) = "mm0_kernel1_L5_B3_A0_C0.cc";
      mm[4] = kernel::create(mm0_kernel1_L5_B4_A0_C0);
      source(mm[4]) = "mm0_kernel1_L5_B4_A0_C0.cc";
      mm[5] = kernel::create(mm0_kernel2_L5_B5_A0_C0);
      source(mm[5]) = "mm0_kernel2_L5_B5_A0_C0.cc";

      for (int i=0; i<L5_NUM_KERNEL;i++){
        runtime<ratio>(mm[i]) = 0.9;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L5_NUM_KERNEL;i++){
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


      for (int i=0; i<L5_NUM_KERNEL;i++){
        connect<window<L5_h1*L5_w1>>(in[i], mm[i].in[0]);
      }

      connect<parameter>(weights_offset, async(mm[0].in[1]));

      for (int i=0; i<L5_NUM_KERNEL;i++){
        if(i==L5_NUM_KERNEL-1){
          connect<stream>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[1]);
        }
      }
      

    };
    
};

#endif /**********__GRAPH_H__**********/