#include "aie_graph_L1.h"

using namespace adf;

const int L1_A=4;
const int L1_B=2;
const int L1_C=2;
const int L1_A_BRO=2;
const int L1_C_BRO=4;

class vit_x4_x2_x2_graph1 : public adf::graph {
public:
    
    input_port in_lhs_0[(L1_A*L1_C/L1_A_BRO)][L1_B];
    input_port in_rhs_0[(L1_A*L1_C/L1_C_BRO)][L1_B];
	output_port out_0[L1_A*L1_C];
    
    input_port in_lhs_1[(L1_A*L1_C/L1_A_BRO)][L1_B];
    input_port in_rhs_1[(L1_A*L1_C/L1_C_BRO)][L1_B];
	output_port out_1[L1_A*L1_C];
    
    input_port in_lhs_2[(L1_A*L1_C/L1_A_BRO)][L1_B];
    input_port in_rhs_2[(L1_A*L1_C/L1_C_BRO)][L1_B];
	output_port out_2[L1_A*L1_C];
    

    simpleGraph_k1_B2_L1 <18, 0>  vit_0_0_0;
    simpleGraph_k1_B2_L1 <19, 1>  vit_0_1_0;
    simpleGraph_k1_B2_L1 <18, 2>  vit_1_0_0;
    simpleGraph_k1_B2_L1 <19, 3>  vit_1_1_0;
    simpleGraph_k1_B2_L1 <18, 4>  vit_2_0_0;
    simpleGraph_k1_B2_L1 <19, 5>  vit_2_1_0;
    simpleGraph_k1_B2_L1 <18, 6>  vit_3_0_0;
    simpleGraph_k1_B2_L1 <19, 7>  vit_3_1_0;
    
    simpleGraph_k1_B2_L1 <20, 0>  vit_0_0_1;
    simpleGraph_k1_B2_L1 <21, 1>  vit_0_1_1;
    simpleGraph_k1_B2_L1 <20, 2>  vit_1_0_1;
    simpleGraph_k1_B2_L1 <21, 3>  vit_1_1_1;
    simpleGraph_k1_B2_L1 <20, 4>  vit_2_0_1;
    simpleGraph_k1_B2_L1 <21, 5>  vit_2_1_1;
    simpleGraph_k1_B2_L1 <20, 6>  vit_3_0_1;
    simpleGraph_k1_B2_L1 <21, 7>  vit_3_1_1;
    
    simpleGraph_k1_B2_L1 <22, 0>  vit_0_0_2;
    simpleGraph_k1_B2_L1 <23, 1>  vit_0_1_2;
    simpleGraph_k1_B2_L1 <22, 2>  vit_1_0_2;
    simpleGraph_k1_B2_L1 <23, 3>  vit_1_1_2;
    simpleGraph_k1_B2_L1 <22, 4>  vit_2_0_2;
    simpleGraph_k1_B2_L1 <23, 5>  vit_2_1_2;
    simpleGraph_k1_B2_L1 <22, 6>  vit_3_0_2;
    simpleGraph_k1_B2_L1 <23, 7>  vit_3_1_2;
    
    

