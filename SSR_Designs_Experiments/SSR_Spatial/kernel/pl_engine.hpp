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
const int PE_OC =48;
const int PE_A=1;
const int PE_INC=3;
const int PE_B=12;
const int PE_C=4;
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
const int ACC0_M0 = 192; 
const int ACC0_K0 = 192;
const int ACC0_N0 = 192;
const int ACC0_HEADS=1;
const int ACC0_H1=32;     //single AIE workload
const int ACC0_W1=48;
const int ACC0_W2=32;
const int ACC0_A=1;     //Acc parallelism
const int ACC0_B=4;
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
const int PART0_OUT_A=2;
const int PART0_OUT_C=1;
const int BUFF0_LHS_PARTA=ACC0_A*PART0_LHS_A; //Total buffer banks
const int BUFF0_LHS_PARTB=ACC0_B*PART0_LHS_B;
const int BUFF0_RHS_PARTB=ACC0_B*PART0_RHS_B;
const int BUFF0_RHS_PARTC=ACC0_C*PART0_RHS_C;
const int BUFF0_OUT_PARTA=ACC0_A*PART0_OUT_A;
const int BUFF0_OUT_PARTC=ACC0_C*PART0_OUT_C;

const int BUFF0_DIM3 =MAX(BUFF0_LHS_PARTA,BUFF0_RHS_PARTB);
const int BUFF0_DIM2 =MAX(BUFF0_LHS_PARTB,BUFF0_RHS_PARTC);

const int BUF0_TYPE0_MAX1=192; //Total buffer size
const int BUF0_TYPE0_MAX0=192;

const int BUF0_TYPE0_DIM1=BUF0_TYPE0_MAX1/BUFF0_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF0_TYPE0_DIM0=BUF0_TYPE0_MAX0/BUFF0_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc1
const int ACC1_M0 = 192; 
const int ACC1_K0 = 192;
const int ACC1_N0 = 192;
const int ACC1_HEADS=1;
const int ACC1_H1=32;     //single AIE workload
const int ACC1_W1=48;
const int ACC1_W2=32;
const int ACC1_A=1;     //Acc parallelism
const int ACC1_B=4;
const int ACC1_C=6;
const int ACC1_LH1=ACC1_H1/NUM_PER_64;
const int ACC1_LW1=ACC1_W1/(NUM_PER_128/NUM_PER_64);
const int ACC1_RW1=ACC1_W1/NUM_PER_64;
const int ACC1_RW2=ACC1_W2/(NUM_PER_128/NUM_PER_64);
const int ACC1_RW1_TRANS=ACC1_W1/(NUM_PER_128/NUM_PER_64);
const int ACC1_RW2_TRANS=ACC1_W2/NUM_PER_64;
const int PART1_LHS_A=1; //Acc Extra buffer partitioning
const int PART1_LHS_B=1;
const int PART1_RHS_B=1;
const int PART1_RHS_C=1;
const int PART1_OUT_A=2;
const int PART1_OUT_C=1;
const int BUFF1_LHS_PARTA=ACC1_A*PART1_LHS_A; //Total buffer banks
const int BUFF1_LHS_PARTB=ACC1_B*PART1_LHS_B;
const int BUFF1_RHS_PARTB=ACC1_B*PART1_RHS_B;
const int BUFF1_RHS_PARTC=ACC1_C*PART1_RHS_C;
const int BUFF1_OUT_PARTA=ACC1_A*PART1_OUT_A;
const int BUFF1_OUT_PARTC=ACC1_C*PART1_OUT_C;

const int BUFF1_DIM3 =MAX(BUFF1_LHS_PARTA,BUFF1_RHS_PARTB);
const int BUFF1_DIM2 =MAX(BUFF1_LHS_PARTB,BUFF1_RHS_PARTC);

const int BUF1_TYPE0_MAX1=192; //Total buffer size
const int BUF1_TYPE0_MAX0=192;

