#ifndef PARA_L1_H
#define PARA_L1_H
#include <adf/stream/types.h>
#define L1_NUM_KERNEL 4
const int L1_h1=32;
const int L1_w1=48;
const int L1_w2=32;
const int L1_size=L1_w1*L1_w2*12;
const int L1_boundry_i=L1_h1/8;
const int L1_boundry_j=L1_w2/4;
const int L1_boundry_k=L1_w1/16-1;
const int L1_jumpA0=32;
const int L1_jumpA1=(L1_h1-8)*L1_w1;
const int L1_jumpA2=L1_jumpA1+L1_jumpA0;
const int L1_jumpB0=32;
const int L1_jumpB1=L1_w2*8;
const int L1_jumpB2=(L1_w1-8)*L1_w2-L1_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]


void mm0_kernel0_L1_B0_A0_C0(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L1_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L1_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L1_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L1_B0_A0_C1(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L1_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L1_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L1_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L1_B0_A0_C2(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L1_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L1_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L1_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L1_B0_A0_C3(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L1_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L1_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L1_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L1_B0_A0_C4(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L1_B1_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L1_B2_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L1_B3_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L1_B0_A0_C5(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L1_B1_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L1_B2_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L1_B3_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

#endif