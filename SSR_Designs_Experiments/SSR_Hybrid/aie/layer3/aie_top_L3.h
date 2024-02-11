#include "aie_graph_L3_B6_A0_C0.h"
#include "aie_graph_L3_B6_A0_C1.h"
#include "aie_graph_L3_B6_A0_C2.h"
#include "aie_graph_L3_B6_A0_C3.h"


using namespace adf;

const int L3_A=1;
const int L3_B=6;
const int L3_C=4;
const int L3_A_BRO=4;
const int L3_C_BRO=1;

class vit_x1_x6_x4_graph3 : public adf::graph {
public:
    
    input_port in_lhs_0[(L3_A*L3_C/L3_A_BRO)][L3_B];
    output_port out_0[L3_A*L3_C];
    

    
    simpleGraph_k3_L3_B6_A0_C0 <6, 6>  vit_0_0_0;
    simpleGraph_k3_L3_B6_A0_C1 <11, 7>  vit_0_1_0;
    simpleGraph_k3_L3_B6_A0_C2 <12, 6>  vit_0_2_0;
    simpleGraph_k3_L3_B6_A0_C3 <17, 7>  vit_0_3_0;
    
    

    vit_x1_x6_x4_graph3() {
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][2],vit_0_0_0.in[2]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][3],vit_0_0_0.in[3]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][4],vit_0_0_0.in[4]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][5],vit_0_0_0.in[5]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][2],vit_0_1_0.in[2]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][3],vit_0_1_0.in[3]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][4],vit_0_1_0.in[4]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][5],vit_0_1_0.in[5]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][2],vit_0_2_0.in[2]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][3],vit_0_2_0.in[3]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][4],vit_0_2_0.in[4]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][5],vit_0_2_0.in[5]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][2],vit_0_3_0.in[2]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][3],vit_0_3_0.in[3]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][4],vit_0_3_0.in[4]);
        connect<stream,window<L3_KH*L3_KW*L3_SEQ>>(in_lhs_0[0][5],vit_0_3_0.in[5]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        
        
    }
};