const int BUF1_TYPE0_DIM1=BUF1_TYPE0_MAX1/BUFF1_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF1_TYPE0_DIM0=BUF1_TYPE0_MAX0/BUFF1_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc2
const int ACC2_M0 = 192; 
const int ACC2_K0 = 192;
const int ACC2_N0 = 192;
const int ACC2_HEADS=1;
const int ACC2_H1=32;     //single AIE workload
const int ACC2_W1=48;
const int ACC2_W2=32;
const int ACC2_A=1;     //Acc parallelism
const int ACC2_B=4;
const int ACC2_C=6;
const int ACC2_LH1=ACC2_H1/NUM_PER_64;
const int ACC2_LW1=ACC2_W1/(NUM_PER_128/NUM_PER_64);
const int ACC2_RW1=ACC2_W1/NUM_PER_64;
const int ACC2_RW2=ACC2_W2/(NUM_PER_128/NUM_PER_64);
const int ACC2_RW1_TRANS=ACC2_W1/(NUM_PER_128/NUM_PER_64);
const int ACC2_RW2_TRANS=ACC2_W2/NUM_PER_64;
const int PART2_LHS_A=1; //Acc Extra buffer partitioning
const int PART2_LHS_B=1;
const int PART2_RHS_B=1;
const int PART2_RHS_C=1;
const int PART2_OUT_A=2;
const int PART2_OUT_C=1;
const int BUFF2_LHS_PARTA=ACC2_A*PART2_LHS_A; //Total buffer banks
const int BUFF2_LHS_PARTB=ACC2_B*PART2_LHS_B;
const int BUFF2_RHS_PARTB=ACC2_B*PART2_RHS_B;
const int BUFF2_RHS_PARTC=ACC2_C*PART2_RHS_C;
const int BUFF2_OUT_PARTA=ACC2_A*PART2_OUT_A;
const int BUFF2_OUT_PARTC=ACC2_C*PART2_OUT_C;

const int BUFF2_DIM3 =MAX(BUFF2_LHS_PARTA,BUFF2_RHS_PARTB);
const int BUFF2_DIM2 =MAX(BUFF2_LHS_PARTB,BUFF2_RHS_PARTC);

const int BUF2_TYPE0_MAX1=192; //Total buffer size
const int BUF2_TYPE0_MAX0=192;

const int BUF2_TYPE0_DIM1=BUF2_TYPE0_MAX1/BUFF2_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF2_TYPE0_DIM0=BUF2_TYPE0_MAX0/BUFF2_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc3
const int ACC3_M0 = 192; 
const int ACC3_K0 = 64;
const int ACC3_N0 = 192;
const int ACC3_HEADS=3;
const int ACC3_H1=32;     //single AIE workload
const int ACC3_W1=32;
const int ACC3_W2=32;
const int ACC3_A=2;     //Acc parallelism
const int ACC3_B=2;
const int ACC3_C=2;
const int ACC3_LH1=ACC3_H1/NUM_PER_64;
const int ACC3_LW1=ACC3_W1/(NUM_PER_128/NUM_PER_64);
const int ACC3_RW1=ACC3_W1/NUM_PER_64;
const int ACC3_RW2=ACC3_W2/(NUM_PER_128/NUM_PER_64);
const int ACC3_RW1_TRANS=ACC3_W1/(NUM_PER_128/NUM_PER_64);
const int ACC3_RW2_TRANS=ACC3_W2/NUM_PER_64;
const int PART3_LHS_A=1; //Acc Extra buffer partitioning
const int PART3_LHS_B=1;
const int PART3_RHS_B=1;
const int PART3_RHS_C=1;
const int PART3_OUT_A=1;
const int PART3_OUT_C=1;
const int BUFF3_LHS_PARTA=ACC3_A*PART3_LHS_A; //Total buffer banks
const int BUFF3_LHS_PARTB=ACC3_B*PART3_LHS_B;
const int BUFF3_RHS_PARTB=ACC3_B*PART3_RHS_B;
const int BUFF3_RHS_PARTC=ACC3_C*PART3_RHS_C;
const int BUFF3_OUT_PARTA=ACC3_A*PART3_OUT_A;
const int BUFF3_OUT_PARTC=ACC3_C*PART3_OUT_C;

const int BUFF3_DIM3 =MAX(BUFF3_LHS_PARTA,BUFF3_RHS_PARTB);
const int BUFF3_DIM2 =MAX(BUFF3_LHS_PARTB,BUFF3_RHS_PARTC);

const int BUF3_TYPE0_MAX1=64; //Total buffer size
const int BUF3_TYPE0_MAX0=192;

