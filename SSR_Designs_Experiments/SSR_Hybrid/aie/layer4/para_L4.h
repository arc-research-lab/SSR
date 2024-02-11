#ifndef PARA_L4_H
#define PARA_L4_H
#include <adf/stream/types.h>
#define L4_NUM_KERNEL 2
const int L4_h1=2;
const int L4_w1=96;
const int L4_w2=200;
const int L4_size=L4_w1*L4_w2*1;
const int L4_boundry_j=L4_w2/8;
const int L4_jumpA0=32;
const int L4_jumpB0=32;

//H1=2 is zero
//matB: [W2/8][W1][8]  matA [W1/8][H1][8] matC [W2]


void mm7_kernel0_L4_B0_A0_C0(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L4_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L4_B0_A0_C1(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L4_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L4_B0_A0_C2(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L4_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L4_B0_A0_C3(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L4_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L4_B0_A0_C4(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L4_B1_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

#endif