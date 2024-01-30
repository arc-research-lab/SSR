#ifndef _PL_ENGINE_HPP_
#define _PL_ENGINE_HPP_
#include <cstring>
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

#define DATA_TYPE 8
#define AXI_WIDTH_A 256
#define AXI_WIDTH_C 128
#define PLIO_WIDTH_128 128
#define PLIO_WIDTH_64 64
const int NUM_PER_DDR =(AXI_WIDTH_A/DATA_TYPE);
const int NUM_PER_128 =(PLIO_WIDTH_128/DATA_TYPE);
const int NUM_PER_64=(PLIO_WIDTH_64/DATA_TYPE);

typedef ap_uint<PLIO_WIDTH_128> data_plio_128;
typedef ap_axiu<PLIO_WIDTH_128,0,0,0> axis_pkt_128;
typedef hls::stream<axis_pkt_128> axis_stream_plio_128;

typedef ap_uint<PLIO_WIDTH_64> data_plio_64;
typedef ap_axiu<PLIO_WIDTH_64,0,0,0> axis_pkt_64;
typedef hls::stream<axis_pkt_64> axis_stream_plio_64;

typedef hls::stream<ap_uint<PLIO_WIDTH_128>> axis_stream_128;
typedef hls::stream<ap_uint<32>> axis_stream_32;
typedef hls::stream<ap_uint<AXI_WIDTH_A>> axis_stream_A;
typedef hls::stream<ap_uint<AXI_WIDTH_C>> axis_stream_C;

typedef struct{
    ap_uint<8> low0;
    ap_uint<8> low1;
    ap_uint<8> high0;
    ap_uint<8> high1;
 } comb_32;

// Definition for patch embedding
const int PE_SEQ_ALL=192;
const int PE_OC_ALL=192;
const int PE_SEQ=32;
const int PE_OC =32;
const int PE_A=1;
const int PE_INC=3;
const int PE_B=6;
const int PE_C=6;
const int PE_KHP=PE_B/PE_INC;
const int PE_K  =16;
const int PE_KH =16/PE_KHP;
const int PE_KW =PE_K;
const int PE_SEQ_Lane=4;
const int image_size=224;
const int PE_SEQ_H=image_size/PE_K;
const int PE_SEQ_W=image_size/PE_K;

// Definition for FC layer
const int FC_M=2;
const int FC_K=192;
const int FC_N=1000;
const int FC_H1=2;
const int FC_W1=96;
const int FC_W2=200;
const int FC_A=1;
const int FC_B=2;
const int FC_C=5;

// Definition for Acc0
const int ACC0_H1=32;     //single AIE workload
const int ACC0_W1=32;
const int ACC0_W2=32;
const int ACC0_A=6;     //Acc parallelism
const int ACC0_B=6;
const int ACC0_C=6;
const int ACC0_LH1=ACC0_H1/NUM_PER_64;
const int ACC0_LW1=ACC0_W1/(NUM_PER_128/NUM_PER_64);
const int ACC0_RW1=ACC0_W1/NUM_PER_64;
const int ACC0_RW2=ACC0_W2/(NUM_PER_128/NUM_PER_64);
const int ACC0_RW1_TRANS=ACC0_W1/(NUM_PER_128/NUM_PER_64);
const int ACC0_RW2_TRANS=ACC0_W2/NUM_PER_64;
const int PART0_LHS_A=1; //Acc Extra buffer partitioning
const int PART0_LHS_B=1;
const int PART0_RHS_B=1;
const int PART0_RHS_C=1;
const int BUFF0_LHS_PARTA=ACC0_A*PART0_LHS_A; //Total buffer banks
const int BUFF0_LHS_PARTB=ACC0_B*PART0_LHS_B;
const int BUFF0_RHS_PARTB=ACC0_B*PART0_RHS_B;
const int BUFF0_RHS_PARTC=ACC0_C*PART0_RHS_C;

const int BUFF0_DIM3 =MAX(BUFF0_LHS_PARTA,BUFF0_RHS_PARTB);
const int BUFF0_DIM2 =MAX(BUFF0_LHS_PARTB,BUFF0_RHS_PARTC);

const int BUF0_TYPE0_MAX1=192; //Total buffer size
const int BUF0_TYPE0_MAX0=768;

const int BUF0_TYPE0_DIM1=BUF0_TYPE0_MAX1/BUFF0_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF0_TYPE0_DIM0=BUF0_TYPE0_MAX0/BUFF0_DIM2/(NUM_PER_128/NUM_PER_64);

const int ACC0_M0 = 192; 
const int ACC0_K0 = 192;
const int ACC0_N0 = 192;
const int ACC0_M1 = 192; 
const int ACC0_K1 = 192;
const int ACC0_N1 = 192;
const int ACC0_M2 = 192; 
const int ACC0_K2 = 192;
const int ACC0_N2 = 192;
const int ACC0_M3 = 192; 
const int ACC0_K3 = 64;
const int ACC0_N3 = 192;
const int ACC0_M4 = 192; 
const int ACC0_K4 = 192;
const int ACC0_N4 = 64;
const int ACC0_M5 = 192; 
const int ACC0_K5 = 192;
const int ACC0_N5 = 192;
const int ACC0_M6 = 192; 
const int ACC0_K6 = 192;
const int ACC0_N6 = 768;
const int ACC0_M7 = 192; 
const int ACC0_K7 = 768;
const int ACC0_N7 = 192;

#endif
