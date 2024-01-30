#include "aie_graph_L9_B2_A0_C0.h"
#include "aie_graph_L9_B2_A0_C1.h"
#include "aie_graph_L9_B2_A0_C2.h"
#include "aie_graph_L9_B2_A0_C3.h"
#include "aie_graph_L9_B2_A0_C4.h"


using namespace adf;

const int L9_A=1;
const int L9_B=2;
const int L9_C=5;
const int L9_A_BRO=5;
const int L9_C_BRO=1;

class vit_x1_x2_x5_graph9 : public adf::graph {
public:
    
    input_port in_lhs_0[(L9_A*L9_C/L9_A_BRO)][L9_B];
    output_port out_0[L9_A*L9_C];
    

    
    simpleGraph_k7_L9_B2_A0_C0 <24, 0>  vit_0_0_0;
    simpleGraph_k7_L9_B2_A0_C1 <25, 1>  vit_0_1_0;
    simpleGraph_k7_L9_B2_A0_C2 <24, 2>  vit_0_2_0;
    simpleGraph_k7_L9_B2_A0_C3 <25, 3>  vit_0_3_0;
    simpleGraph_k7_L9_B2_A0_C4 <24, 4>  vit_0_4_0;
    
    

    vit_x1_x2_x5_graph9() {
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][0],vit_0_4_0.in[0]);
        connect<stream,window<L9_h1*L9_w1>>(in_lhs_0[0][1],vit_0_4_0.in[1]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        connect<stream,stream>(vit_0_4_0.out,out_0[4]);
        
        
    }
};