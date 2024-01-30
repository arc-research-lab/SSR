#ifndef PARA_L8_H
#define PARA_L8_H
#include <adf/stream/types.h>
#define L8_NUM_KERNEL 12

const int L8_KH =4;
const int L8_KW =16;
const int L8_OC =48;
const int L8_SEQ=32;
const int L8_size = L8_KH * L8_KW * L8_OC;

const int L8_bound_seq=L8_SEQ/4;
const int L8_bound_seq1=L8_SEQ/8;
const int L8_bound_oc =L8_OC/4;

const int L8_jumpA0=32;
const int L8_jumpA1=(L8_KH*L8_KW*4);
const int L8_jumpA2=L8_jumpA1-L8_jumpA0;

const int L8_jumpB0=32;
const int L8_jumpB3=(L8_KH*L8_KW*L8_OC);

const int L8_jumpC0=32;
const int L8_jumpC1=L8_OC*8-16;
const int L8_jumpC2=L8_jumpC1-16;


void mm3_kernel0_L8_B0_A0_C0(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L8_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B4_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B5_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B6_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B7_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B8_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B9_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B10_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L8_B11_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L8_B0_A0_C1(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L8_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B4_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B5_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B6_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B7_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B8_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B9_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B10_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L8_B11_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L8_B0_A0_C2(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L8_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B4_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B5_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B6_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B7_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B8_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B9_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B10_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L8_B11_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

void mm3_kernel0_L8_B0_A0_C3(input_window_int8*  __restrict matA, output_stream_acc48*  __restrict matC);
void mm3_kernel1_L8_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B4_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B5_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B6_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B7_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B8_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B9_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel1_L8_B10_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm3_kernel2_L8_B11_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_window_int8* __restrict matC);

#endif