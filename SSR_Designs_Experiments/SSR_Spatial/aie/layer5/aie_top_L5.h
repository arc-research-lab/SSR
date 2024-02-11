#include "aie_graph_L5_B6_A0_C0.h"
#include "aie_graph_L5_B6_A0_C1.h"
#include "aie_graph_L5_B6_A0_C2.h"
#include "aie_graph_L5_B6_A0_C3.h"


using namespace adf;

const int L5_A=1;
const int L5_B=6;
const int L5_C=4;
const int L5_A_BRO=4;
const int L5_C_BRO=1;

class vit_x1_x6_x4_graph5 : public adf::graph {
public:
    
    input_port in_lhs_0[(L5_A*L5_C/L5_A_BRO)][L5_B];
    output_port out_0[L5_A*L5_C];
    

    
    simpleGraph_k0_L5_B6_A0_C0 <44, 0>  vit_0_0_0;
    simpleGraph_k0_L5_B6_A0_C1 <49, 1>  vit_0_1_0;
    simpleGraph_k0_L5_B6_A0_C2 <44, 2>  vit_0_2_0;
    simpleGraph_k0_L5_B6_A0_C3 <49, 3>  vit_0_3_0;
    
    

    vit_x1_x6_x4_graph5() {
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][2],vit_0_0_0.in[2]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][3],vit_0_0_0.in[3]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][4],vit_0_0_0.in[4]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][5],vit_0_0_0.in[5]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][2],vit_0_1_0.in[2]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][3],vit_0_1_0.in[3]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][4],vit_0_1_0.in[4]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][5],vit_0_1_0.in[5]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][2],vit_0_2_0.in[2]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][3],vit_0_2_0.in[3]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][4],vit_0_2_0.in[4]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][5],vit_0_2_0.in[5]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][2],vit_0_3_0.in[2]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][3],vit_0_3_0.in[3]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][4],vit_0_3_0.in[4]);
        connect<stream,window<L5_h1*L5_w1>>(in_lhs_0[0][5],vit_0_3_0.in[5]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        
        
    }
};