#ifndef PARA_L4_H
#define PARA_L4_H
#include <adf/stream/types.h>
#define L4_NUM_KERNEL 2
const int L4_h1=32;
const int L4_w1=96;
const int L4_w2=32;
const int L4_boundry_i=L4_h1/8;
const int L4_boundry_j=L4_w2/4;
const int L4_boundry_k=L4_w1/16-1;
const int L4_jumpA0=32;
const int L4_jumpA1=(L4_h1-8)*L4_w1;
const int L4_jumpA2=L4_jumpA1+L4_jumpA0;
const int L4_jumpB0=32;
const int L4_jumpB1=L4_w2*8;
const int L4_jumpB2=(L4_w1-8)*L4_w2-L4_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]
void mm1_kernel0_L4(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm1_kernel1_L4(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm1_kernel2_L4(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_int8 * __restrict matC);
void mm1_kernel3_L4(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_int8* __restrict matC);

#endif