#include <stdint.h>
#include "pl_engine.hpp"


//int128 A[3][224][224] [IC][H][W]
template<int NC>
void address_image_ddr(axis_stream_32& addrA_out, const int batch, const int iteration) {
#pragma HLS inline off
    const int boundary = PE_SEQ_H*PE_SEQ_W*PE_K*PE_K*batch/NUM_PER_DDR;
    for(int j=0; j< iteration; j++){
        for(int i=0;i< boundary ; i++){
        #pragma HLS PIPELINE II = 1
            addrA_out.write(i);
        }        
    }
}

template<int NC>
void load_image_ddr(ap_uint<AXI_WIDTH_A>* ina, axis_stream_32& addrA_in,axis_stream_A& dataA_out, const int batch, const int iteration) {
#pragma HLS inline off
    ap_uint<AXI_WIDTH_A> temp_data;
    const int boundary = PE_SEQ_H*PE_SEQ_W*PE_K*PE_K*batch*iteration/NUM_PER_DDR;
    for(int i=0;i<boundary;i++){
    #pragma HLS PIPELINE II = 1
        int addr = addrA_in.read();
        temp_data=ina[addr];
        dataA_out.write(temp_data);      
    }
}

// Load image and prune the last four, 196-->192
template<int NC>
void load_image(axis_stream_A& dataA_in, ap_uint<PLIO_WIDTH_128> a_buf[PE_KHP][PE_KH][PE_SEQ_ALL], const bool enable){
#pragma HLS inline off
    if(enable){
        ap_uint<AXI_WIDTH_A> temp_data;
        for (int k = 0; k < PE_SEQ_H; k++) {
            for (int p = 0; p < PE_KHP; p++) {
                for (int j = 0; j < PE_KH; j++) {
                    for(int i=0;i<(PE_SEQ_W/2);i++){
                    #pragma HLS PIPELINE II = 1
                        temp_data=dataA_in.read();
                        int seq =i+k*(PE_SEQ_W/2);
                        int pos0=(seq%(PE_SEQ_ALL/2))*2;
                        if(seq<(PE_SEQ_ALL/2)){
                            a_buf[p][j][pos0]=temp_data(127,0);
                            a_buf[p][j][pos0+1]=temp_data(255,128);
                        }
                    }
                }
            }
        }
    }
}

void load_image_top(axis_stream_A& dataA_in0, axis_stream_A& dataA_in1, axis_stream_A& dataA_in2, ap_uint<PLIO_WIDTH_128> a_buf[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL],const bool enable){
    load_image<0>(dataA_in0, a_buf[0], enable);
    load_image<1>(dataA_in1, a_buf[1], enable);
    load_image<2>(dataA_in2, a_buf[2], enable);
}


void send_Patch(ap_uint<PLIO_WIDTH_128> a_buf[PE_KH][PE_SEQ_ALL],axis_stream_128& s0_out, const bool enable){
   #pragma HLS inline off 
   if(enable){
        data_plio_128 data;
        for (int k = 0; k < (PE_SEQ_ALL/PE_SEQ_Lane); k++) {
            for (int j = 0; j < PE_KH; j++) {
                for (int i = 0; i < PE_SEQ_Lane; i++) {
                #pragma HLS PIPELINE II = 1
                    int pos0= i + k*PE_SEQ_Lane;
                    data= a_buf[j][pos0];
                    s0_out.write(data);
                }
            }
        }
   }
}

void send_patch_transpose(axis_stream_plio_128& txA0, axis_stream_128& s0_in, const bool enable){
   #pragma HLS inline off 
    if(enable){
        ap_uint<8> data0[4][16];
        #pragma HLS ARRAY_PARTITION variable=data0 complete dim=1
        #pragma HLS ARRAY_PARTITION variable=data0 complete dim=2

        ap_uint<8> data1[4][16];
        #pragma HLS ARRAY_PARTITION variable=data1 complete dim=1
        #pragma HLS ARRAY_PARTITION variable=data1 complete dim=2

        data_plio_128 temp;
        axis_pkt_128 temp_data; 
        for (int i = 0; i < (PE_SEQ_ALL)*PE_KH+4; i++) {  
        #pragma HLS PIPELINE II = 1
            int pos0=i%4;
            int pos1=i/4;
            int pos=pos0*4;
            if(i<4){
                temp=s0_in.read();
                data0[pos0][0 ]= temp(7,0)    ;
                data0[pos0][1 ]= temp(15,8)   ;
                data0[pos0][2 ]= temp(23,16)  ;
                data0[pos0][3 ]= temp(31,24)  ;
                data0[pos0][4 ]= temp(39,32)  ;
                data0[pos0][5 ]= temp(47,40)  ;
                data0[pos0][6 ]= temp(55,48)  ;
                data0[pos0][7 ]= temp(63,56)  ;
                data0[pos0][8 ]= temp(71,64)  ;
                data0[pos0][9 ]= temp(79,72)  ;
                data0[pos0][10]= temp(87,80)  ;
                data0[pos0][11]= temp(95,88)  ;
                data0[pos0][12]= temp(103,96) ;
                data0[pos0][13]= temp(111,104);
                data0[pos0][14]= temp(119,112);
                data0[pos0][15]= temp(127,120);
            }
            else if (i>=(PE_SEQ_ALL)*PE_KH){
                temp_data.data(7,0)     = data1[0][pos+0];
                temp_data.data(15,8)    = data1[1][pos+0];
                temp_data.data(23,16)   = data1[2][pos+0];
                temp_data.data(31,24)   = data1[3][pos+0];
                temp_data.data(39,32)   = data1[0][pos+1];
                temp_data.data(47,40)   = data1[1][pos+1];
                temp_data.data(55,48)   = data1[2][pos+1];
                temp_data.data(63,56)   = data1[3][pos+1];
                temp_data.data(71,64)   = data1[0][pos+2];
                temp_data.data(79,72)   = data1[1][pos+2];
                temp_data.data(87,80)   = data1[2][pos+2];
                temp_data.data(95,88)   = data1[3][pos+2];
                temp_data.data(103,96)  = data1[0][pos+3];
                temp_data.data(111,104) = data1[1][pos+3];
                temp_data.data(119,112) = data1[2][pos+3];
                temp_data.data(127,120) = data1[3][pos+3];
                txA0.write(temp_data);
            }
            else if(pos1%2==1){
                temp=s0_in.read();
                data1[pos0][0 ]= temp(7,0)    ;
                data1[pos0][1 ]= temp(15,8)   ;
                data1[pos0][2 ]= temp(23,16)  ;
                data1[pos0][3 ]= temp(31,24)  ;
                data1[pos0][4 ]= temp(39,32)  ;
                data1[pos0][5 ]= temp(47,40)  ;
                data1[pos0][6 ]= temp(55,48)  ;
                data1[pos0][7 ]= temp(63,56)  ;
                data1[pos0][8 ]= temp(71,64)  ;
                data1[pos0][9 ]= temp(79,72)  ;
                data1[pos0][10]= temp(87,80)  ;
                data1[pos0][11]= temp(95,88)  ;
                data1[pos0][12]= temp(103,96) ;
                data1[pos0][13]= temp(111,104);
                data1[pos0][14]= temp(119,112);
                data1[pos0][15]= temp(127,120);
                temp_data.data(7,0)     = data0[0][pos+0];
                temp_data.data(15,8)    = data0[1][pos+0];
                temp_data.data(23,16)   = data0[2][pos+0];
                temp_data.data(31,24)   = data0[3][pos+0];
                temp_data.data(39,32)   = data0[0][pos+1];
                temp_data.data(47,40)   = data0[1][pos+1];
                temp_data.data(55,48)   = data0[2][pos+1];
                temp_data.data(63,56)   = data0[3][pos+1];
                temp_data.data(71,64)   = data0[0][pos+2];
                temp_data.data(79,72)   = data0[1][pos+2];
                temp_data.data(87,80)   = data0[2][pos+2];
                temp_data.data(95,88)   = data0[3][pos+2];
                temp_data.data(103,96)  = data0[0][pos+3];
                temp_data.data(111,104) = data0[1][pos+3];
                temp_data.data(119,112) = data0[2][pos+3];
                temp_data.data(127,120) = data0[3][pos+3];
                txA0.write(temp_data);
            }
            else{
                temp=s0_in.read();
                data0[pos0][0 ]= temp(7,0)    ;
                data0[pos0][1 ]= temp(15,8)   ;
                data0[pos0][2 ]= temp(23,16)  ;
                data0[pos0][3 ]= temp(31,24)  ;
                data0[pos0][4 ]= temp(39,32)  ;
                data0[pos0][5 ]= temp(47,40)  ;
                data0[pos0][6 ]= temp(55,48)  ;
                data0[pos0][7 ]= temp(63,56)  ;
                data0[pos0][8 ]= temp(71,64)  ;
                data0[pos0][9 ]= temp(79,72)  ;
                data0[pos0][10]= temp(87,80)  ;
                data0[pos0][11]= temp(95,88)  ;
                data0[pos0][12]= temp(103,96) ;
                data0[pos0][13]= temp(111,104);
                data0[pos0][14]= temp(119,112);
                data0[pos0][15]= temp(127,120);
                temp_data.data(7,0)     = data1[0][pos+0];
                temp_data.data(15,8)    = data1[1][pos+0];
                temp_data.data(23,16)   = data1[2][pos+0];
                temp_data.data(31,24)   = data1[3][pos+0];
                temp_data.data(39,32)   = data1[0][pos+1];
                temp_data.data(47,40)   = data1[1][pos+1];
                temp_data.data(55,48)   = data1[2][pos+1];
                temp_data.data(63,56)   = data1[3][pos+1];
                temp_data.data(71,64)   = data1[0][pos+2];
                temp_data.data(79,72)   = data1[1][pos+2];
                temp_data.data(87,80)   = data1[2][pos+2];
                temp_data.data(95,88)   = data1[3][pos+2];
                temp_data.data(103,96)  = data1[0][pos+3];
                temp_data.data(111,104) = data1[1][pos+3];
                temp_data.data(119,112) = data1[2][pos+3];
                temp_data.data(127,120) = data1[3][pos+3];
                txA0.write(temp_data);
            }
        }
    }
}

template<int NC>
void send_patch_wrapper0(ap_uint<PLIO_WIDTH_128> a_buf[PE_KH][PE_SEQ_ALL], axis_stream_plio_128& txA0,const bool enable){
#pragma HLS dataflow
    axis_stream_128 s0;
    send_Patch(a_buf,s0, enable);
    send_patch_transpose(txA0,s0,enable);
}

template<int NC>
void receive_Patch_transpose(axis_stream_plio_128& rxC0, axis_stream_128& s_out0, const bool enable){
#pragma HLS inline off 
    if(enable){
        axis_pkt_128 tmp0;
        data_plio_128 data_out0;
        ap_uint<32> data0_0[2][4];
        #pragma HLS ARRAY_PARTITION variable=data0_0 complete dim=1
        #pragma HLS ARRAY_PARTITION variable=data0_0 complete dim=2

        ap_uint<32> data1_0[2][4];
        #pragma HLS ARRAY_PARTITION variable=data1_0 complete dim=1
        #pragma HLS ARRAY_PARTITION variable=data1_0 complete dim=2
        for (int i = 0; i < (PE_SEQ_ALL*PE_OC_ALL/PE_C/NUM_PER_128)+2; i++) {
        #pragma HLS PIPELINE II = 1
            int pos0=i%2;
            int pos1=i/2;
            int pos=pos0*2;
            if (i<2){
                tmp0=rxC0.read();
                data0_0[pos0][0]=tmp0.data(31,0);
                data0_0[pos0][1]=tmp0.data(63,32);
                data0_0[pos0][2]=tmp0.data(95,64);
                data0_0[pos0][3]=tmp0.data(127,96);
            }
            else if(i>=(PE_SEQ_ALL*PE_OC_ALL/PE_C/NUM_PER_128)){
                data_out0(31,0)  = data1_0[0][0+pos];  
                data_out0(63,32) = data1_0[1][0+pos];
                data_out0(95,64) = data1_0[0][1+pos];
                data_out0(127,96)= data1_0[1][1+pos];
                s_out0.write(data_out0);
            }
            else if(pos1%2==0){
                tmp0=rxC0.read();
                data0_0[pos0][0]=tmp0.data(31,0);
                data0_0[pos0][1]=tmp0.data(63,32);
                data0_0[pos0][2]=tmp0.data(95,64);
                data0_0[pos0][3]=tmp0.data(127,96);
                data_out0(31,0)  = data1_0[0][0+pos];  
                data_out0(63,32) = data1_0[1][0+pos];
                data_out0(95,64) = data1_0[0][1+pos];
                data_out0(127,96)= data1_0[1][1+pos];
                s_out0.write(data_out0);
            }
            else{
                tmp0=rxC0.read();
                data1_0[pos0][0]=tmp0.data(31,0);
                data1_0[pos0][1]=tmp0.data(63,32);
                data1_0[pos0][2]=tmp0.data(95,64);
                data1_0[pos0][3]=tmp0.data(127,96);
                data_out0(31,0)  = data0_0[0][0+pos];  
                data_out0(63,32) = data0_0[1][0+pos];
                data_out0(95,64) = data0_0[0][1+pos];
                data_out0(127,96)= data0_0[1][1+pos];
                s_out0.write(data_out0);
            }
        }
    }
}

void address_C_ddr(axis_stream_32& addrC_out, const int batch, const int iteration) {
#pragma HLS inline off
    const int boundary = batch * (FC_N/4);
    for(int j=0; j< iteration; j++){
        for(int i=0;i<boundary;i++){
        #pragma HLS PIPELINE II = 1
            addrC_out.write(i);
        } 
    }
}

void storeC_ddr(ap_uint<AXI_WIDTH_C>* outc, axis_stream_32& addrC_in,axis_stream_C& dataC_in, const int batch, const int iteration) { 
#pragma HLS inline off
    const int boundary = batch * iteration * (FC_N/4);

    for (int i = 0; i < boundary; i++) {
    #pragma HLS PIPELINE II = 1
        ap_uint<AXI_WIDTH_C> temp_data=dataC_in.read();
        ap_uint<32> addr = addrC_in.read();
        outc[addr]=temp_data;
    }
    
}

template<int NC>
void Acc0_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf_lhs0_0[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_h1, const int buffer_w1,
                                      axis_stream_plio_128& Acc0_txA0, 
                                      const bool LHS_bypass, const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;

        for (int x = 0; x < boundary_x; x++) { 
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC0_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC0_LW1)%buffer_w1;
                            int pos1=(h1+x*ACC0_LH1)%buffer_h1;
                            int temp_b=(w1+y*ACC0_LW1)%buffer_w1;
                            int temp_a=(h1+x*ACC0_LH1)%buffer_h1;
                            if(LHS_bypass){
                                tmp.data  =  ap_uint<128>(0);
                            }
                            else{
                                data = buf_lhs0_0[pos1][pos0];
                                
                            }
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc0_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc0_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_lhs[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], 
                                          const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_h1, const int buffer_w1,
                                          axis_stream_plio_128& Acc0_txA0_0, axis_stream_plio_128& Acc0_txA0_1, axis_stream_plio_128& Acc0_txA0_2, axis_stream_plio_128& Acc0_txA0_3, axis_stream_plio_128& Acc0_txA0_4, axis_stream_plio_128& Acc0_txA0_5,  
                                          axis_stream_plio_128& Acc0_txA1_0, axis_stream_plio_128& Acc0_txA1_1, axis_stream_plio_128& Acc0_txA1_2, axis_stream_plio_128& Acc0_txA1_3, axis_stream_plio_128& Acc0_txA1_4, axis_stream_plio_128& Acc0_txA1_5,  
                                          axis_stream_plio_128& Acc0_txA2_0, axis_stream_plio_128& Acc0_txA2_1, axis_stream_plio_128& Acc0_txA2_2, axis_stream_plio_128& Acc0_txA2_3, axis_stream_plio_128& Acc0_txA2_4, axis_stream_plio_128& Acc0_txA2_5,  
                                          axis_stream_plio_128& Acc0_txA3_0, axis_stream_plio_128& Acc0_txA3_1, axis_stream_plio_128& Acc0_txA3_2, axis_stream_plio_128& Acc0_txA3_3, axis_stream_plio_128& Acc0_txA3_4, axis_stream_plio_128& Acc0_txA3_5,  
                                          axis_stream_plio_128& Acc0_txA4_0, axis_stream_plio_128& Acc0_txA4_1, axis_stream_plio_128& Acc0_txA4_2, axis_stream_plio_128& Acc0_txA4_3, axis_stream_plio_128& Acc0_txA4_4, axis_stream_plio_128& Acc0_txA4_5,  
                                          axis_stream_plio_128& Acc0_txA5_0, axis_stream_plio_128& Acc0_txA5_1, axis_stream_plio_128& Acc0_txA5_2, axis_stream_plio_128& Acc0_txA5_3, axis_stream_plio_128& Acc0_txA5_4, axis_stream_plio_128& Acc0_txA5_5,  
                                          const bool LHS_bypass0, const bool LHS_bypass1, const bool LHS_bypass2, const bool enable){
     
    Acc0_send_LHS_type0<0>(buf_lhs[0],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA0_0,LHS_bypass0,enable);
    Acc0_send_LHS_type0<1>(buf_lhs[1],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA0_1,LHS_bypass0,enable);
    Acc0_send_LHS_type0<2>(buf_lhs[2],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA0_2,LHS_bypass1,enable);
    Acc0_send_LHS_type0<3>(buf_lhs[3],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA0_3,LHS_bypass1,enable);
    Acc0_send_LHS_type0<4>(buf_lhs[4],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA0_4,LHS_bypass2,enable);
    Acc0_send_LHS_type0<5>(buf_lhs[5],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA0_5,LHS_bypass2,enable);
    Acc0_send_LHS_type0<6>(buf_lhs[6],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA1_0,LHS_bypass0,enable);
    Acc0_send_LHS_type0<7>(buf_lhs[7],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA1_1,LHS_bypass0,enable);
    Acc0_send_LHS_type0<8>(buf_lhs[8],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA1_2,LHS_bypass1,enable);
    Acc0_send_LHS_type0<9>(buf_lhs[9],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA1_3,LHS_bypass1,enable);
    Acc0_send_LHS_type0<10>(buf_lhs[10],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA1_4,LHS_bypass2,enable);
    Acc0_send_LHS_type0<11>(buf_lhs[11],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA1_5,LHS_bypass2,enable);
    Acc0_send_LHS_type0<12>(buf_lhs[12],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA2_0,LHS_bypass0,enable);
    Acc0_send_LHS_type0<13>(buf_lhs[13],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA2_1,LHS_bypass0,enable);
    Acc0_send_LHS_type0<14>(buf_lhs[14],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA2_2,LHS_bypass1,enable);
    Acc0_send_LHS_type0<15>(buf_lhs[15],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA2_3,LHS_bypass1,enable);
    Acc0_send_LHS_type0<16>(buf_lhs[16],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA2_4,LHS_bypass2,enable);
    Acc0_send_LHS_type0<17>(buf_lhs[17],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA2_5,LHS_bypass2,enable);
    Acc0_send_LHS_type0<18>(buf_lhs[18],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA3_0,LHS_bypass0,enable);
    Acc0_send_LHS_type0<19>(buf_lhs[19],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA3_1,LHS_bypass0,enable);
    Acc0_send_LHS_type0<20>(buf_lhs[20],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA3_2,LHS_bypass1,enable);
    Acc0_send_LHS_type0<21>(buf_lhs[21],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA3_3,LHS_bypass1,enable);
    Acc0_send_LHS_type0<22>(buf_lhs[22],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA3_4,LHS_bypass2,enable);
    Acc0_send_LHS_type0<23>(buf_lhs[23],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA3_5,LHS_bypass2,enable);
    Acc0_send_LHS_type0<24>(buf_lhs[24],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA4_0,LHS_bypass0,enable);
    Acc0_send_LHS_type0<25>(buf_lhs[25],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA4_1,LHS_bypass0,enable);
    Acc0_send_LHS_type0<26>(buf_lhs[26],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA4_2,LHS_bypass1,enable);
    Acc0_send_LHS_type0<27>(buf_lhs[27],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA4_3,LHS_bypass1,enable);
    Acc0_send_LHS_type0<28>(buf_lhs[28],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA4_4,LHS_bypass2,enable);
    Acc0_send_LHS_type0<29>(buf_lhs[29],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA4_5,LHS_bypass2,enable);
    Acc0_send_LHS_type0<30>(buf_lhs[30],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA5_0,LHS_bypass0,enable);
    Acc0_send_LHS_type0<31>(buf_lhs[31],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA5_1,LHS_bypass0,enable);
    Acc0_send_LHS_type0<32>(buf_lhs[32],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA5_2,LHS_bypass1,enable);
    Acc0_send_LHS_type0<33>(buf_lhs[33],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA5_3,LHS_bypass1,enable);
    Acc0_send_LHS_type0<34>(buf_lhs[34],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA5_4,LHS_bypass2,enable);
    Acc0_send_LHS_type0<35>(buf_lhs[35],boundary_x, boundary_y, boundary_z, buffer_h1, buffer_w1,Acc0_txA5_5,LHS_bypass2,enable);
      
}

