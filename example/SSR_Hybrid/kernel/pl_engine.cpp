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
void Acc0_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf0_lhs0_0[(ACC0_M0/PE_OUT_PARTA/NUM_PER_64)][ACC0_K0/PE_OUT_PARTC/(NUM_PER_128/NUM_PER_64)],
                                      ap_uint<PLIO_WIDTH_128> buf1_lhs0_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs0_1[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs0_2[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs1_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs1_1[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs1_2[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs2_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs2_1[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs2_2[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs3_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs3_1[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf1_lhs3_2[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc0_txA0, 
                                      const bool sel, const bool enable){

#pragma HLS inline off

    if(enable){
        
        data_plio_128 data;
        axis_pkt_128 tmp;
        const int buffer_w1 = ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64);
        const int buffer_h1 = ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64;
        for (int x = 0; x < (ACC0_M0/ACC0_A/ACC0_H1); x++) { 
            for (int z = 0; z < (ACC0_N0/ACC0_C/ACC0_W2); z++) {
                for (int y = 0; y < (ACC0_K0/ACC0_B/ACC0_W1); y++) {
                    for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC0_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            if (sel){
                                
                                int pos0=(w1+y*ACC0_LW1);
                                int pos1=(h1+x*ACC0_LH1);
                                data = buf0_lhs0_0[pos1][pos0];
                                
                            }
                            else{
                                int pos0=(w1+y*ACC0_LW1)%buffer_w1;
                                int pos1=(h1+x*ACC0_LH1)%buffer_h1;
                                int temp_b=(w1+y*ACC0_LW1)/buffer_w1;
                                int temp_a=(h1+x*ACC0_LH1)/buffer_h1;

                                if (temp_b==0&&temp_a==0){ 
                                    data = buf1_lhs0_0[pos1][pos0];
                                }
                                else if (temp_b==1&&temp_a==0){ 
                                    data = buf1_lhs0_1[pos1][pos0];
                                }
                                
                                else if (temp_b==2&&temp_a==0){ 
                                    data = buf1_lhs0_2[pos1][pos0];
                                }
                                
                                else if (temp_b==0&&temp_a==1){ 
                                    data = buf1_lhs1_0[pos1][pos0];
                                }
                                
                                else if (temp_b==1&&temp_a==1){ 
                                    data = buf1_lhs1_1[pos1][pos0];
                                }
                                
                                else if (temp_b==2&&temp_a==1){ 
                                    data = buf1_lhs1_2[pos1][pos0];
                                }
                                
                                else if (temp_b==0&&temp_a==2){ 
                                    data = buf1_lhs2_0[pos1][pos0];
                                }
                                
                                else if (temp_b==1&&temp_a==2){ 
                                    data = buf1_lhs2_1[pos1][pos0];
                                }
                                
                                else if (temp_b==2&&temp_a==2){ 
                                    data = buf1_lhs2_2[pos1][pos0];
                                }
                                
                                else if (temp_b==0&&temp_a==3){ 
                                    data = buf1_lhs3_0[pos1][pos0];
                                }
                                
                                else if (temp_b==1&&temp_a==3){ 
                                    data = buf1_lhs3_1[pos1][pos0];
                                }
                                else{
                                    data = buf1_lhs3_2[pos1][pos0];
                                }
                                
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
void Acc0_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf0_lhs[PE_OUT_PARTA*PE_OUT_PARTC][(ACC0_M0/PE_OUT_PARTA/NUM_PER_64)][ACC0_K0/PE_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                             ap_uint<PLIO_WIDTH_128> buf1_lhs[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                             axis_stream_plio_128& Acc0_txA0_0_heads0, axis_stream_plio_128& Acc0_txA0_1_heads0, axis_stream_plio_128& Acc0_txA0_2_heads0, axis_stream_plio_128& Acc0_txA0_3_heads0,  
                             const bool sel, const bool enable){
#pragma HLS inline off
    Acc0_send_LHS_type0<0>(buf0_lhs[0],buf1_lhs[0],buf1_lhs[1],buf1_lhs[2],buf1_lhs[12],buf1_lhs[13],buf1_lhs[14],buf1_lhs[24],buf1_lhs[25],buf1_lhs[26],buf1_lhs[36],buf1_lhs[37],buf1_lhs[38],Acc0_txA0_0_heads0,sel, enable);
    Acc0_send_LHS_type0<1>(buf0_lhs[1],buf1_lhs[3],buf1_lhs[4],buf1_lhs[5],buf1_lhs[15],buf1_lhs[16],buf1_lhs[17],buf1_lhs[27],buf1_lhs[28],buf1_lhs[29],buf1_lhs[39],buf1_lhs[40],buf1_lhs[41],Acc0_txA0_1_heads0,sel, enable);
    Acc0_send_LHS_type0<2>(buf0_lhs[2],buf1_lhs[6],buf1_lhs[7],buf1_lhs[8],buf1_lhs[18],buf1_lhs[19],buf1_lhs[20],buf1_lhs[30],buf1_lhs[31],buf1_lhs[32],buf1_lhs[42],buf1_lhs[43],buf1_lhs[44],Acc0_txA0_2_heads0,sel, enable);
    Acc0_send_LHS_type0<3>(buf0_lhs[3],buf1_lhs[9],buf1_lhs[10],buf1_lhs[11],buf1_lhs[21],buf1_lhs[22],buf1_lhs[23],buf1_lhs[33],buf1_lhs[34],buf1_lhs[35],buf1_lhs[45],buf1_lhs[46],buf1_lhs[47],Acc0_txA0_3_heads0,sel, enable);
      
}




template<int NC>
void Acc0_receive_OUT(axis_stream_plio_128& Acc0_rxC,axis_stream_128& Acc0_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC0_M0/ACC0_A/ACC0_H1); x++) { 
            for (int z = 0; z < (ACC0_N0/ACC0_C/ACC0_W2); z++) {
                for (int y = 0; y < (ACC0_K0/ACC0_B/ACC0_W1); y++) {
                    for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc0_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc0_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc0_receive_buff_type0(axis_stream_128& Acc0_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf_out1_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf_out2_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf_out3_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        const int buffer_w2 = ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64);
        const int buffer_h1 = ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64;
        for (int x = 0; x < (ACC0_M0/ACC0_A/ACC0_H1); x++) { 
            for (int z = 0; z < (ACC0_N0/ACC0_C/ACC0_W2); z++) {          
                for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC0_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc0_sin0.read();
                        
                        int pos0=(w2+z*ACC0_RW2)%buffer_w2;
                        int pos1=(h1+x*ACC0_LH1)%buffer_h1;
                        int temp_c=(w2+z*ACC0_RW2)/buffer_w2;
                        int temp_a=(h1+x*ACC0_LH1)/buffer_h1;
                        if (temp_c==0&&temp_a==0){ 
                            buf_out0_0[pos1][pos0] = data_temp;
                        }
                        
                        
                        else if (temp_c==0&&temp_a==1){ 
                            buf_out1_0[pos1][pos0] = data_temp;
                        }
                        
                        else if (temp_c==0&&temp_a==2){ 
                            buf_out2_0[pos1][pos0] = data_temp;
                        }
                        
                        
                        else{
                            buf_out3_0[pos1][pos0] = data_temp;
                        }
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc0_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc0_rxC0_0_heads0, axis_stream_plio_128& Acc0_rxC0_1_heads0, axis_stream_plio_128& Acc0_rxC0_2_heads0, axis_stream_plio_128& Acc0_rxC0_3_heads0, axis_stream_plio_128& Acc0_rxC0_4_heads0, axis_stream_plio_128& Acc0_rxC0_5_heads0,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc0_plio0_0_heads0_f0;axis_stream_128 Acc0_plio0_1_heads0_f0;axis_stream_128 Acc0_plio0_2_heads0_f0;axis_stream_128 Acc0_plio0_3_heads0_f0;axis_stream_128 Acc0_plio0_4_heads0_f0;axis_stream_128 Acc0_plio0_5_heads0_f0; 
    
    Acc0_receive_OUT<0>(Acc0_rxC0_0_heads0, Acc0_plio0_0_heads0_f0, enable); 
    Acc0_receive_OUT<1>(Acc0_rxC0_1_heads0, Acc0_plio0_1_heads0_f0, enable); 
    Acc0_receive_OUT<2>(Acc0_rxC0_2_heads0, Acc0_plio0_2_heads0_f0, enable); 
    Acc0_receive_OUT<3>(Acc0_rxC0_3_heads0, Acc0_plio0_3_heads0_f0, enable); 
    Acc0_receive_OUT<4>(Acc0_rxC0_4_heads0, Acc0_plio0_4_heads0_f0, enable); 
    Acc0_receive_OUT<5>(Acc0_rxC0_5_heads0, Acc0_plio0_5_heads0_f0, enable); 
    
    
    Acc0_receive_buff_type0<0>(Acc0_plio0_0_heads0_f0,buf_out[0],buf_out[6],buf_out[12],buf_out[18],enable);
    Acc0_receive_buff_type0<1>(Acc0_plio0_1_heads0_f0,buf_out[1],buf_out[7],buf_out[13],buf_out[19],enable);
    Acc0_receive_buff_type0<2>(Acc0_plio0_2_heads0_f0,buf_out[2],buf_out[8],buf_out[14],buf_out[20],enable);
    Acc0_receive_buff_type0<3>(Acc0_plio0_3_heads0_f0,buf_out[3],buf_out[9],buf_out[15],buf_out[21],enable);
    Acc0_receive_buff_type0<4>(Acc0_plio0_4_heads0_f0,buf_out[4],buf_out[10],buf_out[16],buf_out[22],enable);
    Acc0_receive_buff_type0<5>(Acc0_plio0_5_heads0_f0,buf_out[5],buf_out[11],buf_out[17],buf_out[23],enable);
    
}

template<int NC>
void Acc1_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf0_lhs0_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                         ap_uint<PLIO_WIDTH_128> buf1_lhs0_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0*ACC1_HEADS],
                         const int boundary_y, const int boundary_z,
                         axis_stream_plio_128& Acc1_txA0, const bool sel, const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) { 
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int h1 = 0; h1 < ACC1_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC1_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC1_LW1);
                            int pos1=(h1+x*ACC1_LH1);
                            if(sel==0){
                                data = buf0_lhs0_0[pos1][pos0];
                            }
                            else{
                                data = buf1_lhs0_0[pos1][pos0];
                            }  
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc1_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc1_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf0_lhs[BUFF1_LHS_PARTA*BUFF1_LHS_PARTB*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], 
                             ap_uint<PLIO_WIDTH_128> buf1_lhs[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0*ACC1_HEADS],
                             const int boundary_y, const int boundary_z,
                             axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0,  
                             axis_stream_plio_128& Acc1_txA1_0_heads0, axis_stream_plio_128& Acc1_txA1_1_heads0,  
                             axis_stream_plio_128& Acc1_txA2_0_heads0, axis_stream_plio_128& Acc1_txA2_1_heads0,  
                             axis_stream_plio_128& Acc1_txA3_0_heads0, axis_stream_plio_128& Acc1_txA3_1_heads0,  
                             axis_stream_plio_128& Acc1_txA0_0_heads1, axis_stream_plio_128& Acc1_txA0_1_heads1,  
                             axis_stream_plio_128& Acc1_txA1_0_heads1, axis_stream_plio_128& Acc1_txA1_1_heads1,  
                             axis_stream_plio_128& Acc1_txA2_0_heads1, axis_stream_plio_128& Acc1_txA2_1_heads1,  
                             axis_stream_plio_128& Acc1_txA3_0_heads1, axis_stream_plio_128& Acc1_txA3_1_heads1,  
                             axis_stream_plio_128& Acc1_txA0_0_heads2, axis_stream_plio_128& Acc1_txA0_1_heads2,  
                             axis_stream_plio_128& Acc1_txA1_0_heads2, axis_stream_plio_128& Acc1_txA1_1_heads2,  
                             axis_stream_plio_128& Acc1_txA2_0_heads2, axis_stream_plio_128& Acc1_txA2_1_heads2,  
                             axis_stream_plio_128& Acc1_txA3_0_heads2, axis_stream_plio_128& Acc1_txA3_1_heads2,  
                             const bool sel, const bool enable){
    Acc1_send_LHS_type0<0> (buf0_lhs[0], buf1_lhs[0],  boundary_y,boundary_z,Acc1_txA0_0_heads0,sel,enable);
    Acc1_send_LHS_type0<1> (buf0_lhs[1], buf1_lhs[1],  boundary_y,boundary_z,Acc1_txA0_1_heads0,sel,enable);
    Acc1_send_LHS_type0<2> (buf0_lhs[6], buf1_lhs[6],  boundary_y,boundary_z,Acc1_txA1_0_heads0,sel,enable);
    Acc1_send_LHS_type0<3> (buf0_lhs[7], buf1_lhs[7],  boundary_y,boundary_z,Acc1_txA1_1_heads0,sel,enable);
    Acc1_send_LHS_type0<4> (buf0_lhs[12],buf1_lhs[12], boundary_y,boundary_z,Acc1_txA2_0_heads0,sel,enable);
    Acc1_send_LHS_type0<5> (buf0_lhs[13],buf1_lhs[13], boundary_y,boundary_z,Acc1_txA2_1_heads0,sel,enable);
    Acc1_send_LHS_type0<6> (buf0_lhs[18],buf1_lhs[18], boundary_y,boundary_z,Acc1_txA3_0_heads0,sel,enable);
    Acc1_send_LHS_type0<7> (buf0_lhs[19],buf1_lhs[19], boundary_y,boundary_z,Acc1_txA3_1_heads0,sel,enable);
    Acc1_send_LHS_type0<8> (buf0_lhs[2], buf1_lhs[2],  boundary_y,boundary_z,Acc1_txA0_0_heads1,sel,enable);
    Acc1_send_LHS_type0<9> (buf0_lhs[3], buf1_lhs[3],  boundary_y,boundary_z,Acc1_txA0_1_heads1,sel,enable);
    Acc1_send_LHS_type0<10>(buf0_lhs[8], buf1_lhs[8],  boundary_y,boundary_z,Acc1_txA1_0_heads1,sel,enable);
    Acc1_send_LHS_type0<11>(buf0_lhs[9], buf1_lhs[9],  boundary_y,boundary_z,Acc1_txA1_1_heads1,sel,enable);
    Acc1_send_LHS_type0<12>(buf0_lhs[14],buf1_lhs[14], boundary_y,boundary_z,Acc1_txA2_0_heads1,sel,enable);
    Acc1_send_LHS_type0<13>(buf0_lhs[15],buf1_lhs[15], boundary_y,boundary_z,Acc1_txA2_1_heads1,sel,enable);
    Acc1_send_LHS_type0<14>(buf0_lhs[20],buf1_lhs[20], boundary_y,boundary_z,Acc1_txA3_0_heads1,sel,enable);
    Acc1_send_LHS_type0<15>(buf0_lhs[21],buf1_lhs[21], boundary_y,boundary_z,Acc1_txA3_1_heads1,sel,enable);
    Acc1_send_LHS_type0<16>(buf0_lhs[4], buf1_lhs[4],  boundary_y,boundary_z,Acc1_txA0_0_heads2,sel,enable);
    Acc1_send_LHS_type0<17>(buf0_lhs[5], buf1_lhs[5],  boundary_y,boundary_z,Acc1_txA0_1_heads2,sel,enable);
    Acc1_send_LHS_type0<18>(buf0_lhs[10],buf1_lhs[10], boundary_y,boundary_z,Acc1_txA1_0_heads2,sel,enable);
    Acc1_send_LHS_type0<19>(buf0_lhs[11],buf1_lhs[11], boundary_y,boundary_z,Acc1_txA1_1_heads2,sel,enable);
    Acc1_send_LHS_type0<20>(buf0_lhs[16],buf1_lhs[16], boundary_y,boundary_z,Acc1_txA2_0_heads2,sel,enable);
    Acc1_send_LHS_type0<21>(buf0_lhs[17],buf1_lhs[17], boundary_y,boundary_z,Acc1_txA2_1_heads2,sel,enable);
    Acc1_send_LHS_type0<22>(buf0_lhs[22],buf1_lhs[22], boundary_y,boundary_z,Acc1_txA3_0_heads2,sel,enable);
    Acc1_send_LHS_type0<23>(buf0_lhs[23],buf1_lhs[23], boundary_y,boundary_z,Acc1_txA3_1_heads2,sel,enable);
      
}

template<int NC, int TEMP_B, int TEMP_C, int TEMP_B_New, int TEMP_C_New>
void Acc1_send_buff_type0(ap_uint<PLIO_WIDTH_128> buf_rhs[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2,axis_stream_128& Acc1_rhs0,axis_stream_128& Acc1_rhs1,const int sel, const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        if (sel == 0){
            for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
                for (int z = 0; z < boundary_z; z++) {
                    for (int y = 0; y < boundary_y; y++) {
                        for (int w1 = 0; w1 < ACC1_RW1; w1++) {
                            for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                            #pragma HLS PIPELINE II = 1
                                int pos0=(w2+z*ACC1_RW2)%buffer_w2;
                                int pos1=(w1+y*ACC1_RW1)%buffer_w1;
                                int temp_c=(w2+z*ACC1_RW2)/buffer_w2;
                                int temp_b=(w1+y*ACC1_RW1)/buffer_w1;
                                if (temp_b==TEMP_B&&temp_c==TEMP_C){
                                    data = buf_rhs[pos1][pos0];
                                    Acc1_rhs0.write(data);
                                }
                            }
                        }
                    }
                }   
            }
        }
        else{
            for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
                for (int z = 0; z < boundary_z; z++) {
                    for (int y = 0; y < boundary_y; y++) {
                        for (int w1 = 0; w1 < ACC1_RW1_TRANS/4; w1++) {
                            for (int w2 = 0; w2 < ACC1_RW2_TRANS; w2++) {
                                for (int i = 0; i < 4; i++) {
                                #pragma HLS PIPELINE II = 1
                                    int pos0 = (w2 + z * ACC1_RW2_TRANS) % buffer_w2;
                                    int pos1 = (i + w1 * 4 + y * ACC1_RW1_TRANS) % buffer_w1;
                                    int temp_c = (w2 + z * ACC1_RW2_TRANS) / buffer_w2;
                                    int temp_b = (i + w1 * 4 + y * ACC1_RW1_TRANS) / buffer_w1;
                                    if (temp_b==TEMP_B_New&&temp_c==TEMP_C_New){                                        
                                        data = buf_rhs[pos0][pos1];
                                        Acc1_rhs1.write(data);
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
void Acc1_merge0(axis_stream_128& Acc1_sin0_0,axis_stream_128& Acc1_sin1_0,const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2, axis_stream_128& Acc1_sout, const bool enable){

#pragma HLS inline off

    if(enable){

        data_plio_128 data;

        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {          
                    for (int w1 = 0; w1 < ACC1_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                            int temp_c=(w2+z*ACC1_RW2)/buffer_w2;
                            int temp_b=(w1+y*ACC1_RW1)/buffer_w1;

                            if (temp_c==0&&temp_b==0){ 
                                data = Acc1_sin0_0.read();
                            }
                            else if (temp_c==0&&temp_b==1){ 
                                data = Acc1_sin1_0.read();
                            } 
                            Acc1_sout.write(data);
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void Acc1_merge1(axis_stream_128& Acc1_sin0_0,axis_stream_128& Acc1_sin0_1, const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2, axis_stream_128& Acc1_sout, const bool enable){

#pragma HLS inline off

    if(enable){

        data_plio_128 data;

        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int w1 = 0; w1 < ACC1_RW1_TRANS/4; w1++) {
                        for (int w2 = 0; w2 < ACC1_RW2_TRANS; w2++) {
                            for (int i = 0; i < 4; i++) {
                            #pragma HLS PIPELINE II = 1
                                int temp_c = (w2 + z * ACC1_RW2_TRANS) / buffer_w2;
                                int temp_b = (i + w1 * 4 + y * ACC1_RW1_TRANS) / buffer_w1;

                                if (temp_c==0&&temp_b==0){ 
                                    data = Acc1_sin0_0.read();
                                }
                                else if (temp_c==1&&temp_b==0){ 
                                    data = Acc1_sin0_1.read();
                                } 
                                Acc1_sout.write(data);
                            }
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void Acc1_transpose(axis_stream_128& Acc1_sin, const int boundary_y, const int boundary_z,
                          axis_stream_128& Acc1_sout, const bool enable){

#pragma HLS inline off
    if(enable){
        data_plio_128 temp_data;
        data_plio_128 data;

        ap_uint<8> data0[NUM_PER_64][NUM_PER_64];
        #pragma HLS ARRAY_PARTITION variable=data0 complete dim=0

        ap_uint<8> data1[NUM_PER_64][NUM_PER_64];
        #pragma HLS ARRAY_PARTITION variable=data1 complete dim=0

        int total_bound = boundary_y * boundary_z * ACC1_RW1 * ACC1_RW2;
        
        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
            for (int i = 0; i < (total_bound+4); i++) {  
            #pragma HLS PIPELINE II = 1
                int pos0=i%4;
                int pos1=i/4;
                int pos=pos0*2;
                if(i<4){
                    data=Acc1_sin.read();
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
                    Acc1_sout.write(temp_data);
                }
                else if(pos1%2==1){
                    data=Acc1_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data1[un%8][pos+(un/8)]= data(7+un*8,un*8);
                        temp_data(7+un*8,un*8) = data0[pos+(un/8)][(un%8)];
                    }
                    Acc1_sout.write(temp_data);
                }
                else{
                    data=Acc1_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data0[un%8][pos+(un/8)]= data(7+un*8,un*8);
                        temp_data(7+un*8,un*8) = data1[pos+(un/8)][(un%8)];
                    }
                    Acc1_sout.write(temp_data);
                }
            }
        }
    }
}

template<int NC>
void Acc1_send_RHS(axis_stream_128& Acc1_sin0,axis_stream_128& Acc1_sin1,const int boundary_y, const int boundary_z, 
                          axis_stream_plio_128& Acc1_txB0, const int sel, const bool enable){
#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int w1 = 0; w1 < ACC1_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                            
                            if (sel==0){
                                data = Acc1_sin0.read();
                            }
                            else{
                                data = Acc1_sin1.read();
                            }
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc1_txB0.write(tmp);
                            
                        }
                    }
                }
            }
        }
    }
}


template<int NC>
void Acc1_send_RHS_type0_wrapper0(ap_uint<PLIO_WIDTH_128> buf_rhs0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                                  ap_uint<PLIO_WIDTH_128> buf_rhs2[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], 
                                  ap_uint<PLIO_WIDTH_128> buf_rhs1[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                                  ap_uint<PLIO_WIDTH_128> buf_rhs3[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], 
                                  ap_uint<PLIO_WIDTH_128> buf_rhs4[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                                  ap_uint<PLIO_WIDTH_128> buf_rhs6[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], 
                                  ap_uint<PLIO_WIDTH_128> buf_rhs5[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                                  ap_uint<PLIO_WIDTH_128> buf_rhs7[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], //BUFF1_DIM3*BUFF1_DIM2
                                  const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2,
                                  axis_stream_plio_128& Acc1_txB0_0, axis_stream_plio_128& Acc1_txB1_0,  
                                  axis_stream_plio_128& Acc1_txB0_1, axis_stream_plio_128& Acc1_txB1_1,  
                                  const int sel, const bool enable){


#pragma HLS dataflow


    axis_stream_128 Acc1_rhs0_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc1_rhs0_0_f0_p0_0 depth = 16
    axis_stream_128 Acc1_rhs0_0_f0_p1_0;
    #pragma HLS STREAM variable = Acc1_rhs0_0_f0_p1_0 depth = 16
    axis_stream_128 Acc1_rhs0_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc1_rhs0_1_f0_p0_0 depth = 16
    axis_stream_128 Acc1_rhs0_1_f0_p1_0;
    #pragma HLS STREAM variable = Acc1_rhs0_1_f0_p1_0 depth = 16
    axis_stream_128 Acc1_rhs1_0_f0_p0_0;
    #pragma HLS STREAM variable = Acc1_rhs1_0_f0_p0_0 depth = 16
    axis_stream_128 Acc1_rhs1_0_f0_p1_0;
    #pragma HLS STREAM variable = Acc1_rhs1_0_f0_p1_0 depth = 16
    axis_stream_128 Acc1_rhs1_1_f0_p0_0;
    #pragma HLS STREAM variable = Acc1_rhs1_1_f0_p0_0 depth = 16
    axis_stream_128 Acc1_rhs1_1_f0_p1_0;
    #pragma HLS STREAM variable = Acc1_rhs1_1_f0_p1_0 depth = 16
    axis_stream_128 Acc1_rhs0_0_f1_p0_0;
    axis_stream_128 Acc1_rhs0_0_f1_p0_1;
    axis_stream_128 Acc1_rhs1_0_f1_p0_0;
    axis_stream_128 Acc1_rhs1_0_f1_p0_1;
    axis_stream_128 Acc1_rhs0_1_f1_p0_0;
    axis_stream_128 Acc1_rhs0_1_f1_p0_1;
    axis_stream_128 Acc1_rhs1_1_f1_p0_0;
    axis_stream_128 Acc1_rhs1_1_f1_p0_1;
    

    axis_stream_128 Acc1_plio0_0_f0; axis_stream_128 Acc1_plio0_0_f1;  
    axis_stream_128 Acc1_plio0_1_f0; axis_stream_128 Acc1_plio0_1_f1;  
    axis_stream_128 Acc1_plio1_0_f0; axis_stream_128 Acc1_plio1_0_f1;  
    axis_stream_128 Acc1_plio1_1_f0; axis_stream_128 Acc1_plio1_1_f1;  
    

    axis_stream_128 Acc1_s0_0_f1; axis_stream_128 Acc1_s0_1_f1; 
    axis_stream_128 Acc1_s1_0_f1; axis_stream_128 Acc1_s1_1_f1; 
    

    Acc1_send_buff_type0<0,0,0,0,0>(buf_rhs0, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs0_0_f0_p0_0, Acc1_rhs0_0_f1_p0_0, sel, enable);
    Acc1_send_buff_type0<2,1,0,0,1>(buf_rhs2, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs0_0_f0_p1_0, Acc1_rhs0_0_f1_p0_1, sel, enable);
    Acc1_send_buff_type0<1,0,0,0,0>(buf_rhs1, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs0_1_f0_p0_0, Acc1_rhs1_0_f1_p0_0, sel, enable);
    Acc1_send_buff_type0<3,1,0,0,1>(buf_rhs3, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs0_1_f0_p1_0, Acc1_rhs1_0_f1_p0_1, sel, enable);
    Acc1_send_buff_type0<4,0,0,0,0>(buf_rhs4, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs1_0_f0_p0_0, Acc1_rhs0_1_f1_p0_0, sel, enable);
    Acc1_send_buff_type0<6,1,0,0,1>(buf_rhs6, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs1_0_f0_p1_0, Acc1_rhs0_1_f1_p0_1, sel, enable);
    Acc1_send_buff_type0<5,0,0,0,0>(buf_rhs5, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs1_1_f0_p0_0, Acc1_rhs1_1_f1_p0_0, sel, enable);
    Acc1_send_buff_type0<7,1,0,0,1>(buf_rhs7, boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_rhs1_1_f0_p1_0, Acc1_rhs1_1_f1_p0_1, sel, enable);
    
    
    Acc1_merge0<0>(Acc1_rhs0_0_f0_p0_0,Acc1_rhs0_0_f0_p1_0,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio0_0_f0, (sel==0&&enable));
    Acc1_merge1<0>(Acc1_rhs0_0_f1_p0_0,Acc1_rhs0_0_f1_p0_1,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio0_0_f1, (sel==1&&enable)); 
    Acc1_transpose<0>(Acc1_plio0_0_f1, boundary_y, boundary_z, Acc1_s0_0_f1, (sel==1&&enable));  
    Acc1_send_RHS<0>(Acc1_plio0_0_f0, Acc1_s0_0_f1, boundary_y, boundary_z, Acc1_txB0_0, sel, enable);
    
    Acc1_merge0<1>(Acc1_rhs0_1_f0_p0_0,Acc1_rhs0_1_f0_p1_0,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio0_1_f0, (sel==0&&enable));
    Acc1_merge1<1>(Acc1_rhs0_1_f1_p0_0,Acc1_rhs0_1_f1_p0_1,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio0_1_f1, (sel==1&&enable)); 
    Acc1_transpose<1>(Acc1_plio0_1_f1, boundary_y, boundary_z, Acc1_s0_1_f1, (sel==1&&enable));  
    Acc1_send_RHS<1>(Acc1_plio0_1_f0, Acc1_s0_1_f1, boundary_y, boundary_z, Acc1_txB0_1, sel, enable);
    
    
    Acc1_merge0<2>(Acc1_rhs1_0_f0_p0_0,Acc1_rhs1_0_f0_p1_0,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio1_0_f0, (sel==0&&enable));
    Acc1_merge1<2>(Acc1_rhs1_0_f1_p0_0,Acc1_rhs1_0_f1_p0_1,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio1_0_f1, (sel==1&&enable)); 
    Acc1_transpose<2>(Acc1_plio1_0_f1, boundary_y, boundary_z, Acc1_s1_0_f1, (sel==1&&enable));  
    Acc1_send_RHS<2>(Acc1_plio1_0_f0, Acc1_s1_0_f1, boundary_y, boundary_z, Acc1_txB1_0, sel, enable);
    
    Acc1_merge0<3>(Acc1_rhs1_1_f0_p0_0,Acc1_rhs1_1_f0_p1_0,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio1_1_f0, (sel==0&&enable));
    Acc1_merge1<3>(Acc1_rhs1_1_f1_p0_0,Acc1_rhs1_1_f1_p0_1,boundary_y, boundary_z, buffer_w1, buffer_w2, Acc1_plio1_1_f1, (sel==1&&enable)); 
    Acc1_transpose<3>(Acc1_plio1_1_f1, boundary_y, boundary_z, Acc1_s1_1_f1, (sel==1&&enable));  
    Acc1_send_RHS<3>(Acc1_plio1_1_f0, Acc1_s1_1_f1, boundary_y, boundary_z, Acc1_txB1_1, sel, enable);
    
}

template<int NC>
void Acc1_send_RHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_rhs[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], 
                             const int boundary_y, const int boundary_z, const int buffer_w1, const int buffer_w2,
                             axis_stream_plio_128& Acc1_txB0_0_heads0, axis_stream_plio_128& Acc1_txB0_1_heads0,  
                             axis_stream_plio_128& Acc1_txB1_0_heads0, axis_stream_plio_128& Acc1_txB1_1_heads0,  
                             axis_stream_plio_128& Acc1_txB0_0_heads1, axis_stream_plio_128& Acc1_txB0_1_heads1,  
                             axis_stream_plio_128& Acc1_txB1_0_heads1, axis_stream_plio_128& Acc1_txB1_1_heads1,  
                             axis_stream_plio_128& Acc1_txB0_0_heads2, axis_stream_plio_128& Acc1_txB0_1_heads2,  
                             axis_stream_plio_128& Acc1_txB1_0_heads2, axis_stream_plio_128& Acc1_txB1_1_heads2,  
                             const int sel, const bool enable){
    
    Acc1_send_RHS_type0_wrapper0<0>(buf_rhs[0],buf_rhs[1],buf_rhs[6],buf_rhs[7],buf_rhs[12],buf_rhs[13],buf_rhs[18],buf_rhs[19],boundary_y,boundary_z,buffer_w1,buffer_w2,Acc1_txB0_0_heads0, Acc1_txB1_0_heads0, Acc1_txB0_1_heads0,Acc1_txB1_1_heads0,sel,enable);
    Acc1_send_RHS_type0_wrapper0<1>(buf_rhs[2],buf_rhs[3],buf_rhs[8],buf_rhs[9],buf_rhs[14],buf_rhs[15],buf_rhs[20],buf_rhs[21],boundary_y,boundary_z,buffer_w1,buffer_w2,Acc1_txB0_0_heads1, Acc1_txB1_0_heads1, Acc1_txB0_1_heads1,Acc1_txB1_1_heads1,sel,enable);
    Acc1_send_RHS_type0_wrapper0<2>(buf_rhs[4],buf_rhs[5],buf_rhs[10],buf_rhs[11],buf_rhs[16],buf_rhs[17],buf_rhs[22],buf_rhs[23],boundary_y,boundary_z,buffer_w1,buffer_w2,Acc1_txB0_0_heads2, Acc1_txB1_0_heads2, Acc1_txB0_1_heads2,Acc1_txB1_1_heads2,sel,enable);
}

template<int NC>
void Acc1_receive_OUT(axis_stream_plio_128& Acc1_rxC, const int boundary_y, const int boundary_z,axis_stream_128& Acc1_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        ap_int<8>  data_temp1[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp1 complete dim=0

        ap_int<8>  data_temp2[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp2 complete dim=0

        data_plio_128   temp_array[ACC1_LH1][ACC1_RW2];
        #pragma HLS bind_storage variable=temp_array type=RAM_1P impl=LUTRAM

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {          
                for (int y = 0; y < boundary_y; y++) {
                    for (int h1 = 0; h1 < ACC1_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=temp_array type=inter false
                            tmp   =  Acc1_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            if(boundary_y<=1){
                                for (int un=0; un<NUM_PER_128; un++){
                                #pragma HLS UNROLL factor=NUM_PER_128
                                    data_temp(un*8+7,un*8) = data_temp0[un];
                                }
                                
                                Acc1_sout0.write(data_temp);
                                
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
                                    
                                    Acc1_sout0.write(data_temp);
                                    
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
void Acc1_receive_buff_type0(axis_stream_128& Acc1_sin0,const int boundary_z,
                             ap_uint<PLIO_WIDTH_128> buf0_out0_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                             ap_uint<PLIO_WIDTH_128> buf1_out0_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0*ACC1_HEADS],
                             const bool sel, const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;
        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {          
                for (int h1 = 0; h1 < ACC1_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        int pos0=w2+z*ACC1_RW2;
                        int pos1=h1+x*ACC1_LH1;
                        data_temp = Acc1_sin0.read(); 
                        if(sel==0){
                           buf0_out0_0[pos1][pos0] = data_temp;
                        }
                        else{
                           buf1_out0_0[pos1][pos0] = data_temp; 
                        }
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc1_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf0_out[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0], 
                                ap_uint<PLIO_WIDTH_128> buf1_out[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0*ACC1_HEADS],
           const int boundary_y, const int boundary_z,
           axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0,  
           axis_stream_plio_128& Acc1_rxC1_0_heads0, axis_stream_plio_128& Acc1_rxC1_1_heads0,  
           axis_stream_plio_128& Acc1_rxC2_0_heads0, axis_stream_plio_128& Acc1_rxC2_1_heads0,  
           axis_stream_plio_128& Acc1_rxC3_0_heads0, axis_stream_plio_128& Acc1_rxC3_1_heads0,  
           axis_stream_plio_128& Acc1_rxC0_0_heads1, axis_stream_plio_128& Acc1_rxC0_1_heads1,  
           axis_stream_plio_128& Acc1_rxC1_0_heads1, axis_stream_plio_128& Acc1_rxC1_1_heads1,  
           axis_stream_plio_128& Acc1_rxC2_0_heads1, axis_stream_plio_128& Acc1_rxC2_1_heads1,  
           axis_stream_plio_128& Acc1_rxC3_0_heads1, axis_stream_plio_128& Acc1_rxC3_1_heads1,  
           axis_stream_plio_128& Acc1_rxC0_0_heads2, axis_stream_plio_128& Acc1_rxC0_1_heads2,  
           axis_stream_plio_128& Acc1_rxC1_0_heads2, axis_stream_plio_128& Acc1_rxC1_1_heads2,  
           axis_stream_plio_128& Acc1_rxC2_0_heads2, axis_stream_plio_128& Acc1_rxC2_1_heads2,  
           axis_stream_plio_128& Acc1_rxC3_0_heads2, axis_stream_plio_128& Acc1_rxC3_1_heads2,  
           const bool sel, const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc1_plio0_0_heads0_f0;axis_stream_128 Acc1_plio0_1_heads0_f0; 
    axis_stream_128 Acc1_plio1_0_heads0_f0;axis_stream_128 Acc1_plio1_1_heads0_f0; 
    axis_stream_128 Acc1_plio2_0_heads0_f0;axis_stream_128 Acc1_plio2_1_heads0_f0; 
    axis_stream_128 Acc1_plio3_0_heads0_f0;axis_stream_128 Acc1_plio3_1_heads0_f0; 
    
    axis_stream_128 Acc1_plio0_0_heads1_f0;axis_stream_128 Acc1_plio0_1_heads1_f0; 
    axis_stream_128 Acc1_plio1_0_heads1_f0;axis_stream_128 Acc1_plio1_1_heads1_f0; 
    axis_stream_128 Acc1_plio2_0_heads1_f0;axis_stream_128 Acc1_plio2_1_heads1_f0; 
    axis_stream_128 Acc1_plio3_0_heads1_f0;axis_stream_128 Acc1_plio3_1_heads1_f0; 
    
    axis_stream_128 Acc1_plio0_0_heads2_f0;axis_stream_128 Acc1_plio0_1_heads2_f0; 
    axis_stream_128 Acc1_plio1_0_heads2_f0;axis_stream_128 Acc1_plio1_1_heads2_f0; 
    axis_stream_128 Acc1_plio2_0_heads2_f0;axis_stream_128 Acc1_plio2_1_heads2_f0; 
    axis_stream_128 Acc1_plio3_0_heads2_f0;axis_stream_128 Acc1_plio3_1_heads2_f0; 
    
    Acc1_receive_OUT<0>(Acc1_rxC0_0_heads0,  boundary_y, boundary_z, Acc1_plio0_0_heads0_f0, enable); 
    Acc1_receive_OUT<1>(Acc1_rxC0_1_heads0,  boundary_y, boundary_z, Acc1_plio0_1_heads0_f0, enable); 
    Acc1_receive_OUT<2>(Acc1_rxC1_0_heads0,  boundary_y, boundary_z, Acc1_plio1_0_heads0_f0, enable); 
    Acc1_receive_OUT<3>(Acc1_rxC1_1_heads0,  boundary_y, boundary_z, Acc1_plio1_1_heads0_f0, enable); 
    Acc1_receive_OUT<4>(Acc1_rxC2_0_heads0,  boundary_y, boundary_z, Acc1_plio2_0_heads0_f0, enable); 
    Acc1_receive_OUT<5>(Acc1_rxC2_1_heads0,  boundary_y, boundary_z, Acc1_plio2_1_heads0_f0, enable); 
    Acc1_receive_OUT<6>(Acc1_rxC3_0_heads0,  boundary_y, boundary_z, Acc1_plio3_0_heads0_f0, enable); 
    Acc1_receive_OUT<7>(Acc1_rxC3_1_heads0,  boundary_y, boundary_z, Acc1_plio3_1_heads0_f0, enable); 
    Acc1_receive_OUT<8>(Acc1_rxC0_0_heads1,  boundary_y, boundary_z, Acc1_plio0_0_heads1_f0, enable); 
    Acc1_receive_OUT<9>(Acc1_rxC0_1_heads1,  boundary_y, boundary_z, Acc1_plio0_1_heads1_f0, enable); 
    Acc1_receive_OUT<10>(Acc1_rxC1_0_heads1, boundary_y, boundary_z, Acc1_plio1_0_heads1_f0, enable); 
    Acc1_receive_OUT<11>(Acc1_rxC1_1_heads1, boundary_y, boundary_z, Acc1_plio1_1_heads1_f0, enable); 
    Acc1_receive_OUT<12>(Acc1_rxC2_0_heads1, boundary_y, boundary_z, Acc1_plio2_0_heads1_f0, enable); 
    Acc1_receive_OUT<13>(Acc1_rxC2_1_heads1, boundary_y, boundary_z, Acc1_plio2_1_heads1_f0, enable); 
    Acc1_receive_OUT<14>(Acc1_rxC3_0_heads1, boundary_y, boundary_z, Acc1_plio3_0_heads1_f0, enable); 
    Acc1_receive_OUT<15>(Acc1_rxC3_1_heads1, boundary_y, boundary_z, Acc1_plio3_1_heads1_f0, enable); 
    Acc1_receive_OUT<16>(Acc1_rxC0_0_heads2, boundary_y, boundary_z, Acc1_plio0_0_heads2_f0, enable); 
    Acc1_receive_OUT<17>(Acc1_rxC0_1_heads2, boundary_y, boundary_z, Acc1_plio0_1_heads2_f0, enable); 
    Acc1_receive_OUT<18>(Acc1_rxC1_0_heads2, boundary_y, boundary_z, Acc1_plio1_0_heads2_f0, enable); 
    Acc1_receive_OUT<19>(Acc1_rxC1_1_heads2, boundary_y, boundary_z, Acc1_plio1_1_heads2_f0, enable); 
    Acc1_receive_OUT<20>(Acc1_rxC2_0_heads2, boundary_y, boundary_z, Acc1_plio2_0_heads2_f0, enable); 
    Acc1_receive_OUT<21>(Acc1_rxC2_1_heads2, boundary_y, boundary_z, Acc1_plio2_1_heads2_f0, enable); 
    Acc1_receive_OUT<22>(Acc1_rxC3_0_heads2, boundary_y, boundary_z, Acc1_plio3_0_heads2_f0, enable); 
    Acc1_receive_OUT<23>(Acc1_rxC3_1_heads2, boundary_y, boundary_z, Acc1_plio3_1_heads2_f0, enable); 
    
    
    Acc1_receive_buff_type0<0>( Acc1_plio0_0_heads0_f0, boundary_z, buf0_out[0], buf1_out[0],  sel, enable);
    Acc1_receive_buff_type0<1>( Acc1_plio0_1_heads0_f0, boundary_z, buf0_out[1], buf1_out[1],  sel, enable);
    Acc1_receive_buff_type0<2>( Acc1_plio1_0_heads0_f0, boundary_z, buf0_out[6], buf1_out[6],  sel, enable);
    Acc1_receive_buff_type0<3>( Acc1_plio1_1_heads0_f0, boundary_z, buf0_out[7], buf1_out[7],  sel, enable);
    Acc1_receive_buff_type0<4>( Acc1_plio2_0_heads0_f0, boundary_z, buf0_out[12],buf1_out[12], sel, enable);
    Acc1_receive_buff_type0<5>( Acc1_plio2_1_heads0_f0, boundary_z, buf0_out[13],buf1_out[13], sel, enable);
    Acc1_receive_buff_type0<6>( Acc1_plio3_0_heads0_f0, boundary_z, buf0_out[18],buf1_out[18], sel, enable);
    Acc1_receive_buff_type0<7>( Acc1_plio3_1_heads0_f0, boundary_z, buf0_out[19],buf1_out[19], sel, enable);
    Acc1_receive_buff_type0<8>( Acc1_plio0_0_heads1_f0, boundary_z, buf0_out[2], buf1_out[2],  sel, enable);
    Acc1_receive_buff_type0<9>( Acc1_plio0_1_heads1_f0, boundary_z, buf0_out[3], buf1_out[3],  sel, enable);
    Acc1_receive_buff_type0<10>(Acc1_plio1_0_heads1_f0, boundary_z, buf0_out[8], buf1_out[8],  sel, enable);
    Acc1_receive_buff_type0<11>(Acc1_plio1_1_heads1_f0, boundary_z, buf0_out[9], buf1_out[9],  sel, enable);
    Acc1_receive_buff_type0<12>(Acc1_plio2_0_heads1_f0, boundary_z, buf0_out[14],buf1_out[14], sel, enable);
    Acc1_receive_buff_type0<13>(Acc1_plio2_1_heads1_f0, boundary_z, buf0_out[15],buf1_out[15], sel, enable);
    Acc1_receive_buff_type0<14>(Acc1_plio3_0_heads1_f0, boundary_z, buf0_out[20],buf1_out[20], sel, enable);
    Acc1_receive_buff_type0<15>(Acc1_plio3_1_heads1_f0, boundary_z, buf0_out[21],buf1_out[21], sel, enable);
    Acc1_receive_buff_type0<16>(Acc1_plio0_0_heads2_f0, boundary_z, buf0_out[4], buf1_out[4],  sel, enable);
    Acc1_receive_buff_type0<17>(Acc1_plio0_1_heads2_f0, boundary_z, buf0_out[5], buf1_out[5],  sel, enable);
    Acc1_receive_buff_type0<18>(Acc1_plio1_0_heads2_f0, boundary_z, buf0_out[10],buf1_out[10], sel, enable);
    Acc1_receive_buff_type0<19>(Acc1_plio1_1_heads2_f0, boundary_z, buf0_out[11],buf1_out[11], sel, enable);
    Acc1_receive_buff_type0<20>(Acc1_plio2_0_heads2_f0, boundary_z, buf0_out[16],buf1_out[16], sel, enable);
    Acc1_receive_buff_type0<21>(Acc1_plio2_1_heads2_f0, boundary_z, buf0_out[17],buf1_out[17], sel, enable);
    Acc1_receive_buff_type0<22>(Acc1_plio3_0_heads2_f0, boundary_z, buf0_out[22],buf1_out[22], sel, enable);
    Acc1_receive_buff_type0<23>(Acc1_plio3_1_heads2_f0, boundary_z, buf0_out[23],buf1_out[23], sel, enable);
}

template<int NC>
void Acc1_send_dummy_top(ap_uint<PLIO_WIDTH_128> buf_in[BUFF1_RHS_PARTB*BUFF1_RHS_PARTC*ACC1_HEADS][ACC1_K0/BUFF1_RHS_PARTB/NUM_PER_64][ACC1_N0/BUFF1_RHS_PARTC/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf_out[BUFF1_RHS_PARTB*BUFF1_RHS_PARTC*ACC1_HEADS][ACC1_K0/BUFF1_RHS_PARTB/NUM_PER_64][ACC1_N0/BUFF1_RHS_PARTC/(NUM_PER_128/NUM_PER_64)],const bool enable){
#pragma HLS inline off
    if(enable){
        for (int j = 0; j < ACC1_K0/BUFF1_RHS_PARTB/NUM_PER_64; j++) {
            for (int i = 0; i < ACC1_N0/BUFF1_RHS_PARTC/(NUM_PER_128/NUM_PER_64); i++) {
                #pragma HLS PIPELINE II = 1
                for (int k = 0; k < BUFF1_RHS_PARTB*BUFF1_RHS_PARTC*ACC1_HEADS; k++) {
                    buf_out[k][j][i]=buf_in[k][j][i];
                }
            }
        }
    }
}

template<int NC>
void Acc1_top(ap_uint<PLIO_WIDTH_128> buf_lhs0[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
              ap_uint<PLIO_WIDTH_128> buf_rhs0[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
              ap_uint<PLIO_WIDTH_128> buf_rhs1[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
              ap_uint<PLIO_WIDTH_128> buf_out[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
              axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0,  
              axis_stream_plio_128& Acc1_txA1_0_heads0, axis_stream_plio_128& Acc1_txA1_1_heads0,  
              axis_stream_plio_128& Acc1_txA2_0_heads0, axis_stream_plio_128& Acc1_txA2_1_heads0,  
              axis_stream_plio_128& Acc1_txA3_0_heads0, axis_stream_plio_128& Acc1_txA3_1_heads0,  
              axis_stream_plio_128& Acc1_txA0_0_heads1, axis_stream_plio_128& Acc1_txA0_1_heads1,  
              axis_stream_plio_128& Acc1_txA1_0_heads1, axis_stream_plio_128& Acc1_txA1_1_heads1,  
              axis_stream_plio_128& Acc1_txA2_0_heads1, axis_stream_plio_128& Acc1_txA2_1_heads1,  
              axis_stream_plio_128& Acc1_txA3_0_heads1, axis_stream_plio_128& Acc1_txA3_1_heads1,  
              axis_stream_plio_128& Acc1_txA0_0_heads2, axis_stream_plio_128& Acc1_txA0_1_heads2,  
              axis_stream_plio_128& Acc1_txA1_0_heads2, axis_stream_plio_128& Acc1_txA1_1_heads2,  
              axis_stream_plio_128& Acc1_txA2_0_heads2, axis_stream_plio_128& Acc1_txA2_1_heads2,  
              axis_stream_plio_128& Acc1_txA3_0_heads2, axis_stream_plio_128& Acc1_txA3_1_heads2,
              axis_stream_plio_128& Acc1_txB0_0_heads0, axis_stream_plio_128& Acc1_txB0_1_heads0,  
              axis_stream_plio_128& Acc1_txB1_0_heads0, axis_stream_plio_128& Acc1_txB1_1_heads0,  
              axis_stream_plio_128& Acc1_txB0_0_heads1, axis_stream_plio_128& Acc1_txB0_1_heads1,  
              axis_stream_plio_128& Acc1_txB1_0_heads1, axis_stream_plio_128& Acc1_txB1_1_heads1,  
              axis_stream_plio_128& Acc1_txB0_0_heads2, axis_stream_plio_128& Acc1_txB0_1_heads2,  
              axis_stream_plio_128& Acc1_txB1_0_heads2, axis_stream_plio_128& Acc1_txB1_1_heads2,
              axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0,  
              axis_stream_plio_128& Acc1_rxC1_0_heads0, axis_stream_plio_128& Acc1_rxC1_1_heads0,  
              axis_stream_plio_128& Acc1_rxC2_0_heads0, axis_stream_plio_128& Acc1_rxC2_1_heads0,  
              axis_stream_plio_128& Acc1_rxC3_0_heads0, axis_stream_plio_128& Acc1_rxC3_1_heads0,  
              axis_stream_plio_128& Acc1_rxC0_0_heads1, axis_stream_plio_128& Acc1_rxC0_1_heads1,  
              axis_stream_plio_128& Acc1_rxC1_0_heads1, axis_stream_plio_128& Acc1_rxC1_1_heads1,  
              axis_stream_plio_128& Acc1_rxC2_0_heads1, axis_stream_plio_128& Acc1_rxC2_1_heads1,  
              axis_stream_plio_128& Acc1_rxC3_0_heads1, axis_stream_plio_128& Acc1_rxC3_1_heads1,  
              axis_stream_plio_128& Acc1_rxC0_0_heads2, axis_stream_plio_128& Acc1_rxC0_1_heads2,  
              axis_stream_plio_128& Acc1_rxC1_0_heads2, axis_stream_plio_128& Acc1_rxC1_1_heads2,  
              axis_stream_plio_128& Acc1_rxC2_0_heads2, axis_stream_plio_128& Acc1_rxC2_1_heads2,  
              axis_stream_plio_128& Acc1_rxC3_0_heads2, axis_stream_plio_128& Acc1_rxC3_1_heads2, const bool enable){

#pragma HLS inline off    
    ap_uint<PLIO_WIDTH_128> buf_out0[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buf_out0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buf_out0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buf_out1[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0*ACC1_HEADS];
    #pragma HLS bind_storage variable=buf_out1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buf_out1 complete dim=1

    ap_uint<PLIO_WIDTH_128> buf_out2[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0*ACC1_HEADS];
    #pragma HLS bind_storage variable=buf_out2 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buf_out2 complete dim=1

    for(int i=0; i<2; i++){
        if(i==0){          
            Acc1_send_LHS_type0_top<0>(buf_lhs0, buf_out2, 1, 3,//boundary_y,boundary_z,
                                    Acc1_txA0_0_heads0, Acc1_txA0_1_heads0,  
                                    Acc1_txA1_0_heads0, Acc1_txA1_1_heads0,  
                                    Acc1_txA2_0_heads0, Acc1_txA2_1_heads0,  
                                    Acc1_txA3_0_heads0, Acc1_txA3_1_heads0,  
                                    Acc1_txA0_0_heads1, Acc1_txA0_1_heads1,  
                                    Acc1_txA1_0_heads1, Acc1_txA1_1_heads1,  
                                    Acc1_txA2_0_heads1, Acc1_txA2_1_heads1,  
                                    Acc1_txA3_0_heads1, Acc1_txA3_1_heads1,  
                                    Acc1_txA0_0_heads2, Acc1_txA0_1_heads2,  
                                    Acc1_txA1_0_heads2, Acc1_txA1_1_heads2,  
                                    Acc1_txA2_0_heads2, Acc1_txA2_1_heads2,  
                                    Acc1_txA3_0_heads2, Acc1_txA3_1_heads2, 0, enable);
            Acc1_send_RHS_type0_top<0>(buf_rhs0, 1, 3, 16, 6,//boundary_y,boundary_z,buffer_w1, buffer_w2,
                                    Acc1_txB0_0_heads0, Acc1_txB0_1_heads0,  
                                    Acc1_txB1_0_heads0, Acc1_txB1_1_heads0,  
                                    Acc1_txB0_0_heads1, Acc1_txB0_1_heads1,  
                                    Acc1_txB1_0_heads1, Acc1_txB1_1_heads1,  
                                    Acc1_txB0_0_heads2, Acc1_txB0_1_heads2,  
                                    Acc1_txB1_0_heads2, Acc1_txB1_1_heads2,  
                                    1, enable);//sel
            Acc1_receive_OUT_type0_top<0>(buf_out0, buf_out1,1, 3,
                                    Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0,  
                                    Acc1_rxC1_0_heads0, Acc1_rxC1_1_heads0,  
                                    Acc1_rxC2_0_heads0, Acc1_rxC2_1_heads0,  
                                    Acc1_rxC3_0_heads0, Acc1_rxC3_1_heads0,  
                                    Acc1_rxC0_0_heads1, Acc1_rxC0_1_heads1,  
                                    Acc1_rxC1_0_heads1, Acc1_rxC1_1_heads1,  
                                    Acc1_rxC2_0_heads1, Acc1_rxC2_1_heads1,  
                                    Acc1_rxC3_0_heads1, Acc1_rxC3_1_heads1,  
                                    Acc1_rxC0_0_heads2, Acc1_rxC0_1_heads2,  
                                    Acc1_rxC1_0_heads2, Acc1_rxC1_1_heads2,  
                                    Acc1_rxC2_0_heads2, Acc1_rxC2_1_heads2,  
                                    Acc1_rxC3_0_heads2, Acc1_rxC3_1_heads2, 1, enable);
        }
        else{
            Acc1_send_LHS_type0_top<0>(buf_out0, buf_out1,3, 1,//boundary_y,boundary_z,
                                    Acc1_txA0_0_heads0, Acc1_txA0_1_heads0,  
                                    Acc1_txA1_0_heads0, Acc1_txA1_1_heads0,  
                                    Acc1_txA2_0_heads0, Acc1_txA2_1_heads0,  
                                    Acc1_txA3_0_heads0, Acc1_txA3_1_heads0,  
                                    Acc1_txA0_0_heads1, Acc1_txA0_1_heads1,  
                                    Acc1_txA1_0_heads1, Acc1_txA1_1_heads1,  
                                    Acc1_txA2_0_heads1, Acc1_txA2_1_heads1,  
                                    Acc1_txA3_0_heads1, Acc1_txA3_1_heads1,  
                                    Acc1_txA0_0_heads2, Acc1_txA0_1_heads2,  
                                    Acc1_txA1_0_heads2, Acc1_txA1_1_heads2,  
                                    Acc1_txA2_0_heads2, Acc1_txA2_1_heads2,  
                                    Acc1_txA3_0_heads2, Acc1_txA3_1_heads2, 1, enable);
            Acc1_send_RHS_type0_top<0>(buf_rhs1, 3, 1, 6, 16,//boundary_y,boundary_z,buffer_w1, buffer_w2,
                                    Acc1_txB0_0_heads0, Acc1_txB0_1_heads0,  
                                    Acc1_txB1_0_heads0, Acc1_txB1_1_heads0,  
                                    Acc1_txB0_0_heads1, Acc1_txB0_1_heads1,  
                                    Acc1_txB1_0_heads1, Acc1_txB1_1_heads1,  
                                    Acc1_txB0_0_heads2, Acc1_txB0_1_heads2,  
                                    Acc1_txB1_0_heads2, Acc1_txB1_1_heads2,  
                                    0, enable);//sel
            Acc1_receive_OUT_type0_top<0>(buf_out, buf_out2, 3, 1, 
                                    Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0,  
                                    Acc1_rxC1_0_heads0, Acc1_rxC1_1_heads0,  
                                    Acc1_rxC2_0_heads0, Acc1_rxC2_1_heads0,  
                                    Acc1_rxC3_0_heads0, Acc1_rxC3_1_heads0,  
                                    Acc1_rxC0_0_heads1, Acc1_rxC0_1_heads1,  
                                    Acc1_rxC1_0_heads1, Acc1_rxC1_1_heads1,  
                                    Acc1_rxC2_0_heads1, Acc1_rxC2_1_heads1,  
                                    Acc1_rxC3_0_heads1, Acc1_rxC3_1_heads1,  
                                    Acc1_rxC0_0_heads2, Acc1_rxC0_1_heads2,  
                                    Acc1_rxC1_0_heads2, Acc1_rxC1_1_heads2,  
                                    Acc1_rxC2_0_heads2, Acc1_rxC2_1_heads2,  
                                    Acc1_rxC3_0_heads2, Acc1_rxC3_1_heads2, 0, enable);
        }
    }
}

template<int NC>
void Acc2_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf0_lhs0_0[BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                         ap_uint<PLIO_WIDTH_128> buf1_lhs0_0[BUFF2_TYPE0_DIM1][BUFF2_TYPE0_DIM0],
                         const int boundary_y, const int boundary_z,
                         axis_stream_plio_128& Acc2_txA0, const bool sel, const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC2_M0/ACC2_A/ACC2_H1); x++) { 
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int h1 = 0; h1 < ACC2_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC2_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC2_LW1);
                            int pos1=(h1+x*ACC2_LH1);
                            if(sel==0){
                                data = buf0_lhs0_0[pos1][pos0];
                            }
                            else{
                                data = buf1_lhs0_0[pos1][pos0];
                            }
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc2_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc2_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf0_lhs[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                             ap_uint<PLIO_WIDTH_128> buf1_lhs[BUFF2_TYPE0_DIM3*BUFF2_TYPE0_DIM2][BUFF2_TYPE0_DIM1][BUFF2_TYPE0_DIM0],  
                             const int boundary_y, const int boundary_z,
                             axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0, axis_stream_plio_128& Acc2_txA0_4_heads0, axis_stream_plio_128& Acc2_txA0_5_heads0,  
                             axis_stream_plio_128& Acc2_txA1_0_heads0, axis_stream_plio_128& Acc2_txA1_1_heads0, axis_stream_plio_128& Acc2_txA1_2_heads0, axis_stream_plio_128& Acc2_txA1_3_heads0, axis_stream_plio_128& Acc2_txA1_4_heads0, axis_stream_plio_128& Acc2_txA1_5_heads0,  
                             axis_stream_plio_128& Acc2_txA2_0_heads0, axis_stream_plio_128& Acc2_txA2_1_heads0, axis_stream_plio_128& Acc2_txA2_2_heads0, axis_stream_plio_128& Acc2_txA2_3_heads0, axis_stream_plio_128& Acc2_txA2_4_heads0, axis_stream_plio_128& Acc2_txA2_5_heads0,  
                             axis_stream_plio_128& Acc2_txA3_0_heads0, axis_stream_plio_128& Acc2_txA3_1_heads0, axis_stream_plio_128& Acc2_txA3_2_heads0, axis_stream_plio_128& Acc2_txA3_3_heads0, axis_stream_plio_128& Acc2_txA3_4_heads0, axis_stream_plio_128& Acc2_txA3_5_heads0,  
                             const bool sel, const bool enable){
    Acc2_send_LHS_type0<0> (buf0_lhs[0], buf1_lhs[0],  boundary_y,boundary_z,Acc2_txA0_0_heads0,sel,enable);
    Acc2_send_LHS_type0<1> (buf0_lhs[1], buf1_lhs[1],  boundary_y,boundary_z,Acc2_txA0_1_heads0,sel,enable);
    Acc2_send_LHS_type0<2> (buf0_lhs[2], buf1_lhs[2],  boundary_y,boundary_z,Acc2_txA0_2_heads0,sel,enable);
    Acc2_send_LHS_type0<3> (buf0_lhs[3], buf1_lhs[3],  boundary_y,boundary_z,Acc2_txA0_3_heads0,sel,enable);
    Acc2_send_LHS_type0<4> (buf0_lhs[4], buf1_lhs[4],  boundary_y,boundary_z,Acc2_txA0_4_heads0,sel,enable);
    Acc2_send_LHS_type0<5> (buf0_lhs[5], buf1_lhs[5],  boundary_y,boundary_z,Acc2_txA0_5_heads0,sel,enable);
    Acc2_send_LHS_type0<6> (buf0_lhs[6], buf1_lhs[6],  boundary_y,boundary_z,Acc2_txA1_0_heads0,sel,enable);
    Acc2_send_LHS_type0<7> (buf0_lhs[7], buf1_lhs[7],  boundary_y,boundary_z,Acc2_txA1_1_heads0,sel,enable);
    Acc2_send_LHS_type0<8> (buf0_lhs[8], buf1_lhs[8],  boundary_y,boundary_z,Acc2_txA1_2_heads0,sel,enable);
    Acc2_send_LHS_type0<9> (buf0_lhs[9], buf1_lhs[9],  boundary_y,boundary_z,Acc2_txA1_3_heads0,sel,enable);
    Acc2_send_LHS_type0<10>(buf0_lhs[10],buf1_lhs[10], boundary_y,boundary_z,Acc2_txA1_4_heads0,sel,enable);
    Acc2_send_LHS_type0<11>(buf0_lhs[11],buf1_lhs[11], boundary_y,boundary_z,Acc2_txA1_5_heads0,sel,enable);
    Acc2_send_LHS_type0<12>(buf0_lhs[12],buf1_lhs[12], boundary_y,boundary_z,Acc2_txA2_0_heads0,sel,enable);
    Acc2_send_LHS_type0<13>(buf0_lhs[13],buf1_lhs[13], boundary_y,boundary_z,Acc2_txA2_1_heads0,sel,enable);
    Acc2_send_LHS_type0<14>(buf0_lhs[14],buf1_lhs[14], boundary_y,boundary_z,Acc2_txA2_2_heads0,sel,enable);
    Acc2_send_LHS_type0<15>(buf0_lhs[15],buf1_lhs[15], boundary_y,boundary_z,Acc2_txA2_3_heads0,sel,enable);
    Acc2_send_LHS_type0<16>(buf0_lhs[16],buf1_lhs[16], boundary_y,boundary_z,Acc2_txA2_4_heads0,sel,enable);
    Acc2_send_LHS_type0<17>(buf0_lhs[17],buf1_lhs[17], boundary_y,boundary_z,Acc2_txA2_5_heads0,sel,enable);
    Acc2_send_LHS_type0<18>(buf0_lhs[18],buf1_lhs[18], boundary_y,boundary_z,Acc2_txA3_0_heads0,sel,enable);
    Acc2_send_LHS_type0<19>(buf0_lhs[19],buf1_lhs[19], boundary_y,boundary_z,Acc2_txA3_1_heads0,sel,enable);
    Acc2_send_LHS_type0<20>(buf0_lhs[20],buf1_lhs[20], boundary_y,boundary_z,Acc2_txA3_2_heads0,sel,enable);
    Acc2_send_LHS_type0<21>(buf0_lhs[21],buf1_lhs[21], boundary_y,boundary_z,Acc2_txA3_3_heads0,sel,enable);
    Acc2_send_LHS_type0<22>(buf0_lhs[22],buf1_lhs[22], boundary_y,boundary_z,Acc2_txA3_4_heads0,sel,enable);
    Acc2_send_LHS_type0<23>(buf0_lhs[23],buf1_lhs[23], boundary_y,boundary_z,Acc2_txA3_5_heads0,sel,enable);
      
}


template<int NC>
void Acc2_send_RHS_type0(const int boundary_y, const int boundary_z, axis_stream_plio_128& Acc2_txB0, const bool enable){
#pragma HLS inline off
    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC2_M0/ACC2_A/ACC2_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {
                for (int y = 0; y < boundary_y; y++) {
                    for (int w1 = 0; w1 < ACC2_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC2_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=w2+z*ACC2_RW2;
                            int pos1=w1+y*ACC2_RW1;
                            data = (ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1), ap_int<8>(1), ap_int<8>(-1));
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc2_txB0.write(tmp);
                        }
                    }
                }
            }   
        }   
    }
}

template<int NC>
void Acc2_send_RHS_type0_top(const int boundary_y, const int boundary_z,
                             axis_stream_plio_128& Acc2_txB0_0_heads0, axis_stream_plio_128& Acc2_txB0_1_heads0, axis_stream_plio_128& Acc2_txB0_2_heads0, axis_stream_plio_128& Acc2_txB0_3_heads0, axis_stream_plio_128& Acc2_txB0_4_heads0, axis_stream_plio_128& Acc2_txB0_5_heads0,  
                             axis_stream_plio_128& Acc2_txB1_0_heads0, axis_stream_plio_128& Acc2_txB1_1_heads0, axis_stream_plio_128& Acc2_txB1_2_heads0, axis_stream_plio_128& Acc2_txB1_3_heads0, axis_stream_plio_128& Acc2_txB1_4_heads0, axis_stream_plio_128& Acc2_txB1_5_heads0,  
                             axis_stream_plio_128& Acc2_txB2_0_heads0, axis_stream_plio_128& Acc2_txB2_1_heads0, axis_stream_plio_128& Acc2_txB2_2_heads0, axis_stream_plio_128& Acc2_txB2_3_heads0, axis_stream_plio_128& Acc2_txB2_4_heads0, axis_stream_plio_128& Acc2_txB2_5_heads0,  
                             axis_stream_plio_128& Acc2_txB3_0_heads0, axis_stream_plio_128& Acc2_txB3_1_heads0, axis_stream_plio_128& Acc2_txB3_2_heads0, axis_stream_plio_128& Acc2_txB3_3_heads0, axis_stream_plio_128& Acc2_txB3_4_heads0, axis_stream_plio_128& Acc2_txB3_5_heads0,  
                             axis_stream_plio_128& Acc2_txB4_0_heads0, axis_stream_plio_128& Acc2_txB4_1_heads0, axis_stream_plio_128& Acc2_txB4_2_heads0, axis_stream_plio_128& Acc2_txB4_3_heads0, axis_stream_plio_128& Acc2_txB4_4_heads0, axis_stream_plio_128& Acc2_txB4_5_heads0,  
                             axis_stream_plio_128& Acc2_txB5_0_heads0, axis_stream_plio_128& Acc2_txB5_1_heads0, axis_stream_plio_128& Acc2_txB5_2_heads0, axis_stream_plio_128& Acc2_txB5_3_heads0, axis_stream_plio_128& Acc2_txB5_4_heads0, axis_stream_plio_128& Acc2_txB5_5_heads0,  
                             const bool enable){
    Acc2_send_RHS_type0<0>(boundary_y,boundary_z,Acc2_txB0_0_heads0,enable);
    Acc2_send_RHS_type0<1>(boundary_y,boundary_z,Acc2_txB0_1_heads0,enable);
    Acc2_send_RHS_type0<2>(boundary_y,boundary_z,Acc2_txB0_2_heads0,enable);
    Acc2_send_RHS_type0<3>(boundary_y,boundary_z,Acc2_txB0_3_heads0,enable);
    Acc2_send_RHS_type0<4>(boundary_y,boundary_z,Acc2_txB0_4_heads0,enable);
    Acc2_send_RHS_type0<5>(boundary_y,boundary_z,Acc2_txB0_5_heads0,enable);
    Acc2_send_RHS_type0<6>(boundary_y,boundary_z,Acc2_txB1_0_heads0,enable);
    Acc2_send_RHS_type0<7>(boundary_y,boundary_z,Acc2_txB1_1_heads0,enable);
    Acc2_send_RHS_type0<8>(boundary_y,boundary_z,Acc2_txB1_2_heads0,enable);
    Acc2_send_RHS_type0<9>(boundary_y,boundary_z,Acc2_txB1_3_heads0,enable);
    Acc2_send_RHS_type0<10>(boundary_y,boundary_z,Acc2_txB1_4_heads0,enable);
    Acc2_send_RHS_type0<11>(boundary_y,boundary_z,Acc2_txB1_5_heads0,enable);
    Acc2_send_RHS_type0<12>(boundary_y,boundary_z,Acc2_txB2_0_heads0,enable);
    Acc2_send_RHS_type0<13>(boundary_y,boundary_z,Acc2_txB2_1_heads0,enable);
    Acc2_send_RHS_type0<14>(boundary_y,boundary_z,Acc2_txB2_2_heads0,enable);
    Acc2_send_RHS_type0<15>(boundary_y,boundary_z,Acc2_txB2_3_heads0,enable);
    Acc2_send_RHS_type0<16>(boundary_y,boundary_z,Acc2_txB2_4_heads0,enable);
    Acc2_send_RHS_type0<17>(boundary_y,boundary_z,Acc2_txB2_5_heads0,enable);
    Acc2_send_RHS_type0<18>(boundary_y,boundary_z,Acc2_txB3_0_heads0,enable);
    Acc2_send_RHS_type0<19>(boundary_y,boundary_z,Acc2_txB3_1_heads0,enable);
    Acc2_send_RHS_type0<20>(boundary_y,boundary_z,Acc2_txB3_2_heads0,enable);
    Acc2_send_RHS_type0<21>(boundary_y,boundary_z,Acc2_txB3_3_heads0,enable);
    Acc2_send_RHS_type0<22>(boundary_y,boundary_z,Acc2_txB3_4_heads0,enable);
    Acc2_send_RHS_type0<23>(boundary_y,boundary_z,Acc2_txB3_5_heads0,enable);
    Acc2_send_RHS_type0<24>(boundary_y,boundary_z,Acc2_txB4_0_heads0,enable);
    Acc2_send_RHS_type0<25>(boundary_y,boundary_z,Acc2_txB4_1_heads0,enable);
    Acc2_send_RHS_type0<26>(boundary_y,boundary_z,Acc2_txB4_2_heads0,enable);
    Acc2_send_RHS_type0<27>(boundary_y,boundary_z,Acc2_txB4_3_heads0,enable);
    Acc2_send_RHS_type0<28>(boundary_y,boundary_z,Acc2_txB4_4_heads0,enable);
    Acc2_send_RHS_type0<29>(boundary_y,boundary_z,Acc2_txB4_5_heads0,enable);
    Acc2_send_RHS_type0<30>(boundary_y,boundary_z,Acc2_txB5_0_heads0,enable);
    Acc2_send_RHS_type0<31>(boundary_y,boundary_z,Acc2_txB5_1_heads0,enable);
    Acc2_send_RHS_type0<32>(boundary_y,boundary_z,Acc2_txB5_2_heads0,enable);
    Acc2_send_RHS_type0<33>(boundary_y,boundary_z,Acc2_txB5_3_heads0,enable);
    Acc2_send_RHS_type0<34>(boundary_y,boundary_z,Acc2_txB5_4_heads0,enable);
    Acc2_send_RHS_type0<35>(boundary_y,boundary_z,Acc2_txB5_5_heads0,enable);
      
}

template<int NC>
void Acc2_receive_OUT(axis_stream_plio_128& Acc2_rxC, const int boundary_y, const int boundary_z,axis_stream_128& Acc2_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        ap_int<8>  data_temp1[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp1 complete dim=0

        ap_int<8>  data_temp2[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp2 complete dim=0

        data_plio_128   temp_array[ACC2_LH1][ACC2_RW2];
        #pragma HLS bind_storage variable=temp_array type=RAM_1P impl=LUTRAM

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC2_M0/ACC2_A/ACC2_H1); x++) {
            for (int z = 0; z < boundary_z; z++) {          
                for (int y = 0; y < boundary_y; y++) {
                    for (int h1 = 0; h1 < ACC2_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC2_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        #pragma HLS dependence variable=temp_array type=inter false
                            tmp   =  Acc2_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            if(boundary_y<=1){
                                for (int un=0; un<NUM_PER_128; un++){
                                #pragma HLS UNROLL factor=NUM_PER_128
                                    data_temp(un*8+7,un*8) = data_temp0[un];
                                }
                                
                                Acc2_sout0.write(data_temp);
                                
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
                                    
                                    Acc2_sout0.write(data_temp);
                                    
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

template<int NC>
void Acc2_receive_buff_type0(axis_stream_128& Acc2_sin0, const int boundary_z,
                             ap_uint<PLIO_WIDTH_128> buf0_out0[BUFF2_TYPE0_DIM1][BUFF2_TYPE0_DIM0],
                             ap_uint<PLIO_WIDTH_128> buf0_out1_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                             ap_uint<PLIO_WIDTH_128> buf0_out1_1[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                             ap_uint<PLIO_WIDTH_128> buf1_out0_0[ACC2_M2/ACC2_A/NUM_PER_64][ACC2_N2/ACC2_C/(NUM_PER_128/NUM_PER_64)],
                             const bool sel, const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;
        const int buffer_w2 = ACC2_N0/BUFF2_OUT_PARTC/2;
        const int buffer_h1 = ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64;
        for (int x = 0; x < (ACC2_M0/ACC2_A/ACC2_H1); x++) { 
            for (int z = 0; z < boundary_z; z++) {          
                for (int h1 = 0; h1 < ACC2_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC2_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc2_sin0.read();
                        int pos0_temp = (w2+z*ACC2_RW2);
                        int pos1_temp=  (h1+x*ACC2_LH1);
                        int pos0=pos0_temp%buffer_w2;
                        int pos1=pos1_temp%buffer_h1;
                        int temp_c=pos0_temp/buffer_w2;
                        int temp_a=pos1_temp/buffer_h1;
                        if(sel==0){
                            buf0_out0[pos1_temp][pos0_temp] = data_temp;
                        }
                        else{
                            if (temp_c==0&&temp_a==0){ 
                                buf0_out1_0[pos1][pos0] = data_temp;
                            }
                            else{
                                buf0_out1_1[pos1][pos0] = data_temp;
                            }
                            buf1_out0_0[pos1_temp][pos0_temp] = data_temp;
                        }
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc2_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf0_out0[BUFF2_TYPE0_DIM3*BUFF2_TYPE0_DIM2][BUFF2_TYPE0_DIM1][BUFF2_TYPE0_DIM0],
                                ap_uint<PLIO_WIDTH_128> buf0_out1[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                ap_uint<PLIO_WIDTH_128> buf1_out[ACC2_A*ACC2_C*ACC2_HEADS][ACC2_M2/ACC2_A/NUM_PER_64][ACC2_N2/ACC2_C/(NUM_PER_128/NUM_PER_64)],
                                const int boundary_y, const int boundary_z,
                                axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
                                axis_stream_plio_128& Acc2_rxC1_0_heads0, axis_stream_plio_128& Acc2_rxC1_1_heads0, axis_stream_plio_128& Acc2_rxC1_2_heads0, axis_stream_plio_128& Acc2_rxC1_3_heads0, axis_stream_plio_128& Acc2_rxC1_4_heads0, axis_stream_plio_128& Acc2_rxC1_5_heads0,  
                                axis_stream_plio_128& Acc2_rxC2_0_heads0, axis_stream_plio_128& Acc2_rxC2_1_heads0, axis_stream_plio_128& Acc2_rxC2_2_heads0, axis_stream_plio_128& Acc2_rxC2_3_heads0, axis_stream_plio_128& Acc2_rxC2_4_heads0, axis_stream_plio_128& Acc2_rxC2_5_heads0,  
                                axis_stream_plio_128& Acc2_rxC3_0_heads0, axis_stream_plio_128& Acc2_rxC3_1_heads0, axis_stream_plio_128& Acc2_rxC3_2_heads0, axis_stream_plio_128& Acc2_rxC3_3_heads0, axis_stream_plio_128& Acc2_rxC3_4_heads0, axis_stream_plio_128& Acc2_rxC3_5_heads0,  
                                const bool sel, const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc2_plio0_0_heads0_f0;axis_stream_128 Acc2_plio0_1_heads0_f0;axis_stream_128 Acc2_plio0_2_heads0_f0;axis_stream_128 Acc2_plio0_3_heads0_f0;axis_stream_128 Acc2_plio0_4_heads0_f0;axis_stream_128 Acc2_plio0_5_heads0_f0; 
    axis_stream_128 Acc2_plio1_0_heads0_f0;axis_stream_128 Acc2_plio1_1_heads0_f0;axis_stream_128 Acc2_plio1_2_heads0_f0;axis_stream_128 Acc2_plio1_3_heads0_f0;axis_stream_128 Acc2_plio1_4_heads0_f0;axis_stream_128 Acc2_plio1_5_heads0_f0; 
    axis_stream_128 Acc2_plio2_0_heads0_f0;axis_stream_128 Acc2_plio2_1_heads0_f0;axis_stream_128 Acc2_plio2_2_heads0_f0;axis_stream_128 Acc2_plio2_3_heads0_f0;axis_stream_128 Acc2_plio2_4_heads0_f0;axis_stream_128 Acc2_plio2_5_heads0_f0; 
    axis_stream_128 Acc2_plio3_0_heads0_f0;axis_stream_128 Acc2_plio3_1_heads0_f0;axis_stream_128 Acc2_plio3_2_heads0_f0;axis_stream_128 Acc2_plio3_3_heads0_f0;axis_stream_128 Acc2_plio3_4_heads0_f0;axis_stream_128 Acc2_plio3_5_heads0_f0; 
    
    Acc2_receive_OUT<0 >(Acc2_rxC0_0_heads0,boundary_y,boundary_z,Acc2_plio0_0_heads0_f0, enable); 
    Acc2_receive_OUT<1 >(Acc2_rxC0_1_heads0,boundary_y,boundary_z,Acc2_plio0_1_heads0_f0, enable); 
    Acc2_receive_OUT<2 >(Acc2_rxC0_2_heads0,boundary_y,boundary_z,Acc2_plio0_2_heads0_f0, enable); 
    Acc2_receive_OUT<3 >(Acc2_rxC0_3_heads0,boundary_y,boundary_z,Acc2_plio0_3_heads0_f0, enable); 
    Acc2_receive_OUT<4 >(Acc2_rxC0_4_heads0,boundary_y,boundary_z,Acc2_plio0_4_heads0_f0, enable); 
    Acc2_receive_OUT<5 >(Acc2_rxC0_5_heads0,boundary_y,boundary_z,Acc2_plio0_5_heads0_f0, enable); 
    Acc2_receive_OUT<6 >(Acc2_rxC1_0_heads0,boundary_y,boundary_z,Acc2_plio1_0_heads0_f0, enable); 
    Acc2_receive_OUT<7 >(Acc2_rxC1_1_heads0,boundary_y,boundary_z,Acc2_plio1_1_heads0_f0, enable); 
    Acc2_receive_OUT<8 >(Acc2_rxC1_2_heads0,boundary_y,boundary_z,Acc2_plio1_2_heads0_f0, enable); 
    Acc2_receive_OUT<9 >(Acc2_rxC1_3_heads0,boundary_y,boundary_z,Acc2_plio1_3_heads0_f0, enable); 
    Acc2_receive_OUT<10>(Acc2_rxC1_4_heads0,boundary_y,boundary_z,Acc2_plio1_4_heads0_f0, enable); 
    Acc2_receive_OUT<11>(Acc2_rxC1_5_heads0,boundary_y,boundary_z,Acc2_plio1_5_heads0_f0, enable); 
    Acc2_receive_OUT<12>(Acc2_rxC2_0_heads0,boundary_y,boundary_z,Acc2_plio2_0_heads0_f0, enable); 
    Acc2_receive_OUT<13>(Acc2_rxC2_1_heads0,boundary_y,boundary_z,Acc2_plio2_1_heads0_f0, enable); 
    Acc2_receive_OUT<14>(Acc2_rxC2_2_heads0,boundary_y,boundary_z,Acc2_plio2_2_heads0_f0, enable); 
    Acc2_receive_OUT<15>(Acc2_rxC2_3_heads0,boundary_y,boundary_z,Acc2_plio2_3_heads0_f0, enable); 
    Acc2_receive_OUT<16>(Acc2_rxC2_4_heads0,boundary_y,boundary_z,Acc2_plio2_4_heads0_f0, enable); 
    Acc2_receive_OUT<17>(Acc2_rxC2_5_heads0,boundary_y,boundary_z,Acc2_plio2_5_heads0_f0, enable); 
    Acc2_receive_OUT<18>(Acc2_rxC3_0_heads0,boundary_y,boundary_z,Acc2_plio3_0_heads0_f0, enable); 
    Acc2_receive_OUT<19>(Acc2_rxC3_1_heads0,boundary_y,boundary_z,Acc2_plio3_1_heads0_f0, enable); 
    Acc2_receive_OUT<20>(Acc2_rxC3_2_heads0,boundary_y,boundary_z,Acc2_plio3_2_heads0_f0, enable); 
    Acc2_receive_OUT<21>(Acc2_rxC3_3_heads0,boundary_y,boundary_z,Acc2_plio3_3_heads0_f0, enable); 
    Acc2_receive_OUT<22>(Acc2_rxC3_4_heads0,boundary_y,boundary_z,Acc2_plio3_4_heads0_f0, enable); 
    Acc2_receive_OUT<23>(Acc2_rxC3_5_heads0,boundary_y,boundary_z,Acc2_plio3_5_heads0_f0, enable); 
    
    
    Acc2_receive_buff_type0<0>(Acc2_plio0_0_heads0_f0, boundary_z, buf0_out0[0 ],buf0_out1[0], buf0_out1[1], buf1_out[0 ],sel,enable);
    Acc2_receive_buff_type0<1>(Acc2_plio0_1_heads0_f0, boundary_z, buf0_out0[1 ],buf0_out1[2], buf0_out1[3], buf1_out[1 ],sel,enable);
    Acc2_receive_buff_type0<2>(Acc2_plio0_2_heads0_f0, boundary_z, buf0_out0[2 ],buf0_out1[4], buf0_out1[5], buf1_out[2 ],sel,enable);
    Acc2_receive_buff_type0<3>(Acc2_plio0_3_heads0_f0, boundary_z, buf0_out0[3 ],buf0_out1[6], buf0_out1[7], buf1_out[3 ],sel,enable);
    Acc2_receive_buff_type0<4>(Acc2_plio0_4_heads0_f0, boundary_z, buf0_out0[4 ],buf0_out1[8], buf0_out1[9], buf1_out[4 ],sel,enable);
    Acc2_receive_buff_type0<5>(Acc2_plio0_5_heads0_f0, boundary_z, buf0_out0[5 ],buf0_out1[10],buf0_out1[11],buf1_out[5 ],sel,enable);
    Acc2_receive_buff_type0<6>(Acc2_plio1_0_heads0_f0, boundary_z, buf0_out0[6 ],buf0_out1[12],buf0_out1[13],buf1_out[6 ],sel,enable);
    Acc2_receive_buff_type0<7>(Acc2_plio1_1_heads0_f0, boundary_z, buf0_out0[7 ],buf0_out1[14],buf0_out1[15],buf1_out[7 ],sel,enable);
    Acc2_receive_buff_type0<8>(Acc2_plio1_2_heads0_f0, boundary_z, buf0_out0[8 ],buf0_out1[16],buf0_out1[17],buf1_out[8 ],sel,enable);
    Acc2_receive_buff_type0<9>(Acc2_plio1_3_heads0_f0, boundary_z, buf0_out0[9 ],buf0_out1[18],buf0_out1[19],buf1_out[9 ],sel,enable);
    Acc2_receive_buff_type0<10>(Acc2_plio1_4_heads0_f0,boundary_z, buf0_out0[10],buf0_out1[20],buf0_out1[21],buf1_out[10],sel,enable);
    Acc2_receive_buff_type0<11>(Acc2_plio1_5_heads0_f0,boundary_z, buf0_out0[11],buf0_out1[22],buf0_out1[23],buf1_out[11],sel,enable);
    Acc2_receive_buff_type0<12>(Acc2_plio2_0_heads0_f0,boundary_z, buf0_out0[12],buf0_out1[24],buf0_out1[25],buf1_out[12],sel,enable);
    Acc2_receive_buff_type0<13>(Acc2_plio2_1_heads0_f0,boundary_z, buf0_out0[13],buf0_out1[26],buf0_out1[27],buf1_out[13],sel,enable);
    Acc2_receive_buff_type0<14>(Acc2_plio2_2_heads0_f0,boundary_z, buf0_out0[14],buf0_out1[28],buf0_out1[29],buf1_out[14],sel,enable);
    Acc2_receive_buff_type0<15>(Acc2_plio2_3_heads0_f0,boundary_z, buf0_out0[15],buf0_out1[30],buf0_out1[31],buf1_out[15],sel,enable);
    Acc2_receive_buff_type0<16>(Acc2_plio2_4_heads0_f0,boundary_z, buf0_out0[16],buf0_out1[32],buf0_out1[33],buf1_out[16],sel,enable);
    Acc2_receive_buff_type0<17>(Acc2_plio2_5_heads0_f0,boundary_z, buf0_out0[17],buf0_out1[34],buf0_out1[35],buf1_out[17],sel,enable);
    Acc2_receive_buff_type0<18>(Acc2_plio3_0_heads0_f0,boundary_z, buf0_out0[18],buf0_out1[36],buf0_out1[37],buf1_out[18],sel,enable);
    Acc2_receive_buff_type0<19>(Acc2_plio3_1_heads0_f0,boundary_z, buf0_out0[19],buf0_out1[38],buf0_out1[39],buf1_out[19],sel,enable);
    Acc2_receive_buff_type0<20>(Acc2_plio3_2_heads0_f0,boundary_z, buf0_out0[20],buf0_out1[40],buf0_out1[41],buf1_out[20],sel,enable);
    Acc2_receive_buff_type0<21>(Acc2_plio3_3_heads0_f0,boundary_z, buf0_out0[21],buf0_out1[42],buf0_out1[43],buf1_out[21],sel,enable);
    Acc2_receive_buff_type0<22>(Acc2_plio3_4_heads0_f0,boundary_z, buf0_out0[22],buf0_out1[44],buf0_out1[45],buf1_out[22],sel,enable);
    Acc2_receive_buff_type0<23>(Acc2_plio3_5_heads0_f0,boundary_z, buf0_out0[23],buf0_out1[46],buf0_out1[47],buf1_out[23],sel,enable);
    
}

template<int NC>
void m2vector(ap_uint<PLIO_WIDTH_128> buff0[ACC2_A*ACC2_C*ACC2_HEADS][ACC2_M0/ACC2_A/NUM_PER_64][ACC2_N0/ACC2_C/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64],const bool enable){
#pragma HLS inline off

    if (enable){
        data_plio_64 temp_out0;
        data_plio_64 temp_out1;
        data_plio_128 data0;
        data_plio_128 data1;
        ap_uint<8> temp_data[2][8];
        #pragma HLS ARRAY_PARTITION variable=temp_data complete dim=1
        #pragma HLS ARRAY_PARTITION variable=temp_data complete dim=2

        for (int p=0;p<ACC2_C/FC_B;p++){
            for (int j=0; j<(FC_K/ACC2_C/NUM_PER_64);j++){
                for (int i=0; i < 4;i++){
                #pragma HLS PIPELINE II = 1
                    int pos0= i + j * 4;
                    data0 = buff0[p+(ACC2_C*(ACC2_A-1))][(ACC2_M0/ACC2_A/NUM_PER_64-1)][pos0];
                    data1 = buff0[p+(ACC2_C*(ACC2_A-1)+(ACC2_C/FC_B))][(ACC2_M0/ACC2_A/NUM_PER_64-1)][pos0];
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

template<int NC>
void Acc2_m2vector(ap_uint<PLIO_WIDTH_128> buf0_in[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0],
                   ap_uint<PLIO_WIDTH_128> buf0_out[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                   ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64], 
                   axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0, axis_stream_plio_128& Acc2_txA0_4_heads0, axis_stream_plio_128& Acc2_txA0_5_heads0,  
                   axis_stream_plio_128& Acc2_txA1_0_heads0, axis_stream_plio_128& Acc2_txA1_1_heads0, axis_stream_plio_128& Acc2_txA1_2_heads0, axis_stream_plio_128& Acc2_txA1_3_heads0, axis_stream_plio_128& Acc2_txA1_4_heads0, axis_stream_plio_128& Acc2_txA1_5_heads0,  
                   axis_stream_plio_128& Acc2_txA2_0_heads0, axis_stream_plio_128& Acc2_txA2_1_heads0, axis_stream_plio_128& Acc2_txA2_2_heads0, axis_stream_plio_128& Acc2_txA2_3_heads0, axis_stream_plio_128& Acc2_txA2_4_heads0, axis_stream_plio_128& Acc2_txA2_5_heads0,  
                   axis_stream_plio_128& Acc2_txA3_0_heads0, axis_stream_plio_128& Acc2_txA3_1_heads0, axis_stream_plio_128& Acc2_txA3_2_heads0, axis_stream_plio_128& Acc2_txA3_3_heads0, axis_stream_plio_128& Acc2_txA3_4_heads0, axis_stream_plio_128& Acc2_txA3_5_heads0,  
                   axis_stream_plio_128& Acc2_txB0_0_heads0, axis_stream_plio_128& Acc2_txB0_1_heads0, axis_stream_plio_128& Acc2_txB0_2_heads0, axis_stream_plio_128& Acc2_txB0_3_heads0, axis_stream_plio_128& Acc2_txB0_4_heads0, axis_stream_plio_128& Acc2_txB0_5_heads0,  
                   axis_stream_plio_128& Acc2_txB1_0_heads0, axis_stream_plio_128& Acc2_txB1_1_heads0, axis_stream_plio_128& Acc2_txB1_2_heads0, axis_stream_plio_128& Acc2_txB1_3_heads0, axis_stream_plio_128& Acc2_txB1_4_heads0, axis_stream_plio_128& Acc2_txB1_5_heads0,  
                   axis_stream_plio_128& Acc2_txB2_0_heads0, axis_stream_plio_128& Acc2_txB2_1_heads0, axis_stream_plio_128& Acc2_txB2_2_heads0, axis_stream_plio_128& Acc2_txB2_3_heads0, axis_stream_plio_128& Acc2_txB2_4_heads0, axis_stream_plio_128& Acc2_txB2_5_heads0,  
                   axis_stream_plio_128& Acc2_txB3_0_heads0, axis_stream_plio_128& Acc2_txB3_1_heads0, axis_stream_plio_128& Acc2_txB3_2_heads0, axis_stream_plio_128& Acc2_txB3_3_heads0, axis_stream_plio_128& Acc2_txB3_4_heads0, axis_stream_plio_128& Acc2_txB3_5_heads0,  
                   axis_stream_plio_128& Acc2_txB4_0_heads0, axis_stream_plio_128& Acc2_txB4_1_heads0, axis_stream_plio_128& Acc2_txB4_2_heads0, axis_stream_plio_128& Acc2_txB4_3_heads0, axis_stream_plio_128& Acc2_txB4_4_heads0, axis_stream_plio_128& Acc2_txB4_5_heads0,  
                   axis_stream_plio_128& Acc2_txB5_0_heads0, axis_stream_plio_128& Acc2_txB5_1_heads0, axis_stream_plio_128& Acc2_txB5_2_heads0, axis_stream_plio_128& Acc2_txB5_3_heads0, axis_stream_plio_128& Acc2_txB5_4_heads0, axis_stream_plio_128& Acc2_txB5_5_heads0,
                   axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
                   axis_stream_plio_128& Acc2_rxC1_0_heads0, axis_stream_plio_128& Acc2_rxC1_1_heads0, axis_stream_plio_128& Acc2_rxC1_2_heads0, axis_stream_plio_128& Acc2_rxC1_3_heads0, axis_stream_plio_128& Acc2_rxC1_4_heads0, axis_stream_plio_128& Acc2_rxC1_5_heads0,  
                   axis_stream_plio_128& Acc2_rxC2_0_heads0, axis_stream_plio_128& Acc2_rxC2_1_heads0, axis_stream_plio_128& Acc2_rxC2_2_heads0, axis_stream_plio_128& Acc2_rxC2_3_heads0, axis_stream_plio_128& Acc2_rxC2_4_heads0, axis_stream_plio_128& Acc2_rxC2_5_heads0,  
                   axis_stream_plio_128& Acc2_rxC3_0_heads0, axis_stream_plio_128& Acc2_rxC3_1_heads0, axis_stream_plio_128& Acc2_rxC3_2_heads0, axis_stream_plio_128& Acc2_rxC3_3_heads0, axis_stream_plio_128& Acc2_rxC3_4_heads0, axis_stream_plio_128& Acc2_rxC3_5_heads0,  
                   const bool enable_0,const bool enable_1){

#pragma HLS inline off 
    ap_uint<PLIO_WIDTH_128> buf0_out0[BUFF2_TYPE0_DIM3*BUFF2_TYPE0_DIM2][BUFF2_TYPE0_DIM1][BUFF2_TYPE0_DIM0];//Output for Acc0 as well as intermidate data
    #pragma HLS bind_storage variable=buf0_out0 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buf0_out0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buf0_out1[BUFF2_TYPE0_DIM3*BUFF2_TYPE0_DIM2][BUFF2_TYPE0_DIM1][BUFF2_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buf0_out1 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buf0_out1 complete dim=1

    ap_uint<PLIO_WIDTH_128> buf1_out0[ACC2_A*ACC2_C*ACC2_HEADS][ACC2_M0/ACC2_A/NUM_PER_64][ACC2_N0/ACC2_C/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buf1_out0 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buf1_out0 complete dim=1
    
    for(int i=0 ; i < 4; i++){
        if(i==0){
            Acc2_send_LHS_type0_top<0>(buf0_in, buf0_out0, 1, 1, Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0, Acc2_txA0_4_heads0, Acc2_txA0_5_heads0,  Acc2_txA1_0_heads0, Acc2_txA1_1_heads0, Acc2_txA1_2_heads0, Acc2_txA1_3_heads0, Acc2_txA1_4_heads0, Acc2_txA1_5_heads0,  Acc2_txA2_0_heads0, Acc2_txA2_1_heads0, Acc2_txA2_2_heads0, Acc2_txA2_3_heads0, Acc2_txA2_4_heads0, Acc2_txA2_5_heads0,  Acc2_txA3_0_heads0, Acc2_txA3_1_heads0, Acc2_txA3_2_heads0, Acc2_txA3_3_heads0, Acc2_txA3_4_heads0, Acc2_txA3_5_heads0, 0, enable_0);
            Acc2_send_RHS_type0_top<0>(1, 1, Acc2_txB0_0_heads0, Acc2_txB0_1_heads0, Acc2_txB0_2_heads0, Acc2_txB0_3_heads0, Acc2_txB0_4_heads0, Acc2_txB0_5_heads0,Acc2_txB1_0_heads0, Acc2_txB1_1_heads0, Acc2_txB1_2_heads0, Acc2_txB1_3_heads0, Acc2_txB1_4_heads0, Acc2_txB1_5_heads0, Acc2_txB2_0_heads0, Acc2_txB2_1_heads0, Acc2_txB2_2_heads0, Acc2_txB2_3_heads0, Acc2_txB2_4_heads0, Acc2_txB2_5_heads0,Acc2_txB3_0_heads0, Acc2_txB3_1_heads0, Acc2_txB3_2_heads0, Acc2_txB3_3_heads0, Acc2_txB3_4_heads0, Acc2_txB3_5_heads0,Acc2_txB4_0_heads0, Acc2_txB4_1_heads0, Acc2_txB4_2_heads0, Acc2_txB4_3_heads0, Acc2_txB4_4_heads0, Acc2_txB4_5_heads0,Acc2_txB5_0_heads0, Acc2_txB5_1_heads0, Acc2_txB5_2_heads0, Acc2_txB5_3_heads0, Acc2_txB5_4_heads0, Acc2_txB5_5_heads0, enable_0);
            Acc2_receive_OUT_type0_top<0>(buf0_out1, buf0_out, buf1_out0, 1, 1, Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0, Acc2_rxC1_0_heads0, Acc2_rxC1_1_heads0, Acc2_rxC1_2_heads0, Acc2_rxC1_3_heads0, Acc2_rxC1_4_heads0, Acc2_rxC1_5_heads0, Acc2_rxC2_0_heads0, Acc2_rxC2_1_heads0, Acc2_rxC2_2_heads0, Acc2_rxC2_3_heads0, Acc2_rxC2_4_heads0, Acc2_rxC2_5_heads0, Acc2_rxC3_0_heads0, Acc2_rxC3_1_heads0, Acc2_rxC3_2_heads0, Acc2_rxC3_3_heads0, Acc2_rxC3_4_heads0, Acc2_rxC3_5_heads0, 0, enable_0);
        }
        else if(i==1){
            Acc2_send_LHS_type0_top<0>(buf0_in, buf0_out1, 1, 4, Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0, Acc2_txA0_4_heads0, Acc2_txA0_5_heads0,  Acc2_txA1_0_heads0, Acc2_txA1_1_heads0, Acc2_txA1_2_heads0, Acc2_txA1_3_heads0, Acc2_txA1_4_heads0, Acc2_txA1_5_heads0,  Acc2_txA2_0_heads0, Acc2_txA2_1_heads0, Acc2_txA2_2_heads0, Acc2_txA2_3_heads0, Acc2_txA2_4_heads0, Acc2_txA2_5_heads0,  Acc2_txA3_0_heads0, Acc2_txA3_1_heads0, Acc2_txA3_2_heads0, Acc2_txA3_3_heads0, Acc2_txA3_4_heads0, Acc2_txA3_5_heads0, 1, enable_0);
            Acc2_send_RHS_type0_top<0>(1, 4, Acc2_txB0_0_heads0, Acc2_txB0_1_heads0, Acc2_txB0_2_heads0, Acc2_txB0_3_heads0, Acc2_txB0_4_heads0, Acc2_txB0_5_heads0,Acc2_txB1_0_heads0, Acc2_txB1_1_heads0, Acc2_txB1_2_heads0, Acc2_txB1_3_heads0, Acc2_txB1_4_heads0, Acc2_txB1_5_heads0, Acc2_txB2_0_heads0, Acc2_txB2_1_heads0, Acc2_txB2_2_heads0, Acc2_txB2_3_heads0, Acc2_txB2_4_heads0, Acc2_txB2_5_heads0,Acc2_txB3_0_heads0, Acc2_txB3_1_heads0, Acc2_txB3_2_heads0, Acc2_txB3_3_heads0, Acc2_txB3_4_heads0, Acc2_txB3_5_heads0,Acc2_txB4_0_heads0, Acc2_txB4_1_heads0, Acc2_txB4_2_heads0, Acc2_txB4_3_heads0, Acc2_txB4_4_heads0, Acc2_txB4_5_heads0,Acc2_txB5_0_heads0, Acc2_txB5_1_heads0, Acc2_txB5_2_heads0, Acc2_txB5_3_heads0, Acc2_txB5_4_heads0, Acc2_txB5_5_heads0, enable_0);
            Acc2_receive_OUT_type0_top<0>(buf0_out0, buf0_out, buf1_out0, 1, 4, Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0, Acc2_rxC1_0_heads0, Acc2_rxC1_1_heads0, Acc2_rxC1_2_heads0, Acc2_rxC1_3_heads0, Acc2_rxC1_4_heads0, Acc2_rxC1_5_heads0, Acc2_rxC2_0_heads0, Acc2_rxC2_1_heads0, Acc2_rxC2_2_heads0, Acc2_rxC2_3_heads0, Acc2_rxC2_4_heads0, Acc2_rxC2_5_heads0, Acc2_rxC3_0_heads0, Acc2_rxC3_1_heads0, Acc2_rxC3_2_heads0, Acc2_rxC3_3_heads0, Acc2_rxC3_4_heads0, Acc2_rxC3_5_heads0, 0, enable_0);
        }
        else if(i==2){
            Acc2_send_LHS_type0_top<0>(buf0_in, buf0_out0, 4, 1, Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0, Acc2_txA0_4_heads0, Acc2_txA0_5_heads0,  Acc2_txA1_0_heads0, Acc2_txA1_1_heads0, Acc2_txA1_2_heads0, Acc2_txA1_3_heads0, Acc2_txA1_4_heads0, Acc2_txA1_5_heads0,  Acc2_txA2_0_heads0, Acc2_txA2_1_heads0, Acc2_txA2_2_heads0, Acc2_txA2_3_heads0, Acc2_txA2_4_heads0, Acc2_txA2_5_heads0,  Acc2_txA3_0_heads0, Acc2_txA3_1_heads0, Acc2_txA3_2_heads0, Acc2_txA3_3_heads0, Acc2_txA3_4_heads0, Acc2_txA3_5_heads0, 1, enable_0);
            Acc2_send_RHS_type0_top<0>(4, 1, Acc2_txB0_0_heads0, Acc2_txB0_1_heads0, Acc2_txB0_2_heads0, Acc2_txB0_3_heads0, Acc2_txB0_4_heads0, Acc2_txB0_5_heads0,Acc2_txB1_0_heads0, Acc2_txB1_1_heads0, Acc2_txB1_2_heads0, Acc2_txB1_3_heads0, Acc2_txB1_4_heads0, Acc2_txB1_5_heads0, Acc2_txB2_0_heads0, Acc2_txB2_1_heads0, Acc2_txB2_2_heads0, Acc2_txB2_3_heads0, Acc2_txB2_4_heads0, Acc2_txB2_5_heads0,Acc2_txB3_0_heads0, Acc2_txB3_1_heads0, Acc2_txB3_2_heads0, Acc2_txB3_3_heads0, Acc2_txB3_4_heads0, Acc2_txB3_5_heads0,Acc2_txB4_0_heads0, Acc2_txB4_1_heads0, Acc2_txB4_2_heads0, Acc2_txB4_3_heads0, Acc2_txB4_4_heads0, Acc2_txB4_5_heads0,Acc2_txB5_0_heads0, Acc2_txB5_1_heads0, Acc2_txB5_2_heads0, Acc2_txB5_3_heads0, Acc2_txB5_4_heads0, Acc2_txB5_5_heads0, enable_0);
            Acc2_receive_OUT_type0_top<0>(buf0_out1, buf0_out, buf1_out0, 4, 1, Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0, Acc2_rxC1_0_heads0, Acc2_rxC1_1_heads0, Acc2_rxC1_2_heads0, Acc2_rxC1_3_heads0, Acc2_rxC1_4_heads0, Acc2_rxC1_5_heads0, Acc2_rxC2_0_heads0, Acc2_rxC2_1_heads0, Acc2_rxC2_2_heads0, Acc2_rxC2_3_heads0, Acc2_rxC2_4_heads0, Acc2_rxC2_5_heads0, Acc2_rxC3_0_heads0, Acc2_rxC3_1_heads0, Acc2_rxC3_2_heads0, Acc2_rxC3_3_heads0, Acc2_rxC3_4_heads0, Acc2_rxC3_5_heads0, 1, enable_0);
        }
        else{
            m2vector<0>(buf1_out0, buff_fc0, enable_1);
        }
    }
}



template<int NC>
void send_patch_wrapper1(ap_uint<PLIO_WIDTH_128> a_buf[PE_KHP][PE_KH][PE_SEQ_ALL],axis_stream_plio_128& txA0,axis_stream_plio_128& txA1,const bool enable){
    send_patch_wrapper0<0>(a_buf[0], txA0, enable);
    send_patch_wrapper0<1>(a_buf[1], txA1, enable);
}

template<int NC>
void send_patch_top(ap_uint<PLIO_WIDTH_128> a_buf[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL],
                    axis_stream_plio_128& txA0, axis_stream_plio_128& txA1, 
                    axis_stream_plio_128& txA2, axis_stream_plio_128& txA3, 
                    axis_stream_plio_128& txA4, axis_stream_plio_128& txA5, const bool enable){
#pragma HLS inline off
    send_patch_wrapper1<0>(a_buf[0],txA0,txA1,enable);
    send_patch_wrapper1<1>(a_buf[1],txA2,txA3,enable);
    send_patch_wrapper1<2>(a_buf[2],txA4,txA5,enable);
    
}

template<int NC>
void load_image_top(axis_stream_A& dataA_in0, axis_stream_A& dataA_in1, axis_stream_A& dataA_in2, ap_uint<PLIO_WIDTH_128> a_buf[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL],const bool enable){
#pragma HLS inline off
    load_image<0>(dataA_in0, a_buf[0], enable);
    load_image<1>(dataA_in1, a_buf[1], enable);
    load_image<2>(dataA_in2, a_buf[2], enable);
}

template<int NC>
void receive_Patch_buff(axis_stream_128& s_in0,
                        ap_uint<PLIO_WIDTH_128> buff0[(ACC0_M0/PE_OUT_PARTA/NUM_PER_64)][ACC0_K0/PE_OUT_PARTC/(NUM_PER_128/NUM_PER_64)],
                        const bool enable){
#pragma HLS inline off
    if(enable){
        data_plio_128 data_temp;
        for (int h1 = 0; h1 < (PE_SEQ_ALL/NUM_PER_64); h1++) {
            for (int w2 = 0; w2 < (PE_OC/2); w2++) {
            #pragma HLS PIPELINE II = 1
                data_temp=s_in0.read();
                
                int pos0 = w2;
                int pos1 = h1;
                buff0[pos1][pos0] = data_temp;
                
            }
        }
    }
}

template<int NC>
void receive_Patch_top(axis_stream_plio_128& rxC0,axis_stream_plio_128& rxC1,axis_stream_plio_128& rxC2,axis_stream_plio_128& rxC3,ap_uint<PLIO_WIDTH_128> buff0[PE_OUT_PARTA*PE_OUT_PARTC][(ACC0_M0/PE_OUT_PARTA/NUM_PER_64)][ACC0_K0/PE_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], const bool enable){
#pragma HLS dataflow
    
    
    axis_stream_128 s_out0;
    axis_stream_128 s_out1;
    axis_stream_128 s_out2;
    axis_stream_128 s_out3;
    
    receive_Patch_transpose<0>(rxC0, s_out0, enable);
    receive_Patch_buff<0>(s_out0,buff0[0],enable);
    
    receive_Patch_transpose<1>(rxC1, s_out1, enable);
    receive_Patch_buff<1>(s_out1,buff0[1],enable);
    
    receive_Patch_transpose<2>(rxC2, s_out2, enable);
    receive_Patch_buff<2>(s_out2,buff0[2],enable);
    
    receive_Patch_transpose<3>(rxC3, s_out3, enable);
    receive_Patch_buff<3>(s_out3,buff0[3],enable);
    
    
}

template<int NC>
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

template<int NC>
void receive_FC(axis_stream_C& dataC_out, 
                axis_stream_plio_128& rxC0, axis_stream_plio_128& rxC1, axis_stream_plio_128& rxC2, axis_stream_plio_128& rxC3, axis_stream_plio_128& rxC4,  
                const bool enable){
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

void forward_control(axis_stream_plio_128& Acc0_txA0_0_heads0, axis_stream_plio_128& Acc0_txA0_1_heads0, axis_stream_plio_128& Acc0_txA0_2_heads0, axis_stream_plio_128& Acc0_txA0_3_heads0,  
                     axis_stream_plio_128& Acc0_rxC0_0_heads0, axis_stream_plio_128& Acc0_rxC0_1_heads0, axis_stream_plio_128& Acc0_rxC0_2_heads0, axis_stream_plio_128& Acc0_rxC0_3_heads0, axis_stream_plio_128& Acc0_rxC0_4_heads0, axis_stream_plio_128& Acc0_rxC0_5_heads0, axis_stream_plio_128& Acc0_rxC0_6_heads0, axis_stream_plio_128& Acc0_rxC0_7_heads0, axis_stream_plio_128& Acc0_rxC0_8_heads0, axis_stream_plio_128& Acc0_rxC0_9_heads0, axis_stream_plio_128& Acc0_rxC0_10_heads0, axis_stream_plio_128& Acc0_rxC0_11_heads0, axis_stream_plio_128& Acc0_rxC0_12_heads0, axis_stream_plio_128& Acc0_rxC0_13_heads0, axis_stream_plio_128& Acc0_rxC0_14_heads0, axis_stream_plio_128& Acc0_rxC0_15_heads0, axis_stream_plio_128& Acc0_rxC0_16_heads0, axis_stream_plio_128& Acc0_rxC0_17_heads0,  
                     axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0,  
                     axis_stream_plio_128& Acc1_txA1_0_heads0, axis_stream_plio_128& Acc1_txA1_1_heads0,  
                     axis_stream_plio_128& Acc1_txA2_0_heads0, axis_stream_plio_128& Acc1_txA2_1_heads0,  
                     axis_stream_plio_128& Acc1_txA3_0_heads0, axis_stream_plio_128& Acc1_txA3_1_heads0,  
                     axis_stream_plio_128& Acc1_txA0_0_heads1, axis_stream_plio_128& Acc1_txA0_1_heads1,  
                     axis_stream_plio_128& Acc1_txA1_0_heads1, axis_stream_plio_128& Acc1_txA1_1_heads1,  
                     axis_stream_plio_128& Acc1_txA2_0_heads1, axis_stream_plio_128& Acc1_txA2_1_heads1,  
                     axis_stream_plio_128& Acc1_txA3_0_heads1, axis_stream_plio_128& Acc1_txA3_1_heads1,  
                     axis_stream_plio_128& Acc1_txA0_0_heads2, axis_stream_plio_128& Acc1_txA0_1_heads2,  
                     axis_stream_plio_128& Acc1_txA1_0_heads2, axis_stream_plio_128& Acc1_txA1_1_heads2,  
                     axis_stream_plio_128& Acc1_txA2_0_heads2, axis_stream_plio_128& Acc1_txA2_1_heads2,  
                     axis_stream_plio_128& Acc1_txA3_0_heads2, axis_stream_plio_128& Acc1_txA3_1_heads2,  
                     axis_stream_plio_128& Acc1_txB0_0_heads0, axis_stream_plio_128& Acc1_txB1_0_heads0,  
                     axis_stream_plio_128& Acc1_txB0_1_heads0, axis_stream_plio_128& Acc1_txB1_1_heads0,  
                     axis_stream_plio_128& Acc1_txB0_0_heads1, axis_stream_plio_128& Acc1_txB1_0_heads1,  
                     axis_stream_plio_128& Acc1_txB0_1_heads1, axis_stream_plio_128& Acc1_txB1_1_heads1,  
                     axis_stream_plio_128& Acc1_txB0_0_heads2, axis_stream_plio_128& Acc1_txB1_0_heads2,  
                     axis_stream_plio_128& Acc1_txB0_1_heads2, axis_stream_plio_128& Acc1_txB1_1_heads2,  
                     axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0,  
                     axis_stream_plio_128& Acc1_rxC1_0_heads0, axis_stream_plio_128& Acc1_rxC1_1_heads0,  
                     axis_stream_plio_128& Acc1_rxC2_0_heads0, axis_stream_plio_128& Acc1_rxC2_1_heads0,  
                     axis_stream_plio_128& Acc1_rxC3_0_heads0, axis_stream_plio_128& Acc1_rxC3_1_heads0,  
                     axis_stream_plio_128& Acc1_rxC0_0_heads1, axis_stream_plio_128& Acc1_rxC0_1_heads1,  
                     axis_stream_plio_128& Acc1_rxC1_0_heads1, axis_stream_plio_128& Acc1_rxC1_1_heads1,  
                     axis_stream_plio_128& Acc1_rxC2_0_heads1, axis_stream_plio_128& Acc1_rxC2_1_heads1,  
                     axis_stream_plio_128& Acc1_rxC3_0_heads1, axis_stream_plio_128& Acc1_rxC3_1_heads1,  
                     axis_stream_plio_128& Acc1_rxC0_0_heads2, axis_stream_plio_128& Acc1_rxC0_1_heads2,  
                     axis_stream_plio_128& Acc1_rxC1_0_heads2, axis_stream_plio_128& Acc1_rxC1_1_heads2,  
                     axis_stream_plio_128& Acc1_rxC2_0_heads2, axis_stream_plio_128& Acc1_rxC2_1_heads2,  
                     axis_stream_plio_128& Acc1_rxC3_0_heads2, axis_stream_plio_128& Acc1_rxC3_1_heads2,  
                     axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0, axis_stream_plio_128& Acc2_txA0_4_heads0, axis_stream_plio_128& Acc2_txA0_5_heads0,  
                     axis_stream_plio_128& Acc2_txA1_0_heads0, axis_stream_plio_128& Acc2_txA1_1_heads0, axis_stream_plio_128& Acc2_txA1_2_heads0, axis_stream_plio_128& Acc2_txA1_3_heads0, axis_stream_plio_128& Acc2_txA1_4_heads0, axis_stream_plio_128& Acc2_txA1_5_heads0,  
                     axis_stream_plio_128& Acc2_txA2_0_heads0, axis_stream_plio_128& Acc2_txA2_1_heads0, axis_stream_plio_128& Acc2_txA2_2_heads0, axis_stream_plio_128& Acc2_txA2_3_heads0, axis_stream_plio_128& Acc2_txA2_4_heads0, axis_stream_plio_128& Acc2_txA2_5_heads0,  
                     axis_stream_plio_128& Acc2_txA3_0_heads0, axis_stream_plio_128& Acc2_txA3_1_heads0, axis_stream_plio_128& Acc2_txA3_2_heads0, axis_stream_plio_128& Acc2_txA3_3_heads0, axis_stream_plio_128& Acc2_txA3_4_heads0, axis_stream_plio_128& Acc2_txA3_5_heads0,  
                     axis_stream_plio_128& Acc2_txB0_0_heads0, axis_stream_plio_128& Acc2_txB1_0_heads0, axis_stream_plio_128& Acc2_txB2_0_heads0, axis_stream_plio_128& Acc2_txB3_0_heads0, axis_stream_plio_128& Acc2_txB4_0_heads0, axis_stream_plio_128& Acc2_txB5_0_heads0,  
                     axis_stream_plio_128& Acc2_txB0_1_heads0, axis_stream_plio_128& Acc2_txB1_1_heads0, axis_stream_plio_128& Acc2_txB2_1_heads0, axis_stream_plio_128& Acc2_txB3_1_heads0, axis_stream_plio_128& Acc2_txB4_1_heads0, axis_stream_plio_128& Acc2_txB5_1_heads0,  
                     axis_stream_plio_128& Acc2_txB0_2_heads0, axis_stream_plio_128& Acc2_txB1_2_heads0, axis_stream_plio_128& Acc2_txB2_2_heads0, axis_stream_plio_128& Acc2_txB3_2_heads0, axis_stream_plio_128& Acc2_txB4_2_heads0, axis_stream_plio_128& Acc2_txB5_2_heads0,  
                     axis_stream_plio_128& Acc2_txB0_3_heads0, axis_stream_plio_128& Acc2_txB1_3_heads0, axis_stream_plio_128& Acc2_txB2_3_heads0, axis_stream_plio_128& Acc2_txB3_3_heads0, axis_stream_plio_128& Acc2_txB4_3_heads0, axis_stream_plio_128& Acc2_txB5_3_heads0,  
                     axis_stream_plio_128& Acc2_txB0_4_heads0, axis_stream_plio_128& Acc2_txB1_4_heads0, axis_stream_plio_128& Acc2_txB2_4_heads0, axis_stream_plio_128& Acc2_txB3_4_heads0, axis_stream_plio_128& Acc2_txB4_4_heads0, axis_stream_plio_128& Acc2_txB5_4_heads0,  
                     axis_stream_plio_128& Acc2_txB0_5_heads0, axis_stream_plio_128& Acc2_txB1_5_heads0, axis_stream_plio_128& Acc2_txB2_5_heads0, axis_stream_plio_128& Acc2_txB3_5_heads0, axis_stream_plio_128& Acc2_txB4_5_heads0, axis_stream_plio_128& Acc2_txB5_5_heads0,  
                     axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
                     axis_stream_plio_128& Acc2_rxC1_0_heads0, axis_stream_plio_128& Acc2_rxC1_1_heads0, axis_stream_plio_128& Acc2_rxC1_2_heads0, axis_stream_plio_128& Acc2_rxC1_3_heads0, axis_stream_plio_128& Acc2_rxC1_4_heads0, axis_stream_plio_128& Acc2_rxC1_5_heads0,  
                     axis_stream_plio_128& Acc2_rxC2_0_heads0, axis_stream_plio_128& Acc2_rxC2_1_heads0, axis_stream_plio_128& Acc2_rxC2_2_heads0, axis_stream_plio_128& Acc2_rxC2_3_heads0, axis_stream_plio_128& Acc2_rxC2_4_heads0, axis_stream_plio_128& Acc2_rxC2_5_heads0,  
                     axis_stream_plio_128& Acc2_rxC3_0_heads0, axis_stream_plio_128& Acc2_rxC3_1_heads0, axis_stream_plio_128& Acc2_rxC3_2_heads0, axis_stream_plio_128& Acc2_rxC3_3_heads0, axis_stream_plio_128& Acc2_rxC3_4_heads0, axis_stream_plio_128& Acc2_rxC3_5_heads0,  
                     axis_stream_plio_128& Acc3_txA0_0, axis_stream_plio_128& Acc3_txA0_1, axis_stream_plio_128& Acc3_txA0_2, axis_stream_plio_128& Acc3_txA0_3, axis_stream_plio_128& Acc3_txA0_4, axis_stream_plio_128& Acc3_txA0_5,  
                     axis_stream_plio_128& Acc3_rxC0_0, axis_stream_plio_128& Acc3_rxC0_1, axis_stream_plio_128& Acc3_rxC0_2, axis_stream_plio_128& Acc3_rxC0_3,  
                     axis_stream_plio_128& Acc4_txA0_0, axis_stream_plio_128& Acc4_txA0_1,  
                     axis_stream_plio_128& Acc4_rxC0_0, axis_stream_plio_128& Acc4_rxC0_1, axis_stream_plio_128& Acc4_rxC0_2, axis_stream_plio_128& Acc4_rxC0_3, axis_stream_plio_128& Acc4_rxC0_4,  
                     axis_stream_A& dataA_in0, axis_stream_A& dataA_in1, axis_stream_A& dataA_in2, axis_stream_C& dataC_out, const int batch, const int block, const int iteration){
    
    
    ap_uint<PLIO_WIDTH_128> buff_image0[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL];   // 48
    #pragma HLS bind_storage variable=buff_image0 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_image0 complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff_image0 complete dim=2
    #pragma HLS ARRAY_PARTITION variable=buff_image0 cyclic factor=2 dim=4

    ap_uint<PLIO_WIDTH_128> buff_image1[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL];   // 48
    #pragma HLS bind_storage variable=buff_image1 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_image1 complete dim=1
    #pragma HLS ARRAY_PARTITION variable=buff_image1 complete dim=2
    #pragma HLS ARRAY_PARTITION variable=buff_image1 cyclic factor=2 dim=4
    
    ap_uint<PLIO_WIDTH_128> buff_pe_0[PE_OUT_PARTA*PE_OUT_PARTC*ACC0_HEADS][ACC0_M0/PE_OUT_PARTA/NUM_PER_64][ACC0_K0/PE_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff_pe_0 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_pe_0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff_pe_1[PE_OUT_PARTA*PE_OUT_PARTC*ACC0_HEADS][ACC0_M0/PE_OUT_PARTA/NUM_PER_64][ACC0_K0/PE_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff_pe_1 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_pe_1 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff0_0[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff0_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_0 complete dim=1  

    ap_uint<PLIO_WIDTH_128> buff0_1[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)]; 
    #pragma HLS bind_storage variable=buff0_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_1 complete dim=1                   

    ap_uint<PLIO_WIDTH_128> buff1_0_0[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_0_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_0_0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff1_1_0[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_1_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_1_0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff1_2_0[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_2_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_2_0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff1_0_1[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_0_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_0_1 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff1_1_1[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_1_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_1_1 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff1_2_1[BUFF0_OUT_PARTA*(BUFF0_OUT_PARTC/3)*ACC0_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];//[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_2_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_2_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff2_0[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff2_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff2_0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff2_1[BUFF1_DIM3*BUFF1_DIM2*ACC1_HEADS][BUFF1_TYPE0_DIM1][BUFF1_TYPE0_DIM0];
    #pragma HLS bind_storage variable=buff2_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff2_1 complete dim=1

    ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64];
    #pragma HLS bind_storage variable=buff_fc0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff_fc0 complete dim=1 

    ap_uint<PLIO_WIDTH_64> buff_fc1[FC_B][FC_K/FC_B/NUM_PER_64];
    #pragma HLS bind_storage variable=buff_fc1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff_fc1 complete dim=1

    bool enable_load,enable_pe,enable_s0,enable_sel,enable_s1,enable_s2,enable_s3,enable_s4,enable_s5,enable_m2v,enable_fc;
    int batch_iter = (batch-1)/stage + 1;
    int round = batch_iter * stage * block + 2;
    int round_all = round + batch + 1;
    int rd_all = 0 ;
    for (int iter = 0; iter < iteration; iter++) {
        for (int rd=0; rd<round_all; rd++){
            enable_load=(rd<batch);
            enable_pe =( rd>0 && rd<(batch+1) );
            enable_s0 =( (rd>1) && (rd<round) && (((rd-2)%stage)<batch) );
            enable_sel=( (rd>batch+1) && (rd<round) && (((rd-2)%stage)<batch) );
            enable_s1 =( (rd>2) && (rd<round+1) && (((rd-3)%stage)<batch) );
            enable_s2 =( (rd>3) && (rd<round+2) && (((rd-4)%stage)<batch) );
            enable_m2v=( rd>(round-1) && (rd<round_all-1) );
            enable_fc =( rd>(round) && (rd<round_all) );

            
            if(rd%2==0){
                load_image_top<0>(dataA_in0, dataA_in1, dataA_in2, buff_image0, enable_load);
                send_patch_top<0>(buff_image1, Acc3_txA0_0,Acc3_txA0_1,Acc3_txA0_2,Acc3_txA0_3,Acc3_txA0_4,Acc3_txA0_5,enable_pe);
                receive_Patch_top<0>(Acc3_rxC0_0,Acc3_rxC0_1,Acc3_rxC0_2,Acc3_rxC0_3,buff_pe_0, enable_pe);
                Acc0_send_LHS_type0_top<0>(buff_pe_1, buff0_0,Acc0_txA0_0_heads0, Acc0_txA0_1_heads0, Acc0_txA0_2_heads0, Acc0_txA0_3_heads0,  enable_sel, enable_s0);
                Acc0_receive_OUT_type0_top<0>(buff1_0_1,Acc0_rxC0_0_heads0, Acc0_rxC0_1_heads0, Acc0_rxC0_2_heads0, Acc0_rxC0_3_heads0, Acc0_rxC0_4_heads0, Acc0_rxC0_5_heads0, enable_s0);
                Acc0_receive_OUT_type0_top<1>(buff1_1_1,Acc0_rxC0_6_heads0, Acc0_rxC0_7_heads0, Acc0_rxC0_8_heads0, Acc0_rxC0_9_heads0, Acc0_rxC0_10_heads0, Acc0_rxC0_11_heads0, enable_s0);
                Acc0_receive_OUT_type0_top<2>(buff1_2_1,Acc0_rxC0_12_heads0, Acc0_rxC0_13_heads0, Acc0_rxC0_14_heads0, Acc0_rxC0_15_heads0, Acc0_rxC0_16_heads0, Acc0_rxC0_17_heads0, enable_s0);
                Acc1_top<0>(buff1_0_0,buff1_1_0,buff1_2_0,buff2_0,
                Acc1_txA0_0_heads0, Acc1_txA0_1_heads0, Acc1_txA1_0_heads0, Acc1_txA1_1_heads0, Acc1_txA2_0_heads0, Acc1_txA2_1_heads0,
                Acc1_txA3_0_heads0, Acc1_txA3_1_heads0, Acc1_txA0_0_heads1, Acc1_txA0_1_heads1, Acc1_txA1_0_heads1, Acc1_txA1_1_heads1,
                Acc1_txA2_0_heads1, Acc1_txA2_1_heads1, Acc1_txA3_0_heads1, Acc1_txA3_1_heads1, Acc1_txA0_0_heads2, Acc1_txA0_1_heads2,
                Acc1_txA1_0_heads2, Acc1_txA1_1_heads2, Acc1_txA2_0_heads2, Acc1_txA2_1_heads2, Acc1_txA3_0_heads2, Acc1_txA3_1_heads2,
                Acc1_txB0_0_heads0, Acc1_txB0_1_heads0, Acc1_txB1_0_heads0, Acc1_txB1_1_heads0, Acc1_txB0_0_heads1, Acc1_txB0_1_heads1,
                Acc1_txB1_0_heads1, Acc1_txB1_1_heads1, Acc1_txB0_0_heads2, Acc1_txB0_1_heads2, Acc1_txB1_0_heads2, Acc1_txB1_1_heads2,
                Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0, Acc1_rxC1_0_heads0, Acc1_rxC1_1_heads0, Acc1_rxC2_0_heads0, Acc1_rxC2_1_heads0,
                Acc1_rxC3_0_heads0, Acc1_rxC3_1_heads0, Acc1_rxC0_0_heads1, Acc1_rxC0_1_heads1, Acc1_rxC1_0_heads1, Acc1_rxC1_1_heads1,
                Acc1_rxC2_0_heads1, Acc1_rxC2_1_heads1, Acc1_rxC3_0_heads1, Acc1_rxC3_1_heads1, Acc1_rxC0_0_heads2, Acc1_rxC0_1_heads2,
                Acc1_rxC1_0_heads2, Acc1_rxC1_1_heads2, Acc1_rxC2_0_heads2, Acc1_rxC2_1_heads2, Acc1_rxC3_0_heads2, Acc1_rxC3_1_heads2, 
                enable_s1);
                Acc2_m2vector<0>(buff2_1, buff0_1, buff_fc0, 
                Acc2_txA0_0_heads0,Acc2_txA0_1_heads0,Acc2_txA0_2_heads0,Acc2_txA0_3_heads0,Acc2_txA0_4_heads0,Acc2_txA0_5_heads0,  
                Acc2_txA1_0_heads0,Acc2_txA1_1_heads0,Acc2_txA1_2_heads0,Acc2_txA1_3_heads0,Acc2_txA1_4_heads0,Acc2_txA1_5_heads0,  
                Acc2_txA2_0_heads0,Acc2_txA2_1_heads0,Acc2_txA2_2_heads0,Acc2_txA2_3_heads0,Acc2_txA2_4_heads0,Acc2_txA2_5_heads0,  
                Acc2_txA3_0_heads0,Acc2_txA3_1_heads0,Acc2_txA3_2_heads0,Acc2_txA3_3_heads0,Acc2_txA3_4_heads0,Acc2_txA3_5_heads0,  
                Acc2_txB0_0_heads0,Acc2_txB0_1_heads0,Acc2_txB0_2_heads0,Acc2_txB0_3_heads0,Acc2_txB0_4_heads0,Acc2_txB0_5_heads0,  
                Acc2_txB1_0_heads0,Acc2_txB1_1_heads0,Acc2_txB1_2_heads0,Acc2_txB1_3_heads0,Acc2_txB1_4_heads0,Acc2_txB1_5_heads0,  
                Acc2_txB2_0_heads0,Acc2_txB2_1_heads0,Acc2_txB2_2_heads0,Acc2_txB2_3_heads0,Acc2_txB2_4_heads0,Acc2_txB2_5_heads0,  
                Acc2_txB3_0_heads0,Acc2_txB3_1_heads0,Acc2_txB3_2_heads0,Acc2_txB3_3_heads0,Acc2_txB3_4_heads0,Acc2_txB3_5_heads0,  
                Acc2_txB4_0_heads0,Acc2_txB4_1_heads0,Acc2_txB4_2_heads0,Acc2_txB4_3_heads0,Acc2_txB4_4_heads0,Acc2_txB4_5_heads0,  
                Acc2_txB5_0_heads0,Acc2_txB5_1_heads0,Acc2_txB5_2_heads0,Acc2_txB5_3_heads0,Acc2_txB5_4_heads0,Acc2_txB5_5_heads0,
                Acc2_rxC0_0_heads0,Acc2_rxC0_1_heads0,Acc2_rxC0_2_heads0,Acc2_rxC0_3_heads0,Acc2_rxC0_4_heads0,Acc2_rxC0_5_heads0,  
                Acc2_rxC1_0_heads0,Acc2_rxC1_1_heads0,Acc2_rxC1_2_heads0,Acc2_rxC1_3_heads0,Acc2_rxC1_4_heads0,Acc2_rxC1_5_heads0,  
                Acc2_rxC2_0_heads0,Acc2_rxC2_1_heads0,Acc2_rxC2_2_heads0,Acc2_rxC2_3_heads0,Acc2_rxC2_4_heads0,Acc2_rxC2_5_heads0,  
                Acc2_rxC3_0_heads0,Acc2_rxC3_1_heads0,Acc2_rxC3_2_heads0,Acc2_rxC3_3_heads0,Acc2_rxC3_4_heads0,Acc2_rxC3_5_heads0,  
                enable_s2,enable_m2v);
                send_FC<0>(buff_fc1, Acc4_txA0_0,Acc4_txA0_1,enable_fc);
                receive_FC<0>(dataC_out, Acc4_rxC0_0,Acc4_rxC0_1,Acc4_rxC0_2,Acc4_rxC0_3,Acc4_rxC0_4,enable_fc);
            }
            else{
                load_image_top<0>(dataA_in0, dataA_in1, dataA_in2, buff_image1, enable_load);
                send_patch_top<0>(buff_image0, Acc3_txA0_0,Acc3_txA0_1,Acc3_txA0_2,Acc3_txA0_3,Acc3_txA0_4,Acc3_txA0_5,enable_pe);
                receive_Patch_top<0>(Acc3_rxC0_0,Acc3_rxC0_1,Acc3_rxC0_2,Acc3_rxC0_3,buff_pe_1, enable_pe);
                Acc0_send_LHS_type0_top<0>(buff_pe_0, buff0_1,Acc0_txA0_0_heads0, Acc0_txA0_1_heads0, Acc0_txA0_2_heads0, Acc0_txA0_3_heads0,  enable_sel, enable_s0);
                Acc0_receive_OUT_type0_top<0>(buff1_0_0,Acc0_rxC0_0_heads0, Acc0_rxC0_1_heads0, Acc0_rxC0_2_heads0, Acc0_rxC0_3_heads0, Acc0_rxC0_4_heads0, Acc0_rxC0_5_heads0, enable_s0);
                Acc0_receive_OUT_type0_top<1>(buff1_1_0,Acc0_rxC0_6_heads0, Acc0_rxC0_7_heads0, Acc0_rxC0_8_heads0, Acc0_rxC0_9_heads0, Acc0_rxC0_10_heads0, Acc0_rxC0_11_heads0, enable_s0);
                Acc0_receive_OUT_type0_top<2>(buff1_2_0,Acc0_rxC0_12_heads0, Acc0_rxC0_13_heads0, Acc0_rxC0_14_heads0, Acc0_rxC0_15_heads0, Acc0_rxC0_16_heads0, Acc0_rxC0_17_heads0, enable_s0);
                Acc1_top<0>(buff1_0_1,buff1_1_1,buff1_2_1,buff2_1,
                Acc1_txA0_0_heads0, Acc1_txA0_1_heads0, Acc1_txA1_0_heads0, Acc1_txA1_1_heads0, Acc1_txA2_0_heads0, Acc1_txA2_1_heads0,
                Acc1_txA3_0_heads0, Acc1_txA3_1_heads0, Acc1_txA0_0_heads1, Acc1_txA0_1_heads1, Acc1_txA1_0_heads1, Acc1_txA1_1_heads1,
                Acc1_txA2_0_heads1, Acc1_txA2_1_heads1, Acc1_txA3_0_heads1, Acc1_txA3_1_heads1, Acc1_txA0_0_heads2, Acc1_txA0_1_heads2,
                Acc1_txA1_0_heads2, Acc1_txA1_1_heads2, Acc1_txA2_0_heads2, Acc1_txA2_1_heads2, Acc1_txA3_0_heads2, Acc1_txA3_1_heads2,
                Acc1_txB0_0_heads0, Acc1_txB0_1_heads0, Acc1_txB1_0_heads0, Acc1_txB1_1_heads0, Acc1_txB0_0_heads1, Acc1_txB0_1_heads1,
                Acc1_txB1_0_heads1, Acc1_txB1_1_heads1, Acc1_txB0_0_heads2, Acc1_txB0_1_heads2, Acc1_txB1_0_heads2, Acc1_txB1_1_heads2,
                Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0, Acc1_rxC1_0_heads0, Acc1_rxC1_1_heads0, Acc1_rxC2_0_heads0, Acc1_rxC2_1_heads0,
                Acc1_rxC3_0_heads0, Acc1_rxC3_1_heads0, Acc1_rxC0_0_heads1, Acc1_rxC0_1_heads1, Acc1_rxC1_0_heads1, Acc1_rxC1_1_heads1,
                Acc1_rxC2_0_heads1, Acc1_rxC2_1_heads1, Acc1_rxC3_0_heads1, Acc1_rxC3_1_heads1, Acc1_rxC0_0_heads2, Acc1_rxC0_1_heads2,
                Acc1_rxC1_0_heads2, Acc1_rxC1_1_heads2, Acc1_rxC2_0_heads2, Acc1_rxC2_1_heads2, Acc1_rxC3_0_heads2, Acc1_rxC3_1_heads2, 
                enable_s1);
                Acc2_m2vector<0>(buff2_0, buff0_0, buff_fc1, 
                Acc2_txA0_0_heads0,Acc2_txA0_1_heads0,Acc2_txA0_2_heads0,Acc2_txA0_3_heads0,Acc2_txA0_4_heads0,Acc2_txA0_5_heads0,  
                Acc2_txA1_0_heads0,Acc2_txA1_1_heads0,Acc2_txA1_2_heads0,Acc2_txA1_3_heads0,Acc2_txA1_4_heads0,Acc2_txA1_5_heads0,  
                Acc2_txA2_0_heads0,Acc2_txA2_1_heads0,Acc2_txA2_2_heads0,Acc2_txA2_3_heads0,Acc2_txA2_4_heads0,Acc2_txA2_5_heads0,  
                Acc2_txA3_0_heads0,Acc2_txA3_1_heads0,Acc2_txA3_2_heads0,Acc2_txA3_3_heads0,Acc2_txA3_4_heads0,Acc2_txA3_5_heads0,  
                Acc2_txB0_0_heads0,Acc2_txB0_1_heads0,Acc2_txB0_2_heads0,Acc2_txB0_3_heads0,Acc2_txB0_4_heads0,Acc2_txB0_5_heads0,  
                Acc2_txB1_0_heads0,Acc2_txB1_1_heads0,Acc2_txB1_2_heads0,Acc2_txB1_3_heads0,Acc2_txB1_4_heads0,Acc2_txB1_5_heads0,  
                Acc2_txB2_0_heads0,Acc2_txB2_1_heads0,Acc2_txB2_2_heads0,Acc2_txB2_3_heads0,Acc2_txB2_4_heads0,Acc2_txB2_5_heads0,  
                Acc2_txB3_0_heads0,Acc2_txB3_1_heads0,Acc2_txB3_2_heads0,Acc2_txB3_3_heads0,Acc2_txB3_4_heads0,Acc2_txB3_5_heads0,  
                Acc2_txB4_0_heads0,Acc2_txB4_1_heads0,Acc2_txB4_2_heads0,Acc2_txB4_3_heads0,Acc2_txB4_4_heads0,Acc2_txB4_5_heads0,  
                Acc2_txB5_0_heads0,Acc2_txB5_1_heads0,Acc2_txB5_2_heads0,Acc2_txB5_3_heads0,Acc2_txB5_4_heads0,Acc2_txB5_5_heads0,
                Acc2_rxC0_0_heads0,Acc2_rxC0_1_heads0,Acc2_rxC0_2_heads0,Acc2_rxC0_3_heads0,Acc2_rxC0_4_heads0,Acc2_rxC0_5_heads0,  
                Acc2_rxC1_0_heads0,Acc2_rxC1_1_heads0,Acc2_rxC1_2_heads0,Acc2_rxC1_3_heads0,Acc2_rxC1_4_heads0,Acc2_rxC1_5_heads0,  
                Acc2_rxC2_0_heads0,Acc2_rxC2_1_heads0,Acc2_rxC2_2_heads0,Acc2_rxC2_3_heads0,Acc2_rxC2_4_heads0,Acc2_rxC2_5_heads0,  
                Acc2_rxC3_0_heads0,Acc2_rxC3_1_heads0,Acc2_rxC3_2_heads0,Acc2_rxC3_3_heads0,Acc2_rxC3_4_heads0,Acc2_rxC3_5_heads0,  
                enable_s2,enable_m2v);
                send_FC<0>(buff_fc0, Acc4_txA0_0,Acc4_txA0_1,enable_fc);
                receive_FC<0>(dataC_out, Acc4_rxC0_0,Acc4_rxC0_1,Acc4_rxC0_2,Acc4_rxC0_3,Acc4_rxC0_4,enable_fc);
            }
        }
    }
}

void pl_engine( ap_uint<AXI_WIDTH_A>* ina0, ap_uint<AXI_WIDTH_A>* ina1, ap_uint<AXI_WIDTH_A>* ina2, ap_uint<AXI_WIDTH_C>* out0, 
               axis_stream_plio_128& Acc0_txA0_0_heads0, axis_stream_plio_128& Acc0_txA0_1_heads0, axis_stream_plio_128& Acc0_txA0_2_heads0, axis_stream_plio_128& Acc0_txA0_3_heads0,  
               axis_stream_plio_128& Acc0_rxC0_0_heads0, axis_stream_plio_128& Acc0_rxC0_1_heads0, axis_stream_plio_128& Acc0_rxC0_2_heads0, axis_stream_plio_128& Acc0_rxC0_3_heads0, axis_stream_plio_128& Acc0_rxC0_4_heads0, axis_stream_plio_128& Acc0_rxC0_5_heads0, axis_stream_plio_128& Acc0_rxC0_6_heads0, axis_stream_plio_128& Acc0_rxC0_7_heads0, axis_stream_plio_128& Acc0_rxC0_8_heads0, axis_stream_plio_128& Acc0_rxC0_9_heads0, axis_stream_plio_128& Acc0_rxC0_10_heads0, axis_stream_plio_128& Acc0_rxC0_11_heads0, axis_stream_plio_128& Acc0_rxC0_12_heads0, axis_stream_plio_128& Acc0_rxC0_13_heads0, axis_stream_plio_128& Acc0_rxC0_14_heads0, axis_stream_plio_128& Acc0_rxC0_15_heads0, axis_stream_plio_128& Acc0_rxC0_16_heads0, axis_stream_plio_128& Acc0_rxC0_17_heads0,  
               axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0,  
               axis_stream_plio_128& Acc1_txA1_0_heads0, axis_stream_plio_128& Acc1_txA1_1_heads0,  
               axis_stream_plio_128& Acc1_txA2_0_heads0, axis_stream_plio_128& Acc1_txA2_1_heads0,  
               axis_stream_plio_128& Acc1_txA3_0_heads0, axis_stream_plio_128& Acc1_txA3_1_heads0,  
               axis_stream_plio_128& Acc1_txA0_0_heads1, axis_stream_plio_128& Acc1_txA0_1_heads1,  
               axis_stream_plio_128& Acc1_txA1_0_heads1, axis_stream_plio_128& Acc1_txA1_1_heads1,  
               axis_stream_plio_128& Acc1_txA2_0_heads1, axis_stream_plio_128& Acc1_txA2_1_heads1,  
               axis_stream_plio_128& Acc1_txA3_0_heads1, axis_stream_plio_128& Acc1_txA3_1_heads1,  
               axis_stream_plio_128& Acc1_txA0_0_heads2, axis_stream_plio_128& Acc1_txA0_1_heads2,  
               axis_stream_plio_128& Acc1_txA1_0_heads2, axis_stream_plio_128& Acc1_txA1_1_heads2,  
               axis_stream_plio_128& Acc1_txA2_0_heads2, axis_stream_plio_128& Acc1_txA2_1_heads2,  
               axis_stream_plio_128& Acc1_txA3_0_heads2, axis_stream_plio_128& Acc1_txA3_1_heads2,  
               axis_stream_plio_128& Acc1_txB0_0_heads0, axis_stream_plio_128& Acc1_txB1_0_heads0,  
               axis_stream_plio_128& Acc1_txB0_1_heads0, axis_stream_plio_128& Acc1_txB1_1_heads0,  
               axis_stream_plio_128& Acc1_txB0_0_heads1, axis_stream_plio_128& Acc1_txB1_0_heads1,  
               axis_stream_plio_128& Acc1_txB0_1_heads1, axis_stream_plio_128& Acc1_txB1_1_heads1,  
               axis_stream_plio_128& Acc1_txB0_0_heads2, axis_stream_plio_128& Acc1_txB1_0_heads2,  
               axis_stream_plio_128& Acc1_txB0_1_heads2, axis_stream_plio_128& Acc1_txB1_1_heads2,  
               axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0,  
               axis_stream_plio_128& Acc1_rxC1_0_heads0, axis_stream_plio_128& Acc1_rxC1_1_heads0,  
               axis_stream_plio_128& Acc1_rxC2_0_heads0, axis_stream_plio_128& Acc1_rxC2_1_heads0,  
               axis_stream_plio_128& Acc1_rxC3_0_heads0, axis_stream_plio_128& Acc1_rxC3_1_heads0,  
               axis_stream_plio_128& Acc1_rxC0_0_heads1, axis_stream_plio_128& Acc1_rxC0_1_heads1,  
               axis_stream_plio_128& Acc1_rxC1_0_heads1, axis_stream_plio_128& Acc1_rxC1_1_heads1,  
               axis_stream_plio_128& Acc1_rxC2_0_heads1, axis_stream_plio_128& Acc1_rxC2_1_heads1,  
               axis_stream_plio_128& Acc1_rxC3_0_heads1, axis_stream_plio_128& Acc1_rxC3_1_heads1,  
               axis_stream_plio_128& Acc1_rxC0_0_heads2, axis_stream_plio_128& Acc1_rxC0_1_heads2,  
               axis_stream_plio_128& Acc1_rxC1_0_heads2, axis_stream_plio_128& Acc1_rxC1_1_heads2,  
               axis_stream_plio_128& Acc1_rxC2_0_heads2, axis_stream_plio_128& Acc1_rxC2_1_heads2,  
               axis_stream_plio_128& Acc1_rxC3_0_heads2, axis_stream_plio_128& Acc1_rxC3_1_heads2,  
               axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0, axis_stream_plio_128& Acc2_txA0_4_heads0, axis_stream_plio_128& Acc2_txA0_5_heads0,  
               axis_stream_plio_128& Acc2_txA1_0_heads0, axis_stream_plio_128& Acc2_txA1_1_heads0, axis_stream_plio_128& Acc2_txA1_2_heads0, axis_stream_plio_128& Acc2_txA1_3_heads0, axis_stream_plio_128& Acc2_txA1_4_heads0, axis_stream_plio_128& Acc2_txA1_5_heads0,  
               axis_stream_plio_128& Acc2_txA2_0_heads0, axis_stream_plio_128& Acc2_txA2_1_heads0, axis_stream_plio_128& Acc2_txA2_2_heads0, axis_stream_plio_128& Acc2_txA2_3_heads0, axis_stream_plio_128& Acc2_txA2_4_heads0, axis_stream_plio_128& Acc2_txA2_5_heads0,  
               axis_stream_plio_128& Acc2_txA3_0_heads0, axis_stream_plio_128& Acc2_txA3_1_heads0, axis_stream_plio_128& Acc2_txA3_2_heads0, axis_stream_plio_128& Acc2_txA3_3_heads0, axis_stream_plio_128& Acc2_txA3_4_heads0, axis_stream_plio_128& Acc2_txA3_5_heads0,  
               axis_stream_plio_128& Acc2_txB0_0_heads0, axis_stream_plio_128& Acc2_txB1_0_heads0, axis_stream_plio_128& Acc2_txB2_0_heads0, axis_stream_plio_128& Acc2_txB3_0_heads0, axis_stream_plio_128& Acc2_txB4_0_heads0, axis_stream_plio_128& Acc2_txB5_0_heads0,  
               axis_stream_plio_128& Acc2_txB0_1_heads0, axis_stream_plio_128& Acc2_txB1_1_heads0, axis_stream_plio_128& Acc2_txB2_1_heads0, axis_stream_plio_128& Acc2_txB3_1_heads0, axis_stream_plio_128& Acc2_txB4_1_heads0, axis_stream_plio_128& Acc2_txB5_1_heads0,  
               axis_stream_plio_128& Acc2_txB0_2_heads0, axis_stream_plio_128& Acc2_txB1_2_heads0, axis_stream_plio_128& Acc2_txB2_2_heads0, axis_stream_plio_128& Acc2_txB3_2_heads0, axis_stream_plio_128& Acc2_txB4_2_heads0, axis_stream_plio_128& Acc2_txB5_2_heads0,  
               axis_stream_plio_128& Acc2_txB0_3_heads0, axis_stream_plio_128& Acc2_txB1_3_heads0, axis_stream_plio_128& Acc2_txB2_3_heads0, axis_stream_plio_128& Acc2_txB3_3_heads0, axis_stream_plio_128& Acc2_txB4_3_heads0, axis_stream_plio_128& Acc2_txB5_3_heads0,  
               axis_stream_plio_128& Acc2_txB0_4_heads0, axis_stream_plio_128& Acc2_txB1_4_heads0, axis_stream_plio_128& Acc2_txB2_4_heads0, axis_stream_plio_128& Acc2_txB3_4_heads0, axis_stream_plio_128& Acc2_txB4_4_heads0, axis_stream_plio_128& Acc2_txB5_4_heads0,  
               axis_stream_plio_128& Acc2_txB0_5_heads0, axis_stream_plio_128& Acc2_txB1_5_heads0, axis_stream_plio_128& Acc2_txB2_5_heads0, axis_stream_plio_128& Acc2_txB3_5_heads0, axis_stream_plio_128& Acc2_txB4_5_heads0, axis_stream_plio_128& Acc2_txB5_5_heads0,  
               axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
               axis_stream_plio_128& Acc2_rxC1_0_heads0, axis_stream_plio_128& Acc2_rxC1_1_heads0, axis_stream_plio_128& Acc2_rxC1_2_heads0, axis_stream_plio_128& Acc2_rxC1_3_heads0, axis_stream_plio_128& Acc2_rxC1_4_heads0, axis_stream_plio_128& Acc2_rxC1_5_heads0,  
               axis_stream_plio_128& Acc2_rxC2_0_heads0, axis_stream_plio_128& Acc2_rxC2_1_heads0, axis_stream_plio_128& Acc2_rxC2_2_heads0, axis_stream_plio_128& Acc2_rxC2_3_heads0, axis_stream_plio_128& Acc2_rxC2_4_heads0, axis_stream_plio_128& Acc2_rxC2_5_heads0,  
               axis_stream_plio_128& Acc2_rxC3_0_heads0, axis_stream_plio_128& Acc2_rxC3_1_heads0, axis_stream_plio_128& Acc2_rxC3_2_heads0, axis_stream_plio_128& Acc2_rxC3_3_heads0, axis_stream_plio_128& Acc2_rxC3_4_heads0, axis_stream_plio_128& Acc2_rxC3_5_heads0,  
               axis_stream_plio_128& Acc3_txA0_0, axis_stream_plio_128& Acc3_txA0_1, axis_stream_plio_128& Acc3_txA0_2, axis_stream_plio_128& Acc3_txA0_3, axis_stream_plio_128& Acc3_txA0_4, axis_stream_plio_128& Acc3_txA0_5,  
               axis_stream_plio_128& Acc3_rxC0_0, axis_stream_plio_128& Acc3_rxC0_1, axis_stream_plio_128& Acc3_rxC0_2, axis_stream_plio_128& Acc3_rxC0_3,  
               axis_stream_plio_128& Acc4_txA0_0, axis_stream_plio_128& Acc4_txA0_1,  
               axis_stream_plio_128& Acc4_rxC0_0, axis_stream_plio_128& Acc4_rxC0_1, axis_stream_plio_128& Acc4_rxC0_2, axis_stream_plio_128& Acc4_rxC0_3, axis_stream_plio_128& Acc4_rxC0_4,  
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
    
    #pragma HLS interface axis port=Acc0_txA0_0_heads0
    #pragma HLS interface axis port=Acc0_txA0_1_heads0
    #pragma HLS interface axis port=Acc0_txA0_2_heads0
    #pragma HLS interface axis port=Acc0_txA0_3_heads0
    #pragma HLS interface axis port=Acc0_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_3_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_4_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_5_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_6_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_7_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_8_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_9_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_10_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_11_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_12_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_13_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_14_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_15_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_16_heads0  
    #pragma HLS interface axis port=Acc0_rxC0_17_heads0  
    #pragma HLS interface axis port=Acc1_txA0_0_heads0
    #pragma HLS interface axis port=Acc1_txA0_1_heads0
    #pragma HLS interface axis port=Acc1_txA1_0_heads0
    #pragma HLS interface axis port=Acc1_txA1_1_heads0
    #pragma HLS interface axis port=Acc1_txA2_0_heads0
    #pragma HLS interface axis port=Acc1_txA2_1_heads0
    #pragma HLS interface axis port=Acc1_txA3_0_heads0
    #pragma HLS interface axis port=Acc1_txA3_1_heads0
    #pragma HLS interface axis port=Acc1_txA0_0_heads1
    #pragma HLS interface axis port=Acc1_txA0_1_heads1
    #pragma HLS interface axis port=Acc1_txA1_0_heads1
    #pragma HLS interface axis port=Acc1_txA1_1_heads1
    #pragma HLS interface axis port=Acc1_txA2_0_heads1
    #pragma HLS interface axis port=Acc1_txA2_1_heads1
    #pragma HLS interface axis port=Acc1_txA3_0_heads1
    #pragma HLS interface axis port=Acc1_txA3_1_heads1
    #pragma HLS interface axis port=Acc1_txA0_0_heads2
    #pragma HLS interface axis port=Acc1_txA0_1_heads2
    #pragma HLS interface axis port=Acc1_txA1_0_heads2
    #pragma HLS interface axis port=Acc1_txA1_1_heads2
    #pragma HLS interface axis port=Acc1_txA2_0_heads2
    #pragma HLS interface axis port=Acc1_txA2_1_heads2
    #pragma HLS interface axis port=Acc1_txA3_0_heads2
    #pragma HLS interface axis port=Acc1_txA3_1_heads2
    #pragma HLS interface axis port=Acc1_txB0_0_heads0 
    #pragma HLS interface axis port=Acc1_txB1_0_heads0 
    #pragma HLS interface axis port=Acc1_txB0_1_heads0 
    #pragma HLS interface axis port=Acc1_txB1_1_heads0 
    #pragma HLS interface axis port=Acc1_txB0_0_heads1 
    #pragma HLS interface axis port=Acc1_txB1_0_heads1 
    #pragma HLS interface axis port=Acc1_txB0_1_heads1 
    #pragma HLS interface axis port=Acc1_txB1_1_heads1 
    #pragma HLS interface axis port=Acc1_txB0_0_heads2 
    #pragma HLS interface axis port=Acc1_txB1_0_heads2 
    #pragma HLS interface axis port=Acc1_txB0_1_heads2 
    #pragma HLS interface axis port=Acc1_txB1_1_heads2 
    #pragma HLS interface axis port=Acc1_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc1_rxC1_0_heads0  
    #pragma HLS interface axis port=Acc1_rxC1_1_heads0  
    #pragma HLS interface axis port=Acc1_rxC2_0_heads0  
    #pragma HLS interface axis port=Acc1_rxC2_1_heads0  
    #pragma HLS interface axis port=Acc1_rxC3_0_heads0  
    #pragma HLS interface axis port=Acc1_rxC3_1_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_0_heads1  
    #pragma HLS interface axis port=Acc1_rxC0_1_heads1  
    #pragma HLS interface axis port=Acc1_rxC1_0_heads1  
    #pragma HLS interface axis port=Acc1_rxC1_1_heads1  
    #pragma HLS interface axis port=Acc1_rxC2_0_heads1  
    #pragma HLS interface axis port=Acc1_rxC2_1_heads1  
    #pragma HLS interface axis port=Acc1_rxC3_0_heads1  
    #pragma HLS interface axis port=Acc1_rxC3_1_heads1  
    #pragma HLS interface axis port=Acc1_rxC0_0_heads2  
    #pragma HLS interface axis port=Acc1_rxC0_1_heads2  
    #pragma HLS interface axis port=Acc1_rxC1_0_heads2  
    #pragma HLS interface axis port=Acc1_rxC1_1_heads2  
    #pragma HLS interface axis port=Acc1_rxC2_0_heads2  
    #pragma HLS interface axis port=Acc1_rxC2_1_heads2  
    #pragma HLS interface axis port=Acc1_rxC3_0_heads2  
    #pragma HLS interface axis port=Acc1_rxC3_1_heads2  
    #pragma HLS interface axis port=Acc2_txA0_0_heads0
    #pragma HLS interface axis port=Acc2_txA0_1_heads0
    #pragma HLS interface axis port=Acc2_txA0_2_heads0
    #pragma HLS interface axis port=Acc2_txA0_3_heads0
    #pragma HLS interface axis port=Acc2_txA0_4_heads0
    #pragma HLS interface axis port=Acc2_txA0_5_heads0
    #pragma HLS interface axis port=Acc2_txA1_0_heads0
    #pragma HLS interface axis port=Acc2_txA1_1_heads0
    #pragma HLS interface axis port=Acc2_txA1_2_heads0
    #pragma HLS interface axis port=Acc2_txA1_3_heads0
    #pragma HLS interface axis port=Acc2_txA1_4_heads0
    #pragma HLS interface axis port=Acc2_txA1_5_heads0
    #pragma HLS interface axis port=Acc2_txA2_0_heads0
    #pragma HLS interface axis port=Acc2_txA2_1_heads0
    #pragma HLS interface axis port=Acc2_txA2_2_heads0
    #pragma HLS interface axis port=Acc2_txA2_3_heads0
    #pragma HLS interface axis port=Acc2_txA2_4_heads0
    #pragma HLS interface axis port=Acc2_txA2_5_heads0
    #pragma HLS interface axis port=Acc2_txA3_0_heads0
    #pragma HLS interface axis port=Acc2_txA3_1_heads0
    #pragma HLS interface axis port=Acc2_txA3_2_heads0
    #pragma HLS interface axis port=Acc2_txA3_3_heads0
    #pragma HLS interface axis port=Acc2_txA3_4_heads0
    #pragma HLS interface axis port=Acc2_txA3_5_heads0
    #pragma HLS interface axis port=Acc2_txB0_0_heads0 
    #pragma HLS interface axis port=Acc2_txB1_0_heads0 
    #pragma HLS interface axis port=Acc2_txB2_0_heads0 
    #pragma HLS interface axis port=Acc2_txB3_0_heads0 
    #pragma HLS interface axis port=Acc2_txB4_0_heads0 
    #pragma HLS interface axis port=Acc2_txB5_0_heads0 
    #pragma HLS interface axis port=Acc2_txB0_1_heads0 
    #pragma HLS interface axis port=Acc2_txB1_1_heads0 
    #pragma HLS interface axis port=Acc2_txB2_1_heads0 
    #pragma HLS interface axis port=Acc2_txB3_1_heads0 
    #pragma HLS interface axis port=Acc2_txB4_1_heads0 
    #pragma HLS interface axis port=Acc2_txB5_1_heads0 
    #pragma HLS interface axis port=Acc2_txB0_2_heads0 
    #pragma HLS interface axis port=Acc2_txB1_2_heads0 
    #pragma HLS interface axis port=Acc2_txB2_2_heads0 
    #pragma HLS interface axis port=Acc2_txB3_2_heads0 
    #pragma HLS interface axis port=Acc2_txB4_2_heads0 
    #pragma HLS interface axis port=Acc2_txB5_2_heads0 
    #pragma HLS interface axis port=Acc2_txB0_3_heads0 
    #pragma HLS interface axis port=Acc2_txB1_3_heads0 
    #pragma HLS interface axis port=Acc2_txB2_3_heads0 
    #pragma HLS interface axis port=Acc2_txB3_3_heads0 
    #pragma HLS interface axis port=Acc2_txB4_3_heads0 
    #pragma HLS interface axis port=Acc2_txB5_3_heads0 
    #pragma HLS interface axis port=Acc2_txB0_4_heads0 
    #pragma HLS interface axis port=Acc2_txB1_4_heads0 
    #pragma HLS interface axis port=Acc2_txB2_4_heads0 
    #pragma HLS interface axis port=Acc2_txB3_4_heads0 
    #pragma HLS interface axis port=Acc2_txB4_4_heads0 
    #pragma HLS interface axis port=Acc2_txB5_4_heads0 
    #pragma HLS interface axis port=Acc2_txB0_5_heads0 
    #pragma HLS interface axis port=Acc2_txB1_5_heads0 
    #pragma HLS interface axis port=Acc2_txB2_5_heads0 
    #pragma HLS interface axis port=Acc2_txB3_5_heads0 
    #pragma HLS interface axis port=Acc2_txB4_5_heads0 
    #pragma HLS interface axis port=Acc2_txB5_5_heads0 
    #pragma HLS interface axis port=Acc2_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_3_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_4_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_5_heads0  
    #pragma HLS interface axis port=Acc2_rxC1_0_heads0  
    #pragma HLS interface axis port=Acc2_rxC1_1_heads0  
    #pragma HLS interface axis port=Acc2_rxC1_2_heads0  
    #pragma HLS interface axis port=Acc2_rxC1_3_heads0  
    #pragma HLS interface axis port=Acc2_rxC1_4_heads0  
    #pragma HLS interface axis port=Acc2_rxC1_5_heads0  
    #pragma HLS interface axis port=Acc2_rxC2_0_heads0  
    #pragma HLS interface axis port=Acc2_rxC2_1_heads0  
    #pragma HLS interface axis port=Acc2_rxC2_2_heads0  
    #pragma HLS interface axis port=Acc2_rxC2_3_heads0  
    #pragma HLS interface axis port=Acc2_rxC2_4_heads0  
    #pragma HLS interface axis port=Acc2_rxC2_5_heads0  
    #pragma HLS interface axis port=Acc2_rxC3_0_heads0  
    #pragma HLS interface axis port=Acc2_rxC3_1_heads0  
    #pragma HLS interface axis port=Acc2_rxC3_2_heads0  
    #pragma HLS interface axis port=Acc2_rxC3_3_heads0  
    #pragma HLS interface axis port=Acc2_rxC3_4_heads0  
    #pragma HLS interface axis port=Acc2_rxC3_5_heads0  
    
                
    #pragma HLS interface axis port=Acc3_txA0_0
    #pragma HLS interface axis port=Acc3_txA0_1
    #pragma HLS interface axis port=Acc3_txA0_2
    #pragma HLS interface axis port=Acc3_txA0_3
    #pragma HLS interface axis port=Acc3_txA0_4
    #pragma HLS interface axis port=Acc3_txA0_5
     
    #pragma HLS interface axis port=Acc3_rxC0_0
    #pragma HLS interface axis port=Acc3_rxC0_1
    #pragma HLS interface axis port=Acc3_rxC0_2
    #pragma HLS interface axis port=Acc3_rxC0_3
     
    #pragma HLS interface axis port=Acc4_txA0_0
    #pragma HLS interface axis port=Acc4_txA0_1
     
    #pragma HLS interface axis port=Acc4_rxC0_0
    #pragma HLS interface axis port=Acc4_rxC0_1
    #pragma HLS interface axis port=Acc4_rxC0_2
    #pragma HLS interface axis port=Acc4_rxC0_3
    #pragma HLS interface axis port=Acc4_rxC0_4
     
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


    forward_control(Acc0_txA0_0_heads0, Acc0_txA0_1_heads0, Acc0_txA0_2_heads0, Acc0_txA0_3_heads0,  
                    Acc0_rxC0_0_heads0, Acc0_rxC0_1_heads0, Acc0_rxC0_2_heads0, Acc0_rxC0_3_heads0, Acc0_rxC0_4_heads0, Acc0_rxC0_5_heads0, Acc0_rxC0_6_heads0, Acc0_rxC0_7_heads0, Acc0_rxC0_8_heads0, Acc0_rxC0_9_heads0, Acc0_rxC0_10_heads0, Acc0_rxC0_11_heads0, Acc0_rxC0_12_heads0, Acc0_rxC0_13_heads0, Acc0_rxC0_14_heads0, Acc0_rxC0_15_heads0, Acc0_rxC0_16_heads0, Acc0_rxC0_17_heads0,  
                    Acc1_txA0_0_heads0, Acc1_txA0_1_heads0,  
                    Acc1_txA1_0_heads0, Acc1_txA1_1_heads0,  
                    Acc1_txA2_0_heads0, Acc1_txA2_1_heads0,  
                    Acc1_txA3_0_heads0, Acc1_txA3_1_heads0,  
                    Acc1_txA0_0_heads1, Acc1_txA0_1_heads1,  
                    Acc1_txA1_0_heads1, Acc1_txA1_1_heads1,  
                    Acc1_txA2_0_heads1, Acc1_txA2_1_heads1,  
                    Acc1_txA3_0_heads1, Acc1_txA3_1_heads1,  
                    Acc1_txA0_0_heads2, Acc1_txA0_1_heads2,  
                    Acc1_txA1_0_heads2, Acc1_txA1_1_heads2,  
                    Acc1_txA2_0_heads2, Acc1_txA2_1_heads2,  
                    Acc1_txA3_0_heads2, Acc1_txA3_1_heads2,  
                    Acc1_txB0_0_heads0, Acc1_txB1_0_heads0,  
                    Acc1_txB0_1_heads0, Acc1_txB1_1_heads0,  
                    Acc1_txB0_0_heads1, Acc1_txB1_0_heads1,  
                    Acc1_txB0_1_heads1, Acc1_txB1_1_heads1,  
                    Acc1_txB0_0_heads2, Acc1_txB1_0_heads2,  
                    Acc1_txB0_1_heads2, Acc1_txB1_1_heads2,  
                    Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0,  
                    Acc1_rxC1_0_heads0, Acc1_rxC1_1_heads0,  
                    Acc1_rxC2_0_heads0, Acc1_rxC2_1_heads0,  
                    Acc1_rxC3_0_heads0, Acc1_rxC3_1_heads0,  
                    Acc1_rxC0_0_heads1, Acc1_rxC0_1_heads1,  
                    Acc1_rxC1_0_heads1, Acc1_rxC1_1_heads1,  
                    Acc1_rxC2_0_heads1, Acc1_rxC2_1_heads1,  
                    Acc1_rxC3_0_heads1, Acc1_rxC3_1_heads1,  
                    Acc1_rxC0_0_heads2, Acc1_rxC0_1_heads2,  
                    Acc1_rxC1_0_heads2, Acc1_rxC1_1_heads2,  
                    Acc1_rxC2_0_heads2, Acc1_rxC2_1_heads2,  
                    Acc1_rxC3_0_heads2, Acc1_rxC3_1_heads2,  
                    Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0, Acc2_txA0_4_heads0, Acc2_txA0_5_heads0,  
                    Acc2_txA1_0_heads0, Acc2_txA1_1_heads0, Acc2_txA1_2_heads0, Acc2_txA1_3_heads0, Acc2_txA1_4_heads0, Acc2_txA1_5_heads0,  
                    Acc2_txA2_0_heads0, Acc2_txA2_1_heads0, Acc2_txA2_2_heads0, Acc2_txA2_3_heads0, Acc2_txA2_4_heads0, Acc2_txA2_5_heads0,  
                    Acc2_txA3_0_heads0, Acc2_txA3_1_heads0, Acc2_txA3_2_heads0, Acc2_txA3_3_heads0, Acc2_txA3_4_heads0, Acc2_txA3_5_heads0,  
                    Acc2_txB0_0_heads0, Acc2_txB1_0_heads0, Acc2_txB2_0_heads0, Acc2_txB3_0_heads0, Acc2_txB4_0_heads0, Acc2_txB5_0_heads0,  
                    Acc2_txB0_1_heads0, Acc2_txB1_1_heads0, Acc2_txB2_1_heads0, Acc2_txB3_1_heads0, Acc2_txB4_1_heads0, Acc2_txB5_1_heads0,  
                    Acc2_txB0_2_heads0, Acc2_txB1_2_heads0, Acc2_txB2_2_heads0, Acc2_txB3_2_heads0, Acc2_txB4_2_heads0, Acc2_txB5_2_heads0,  
                    Acc2_txB0_3_heads0, Acc2_txB1_3_heads0, Acc2_txB2_3_heads0, Acc2_txB3_3_heads0, Acc2_txB4_3_heads0, Acc2_txB5_3_heads0,  
                    Acc2_txB0_4_heads0, Acc2_txB1_4_heads0, Acc2_txB2_4_heads0, Acc2_txB3_4_heads0, Acc2_txB4_4_heads0, Acc2_txB5_4_heads0,  
                    Acc2_txB0_5_heads0, Acc2_txB1_5_heads0, Acc2_txB2_5_heads0, Acc2_txB3_5_heads0, Acc2_txB4_5_heads0, Acc2_txB5_5_heads0,  
                    Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0,  
                    Acc2_rxC1_0_heads0, Acc2_rxC1_1_heads0, Acc2_rxC1_2_heads0, Acc2_rxC1_3_heads0, Acc2_rxC1_4_heads0, Acc2_rxC1_5_heads0,  
                    Acc2_rxC2_0_heads0, Acc2_rxC2_1_heads0, Acc2_rxC2_2_heads0, Acc2_rxC2_3_heads0, Acc2_rxC2_4_heads0, Acc2_rxC2_5_heads0,  
                    Acc2_rxC3_0_heads0, Acc2_rxC3_1_heads0, Acc2_rxC3_2_heads0, Acc2_rxC3_3_heads0, Acc2_rxC3_4_heads0, Acc2_rxC3_5_heads0,  
                    Acc3_txA0_0, Acc3_txA0_1, Acc3_txA0_2, Acc3_txA0_3, Acc3_txA0_4, Acc3_txA0_5,  
                    Acc3_rxC0_0, Acc3_rxC0_1, Acc3_rxC0_2, Acc3_rxC0_3,  
                    Acc4_txA0_0, Acc4_txA0_1,  
                    Acc4_rxC0_0, Acc4_rxC0_1, Acc4_rxC0_2, Acc4_rxC0_3, Acc4_rxC0_4,  
                    dataA_in0, dataA_in1, dataA_in2, dataC_out, batch, block, iteration);

    address_C_ddr(addrC_in,batch,iteration);
    storeC_ddr(out0,addrC_in,dataC_out,batch,iteration);
}