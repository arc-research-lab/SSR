#include "aie_graph_L7_B24_A0_C0.h"
#include "aie_graph_L7_B24_A0_C1.h"
#include "aie_graph_L7_B24_A0_C2.h"
#include "aie_graph_L7_B24_A0_C3.h"


using namespace adf;

const int L7_A=1;
const int L7_B=24;
const int L7_C=4;
const int L7_A_BRO=4;
const int L7_C_BRO=1;

class vit_x1_x24_x4_graph7 : public adf::graph {
public:
    
    input_port in_lhs_0[(L7_A*L7_C/L7_A_BRO)][L7_B];
    output_port out_0[L7_A*L7_C];
    

    
    simpleGraph_k0_L7_B24_A0_C0 <26, 4>  vit_0_0_0;
    simpleGraph_k0_L7_B24_A0_C1 <49, 5>  vit_0_1_0;
    simpleGraph_k0_L7_B24_A0_C2 <26, 6>  vit_0_2_0;
    simpleGraph_k0_L7_B24_A0_C3 <49, 7>  vit_0_3_0;
    
    

    vit_x1_x24_x4_graph7() {
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][0],vit_0_0_0.in[0]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][1],vit_0_0_0.in[1]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][2],vit_0_0_0.in[2]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][3],vit_0_0_0.in[3]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][4],vit_0_0_0.in[4]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][5],vit_0_0_0.in[5]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][6],vit_0_0_0.in[6]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][7],vit_0_0_0.in[7]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][8],vit_0_0_0.in[8]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][9],vit_0_0_0.in[9]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][10],vit_0_0_0.in[10]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][11],vit_0_0_0.in[11]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][12],vit_0_0_0.in[12]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][13],vit_0_0_0.in[13]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][14],vit_0_0_0.in[14]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][15],vit_0_0_0.in[15]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][16],vit_0_0_0.in[16]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][17],vit_0_0_0.in[17]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][18],vit_0_0_0.in[18]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][19],vit_0_0_0.in[19]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][20],vit_0_0_0.in[20]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][21],vit_0_0_0.in[21]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][22],vit_0_0_0.in[22]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][23],vit_0_0_0.in[23]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][0],vit_0_1_0.in[0]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][1],vit_0_1_0.in[1]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][2],vit_0_1_0.in[2]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][3],vit_0_1_0.in[3]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][4],vit_0_1_0.in[4]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][5],vit_0_1_0.in[5]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][6],vit_0_1_0.in[6]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][7],vit_0_1_0.in[7]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][8],vit_0_1_0.in[8]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][9],vit_0_1_0.in[9]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][10],vit_0_1_0.in[10]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][11],vit_0_1_0.in[11]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][12],vit_0_1_0.in[12]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][13],vit_0_1_0.in[13]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][14],vit_0_1_0.in[14]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][15],vit_0_1_0.in[15]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][16],vit_0_1_0.in[16]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][17],vit_0_1_0.in[17]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][18],vit_0_1_0.in[18]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][19],vit_0_1_0.in[19]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][20],vit_0_1_0.in[20]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][21],vit_0_1_0.in[21]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][22],vit_0_1_0.in[22]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][23],vit_0_1_0.in[23]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][0],vit_0_2_0.in[0]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][1],vit_0_2_0.in[1]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][2],vit_0_2_0.in[2]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][3],vit_0_2_0.in[3]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][4],vit_0_2_0.in[4]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][5],vit_0_2_0.in[5]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][6],vit_0_2_0.in[6]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][7],vit_0_2_0.in[7]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][8],vit_0_2_0.in[8]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][9],vit_0_2_0.in[9]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][10],vit_0_2_0.in[10]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][11],vit_0_2_0.in[11]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][12],vit_0_2_0.in[12]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][13],vit_0_2_0.in[13]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][14],vit_0_2_0.in[14]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][15],vit_0_2_0.in[15]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][16],vit_0_2_0.in[16]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][17],vit_0_2_0.in[17]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][18],vit_0_2_0.in[18]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][19],vit_0_2_0.in[19]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][20],vit_0_2_0.in[20]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][21],vit_0_2_0.in[21]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][22],vit_0_2_0.in[22]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][23],vit_0_2_0.in[23]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][0],vit_0_3_0.in[0]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][1],vit_0_3_0.in[1]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][2],vit_0_3_0.in[2]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][3],vit_0_3_0.in[3]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][4],vit_0_3_0.in[4]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][5],vit_0_3_0.in[5]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][6],vit_0_3_0.in[6]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][7],vit_0_3_0.in[7]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][8],vit_0_3_0.in[8]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][9],vit_0_3_0.in[9]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][10],vit_0_3_0.in[10]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][11],vit_0_3_0.in[11]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][12],vit_0_3_0.in[12]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][13],vit_0_3_0.in[13]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][14],vit_0_3_0.in[14]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][15],vit_0_3_0.in[15]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][16],vit_0_3_0.in[16]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][17],vit_0_3_0.in[17]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][18],vit_0_3_0.in[18]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][19],vit_0_3_0.in[19]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][20],vit_0_3_0.in[20]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][21],vit_0_3_0.in[21]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][22],vit_0_3_0.in[22]);
        connect<stream,window<L7_h1*L7_w1>>(in_lhs_0[0][23],vit_0_3_0.in[23]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_0_2_0.out,out_0[2]);
        connect<stream,stream>(vit_0_3_0.out,out_0[3]);
        
        
    }
};