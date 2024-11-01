#include "layer0/aie_top_L0.h"
#include "layer1/aie_top_L1.h"
#include "layer2/aie_top_L2.h"
#include "layer3/aie_top_L3.h"
#include "layer4/aie_top_L4.h"


using namespace adf;

class aie_top_all : public graph
{
public:
    //First: B, Second:(C//A_BRO), Third:A
    input_plio LHS_in_L0_0[4];
    input_plio LHS_in_L1[3][8];
    input_plio LHS_in_L2_0[24];
    input_plio LHS_in_L3_0[6];
    input_plio LHS_in_L4_0[2];

    //First: B, Second:(A//C_BRO), Third:C
    input_plio RHS_in_L1[3][4];
    input_plio RHS_in_L2_0[36];
    
    //First: C, Second:A
    output_plio out_L0_0[18];
    output_plio out_L1_0[8];
    output_plio out_L1_1[8];
    output_plio out_L1_2[8];
    output_plio out_L2_0[24];
    output_plio out_L3_0[4];
    output_plio out_L4_0[5];

    vit_x1_x4_x18_graph0  vit_graph0;
    vit_x4_x2_x2_graph1  vit_graph1;
    vit_x4_x6_x6_graph2  vit_graph2;
    vit_x1_x6_x4_graph3  vit_graph3;
    vit_x1_x2_x5_graph4  vit_graph4;