const int BUF3_TYPE0_DIM1=BUF3_TYPE0_MAX1/BUFF3_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF3_TYPE0_DIM0=BUF3_TYPE0_MAX0/BUFF3_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc4
const int ACC4_M0 = 192; 
const int ACC4_K0 = 192;
const int ACC4_N0 = 64;
const int ACC4_HEADS=3;
const int ACC4_H1=32;     //single AIE workload
const int ACC4_W1=96;
const int ACC4_W2=32;
const int ACC4_A=2;     //Acc parallelism
const int ACC4_B=2;
const int ACC4_C=2;
const int ACC4_LH1=ACC4_H1/NUM_PER_64;
const int ACC4_LW1=ACC4_W1/(NUM_PER_128/NUM_PER_64);
const int ACC4_RW1=ACC4_W1/NUM_PER_64;
const int ACC4_RW2=ACC4_W2/(NUM_PER_128/NUM_PER_64);
const int ACC4_RW1_TRANS=ACC4_W1/(NUM_PER_128/NUM_PER_64);
const int ACC4_RW2_TRANS=ACC4_W2/NUM_PER_64;
const int PART4_LHS_A=1; //Acc Extra buffer partitioning
const int PART4_LHS_B=1;
const int PART4_RHS_B=1;
const int PART4_RHS_C=1;
const int PART4_OUT_A=1;
const int PART4_OUT_C=1;
const int BUFF4_LHS_PARTA=ACC4_A*PART4_LHS_A; //Total buffer banks
const int BUFF4_LHS_PARTB=ACC4_B*PART4_LHS_B;
const int BUFF4_RHS_PARTB=ACC4_B*PART4_RHS_B;
const int BUFF4_RHS_PARTC=ACC4_C*PART4_RHS_C;
const int BUFF4_OUT_PARTA=ACC4_A*PART4_OUT_A;
const int BUFF4_OUT_PARTC=ACC4_C*PART4_OUT_C;

const int BUFF4_DIM3 =MAX(BUFF4_LHS_PARTA,BUFF4_RHS_PARTB);
const int BUFF4_DIM2 =MAX(BUFF4_LHS_PARTB,BUFF4_RHS_PARTC);

const int BUF4_TYPE0_MAX1=192; //Total buffer size
const int BUF4_TYPE0_MAX0=64;

const int BUF4_TYPE0_DIM1=BUF4_TYPE0_MAX1/BUFF4_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF4_TYPE0_DIM0=BUF4_TYPE0_MAX0/BUFF4_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc5
const int ACC5_M0 = 192; 
const int ACC5_K0 = 192;
const int ACC5_N0 = 192;
const int ACC5_HEADS=1;
const int ACC5_H1=32;     //single AIE workload
const int ACC5_W1=32;
const int ACC5_W2=48;
const int ACC5_A=1;     //Acc parallelism
const int ACC5_B=6;
const int ACC5_C=4;
const int ACC5_LH1=ACC5_H1/NUM_PER_64;
const int ACC5_LW1=ACC5_W1/(NUM_PER_128/NUM_PER_64);
const int ACC5_RW1=ACC5_W1/NUM_PER_64;
const int ACC5_RW2=ACC5_W2/(NUM_PER_128/NUM_PER_64);
const int ACC5_RW1_TRANS=ACC5_W1/(NUM_PER_128/NUM_PER_64);
const int ACC5_RW2_TRANS=ACC5_W2/NUM_PER_64;
const int PART5_LHS_A=2; //Acc Extra buffer partitioning
const int PART5_LHS_B=1;
const int PART5_RHS_B=1;
const int PART5_RHS_C=1;
const int PART5_OUT_A=1;
const int PART5_OUT_C=1;
const int BUFF5_LHS_PARTA=ACC5_A*PART5_LHS_A; //Total buffer banks
const int BUFF5_LHS_PARTB=ACC5_B*PART5_LHS_B;
const int BUFF5_RHS_PARTB=ACC5_B*PART5_RHS_B;
const int BUFF5_RHS_PARTC=ACC5_C*PART5_RHS_C;
const int BUFF5_OUT_PARTA=ACC5_A*PART5_OUT_A;
const int BUFF5_OUT_PARTC=ACC5_C*PART5_OUT_C;

const int BUFF5_DIM3 =MAX(BUFF5_LHS_PARTA,BUFF5_RHS_PARTB);
const int BUFF5_DIM2 =MAX(BUFF5_LHS_PARTB,BUFF5_RHS_PARTC);

const int BUF5_TYPE0_MAX1=192; //Total buffer size
const int BUF5_TYPE0_MAX0=192;