    vit_x4_x2_x2_graph1 () {

        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[0][0],vit_0_0_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[0][1],vit_0_0_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[0][0],vit_0_1_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[0][1],vit_0_1_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[1][0],vit_1_0_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[1][1],vit_1_0_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[1][0],vit_1_1_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[1][1],vit_1_1_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[2][0],vit_2_0_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[2][1],vit_2_0_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[2][0],vit_2_1_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[2][1],vit_2_1_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[3][0],vit_3_0_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[3][1],vit_3_0_0.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[3][0],vit_3_1_0.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_0[3][1],vit_3_1_0.in0[1]);
        
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[0][0],vit_0_0_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[0][1],vit_0_0_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[0][0],vit_0_1_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[0][1],vit_0_1_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[1][0],vit_1_0_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[1][1],vit_1_0_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[1][0],vit_1_1_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[1][1],vit_1_1_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[2][0],vit_2_0_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[2][1],vit_2_0_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[2][0],vit_2_1_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[2][1],vit_2_1_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[3][0],vit_3_0_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[3][1],vit_3_0_1.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[3][0],vit_3_1_1.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_1[3][1],vit_3_1_1.in0[1]);
        
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[0][0],vit_0_0_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[0][1],vit_0_0_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[0][0],vit_0_1_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[0][1],vit_0_1_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[1][0],vit_1_0_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[1][1],vit_1_0_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[1][0],vit_1_1_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[1][1],vit_1_1_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[2][0],vit_2_0_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[2][1],vit_2_0_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[2][0],vit_2_1_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[2][1],vit_2_1_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[3][0],vit_3_0_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[3][1],vit_3_0_2.in0[1]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[3][0],vit_3_1_2.in0[0]);
        connect<stream,window<L1_h1*L1_w1>>(in_lhs_2[3][1],vit_3_1_2.in0[1]);
        
        

        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][0],vit_0_0_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][1],vit_0_0_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][0],vit_1_0_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][1],vit_1_0_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][0],vit_2_0_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][1],vit_2_0_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][0],vit_3_0_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[0][1],vit_3_0_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][0],vit_0_1_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][1],vit_0_1_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][0],vit_1_1_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][1],vit_1_1_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][0],vit_2_1_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][1],vit_2_1_0.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][0],vit_3_1_0.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_0[1][1],vit_3_1_0.in1[1]);
        
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][0],vit_0_0_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][1],vit_0_0_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][0],vit_1_0_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][1],vit_1_0_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][0],vit_2_0_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][1],vit_2_0_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][0],vit_3_0_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[0][1],vit_3_0_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][0],vit_0_1_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][1],vit_0_1_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][0],vit_1_1_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][1],vit_1_1_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][0],vit_2_1_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][1],vit_2_1_1.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][0],vit_3_1_1.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_1[1][1],vit_3_1_1.in1[1]);
        
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][0],vit_0_0_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][1],vit_0_0_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][0],vit_1_0_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][1],vit_1_0_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][0],vit_2_0_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][1],vit_2_0_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][0],vit_3_0_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[0][1],vit_3_0_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][0],vit_0_1_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][1],vit_0_1_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][0],vit_1_1_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][1],vit_1_1_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][0],vit_2_1_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][1],vit_2_1_2.in1[1]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][0],vit_3_1_2.in1[0]);
        connect<stream,window<L1_w1*L1_w2>>(in_rhs_2[1][1],vit_3_1_2.in1[1]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_1_0_0.out,out_0[2]);
        connect<stream,stream>(vit_1_1_0.out,out_0[3]);
        connect<stream,stream>(vit_2_0_0.out,out_0[4]);
        connect<stream,stream>(vit_2_1_0.out,out_0[5]);
        connect<stream,stream>(vit_3_0_0.out,out_0[6]);
        connect<stream,stream>(vit_3_1_0.out,out_0[7]);
        
        connect<stream,stream>(vit_0_0_1.out,out_1[0]);
        connect<stream,stream>(vit_0_1_1.out,out_1[1]);
        connect<stream,stream>(vit_1_0_1.out,out_1[2]);
        connect<stream,stream>(vit_1_1_1.out,out_1[3]);
        connect<stream,stream>(vit_2_0_1.out,out_1[4]);
        connect<stream,stream>(vit_2_1_1.out,out_1[5]);
        connect<stream,stream>(vit_3_0_1.out,out_1[6]);
        connect<stream,stream>(vit_3_1_1.out,out_1[7]);
        
        connect<stream,stream>(vit_0_0_2.out,out_2[0]);
        connect<stream,stream>(vit_0_1_2.out,out_2[1]);
        connect<stream,stream>(vit_1_0_2.out,out_2[2]);
        connect<stream,stream>(vit_1_1_2.out,out_2[3]);
        connect<stream,stream>(vit_2_0_2.out,out_2[4]);
        connect<stream,stream>(vit_2_1_2.out,out_2[5]);
        connect<stream,stream>(vit_3_0_2.out,out_2[6]);
        connect<stream,stream>(vit_3_1_2.out,out_2[7]);
        
        
    }
};