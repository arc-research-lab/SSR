#ifndef PARA_L7_H
#define PARA_L7_H
#include <adf/stream/types.h>
#define L7_NUM_KERNEL 24
const int L7_h1=32;
const int L7_w1=32;
const int L7_w2=48;
const int L7_size=L7_w1*L7_w2*12;
const int L7_boundry_i=L7_h1/8;
const int L7_boundry_j=L7_w2/4;
const int L7_boundry_k=L7_w1/16-1;
const int L7_jumpA0=32;
const int L7_jumpA1=(L7_h1-8)*L7_w1;
const int L7_jumpA2=L7_jumpA1+L7_jumpA0;
const int L7_jumpB0=32;
const int L7_jumpB1=L7_w2*8;
const int L7_jumpB2=(L7_w1-8)*L7_w2-L7_jumpB0;

//matA: [H1/8][W1][8]  matB [W1/8][W2][8] matC [H1/8][W2][8]


void mm0_kernel0_L7_B0_A0_C0(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L7_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B4_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B5_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B6_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B7_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B8_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B9_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B10_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B11_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B12_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B13_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B14_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B15_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B16_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B17_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B18_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B19_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B20_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B21_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B22_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L7_B23_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L7_B0_A0_C1(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L7_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B4_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B5_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B6_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B7_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B8_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B9_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B10_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B11_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B12_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B13_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B14_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B15_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B16_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B17_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B18_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B19_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B20_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B21_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B22_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L7_B23_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L7_B0_A0_C2(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L7_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B4_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B5_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B6_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B7_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B8_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B9_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B10_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B11_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B12_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B13_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B14_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B15_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B16_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B17_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B18_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B19_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B20_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B21_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B22_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L7_B23_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L7_B0_A0_C3(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L7_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B4_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B5_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B6_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B7_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B8_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B9_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B10_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B11_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B12_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B13_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B14_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B15_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B16_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B17_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B18_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B19_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B20_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B21_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L7_B22_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L7_B23_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

#endif