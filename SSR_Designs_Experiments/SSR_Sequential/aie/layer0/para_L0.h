#ifndef PARA_L0_H
#define PARA_L0_H
#include <adf/stream/types.h>
#define L0_NUM_KERNEL 6
const int L0_h1=32;
const int L0_w1=32;
const int L0_w2=32;
const int L0_boundry_i=L0_h1/8;
const int L0_boundry_j=L0_w2/4;
const int L0_boundry_k=L0_w1/16-1;
const int L0_jumpA0=32;
const int L0_jumpA1=(L0_h1-8)*L0_w1;
const int L0_jumpA2=L0_jumpA1+L0_jumpA0;
const int L0_jumpB0=32;
const int L0_jumpB1=L0_w2*8;
const int L0_jumpB2=(L0_w1-8)*L0_w2-L0_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]
void mm1_kernel0_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_acc48* __restrict matC);
void mm1_kernel1_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm1_kernel2_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, input_stream_acc48* __restrict acc_in, output_stream_int8 * __restrict matC);
void mm1_kernel3_L0(input_window_int8* __restrict matA, input_window_int8*  __restrict matB, output_stream_int8* __restrict matC);

#endif