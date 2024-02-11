#ifndef PARA_L1_H
#define PARA_L1_H
#include <adf/stream/types.h>
#define L1_NUM_KERNEL 2
const int L1_h1=48;
const int L1_w1=32;
const int L1_w2=32;
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
void mm1_kernel0_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm1_kernel1_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm1_kernel2_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_int8 * __restrict matC);
void mm1_kernel3_L1(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_int8* __restrict matC);

#endif