template<int NC, int TEMP_B, int TEMP_C, int TEMP_B_New, int TEMP_C_New>
void Acc0_send_buff_type0(ap_uint<PLIO_WIDTH_128> buf_rhs[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2,axis_stream_128& Acc0_rhs0,axis_stream_128& Acc0_rhs1,const int sel, const bool para_sel, const bool RHS_bypass_type0, const bool RHS_bypass_type1, const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        if (sel == 0){
            for (int x = 0; x < boundary_x; x++) {
                for (int z = 0; z < boundary_z; z++) {
                    for (int y = 0; y < boundary_y; y++) {
                        for (int w1 = 0; w1 < ACC0_RW1; w1++) {
                            for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                            #pragma HLS PIPELINE II = 1
                                int pos0=(w2+z*ACC0_RW2)%buffer_w2;
                                int pos1=(w1+y*ACC0_RW1)%buffer_w1;
                                int temp_c=(w2+z*ACC0_RW2)/buffer_w2;
                                int temp_b=(w1+y*ACC0_RW1)/buffer_w1;
                                if (temp_b==TEMP_B&&temp_c==TEMP_C){
                                    if (RHS_bypass_type0){
                                        data = ap_uint<128>(0);
                                    }
                                    else if (para_sel==1){
                                        data = (ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1));
                                    }
                                    else{
                                        data = buf_rhs[pos1][pos0];
                                    }
                                    Acc0_rhs0.write(data);
                                }
                            }
                        }
                    }
                }   
            }
        }
        else{
            for (int x = 0; x < boundary_x; x++) {
                for (int z = 0; z < boundary_z; z++) {
                    for (int y = 0; y < boundary_y; y++) {
                        for (int w1 = 0; w1 < ACC0_RW1_TRANS/4; w1++) {
                            for (int w2 = 0; w2 < ACC0_RW2_TRANS; w2++) {
                                for (int i = 0; i < 4; i++) {
                                #pragma HLS PIPELINE II = 1
                                    int pos0 = (w2 + z * ACC0_RW2_TRANS) % buffer_w2;
                                    int pos1 = (i + w1 * 4 + y * (ACC0_RW1_TRANS * 4)) % buffer_w1;
                                    int temp_c = (w2 + z * ACC0_RW2_TRANS) / buffer_w2;
                                    int temp_b = (i + w1 * 4 + y * (ACC0_RW1_TRANS * 4)) / buffer_w1;
                                    if (temp_b==TEMP_B_New&&temp_c==TEMP_C_New){
                                        if (RHS_bypass_type1){
                                            data = ap_uint<128>(0);
                                        }
                                        else if (para_sel==1){
                                            data = (ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1));
                                        }
                                        else{
                                            data = buf_rhs[pos0][pos1];
                                        }
                                        Acc0_rhs1.write(data);
                                    }
                                }
                            }
                        }
                    }
                }   
            }
        }
        
    }
}

//Here the split is for extra partitions
template<int NC>
void Acc0_merge0(axis_stream_128& Acc0_sin0_0,const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2, axis_stream_128& Acc0_sout, const bool enable){

#pragma HLS inline off

    if(enable){

        data_plio_128 data;

        for (int x = 0; x < boundary_x; x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {          
                    for (int w1 = 0; w1 < ACC0_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                            int temp_c=(w2+z*ACC0_RW2)/buffer_w2;
                            int temp_b=(w1+y*ACC0_RW1)/buffer_w1;

                            data = Acc0_sin0_0.read();
                            Acc0_sout.write(data);
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void Acc0_merge1(axis_stream_128& Acc0_sin0_0,const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2, axis_stream_128& Acc0_sout, const bool enable){

#pragma HLS inline off

    if(enable){

        data_plio_128 data;

        for (int x = 0; x < boundary_x; x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int w1 = 0; w1 < ACC0_RW1_TRANS/4; w1++) {
                        for (int w2 = 0; w2 < ACC0_RW2_TRANS; w2++) {
                            for (int i = 0; i < 4; i++) {
                            #pragma HLS PIPELINE II = 1
                                int temp_c = (w2 + z * ACC0_RW2_TRANS) / buffer_w2;
                                int temp_b = (i + w1 * 4 + y * (ACC0_RW1_TRANS * 4)) / buffer_w1;

                                data = Acc0_sin0_0.read();
                                Acc0_sout.write(data);
                            }
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void Acc0_transpose(axis_stream_128& Acc0_sin, const int boundary_x, const int boundary_y, const int boundary_z,
                          axis_stream_128& Acc0_sout, const bool enable){

#pragma HLS inline off
    if(enable){
        data_plio_128 temp_data;
        data_plio_128 data;

        ap_uint<8> data0[NUM_PER_64][NUM_PER_64];
        #pragma HLS ARRAY_PARTITION variable=data0 complete dim=0

        ap_uint<8> data1[NUM_PER_64][NUM_PER_64];
        #pragma HLS ARRAY_PARTITION variable=data1 complete dim=0

        int total_bound = boundary_y * boundary_z * ACC0_RW1 * ACC0_RW2;
        
        for (int x = 0; x < boundary_x; x++) {
            for (int i = 0; i < (total_bound+4); i++) {  
            #pragma HLS PIPELINE II = 1
                int pos0=i%4;
                int pos1=i/4;
                int pos=pos0*2;
                if(i<4){
                    data=Acc0_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data0[un%8][pos+(un/8)]= data(7+un*8,un*8);
                    }
                }
                else if (i>=total_bound){
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        temp_data(7+un*8,un*8) = data1[pos+(un/8)][(un%8)];
                    }
                    Acc0_sout.write(temp_data);
                }
                else if(pos1%2==1){
                    data=Acc0_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data1[un%8][pos+(un/8)]= data(7+un*8,un*8);
                        temp_data(7+un*8,un*8) = data0[pos+(un/8)][(un%8)];
                    }
                    Acc0_sout.write(temp_data);
                }
                else{
                    data=Acc0_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data0[un%8][pos+(un/8)]= data(7+un*8,un*8);
                        temp_data(7+un*8,un*8) = data1[pos+(un/8)][(un%8)];
                    }
                    Acc0_sout.write(temp_data);
                }
            }
        }
    }
}

template<int NC>
void Acc0_send_RHS(axis_stream_128& Acc0_sin0,axis_stream_128& Acc0_sin1,const int boundary_x, const int boundary_y, const int boundary_z, 
                          axis_stream_plio_128& Acc0_txB0, axis_stream_plio_128& Acc0_txB1, const int sel, const bool enable){
#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;

        for (int x = 0; x < boundary_x; x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int w1 = 0; w1 < ACC0_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                            
                            if (sel==0){
                                data = Acc0_sin0.read();
                            }
                            else{
                                data = Acc0_sin1.read();
                            }
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc0_txB0.write(tmp);
                            Acc0_txB1.write(tmp);
                            
                        }
                    }
                }
            }
        }
    }
}


