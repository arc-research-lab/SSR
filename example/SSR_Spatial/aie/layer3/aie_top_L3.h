#include "aie_graph_L3.h"

using namespace adf;

const int L3_A=2;
const int L3_B=2;
const int L3_C=2;
const int L3_A_BRO=2;
const int L3_C_BRO=2;

class vit_x2_x2_x2_graph3 : public adf::graph {
public:
    
    input_port in_lhs_0[(L3_A*L3_C/L3_A_BRO)][L3_B];
    input_port in_rhs_0[(L3_A*L3_C/L3_C_BRO)][L3_B];
	output_port out_0[L3_A*L3_C];
    
    input_port in_lhs_1[(L3_A*L3_C/L3_A_BRO)][L3_B];
    input_port in_rhs_1[(L3_A*L3_C/L3_C_BRO)][L3_B];
	output_port out_1[L3_A*L3_C];
    
    input_port in_lhs_2[(L3_A*L3_C/L3_A_BRO)][L3_B];
    input_port in_rhs_2[(L3_A*L3_C/L3_C_BRO)][L3_B];
	output_port out_2[L3_A*L3_C];
    

    simpleGraph_k1_B2_L3 <12, 0>  vit_0_0_0;
    simpleGraph_k1_B2_L3 <13, 1>  vit_0_1_0;
    simpleGraph_k1_B2_L3 <14, 0>  vit_1_0_0;
    simpleGraph_k1_B2_L3 <15, 1>  vit_1_1_0;
    
    simpleGraph_k1_B2_L3 <16, 0>  vit_0_0_1;
    simpleGraph_k1_B2_L3 <17, 1>  vit_0_1_1;
    simpleGraph_k1_B2_L3 <18, 0>  vit_1_0_1;
    simpleGraph_k1_B2_L3 <19, 1>  vit_1_1_1;
    
    simpleGraph_k1_B2_L3 <20, 0>  vit_0_0_2;
    simpleGraph_k1_B2_L3 <21, 1>  vit_0_1_2;
    simpleGraph_k1_B2_L3 <22, 0>  vit_1_0_2;
    simpleGraph_k1_B2_L3 <23, 1>  vit_1_1_2;
    
    

    vit_x2_x2_x2_graph3 () {

        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[0][0],vit_0_0_0.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[0][1],vit_0_0_0.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[0][0],vit_0_1_0.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[0][1],vit_0_1_0.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[1][0],vit_1_0_0.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[1][1],vit_1_0_0.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[1][0],vit_1_1_0.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_0[1][1],vit_1_1_0.in0[1]);
        
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[0][0],vit_0_0_1.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[0][1],vit_0_0_1.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[0][0],vit_0_1_1.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[0][1],vit_0_1_1.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[1][0],vit_1_0_1.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[1][1],vit_1_0_1.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[1][0],vit_1_1_1.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_1[1][1],vit_1_1_1.in0[1]);
        
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[0][0],vit_0_0_2.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[0][1],vit_0_0_2.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[0][0],vit_0_1_2.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[0][1],vit_0_1_2.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[1][0],vit_1_0_2.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[1][1],vit_1_0_2.in0[1]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[1][0],vit_1_1_2.in0[0]);
        connect<stream,window<L3_h1*L3_w1>>(in_lhs_2[1][1],vit_1_1_2.in0[1]);
        
        

        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[0][0],vit_0_0_0.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[0][1],vit_0_0_0.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[0][0],vit_1_0_0.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[0][1],vit_1_0_0.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[1][0],vit_0_1_0.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[1][1],vit_0_1_0.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[1][0],vit_1_1_0.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_0[1][1],vit_1_1_0.in1[1]);
        
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[0][0],vit_0_0_1.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[0][1],vit_0_0_1.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[0][0],vit_1_0_1.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[0][1],vit_1_0_1.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[1][0],vit_0_1_1.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[1][1],vit_0_1_1.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[1][0],vit_1_1_1.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_1[1][1],vit_1_1_1.in1[1]);
        
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[0][0],vit_0_0_2.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[0][1],vit_0_0_2.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[0][0],vit_1_0_2.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[0][1],vit_1_0_2.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[1][0],vit_0_1_2.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[1][1],vit_0_1_2.in1[1]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[1][0],vit_1_1_2.in1[0]);
        connect<stream,window<L3_w1*L3_w2>>(in_rhs_2[1][1],vit_1_1_2.in1[1]);
        
        

        connect<stream,stream>(vit_0_0_0.out,out_0[0]);
        connect<stream,stream>(vit_0_1_0.out,out_0[1]);
        connect<stream,stream>(vit_1_0_0.out,out_0[2]);
        connect<stream,stream>(vit_1_1_0.out,out_0[3]);
        
        connect<stream,stream>(vit_0_0_1.out,out_1[0]);
        connect<stream,stream>(vit_0_1_1.out,out_1[1]);
        connect<stream,stream>(vit_1_0_1.out,out_1[2]);
        connect<stream,stream>(vit_1_1_1.out,out_1[3]);
        
        connect<stream,stream>(vit_0_0_2.out,out_2[0]);
        connect<stream,stream>(vit_0_1_2.out,out_2[1]);
        connect<stream,stream>(vit_1_0_2.out,out_2[2]);
        connect<stream,stream>(vit_1_1_2.out,out_2[3]);
        
        
    }
};