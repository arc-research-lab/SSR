#include "aie_graph_L2_B2_A0_C0.h"
#include "aie_graph_L2_B2_A0_C1.h"
#include "aie_graph_L2_B2_A0_C2.h"
#include "aie_graph_L2_B2_A0_C3.h"
#include "aie_graph_L2_B2_A0_C4.h"


using namespace adf;

const int L2_A=1;
const int L2_B=2;
const int L2_C=5;
const int L2_A_BRO=5;
const int L2_C_BRO=1;

class vit_x1_x2_x5_graph2 : public adf::graph {
public:
    
    input_port in_lhs_0[(L2_A*L2_C/L2_A_BRO)][L2_B];
    output_port out_0[L2_A*L2_C];
    

    
    simpleGraph_k7_L2_B2_A0_C0 <43, 0>  vit_0_0_0;
    simpleGraph_k7_L2_B2_A0_C1 <44, 1>  vit_0_1_0;
    simpleGraph_k7_L2_B2_A0_C2 <45, 0>  vit_0_2_0;
    simpleGraph_k7_L2_B2_A0_C3 <46, 1>  vit_0_3_0;
    simpleGraph_k7_L2_B2_A0_C4 <47, 0>  vit_0_4_0;
    
    

    vit_x1_x2_x5_graph2() {
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][0],vit_0_4_0.in[0]);
        connect<stream,window<L2_h1*L2_w1>>(in_lhs_0[0][1],vit_0_4_0.in[1]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        connect<stream,stream>(vit_0_4_0.out,out_0[4]);
        
        
    }
};