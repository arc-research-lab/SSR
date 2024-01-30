#ifndef PARA_L3_H
#define PARA_L3_H
#include <adf/stream/types.h>
#define L3_NUM_KERNEL 2
const int L3_h1=32;
const int L3_w1=32;
const int L3_w2=32;
const int L3_boundry_i=L3_h1/8;
const int L3_boundry_j=L3_w2/4;
const int L3_boundry_k=L3_w1/16-1;
const int L3_jumpA0=32;
const int L3_jumpA1=(L3_h1-8)*L3_w1;
const int L3_jumpA2=L3_jumpA1+L3_jumpA0;
const int L3_jumpB0=32;
const int L3_jumpB1=L3_w2*8;
const int L3_jumpB2=(L3_w1-8)*L3_w2-L3_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]
void mm1_kernel0_L3(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm1_kernel1_L3(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm1_kernel2_L3(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_int8 * __restrict matC);
void mm1_kernel3_L3(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_int8* __restrict matC);

#endif