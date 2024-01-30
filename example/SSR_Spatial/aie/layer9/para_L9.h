#ifndef PARA_L9_H
#define PARA_L9_H
#include <adf/stream/types.h>
#define L9_NUM_KERNEL 2
const int L9_h1=2;
const int L9_w1=96;
const int L9_w2=200;
const int L9_size=L9_w1*L9_w2*1;
const int L9_boundry_j=L9_w2/8;
const int L9_jumpA0=32;
const int L9_jumpB0=32;

//H1=2 is zero
//matB: [W2/8][W1][8]  matA [W1/8][H1][8] matC [W2]


void mm7_kernel0_L9_B0_A0_C0(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L9_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L9_B0_A0_C1(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L9_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L9_B0_A0_C2(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L9_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L9_B0_A0_C3(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L9_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

void mm7_kernel0_L9_B0_A0_C4(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm7_kernel2_L9_B1_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int32* __restrict matC);

#endif