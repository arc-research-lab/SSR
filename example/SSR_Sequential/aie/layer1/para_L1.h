#ifndef PARA_L1_H
#define PARA_L1_H
#include <adf/stream/types.h>
#define L1_NUM_KERNEL 6

const int L1_KH =8;
const int L1_KW =16;
const int L1_OC =32;
const int L1_SEQ=32;
const int L1_size = L1_KH * L1_KW * L1_OC;

const int L1_bound_seq=L1_SEQ/4;
const int L1_bound_seq1=L1_SEQ/8;
const int L1_bound_oc =L1_OC/4;

const int L1_jumpA0=32;
const int L1_jumpA1=(L1_KH*L1_KW*4);
const int L1_jumpA2=L1_jumpA1-L1_jumpA0;

const int L1_jumpB0=32;
const int L1_jumpB3=(L1_KH*L1_KW*L1_OC);

const int L1_jumpC0=32;
const int L1_jumpC1=L1_OC*8-16;
const int L1_jumpC2=L1_jumpC1-16;


void mm3_kernel0_L1_B0_A0_C0(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L1_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B4_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L1_B5_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L1_B0_A0_C1(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L1_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B4_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L1_B5_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L1_B0_A0_C2(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L1_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B4_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L1_B5_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L1_B0_A0_C3(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L1_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B4_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L1_B5_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L1_B0_A0_C4(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L1_B1_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B2_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B3_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B4_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L1_B5_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L1_B0_A0_C5(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L1_B1_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B2_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B3_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L1_B4_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L1_B5_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

#endif