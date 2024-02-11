#include "aie_graph_L6_B4_A0_C0.h"
#include "aie_graph_L6_B4_A0_C1.h"
#include "aie_graph_L6_B4_A0_C2.h"
#include "aie_graph_L6_B4_A0_C3.h"
#include "aie_graph_L6_B4_A0_C4.h"
#include "aie_graph_L6_B4_A0_C5.h"
#include "aie_graph_L6_B4_A0_C6.h"
#include "aie_graph_L6_B4_A0_C7.h"
#include "aie_graph_L6_B4_A0_C8.h"
#include "aie_graph_L6_B4_A0_C9.h"
#include "aie_graph_L6_B4_A0_C10.h"
#include "aie_graph_L6_B4_A0_C11.h"
#include "aie_graph_L6_B4_A0_C12.h"
#include "aie_graph_L6_B4_A0_C13.h"
#include "aie_graph_L6_B4_A0_C14.h"
#include "aie_graph_L6_B4_A0_C15.h"
#include "aie_graph_L6_B4_A0_C16.h"
#include "aie_graph_L6_B4_A0_C17.h"
#include "aie_graph_L6_B4_A0_C18.h"
#include "aie_graph_L6_B4_A0_C19.h"
#include "aie_graph_L6_B4_A0_C20.h"
#include "aie_graph_L6_B4_A0_C21.h"
#include "aie_graph_L6_B4_A0_C22.h"
#include "aie_graph_L6_B4_A0_C23.h"


using namespace adf;

const int L6_A=1;
const int L6_B=4;
const int L6_C=24;
const int L6_A_BRO=8;
const int L6_C_BRO=1;

class vit_x1_x4_x24_graph6 : public adf::graph {
public:
    
    input_port in_lhs_0[(L6_A*L6_C/L6_A_BRO)][L6_B];
    output_port out_0[L6_A*L6_C];
    

    
    simpleGraph_k0_L6_B4_A0_C0 <0, 0>  vit_0_0_0;
    simpleGraph_k0_L6_B4_A0_C1 <3, 1>  vit_0_1_0;
    simpleGraph_k0_L6_B4_A0_C2 <0, 2>  vit_0_2_0;
    simpleGraph_k0_L6_B4_A0_C3 <3, 3>  vit_0_3_0;
    simpleGraph_k0_L6_B4_A0_C4 <0, 4>  vit_0_4_0;
    simpleGraph_k0_L6_B4_A0_C5 <3, 5>  vit_0_5_0;
    simpleGraph_k0_L6_B4_A0_C6 <0, 6>  vit_0_6_0;
    simpleGraph_k0_L6_B4_A0_C7 <3, 7>  vit_0_7_0;
    simpleGraph_k0_L6_B4_A0_C8 <4, 0>  vit_0_8_0;
    simpleGraph_k0_L6_B4_A0_C9 <7, 1>  vit_0_9_0;
    simpleGraph_k0_L6_B4_A0_C10 <4, 2>  vit_0_10_0;
    simpleGraph_k0_L6_B4_A0_C11 <7, 3>  vit_0_11_0;
    simpleGraph_k0_L6_B4_A0_C12 <4, 4>  vit_0_12_0;
    simpleGraph_k0_L6_B4_A0_C13 <7, 5>  vit_0_13_0;
    simpleGraph_k0_L6_B4_A0_C14 <4, 6>  vit_0_14_0;
    simpleGraph_k0_L6_B4_A0_C15 <7, 7>  vit_0_15_0;
    simpleGraph_k0_L6_B4_A0_C16 <8, 0>  vit_0_16_0;
    simpleGraph_k0_L6_B4_A0_C17 <11, 1>  vit_0_17_0;
    simpleGraph_k0_L6_B4_A0_C18 <8, 2>  vit_0_18_0;
    simpleGraph_k0_L6_B4_A0_C19 <11, 3>  vit_0_19_0;
    simpleGraph_k0_L6_B4_A0_C20 <8, 4>  vit_0_20_0;
    simpleGraph_k0_L6_B4_A0_C21 <11, 5>  vit_0_21_0;
    simpleGraph_k0_L6_B4_A0_C22 <8, 6>  vit_0_22_0;
    simpleGraph_k0_L6_B4_A0_C23 <11, 7>  vit_0_23_0;
    
    

    vit_x1_x4_x24_graph6() {
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_0_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_0_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_1_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_1_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_2_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_2_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_3_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_3_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_4_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_4_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_4_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_4_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_5_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_5_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_5_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_5_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_6_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_6_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_6_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_6_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][0],vit_0_7_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][1],vit_0_7_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][2],vit_0_7_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[0][3],vit_0_7_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_8_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_8_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_8_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_8_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_9_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_9_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_9_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_9_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_10_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_10_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_10_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_10_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_11_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_11_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_11_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_11_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_12_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_12_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_12_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_12_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_13_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_13_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_13_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_13_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_14_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_14_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_14_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_14_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][0],vit_0_15_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][1],vit_0_15_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][2],vit_0_15_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[1][3],vit_0_15_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_16_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_16_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_16_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_16_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_17_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_17_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_17_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_17_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_18_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_18_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_18_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_18_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_19_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_19_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_19_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_19_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_20_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_20_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_20_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_20_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_21_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_21_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_21_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_21_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_22_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_22_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_22_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_22_0.in[3]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][0],vit_0_23_0.in[0]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][1],vit_0_23_0.in[1]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][2],vit_0_23_0.in[2]);
        connect<stream,window<L6_h1*L6_w1>>(in_lhs_0[2][3],vit_0_23_0.in[3]);
        
        

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
        connect<stream,stream>(vit_0_18_0.out,out_0[18]);
        connect<stream,stream>(vit_0_19_0.out,out_0[19]);
        connect<stream,stream>(vit_0_20_0.out,out_0[20]);
        connect<stream,stream>(vit_0_21_0.out,out_0[21]);
        connect<stream,stream>(vit_0_22_0.out,out_0[22]);
        connect<stream,stream>(vit_0_23_0.out,out_0[23]);
        
        
    }
};