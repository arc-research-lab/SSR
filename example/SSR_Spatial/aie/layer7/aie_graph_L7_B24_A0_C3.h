#ifndef __GRAPH_L7_B24_A0_C3_H__
#define __GRAPH_L7_B24_A0_C3_H__

#include <adf.h>
#include "para_L7.h"
#include <stdio.h>
using namespace adf;


template <int COL_OFFSET, int ROW_OFFSET>
class simpleGraph_k0_L7_B24_A0_C3: public graph {
  private:
    kernel mm[L7_NUM_KERNEL];

  public:
    port<input> in[L7_NUM_KERNEL];
    port<input> weights_offset;
    port<output> out;
    
    simpleGraph_k0_L7_B24_A0_C3() {
      
      mm[0] = kernel::create(mm0_kernel0_L7_B0_A0_C3);
      source(mm[0]) = "mm0_kernel0_L7_B0_A0_C3.cc";
      mm[1] = kernel::create(mm0_kernel1_L7_B1_A0_C3);
      source(mm[1]) = "mm0_kernel1_L7_B1_A0_C3.cc";
      mm[2] = kernel::create(mm0_kernel1_L7_B2_A0_C3);
      source(mm[2]) = "mm0_kernel1_L7_B2_A0_C3.cc";
      mm[3] = kernel::create(mm0_kernel1_L7_B3_A0_C3);
      source(mm[3]) = "mm0_kernel1_L7_B3_A0_C3.cc";
      mm[4] = kernel::create(mm0_kernel1_L7_B4_A0_C3);
      source(mm[4]) = "mm0_kernel1_L7_B4_A0_C3.cc";
      mm[5] = kernel::create(mm0_kernel1_L7_B5_A0_C3);
      source(mm[5]) = "mm0_kernel1_L7_B5_A0_C3.cc";
      mm[6] = kernel::create(mm0_kernel1_L7_B6_A0_C3);
      source(mm[6]) = "mm0_kernel1_L7_B6_A0_C3.cc";
      mm[7] = kernel::create(mm0_kernel1_L7_B7_A0_C3);
      source(mm[7]) = "mm0_kernel1_L7_B7_A0_C3.cc";
      mm[8] = kernel::create(mm0_kernel1_L7_B8_A0_C3);
      source(mm[8]) = "mm0_kernel1_L7_B8_A0_C3.cc";
      mm[9] = kernel::create(mm0_kernel1_L7_B9_A0_C3);
      source(mm[9]) = "mm0_kernel1_L7_B9_A0_C3.cc";
      mm[10] = kernel::create(mm0_kernel1_L7_B10_A0_C3);
      source(mm[10]) = "mm0_kernel1_L7_B10_A0_C3.cc";
      mm[11] = kernel::create(mm0_kernel1_L7_B11_A0_C3);
      source(mm[11]) = "mm0_kernel1_L7_B11_A0_C3.cc";
      mm[12] = kernel::create(mm0_kernel1_L7_B12_A0_C3);
      source(mm[12]) = "mm0_kernel1_L7_B12_A0_C3.cc";
      mm[13] = kernel::create(mm0_kernel1_L7_B13_A0_C3);
      source(mm[13]) = "mm0_kernel1_L7_B13_A0_C3.cc";
      mm[14] = kernel::create(mm0_kernel1_L7_B14_A0_C3);
      source(mm[14]) = "mm0_kernel1_L7_B14_A0_C3.cc";
      mm[15] = kernel::create(mm0_kernel1_L7_B15_A0_C3);
      source(mm[15]) = "mm0_kernel1_L7_B15_A0_C3.cc";
      mm[16] = kernel::create(mm0_kernel1_L7_B16_A0_C3);
      source(mm[16]) = "mm0_kernel1_L7_B16_A0_C3.cc";
      mm[17] = kernel::create(mm0_kernel1_L7_B17_A0_C3);
      source(mm[17]) = "mm0_kernel1_L7_B17_A0_C3.cc";
      mm[18] = kernel::create(mm0_kernel1_L7_B18_A0_C3);
      source(mm[18]) = "mm0_kernel1_L7_B18_A0_C3.cc";
      mm[19] = kernel::create(mm0_kernel1_L7_B19_A0_C3);
      source(mm[19]) = "mm0_kernel1_L7_B19_A0_C3.cc";
      mm[20] = kernel::create(mm0_kernel1_L7_B20_A0_C3);
      source(mm[20]) = "mm0_kernel1_L7_B20_A0_C3.cc";
      mm[21] = kernel::create(mm0_kernel1_L7_B21_A0_C3);
      source(mm[21]) = "mm0_kernel1_L7_B21_A0_C3.cc";
      mm[22] = kernel::create(mm0_kernel1_L7_B22_A0_C3);
      source(mm[22]) = "mm0_kernel1_L7_B22_A0_C3.cc";
      mm[23] = kernel::create(mm0_kernel2_L7_B23_A0_C3);
      source(mm[23]) = "mm0_kernel2_L7_B23_A0_C3.cc";

      for (int i=0; i<L7_NUM_KERNEL;i++){
        runtime<ratio>(mm[i]) = 0.9;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L7_NUM_KERNEL;i++){
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


      for (int i=0; i<L7_NUM_KERNEL;i++){
        connect<window<L7_h1*L7_w1>>(in[i], mm[i].in[0]);
      }

      connect<parameter>(weights_offset, async(mm[0].in[1]));

      for (int i=0; i<L7_NUM_KERNEL;i++){
        if(i==L7_NUM_KERNEL-1){
          connect<stream>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[1]);
        }
      }
      

    };
    
};

#endif /**********__GRAPH_H__**********/