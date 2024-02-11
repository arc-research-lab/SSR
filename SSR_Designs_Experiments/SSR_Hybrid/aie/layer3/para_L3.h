#ifndef PARA_L3_H
#define PARA_L3_H
#include <adf/stream/types.h>
#define L3_NUM_KERNEL 6

const int L3_KH =8;
const int L3_KW =16;
const int L3_OC =48;
const int L3_SEQ=32;
const int L3_size = L3_KH * L3_KW * L3_OC;

const int L3_bound_seq=L3_SEQ/4;
const int L3_bound_seq1=L3_SEQ/8;
const int L3_bound_oc =L3_OC/4;

const int L3_jumpA0=32;
const int L3_jumpA1=(L3_KH*L3_KW*4);
const int L3_jumpA2=L3_jumpA1-L3_jumpA0;

const int L3_jumpB0=32;
const int L3_jumpB3=(L3_KH*L3_KW*L3_OC);

const int L3_jumpC0=32;
const int L3_jumpC1=L3_OC*8-16;
const int L3_jumpC2=L3_jumpC1-16;


void mm3_kernel0_L3_B0_A0_C0(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L3_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B4_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L3_B5_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L3_B0_A0_C1(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L3_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B4_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L3_B5_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L3_B0_A0_C2(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L3_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B4_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L3_B5_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L3_B0_A0_C3(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L3_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L3_B4_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L3_B5_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

#endif