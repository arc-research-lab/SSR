#include "layer0/aie_top_L0.h"
#include "layer1/aie_top_L1.h"
#include "layer2/aie_top_L2.h"
#include "layer3/aie_top_L3.h"
#include "layer4/aie_top_L4.h"
#include "layer5/aie_top_L5.h"
#include "layer6/aie_top_L6.h"
#include "layer7/aie_top_L7.h"
#include "layer8/aie_top_L8.h"
#include "layer9/aie_top_L9.h"
using namespace adf;

//First: B, Second:(C//A_BRO), Third:A
PLIO* LHS_in0_L0_0 = new PLIO("LHS_in0_L0_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L0_0 = new PLIO("LHS_in1_L0_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L0_0 = new PLIO("LHS_in2_L0_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L0_0 = new PLIO("LHS_in3_L0_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L1_0 = new PLIO("LHS_in0_L1_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L1_0 = new PLIO("LHS_in1_L1_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L1_0 = new PLIO("LHS_in2_L1_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L1_0 = new PLIO("LHS_in3_L1_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L2_0 = new PLIO("LHS_in0_L2_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L2_0 = new PLIO("LHS_in1_L2_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L2_0 = new PLIO("LHS_in2_L2_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L2_0 = new PLIO("LHS_in3_L2_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L3_0 = new PLIO("LHS_in0_L3_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L3_0 = new PLIO("LHS_in1_L3_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L3_0 = new PLIO("LHS_in2_L3_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L3_0 = new PLIO("LHS_in3_L3_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L3_1 = new PLIO("LHS_in0_L3_1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L3_1 = new PLIO("LHS_in1_L3_1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L3_1 = new PLIO("LHS_in2_L3_1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L3_1 = new PLIO("LHS_in3_L3_1", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L3_2 = new PLIO("LHS_in0_L3_2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L3_2 = new PLIO("LHS_in1_L3_2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L3_2 = new PLIO("LHS_in2_L3_2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L3_2 = new PLIO("LHS_in3_L3_2", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L4_0 = new PLIO("LHS_in0_L4_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L4_0 = new PLIO("LHS_in1_L4_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L4_0 = new PLIO("LHS_in2_L4_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L4_0 = new PLIO("LHS_in3_L4_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L4_1 = new PLIO("LHS_in0_L4_1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L4_1 = new PLIO("LHS_in1_L4_1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L4_1 = new PLIO("LHS_in2_L4_1", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L4_1 = new PLIO("LHS_in3_L4_1", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L4_2 = new PLIO("LHS_in0_L4_2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L4_2 = new PLIO("LHS_in1_L4_2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L4_2 = new PLIO("LHS_in2_L4_2", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L4_2 = new PLIO("LHS_in3_L4_2", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L5_0 = new PLIO("LHS_in0_L5_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L5_0 = new PLIO("LHS_in1_L5_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L5_0 = new PLIO("LHS_in2_L5_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L5_0 = new PLIO("LHS_in3_L5_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in4_L5_0 = new PLIO("LHS_in4_L5_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in5_L5_0 = new PLIO("LHS_in5_L5_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L6_0 = new PLIO("LHS_in0_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L6_0 = new PLIO("LHS_in1_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L6_0 = new PLIO("LHS_in2_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L6_0 = new PLIO("LHS_in3_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in4_L6_0 = new PLIO("LHS_in4_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in5_L6_0 = new PLIO("LHS_in5_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in6_L6_0 = new PLIO("LHS_in6_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in7_L6_0 = new PLIO("LHS_in7_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in8_L6_0 = new PLIO("LHS_in8_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in9_L6_0 = new PLIO("LHS_in9_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in10_L6_0 = new PLIO("LHS_in10_L6_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in11_L6_0 = new PLIO("LHS_in11_L6_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L7_0 = new PLIO("LHS_in0_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in1_L7_0 = new PLIO("LHS_in1_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in2_L7_0 = new PLIO("LHS_in2_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in3_L7_0 = new PLIO("LHS_in3_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in4_L7_0 = new PLIO("LHS_in4_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in5_L7_0 = new PLIO("LHS_in5_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in6_L7_0 = new PLIO("LHS_in6_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in7_L7_0 = new PLIO("LHS_in7_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in8_L7_0 = new PLIO("LHS_in8_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in9_L7_0 = new PLIO("LHS_in9_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in10_L7_0 = new PLIO("LHS_in10_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in11_L7_0 = new PLIO("LHS_in11_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in12_L7_0 = new PLIO("LHS_in12_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in13_L7_0 = new PLIO("LHS_in13_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in14_L7_0 = new PLIO("LHS_in14_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in15_L7_0 = new PLIO("LHS_in15_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in16_L7_0 = new PLIO("LHS_in16_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in17_L7_0 = new PLIO("LHS_in17_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in18_L7_0 = new PLIO("LHS_in18_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in19_L7_0 = new PLIO("LHS_in19_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in20_L7_0 = new PLIO("LHS_in20_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in21_L7_0 = new PLIO("LHS_in21_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in22_L7_0 = new PLIO("LHS_in22_L7_0", adf::plio_128_bits, "data/input0.txt",230);
PLIO* LHS_in23_L7_0 = new PLIO("LHS_in23_L7_0", adf::plio_128_bits, "data/input0.txt",230);

PLIO* LHS_in0_L8_0 = new PLIO("LHS_in0_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in1_L8_0 = new PLIO("LHS_in1_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in2_L8_0 = new PLIO("LHS_in2_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in3_L8_0 = new PLIO("LHS_in3_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in4_L8_0 = new PLIO("LHS_in4_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in5_L8_0 = new PLIO("LHS_in5_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in6_L8_0 = new PLIO("LHS_in6_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in7_L8_0 = new PLIO("LHS_in7_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in8_L8_0 = new PLIO("LHS_in8_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in9_L8_0 = new PLIO("LHS_in9_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in10_L8_0 = new PLIO("LHS_in10_L8_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in11_L8_0 = new PLIO("LHS_in11_L8_0", adf::plio_128_bits, "data/input2.txt",230);

PLIO* LHS_in0_L9_0 = new PLIO("LHS_in0_L9_0", adf::plio_128_bits, "data/input2.txt",230);
PLIO* LHS_in1_L9_0 = new PLIO("LHS_in1_L9_0", adf::plio_128_bits, "data/input2.txt",230);

//First: B, Second:(A//C_BRO), Third:C



PLIO* RHS_in0_L3_0 = new PLIO("RHS_in0_L3_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L3_0 = new PLIO("RHS_in1_L3_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L3_0 = new PLIO("RHS_in2_L3_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L3_0 = new PLIO("RHS_in3_L3_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L3_1 = new PLIO("RHS_in0_L3_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L3_1 = new PLIO("RHS_in1_L3_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L3_1 = new PLIO("RHS_in2_L3_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L3_1 = new PLIO("RHS_in3_L3_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L3_2 = new PLIO("RHS_in0_L3_2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L3_2 = new PLIO("RHS_in1_L3_2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L3_2 = new PLIO("RHS_in2_L3_2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L3_2 = new PLIO("RHS_in3_L3_2", adf::plio_128_bits, "data/input1.txt",230);


PLIO* RHS_in0_L4_0 = new PLIO("RHS_in0_L4_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L4_0 = new PLIO("RHS_in1_L4_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L4_0 = new PLIO("RHS_in2_L4_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L4_0 = new PLIO("RHS_in3_L4_0", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L4_1 = new PLIO("RHS_in0_L4_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L4_1 = new PLIO("RHS_in1_L4_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L4_1 = new PLIO("RHS_in2_L4_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L4_1 = new PLIO("RHS_in3_L4_1", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in0_L4_2 = new PLIO("RHS_in0_L4_2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in1_L4_2 = new PLIO("RHS_in1_L4_2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in2_L4_2 = new PLIO("RHS_in2_L4_2", adf::plio_128_bits, "data/input1.txt",230);
PLIO* RHS_in3_L4_2 = new PLIO("RHS_in3_L4_2", adf::plio_128_bits, "data/input1.txt",230);







//First: C, Second:A
PLIO* out0_L0_0 = new PLIO("out0_L0_0", adf::plio_128_bits, "data/output0.txt",230);
PLIO* out1_L0_0 = new PLIO("out1_L0_0", adf::plio_128_bits, "data/output1.txt",230);
PLIO* out2_L0_0 = new PLIO("out2_L0_0", adf::plio_128_bits, "data/output2.txt",230);
PLIO* out3_L0_0 = new PLIO("out3_L0_0", adf::plio_128_bits, "data/output3.txt",230);
PLIO* out4_L0_0 = new PLIO("out4_L0_0", adf::plio_128_bits, "data/output4.txt",230);
PLIO* out5_L0_0 = new PLIO("out5_L0_0", adf::plio_128_bits, "data/output5.txt",230);

PLIO* out0_L1_0 = new PLIO("out0_L1_0", adf::plio_128_bits, "data/output6.txt",230);
PLIO* out1_L1_0 = new PLIO("out1_L1_0", adf::plio_128_bits, "data/output7.txt",230);
PLIO* out2_L1_0 = new PLIO("out2_L1_0", adf::plio_128_bits, "data/output8.txt",230);
PLIO* out3_L1_0 = new PLIO("out3_L1_0", adf::plio_128_bits, "data/output9.txt",230);
PLIO* out4_L1_0 = new PLIO("out4_L1_0", adf::plio_128_bits, "data/output10.txt",230);
PLIO* out5_L1_0 = new PLIO("out5_L1_0", adf::plio_128_bits, "data/output11.txt",230);

PLIO* out0_L2_0 = new PLIO("out0_L2_0", adf::plio_128_bits, "data/output12.txt",230);
PLIO* out1_L2_0 = new PLIO("out1_L2_0", adf::plio_128_bits, "data/output13.txt",230);
PLIO* out2_L2_0 = new PLIO("out2_L2_0", adf::plio_128_bits, "data/output14.txt",230);
PLIO* out3_L2_0 = new PLIO("out3_L2_0", adf::plio_128_bits, "data/output15.txt",230);
PLIO* out4_L2_0 = new PLIO("out4_L2_0", adf::plio_128_bits, "data/output16.txt",230);
PLIO* out5_L2_0 = new PLIO("out5_L2_0", adf::plio_128_bits, "data/output17.txt",230);

PLIO* out0_L3_0 = new PLIO("out0_L3_0", adf::plio_128_bits, "data/output18.txt",230);
PLIO* out1_L3_0 = new PLIO("out1_L3_0", adf::plio_128_bits, "data/output19.txt",230);
PLIO* out2_L3_0 = new PLIO("out2_L3_0", adf::plio_128_bits, "data/output20.txt",230);
PLIO* out3_L3_0 = new PLIO("out3_L3_0", adf::plio_128_bits, "data/output21.txt",230);

PLIO* out0_L3_1 = new PLIO("out0_L3_1", adf::plio_128_bits, "data/output22.txt",230);
PLIO* out1_L3_1 = new PLIO("out1_L3_1", adf::plio_128_bits, "data/output23.txt",230);
PLIO* out2_L3_1 = new PLIO("out2_L3_1", adf::plio_128_bits, "data/output24.txt",230);
PLIO* out3_L3_1 = new PLIO("out3_L3_1", adf::plio_128_bits, "data/output25.txt",230);

PLIO* out0_L3_2 = new PLIO("out0_L3_2", adf::plio_128_bits, "data/output26.txt",230);
PLIO* out1_L3_2 = new PLIO("out1_L3_2", adf::plio_128_bits, "data/output27.txt",230);
PLIO* out2_L3_2 = new PLIO("out2_L3_2", adf::plio_128_bits, "data/output28.txt",230);
PLIO* out3_L3_2 = new PLIO("out3_L3_2", adf::plio_128_bits, "data/output29.txt",230);

PLIO* out0_L4_0 = new PLIO("out0_L4_0", adf::plio_128_bits, "data/output30.txt",230);
PLIO* out1_L4_0 = new PLIO("out1_L4_0", adf::plio_128_bits, "data/output31.txt",230);
PLIO* out2_L4_0 = new PLIO("out2_L4_0", adf::plio_128_bits, "data/output32.txt",230);
PLIO* out3_L4_0 = new PLIO("out3_L4_0", adf::plio_128_bits, "data/output33.txt",230);

PLIO* out0_L4_1 = new PLIO("out0_L4_1", adf::plio_128_bits, "data/output34.txt",230);
PLIO* out1_L4_1 = new PLIO("out1_L4_1", adf::plio_128_bits, "data/output35.txt",230);
PLIO* out2_L4_1 = new PLIO("out2_L4_1", adf::plio_128_bits, "data/output36.txt",230);
PLIO* out3_L4_1 = new PLIO("out3_L4_1", adf::plio_128_bits, "data/output37.txt",230);

PLIO* out0_L4_2 = new PLIO("out0_L4_2", adf::plio_128_bits, "data/output38.txt",230);
PLIO* out1_L4_2 = new PLIO("out1_L4_2", adf::plio_128_bits, "data/output39.txt",230);
PLIO* out2_L4_2 = new PLIO("out2_L4_2", adf::plio_128_bits, "data/output40.txt",230);
PLIO* out3_L4_2 = new PLIO("out3_L4_2", adf::plio_128_bits, "data/output41.txt",230);

PLIO* out0_L5_0 = new PLIO("out0_L5_0", adf::plio_128_bits, "data/output42.txt",230);
PLIO* out1_L5_0 = new PLIO("out1_L5_0", adf::plio_128_bits, "data/output43.txt",230);
PLIO* out2_L5_0 = new PLIO("out2_L5_0", adf::plio_128_bits, "data/output44.txt",230);
PLIO* out3_L5_0 = new PLIO("out3_L5_0", adf::plio_128_bits, "data/output45.txt",230);

PLIO* out0_L6_0 = new PLIO("out0_L6_0", adf::plio_128_bits, "data/output46.txt",230);
PLIO* out1_L6_0 = new PLIO("out1_L6_0", adf::plio_128_bits, "data/output47.txt",230);
PLIO* out2_L6_0 = new PLIO("out2_L6_0", adf::plio_128_bits, "data/output48.txt",230);
PLIO* out3_L6_0 = new PLIO("out3_L6_0", adf::plio_128_bits, "data/output49.txt",230);
PLIO* out4_L6_0 = new PLIO("out4_L6_0", adf::plio_128_bits, "data/output50.txt",230);
PLIO* out5_L6_0 = new PLIO("out5_L6_0", adf::plio_128_bits, "data/output51.txt",230);
PLIO* out6_L6_0 = new PLIO("out6_L6_0", adf::plio_128_bits, "data/output52.txt",230);
PLIO* out7_L6_0 = new PLIO("out7_L6_0", adf::plio_128_bits, "data/output53.txt",230);
PLIO* out8_L6_0 = new PLIO("out8_L6_0", adf::plio_128_bits, "data/output54.txt",230);
PLIO* out9_L6_0 = new PLIO("out9_L6_0", adf::plio_128_bits, "data/output55.txt",230);
PLIO* out10_L6_0 = new PLIO("out10_L6_0", adf::plio_128_bits, "data/output56.txt",230);
PLIO* out11_L6_0 = new PLIO("out11_L6_0", adf::plio_128_bits, "data/output57.txt",230);
PLIO* out12_L6_0 = new PLIO("out12_L6_0", adf::plio_128_bits, "data/output58.txt",230);
PLIO* out13_L6_0 = new PLIO("out13_L6_0", adf::plio_128_bits, "data/output59.txt",230);
PLIO* out14_L6_0 = new PLIO("out14_L6_0", adf::plio_128_bits, "data/output60.txt",230);
PLIO* out15_L6_0 = new PLIO("out15_L6_0", adf::plio_128_bits, "data/output61.txt",230);
PLIO* out16_L6_0 = new PLIO("out16_L6_0", adf::plio_128_bits, "data/output62.txt",230);
PLIO* out17_L6_0 = new PLIO("out17_L6_0", adf::plio_128_bits, "data/output63.txt",230);
PLIO* out18_L6_0 = new PLIO("out18_L6_0", adf::plio_128_bits, "data/output64.txt",230);
PLIO* out19_L6_0 = new PLIO("out19_L6_0", adf::plio_128_bits, "data/output65.txt",230);
PLIO* out20_L6_0 = new PLIO("out20_L6_0", adf::plio_128_bits, "data/output66.txt",230);
PLIO* out21_L6_0 = new PLIO("out21_L6_0", adf::plio_128_bits, "data/output67.txt",230);
PLIO* out22_L6_0 = new PLIO("out22_L6_0", adf::plio_128_bits, "data/output68.txt",230);
PLIO* out23_L6_0 = new PLIO("out23_L6_0", adf::plio_128_bits, "data/output69.txt",230);

PLIO* out0_L7_0 = new PLIO("out0_L7_0", adf::plio_128_bits, "data/output70.txt",230);
PLIO* out1_L7_0 = new PLIO("out1_L7_0", adf::plio_128_bits, "data/output71.txt",230);
PLIO* out2_L7_0 = new PLIO("out2_L7_0", adf::plio_128_bits, "data/output72.txt",230);
PLIO* out3_L7_0 = new PLIO("out3_L7_0", adf::plio_128_bits, "data/output73.txt",230);

PLIO* out0_L8_0 = new PLIO("out0_L8_0", adf::plio_128_bits, "data/output74.txt",230);
PLIO* out1_L8_0 = new PLIO("out1_L8_0", adf::plio_128_bits, "data/output75.txt",230);
PLIO* out2_L8_0 = new PLIO("out2_L8_0", adf::plio_128_bits, "data/output76.txt",230);
PLIO* out3_L8_0 = new PLIO("out3_L8_0", adf::plio_128_bits, "data/output77.txt",230);

PLIO* out0_L9_0 = new PLIO("out0_L9_0", adf::plio_128_bits, "data/output78.txt",230);
PLIO* out1_L9_0 = new PLIO("out1_L9_0", adf::plio_128_bits, "data/output79.txt",230);
PLIO* out2_L9_0 = new PLIO("out2_L9_0", adf::plio_128_bits, "data/output80.txt",230);
PLIO* out3_L9_0 = new PLIO("out3_L9_0", adf::plio_128_bits, "data/output81.txt",230);
PLIO* out4_L9_0 = new PLIO("out4_L9_0", adf::plio_128_bits, "data/output82.txt",230);

simulation::platform<116, 83> platform(
LHS_in0_L0_0,
LHS_in1_L0_0,
LHS_in2_L0_0,
LHS_in3_L0_0,
LHS_in0_L1_0,
LHS_in1_L1_0,
LHS_in2_L1_0,
LHS_in3_L1_0,
LHS_in0_L2_0,
LHS_in1_L2_0,
LHS_in2_L2_0,
LHS_in3_L2_0,
LHS_in0_L3_0,
LHS_in1_L3_0,
LHS_in2_L3_0,
LHS_in3_L3_0,
LHS_in0_L3_1,
LHS_in1_L3_1,
LHS_in2_L3_1,
LHS_in3_L3_1,
LHS_in0_L3_2,
LHS_in1_L3_2,
LHS_in2_L3_2,
LHS_in3_L3_2,
LHS_in0_L4_0,
LHS_in1_L4_0,
LHS_in2_L4_0,
LHS_in3_L4_0,
LHS_in0_L4_1,
LHS_in1_L4_1,
LHS_in2_L4_1,
LHS_in3_L4_1,
LHS_in0_L4_2,
LHS_in1_L4_2,
LHS_in2_L4_2,
LHS_in3_L4_2,
LHS_in0_L5_0,
LHS_in1_L5_0,
LHS_in2_L5_0,
LHS_in3_L5_0,
LHS_in4_L5_0,
LHS_in5_L5_0,
LHS_in0_L6_0,
LHS_in1_L6_0,
LHS_in2_L6_0,
LHS_in3_L6_0,
LHS_in4_L6_0,
LHS_in5_L6_0,
LHS_in6_L6_0,
LHS_in7_L6_0,
LHS_in8_L6_0,
LHS_in9_L6_0,
LHS_in10_L6_0,
LHS_in11_L6_0,
LHS_in0_L7_0,
LHS_in1_L7_0,
LHS_in2_L7_0,
LHS_in3_L7_0,
LHS_in4_L7_0,
LHS_in5_L7_0,
LHS_in6_L7_0,
LHS_in7_L7_0,
LHS_in8_L7_0,
LHS_in9_L7_0,
LHS_in10_L7_0,
LHS_in11_L7_0,
LHS_in12_L7_0,
LHS_in13_L7_0,
LHS_in14_L7_0,
LHS_in15_L7_0,
LHS_in16_L7_0,
LHS_in17_L7_0,
LHS_in18_L7_0,
LHS_in19_L7_0,
LHS_in20_L7_0,
LHS_in21_L7_0,
LHS_in22_L7_0,
LHS_in23_L7_0,
LHS_in0_L8_0,
LHS_in1_L8_0,
LHS_in2_L8_0,
LHS_in3_L8_0,
LHS_in4_L8_0,
LHS_in5_L8_0,
LHS_in6_L8_0,
LHS_in7_L8_0,
LHS_in8_L8_0,
LHS_in9_L8_0,
LHS_in10_L8_0,
LHS_in11_L8_0,
LHS_in0_L9_0,
LHS_in1_L9_0,
RHS_in0_L3_0, 
RHS_in1_L3_0, 
RHS_in2_L3_0, 
RHS_in3_L3_0, 
RHS_in0_L3_1, 
RHS_in1_L3_1, 
RHS_in2_L3_1, 
RHS_in3_L3_1, 
RHS_in0_L3_2, 
RHS_in1_L3_2, 
RHS_in2_L3_2, 
RHS_in3_L3_2, 
RHS_in0_L4_0, 
RHS_in1_L4_0, 
RHS_in2_L4_0, 
RHS_in3_L4_0, 
RHS_in0_L4_1, 
RHS_in1_L4_1, 
RHS_in2_L4_1, 
RHS_in3_L4_1, 
RHS_in0_L4_2, 
RHS_in1_L4_2, 
RHS_in2_L4_2, 
RHS_in3_L4_2, 
out0_L0_0,
out1_L0_0,
out2_L0_0,
out3_L0_0,
out4_L0_0,
out5_L0_0,
out0_L1_0,
out1_L1_0,
out2_L1_0,
out3_L1_0,
out4_L1_0,
out5_L1_0,
out0_L2_0,
out1_L2_0,
out2_L2_0,
out3_L2_0,
out4_L2_0,
out5_L2_0,
out0_L3_0,
out1_L3_0,
out2_L3_0,
out3_L3_0,
out0_L3_1,
out1_L3_1,
out2_L3_1,
out3_L3_1,
out0_L3_2,
out1_L3_2,
out2_L3_2,
out3_L3_2,
out0_L4_0,
out1_L4_0,
out2_L4_0,
out3_L4_0,
out0_L4_1,
out1_L4_1,
out2_L4_1,
out3_L4_1,
out0_L4_2,
out1_L4_2,
out2_L4_2,
out3_L4_2,
out0_L5_0,
out1_L5_0,
out2_L5_0,
out3_L5_0,
out0_L6_0,
out1_L6_0,
out2_L6_0,
out3_L6_0,
out4_L6_0,
out5_L6_0,
out6_L6_0,
out7_L6_0,
out8_L6_0,
out9_L6_0,
out10_L6_0,
out11_L6_0,
out12_L6_0,
out13_L6_0,
out14_L6_0,
out15_L6_0,
out16_L6_0,
out17_L6_0,
out18_L6_0,
out19_L6_0,
out20_L6_0,
out21_L6_0,
out22_L6_0,
out23_L6_0,
out0_L7_0,
out1_L7_0,
out2_L7_0,
out3_L7_0,
out0_L8_0,
out1_L8_0,
out2_L8_0,
out3_L8_0,
out0_L9_0,
out1_L9_0,
out2_L9_0,
out3_L9_0,
out4_L9_0 
);


vit_x1_x4_x6_graph0  vit_graph0;
vit_x1_x4_x6_graph1  vit_graph1;
vit_x1_x4_x6_graph2  vit_graph2;
vit_x2_x2_x2_graph3  vit_graph3;
vit_x2_x2_x2_graph4  vit_graph4;
vit_x1_x6_x4_graph5  vit_graph5;
vit_x1_x4_x24_graph6  vit_graph6;
vit_x1_x24_x4_graph7  vit_graph7;
vit_x1_x12_x4_graph8  vit_graph8;
vit_x1_x2_x5_graph9  vit_graph9;


connect<> net_lhs_in0_L0_0 (platform.src[0], vit_graph0.in_lhs_0[0][0]);
connect<> net_lhs_in1_L0_0 (platform.src[1], vit_graph0.in_lhs_0[0][1]);
connect<> net_lhs_in2_L0_0 (platform.src[2], vit_graph0.in_lhs_0[0][2]);
connect<> net_lhs_in3_L0_0 (platform.src[3], vit_graph0.in_lhs_0[0][3]);

connect<> net_lhs_in0_L1_0 (platform.src[4], vit_graph1.in_lhs_0[0][0]);
connect<> net_lhs_in1_L1_0 (platform.src[5], vit_graph1.in_lhs_0[0][1]);
connect<> net_lhs_in2_L1_0 (platform.src[6], vit_graph1.in_lhs_0[0][2]);
connect<> net_lhs_in3_L1_0 (platform.src[7], vit_graph1.in_lhs_0[0][3]);

connect<> net_lhs_in0_L2_0 (platform.src[8], vit_graph2.in_lhs_0[0][0]);
connect<> net_lhs_in1_L2_0 (platform.src[9], vit_graph2.in_lhs_0[0][1]);
connect<> net_lhs_in2_L2_0 (platform.src[10], vit_graph2.in_lhs_0[0][2]);
connect<> net_lhs_in3_L2_0 (platform.src[11], vit_graph2.in_lhs_0[0][3]);

connect<> net_lhs_in0_L3_0 (platform.src[12], vit_graph3.in_lhs_0[0][0]);
connect<> net_lhs_in1_L3_0 (platform.src[13], vit_graph3.in_lhs_0[0][1]);
connect<> net_lhs_in2_L3_0 (platform.src[14], vit_graph3.in_lhs_0[1][0]);
connect<> net_lhs_in3_L3_0 (platform.src[15], vit_graph3.in_lhs_0[1][1]);
connect<> net_lhs_in0_L3_1 (platform.src[16], vit_graph3.in_lhs_1[0][0]);
connect<> net_lhs_in1_L3_1 (platform.src[17], vit_graph3.in_lhs_1[0][1]);
connect<> net_lhs_in2_L3_1 (platform.src[18], vit_graph3.in_lhs_1[1][0]);
connect<> net_lhs_in3_L3_1 (platform.src[19], vit_graph3.in_lhs_1[1][1]);
connect<> net_lhs_in0_L3_2 (platform.src[20], vit_graph3.in_lhs_2[0][0]);
connect<> net_lhs_in1_L3_2 (platform.src[21], vit_graph3.in_lhs_2[0][1]);
connect<> net_lhs_in2_L3_2 (platform.src[22], vit_graph3.in_lhs_2[1][0]);
connect<> net_lhs_in3_L3_2 (platform.src[23], vit_graph3.in_lhs_2[1][1]);

connect<> net_lhs_in0_L4_0 (platform.src[24], vit_graph4.in_lhs_0[0][0]);
connect<> net_lhs_in1_L4_0 (platform.src[25], vit_graph4.in_lhs_0[0][1]);
connect<> net_lhs_in2_L4_0 (platform.src[26], vit_graph4.in_lhs_0[1][0]);
connect<> net_lhs_in3_L4_0 (platform.src[27], vit_graph4.in_lhs_0[1][1]);
connect<> net_lhs_in0_L4_1 (platform.src[28], vit_graph4.in_lhs_1[0][0]);
connect<> net_lhs_in1_L4_1 (platform.src[29], vit_graph4.in_lhs_1[0][1]);
connect<> net_lhs_in2_L4_1 (platform.src[30], vit_graph4.in_lhs_1[1][0]);
connect<> net_lhs_in3_L4_1 (platform.src[31], vit_graph4.in_lhs_1[1][1]);
connect<> net_lhs_in0_L4_2 (platform.src[32], vit_graph4.in_lhs_2[0][0]);
connect<> net_lhs_in1_L4_2 (platform.src[33], vit_graph4.in_lhs_2[0][1]);
connect<> net_lhs_in2_L4_2 (platform.src[34], vit_graph4.in_lhs_2[1][0]);
connect<> net_lhs_in3_L4_2 (platform.src[35], vit_graph4.in_lhs_2[1][1]);

connect<> net_lhs_in0_L5_0 (platform.src[36], vit_graph5.in_lhs_0[0][0]);
connect<> net_lhs_in1_L5_0 (platform.src[37], vit_graph5.in_lhs_0[0][1]);
connect<> net_lhs_in2_L5_0 (platform.src[38], vit_graph5.in_lhs_0[0][2]);
connect<> net_lhs_in3_L5_0 (platform.src[39], vit_graph5.in_lhs_0[0][3]);
connect<> net_lhs_in4_L5_0 (platform.src[40], vit_graph5.in_lhs_0[0][4]);
connect<> net_lhs_in5_L5_0 (platform.src[41], vit_graph5.in_lhs_0[0][5]);

connect<> net_lhs_in0_L6_0 (platform.src[42], vit_graph6.in_lhs_0[0][0]);
connect<> net_lhs_in1_L6_0 (platform.src[43], vit_graph6.in_lhs_0[0][1]);
connect<> net_lhs_in2_L6_0 (platform.src[44], vit_graph6.in_lhs_0[0][2]);
connect<> net_lhs_in3_L6_0 (platform.src[45], vit_graph6.in_lhs_0[0][3]);
connect<> net_lhs_in4_L6_0 (platform.src[46], vit_graph6.in_lhs_0[1][0]);
connect<> net_lhs_in5_L6_0 (platform.src[47], vit_graph6.in_lhs_0[1][1]);
connect<> net_lhs_in6_L6_0 (platform.src[48], vit_graph6.in_lhs_0[1][2]);
connect<> net_lhs_in7_L6_0 (platform.src[49], vit_graph6.in_lhs_0[1][3]);
connect<> net_lhs_in8_L6_0 (platform.src[50], vit_graph6.in_lhs_0[2][0]);
connect<> net_lhs_in9_L6_0 (platform.src[51], vit_graph6.in_lhs_0[2][1]);
connect<> net_lhs_in10_L6_0 (platform.src[52], vit_graph6.in_lhs_0[2][2]);
connect<> net_lhs_in11_L6_0 (platform.src[53], vit_graph6.in_lhs_0[2][3]);

connect<> net_lhs_in0_L7_0 (platform.src[54], vit_graph7.in_lhs_0[0][0]);
connect<> net_lhs_in1_L7_0 (platform.src[55], vit_graph7.in_lhs_0[0][1]);
connect<> net_lhs_in2_L7_0 (platform.src[56], vit_graph7.in_lhs_0[0][2]);
connect<> net_lhs_in3_L7_0 (platform.src[57], vit_graph7.in_lhs_0[0][3]);
connect<> net_lhs_in4_L7_0 (platform.src[58], vit_graph7.in_lhs_0[0][4]);
connect<> net_lhs_in5_L7_0 (platform.src[59], vit_graph7.in_lhs_0[0][5]);
connect<> net_lhs_in6_L7_0 (platform.src[60], vit_graph7.in_lhs_0[0][6]);
connect<> net_lhs_in7_L7_0 (platform.src[61], vit_graph7.in_lhs_0[0][7]);
connect<> net_lhs_in8_L7_0 (platform.src[62], vit_graph7.in_lhs_0[0][8]);
connect<> net_lhs_in9_L7_0 (platform.src[63], vit_graph7.in_lhs_0[0][9]);
connect<> net_lhs_in10_L7_0 (platform.src[64], vit_graph7.in_lhs_0[0][10]);
connect<> net_lhs_in11_L7_0 (platform.src[65], vit_graph7.in_lhs_0[0][11]);
connect<> net_lhs_in12_L7_0 (platform.src[66], vit_graph7.in_lhs_0[0][12]);
connect<> net_lhs_in13_L7_0 (platform.src[67], vit_graph7.in_lhs_0[0][13]);
connect<> net_lhs_in14_L7_0 (platform.src[68], vit_graph7.in_lhs_0[0][14]);
connect<> net_lhs_in15_L7_0 (platform.src[69], vit_graph7.in_lhs_0[0][15]);
connect<> net_lhs_in16_L7_0 (platform.src[70], vit_graph7.in_lhs_0[0][16]);
connect<> net_lhs_in17_L7_0 (platform.src[71], vit_graph7.in_lhs_0[0][17]);
connect<> net_lhs_in18_L7_0 (platform.src[72], vit_graph7.in_lhs_0[0][18]);
connect<> net_lhs_in19_L7_0 (platform.src[73], vit_graph7.in_lhs_0[0][19]);
connect<> net_lhs_in20_L7_0 (platform.src[74], vit_graph7.in_lhs_0[0][20]);
connect<> net_lhs_in21_L7_0 (platform.src[75], vit_graph7.in_lhs_0[0][21]);
connect<> net_lhs_in22_L7_0 (platform.src[76], vit_graph7.in_lhs_0[0][22]);
connect<> net_lhs_in23_L7_0 (platform.src[77], vit_graph7.in_lhs_0[0][23]);

connect<> net_lhs_in0_L8_0 (platform.src[78], vit_graph8.in_lhs_0[0][0]);
connect<> net_lhs_in1_L8_0 (platform.src[79], vit_graph8.in_lhs_0[0][1]);
connect<> net_lhs_in2_L8_0 (platform.src[80], vit_graph8.in_lhs_0[0][2]);
connect<> net_lhs_in3_L8_0 (platform.src[81], vit_graph8.in_lhs_0[0][3]);
connect<> net_lhs_in4_L8_0 (platform.src[82], vit_graph8.in_lhs_0[0][4]);
connect<> net_lhs_in5_L8_0 (platform.src[83], vit_graph8.in_lhs_0[0][5]);
connect<> net_lhs_in6_L8_0 (platform.src[84], vit_graph8.in_lhs_0[0][6]);
connect<> net_lhs_in7_L8_0 (platform.src[85], vit_graph8.in_lhs_0[0][7]);
connect<> net_lhs_in8_L8_0 (platform.src[86], vit_graph8.in_lhs_0[0][8]);
connect<> net_lhs_in9_L8_0 (platform.src[87], vit_graph8.in_lhs_0[0][9]);
connect<> net_lhs_in10_L8_0 (platform.src[88], vit_graph8.in_lhs_0[0][10]);
connect<> net_lhs_in11_L8_0 (platform.src[89], vit_graph8.in_lhs_0[0][11]);

connect<> net_lhs_in0_L9_0 (platform.src[90], vit_graph9.in_lhs_0[0][0]);
connect<> net_lhs_in1_L9_0 (platform.src[91], vit_graph9.in_lhs_0[0][1]);

connect<> net_rhs_in0_L3_0 (platform.src[92], vit_graph3.in_rhs_0[0][0]);
connect<> net_rhs_in1_L3_0 (platform.src[93], vit_graph3.in_rhs_0[0][1]);
connect<> net_rhs_in2_L3_0 (platform.src[94], vit_graph3.in_rhs_0[1][0]);
connect<> net_rhs_in3_L3_0 (platform.src[95], vit_graph3.in_rhs_0[1][1]);
connect<> net_rhs_in0_L3_1 (platform.src[96], vit_graph3.in_rhs_1[0][0]);
connect<> net_rhs_in1_L3_1 (platform.src[97], vit_graph3.in_rhs_1[0][1]);
connect<> net_rhs_in2_L3_1 (platform.src[98], vit_graph3.in_rhs_1[1][0]);
connect<> net_rhs_in3_L3_1 (platform.src[99], vit_graph3.in_rhs_1[1][1]);
connect<> net_rhs_in0_L3_2 (platform.src[100], vit_graph3.in_rhs_2[0][0]);
connect<> net_rhs_in1_L3_2 (platform.src[101], vit_graph3.in_rhs_2[0][1]);
connect<> net_rhs_in2_L3_2 (platform.src[102], vit_graph3.in_rhs_2[1][0]);
connect<> net_rhs_in3_L3_2 (platform.src[103], vit_graph3.in_rhs_2[1][1]);

connect<> net_rhs_in0_L4_0 (platform.src[104], vit_graph4.in_rhs_0[0][0]);
connect<> net_rhs_in1_L4_0 (platform.src[105], vit_graph4.in_rhs_0[0][1]);
connect<> net_rhs_in2_L4_0 (platform.src[106], vit_graph4.in_rhs_0[1][0]);
connect<> net_rhs_in3_L4_0 (platform.src[107], vit_graph4.in_rhs_0[1][1]);
connect<> net_rhs_in0_L4_1 (platform.src[108], vit_graph4.in_rhs_1[0][0]);
connect<> net_rhs_in1_L4_1 (platform.src[109], vit_graph4.in_rhs_1[0][1]);
connect<> net_rhs_in2_L4_1 (platform.src[110], vit_graph4.in_rhs_1[1][0]);
connect<> net_rhs_in3_L4_1 (platform.src[111], vit_graph4.in_rhs_1[1][1]);
connect<> net_rhs_in0_L4_2 (platform.src[112], vit_graph4.in_rhs_2[0][0]);
connect<> net_rhs_in1_L4_2 (platform.src[113], vit_graph4.in_rhs_2[0][1]);
connect<> net_rhs_in2_L4_2 (platform.src[114], vit_graph4.in_rhs_2[1][0]);
connect<> net_rhs_in3_L4_2 (platform.src[115], vit_graph4.in_rhs_2[1][1]);

connect<> net_out0_L0_0 (vit_graph0.out_0[0], platform.sink[0]);
connect<> net_out1_L0_0 (vit_graph0.out_0[1], platform.sink[1]);
connect<> net_out2_L0_0 (vit_graph0.out_0[2], platform.sink[2]);
connect<> net_out3_L0_0 (vit_graph0.out_0[3], platform.sink[3]);
connect<> net_out4_L0_0 (vit_graph0.out_0[4], platform.sink[4]);
connect<> net_out5_L0_0 (vit_graph0.out_0[5], platform.sink[5]);

connect<> net_out0_L1_0 (vit_graph1.out_0[0], platform.sink[6]);
connect<> net_out1_L1_0 (vit_graph1.out_0[1], platform.sink[7]);
connect<> net_out2_L1_0 (vit_graph1.out_0[2], platform.sink[8]);
connect<> net_out3_L1_0 (vit_graph1.out_0[3], platform.sink[9]);
connect<> net_out4_L1_0 (vit_graph1.out_0[4], platform.sink[10]);
connect<> net_out5_L1_0 (vit_graph1.out_0[5], platform.sink[11]);

connect<> net_out0_L2_0 (vit_graph2.out_0[0], platform.sink[12]);
connect<> net_out1_L2_0 (vit_graph2.out_0[1], platform.sink[13]);
connect<> net_out2_L2_0 (vit_graph2.out_0[2], platform.sink[14]);
connect<> net_out3_L2_0 (vit_graph2.out_0[3], platform.sink[15]);
connect<> net_out4_L2_0 (vit_graph2.out_0[4], platform.sink[16]);
connect<> net_out5_L2_0 (vit_graph2.out_0[5], platform.sink[17]);

connect<> net_out0_L3_0 (vit_graph3.out_0[0], platform.sink[18]);
connect<> net_out1_L3_0 (vit_graph3.out_0[1], platform.sink[19]);
connect<> net_out2_L3_0 (vit_graph3.out_0[2], platform.sink[20]);
connect<> net_out3_L3_0 (vit_graph3.out_0[3], platform.sink[21]);
connect<> net_out0_L3_1 (vit_graph3.out_1[0], platform.sink[22]);
connect<> net_out1_L3_1 (vit_graph3.out_1[1], platform.sink[23]);
connect<> net_out2_L3_1 (vit_graph3.out_1[2], platform.sink[24]);
connect<> net_out3_L3_1 (vit_graph3.out_1[3], platform.sink[25]);
connect<> net_out0_L3_2 (vit_graph3.out_2[0], platform.sink[26]);
connect<> net_out1_L3_2 (vit_graph3.out_2[1], platform.sink[27]);
connect<> net_out2_L3_2 (vit_graph3.out_2[2], platform.sink[28]);
connect<> net_out3_L3_2 (vit_graph3.out_2[3], platform.sink[29]);

connect<> net_out0_L4_0 (vit_graph4.out_0[0], platform.sink[30]);
connect<> net_out1_L4_0 (vit_graph4.out_0[1], platform.sink[31]);
connect<> net_out2_L4_0 (vit_graph4.out_0[2], platform.sink[32]);
connect<> net_out3_L4_0 (vit_graph4.out_0[3], platform.sink[33]);
connect<> net_out0_L4_1 (vit_graph4.out_1[0], platform.sink[34]);
connect<> net_out1_L4_1 (vit_graph4.out_1[1], platform.sink[35]);
connect<> net_out2_L4_1 (vit_graph4.out_1[2], platform.sink[36]);
connect<> net_out3_L4_1 (vit_graph4.out_1[3], platform.sink[37]);
connect<> net_out0_L4_2 (vit_graph4.out_2[0], platform.sink[38]);
connect<> net_out1_L4_2 (vit_graph4.out_2[1], platform.sink[39]);
connect<> net_out2_L4_2 (vit_graph4.out_2[2], platform.sink[40]);
connect<> net_out3_L4_2 (vit_graph4.out_2[3], platform.sink[41]);

connect<> net_out0_L5_0 (vit_graph5.out_0[0], platform.sink[42]);
connect<> net_out1_L5_0 (vit_graph5.out_0[1], platform.sink[43]);
connect<> net_out2_L5_0 (vit_graph5.out_0[2], platform.sink[44]);
connect<> net_out3_L5_0 (vit_graph5.out_0[3], platform.sink[45]);

connect<> net_out0_L6_0 (vit_graph6.out_0[0], platform.sink[46]);
connect<> net_out1_L6_0 (vit_graph6.out_0[1], platform.sink[47]);
connect<> net_out2_L6_0 (vit_graph6.out_0[2], platform.sink[48]);
connect<> net_out3_L6_0 (vit_graph6.out_0[3], platform.sink[49]);
connect<> net_out4_L6_0 (vit_graph6.out_0[4], platform.sink[50]);
connect<> net_out5_L6_0 (vit_graph6.out_0[5], platform.sink[51]);
connect<> net_out6_L6_0 (vit_graph6.out_0[6], platform.sink[52]);
connect<> net_out7_L6_0 (vit_graph6.out_0[7], platform.sink[53]);
connect<> net_out8_L6_0 (vit_graph6.out_0[8], platform.sink[54]);
connect<> net_out9_L6_0 (vit_graph6.out_0[9], platform.sink[55]);
connect<> net_out10_L6_0 (vit_graph6.out_0[10], platform.sink[56]);
connect<> net_out11_L6_0 (vit_graph6.out_0[11], platform.sink[57]);
connect<> net_out12_L6_0 (vit_graph6.out_0[12], platform.sink[58]);
connect<> net_out13_L6_0 (vit_graph6.out_0[13], platform.sink[59]);
connect<> net_out14_L6_0 (vit_graph6.out_0[14], platform.sink[60]);
connect<> net_out15_L6_0 (vit_graph6.out_0[15], platform.sink[61]);
connect<> net_out16_L6_0 (vit_graph6.out_0[16], platform.sink[62]);
connect<> net_out17_L6_0 (vit_graph6.out_0[17], platform.sink[63]);
connect<> net_out18_L6_0 (vit_graph6.out_0[18], platform.sink[64]);
connect<> net_out19_L6_0 (vit_graph6.out_0[19], platform.sink[65]);
connect<> net_out20_L6_0 (vit_graph6.out_0[20], platform.sink[66]);
connect<> net_out21_L6_0 (vit_graph6.out_0[21], platform.sink[67]);
connect<> net_out22_L6_0 (vit_graph6.out_0[22], platform.sink[68]);
connect<> net_out23_L6_0 (vit_graph6.out_0[23], platform.sink[69]);

connect<> net_out0_L7_0 (vit_graph7.out_0[0], platform.sink[70]);
connect<> net_out1_L7_0 (vit_graph7.out_0[1], platform.sink[71]);
connect<> net_out2_L7_0 (vit_graph7.out_0[2], platform.sink[72]);
connect<> net_out3_L7_0 (vit_graph7.out_0[3], platform.sink[73]);

connect<> net_out0_L8_0 (vit_graph8.out_0[0], platform.sink[74]);
connect<> net_out1_L8_0 (vit_graph8.out_0[1], platform.sink[75]);
connect<> net_out2_L8_0 (vit_graph8.out_0[2], platform.sink[76]);
connect<> net_out3_L8_0 (vit_graph8.out_0[3], platform.sink[77]);

connect<> net_out0_L9_0 (vit_graph9.out_0[0], platform.sink[78]);
connect<> net_out1_L9_0 (vit_graph9.out_0[1], platform.sink[79]);
connect<> net_out2_L9_0 (vit_graph9.out_0[2], platform.sink[80]);
connect<> net_out3_L9_0 (vit_graph9.out_0[3], platform.sink[81]);
connect<> net_out4_L9_0 (vit_graph9.out_0[4], platform.sink[82]);

#ifdef __AIESIM__
int main(int argc, char** argv) {
    vit_graph0.init();
    vit_graph1.init();
    vit_graph2.init();
    vit_graph3.init();
    vit_graph4.init();
    vit_graph5.init();
    vit_graph6.init();
    vit_graph7.init();
    vit_graph8.init();
    vit_graph9.init();
    
    vit_graph0.update(vit_graph0.vit_0_0_0.weights_offset,0);
    vit_graph0.update(vit_graph0.vit_0_1_0.weights_offset,0);
    vit_graph0.update(vit_graph0.vit_0_2_0.weights_offset,0);
    vit_graph0.update(vit_graph0.vit_0_3_0.weights_offset,0);
    vit_graph0.update(vit_graph0.vit_0_4_0.weights_offset,0);
    vit_graph0.update(vit_graph0.vit_0_5_0.weights_offset,0);
    
    vit_graph0.run(6);
    
    vit_graph1.update(vit_graph1.vit_0_0_0.weights_offset,0);
    vit_graph1.update(vit_graph1.vit_0_1_0.weights_offset,0);
    vit_graph1.update(vit_graph1.vit_0_2_0.weights_offset,0);
    vit_graph1.update(vit_graph1.vit_0_3_0.weights_offset,0);
    vit_graph1.update(vit_graph1.vit_0_4_0.weights_offset,0);
    vit_graph1.update(vit_graph1.vit_0_5_0.weights_offset,0);
    
    vit_graph1.run(6);
    
    vit_graph2.update(vit_graph2.vit_0_0_0.weights_offset,0);
    vit_graph2.update(vit_graph2.vit_0_1_0.weights_offset,0);
    vit_graph2.update(vit_graph2.vit_0_2_0.weights_offset,0);
    vit_graph2.update(vit_graph2.vit_0_3_0.weights_offset,0);
    vit_graph2.update(vit_graph2.vit_0_4_0.weights_offset,0);
    vit_graph2.update(vit_graph2.vit_0_5_0.weights_offset,0);
    
    vit_graph2.run(6);
    vit_graph3.run(9);
    vit_graph4.run(9);
    
    vit_graph5.update(vit_graph5.vit_0_0_0.weights_offset,0);
    vit_graph5.update(vit_graph5.vit_0_1_0.weights_offset,0);
    vit_graph5.update(vit_graph5.vit_0_2_0.weights_offset,0);
    vit_graph5.update(vit_graph5.vit_0_3_0.weights_offset,0);
    
    vit_graph5.run(6);
    
    vit_graph6.update(vit_graph6.vit_0_0_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_1_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_2_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_3_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_4_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_5_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_6_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_7_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_8_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_9_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_10_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_11_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_12_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_13_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_14_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_15_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_16_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_17_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_18_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_19_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_20_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_21_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_22_0.weights_offset,0);
    vit_graph6.update(vit_graph6.vit_0_23_0.weights_offset,0);
    
    vit_graph6.run(6);
    
    vit_graph7.update(vit_graph7.vit_0_0_0.weights_offset,0);
    vit_graph7.update(vit_graph7.vit_0_1_0.weights_offset,0);
    vit_graph7.update(vit_graph7.vit_0_2_0.weights_offset,0);
    vit_graph7.update(vit_graph7.vit_0_3_0.weights_offset,0);
    
    vit_graph7.run(6);
    vit_graph8.run(9);
    vit_graph9.run(9);
    vit_graph0.end();
    vit_graph1.end();
    vit_graph2.end();
    vit_graph3.end();
    vit_graph4.end();
    vit_graph5.end();
    vit_graph6.end();
    vit_graph7.end();
    vit_graph8.end();
    vit_graph9.end();
    return 0;
}
#endif