template<int NC>
void Acc0_send_RHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_rhs[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], 
            const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2,
            axis_stream_plio_128& Acc0_txB0_0, axis_stream_plio_128& Acc0_txB1_0, axis_stream_plio_128& Acc0_txB2_0, axis_stream_plio_128& Acc0_txB3_0, axis_stream_plio_128& Acc0_txB4_0, axis_stream_plio_128& Acc0_txB5_0,  
            axis_stream_plio_128& Acc0_txB0_1, axis_stream_plio_128& Acc0_txB1_1, axis_stream_plio_128& Acc0_txB2_1, axis_stream_plio_128& Acc0_txB3_1, axis_stream_plio_128& Acc0_txB4_1, axis_stream_plio_128& Acc0_txB5_1,  
            axis_stream_plio_128& Acc0_txB0_2, axis_stream_plio_128& Acc0_txB1_2, axis_stream_plio_128& Acc0_txB2_2, axis_stream_plio_128& Acc0_txB3_2, axis_stream_plio_128& Acc0_txB4_2, axis_stream_plio_128& Acc0_txB5_2,  
            axis_stream_plio_128& Acc0_txB0_3, axis_stream_plio_128& Acc0_txB1_3, axis_stream_plio_128& Acc0_txB2_3, axis_stream_plio_128& Acc0_txB3_3, axis_stream_plio_128& Acc0_txB4_3, axis_stream_plio_128& Acc0_txB5_3,  
            axis_stream_plio_128& Acc0_txB0_4, axis_stream_plio_128& Acc0_txB1_4, axis_stream_plio_128& Acc0_txB2_4, axis_stream_plio_128& Acc0_txB3_4, axis_stream_plio_128& Acc0_txB4_4, axis_stream_plio_128& Acc0_txB5_4,  
            axis_stream_plio_128& Acc0_txB0_5, axis_stream_plio_128& Acc0_txB1_5, axis_stream_plio_128& Acc0_txB2_5, axis_stream_plio_128& Acc0_txB3_5, axis_stream_plio_128& Acc0_txB4_5, axis_stream_plio_128& Acc0_txB5_5,  
            axis_stream_plio_128& Acc0_txB0_6, axis_stream_plio_128& Acc0_txB1_6, axis_stream_plio_128& Acc0_txB2_6, axis_stream_plio_128& Acc0_txB3_6, axis_stream_plio_128& Acc0_txB4_6, axis_stream_plio_128& Acc0_txB5_6,  
            axis_stream_plio_128& Acc0_txB0_7, axis_stream_plio_128& Acc0_txB1_7, axis_stream_plio_128& Acc0_txB2_7, axis_stream_plio_128& Acc0_txB3_7, axis_stream_plio_128& Acc0_txB4_7, axis_stream_plio_128& Acc0_txB5_7,  
            axis_stream_plio_128& Acc0_txB0_8, axis_stream_plio_128& Acc0_txB1_8, axis_stream_plio_128& Acc0_txB2_8, axis_stream_plio_128& Acc0_txB3_8, axis_stream_plio_128& Acc0_txB4_8, axis_stream_plio_128& Acc0_txB5_8,  
            axis_stream_plio_128& Acc0_txB0_9, axis_stream_plio_128& Acc0_txB1_9, axis_stream_plio_128& Acc0_txB2_9, axis_stream_plio_128& Acc0_txB3_9, axis_stream_plio_128& Acc0_txB4_9, axis_stream_plio_128& Acc0_txB5_9,  
            axis_stream_plio_128& Acc0_txB0_10, axis_stream_plio_128& Acc0_txB1_10, axis_stream_plio_128& Acc0_txB2_10, axis_stream_plio_128& Acc0_txB3_10, axis_stream_plio_128& Acc0_txB4_10, axis_stream_plio_128& Acc0_txB5_10,  
            axis_stream_plio_128& Acc0_txB0_11, axis_stream_plio_128& Acc0_txB1_11, axis_stream_plio_128& Acc0_txB2_11, axis_stream_plio_128& Acc0_txB3_11, axis_stream_plio_128& Acc0_txB4_11, axis_stream_plio_128& Acc0_txB5_11,  
            const int sel, const bool para_sel, const bool RHS_bypass0_type0, const bool RHS_bypass1_type0, const bool RHS_bypass2_type0, const bool RHS_bypass0_type1, const bool RHS_bypass1_type1, const bool RHS_bypass2_type1, const bool enable){


#pragma HLS dataflow


    axis_stream_128 Acc0_rhs0_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs0_0_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs0_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs0_1_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs0_2_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs0_2_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs0_3_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs0_3_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs0_4_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs0_4_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs0_5_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs0_5_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs1_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs1_0_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs1_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs1_1_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs1_2_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs1_2_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs1_3_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs1_3_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs1_4_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs1_4_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs1_5_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs1_5_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs2_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs2_0_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs2_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs2_1_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs2_2_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs2_2_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs2_3_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs2_3_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs2_4_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs2_4_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs2_5_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs2_5_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs3_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs3_0_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs3_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs3_1_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs3_2_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs3_2_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs3_3_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs3_3_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs3_4_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs3_4_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs3_5_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs3_5_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs4_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs4_0_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs4_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs4_1_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs4_2_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs4_2_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs4_3_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs4_3_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs4_4_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs4_4_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs4_5_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs4_5_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs5_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs5_0_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs5_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs5_1_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs5_2_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs5_2_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs5_3_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs5_3_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs5_4_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs5_4_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs5_5_f0_p0_0;
    #pragma HLS STREAM variable = Acc0_rhs5_5_f0_p0_0 depth = 16
    axis_stream_128 Acc0_rhs0_0_f1_p0_0;
    axis_stream_128 Acc0_rhs1_0_f1_p0_0;
    axis_stream_128 Acc0_rhs2_0_f1_p0_0;
    axis_stream_128 Acc0_rhs3_0_f1_p0_0;
    axis_stream_128 Acc0_rhs4_0_f1_p0_0;
    axis_stream_128 Acc0_rhs5_0_f1_p0_0;
    axis_stream_128 Acc0_rhs0_1_f1_p0_0;
    axis_stream_128 Acc0_rhs1_1_f1_p0_0;
    axis_stream_128 Acc0_rhs2_1_f1_p0_0;
    axis_stream_128 Acc0_rhs3_1_f1_p0_0;
    axis_stream_128 Acc0_rhs4_1_f1_p0_0;
    axis_stream_128 Acc0_rhs5_1_f1_p0_0;
    axis_stream_128 Acc0_rhs0_2_f1_p0_0;
    axis_stream_128 Acc0_rhs1_2_f1_p0_0;
    axis_stream_128 Acc0_rhs2_2_f1_p0_0;
    axis_stream_128 Acc0_rhs3_2_f1_p0_0;
    axis_stream_128 Acc0_rhs4_2_f1_p0_0;
    axis_stream_128 Acc0_rhs5_2_f1_p0_0;
    axis_stream_128 Acc0_rhs0_3_f1_p0_0;
    axis_stream_128 Acc0_rhs1_3_f1_p0_0;
    axis_stream_128 Acc0_rhs2_3_f1_p0_0;
    axis_stream_128 Acc0_rhs3_3_f1_p0_0;
    axis_stream_128 Acc0_rhs4_3_f1_p0_0;
    axis_stream_128 Acc0_rhs5_3_f1_p0_0;
    axis_stream_128 Acc0_rhs0_4_f1_p0_0;
    axis_stream_128 Acc0_rhs1_4_f1_p0_0;
    axis_stream_128 Acc0_rhs2_4_f1_p0_0;
    axis_stream_128 Acc0_rhs3_4_f1_p0_0;
    axis_stream_128 Acc0_rhs4_4_f1_p0_0;
    axis_stream_128 Acc0_rhs5_4_f1_p0_0;
    axis_stream_128 Acc0_rhs0_5_f1_p0_0;
    axis_stream_128 Acc0_rhs1_5_f1_p0_0;
    axis_stream_128 Acc0_rhs2_5_f1_p0_0;
    axis_stream_128 Acc0_rhs3_5_f1_p0_0;
    axis_stream_128 Acc0_rhs4_5_f1_p0_0;
    axis_stream_128 Acc0_rhs5_5_f1_p0_0;
    

    axis_stream_128 Acc0_plio0_0_f0; axis_stream_128 Acc0_plio0_0_f1;  
    axis_stream_128 Acc0_plio0_1_f0; axis_stream_128 Acc0_plio0_1_f1;  
    axis_stream_128 Acc0_plio0_2_f0; axis_stream_128 Acc0_plio0_2_f1;  
    axis_stream_128 Acc0_plio0_3_f0; axis_stream_128 Acc0_plio0_3_f1;  
    axis_stream_128 Acc0_plio0_4_f0; axis_stream_128 Acc0_plio0_4_f1;  
    axis_stream_128 Acc0_plio0_5_f0; axis_stream_128 Acc0_plio0_5_f1;  
    axis_stream_128 Acc0_plio1_0_f0; axis_stream_128 Acc0_plio1_0_f1;  
    axis_stream_128 Acc0_plio1_1_f0; axis_stream_128 Acc0_plio1_1_f1;  
    axis_stream_128 Acc0_plio1_2_f0; axis_stream_128 Acc0_plio1_2_f1;  
    axis_stream_128 Acc0_plio1_3_f0; axis_stream_128 Acc0_plio1_3_f1;  
    axis_stream_128 Acc0_plio1_4_f0; axis_stream_128 Acc0_plio1_4_f1;  
    axis_stream_128 Acc0_plio1_5_f0; axis_stream_128 Acc0_plio1_5_f1;  
    axis_stream_128 Acc0_plio2_0_f0; axis_stream_128 Acc0_plio2_0_f1;  
    axis_stream_128 Acc0_plio2_1_f0; axis_stream_128 Acc0_plio2_1_f1;  
    axis_stream_128 Acc0_plio2_2_f0; axis_stream_128 Acc0_plio2_2_f1;  
    axis_stream_128 Acc0_plio2_3_f0; axis_stream_128 Acc0_plio2_3_f1;  
    axis_stream_128 Acc0_plio2_4_f0; axis_stream_128 Acc0_plio2_4_f1;  
    axis_stream_128 Acc0_plio2_5_f0; axis_stream_128 Acc0_plio2_5_f1;  
    axis_stream_128 Acc0_plio3_0_f0; axis_stream_128 Acc0_plio3_0_f1;  
    axis_stream_128 Acc0_plio3_1_f0; axis_stream_128 Acc0_plio3_1_f1;  
    axis_stream_128 Acc0_plio3_2_f0; axis_stream_128 Acc0_plio3_2_f1;  
    axis_stream_128 Acc0_plio3_3_f0; axis_stream_128 Acc0_plio3_3_f1;  
    axis_stream_128 Acc0_plio3_4_f0; axis_stream_128 Acc0_plio3_4_f1;  
    axis_stream_128 Acc0_plio3_5_f0; axis_stream_128 Acc0_plio3_5_f1;  
    axis_stream_128 Acc0_plio4_0_f0; axis_stream_128 Acc0_plio4_0_f1;  
    axis_stream_128 Acc0_plio4_1_f0; axis_stream_128 Acc0_plio4_1_f1;  
    axis_stream_128 Acc0_plio4_2_f0; axis_stream_128 Acc0_plio4_2_f1;  
    axis_stream_128 Acc0_plio4_3_f0; axis_stream_128 Acc0_plio4_3_f1;  
    axis_stream_128 Acc0_plio4_4_f0; axis_stream_128 Acc0_plio4_4_f1;  
    axis_stream_128 Acc0_plio4_5_f0; axis_stream_128 Acc0_plio4_5_f1;  
    axis_stream_128 Acc0_plio5_0_f0; axis_stream_128 Acc0_plio5_0_f1;  
    axis_stream_128 Acc0_plio5_1_f0; axis_stream_128 Acc0_plio5_1_f1;  
    axis_stream_128 Acc0_plio5_2_f0; axis_stream_128 Acc0_plio5_2_f1;  
    axis_stream_128 Acc0_plio5_3_f0; axis_stream_128 Acc0_plio5_3_f1;  
    axis_stream_128 Acc0_plio5_4_f0; axis_stream_128 Acc0_plio5_4_f1;  
    axis_stream_128 Acc0_plio5_5_f0; axis_stream_128 Acc0_plio5_5_f1;  
    

    axis_stream_128 Acc0_s0_0_f1; axis_stream_128 Acc0_s0_1_f1; axis_stream_128 Acc0_s0_2_f1; axis_stream_128 Acc0_s0_3_f1; axis_stream_128 Acc0_s0_4_f1; axis_stream_128 Acc0_s0_5_f1; 
    axis_stream_128 Acc0_s1_0_f1; axis_stream_128 Acc0_s1_1_f1; axis_stream_128 Acc0_s1_2_f1; axis_stream_128 Acc0_s1_3_f1; axis_stream_128 Acc0_s1_4_f1; axis_stream_128 Acc0_s1_5_f1; 
    axis_stream_128 Acc0_s2_0_f1; axis_stream_128 Acc0_s2_1_f1; axis_stream_128 Acc0_s2_2_f1; axis_stream_128 Acc0_s2_3_f1; axis_stream_128 Acc0_s2_4_f1; axis_stream_128 Acc0_s2_5_f1; 
    axis_stream_128 Acc0_s3_0_f1; axis_stream_128 Acc0_s3_1_f1; axis_stream_128 Acc0_s3_2_f1; axis_stream_128 Acc0_s3_3_f1; axis_stream_128 Acc0_s3_4_f1; axis_stream_128 Acc0_s3_5_f1; 
    axis_stream_128 Acc0_s4_0_f1; axis_stream_128 Acc0_s4_1_f1; axis_stream_128 Acc0_s4_2_f1; axis_stream_128 Acc0_s4_3_f1; axis_stream_128 Acc0_s4_4_f1; axis_stream_128 Acc0_s4_5_f1; 
    axis_stream_128 Acc0_s5_0_f1; axis_stream_128 Acc0_s5_1_f1; axis_stream_128 Acc0_s5_2_f1; axis_stream_128 Acc0_s5_3_f1; axis_stream_128 Acc0_s5_4_f1; axis_stream_128 Acc0_s5_5_f1; 
    

    Acc0_send_buff_type0<0,0,0,0,0>(buf_rhs[0], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs0_0_f0_p0_0, Acc0_rhs0_0_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<1,0,0,0,0>(buf_rhs[1], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs0_1_f0_p0_0, Acc0_rhs1_0_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<2,0,0,0,0>(buf_rhs[2], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs0_2_f0_p0_0, Acc0_rhs2_0_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<3,0,0,0,0>(buf_rhs[3], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs0_3_f0_p0_0, Acc0_rhs3_0_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<4,0,0,0,0>(buf_rhs[4], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs0_4_f0_p0_0, Acc0_rhs4_0_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<5,0,0,0,0>(buf_rhs[5], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs0_5_f0_p0_0, Acc0_rhs5_0_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<6,0,0,0,0>(buf_rhs[6], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs1_0_f0_p0_0, Acc0_rhs0_1_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<7,0,0,0,0>(buf_rhs[7], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs1_1_f0_p0_0, Acc0_rhs1_1_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<8,0,0,0,0>(buf_rhs[8], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs1_2_f0_p0_0, Acc0_rhs2_1_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<9,0,0,0,0>(buf_rhs[9], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs1_3_f0_p0_0, Acc0_rhs3_1_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<10,0,0,0,0>(buf_rhs[10], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs1_4_f0_p0_0, Acc0_rhs4_1_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<11,0,0,0,0>(buf_rhs[11], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs1_5_f0_p0_0, Acc0_rhs5_1_f1_p0_0, sel, para_sel, RHS_bypass0_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<12,0,0,0,0>(buf_rhs[12], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs2_0_f0_p0_0, Acc0_rhs0_2_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<13,0,0,0,0>(buf_rhs[13], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs2_1_f0_p0_0, Acc0_rhs1_2_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<14,0,0,0,0>(buf_rhs[14], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs2_2_f0_p0_0, Acc0_rhs2_2_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<15,0,0,0,0>(buf_rhs[15], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs2_3_f0_p0_0, Acc0_rhs3_2_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<16,0,0,0,0>(buf_rhs[16], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs2_4_f0_p0_0, Acc0_rhs4_2_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<17,0,0,0,0>(buf_rhs[17], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs2_5_f0_p0_0, Acc0_rhs5_2_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<18,0,0,0,0>(buf_rhs[18], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs3_0_f0_p0_0, Acc0_rhs0_3_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<19,0,0,0,0>(buf_rhs[19], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs3_1_f0_p0_0, Acc0_rhs1_3_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<20,0,0,0,0>(buf_rhs[20], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs3_2_f0_p0_0, Acc0_rhs2_3_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<21,0,0,0,0>(buf_rhs[21], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs3_3_f0_p0_0, Acc0_rhs3_3_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<22,0,0,0,0>(buf_rhs[22], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs3_4_f0_p0_0, Acc0_rhs4_3_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<23,0,0,0,0>(buf_rhs[23], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs3_5_f0_p0_0, Acc0_rhs5_3_f1_p0_0, sel, para_sel, RHS_bypass1_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<24,0,0,0,0>(buf_rhs[24], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs4_0_f0_p0_0, Acc0_rhs0_4_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<25,0,0,0,0>(buf_rhs[25], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs4_1_f0_p0_0, Acc0_rhs1_4_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<26,0,0,0,0>(buf_rhs[26], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs4_2_f0_p0_0, Acc0_rhs2_4_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<27,0,0,0,0>(buf_rhs[27], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs4_3_f0_p0_0, Acc0_rhs3_4_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<28,0,0,0,0>(buf_rhs[28], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs4_4_f0_p0_0, Acc0_rhs4_4_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<29,0,0,0,0>(buf_rhs[29], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs4_5_f0_p0_0, Acc0_rhs5_4_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<30,0,0,0,0>(buf_rhs[30], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs5_0_f0_p0_0, Acc0_rhs0_5_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<31,0,0,0,0>(buf_rhs[31], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs5_1_f0_p0_0, Acc0_rhs1_5_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<32,0,0,0,0>(buf_rhs[32], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs5_2_f0_p0_0, Acc0_rhs2_5_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass2_type1, enable);
    Acc0_send_buff_type0<33,0,0,0,0>(buf_rhs[33], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs5_3_f0_p0_0, Acc0_rhs3_5_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass0_type1, enable);
    Acc0_send_buff_type0<34,0,0,0,0>(buf_rhs[34], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs5_4_f0_p0_0, Acc0_rhs4_5_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass1_type1, enable);
    Acc0_send_buff_type0<35,0,0,0,0>(buf_rhs[35], boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_rhs5_5_f0_p0_0, Acc0_rhs5_5_f1_p0_0, sel, para_sel, RHS_bypass2_type0, RHS_bypass2_type1, enable);
    
    
    Acc0_merge0<0>(Acc0_rhs0_0_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_0_f0, (sel==0&&enable));
    Acc0_merge1<0>(Acc0_rhs0_0_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_0_f1, (sel==1&&enable)); 
    Acc0_transpose<0>(Acc0_plio0_0_f1, boundary_x, boundary_y, boundary_z, Acc0_s0_0_f1, (sel==1&&enable));  
    Acc0_send_RHS<0>(Acc0_plio0_0_f0, Acc0_s0_0_f1, boundary_x, boundary_y, boundary_z, Acc0_txB0_0, Acc0_txB0_1, sel, enable);
    
    Acc0_merge0<1>(Acc0_rhs0_1_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_1_f0, (sel==0&&enable));
    Acc0_merge1<1>(Acc0_rhs0_1_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_1_f1, (sel==1&&enable)); 
    Acc0_transpose<1>(Acc0_plio0_1_f1, boundary_x, boundary_y, boundary_z, Acc0_s0_1_f1, (sel==1&&enable));  
    Acc0_send_RHS<1>(Acc0_plio0_1_f0, Acc0_s0_1_f1, boundary_x, boundary_y, boundary_z, Acc0_txB0_2, Acc0_txB0_3, sel, enable);
    
    Acc0_merge0<2>(Acc0_rhs0_2_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_2_f0, (sel==0&&enable));
    Acc0_merge1<2>(Acc0_rhs0_2_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_2_f1, (sel==1&&enable)); 
    Acc0_transpose<2>(Acc0_plio0_2_f1, boundary_x, boundary_y, boundary_z, Acc0_s0_2_f1, (sel==1&&enable));  
    Acc0_send_RHS<2>(Acc0_plio0_2_f0, Acc0_s0_2_f1, boundary_x, boundary_y, boundary_z, Acc0_txB0_4, Acc0_txB0_5, sel, enable);
    
    Acc0_merge0<3>(Acc0_rhs0_3_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_3_f0, (sel==0&&enable));
    Acc0_merge1<3>(Acc0_rhs0_3_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_3_f1, (sel==1&&enable)); 
    Acc0_transpose<3>(Acc0_plio0_3_f1, boundary_x, boundary_y, boundary_z, Acc0_s0_3_f1, (sel==1&&enable));  
    Acc0_send_RHS<3>(Acc0_plio0_3_f0, Acc0_s0_3_f1, boundary_x, boundary_y, boundary_z, Acc0_txB0_6, Acc0_txB0_7, sel, enable);
    
    Acc0_merge0<4>(Acc0_rhs0_4_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_4_f0, (sel==0&&enable));
    Acc0_merge1<4>(Acc0_rhs0_4_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_4_f1, (sel==1&&enable)); 
    Acc0_transpose<4>(Acc0_plio0_4_f1, boundary_x, boundary_y, boundary_z, Acc0_s0_4_f1, (sel==1&&enable));  
    Acc0_send_RHS<4>(Acc0_plio0_4_f0, Acc0_s0_4_f1, boundary_x, boundary_y, boundary_z, Acc0_txB0_8, Acc0_txB0_9, sel, enable);
    
    Acc0_merge0<5>(Acc0_rhs0_5_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_5_f0, (sel==0&&enable));
    Acc0_merge1<5>(Acc0_rhs0_5_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio0_5_f1, (sel==1&&enable)); 
    Acc0_transpose<5>(Acc0_plio0_5_f1, boundary_x, boundary_y, boundary_z, Acc0_s0_5_f1, (sel==1&&enable));  
    Acc0_send_RHS<5>(Acc0_plio0_5_f0, Acc0_s0_5_f1, boundary_x, boundary_y, boundary_z, Acc0_txB0_10, Acc0_txB0_11, sel, enable);
    
    
    Acc0_merge0<6>(Acc0_rhs1_0_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_0_f0, (sel==0&&enable));
    Acc0_merge1<6>(Acc0_rhs1_0_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_0_f1, (sel==1&&enable)); 
    Acc0_transpose<6>(Acc0_plio1_0_f1, boundary_x, boundary_y, boundary_z, Acc0_s1_0_f1, (sel==1&&enable));  
    Acc0_send_RHS<6>(Acc0_plio1_0_f0, Acc0_s1_0_f1, boundary_x, boundary_y, boundary_z, Acc0_txB1_0, Acc0_txB1_1, sel, enable);
    
    Acc0_merge0<7>(Acc0_rhs1_1_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_1_f0, (sel==0&&enable));
    Acc0_merge1<7>(Acc0_rhs1_1_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_1_f1, (sel==1&&enable)); 
    Acc0_transpose<7>(Acc0_plio1_1_f1, boundary_x, boundary_y, boundary_z, Acc0_s1_1_f1, (sel==1&&enable));  
    Acc0_send_RHS<7>(Acc0_plio1_1_f0, Acc0_s1_1_f1, boundary_x, boundary_y, boundary_z, Acc0_txB1_2, Acc0_txB1_3, sel, enable);
    
    Acc0_merge0<8>(Acc0_rhs1_2_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_2_f0, (sel==0&&enable));
    Acc0_merge1<8>(Acc0_rhs1_2_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_2_f1, (sel==1&&enable)); 
    Acc0_transpose<8>(Acc0_plio1_2_f1, boundary_x, boundary_y, boundary_z, Acc0_s1_2_f1, (sel==1&&enable));  
    Acc0_send_RHS<8>(Acc0_plio1_2_f0, Acc0_s1_2_f1, boundary_x, boundary_y, boundary_z, Acc0_txB1_4, Acc0_txB1_5, sel, enable);
    
    Acc0_merge0<9>(Acc0_rhs1_3_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_3_f0, (sel==0&&enable));
    Acc0_merge1<9>(Acc0_rhs1_3_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_3_f1, (sel==1&&enable)); 
    Acc0_transpose<9>(Acc0_plio1_3_f1, boundary_x, boundary_y, boundary_z, Acc0_s1_3_f1, (sel==1&&enable));  
    Acc0_send_RHS<9>(Acc0_plio1_3_f0, Acc0_s1_3_f1, boundary_x, boundary_y, boundary_z, Acc0_txB1_6, Acc0_txB1_7, sel, enable);
    
    Acc0_merge0<10>(Acc0_rhs1_4_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_4_f0, (sel==0&&enable));
    Acc0_merge1<10>(Acc0_rhs1_4_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_4_f1, (sel==1&&enable)); 
    Acc0_transpose<10>(Acc0_plio1_4_f1, boundary_x, boundary_y, boundary_z, Acc0_s1_4_f1, (sel==1&&enable));  
    Acc0_send_RHS<10>(Acc0_plio1_4_f0, Acc0_s1_4_f1, boundary_x, boundary_y, boundary_z, Acc0_txB1_8, Acc0_txB1_9, sel, enable);
    
    Acc0_merge0<11>(Acc0_rhs1_5_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_5_f0, (sel==0&&enable));
    Acc0_merge1<11>(Acc0_rhs1_5_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio1_5_f1, (sel==1&&enable)); 
    Acc0_transpose<11>(Acc0_plio1_5_f1, boundary_x, boundary_y, boundary_z, Acc0_s1_5_f1, (sel==1&&enable));  
    Acc0_send_RHS<11>(Acc0_plio1_5_f0, Acc0_s1_5_f1, boundary_x, boundary_y, boundary_z, Acc0_txB1_10, Acc0_txB1_11, sel, enable);
    
    
    Acc0_merge0<12>(Acc0_rhs2_0_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_0_f0, (sel==0&&enable));
    Acc0_merge1<12>(Acc0_rhs2_0_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_0_f1, (sel==1&&enable)); 
    Acc0_transpose<12>(Acc0_plio2_0_f1, boundary_x, boundary_y, boundary_z, Acc0_s2_0_f1, (sel==1&&enable));  
    Acc0_send_RHS<12>(Acc0_plio2_0_f0, Acc0_s2_0_f1, boundary_x, boundary_y, boundary_z, Acc0_txB2_0, Acc0_txB2_1, sel, enable);
    
    Acc0_merge0<13>(Acc0_rhs2_1_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_1_f0, (sel==0&&enable));
    Acc0_merge1<13>(Acc0_rhs2_1_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_1_f1, (sel==1&&enable)); 
    Acc0_transpose<13>(Acc0_plio2_1_f1, boundary_x, boundary_y, boundary_z, Acc0_s2_1_f1, (sel==1&&enable));  
    Acc0_send_RHS<13>(Acc0_plio2_1_f0, Acc0_s2_1_f1, boundary_x, boundary_y, boundary_z, Acc0_txB2_2, Acc0_txB2_3, sel, enable);
    
    Acc0_merge0<14>(Acc0_rhs2_2_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_2_f0, (sel==0&&enable));
    Acc0_merge1<14>(Acc0_rhs2_2_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_2_f1, (sel==1&&enable)); 
    Acc0_transpose<14>(Acc0_plio2_2_f1, boundary_x, boundary_y, boundary_z, Acc0_s2_2_f1, (sel==1&&enable));  
    Acc0_send_RHS<14>(Acc0_plio2_2_f0, Acc0_s2_2_f1, boundary_x, boundary_y, boundary_z, Acc0_txB2_4, Acc0_txB2_5, sel, enable);
    
    Acc0_merge0<15>(Acc0_rhs2_3_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_3_f0, (sel==0&&enable));
    Acc0_merge1<15>(Acc0_rhs2_3_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_3_f1, (sel==1&&enable)); 
    Acc0_transpose<15>(Acc0_plio2_3_f1, boundary_x, boundary_y, boundary_z, Acc0_s2_3_f1, (sel==1&&enable));  
    Acc0_send_RHS<15>(Acc0_plio2_3_f0, Acc0_s2_3_f1, boundary_x, boundary_y, boundary_z, Acc0_txB2_6, Acc0_txB2_7, sel, enable);
    
    Acc0_merge0<16>(Acc0_rhs2_4_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_4_f0, (sel==0&&enable));
    Acc0_merge1<16>(Acc0_rhs2_4_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_4_f1, (sel==1&&enable)); 
    Acc0_transpose<16>(Acc0_plio2_4_f1, boundary_x, boundary_y, boundary_z, Acc0_s2_4_f1, (sel==1&&enable));  
    Acc0_send_RHS<16>(Acc0_plio2_4_f0, Acc0_s2_4_f1, boundary_x, boundary_y, boundary_z, Acc0_txB2_8, Acc0_txB2_9, sel, enable);
    
    Acc0_merge0<17>(Acc0_rhs2_5_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_5_f0, (sel==0&&enable));
    Acc0_merge1<17>(Acc0_rhs2_5_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio2_5_f1, (sel==1&&enable)); 
    Acc0_transpose<17>(Acc0_plio2_5_f1, boundary_x, boundary_y, boundary_z, Acc0_s2_5_f1, (sel==1&&enable));  
    Acc0_send_RHS<17>(Acc0_plio2_5_f0, Acc0_s2_5_f1, boundary_x, boundary_y, boundary_z, Acc0_txB2_10, Acc0_txB2_11, sel, enable);
    
    
    Acc0_merge0<18>(Acc0_rhs3_0_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_0_f0, (sel==0&&enable));
    Acc0_merge1<18>(Acc0_rhs3_0_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_0_f1, (sel==1&&enable)); 
    Acc0_transpose<18>(Acc0_plio3_0_f1, boundary_x, boundary_y, boundary_z, Acc0_s3_0_f1, (sel==1&&enable));  
    Acc0_send_RHS<18>(Acc0_plio3_0_f0, Acc0_s3_0_f1, boundary_x, boundary_y, boundary_z, Acc0_txB3_0, Acc0_txB3_1, sel, enable);
    
    Acc0_merge0<19>(Acc0_rhs3_1_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_1_f0, (sel==0&&enable));
    Acc0_merge1<19>(Acc0_rhs3_1_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_1_f1, (sel==1&&enable)); 
    Acc0_transpose<19>(Acc0_plio3_1_f1, boundary_x, boundary_y, boundary_z, Acc0_s3_1_f1, (sel==1&&enable));  
    Acc0_send_RHS<19>(Acc0_plio3_1_f0, Acc0_s3_1_f1, boundary_x, boundary_y, boundary_z, Acc0_txB3_2, Acc0_txB3_3, sel, enable);
    
    Acc0_merge0<20>(Acc0_rhs3_2_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_2_f0, (sel==0&&enable));
    Acc0_merge1<20>(Acc0_rhs3_2_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_2_f1, (sel==1&&enable)); 
    Acc0_transpose<20>(Acc0_plio3_2_f1, boundary_x, boundary_y, boundary_z, Acc0_s3_2_f1, (sel==1&&enable));  
    Acc0_send_RHS<20>(Acc0_plio3_2_f0, Acc0_s3_2_f1, boundary_x, boundary_y, boundary_z, Acc0_txB3_4, Acc0_txB3_5, sel, enable);
    
    Acc0_merge0<21>(Acc0_rhs3_3_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_3_f0, (sel==0&&enable));
    Acc0_merge1<21>(Acc0_rhs3_3_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_3_f1, (sel==1&&enable)); 
    Acc0_transpose<21>(Acc0_plio3_3_f1, boundary_x, boundary_y, boundary_z, Acc0_s3_3_f1, (sel==1&&enable));  
    Acc0_send_RHS<21>(Acc0_plio3_3_f0, Acc0_s3_3_f1, boundary_x, boundary_y, boundary_z, Acc0_txB3_6, Acc0_txB3_7, sel, enable);
    
    Acc0_merge0<22>(Acc0_rhs3_4_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_4_f0, (sel==0&&enable));
    Acc0_merge1<22>(Acc0_rhs3_4_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_4_f1, (sel==1&&enable)); 
    Acc0_transpose<22>(Acc0_plio3_4_f1, boundary_x, boundary_y, boundary_z, Acc0_s3_4_f1, (sel==1&&enable));  
    Acc0_send_RHS<22>(Acc0_plio3_4_f0, Acc0_s3_4_f1, boundary_x, boundary_y, boundary_z, Acc0_txB3_8, Acc0_txB3_9, sel, enable);
    
    Acc0_merge0<23>(Acc0_rhs3_5_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_5_f0, (sel==0&&enable));
    Acc0_merge1<23>(Acc0_rhs3_5_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio3_5_f1, (sel==1&&enable)); 
    Acc0_transpose<23>(Acc0_plio3_5_f1, boundary_x, boundary_y, boundary_z, Acc0_s3_5_f1, (sel==1&&enable));  
    Acc0_send_RHS<23>(Acc0_plio3_5_f0, Acc0_s3_5_f1, boundary_x, boundary_y, boundary_z, Acc0_txB3_10, Acc0_txB3_11, sel, enable);
    
    
    Acc0_merge0<24>(Acc0_rhs4_0_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_0_f0, (sel==0&&enable));
    Acc0_merge1<24>(Acc0_rhs4_0_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_0_f1, (sel==1&&enable)); 
    Acc0_transpose<24>(Acc0_plio4_0_f1, boundary_x, boundary_y, boundary_z, Acc0_s4_0_f1, (sel==1&&enable));  
    Acc0_send_RHS<24>(Acc0_plio4_0_f0, Acc0_s4_0_f1, boundary_x, boundary_y, boundary_z, Acc0_txB4_0, Acc0_txB4_1, sel, enable);
    
    Acc0_merge0<25>(Acc0_rhs4_1_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_1_f0, (sel==0&&enable));
    Acc0_merge1<25>(Acc0_rhs4_1_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_1_f1, (sel==1&&enable)); 
    Acc0_transpose<25>(Acc0_plio4_1_f1, boundary_x, boundary_y, boundary_z, Acc0_s4_1_f1, (sel==1&&enable));  
    Acc0_send_RHS<25>(Acc0_plio4_1_f0, Acc0_s4_1_f1, boundary_x, boundary_y, boundary_z, Acc0_txB4_2, Acc0_txB4_3, sel, enable);
    
    Acc0_merge0<26>(Acc0_rhs4_2_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_2_f0, (sel==0&&enable));
    Acc0_merge1<26>(Acc0_rhs4_2_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_2_f1, (sel==1&&enable)); 
    Acc0_transpose<26>(Acc0_plio4_2_f1, boundary_x, boundary_y, boundary_z, Acc0_s4_2_f1, (sel==1&&enable));  
    Acc0_send_RHS<26>(Acc0_plio4_2_f0, Acc0_s4_2_f1, boundary_x, boundary_y, boundary_z, Acc0_txB4_4, Acc0_txB4_5, sel, enable);
    
    Acc0_merge0<27>(Acc0_rhs4_3_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_3_f0, (sel==0&&enable));
    Acc0_merge1<27>(Acc0_rhs4_3_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_3_f1, (sel==1&&enable)); 
    Acc0_transpose<27>(Acc0_plio4_3_f1, boundary_x, boundary_y, boundary_z, Acc0_s4_3_f1, (sel==1&&enable));  
    Acc0_send_RHS<27>(Acc0_plio4_3_f0, Acc0_s4_3_f1, boundary_x, boundary_y, boundary_z, Acc0_txB4_6, Acc0_txB4_7, sel, enable);
    
    Acc0_merge0<28>(Acc0_rhs4_4_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_4_f0, (sel==0&&enable));
    Acc0_merge1<28>(Acc0_rhs4_4_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_4_f1, (sel==1&&enable)); 
    Acc0_transpose<28>(Acc0_plio4_4_f1, boundary_x, boundary_y, boundary_z, Acc0_s4_4_f1, (sel==1&&enable));  
    Acc0_send_RHS<28>(Acc0_plio4_4_f0, Acc0_s4_4_f1, boundary_x, boundary_y, boundary_z, Acc0_txB4_8, Acc0_txB4_9, sel, enable);
    
    Acc0_merge0<29>(Acc0_rhs4_5_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_5_f0, (sel==0&&enable));
    Acc0_merge1<29>(Acc0_rhs4_5_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio4_5_f1, (sel==1&&enable)); 
    Acc0_transpose<29>(Acc0_plio4_5_f1, boundary_x, boundary_y, boundary_z, Acc0_s4_5_f1, (sel==1&&enable));  
    Acc0_send_RHS<29>(Acc0_plio4_5_f0, Acc0_s4_5_f1, boundary_x, boundary_y, boundary_z, Acc0_txB4_10, Acc0_txB4_11, sel, enable);
    
    
    Acc0_merge0<30>(Acc0_rhs5_0_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_0_f0, (sel==0&&enable));
    Acc0_merge1<30>(Acc0_rhs5_0_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_0_f1, (sel==1&&enable)); 
    Acc0_transpose<30>(Acc0_plio5_0_f1, boundary_x, boundary_y, boundary_z, Acc0_s5_0_f1, (sel==1&&enable));  
    Acc0_send_RHS<30>(Acc0_plio5_0_f0, Acc0_s5_0_f1, boundary_x, boundary_y, boundary_z, Acc0_txB5_0, Acc0_txB5_1, sel, enable);
    
    Acc0_merge0<31>(Acc0_rhs5_1_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_1_f0, (sel==0&&enable));
    Acc0_merge1<31>(Acc0_rhs5_1_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_1_f1, (sel==1&&enable)); 
    Acc0_transpose<31>(Acc0_plio5_1_f1, boundary_x, boundary_y, boundary_z, Acc0_s5_1_f1, (sel==1&&enable));  
    Acc0_send_RHS<31>(Acc0_plio5_1_f0, Acc0_s5_1_f1, boundary_x, boundary_y, boundary_z, Acc0_txB5_2, Acc0_txB5_3, sel, enable);
    
    Acc0_merge0<32>(Acc0_rhs5_2_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_2_f0, (sel==0&&enable));
    Acc0_merge1<32>(Acc0_rhs5_2_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_2_f1, (sel==1&&enable)); 
    Acc0_transpose<32>(Acc0_plio5_2_f1, boundary_x, boundary_y, boundary_z, Acc0_s5_2_f1, (sel==1&&enable));  
    Acc0_send_RHS<32>(Acc0_plio5_2_f0, Acc0_s5_2_f1, boundary_x, boundary_y, boundary_z, Acc0_txB5_4, Acc0_txB5_5, sel, enable);
    
    Acc0_merge0<33>(Acc0_rhs5_3_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_3_f0, (sel==0&&enable));
    Acc0_merge1<33>(Acc0_rhs5_3_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_3_f1, (sel==1&&enable)); 
    Acc0_transpose<33>(Acc0_plio5_3_f1, boundary_x, boundary_y, boundary_z, Acc0_s5_3_f1, (sel==1&&enable));  
    Acc0_send_RHS<33>(Acc0_plio5_3_f0, Acc0_s5_3_f1, boundary_x, boundary_y, boundary_z, Acc0_txB5_6, Acc0_txB5_7, sel, enable);
    
    Acc0_merge0<34>(Acc0_rhs5_4_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_4_f0, (sel==0&&enable));
    Acc0_merge1<34>(Acc0_rhs5_4_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_4_f1, (sel==1&&enable)); 
    Acc0_transpose<34>(Acc0_plio5_4_f1, boundary_x, boundary_y, boundary_z, Acc0_s5_4_f1, (sel==1&&enable));  
    Acc0_send_RHS<34>(Acc0_plio5_4_f0, Acc0_s5_4_f1, boundary_x, boundary_y, boundary_z, Acc0_txB5_8, Acc0_txB5_9, sel, enable);
    
    Acc0_merge0<35>(Acc0_rhs5_5_f0_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_5_f0, (sel==0&&enable));
    Acc0_merge1<35>(Acc0_rhs5_5_f1_p0_0,boundary_x, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc0_plio5_5_f1, (sel==1&&enable)); 
    Acc0_transpose<35>(Acc0_plio5_5_f1, boundary_x, boundary_y, boundary_z, Acc0_s5_5_f1, (sel==1&&enable));  
    Acc0_send_RHS<35>(Acc0_plio5_5_f0, Acc0_s5_5_f1, boundary_x, boundary_y, boundary_z, Acc0_txB5_10, Acc0_txB5_11, sel, enable);
    


}

// Sel here is for different function in HCE: 0 for bypass, 1 for transpose
template<int NC>
void Acc0_receive_OUT(axis_stream_plio_128& Acc0_rxC, const int boundary_x, const int boundary_y, const int boundary_z,axis_stream_128& Acc0_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        ap_int<8>  data_temp1[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp1 complete dim=0

        ap_int<8>  data_temp2[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp2 complete dim=0

        data_plio_128   temp_array[ACC0_LH1][ACC0_RW2];
        #pragma HLS bind_storage variable=temp_array type=RAM_1P impl=LUTRAM

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < boundary_x; x++) {
            for (int z = 0; z < boundary_z; z++) {          
                for (int y = 0; y < boundary_y; y++) {
                    for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=temp_array type=inter false
                            tmp   =  Acc0_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            if(boundary_y<=1){
                                for (int un=0; un<NUM_PER_128; un++){
                                #pragma HLS UNROLL factor=NUM_PER_128
                                    data_temp(un*8+7,un*8) = data_temp0[un];
                                }
                                
                                Acc0_sout0.write(data_temp);
                                
                            }
                            else{
                                if(y==0){
                                    for (int un=0; un<NUM_PER_128; un++){
                                    #pragma HLS UNROLL factor=NUM_PER_128
                                        data_temp1[un] = 0;
                                        data_temp2[un] = data_temp0[un] + data_temp1[un];
                                    }
                                }
                                else if(y==boundary_y-1){
                                    for (int un=0; un<NUM_PER_128; un++){
                                    #pragma HLS UNROLL factor=NUM_PER_128
                                        data_temp1[un] = temp_array[h1][w2](un*8+7,un*8);
                                        data_temp2[un] = data_temp0[un] + data_temp1[un];
                                        data_temp(un*8+7,un*8) = data_temp2[un];
                                    }
                                    
                                    Acc0_sout0.write(data_temp);
                                    
                                }
                                else{
                                    for (int un=0; un<NUM_PER_128; un++){
                                    #pragma HLS UNROLL factor=NUM_PER_128
                                        data_temp1[un] = temp_array[h1][w2](un*8+7,un*8);
                                        data_temp2[un] = data_temp0[un] + data_temp1[un];
                                    }
                                }   
                            }
                        }
                    }
                }  
            }
        }
    }
}



//Receive Data to buffer according to different functionalities
template<int NC>
void Acc0_receive_buff_type0(axis_stream_128& Acc0_sin0,const int boundary_x, const int boundary_z, const int buffer_h1, const int buffer_w2,
                                          ap_uint<PLIO_WIDTH_128> buf_out0_0[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],  
                                          const bool OUT_bypass, const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;
        for (int x = 0; x < boundary_x; x++) {
            for (int z = 0; z < boundary_z; z++) {          
                for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        int pos0=(w2+z*ACC0_RW2)%buffer_w2;
                        int pos1=(h1+x*ACC0_LH1)%buffer_h1;
                        int temp_c=(w2+z*ACC0_RW2)/buffer_w2;
                        int temp_a=(h1+x*ACC0_LH1)/buffer_h1;
                        data_temp = Acc0_sin0.read();
                        
                        if(OUT_bypass==0){
                            buf_out0_0[pos1][pos0] = data_temp;
                            
                        }
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc0_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], 
           const int boundary_x, const int boundary_y, const int boundary_z, const int buffer_h1, const int buffer_w2,
           axis_stream_plio_128& Acc0_rxC0_0, axis_stream_plio_128& Acc0_rxC0_1, axis_stream_plio_128& Acc0_rxC0_2, axis_stream_plio_128& Acc0_rxC0_3, axis_stream_plio_128& Acc0_rxC0_4, axis_stream_plio_128& Acc0_rxC0_5,  
           axis_stream_plio_128& Acc0_rxC1_0, axis_stream_plio_128& Acc0_rxC1_1, axis_stream_plio_128& Acc0_rxC1_2, axis_stream_plio_128& Acc0_rxC1_3, axis_stream_plio_128& Acc0_rxC1_4, axis_stream_plio_128& Acc0_rxC1_5,  
           axis_stream_plio_128& Acc0_rxC2_0, axis_stream_plio_128& Acc0_rxC2_1, axis_stream_plio_128& Acc0_rxC2_2, axis_stream_plio_128& Acc0_rxC2_3, axis_stream_plio_128& Acc0_rxC2_4, axis_stream_plio_128& Acc0_rxC2_5,  
           axis_stream_plio_128& Acc0_rxC3_0, axis_stream_plio_128& Acc0_rxC3_1, axis_stream_plio_128& Acc0_rxC3_2, axis_stream_plio_128& Acc0_rxC3_3, axis_stream_plio_128& Acc0_rxC3_4, axis_stream_plio_128& Acc0_rxC3_5,  
           axis_stream_plio_128& Acc0_rxC4_0, axis_stream_plio_128& Acc0_rxC4_1, axis_stream_plio_128& Acc0_rxC4_2, axis_stream_plio_128& Acc0_rxC4_3, axis_stream_plio_128& Acc0_rxC4_4, axis_stream_plio_128& Acc0_rxC4_5,  
           axis_stream_plio_128& Acc0_rxC5_0, axis_stream_plio_128& Acc0_rxC5_1, axis_stream_plio_128& Acc0_rxC5_2, axis_stream_plio_128& Acc0_rxC5_3, axis_stream_plio_128& Acc0_rxC5_4, axis_stream_plio_128& Acc0_rxC5_5,  
           const bool OUT_bypass0, const bool OUT_bypass1, const bool OUT_bypass2,  const bool enable){

#pragma HLS dataflow

    axis_stream_128 Acc0_plio0_0_f0;axis_stream_128 Acc0_plio0_1_f0;axis_stream_128 Acc0_plio0_2_f0;axis_stream_128 Acc0_plio0_3_f0;axis_stream_128 Acc0_plio0_4_f0;axis_stream_128 Acc0_plio0_5_f0; 
    axis_stream_128 Acc0_plio1_0_f0;axis_stream_128 Acc0_plio1_1_f0;axis_stream_128 Acc0_plio1_2_f0;axis_stream_128 Acc0_plio1_3_f0;axis_stream_128 Acc0_plio1_4_f0;axis_stream_128 Acc0_plio1_5_f0; 
    axis_stream_128 Acc0_plio2_0_f0;axis_stream_128 Acc0_plio2_1_f0;axis_stream_128 Acc0_plio2_2_f0;axis_stream_128 Acc0_plio2_3_f0;axis_stream_128 Acc0_plio2_4_f0;axis_stream_128 Acc0_plio2_5_f0; 
    axis_stream_128 Acc0_plio3_0_f0;axis_stream_128 Acc0_plio3_1_f0;axis_stream_128 Acc0_plio3_2_f0;axis_stream_128 Acc0_plio3_3_f0;axis_stream_128 Acc0_plio3_4_f0;axis_stream_128 Acc0_plio3_5_f0; 
    axis_stream_128 Acc0_plio4_0_f0;axis_stream_128 Acc0_plio4_1_f0;axis_stream_128 Acc0_plio4_2_f0;axis_stream_128 Acc0_plio4_3_f0;axis_stream_128 Acc0_plio4_4_f0;axis_stream_128 Acc0_plio4_5_f0; 
    axis_stream_128 Acc0_plio5_0_f0;axis_stream_128 Acc0_plio5_1_f0;axis_stream_128 Acc0_plio5_2_f0;axis_stream_128 Acc0_plio5_3_f0;axis_stream_128 Acc0_plio5_4_f0;axis_stream_128 Acc0_plio5_5_f0; 
    Acc0_receive_OUT<0>(Acc0_rxC0_0, boundary_x, boundary_y, boundary_z, Acc0_plio0_0_f0, enable);
    Acc0_receive_OUT<1>(Acc0_rxC0_1, boundary_x, boundary_y, boundary_z, Acc0_plio0_1_f0, enable);
    Acc0_receive_OUT<2>(Acc0_rxC0_2, boundary_x, boundary_y, boundary_z, Acc0_plio0_2_f0, enable);
    Acc0_receive_OUT<3>(Acc0_rxC0_3, boundary_x, boundary_y, boundary_z, Acc0_plio0_3_f0, enable);
    Acc0_receive_OUT<4>(Acc0_rxC0_4, boundary_x, boundary_y, boundary_z, Acc0_plio0_4_f0, enable);
    Acc0_receive_OUT<5>(Acc0_rxC0_5, boundary_x, boundary_y, boundary_z, Acc0_plio0_5_f0, enable);
    Acc0_receive_OUT<6>(Acc0_rxC1_0, boundary_x, boundary_y, boundary_z, Acc0_plio1_0_f0, enable);
    Acc0_receive_OUT<7>(Acc0_rxC1_1, boundary_x, boundary_y, boundary_z, Acc0_plio1_1_f0, enable);
    Acc0_receive_OUT<8>(Acc0_rxC1_2, boundary_x, boundary_y, boundary_z, Acc0_plio1_2_f0, enable);
    Acc0_receive_OUT<9>(Acc0_rxC1_3, boundary_x, boundary_y, boundary_z, Acc0_plio1_3_f0, enable);
    Acc0_receive_OUT<10>(Acc0_rxC1_4, boundary_x, boundary_y, boundary_z, Acc0_plio1_4_f0, enable);
    Acc0_receive_OUT<11>(Acc0_rxC1_5, boundary_x, boundary_y, boundary_z, Acc0_plio1_5_f0, enable);
    Acc0_receive_OUT<12>(Acc0_rxC2_0, boundary_x, boundary_y, boundary_z, Acc0_plio2_0_f0, enable);
    Acc0_receive_OUT<13>(Acc0_rxC2_1, boundary_x, boundary_y, boundary_z, Acc0_plio2_1_f0, enable);
    Acc0_receive_OUT<14>(Acc0_rxC2_2, boundary_x, boundary_y, boundary_z, Acc0_plio2_2_f0, enable);
    Acc0_receive_OUT<15>(Acc0_rxC2_3, boundary_x, boundary_y, boundary_z, Acc0_plio2_3_f0, enable);
    Acc0_receive_OUT<16>(Acc0_rxC2_4, boundary_x, boundary_y, boundary_z, Acc0_plio2_4_f0, enable);
    Acc0_receive_OUT<17>(Acc0_rxC2_5, boundary_x, boundary_y, boundary_z, Acc0_plio2_5_f0, enable);
    Acc0_receive_OUT<18>(Acc0_rxC3_0, boundary_x, boundary_y, boundary_z, Acc0_plio3_0_f0, enable);
    Acc0_receive_OUT<19>(Acc0_rxC3_1, boundary_x, boundary_y, boundary_z, Acc0_plio3_1_f0, enable);
    Acc0_receive_OUT<20>(Acc0_rxC3_2, boundary_x, boundary_y, boundary_z, Acc0_plio3_2_f0, enable);
    Acc0_receive_OUT<21>(Acc0_rxC3_3, boundary_x, boundary_y, boundary_z, Acc0_plio3_3_f0, enable);
    Acc0_receive_OUT<22>(Acc0_rxC3_4, boundary_x, boundary_y, boundary_z, Acc0_plio3_4_f0, enable);
    Acc0_receive_OUT<23>(Acc0_rxC3_5, boundary_x, boundary_y, boundary_z, Acc0_plio3_5_f0, enable);
    Acc0_receive_OUT<24>(Acc0_rxC4_0, boundary_x, boundary_y, boundary_z, Acc0_plio4_0_f0, enable);
    Acc0_receive_OUT<25>(Acc0_rxC4_1, boundary_x, boundary_y, boundary_z, Acc0_plio4_1_f0, enable);
    Acc0_receive_OUT<26>(Acc0_rxC4_2, boundary_x, boundary_y, boundary_z, Acc0_plio4_2_f0, enable);
    Acc0_receive_OUT<27>(Acc0_rxC4_3, boundary_x, boundary_y, boundary_z, Acc0_plio4_3_f0, enable);
    Acc0_receive_OUT<28>(Acc0_rxC4_4, boundary_x, boundary_y, boundary_z, Acc0_plio4_4_f0, enable);
    Acc0_receive_OUT<29>(Acc0_rxC4_5, boundary_x, boundary_y, boundary_z, Acc0_plio4_5_f0, enable);
    Acc0_receive_OUT<30>(Acc0_rxC5_0, boundary_x, boundary_y, boundary_z, Acc0_plio5_0_f0, enable);
    Acc0_receive_OUT<31>(Acc0_rxC5_1, boundary_x, boundary_y, boundary_z, Acc0_plio5_1_f0, enable);
    Acc0_receive_OUT<32>(Acc0_rxC5_2, boundary_x, boundary_y, boundary_z, Acc0_plio5_2_f0, enable);
    Acc0_receive_OUT<33>(Acc0_rxC5_3, boundary_x, boundary_y, boundary_z, Acc0_plio5_3_f0, enable);
    Acc0_receive_OUT<34>(Acc0_rxC5_4, boundary_x, boundary_y, boundary_z, Acc0_plio5_4_f0, enable);
    Acc0_receive_OUT<35>(Acc0_rxC5_5, boundary_x, boundary_y, boundary_z, Acc0_plio5_5_f0, enable);
    
    //Different streams in each module refer to different functionality
    Acc0_receive_buff_type0<0>(Acc0_plio0_0_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[0],OUT_bypass0, enable);
    Acc0_receive_buff_type0<1>(Acc0_plio0_1_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[1],OUT_bypass0, enable);
    Acc0_receive_buff_type0<2>(Acc0_plio0_2_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[2],OUT_bypass1, enable);
    Acc0_receive_buff_type0<3>(Acc0_plio0_3_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[3],OUT_bypass1, enable);
    Acc0_receive_buff_type0<4>(Acc0_plio0_4_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[4],OUT_bypass2, enable);
    Acc0_receive_buff_type0<5>(Acc0_plio0_5_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[5],OUT_bypass2, enable);
    Acc0_receive_buff_type0<6>(Acc0_plio1_0_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[6],OUT_bypass0, enable);
    Acc0_receive_buff_type0<7>(Acc0_plio1_1_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[7],OUT_bypass0, enable);
    Acc0_receive_buff_type0<8>(Acc0_plio1_2_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[8],OUT_bypass1, enable);
    Acc0_receive_buff_type0<9>(Acc0_plio1_3_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[9],OUT_bypass1, enable);
    Acc0_receive_buff_type0<10>(Acc0_plio1_4_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[10],OUT_bypass2, enable);
    Acc0_receive_buff_type0<11>(Acc0_plio1_5_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[11],OUT_bypass2, enable);
    Acc0_receive_buff_type0<12>(Acc0_plio2_0_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[12],OUT_bypass0, enable);
    Acc0_receive_buff_type0<13>(Acc0_plio2_1_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[13],OUT_bypass0, enable);
    Acc0_receive_buff_type0<14>(Acc0_plio2_2_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[14],OUT_bypass1, enable);
    Acc0_receive_buff_type0<15>(Acc0_plio2_3_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[15],OUT_bypass1, enable);
    Acc0_receive_buff_type0<16>(Acc0_plio2_4_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[16],OUT_bypass2, enable);
    Acc0_receive_buff_type0<17>(Acc0_plio2_5_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[17],OUT_bypass2, enable);
    Acc0_receive_buff_type0<18>(Acc0_plio3_0_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[18],OUT_bypass0, enable);
    Acc0_receive_buff_type0<19>(Acc0_plio3_1_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[19],OUT_bypass0, enable);
    Acc0_receive_buff_type0<20>(Acc0_plio3_2_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[20],OUT_bypass1, enable);
    Acc0_receive_buff_type0<21>(Acc0_plio3_3_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[21],OUT_bypass1, enable);
    Acc0_receive_buff_type0<22>(Acc0_plio3_4_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[22],OUT_bypass2, enable);
    Acc0_receive_buff_type0<23>(Acc0_plio3_5_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[23],OUT_bypass2, enable);
    Acc0_receive_buff_type0<24>(Acc0_plio4_0_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[24],OUT_bypass0, enable);
    Acc0_receive_buff_type0<25>(Acc0_plio4_1_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[25],OUT_bypass0, enable);
    Acc0_receive_buff_type0<26>(Acc0_plio4_2_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[26],OUT_bypass1, enable);
    Acc0_receive_buff_type0<27>(Acc0_plio4_3_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[27],OUT_bypass1, enable);
    Acc0_receive_buff_type0<28>(Acc0_plio4_4_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[28],OUT_bypass2, enable);
    Acc0_receive_buff_type0<29>(Acc0_plio4_5_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[29],OUT_bypass2, enable);
    Acc0_receive_buff_type0<30>(Acc0_plio5_0_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[30],OUT_bypass0, enable);
    Acc0_receive_buff_type0<31>(Acc0_plio5_1_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[31],OUT_bypass0, enable);
    Acc0_receive_buff_type0<32>(Acc0_plio5_2_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[32],OUT_bypass1, enable);
    Acc0_receive_buff_type0<33>(Acc0_plio5_3_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[33],OUT_bypass1, enable);
    Acc0_receive_buff_type0<34>(Acc0_plio5_4_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[34],OUT_bypass2, enable);
    Acc0_receive_buff_type0<35>(Acc0_plio5_5_f0,boundary_x, boundary_z, buffer_h1, buffer_w2,buf_out[35],OUT_bypass2, enable);
    
}












template<int NC>
void send_patch_wrapper1(ap_uint<PLIO_WIDTH_128> a_buf[PE_KHP][PE_KH][PE_SEQ_ALL],axis_stream_plio_128& txA0,axis_stream_plio_128& txA1,const bool enable){
    send_patch_wrapper0<0>(a_buf[0], txA0, enable);
    send_patch_wrapper0<1>(a_buf[1], txA1, enable);
    
}


void send_patch_top(ap_uint<PLIO_WIDTH_128> a_buf[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL],
                    axis_stream_plio_128& txA0, axis_stream_plio_128& txA1, 
                    axis_stream_plio_128& txA2, axis_stream_plio_128& txA3, 
                    axis_stream_plio_128& txA4, axis_stream_plio_128& txA5, const bool enable){
    send_patch_wrapper1<0>(a_buf[0],txA0,txA1,enable);
    send_patch_wrapper1<1>(a_buf[1],txA2,txA3,enable);
    send_patch_wrapper1<2>(a_buf[2],txA4,txA5,enable);
    
}

template<int NC>
void receive_Patch_buff(axis_stream_128& s_in0,
                        ap_uint<PLIO_WIDTH_128> buff0[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],
                        ap_uint<PLIO_WIDTH_128> buff1[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],
                        ap_uint<PLIO_WIDTH_128> buff2[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],
                        ap_uint<PLIO_WIDTH_128> buff3[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],
                        ap_uint<PLIO_WIDTH_128> buff4[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],
                        ap_uint<PLIO_WIDTH_128> buff5[BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],
                        const bool enable){
#pragma HLS inline off
    if(enable){
        data_plio_128 data_temp;
        const int pe_w2 =  PE_OC_ALL / PE_C / (BUFF0_DIM2/PE_C) / 2;
        const int pe_w1 = PE_SEQ_ALL / BUFF0_DIM3 / NUM_PER_64;
        for (int h1 = 0; h1 < (PE_SEQ_ALL/NUM_PER_64); h1++) {
            for (int w2 = 0; w2 < (PE_OC/2); w2++) {
            #pragma HLS PIPELINE II = 1
                int pos0 = w2 % pe_w2;
                int pos1 = h1 % pe_w1;
                int temp_b = w2 / pe_w2;
                int temp_a = h1 / pe_w1;
                data_temp=s_in0.read();
                if (temp_b==0&&temp_a==0){ 
                    buff0[pos1][pos0] = data_temp;
                }
                else if (temp_b==0&&temp_a==1){ 
                    buff1[pos1][pos0] = data_temp;
                }
                else if (temp_b==0&&temp_a==2){ 
                    buff2[pos1][pos0] = data_temp;
                }
                else if (temp_b==0&&temp_a==3){ 
                    buff3[pos1][pos0] = data_temp;
                }
                else if (temp_b==0&&temp_a==4){ 
                    buff4[pos1][pos0] = data_temp;
                }
                else{
                    buff5[pos1][pos0] = data_temp;
                }
                
            }
        }
    }
}

void receive_Patch_top(axis_stream_plio_128& rxC0,axis_stream_plio_128& rxC1,axis_stream_plio_128& rxC2,axis_stream_plio_128& rxC3,axis_stream_plio_128& rxC4,axis_stream_plio_128& rxC5,ap_uint<PLIO_WIDTH_128> buff0[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], const bool enable){
#pragma HLS dataflow
    
    axis_stream_128 s_out0;
    axis_stream_128 s_out1;
    axis_stream_128 s_out2;
    axis_stream_128 s_out3;
    axis_stream_128 s_out4;
    axis_stream_128 s_out5;
    receive_Patch_transpose<0>(rxC0, s_out0, enable);
    receive_Patch_buff<0>(s_out0,buff0[0],buff0[6],buff0[12],buff0[18],buff0[24],buff0[30],enable);
    
    receive_Patch_transpose<1>(rxC1, s_out1, enable);
    receive_Patch_buff<1>(s_out1,buff0[1],buff0[7],buff0[13],buff0[19],buff0[25],buff0[31],enable);
    
    receive_Patch_transpose<2>(rxC2, s_out2, enable);
    receive_Patch_buff<2>(s_out2,buff0[2],buff0[8],buff0[14],buff0[20],buff0[26],buff0[32],enable);
    
    receive_Patch_transpose<3>(rxC3, s_out3, enable);
    receive_Patch_buff<3>(s_out3,buff0[3],buff0[9],buff0[15],buff0[21],buff0[27],buff0[33],enable);
    
    receive_Patch_transpose<4>(rxC4, s_out4, enable);
    receive_Patch_buff<4>(s_out4,buff0[4],buff0[10],buff0[16],buff0[22],buff0[28],buff0[34],enable);
    
    receive_Patch_transpose<5>(rxC5, s_out5, enable);
    receive_Patch_buff<5>(s_out5,buff0[5],buff0[11],buff0[17],buff0[23],buff0[29],buff0[35],enable);
    
}


template<int NC>
void image_pe(axis_stream_A& dataA_in0, axis_stream_A& dataA_in1, axis_stream_A& dataA_in2,
              axis_stream_plio_128& txA0, axis_stream_plio_128& txA1, 
              axis_stream_plio_128& txA2, axis_stream_plio_128& txA3, 
              axis_stream_plio_128& txA4, axis_stream_plio_128& txA5, 
              axis_stream_plio_128& rxC0,axis_stream_plio_128& rxC1,axis_stream_plio_128& rxC2,axis_stream_plio_128& rxC3,axis_stream_plio_128& rxC4,axis_stream_plio_128& rxC5,ap_uint<PLIO_WIDTH_128> buff0[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], const bool enable){
    
    ap_uint<PLIO_WIDTH_128> buff_image0[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL];   // 48
    #pragma HLS bind_storage variable=buff_image0 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_image0 complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff_image0 complete dim=2
    #pragma HLS ARRAY_PARTITION variable=buff_image0 cyclic factor=2 dim=4
    
    for(int i=0 ; i < 2; i++){
        if(i==0){
            load_image_top(dataA_in0, dataA_in1, dataA_in2, buff_image0, enable);
        }
        else{
            send_patch_top(buff_image0,txA0,txA1,txA2,txA3,txA4,txA5,enable);
            receive_Patch_top(rxC0,rxC1,rxC2,rxC3,rxC4,rxC5,buff0, enable);
        }
    }
}

void m2vector(ap_uint<PLIO_WIDTH_128> buff0[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64],const bool enable){
#pragma HLS inline off

    if (enable){
        data_plio_64 temp_out0;
        data_plio_64 temp_out1;
        data_plio_128 data0;
        data_plio_128 data1;
        ap_uint<8> temp_data[2][8];
        #pragma HLS ARRAY_PARTITION variable=temp_data complete dim=1
        #pragma HLS ARRAY_PARTITION variable=temp_data complete dim=2

        for (int p=0;p<BUFF0_DIM2/FC_B;p++){
            for (int j=0; j<(FC_K/BUFF0_DIM2/NUM_PER_64);j++){
                for (int i=0; i < 4;i++){
                #pragma HLS PIPELINE II = 1
                    int pos0= i + j * 4;
                    data0 = buff0[p+(BUFF0_DIM3*(BUFF0_DIM2-1))][(ACC0_M7/BUFF0_DIM3/NUM_PER_64-1)][pos0];
                    data1 = buff0[p+(BUFF0_DIM3*(BUFF0_DIM2-1)+(BUFF0_DIM2/FC_B))][(ACC0_M7/BUFF0_DIM3/NUM_PER_64-1)][pos0];
                    temp_data[0][i*2]  =data0(63,56); 
                    temp_data[0][i*2+1]=data0(127,120);
                    temp_data[1][i*2]  =data1(63,56); 
                    temp_data[1][i*2+1]=data1(127,120);
                    if(i==3){
                        int pos1=j+p*(FC_K/FC_B/NUM_PER_64);
                        for (int un=0; un<NUM_PER_64;un++){
                        #pragma HLS UNROLL factor=NUM_PER_64
                            temp_out0(un*8+7,8*un)=temp_data[0][un];
                            temp_out1(un*8+7,8*un)=temp_data[1][un];
                        }            
                        buff_fc0[0][pos1]=temp_out0;
                        buff_fc0[1][pos1]=temp_out1;
                    } 
                }
            }
        }
        
    }
}

void buf_move(ap_uint<PLIO_WIDTH_128> buff[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],ap_uint<PLIO_WIDTH_128> buff0[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0],const bool enable){
#pragma HLS inline off
    if (enable){
        
        for (int j = 0; j < BUF0_TYPE0_DIM1; j++) {
            for (int i = 0; i < BUF0_TYPE0_DIM0; i++) { 
            #pragma HLS PIPELINE II = 1
                for (int un = 0; un < BUFF0_DIM3*BUFF0_DIM2; un++) {
                    buff0[un][j][i] = buff[un][j][i];
                } 
            }
        }
    }
}

template<int NC>
void compute(axis_stream_plio_128& Acc0_txA0_0, axis_stream_plio_128& Acc0_txA0_1, axis_stream_plio_128& Acc0_txA0_2, axis_stream_plio_128& Acc0_txA0_3, axis_stream_plio_128& Acc0_txA0_4, axis_stream_plio_128& Acc0_txA0_5,  
             axis_stream_plio_128& Acc0_txA1_0, axis_stream_plio_128& Acc0_txA1_1, axis_stream_plio_128& Acc0_txA1_2, axis_stream_plio_128& Acc0_txA1_3, axis_stream_plio_128& Acc0_txA1_4, axis_stream_plio_128& Acc0_txA1_5,  
             axis_stream_plio_128& Acc0_txA2_0, axis_stream_plio_128& Acc0_txA2_1, axis_stream_plio_128& Acc0_txA2_2, axis_stream_plio_128& Acc0_txA2_3, axis_stream_plio_128& Acc0_txA2_4, axis_stream_plio_128& Acc0_txA2_5,  
             axis_stream_plio_128& Acc0_txA3_0, axis_stream_plio_128& Acc0_txA3_1, axis_stream_plio_128& Acc0_txA3_2, axis_stream_plio_128& Acc0_txA3_3, axis_stream_plio_128& Acc0_txA3_4, axis_stream_plio_128& Acc0_txA3_5,  
             axis_stream_plio_128& Acc0_txA4_0, axis_stream_plio_128& Acc0_txA4_1, axis_stream_plio_128& Acc0_txA4_2, axis_stream_plio_128& Acc0_txA4_3, axis_stream_plio_128& Acc0_txA4_4, axis_stream_plio_128& Acc0_txA4_5,  
             axis_stream_plio_128& Acc0_txA5_0, axis_stream_plio_128& Acc0_txA5_1, axis_stream_plio_128& Acc0_txA5_2, axis_stream_plio_128& Acc0_txA5_3, axis_stream_plio_128& Acc0_txA5_4, axis_stream_plio_128& Acc0_txA5_5,  
             axis_stream_plio_128& Acc0_txB0_0, axis_stream_plio_128& Acc0_txB1_0, axis_stream_plio_128& Acc0_txB2_0, axis_stream_plio_128& Acc0_txB3_0, axis_stream_plio_128& Acc0_txB4_0, axis_stream_plio_128& Acc0_txB5_0,  
             axis_stream_plio_128& Acc0_txB0_1, axis_stream_plio_128& Acc0_txB1_1, axis_stream_plio_128& Acc0_txB2_1, axis_stream_plio_128& Acc0_txB3_1, axis_stream_plio_128& Acc0_txB4_1, axis_stream_plio_128& Acc0_txB5_1,  
             axis_stream_plio_128& Acc0_txB0_2, axis_stream_plio_128& Acc0_txB1_2, axis_stream_plio_128& Acc0_txB2_2, axis_stream_plio_128& Acc0_txB3_2, axis_stream_plio_128& Acc0_txB4_2, axis_stream_plio_128& Acc0_txB5_2,  
             axis_stream_plio_128& Acc0_txB0_3, axis_stream_plio_128& Acc0_txB1_3, axis_stream_plio_128& Acc0_txB2_3, axis_stream_plio_128& Acc0_txB3_3, axis_stream_plio_128& Acc0_txB4_3, axis_stream_plio_128& Acc0_txB5_3,  
             axis_stream_plio_128& Acc0_txB0_4, axis_stream_plio_128& Acc0_txB1_4, axis_stream_plio_128& Acc0_txB2_4, axis_stream_plio_128& Acc0_txB3_4, axis_stream_plio_128& Acc0_txB4_4, axis_stream_plio_128& Acc0_txB5_4,  
             axis_stream_plio_128& Acc0_txB0_5, axis_stream_plio_128& Acc0_txB1_5, axis_stream_plio_128& Acc0_txB2_5, axis_stream_plio_128& Acc0_txB3_5, axis_stream_plio_128& Acc0_txB4_5, axis_stream_plio_128& Acc0_txB5_5,  
             axis_stream_plio_128& Acc0_txB0_6, axis_stream_plio_128& Acc0_txB1_6, axis_stream_plio_128& Acc0_txB2_6, axis_stream_plio_128& Acc0_txB3_6, axis_stream_plio_128& Acc0_txB4_6, axis_stream_plio_128& Acc0_txB5_6,  
             axis_stream_plio_128& Acc0_txB0_7, axis_stream_plio_128& Acc0_txB1_7, axis_stream_plio_128& Acc0_txB2_7, axis_stream_plio_128& Acc0_txB3_7, axis_stream_plio_128& Acc0_txB4_7, axis_stream_plio_128& Acc0_txB5_7,  
             axis_stream_plio_128& Acc0_txB0_8, axis_stream_plio_128& Acc0_txB1_8, axis_stream_plio_128& Acc0_txB2_8, axis_stream_plio_128& Acc0_txB3_8, axis_stream_plio_128& Acc0_txB4_8, axis_stream_plio_128& Acc0_txB5_8,  
             axis_stream_plio_128& Acc0_txB0_9, axis_stream_plio_128& Acc0_txB1_9, axis_stream_plio_128& Acc0_txB2_9, axis_stream_plio_128& Acc0_txB3_9, axis_stream_plio_128& Acc0_txB4_9, axis_stream_plio_128& Acc0_txB5_9,  
             axis_stream_plio_128& Acc0_txB0_10, axis_stream_plio_128& Acc0_txB1_10, axis_stream_plio_128& Acc0_txB2_10, axis_stream_plio_128& Acc0_txB3_10, axis_stream_plio_128& Acc0_txB4_10, axis_stream_plio_128& Acc0_txB5_10,  
             axis_stream_plio_128& Acc0_txB0_11, axis_stream_plio_128& Acc0_txB1_11, axis_stream_plio_128& Acc0_txB2_11, axis_stream_plio_128& Acc0_txB3_11, axis_stream_plio_128& Acc0_txB4_11, axis_stream_plio_128& Acc0_txB5_11,  
             axis_stream_plio_128& Acc0_rxC0_0, axis_stream_plio_128& Acc0_rxC0_1, axis_stream_plio_128& Acc0_rxC0_2, axis_stream_plio_128& Acc0_rxC0_3, axis_stream_plio_128& Acc0_rxC0_4, axis_stream_plio_128& Acc0_rxC0_5,  
             axis_stream_plio_128& Acc0_rxC1_0, axis_stream_plio_128& Acc0_rxC1_1, axis_stream_plio_128& Acc0_rxC1_2, axis_stream_plio_128& Acc0_rxC1_3, axis_stream_plio_128& Acc0_rxC1_4, axis_stream_plio_128& Acc0_rxC1_5,  
             axis_stream_plio_128& Acc0_rxC2_0, axis_stream_plio_128& Acc0_rxC2_1, axis_stream_plio_128& Acc0_rxC2_2, axis_stream_plio_128& Acc0_rxC2_3, axis_stream_plio_128& Acc0_rxC2_4, axis_stream_plio_128& Acc0_rxC2_5,  
             axis_stream_plio_128& Acc0_rxC3_0, axis_stream_plio_128& Acc0_rxC3_1, axis_stream_plio_128& Acc0_rxC3_2, axis_stream_plio_128& Acc0_rxC3_3, axis_stream_plio_128& Acc0_rxC3_4, axis_stream_plio_128& Acc0_rxC3_5,  
             axis_stream_plio_128& Acc0_rxC4_0, axis_stream_plio_128& Acc0_rxC4_1, axis_stream_plio_128& Acc0_rxC4_2, axis_stream_plio_128& Acc0_rxC4_3, axis_stream_plio_128& Acc0_rxC4_4, axis_stream_plio_128& Acc0_rxC4_5,  
             axis_stream_plio_128& Acc0_rxC5_0, axis_stream_plio_128& Acc0_rxC5_1, axis_stream_plio_128& Acc0_rxC5_2, axis_stream_plio_128& Acc0_rxC5_3, axis_stream_plio_128& Acc0_rxC5_4, axis_stream_plio_128& Acc0_rxC5_5,  
             ap_uint<PLIO_WIDTH_128> buff[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0], ap_uint<PLIO_WIDTH_64> buff_fc[FC_B][FC_K/FC_B/NUM_PER_64], const int block, const bool enable){
    
    //LHS Partition: [A x part_A x B x part_B] where part-B is consecutive, similar for RHS and OUT
    ap_uint<PLIO_WIDTH_128> buff0[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff1[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff2[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff2 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff2 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff3[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff3 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff3 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff4[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff4 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff4 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff5[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff5 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff5 complete dim=1
    buf_move(buff,buff0,enable);

    for (int j = 0; j < block; j++) { 
        {   
            Acc0_send_LHS_type0_top<0>(buff0, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff1, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 1, 0, 0, 0, 0, 0, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff2, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff0, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff1, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 1, 0, 0, 0, 0, 0, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff3, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff0, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff1, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 1, 0, 0, 0, 0, 0, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff4, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff2, 1, 1, 1, 4, 6,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 1, 1, enable);

            Acc0_send_RHS_type0_top<0>(buff3, 1, 1, 1, 16, 4,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            1, 0, 0, 1, 1, 1, 1, 1, enable);

            Acc0_receive_OUT_type0_top<0>(buff0, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff2, 1, 1, 1, 4, 6,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            1, 0, 1, enable);

            Acc0_send_RHS_type0_top<0>(buff3, 1, 1, 1, 16, 4,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            1, 0, 1, 0, 1, 1, 1, 1, enable);

            Acc0_receive_OUT_type0_top<0>(buff1, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff2, 1, 1, 1, 4, 6,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            1, 1, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff3, 1, 1, 1, 16, 4,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            1, 0, 1, 1, 0, 1, 1, 1, enable);

            Acc0_receive_OUT_type0_top<0>(buff5, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff0, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff4, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 0, 1, 1, 1, 0, 1, 1, enable);

            Acc0_receive_OUT_type0_top<0>(buff2, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 1, 1, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff1, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff4, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 0, 1, 1, 1, 1, 0, 1, enable);

            Acc0_receive_OUT_type0_top<0>(buff2, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            1, 0, 1, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff5, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff4, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 0, 1, 1, 1, 1, 1, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff2, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            1, 1, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff2, 1, 1, 1, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff3, 1, 1, 1, 4, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 1, 0, 0, 0, 0, 0, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff0, 1, 1, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff0, 1, 1, 4, 4, 16,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff1, 1, 1, 4, 4, 64,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 1, 0, 0, 0, 0, 0, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff2, 1, 1, 4, 4, 64,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        {   
            Acc0_send_LHS_type0_top<0>(buff2, 1, 4, 1, 4, 64,
            Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
            Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
            Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
            Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
            Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
            Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
            0, 0, 0, enable);

            Acc0_send_RHS_type0_top<0>(buff3, 1, 4, 1, 16, 16,
            Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
            Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
            Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
            Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
            Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
            Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
            Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
            Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
            Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
            Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
            Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
            Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
            0, 1, 0, 0, 0, 0, 0, 0, enable);

            Acc0_receive_OUT_type0_top<0>(buff0, 1, 4, 1, 4, 16,
            Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
            Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
            Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
            Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
            Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
            Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
            0, 0, 0, enable);
        }
        
    }

    m2vector(buff0, buff_fc,enable);

}


void send_FC(ap_uint<PLIO_WIDTH_64>  buff_fc[FC_B][FC_K/FC_B/NUM_PER_64], axis_stream_plio_128& txA0, axis_stream_plio_128& txA1, bool enable){
   #pragma HLS inline off 
   if(enable){
        data_plio_128 data0;
        data_plio_128 data1;
        axis_pkt_128 tmp0;
        axis_pkt_128 tmp1;
        for (int i = 0; i < (FC_K/FC_B/NUM_PER_64); i++) {
        #pragma HLS PIPELINE II = 1
            data0(63,0) = buff_fc[0][i];
            data0(127,64) = ap_uint<64>(0);

            data1(63,0) = buff_fc[1][i];
            data1(127,64) = ap_uint<64>(0);

            tmp0.data  =  data0;
            tmp0.keep  = -1;
            txA0.write(tmp0);

            tmp1.data  =  data1;
            tmp1.keep  = -1;
            txA1.write(tmp1);  
        }
   }
}

void receive_FC(axis_stream_C& dataC_out, axis_stream_plio_128& rxC0, axis_stream_plio_128& rxC1, 
                axis_stream_plio_128& rxC2, axis_stream_plio_128& rxC3, axis_stream_plio_128& rxC4, const bool enable){
   #pragma HLS inline off 
    if(enable){
        axis_pkt_128 tmp0;
        axis_pkt_128 tmp1;
        axis_pkt_128 tmp2;
        axis_pkt_128 tmp3;
        axis_pkt_128 tmp4;
        ap_uint<PLIO_WIDTH_128> data_temp0;

        ap_uint<PLIO_WIDTH_128> c_buf[FC_C][FC_N/FC_C/4];
        #pragma HLS ARRAY_PARTITION variable=c_buf complete dim=1

        for (int j = 0; j < (FC_N/FC_C/4); j++) {
        #pragma HLS PIPELINE II = 1
            tmp0=rxC0.read();
            tmp1=rxC1.read();
            tmp2=rxC2.read();
            tmp3=rxC3.read();
            tmp4=rxC4.read();
            c_buf[0][j]=tmp0.data;
            c_buf[1][j]=tmp1.data;
            c_buf[2][j]=tmp2.data;
            c_buf[3][j]=tmp3.data;
            c_buf[4][j]=tmp4.data;
        }
        for (int i = 0; i < (FC_C); i++) {
            for (int j = 0; j < (FC_N/FC_C/4); j++) {
            #pragma HLS PIPELINE II = 1
                data_temp0=c_buf[i][j];
                dataC_out.write(data_temp0);
            }
        }
        
    }
}

template<int NC>
void FC_top(ap_uint<PLIO_WIDTH_64>  buff_fc[FC_B][FC_K/FC_B/NUM_PER_64], axis_stream_plio_128& txA0, axis_stream_plio_128& txA1,
            axis_stream_plio_128& rxC0, axis_stream_plio_128& rxC1, axis_stream_plio_128& rxC2, axis_stream_plio_128& rxC3, 
            axis_stream_plio_128& rxC4, axis_stream_C& dataC_out, const bool enable){
    
    send_FC(buff_fc, txA0, txA1, enable);
    receive_FC(dataC_out, rxC0, rxC1, rxC2, rxC3, rxC4, enable);

}

void forward_control(axis_stream_plio_128& Acc0_txA0_0, axis_stream_plio_128& Acc0_txA0_1, axis_stream_plio_128& Acc0_txA0_2, axis_stream_plio_128& Acc0_txA0_3, axis_stream_plio_128& Acc0_txA0_4, axis_stream_plio_128& Acc0_txA0_5,  
                     axis_stream_plio_128& Acc0_txA1_0, axis_stream_plio_128& Acc0_txA1_1, axis_stream_plio_128& Acc0_txA1_2, axis_stream_plio_128& Acc0_txA1_3, axis_stream_plio_128& Acc0_txA1_4, axis_stream_plio_128& Acc0_txA1_5,  
                     axis_stream_plio_128& Acc0_txA2_0, axis_stream_plio_128& Acc0_txA2_1, axis_stream_plio_128& Acc0_txA2_2, axis_stream_plio_128& Acc0_txA2_3, axis_stream_plio_128& Acc0_txA2_4, axis_stream_plio_128& Acc0_txA2_5,  
                     axis_stream_plio_128& Acc0_txA3_0, axis_stream_plio_128& Acc0_txA3_1, axis_stream_plio_128& Acc0_txA3_2, axis_stream_plio_128& Acc0_txA3_3, axis_stream_plio_128& Acc0_txA3_4, axis_stream_plio_128& Acc0_txA3_5,  
                     axis_stream_plio_128& Acc0_txA4_0, axis_stream_plio_128& Acc0_txA4_1, axis_stream_plio_128& Acc0_txA4_2, axis_stream_plio_128& Acc0_txA4_3, axis_stream_plio_128& Acc0_txA4_4, axis_stream_plio_128& Acc0_txA4_5,  
                     axis_stream_plio_128& Acc0_txA5_0, axis_stream_plio_128& Acc0_txA5_1, axis_stream_plio_128& Acc0_txA5_2, axis_stream_plio_128& Acc0_txA5_3, axis_stream_plio_128& Acc0_txA5_4, axis_stream_plio_128& Acc0_txA5_5,  
                     axis_stream_plio_128& Acc0_txB0_0, axis_stream_plio_128& Acc0_txB1_0, axis_stream_plio_128& Acc0_txB2_0, axis_stream_plio_128& Acc0_txB3_0, axis_stream_plio_128& Acc0_txB4_0, axis_stream_plio_128& Acc0_txB5_0,  
                     axis_stream_plio_128& Acc0_txB0_1, axis_stream_plio_128& Acc0_txB1_1, axis_stream_plio_128& Acc0_txB2_1, axis_stream_plio_128& Acc0_txB3_1, axis_stream_plio_128& Acc0_txB4_1, axis_stream_plio_128& Acc0_txB5_1,  
                     axis_stream_plio_128& Acc0_txB0_2, axis_stream_plio_128& Acc0_txB1_2, axis_stream_plio_128& Acc0_txB2_2, axis_stream_plio_128& Acc0_txB3_2, axis_stream_plio_128& Acc0_txB4_2, axis_stream_plio_128& Acc0_txB5_2,  
                     axis_stream_plio_128& Acc0_txB0_3, axis_stream_plio_128& Acc0_txB1_3, axis_stream_plio_128& Acc0_txB2_3, axis_stream_plio_128& Acc0_txB3_3, axis_stream_plio_128& Acc0_txB4_3, axis_stream_plio_128& Acc0_txB5_3,  
                     axis_stream_plio_128& Acc0_txB0_4, axis_stream_plio_128& Acc0_txB1_4, axis_stream_plio_128& Acc0_txB2_4, axis_stream_plio_128& Acc0_txB3_4, axis_stream_plio_128& Acc0_txB4_4, axis_stream_plio_128& Acc0_txB5_4,  
                     axis_stream_plio_128& Acc0_txB0_5, axis_stream_plio_128& Acc0_txB1_5, axis_stream_plio_128& Acc0_txB2_5, axis_stream_plio_128& Acc0_txB3_5, axis_stream_plio_128& Acc0_txB4_5, axis_stream_plio_128& Acc0_txB5_5,  
                     axis_stream_plio_128& Acc0_txB0_6, axis_stream_plio_128& Acc0_txB1_6, axis_stream_plio_128& Acc0_txB2_6, axis_stream_plio_128& Acc0_txB3_6, axis_stream_plio_128& Acc0_txB4_6, axis_stream_plio_128& Acc0_txB5_6,  
                     axis_stream_plio_128& Acc0_txB0_7, axis_stream_plio_128& Acc0_txB1_7, axis_stream_plio_128& Acc0_txB2_7, axis_stream_plio_128& Acc0_txB3_7, axis_stream_plio_128& Acc0_txB4_7, axis_stream_plio_128& Acc0_txB5_7,  
                     axis_stream_plio_128& Acc0_txB0_8, axis_stream_plio_128& Acc0_txB1_8, axis_stream_plio_128& Acc0_txB2_8, axis_stream_plio_128& Acc0_txB3_8, axis_stream_plio_128& Acc0_txB4_8, axis_stream_plio_128& Acc0_txB5_8,  
                     axis_stream_plio_128& Acc0_txB0_9, axis_stream_plio_128& Acc0_txB1_9, axis_stream_plio_128& Acc0_txB2_9, axis_stream_plio_128& Acc0_txB3_9, axis_stream_plio_128& Acc0_txB4_9, axis_stream_plio_128& Acc0_txB5_9,  
                     axis_stream_plio_128& Acc0_txB0_10, axis_stream_plio_128& Acc0_txB1_10, axis_stream_plio_128& Acc0_txB2_10, axis_stream_plio_128& Acc0_txB3_10, axis_stream_plio_128& Acc0_txB4_10, axis_stream_plio_128& Acc0_txB5_10,  
                     axis_stream_plio_128& Acc0_txB0_11, axis_stream_plio_128& Acc0_txB1_11, axis_stream_plio_128& Acc0_txB2_11, axis_stream_plio_128& Acc0_txB3_11, axis_stream_plio_128& Acc0_txB4_11, axis_stream_plio_128& Acc0_txB5_11,  
                     axis_stream_plio_128& Acc0_rxC0_0, axis_stream_plio_128& Acc0_rxC0_1, axis_stream_plio_128& Acc0_rxC0_2, axis_stream_plio_128& Acc0_rxC0_3, axis_stream_plio_128& Acc0_rxC0_4, axis_stream_plio_128& Acc0_rxC0_5,  
                     axis_stream_plio_128& Acc0_rxC1_0, axis_stream_plio_128& Acc0_rxC1_1, axis_stream_plio_128& Acc0_rxC1_2, axis_stream_plio_128& Acc0_rxC1_3, axis_stream_plio_128& Acc0_rxC1_4, axis_stream_plio_128& Acc0_rxC1_5,  
                     axis_stream_plio_128& Acc0_rxC2_0, axis_stream_plio_128& Acc0_rxC2_1, axis_stream_plio_128& Acc0_rxC2_2, axis_stream_plio_128& Acc0_rxC2_3, axis_stream_plio_128& Acc0_rxC2_4, axis_stream_plio_128& Acc0_rxC2_5,  
                     axis_stream_plio_128& Acc0_rxC3_0, axis_stream_plio_128& Acc0_rxC3_1, axis_stream_plio_128& Acc0_rxC3_2, axis_stream_plio_128& Acc0_rxC3_3, axis_stream_plio_128& Acc0_rxC3_4, axis_stream_plio_128& Acc0_rxC3_5,  
                     axis_stream_plio_128& Acc0_rxC4_0, axis_stream_plio_128& Acc0_rxC4_1, axis_stream_plio_128& Acc0_rxC4_2, axis_stream_plio_128& Acc0_rxC4_3, axis_stream_plio_128& Acc0_rxC4_4, axis_stream_plio_128& Acc0_rxC4_5,  
                     axis_stream_plio_128& Acc0_rxC5_0, axis_stream_plio_128& Acc0_rxC5_1, axis_stream_plio_128& Acc0_rxC5_2, axis_stream_plio_128& Acc0_rxC5_3, axis_stream_plio_128& Acc0_rxC5_4, axis_stream_plio_128& Acc0_rxC5_5,  
                     axis_stream_plio_128& Acc1_txA0_0, axis_stream_plio_128& Acc1_txA0_1, axis_stream_plio_128& Acc1_txA0_2, axis_stream_plio_128& Acc1_txA0_3, axis_stream_plio_128& Acc1_txA0_4, axis_stream_plio_128& Acc1_txA0_5,  
                     axis_stream_plio_128& Acc1_rxC0_0, axis_stream_plio_128& Acc1_rxC0_1, axis_stream_plio_128& Acc1_rxC0_2, axis_stream_plio_128& Acc1_rxC0_3, axis_stream_plio_128& Acc1_rxC0_4, axis_stream_plio_128& Acc1_rxC0_5,  
                     axis_stream_plio_128& Acc2_txA0_0, axis_stream_plio_128& Acc2_txA0_1,  
                     axis_stream_plio_128& Acc2_rxC0_0, axis_stream_plio_128& Acc2_rxC0_1, axis_stream_plio_128& Acc2_rxC0_2, axis_stream_plio_128& Acc2_rxC0_3, axis_stream_plio_128& Acc2_rxC0_4,  
                     axis_stream_A& dataA_in0, axis_stream_A& dataA_in1, axis_stream_A& dataA_in2, axis_stream_C& dataC_out, const int batch, const int block, const int iteration){
    

    //LHS Partition: [A x part_A x B x part_B] where part-B is consecutive, similar for RHS and OUT
    ap_uint<PLIO_WIDTH_128> buff0[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff1[BUFF0_DIM3*BUFF0_DIM2][BUF0_TYPE0_DIM1][BUF0_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0 complete dim=1

    ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64];
    #pragma HLS bind_storage variable=buff_fc0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff_fc0 complete dim=1 

    ap_uint<PLIO_WIDTH_64> buff_fc1[FC_B][FC_K/FC_B/NUM_PER_64];
    #pragma HLS bind_storage variable=buff_fc1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff_fc1 complete dim=1

    for (int iter = 0; iter < iteration; iter++) {
        for (int i = 0; i < batch + 2; i++) {
            bool enable_load = ( i < batch );
            bool enable_comp = ( i > 0 && i < (batch+1) );
            bool enable_store = ( i > 1 );
            if (i%2==0){
                image_pe<0>(dataA_in0, dataA_in1, dataA_in2,Acc1_txA0_0,Acc1_txA0_1,Acc1_txA0_2,Acc1_txA0_3,Acc1_txA0_4,Acc1_txA0_5,Acc1_rxC0_0,Acc1_rxC0_1,Acc1_rxC0_2,Acc1_rxC0_3,Acc1_rxC0_4,Acc1_rxC0_5,buff0, enable_load);
                compute<0>(Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
                Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
                Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
                Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
                Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
                Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
                Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
                Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
                Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
                Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
                Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
                Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
                Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
                Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
                Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
                Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
                Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
                Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
                Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
                Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
                Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
                Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
                Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
                Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
                buff1, buff_fc0, block, enable_comp);
                FC_top<0>(buff_fc1,Acc2_txA0_0,Acc2_txA0_1,Acc2_rxC0_0,Acc2_rxC0_1,Acc2_rxC0_2,Acc2_rxC0_3,Acc2_rxC0_4,dataC_out, enable_store);
            }
            else{
                image_pe<0>(dataA_in0, dataA_in1, dataA_in2,Acc1_txA0_0,Acc1_txA0_1,Acc1_txA0_2,Acc1_txA0_3,Acc1_txA0_4,Acc1_txA0_5,Acc1_rxC0_0,Acc1_rxC0_1,Acc1_rxC0_2,Acc1_rxC0_3,Acc1_rxC0_4,Acc1_rxC0_5,buff1, enable_load);
                compute<0>(Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
                Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
                Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
                Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
                Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
                Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
                Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
                Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
                Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
                Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
                Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
                Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
                Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
                Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
                Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
                Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
                Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
                Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
                Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
                Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
                Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
                Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
                Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
                Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
                buff0, buff_fc1, block, enable_comp);
                FC_top<0>(buff_fc0,Acc2_txA0_0,Acc2_txA0_1,Acc2_rxC0_0,Acc2_rxC0_1,Acc2_rxC0_2,Acc2_rxC0_3,Acc2_rxC0_4,dataC_out, enable_store);
            }
        }
    }          
}









void pl_engine( ap_uint<AXI_WIDTH_A>* ina0, ap_uint<AXI_WIDTH_A>* ina1, ap_uint<AXI_WIDTH_A>* ina2, ap_uint<AXI_WIDTH_C>* out0, 
                axis_stream_plio_128& Acc0_txA0_0, axis_stream_plio_128& Acc0_txA0_1, axis_stream_plio_128& Acc0_txA0_2, axis_stream_plio_128& Acc0_txA0_3, axis_stream_plio_128& Acc0_txA0_4, axis_stream_plio_128& Acc0_txA0_5,  
                axis_stream_plio_128& Acc0_txA1_0, axis_stream_plio_128& Acc0_txA1_1, axis_stream_plio_128& Acc0_txA1_2, axis_stream_plio_128& Acc0_txA1_3, axis_stream_plio_128& Acc0_txA1_4, axis_stream_plio_128& Acc0_txA1_5,  
                axis_stream_plio_128& Acc0_txA2_0, axis_stream_plio_128& Acc0_txA2_1, axis_stream_plio_128& Acc0_txA2_2, axis_stream_plio_128& Acc0_txA2_3, axis_stream_plio_128& Acc0_txA2_4, axis_stream_plio_128& Acc0_txA2_5,  
                axis_stream_plio_128& Acc0_txA3_0, axis_stream_plio_128& Acc0_txA3_1, axis_stream_plio_128& Acc0_txA3_2, axis_stream_plio_128& Acc0_txA3_3, axis_stream_plio_128& Acc0_txA3_4, axis_stream_plio_128& Acc0_txA3_5,  
                axis_stream_plio_128& Acc0_txA4_0, axis_stream_plio_128& Acc0_txA4_1, axis_stream_plio_128& Acc0_txA4_2, axis_stream_plio_128& Acc0_txA4_3, axis_stream_plio_128& Acc0_txA4_4, axis_stream_plio_128& Acc0_txA4_5,  
                axis_stream_plio_128& Acc0_txA5_0, axis_stream_plio_128& Acc0_txA5_1, axis_stream_plio_128& Acc0_txA5_2, axis_stream_plio_128& Acc0_txA5_3, axis_stream_plio_128& Acc0_txA5_4, axis_stream_plio_128& Acc0_txA5_5,  
                axis_stream_plio_128& Acc0_txB0_0, axis_stream_plio_128& Acc0_txB1_0, axis_stream_plio_128& Acc0_txB2_0, axis_stream_plio_128& Acc0_txB3_0, axis_stream_plio_128& Acc0_txB4_0, axis_stream_plio_128& Acc0_txB5_0,  
                axis_stream_plio_128& Acc0_txB0_1, axis_stream_plio_128& Acc0_txB1_1, axis_stream_plio_128& Acc0_txB2_1, axis_stream_plio_128& Acc0_txB3_1, axis_stream_plio_128& Acc0_txB4_1, axis_stream_plio_128& Acc0_txB5_1,  
                axis_stream_plio_128& Acc0_txB0_2, axis_stream_plio_128& Acc0_txB1_2, axis_stream_plio_128& Acc0_txB2_2, axis_stream_plio_128& Acc0_txB3_2, axis_stream_plio_128& Acc0_txB4_2, axis_stream_plio_128& Acc0_txB5_2,  
                axis_stream_plio_128& Acc0_txB0_3, axis_stream_plio_128& Acc0_txB1_3, axis_stream_plio_128& Acc0_txB2_3, axis_stream_plio_128& Acc0_txB3_3, axis_stream_plio_128& Acc0_txB4_3, axis_stream_plio_128& Acc0_txB5_3,  
                axis_stream_plio_128& Acc0_txB0_4, axis_stream_plio_128& Acc0_txB1_4, axis_stream_plio_128& Acc0_txB2_4, axis_stream_plio_128& Acc0_txB3_4, axis_stream_plio_128& Acc0_txB4_4, axis_stream_plio_128& Acc0_txB5_4,  
                axis_stream_plio_128& Acc0_txB0_5, axis_stream_plio_128& Acc0_txB1_5, axis_stream_plio_128& Acc0_txB2_5, axis_stream_plio_128& Acc0_txB3_5, axis_stream_plio_128& Acc0_txB4_5, axis_stream_plio_128& Acc0_txB5_5,  
                axis_stream_plio_128& Acc0_txB0_6, axis_stream_plio_128& Acc0_txB1_6, axis_stream_plio_128& Acc0_txB2_6, axis_stream_plio_128& Acc0_txB3_6, axis_stream_plio_128& Acc0_txB4_6, axis_stream_plio_128& Acc0_txB5_6,  
                axis_stream_plio_128& Acc0_txB0_7, axis_stream_plio_128& Acc0_txB1_7, axis_stream_plio_128& Acc0_txB2_7, axis_stream_plio_128& Acc0_txB3_7, axis_stream_plio_128& Acc0_txB4_7, axis_stream_plio_128& Acc0_txB5_7,  
                axis_stream_plio_128& Acc0_txB0_8, axis_stream_plio_128& Acc0_txB1_8, axis_stream_plio_128& Acc0_txB2_8, axis_stream_plio_128& Acc0_txB3_8, axis_stream_plio_128& Acc0_txB4_8, axis_stream_plio_128& Acc0_txB5_8,  
                axis_stream_plio_128& Acc0_txB0_9, axis_stream_plio_128& Acc0_txB1_9, axis_stream_plio_128& Acc0_txB2_9, axis_stream_plio_128& Acc0_txB3_9, axis_stream_plio_128& Acc0_txB4_9, axis_stream_plio_128& Acc0_txB5_9,  
                axis_stream_plio_128& Acc0_txB0_10, axis_stream_plio_128& Acc0_txB1_10, axis_stream_plio_128& Acc0_txB2_10, axis_stream_plio_128& Acc0_txB3_10, axis_stream_plio_128& Acc0_txB4_10, axis_stream_plio_128& Acc0_txB5_10,  
                axis_stream_plio_128& Acc0_txB0_11, axis_stream_plio_128& Acc0_txB1_11, axis_stream_plio_128& Acc0_txB2_11, axis_stream_plio_128& Acc0_txB3_11, axis_stream_plio_128& Acc0_txB4_11, axis_stream_plio_128& Acc0_txB5_11,  
                axis_stream_plio_128& Acc0_rxC0_0, axis_stream_plio_128& Acc0_rxC0_1, axis_stream_plio_128& Acc0_rxC0_2, axis_stream_plio_128& Acc0_rxC0_3, axis_stream_plio_128& Acc0_rxC0_4, axis_stream_plio_128& Acc0_rxC0_5,  
                axis_stream_plio_128& Acc0_rxC1_0, axis_stream_plio_128& Acc0_rxC1_1, axis_stream_plio_128& Acc0_rxC1_2, axis_stream_plio_128& Acc0_rxC1_3, axis_stream_plio_128& Acc0_rxC1_4, axis_stream_plio_128& Acc0_rxC1_5,  
                axis_stream_plio_128& Acc0_rxC2_0, axis_stream_plio_128& Acc0_rxC2_1, axis_stream_plio_128& Acc0_rxC2_2, axis_stream_plio_128& Acc0_rxC2_3, axis_stream_plio_128& Acc0_rxC2_4, axis_stream_plio_128& Acc0_rxC2_5,  
                axis_stream_plio_128& Acc0_rxC3_0, axis_stream_plio_128& Acc0_rxC3_1, axis_stream_plio_128& Acc0_rxC3_2, axis_stream_plio_128& Acc0_rxC3_3, axis_stream_plio_128& Acc0_rxC3_4, axis_stream_plio_128& Acc0_rxC3_5,  
                axis_stream_plio_128& Acc0_rxC4_0, axis_stream_plio_128& Acc0_rxC4_1, axis_stream_plio_128& Acc0_rxC4_2, axis_stream_plio_128& Acc0_rxC4_3, axis_stream_plio_128& Acc0_rxC4_4, axis_stream_plio_128& Acc0_rxC4_5,  
                axis_stream_plio_128& Acc0_rxC5_0, axis_stream_plio_128& Acc0_rxC5_1, axis_stream_plio_128& Acc0_rxC5_2, axis_stream_plio_128& Acc0_rxC5_3, axis_stream_plio_128& Acc0_rxC5_4, axis_stream_plio_128& Acc0_rxC5_5,  
                axis_stream_plio_128& Acc1_txA0_0, axis_stream_plio_128& Acc1_txA0_1, axis_stream_plio_128& Acc1_txA0_2, axis_stream_plio_128& Acc1_txA0_3, axis_stream_plio_128& Acc1_txA0_4, axis_stream_plio_128& Acc1_txA0_5,  
                axis_stream_plio_128& Acc1_rxC0_0, axis_stream_plio_128& Acc1_rxC0_1, axis_stream_plio_128& Acc1_rxC0_2, axis_stream_plio_128& Acc1_rxC0_3, axis_stream_plio_128& Acc1_rxC0_4, axis_stream_plio_128& Acc1_rxC0_5,  
                axis_stream_plio_128& Acc2_txA0_0, axis_stream_plio_128& Acc2_txA0_1,  
                axis_stream_plio_128& Acc2_rxC0_0, axis_stream_plio_128& Acc2_rxC0_1, axis_stream_plio_128& Acc2_rxC0_2, axis_stream_plio_128& Acc2_rxC0_3, axis_stream_plio_128& Acc2_rxC0_4,  
                const int batch, const int block, const int iteration){

    #pragma HLS interface m_axi offset=slave bundle=gmem0 port=ina0 max_read_burst_length=32 num_read_outstanding=32
    #pragma HLS interface s_axilite bundle=control port=ina0
    #pragma HLS interface m_axi offset=slave bundle=gmem1 port=ina1 max_read_burst_length=32 num_read_outstanding=32
    #pragma HLS interface s_axilite bundle=control port=ina1
    #pragma HLS interface m_axi offset=slave bundle=gmem2 port=ina2 max_read_burst_length=32 num_read_outstanding=32
    #pragma HLS interface s_axilite bundle=control port=ina2
    #pragma HLS interface m_axi offset=slave bundle=gmem3 port=out0 max_write_burst_length=32 num_write_outstanding=32
    #pragma HLS interface s_axilite bundle=control port=out0
    #pragma HLS interface s_axilite bundle=control port=batch
    #pragma HLS interface s_axilite bundle=control port=block
    #pragma HLS interface s_axilite bundle=control port=iteration
    
    #pragma HLS interface axis port=Acc0_txA0_0
    #pragma HLS interface axis port=Acc0_txA0_1
    #pragma HLS interface axis port=Acc0_txA0_2
    #pragma HLS interface axis port=Acc0_txA0_3
    #pragma HLS interface axis port=Acc0_txA0_4
    #pragma HLS interface axis port=Acc0_txA0_5
    #pragma HLS interface axis port=Acc0_txA1_0
    #pragma HLS interface axis port=Acc0_txA1_1
    #pragma HLS interface axis port=Acc0_txA1_2
    #pragma HLS interface axis port=Acc0_txA1_3
    #pragma HLS interface axis port=Acc0_txA1_4
    #pragma HLS interface axis port=Acc0_txA1_5
    #pragma HLS interface axis port=Acc0_txA2_0
    #pragma HLS interface axis port=Acc0_txA2_1
    #pragma HLS interface axis port=Acc0_txA2_2
    #pragma HLS interface axis port=Acc0_txA2_3
    #pragma HLS interface axis port=Acc0_txA2_4
    #pragma HLS interface axis port=Acc0_txA2_5
    #pragma HLS interface axis port=Acc0_txA3_0
    #pragma HLS interface axis port=Acc0_txA3_1
    #pragma HLS interface axis port=Acc0_txA3_2
    #pragma HLS interface axis port=Acc0_txA3_3
    #pragma HLS interface axis port=Acc0_txA3_4
    #pragma HLS interface axis port=Acc0_txA3_5
    #pragma HLS interface axis port=Acc0_txA4_0
    #pragma HLS interface axis port=Acc0_txA4_1
    #pragma HLS interface axis port=Acc0_txA4_2
    #pragma HLS interface axis port=Acc0_txA4_3
    #pragma HLS interface axis port=Acc0_txA4_4
    #pragma HLS interface axis port=Acc0_txA4_5
    #pragma HLS interface axis port=Acc0_txA5_0
    #pragma HLS interface axis port=Acc0_txA5_1
    #pragma HLS interface axis port=Acc0_txA5_2
    #pragma HLS interface axis port=Acc0_txA5_3
    #pragma HLS interface axis port=Acc0_txA5_4
    #pragma HLS interface axis port=Acc0_txA5_5
    #pragma HLS interface axis port=Acc0_txB0_0 
    #pragma HLS interface axis port=Acc0_txB1_0 
    #pragma HLS interface axis port=Acc0_txB2_0 
    #pragma HLS interface axis port=Acc0_txB3_0 
    #pragma HLS interface axis port=Acc0_txB4_0 
    #pragma HLS interface axis port=Acc0_txB5_0 
    #pragma HLS interface axis port=Acc0_txB0_1 
    #pragma HLS interface axis port=Acc0_txB1_1 
    #pragma HLS interface axis port=Acc0_txB2_1 
    #pragma HLS interface axis port=Acc0_txB3_1 
    #pragma HLS interface axis port=Acc0_txB4_1 
    #pragma HLS interface axis port=Acc0_txB5_1 
    #pragma HLS interface axis port=Acc0_txB0_2 
    #pragma HLS interface axis port=Acc0_txB1_2 
    #pragma HLS interface axis port=Acc0_txB2_2 
    #pragma HLS interface axis port=Acc0_txB3_2 
    #pragma HLS interface axis port=Acc0_txB4_2 
    #pragma HLS interface axis port=Acc0_txB5_2 
    #pragma HLS interface axis port=Acc0_txB0_3 
    #pragma HLS interface axis port=Acc0_txB1_3 
    #pragma HLS interface axis port=Acc0_txB2_3 
    #pragma HLS interface axis port=Acc0_txB3_3 
    #pragma HLS interface axis port=Acc0_txB4_3 
    #pragma HLS interface axis port=Acc0_txB5_3 
    #pragma HLS interface axis port=Acc0_txB0_4 
    #pragma HLS interface axis port=Acc0_txB1_4 
    #pragma HLS interface axis port=Acc0_txB2_4 
    #pragma HLS interface axis port=Acc0_txB3_4 
    #pragma HLS interface axis port=Acc0_txB4_4 
    #pragma HLS interface axis port=Acc0_txB5_4 
    #pragma HLS interface axis port=Acc0_txB0_5 
    #pragma HLS interface axis port=Acc0_txB1_5 
    #pragma HLS interface axis port=Acc0_txB2_5 
    #pragma HLS interface axis port=Acc0_txB3_5 
    #pragma HLS interface axis port=Acc0_txB4_5 
    #pragma HLS interface axis port=Acc0_txB5_5 
    #pragma HLS interface axis port=Acc0_txB0_6 
    #pragma HLS interface axis port=Acc0_txB1_6 
    #pragma HLS interface axis port=Acc0_txB2_6 
    #pragma HLS interface axis port=Acc0_txB3_6 
    #pragma HLS interface axis port=Acc0_txB4_6 
    #pragma HLS interface axis port=Acc0_txB5_6 
    #pragma HLS interface axis port=Acc0_txB0_7 
    #pragma HLS interface axis port=Acc0_txB1_7 
    #pragma HLS interface axis port=Acc0_txB2_7 
    #pragma HLS interface axis port=Acc0_txB3_7 
    #pragma HLS interface axis port=Acc0_txB4_7 
    #pragma HLS interface axis port=Acc0_txB5_7 
    #pragma HLS interface axis port=Acc0_txB0_8 
    #pragma HLS interface axis port=Acc0_txB1_8 
    #pragma HLS interface axis port=Acc0_txB2_8 
    #pragma HLS interface axis port=Acc0_txB3_8 
    #pragma HLS interface axis port=Acc0_txB4_8 
    #pragma HLS interface axis port=Acc0_txB5_8 
    #pragma HLS interface axis port=Acc0_txB0_9 
    #pragma HLS interface axis port=Acc0_txB1_9 
    #pragma HLS interface axis port=Acc0_txB2_9 
    #pragma HLS interface axis port=Acc0_txB3_9 
    #pragma HLS interface axis port=Acc0_txB4_9 
    #pragma HLS interface axis port=Acc0_txB5_9 
    #pragma HLS interface axis port=Acc0_txB0_10 
    #pragma HLS interface axis port=Acc0_txB1_10 
    #pragma HLS interface axis port=Acc0_txB2_10 
    #pragma HLS interface axis port=Acc0_txB3_10 
    #pragma HLS interface axis port=Acc0_txB4_10 
    #pragma HLS interface axis port=Acc0_txB5_10 
    #pragma HLS interface axis port=Acc0_txB0_11 
    #pragma HLS interface axis port=Acc0_txB1_11 
    #pragma HLS interface axis port=Acc0_txB2_11 
    #pragma HLS interface axis port=Acc0_txB3_11 
    #pragma HLS interface axis port=Acc0_txB4_11 
    #pragma HLS interface axis port=Acc0_txB5_11 
    #pragma HLS interface axis port=Acc0_rxC0_0  
    #pragma HLS interface axis port=Acc0_rxC0_1  
    #pragma HLS interface axis port=Acc0_rxC0_2  
    #pragma HLS interface axis port=Acc0_rxC0_3  
    #pragma HLS interface axis port=Acc0_rxC0_4  
    #pragma HLS interface axis port=Acc0_rxC0_5  
    #pragma HLS interface axis port=Acc0_rxC1_0  
    #pragma HLS interface axis port=Acc0_rxC1_1  
    #pragma HLS interface axis port=Acc0_rxC1_2  
    #pragma HLS interface axis port=Acc0_rxC1_3  
    #pragma HLS interface axis port=Acc0_rxC1_4  
    #pragma HLS interface axis port=Acc0_rxC1_5  
    #pragma HLS interface axis port=Acc0_rxC2_0  
    #pragma HLS interface axis port=Acc0_rxC2_1  
    #pragma HLS interface axis port=Acc0_rxC2_2  
    #pragma HLS interface axis port=Acc0_rxC2_3  
    #pragma HLS interface axis port=Acc0_rxC2_4  
    #pragma HLS interface axis port=Acc0_rxC2_5  
    #pragma HLS interface axis port=Acc0_rxC3_0  
    #pragma HLS interface axis port=Acc0_rxC3_1  
    #pragma HLS interface axis port=Acc0_rxC3_2  
    #pragma HLS interface axis port=Acc0_rxC3_3  
    #pragma HLS interface axis port=Acc0_rxC3_4  
    #pragma HLS interface axis port=Acc0_rxC3_5  
    #pragma HLS interface axis port=Acc0_rxC4_0  
    #pragma HLS interface axis port=Acc0_rxC4_1  
    #pragma HLS interface axis port=Acc0_rxC4_2  
    #pragma HLS interface axis port=Acc0_rxC4_3  
    #pragma HLS interface axis port=Acc0_rxC4_4  
    #pragma HLS interface axis port=Acc0_rxC4_5  
    #pragma HLS interface axis port=Acc0_rxC5_0  
    #pragma HLS interface axis port=Acc0_rxC5_1  
    #pragma HLS interface axis port=Acc0_rxC5_2  
    #pragma HLS interface axis port=Acc0_rxC5_3  
    #pragma HLS interface axis port=Acc0_rxC5_4  
    #pragma HLS interface axis port=Acc0_rxC5_5  
    

    #pragma HLS interface axis port=Acc1_txA0_0
    #pragma HLS interface axis port=Acc1_txA0_1
    #pragma HLS interface axis port=Acc1_txA0_2
    #pragma HLS interface axis port=Acc1_txA0_3
    #pragma HLS interface axis port=Acc1_txA0_4
    #pragma HLS interface axis port=Acc1_txA0_5
     
    #pragma HLS interface axis port=Acc1_rxC0_0
    #pragma HLS interface axis port=Acc1_rxC0_1
    #pragma HLS interface axis port=Acc1_rxC0_2
    #pragma HLS interface axis port=Acc1_rxC0_3
    #pragma HLS interface axis port=Acc1_rxC0_4
    #pragma HLS interface axis port=Acc1_rxC0_5
     
    #pragma HLS interface axis port=Acc2_txA0_0
    #pragma HLS interface axis port=Acc2_txA0_1
     
    #pragma HLS interface axis port=Acc2_rxC0_0
    #pragma HLS interface axis port=Acc2_rxC0_1
    #pragma HLS interface axis port=Acc2_rxC0_2
    #pragma HLS interface axis port=Acc2_rxC0_3
    #pragma HLS interface axis port=Acc2_rxC0_4
     
    #pragma HLS interface s_axilite bundle=control port=return

    #pragma HLS dataflow
    axis_stream_A dataA_in0;
    axis_stream_A dataA_in1;
    axis_stream_A dataA_in2;
    axis_stream_32 addrA_in0;
    axis_stream_32 addrA_in1;
    axis_stream_32 addrA_in2;

    axis_stream_C dataC_out;
    axis_stream_32 addrC_in;

    address_image_ddr<0>(addrA_in0,batch,iteration);
    load_image_ddr<0>(ina0,addrA_in0,dataA_in0,batch,iteration);

    address_image_ddr<1>(addrA_in1,batch,iteration);
    load_image_ddr<1>(ina1,addrA_in1,dataA_in1,batch,iteration);

    address_image_ddr<2>(addrA_in2,batch,iteration);
    load_image_ddr<2>(ina2,addrA_in2,dataA_in2,batch,iteration);


    forward_control(Acc0_txA0_0, Acc0_txA0_1, Acc0_txA0_2, Acc0_txA0_3, Acc0_txA0_4, Acc0_txA0_5,  
                    Acc0_txA1_0, Acc0_txA1_1, Acc0_txA1_2, Acc0_txA1_3, Acc0_txA1_4, Acc0_txA1_5,  
                    Acc0_txA2_0, Acc0_txA2_1, Acc0_txA2_2, Acc0_txA2_3, Acc0_txA2_4, Acc0_txA2_5,  
                    Acc0_txA3_0, Acc0_txA3_1, Acc0_txA3_2, Acc0_txA3_3, Acc0_txA3_4, Acc0_txA3_5,  
                    Acc0_txA4_0, Acc0_txA4_1, Acc0_txA4_2, Acc0_txA4_3, Acc0_txA4_4, Acc0_txA4_5,  
                    Acc0_txA5_0, Acc0_txA5_1, Acc0_txA5_2, Acc0_txA5_3, Acc0_txA5_4, Acc0_txA5_5,  
                    Acc0_txB0_0, Acc0_txB1_0, Acc0_txB2_0, Acc0_txB3_0, Acc0_txB4_0, Acc0_txB5_0,  
                    Acc0_txB0_1, Acc0_txB1_1, Acc0_txB2_1, Acc0_txB3_1, Acc0_txB4_1, Acc0_txB5_1,  
                    Acc0_txB0_2, Acc0_txB1_2, Acc0_txB2_2, Acc0_txB3_2, Acc0_txB4_2, Acc0_txB5_2,  
                    Acc0_txB0_3, Acc0_txB1_3, Acc0_txB2_3, Acc0_txB3_3, Acc0_txB4_3, Acc0_txB5_3,  
                    Acc0_txB0_4, Acc0_txB1_4, Acc0_txB2_4, Acc0_txB3_4, Acc0_txB4_4, Acc0_txB5_4,  
                    Acc0_txB0_5, Acc0_txB1_5, Acc0_txB2_5, Acc0_txB3_5, Acc0_txB4_5, Acc0_txB5_5,  
                    Acc0_txB0_6, Acc0_txB1_6, Acc0_txB2_6, Acc0_txB3_6, Acc0_txB4_6, Acc0_txB5_6,  
                    Acc0_txB0_7, Acc0_txB1_7, Acc0_txB2_7, Acc0_txB3_7, Acc0_txB4_7, Acc0_txB5_7,  
                    Acc0_txB0_8, Acc0_txB1_8, Acc0_txB2_8, Acc0_txB3_8, Acc0_txB4_8, Acc0_txB5_8,  
                    Acc0_txB0_9, Acc0_txB1_9, Acc0_txB2_9, Acc0_txB3_9, Acc0_txB4_9, Acc0_txB5_9,  
                    Acc0_txB0_10, Acc0_txB1_10, Acc0_txB2_10, Acc0_txB3_10, Acc0_txB4_10, Acc0_txB5_10,  
                    Acc0_txB0_11, Acc0_txB1_11, Acc0_txB2_11, Acc0_txB3_11, Acc0_txB4_11, Acc0_txB5_11,  
                    Acc0_rxC0_0, Acc0_rxC0_1, Acc0_rxC0_2, Acc0_rxC0_3, Acc0_rxC0_4, Acc0_rxC0_5,  
                    Acc0_rxC1_0, Acc0_rxC1_1, Acc0_rxC1_2, Acc0_rxC1_3, Acc0_rxC1_4, Acc0_rxC1_5,  
                    Acc0_rxC2_0, Acc0_rxC2_1, Acc0_rxC2_2, Acc0_rxC2_3, Acc0_rxC2_4, Acc0_rxC2_5,  
                    Acc0_rxC3_0, Acc0_rxC3_1, Acc0_rxC3_2, Acc0_rxC3_3, Acc0_rxC3_4, Acc0_rxC3_5,  
                    Acc0_rxC4_0, Acc0_rxC4_1, Acc0_rxC4_2, Acc0_rxC4_3, Acc0_rxC4_4, Acc0_rxC4_5,  
                    Acc0_rxC5_0, Acc0_rxC5_1, Acc0_rxC5_2, Acc0_rxC5_3, Acc0_rxC5_4, Acc0_rxC5_5,  
                    Acc1_txA0_0, Acc1_txA0_1, Acc1_txA0_2, Acc1_txA0_3, Acc1_txA0_4, Acc1_txA0_5,  
                    Acc1_rxC0_0, Acc1_rxC0_1, Acc1_rxC0_2, Acc1_rxC0_3, Acc1_rxC0_4, Acc1_rxC0_5,  
                    Acc2_txA0_0, Acc2_txA0_1,  
                    Acc2_rxC0_0, Acc2_rxC0_1, Acc2_rxC0_2, Acc2_rxC0_3, Acc2_rxC0_4,  
                    dataA_in0, dataA_in1, dataA_in2, dataC_out, batch, block, iteration);

    address_C_ddr(addrC_in,batch,iteration);
    storeC_ddr(out0,addrC_in,dataC_out,batch,iteration);
}
