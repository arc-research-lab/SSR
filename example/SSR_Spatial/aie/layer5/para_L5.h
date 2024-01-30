#ifndef PARA_L5_H
#define PARA_L5_H
#include <adf/stream/types.h>
#define L5_NUM_KERNEL 6
const int L5_h1=32;
const int L5_w1=32;
const int L5_w2=48;
const int L5_size=L5_w1*L5_w2*12;
const int L5_boundry_i=L5_h1/8;
const int L5_boundry_j=L5_w2/4;
const int L5_boundry_k=L5_w1/16-1;
const int L5_jumpA0=32;
const int L5_jumpA1=(L5_h1-8)*L5_w1;
const int L5_jumpA2=L5_jumpA1+L5_jumpA0;
const int L5_jumpB0=32;
const int L5_jumpB1=L5_w2*8;
const int L5_jumpB2=(L5_w1-8)*L5_w2-L5_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]


void mm0_kernel0_L5_B0_A0_C0(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L5_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B4_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L5_B5_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L5_B0_A0_C1(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L5_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B4_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L5_B5_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L5_B0_A0_C2(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L5_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B4_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L5_B5_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L5_B0_A0_C3(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L5_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L5_B4_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L5_B5_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

#endif