    aie_top_all() {
        for (int i = 0; i < 4; i++) {
            LHS_in_L0_0[i] = input_plio::create("LHS_in" + std::to_string(i) + "_L0_0", adf::plio_128_bits, "data/input0.txt", 230);
            connect<> net_lhs_in_L0_0 (LHS_in_L0_0[i].out[0], vit_graph0.in_lhs_0[0][i]);
        }
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 8; j++)
                LHS_in_L1[i][j] = input_plio::create("LHS_in" + std::to_string(j) + "_L1_" + std::to_string(i), adf::plio_128_bits, "data/input0.txt", 230);
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 2; k++)
                connect<> (LHS_in_L1[0][j*2+k].out[0], vit_graph1.in_lhs_0[j][k]);
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 2; k++)
                connect<> (LHS_in_L1[1][j*2+k].out[0], vit_graph1.in_lhs_1[j][k]);
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 2; k++)
                connect<> (LHS_in_L1[2][j*2+k].out[0], vit_graph1.in_lhs_2[j][k]);

        for (int i = 0; i < 24; i++)
            LHS_in_L2_0[i] = input_plio::create("LHS_in" + std::to_string(i) + "_L2_0", adf::plio_128_bits, "data/input0.txt", 230);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 6; j++)
                connect<> (LHS_in_L2_0[i*6+j].out[0], vit_graph2.in_lhs_0[i][j]);
    
        for (int i = 0; i < 6; i++) {
            LHS_in_L3_0[i] = input_plio::create("LHS_in" + std::to_string(i) + "_L3_0", adf::plio_128_bits, "data/input2.txt", 230);
            connect<> net_lhs_in_L3_0 (LHS_in_L3_0[i].out[0], vit_graph3.in_lhs_0[0][i]);
        }
        for (int i = 0; i < 2; i++) {
            LHS_in_L4_0[i] = input_plio::create("LHS_in" + std::to_string(i) + "_L4_0", adf::plio_128_bits, "data/input2.txt", 230);
            connect<> net_lhs_in_L4_0 (LHS_in_L4_0[i].out[0], vit_graph4.in_lhs_0[0][i]);
        }


        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 4; j++)
                RHS_in_L1[i][j] = input_plio::create("RHS_in" + std::to_string(j) + "_L1_" + std::to_string(i), adf::plio_128_bits, "data/input1.txt", 230);
        connect<> net_rhs_in0_L1_0 (RHS_in_L1[0][0].out[0], vit_graph1.in_rhs_0[0][0]);
        connect<> net_rhs_in1_L1_0 (RHS_in_L1[0][1].out[0], vit_graph1.in_rhs_0[0][1]);
        connect<> net_rhs_in2_L1_0 (RHS_in_L1[0][2].out[0], vit_graph1.in_rhs_0[1][0]);
        connect<> net_rhs_in3_L1_0 (RHS_in_L1[0][3].out[0], vit_graph1.in_rhs_0[1][1]);
        connect<> net_rhs_in0_L1_1 (RHS_in_L1[1][0].out[0], vit_graph1.in_rhs_1[0][0]);
        connect<> net_rhs_in1_L1_1 (RHS_in_L1[1][1].out[0], vit_graph1.in_rhs_1[0][1]);
        connect<> net_rhs_in2_L1_1 (RHS_in_L1[1][2].out[0], vit_graph1.in_rhs_1[1][0]);
        connect<> net_rhs_in3_L1_1 (RHS_in_L1[1][3].out[0], vit_graph1.in_rhs_1[1][1]);
        connect<> net_rhs_in0_L1_2 (RHS_in_L1[2][0].out[0], vit_graph1.in_rhs_2[0][0]);
        connect<> net_rhs_in1_L1_2 (RHS_in_L1[2][1].out[0], vit_graph1.in_rhs_2[0][1]);
        connect<> net_rhs_in2_L1_2 (RHS_in_L1[2][2].out[0], vit_graph1.in_rhs_2[1][0]);
        connect<> net_rhs_in3_L1_2 (RHS_in_L1[2][3].out[0], vit_graph1.in_rhs_2[1][1]);
        for (int i = 0; i < 36; i++)
            RHS_in_L2_0[i] = input_plio::create("RHS_in" + std::to_string(i) + "_L2_0", adf::plio_128_bits, "data/input1.txt", 230);
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                connect<> (RHS_in_L2_0[i*6+j].out[0], vit_graph2.in_rhs_0[i][j]);

        for (int i = 0; i < 18; i++) {
            out_L0_0[i] = output_plio::create("out" + std::to_string(i) + "_L0_0", adf::plio_128_bits, "data/output" + std::to_string(i) + ".txt", 230);
            connect<> net_outi_L0_0 (vit_graph0.out_0[i], out_L0_0[i].in[0]);
        }
        for (int i = 0; i < 8; i++) {
            out_L1_0[i] = output_plio::create("out" + std::to_string(i) + "_L1_0", adf::plio_128_bits, "data/output" + std::to_string(18 + i) + ".txt", 230);
            connect<> net_outi_L1_0 (vit_graph1.out_0[i], out_L1_0[i].in[0]);
        }
        for (int i = 0; i < 8; i++) {
            out_L1_1[i] = output_plio::create("out" + std::to_string(i) + "_L1_1", adf::plio_128_bits, "data/output" + std::to_string(26 + i) + ".txt", 230);
            connect<> net_outi_L1_1 (vit_graph1.out_1[i], out_L1_1[i].in[0]);
        }
        for (int i = 0; i < 8; i++) {
            out_L1_2[i] = output_plio::create("out" + std::to_string(i) + "_L1_2", adf::plio_128_bits, "data/output" + std::to_string(34 + i) + ".txt", 230);
            connect<> net_outi_L1_2 (vit_graph1.out_2[i], out_L1_2[i].in[0]);
        }
        for (int i = 0; i < 24; i++) {
            out_L2_0[i] = output_plio::create("out" + std::to_string(i) + "_L2_0", adf::plio_128_bits, "data/output" + std::to_string(42 + i) + ".txt", 230);
            connect<> net_outi_L2_0 (vit_graph2.out_0[i], out_L2_0[i].in[0]);
        }
        for (int i = 0; i < 4; i++) {
            out_L3_0[i] = output_plio::create("out" + std::to_string(i) + "_L3_0", adf::plio_128_bits, "data/output" + std::to_string(66 + i) + ".txt", 230);
            connect<> net_outi_L3_0 (vit_graph3.out_0[i], out_L3_0[i].in[0]);
        }
        for (int i = 0; i < 5; i++) {
            out_L4_0[i] = output_plio::create("out" + std::to_string(i) + "_L4_0", adf::plio_128_bits, "data/output" + std::to_string(70 + i) + ".txt", 230);
            connect<> net_outi_L4_0 (vit_graph4.out_0[i], out_L4_0[i].in[0]);
        }
    }
};