const int BUF5_TYPE0_DIM1=BUF5_TYPE0_MAX1/BUFF5_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF5_TYPE0_DIM0=BUF5_TYPE0_MAX0/BUFF5_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc6
const int ACC6_M0 = 192; 
const int ACC6_K0 = 192;
const int ACC6_N0 = 768;
const int ACC6_HEADS=1;
const int ACC6_H1=32;     //single AIE workload
const int ACC6_W1=48;
const int ACC6_W2=32;
const int ACC6_A=1;     //Acc parallelism
const int ACC6_B=4;
const int ACC6_C=24;
const int ACC6_LH1=ACC6_H1/NUM_PER_64;
const int ACC6_LW1=ACC6_W1/(NUM_PER_128/NUM_PER_64);
const int ACC6_RW1=ACC6_W1/NUM_PER_64;
const int ACC6_RW2=ACC6_W2/(NUM_PER_128/NUM_PER_64);
const int ACC6_RW1_TRANS=ACC6_W1/(NUM_PER_128/NUM_PER_64);
const int ACC6_RW2_TRANS=ACC6_W2/NUM_PER_64;
const int PART6_LHS_A=1; //Acc Extra buffer partitioning
const int PART6_LHS_B=1;
const int PART6_RHS_B=1;
const int PART6_RHS_C=1;
const int PART6_OUT_A=1;
const int PART6_OUT_C=1;
const int BUFF6_LHS_PARTA=ACC6_A*PART6_LHS_A; //Total buffer banks
const int BUFF6_LHS_PARTB=ACC6_B*PART6_LHS_B;
const int BUFF6_RHS_PARTB=ACC6_B*PART6_RHS_B;
const int BUFF6_RHS_PARTC=ACC6_C*PART6_RHS_C;
const int BUFF6_OUT_PARTA=ACC6_A*PART6_OUT_A;
const int BUFF6_OUT_PARTC=ACC6_C*PART6_OUT_C;

const int BUFF6_DIM3 =MAX(BUFF6_LHS_PARTA,BUFF6_RHS_PARTB);
const int BUFF6_DIM2 =MAX(BUFF6_LHS_PARTB,BUFF6_RHS_PARTC);

const int BUF6_TYPE0_MAX1=192; //Total buffer size
const int BUF6_TYPE0_MAX0=768;

const int BUF6_TYPE0_DIM1=BUF6_TYPE0_MAX1/BUFF6_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF6_TYPE0_DIM0=BUF6_TYPE0_MAX0/BUFF6_DIM2/(NUM_PER_128/NUM_PER_64);



// Definition for Acc7
const int ACC7_M0 = 192; 
const int ACC7_K0 = 768;
const int ACC7_N0 = 192;
const int ACC7_HEADS=1;
const int ACC7_H1=32;     //single AIE workload
const int ACC7_W1=32;
const int ACC7_W2=48;
const int ACC7_A=1;     //Acc parallelism
const int ACC7_B=24;
const int ACC7_C=4;
const int ACC7_LH1=ACC7_H1/NUM_PER_64;
const int ACC7_LW1=ACC7_W1/(NUM_PER_128/NUM_PER_64);
const int ACC7_RW1=ACC7_W1/NUM_PER_64;
const int ACC7_RW2=ACC7_W2/(NUM_PER_128/NUM_PER_64);
const int ACC7_RW1_TRANS=ACC7_W1/(NUM_PER_128/NUM_PER_64);
const int ACC7_RW2_TRANS=ACC7_W2/NUM_PER_64;
const int PART7_LHS_A=1; //Acc Extra buffer partitioning
const int PART7_LHS_B=1;
const int PART7_RHS_B=1;
const int PART7_RHS_C=1;
const int PART7_OUT_A=1;
const int PART7_OUT_C=1;
const int BUFF7_LHS_PARTA=ACC7_A*PART7_LHS_A; //Total buffer banks
const int BUFF7_LHS_PARTB=ACC7_B*PART7_LHS_B;
const int BUFF7_RHS_PARTB=ACC7_B*PART7_RHS_B;
const int BUFF7_RHS_PARTC=ACC7_C*PART7_RHS_C;
const int BUFF7_OUT_PARTA=ACC7_A*PART7_OUT_A;
const int BUFF7_OUT_PARTC=ACC7_C*PART7_OUT_C;

const int BUFF7_DIM3 =MAX(BUFF7_LHS_PARTA,BUFF7_RHS_PARTB);
const int BUFF7_DIM2 =MAX(BUFF7_LHS_PARTB,BUFF7_RHS_PARTC);

const int BUF7_TYPE0_MAX1=192; //Total buffer size
const int BUF7_TYPE0_MAX0=768;

const int BUF7_TYPE0_DIM1=BUF7_TYPE0_MAX1/BUFF7_DIM3/NUM_PER_64; //Buffer size after partitioning
const int BUF7_TYPE0_DIM0=BUF7_TYPE0_MAX0/BUFF7_DIM2/(NUM_PER_128/NUM_PER_64);

#endif