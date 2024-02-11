#include "aie_graph_L0_B4_A0_C0.h"
#include "aie_graph_L0_B4_A0_C1.h"
#include "aie_graph_L0_B4_A0_C2.h"
#include "aie_graph_L0_B4_A0_C3.h"
#include "aie_graph_L0_B4_A0_C4.h"
#include "aie_graph_L0_B4_A0_C5.h"


using namespace adf;

const int L0_A=1;
const int L0_B=4;
const int L0_C=6;
const int L0_A_BRO=6;
const int L0_C_BRO=1;

class vit_x1_x4_x6_graph0 : public adf::graph {
public:
    
    input_port in_lhs_0[(L0_A*L0_C/L0_A_BRO)][L0_B];
    output_port out_0[L0_A*L0_C];
    

    
    simpleGraph_k0_L0_B4_A0_C0 <12, 2>  vit_0_0_0;
    simpleGraph_k0_L0_B4_A0_C1 <15, 3>  vit_0_1_0;
    simpleGraph_k0_L0_B4_A0_C2 <12, 4>  vit_0_2_0;
    simpleGraph_k0_L0_B4_A0_C3 <15, 5>  vit_0_3_0;
    simpleGraph_k0_L0_B4_A0_C4 <12, 6>  vit_0_4_0;
    simpleGraph_k0_L0_B4_A0_C5 <15, 7>  vit_0_5_0;
    
    

    vit_x1_x4_x6_graph0() {
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
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        connect<stream,stream>(vit_0_4_0.out,out_0[4]);
        connect<stream,stream>(vit_0_5_0.out,out_0[5]);
        
        
    }
};