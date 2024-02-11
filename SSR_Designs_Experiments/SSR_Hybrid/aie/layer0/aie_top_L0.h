#include "aie_graph_L0_B4_A0_C0.h"
#include "aie_graph_L0_B4_A0_C1.h"
#include "aie_graph_L0_B4_A0_C2.h"
#include "aie_graph_L0_B4_A0_C3.h"
#include "aie_graph_L0_B4_A0_C4.h"
#include "aie_graph_L0_B4_A0_C5.h"
#include "aie_graph_L0_B4_A0_C6.h"
#include "aie_graph_L0_B4_A0_C7.h"
#include "aie_graph_L0_B4_A0_C8.h"
#include "aie_graph_L0_B4_A0_C9.h"
#include "aie_graph_L0_B4_A0_C10.h"
#include "aie_graph_L0_B4_A0_C11.h"
#include "aie_graph_L0_B4_A0_C12.h"
#include "aie_graph_L0_B4_A0_C13.h"
#include "aie_graph_L0_B4_A0_C14.h"
#include "aie_graph_L0_B4_A0_C15.h"
#include "aie_graph_L0_B4_A0_C16.h"
#include "aie_graph_L0_B4_A0_C17.h"


using namespace adf;

const int L0_A=1;
const int L0_B=4;
const int L0_C=18;
const int L0_A_BRO=18;
const int L0_C_BRO=1;

class vit_x1_x4_x18_graph0 : public adf::graph {
public:
    
    input_port in_lhs_0[(L0_A*L0_C/L0_A_BRO)][L0_B];
    output_port out_0[L0_A*L0_C];
    

    
    simpleGraph_k0_L0_B4_A0_C0 <6, 0>  vit_0_0_0;
    simpleGraph_k0_L0_B4_A0_C1 <9, 1>  vit_0_1_0;
    simpleGraph_k0_L0_B4_A0_C2 <6, 2>  vit_0_2_0;
    simpleGraph_k0_L0_B4_A0_C3 <9, 3>  vit_0_3_0;
    simpleGraph_k0_L0_B4_A0_C4 <6, 4>  vit_0_4_0;
    simpleGraph_k0_L0_B4_A0_C5 <9, 5>  vit_0_5_0;
    simpleGraph_k0_L0_B4_A0_C6 <10, 0>  vit_0_6_0;
    simpleGraph_k0_L0_B4_A0_C7 <13, 1>  vit_0_7_0;
    simpleGraph_k0_L0_B4_A0_C8 <10, 2>  vit_0_8_0;
    simpleGraph_k0_L0_B4_A0_C9 <13, 3>  vit_0_9_0;
    simpleGraph_k0_L0_B4_A0_C10 <10, 4>  vit_0_10_0;
    simpleGraph_k0_L0_B4_A0_C11 <13, 5>  vit_0_11_0;
    simpleGraph_k0_L0_B4_A0_C12 <14, 0>  vit_0_12_0;
    simpleGraph_k0_L0_B4_A0_C13 <17, 1>  vit_0_13_0;
    simpleGraph_k0_L0_B4_A0_C14 <14, 2>  vit_0_14_0;
    simpleGraph_k0_L0_B4_A0_C15 <17, 3>  vit_0_15_0;
    simpleGraph_k0_L0_B4_A0_C16 <14, 4>  vit_0_16_0;
    simpleGraph_k0_L0_B4_A0_C17 <17, 5>  vit_0_17_0;
    
    

    vit_x1_x4_x18_graph0() {
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_0_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_0_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_1_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_1_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_2_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_2_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_3_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_3_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_4_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_4_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_4_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_4_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_5_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_5_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_5_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_5_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_6_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_6_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_6_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_6_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_7_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_7_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_7_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_7_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_8_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_8_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_8_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_8_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_9_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_9_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_9_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_9_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_10_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_10_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_10_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_10_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_11_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_11_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_11_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_11_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_12_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_12_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_12_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_12_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_13_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_13_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_13_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_13_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_14_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_14_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_14_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_14_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_15_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_15_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_15_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_15_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_16_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_16_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_16_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_16_0.in[3]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][0],vit_0_17_0.in[0]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][1],vit_0_17_0.in[1]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][2],vit_0_17_0.in[2]);
        connect<stream,window<L0_h1*L0_w1>>(in_lhs_0[0][3],vit_0_17_0.in[3]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        connect<stream,stream>(vit_0_4_0.out,out_0[4]);
        connect<stream,stream>(vit_0_5_0.out,out_0[5]);
        connect<stream,stream>(vit_0_6_0.out,out_0[6]);
        connect<stream,stream>(vit_0_7_0.out,out_0[7]);
        connect<stream,stream>(vit_0_8_0.out,out_0[8]);
        connect<stream,stream>(vit_0_9_0.out,out_0[9]);
        connect<stream,stream>(vit_0_10_0.out,out_0[10]);
        connect<stream,stream>(vit_0_11_0.out,out_0[11]);
        connect<stream,stream>(vit_0_12_0.out,out_0[12]);
        connect<stream,stream>(vit_0_13_0.out,out_0[13]);
        connect<stream,stream>(vit_0_14_0.out,out_0[14]);
        connect<stream,stream>(vit_0_15_0.out,out_0[15]);
        connect<stream,stream>(vit_0_16_0.out,out_0[16]);
        connect<stream,stream>(vit_0_17_0.out,out_0[17]);
        
        
    }
};