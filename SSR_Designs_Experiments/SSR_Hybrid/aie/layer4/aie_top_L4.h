#include "aie_graph_L4_B2_A0_C0.h"
#include "aie_graph_L4_B2_A0_C1.h"
#include "aie_graph_L4_B2_A0_C2.h"
#include "aie_graph_L4_B2_A0_C3.h"
#include "aie_graph_L4_B2_A0_C4.h"


using namespace adf;

const int L4_A=1;
const int L4_B=2;
const int L4_C=5;
const int L4_A_BRO=5;
const int L4_C_BRO=1;

class vit_x1_x2_x5_graph4 : public adf::graph {
public:
    
    input_port in_lhs_0[(L4_A*L4_C/L4_A_BRO)][L4_B];
    output_port out_0[L4_A*L4_C];
    

    
    simpleGraph_k7_L4_B2_A0_C0 <43, 0>  vit_0_0_0;
    simpleGraph_k7_L4_B2_A0_C1 <44, 1>  vit_0_1_0;
    simpleGraph_k7_L4_B2_A0_C2 <43, 2>  vit_0_2_0;
    simpleGraph_k7_L4_B2_A0_C3 <44, 3>  vit_0_3_0;
    simpleGraph_k7_L4_B2_A0_C4 <43, 4>  vit_0_4_0;
    
    

    vit_x1_x2_x5_graph4() {
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][0],vit_0_4_0.in[0]);
        connect<stream,window<L4_h1*L4_w1>>(in_lhs_0[0][1],vit_0_4_0.in[1]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        connect<stream,stream>(vit_0_4_0.out,out_0[4]);
        
        
    }
};