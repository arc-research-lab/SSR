#ifndef PARA_L2_H
#define PARA_L2_H
#include <adf/stream/types.h>
#define L2_NUM_KERNEL 2
const int L2_h1=2;
const int L2_w1=96;
const int L2_w2=200;
const int L2_size=L2_w1*L2_w2*1;
const int L2_boundry_j=L2_w2/8;
const int L2_jumpA0=32;
const int L2_jumpB0=32;

//H1=2 is zero
//matB: [W2/8][W1][8]  matA [W1/8][H1][8] matC [W2]


void mm7_kernel0_L2_B0_A0_C0(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L2_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L2_B0_A0_C1(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L2_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L2_B0_A0_C2(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L2_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L2_B0_A0_C3(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L2_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L2_B0_A0_C4(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L2_B1_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

#endif