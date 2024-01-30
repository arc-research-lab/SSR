#ifndef PARA_L0_H
#define PARA_L0_H
#include <adf/stream/types.h>
#define L0_NUM_KERNEL 4
const int L0_h1=32;
const int L0_w1=48;
const int L0_w2=32;
const int L0_size=L0_w1*L0_w2*12;
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


void mm0_kernel0_L0_B0_A0_C0(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C0(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C1(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C1(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C2(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C2(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C3(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C3(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C4(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C4(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C5(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C5(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C6(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C6(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C6(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C6(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C7(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C7(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C7(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C7(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C8(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C8(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C8(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C8(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C9(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C9(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C9(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C9(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C10(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C10(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C10(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C10(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C11(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C11(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C11(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C11(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C12(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C12(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C12(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C12(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C13(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C13(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C13(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C13(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C14(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C14(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C14(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C14(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C15(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C15(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C15(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C15(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C16(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C16(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C16(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C16(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

void mm0_kernel0_L0_B0_A0_C17(input_window_int8*  __restrict matA, const int32 weights_offset, output_stream_acc48*  __restrict matC);
void mm0_kernel1_L0_B1_A0_C17(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel1_L0_B2_A0_C17(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_acc48* __restrict matC);
void mm0_kernel2_L0_B3_A0_C17(input_window_int8* __restrict matA, input_stream_acc48* __restrict acc_in, output_stream_int8* __restrict matC);

#endif