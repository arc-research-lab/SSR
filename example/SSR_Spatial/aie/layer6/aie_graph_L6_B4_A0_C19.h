#ifndef __GRAPH_L6_B4_A0_C19_H__
#define __GRAPH_L6_B4_A0_C19_H__

#include <adf.h>
#include "para_L6.h"
#include <stdio.h>
using namespace adf;


template <int COL_OFFSET, int ROW_OFFSET>
class simpleGraph_k0_L6_B4_A0_C19: public graph {
  private:
    kernel mm[L6_NUM_KERNEL];

  public:
    port<input> in[L6_NUM_KERNEL];
    port<input> weights_offset;
    port<output> out;
    
    simpleGraph_k0_L6_B4_A0_C19() {
      
      mm[0] = kernel::create(mm0_kernel0_L6_B0_A0_C19);
      source(mm[0]) = "mm0_kernel0_L6_B0_A0_C19.cc";
      mm[1] = kernel::create(mm0_kernel1_L6_B1_A0_C19);
      source(mm[1]) = "mm0_kernel1_L6_B1_A0_C19.cc";
      mm[2] = kernel::create(mm0_kernel1_L6_B2_A0_C19);
      source(mm[2]) = "mm0_kernel1_L6_B2_A0_C19.cc";
      mm[3] = kernel::create(mm0_kernel2_L6_B3_A0_C19);
      source(mm[3]) = "mm0_kernel2_L6_B3_A0_C19.cc";

      for (int i=0; i<L6_NUM_KERNEL;i++){
        runtime<ratio>(mm[i]) = 0.9;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L6_NUM_KERNEL;i++){
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


      for (int i=0; i<L6_NUM_KERNEL;i++){
        connect<window<L6_h1*L6_w1>>(in[i], mm[i].in[0]);
      }

      connect<parameter>(weights_offset, async(mm[0].in[1]));

      for (int i=0; i<L6_NUM_KERNEL;i++){
        if(i==L6_NUM_KERNEL-1){
          connect<stream>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[1]);
        }
      }
      

    };
    
};

#endif /**********__GRAPH_H__**********/