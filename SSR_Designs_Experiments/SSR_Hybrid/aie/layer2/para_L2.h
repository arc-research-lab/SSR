#ifndef PARA_L2_H
#define PARA_L2_H
#include <adf/stream/types.h>
#define L2_NUM_KERNEL 6
const int L2_h1=48;
const int L2_w1=32;
const int L2_w2=32;
const int L2_boundry_i=L2_h1/8;
const int L2_boundry_j=L2_w2/4;
const int L2_boundry_k=L2_w1/16-1;
const int L2_jumpA0=32;
const int L2_jumpA1=(L2_h1-8)*L2_w1;
const int L2_jumpA2=L2_jumpA1+L2_jumpA0;
const int L2_jumpB0=32;
const int L2_jumpB1=L2_w2*8;
const int L2_jumpB2=(L2_w1-8)*L2_w2-L2_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]
void mm1_kernel0_L2(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm1_kernel1_L2(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm1_kernel2_L2(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_int8 * __restrict matC);
void mm1_kernel3_L2(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_int8* __restrict matC);

#endif