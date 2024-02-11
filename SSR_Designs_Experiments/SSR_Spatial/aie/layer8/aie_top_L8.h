#include "aie_graph_L8_B12_A0_C0.h"
#include "aie_graph_L8_B12_A0_C1.h"
#include "aie_graph_L8_B12_A0_C2.h"
#include "aie_graph_L8_B12_A0_C3.h"


using namespace adf;

const int L8_A=1;
const int L8_B=12;
const int L8_C=4;
const int L8_A_BRO=4;
const int L8_C_BRO=1;

class vit_x1_x12_x4_graph8 : public adf::graph {
public:
    
    input_port in_lhs_0[(L8_A*L8_C/L8_A_BRO)][L8_B];
    output_port out_0[L8_A*L8_C];
    

    
    simpleGraph_k3_L8_B12_A0_C0 <32, 0>  vit_0_0_0;
    simpleGraph_k3_L8_B12_A0_C1 <43, 1>  vit_0_1_0;
    simpleGraph_k3_L8_B12_A0_C2 <32, 2>  vit_0_2_0;
    simpleGraph_k3_L8_B12_A0_C3 <43, 3>  vit_0_3_0;
    
    

    vit_x1_x12_x4_graph8() {
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][2],vit_0_0_0.in[2]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][3],vit_0_0_0.in[3]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][4],vit_0_0_0.in[4]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][5],vit_0_0_0.in[5]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][6],vit_0_0_0.in[6]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][7],vit_0_0_0.in[7]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][8],vit_0_0_0.in[8]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][9],vit_0_0_0.in[9]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][10],vit_0_0_0.in[10]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][11],vit_0_0_0.in[11]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][2],vit_0_1_0.in[2]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][3],vit_0_1_0.in[3]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][4],vit_0_1_0.in[4]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][5],vit_0_1_0.in[5]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][6],vit_0_1_0.in[6]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][7],vit_0_1_0.in[7]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][8],vit_0_1_0.in[8]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][9],vit_0_1_0.in[9]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][10],vit_0_1_0.in[10]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][11],vit_0_1_0.in[11]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][2],vit_0_2_0.in[2]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][3],vit_0_2_0.in[3]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][4],vit_0_2_0.in[4]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][5],vit_0_2_0.in[5]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][6],vit_0_2_0.in[6]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][7],vit_0_2_0.in[7]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][8],vit_0_2_0.in[8]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][9],vit_0_2_0.in[9]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][10],vit_0_2_0.in[10]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][11],vit_0_2_0.in[11]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][2],vit_0_3_0.in[2]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][3],vit_0_3_0.in[3]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][4],vit_0_3_0.in[4]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][5],vit_0_3_0.in[5]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][6],vit_0_3_0.in[6]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][7],vit_0_3_0.in[7]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][8],vit_0_3_0.in[8]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][9],vit_0_3_0.in[9]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][10],vit_0_3_0.in[10]);
        connect<stream,window<L8_KH*L8_KW*L8_SEQ>>(in_lhs_0[0][11],vit_0_3_0.in[11]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        
        
    }
};