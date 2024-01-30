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
    
}template<int NC>
void Acc0_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf0_lhs0_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      ap_uint<PLIO_WIDTH_128> buf1_lhs0_0[ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc0_txA0, 
                                      axis_stream_plio_128& Acc1_txA0, 
                                      axis_stream_plio_128& Acc2_txA0, 
                                      const bool sel, const bool enable){

#pragma HLS inline off

    if(enable){
        
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC0_M0/ACC0_A/ACC0_H1); x++) { 
            for (int z = 0; z < (ACC0_N0/ACC0_C/ACC0_W2); z++) {
                for (int y = 0; y < (ACC0_K0/ACC0_B/ACC0_W1); y++) {
                    for (int h1 = 0; h1 < ACC0_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC0_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC0_LW1);
                            int pos1=(h1+x*ACC0_LH1);
                            if (sel){
                                data = buf1_lhs0_0[pos1][pos0];
                            }
                            else{
                                data = buf0_lhs0_0[pos1][pos0];
                            }
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc0_txA0.write(tmp);
                            Acc1_txA0.write(tmp);
                            Acc2_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc0_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf0_lhs[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf1_lhs[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc0_txA0_0_heads0, axis_stream_plio_128& Acc0_txA0_1_heads0, axis_stream_plio_128& Acc0_txA0_2_heads0, axis_stream_plio_128& Acc0_txA0_3_heads0,  
                                          axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0, axis_stream_plio_128& Acc1_txA0_2_heads0, axis_stream_plio_128& Acc1_txA0_3_heads0,  
                                          axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0,  
                                          const bool sel, const bool enable){
    Acc0_send_LHS_type0<0>(buf0_lhs[0],buf1_lhs[0],Acc0_txA0_0_heads0,Acc1_txA0_0_heads0,Acc2_txA0_0_heads0,sel, enable);
    Acc0_send_LHS_type0<1>(buf0_lhs[1],buf1_lhs[1],Acc0_txA0_1_heads0,Acc1_txA0_1_heads0,Acc2_txA0_1_heads0,sel, enable);
    Acc0_send_LHS_type0<2>(buf0_lhs[2],buf1_lhs[2],Acc0_txA0_2_heads0,Acc1_txA0_2_heads0,Acc2_txA0_2_heads0,sel, enable);
    Acc0_send_LHS_type0<3>(buf0_lhs[3],buf1_lhs[3],Acc0_txA0_3_heads0,Acc1_txA0_3_heads0,Acc2_txA0_3_heads0,sel, enable);
      
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
void Acc0_receive_buff_type0(axis_stream_128& Acc0_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf_out1_0[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        const int buffer_w2 = ACC0_N0/BUFF0_OUT_PARTA/(NUM_PER_128/NUM_PER_64);
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
                        
                        else{
                            buf_out1_0[pos1][pos0] = data_temp;
                        }
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc0_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF0_OUT_PARTA*BUFF0_OUT_PARTC*ACC0_HEADS][ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
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
    
    
    Acc0_receive_buff_type0<0>(Acc0_plio0_0_heads0_f0,buf_out[0],buf_out[6],enable);
    Acc0_receive_buff_type0<1>(Acc0_plio0_1_heads0_f0,buf_out[1],buf_out[7],enable);
    Acc0_receive_buff_type0<2>(Acc0_plio0_2_heads0_f0,buf_out[2],buf_out[8],enable);
    Acc0_receive_buff_type0<3>(Acc0_plio0_3_heads0_f0,buf_out[3],buf_out[9],enable);
    Acc0_receive_buff_type0<4>(Acc0_plio0_4_heads0_f0,buf_out[4],buf_out[10],enable);
    Acc0_receive_buff_type0<5>(Acc0_plio0_5_heads0_f0,buf_out[5],buf_out[11],enable);
    
}






template<int NC>
void Acc1_receive_OUT(axis_stream_plio_128& Acc1_rxC,axis_stream_128& Acc1_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) { 
            for (int z = 0; z < (ACC1_N0/ACC1_C/ACC1_W2); z++) {
                for (int y = 0; y < (ACC1_K0/ACC1_B/ACC1_W1); y++) {
                    for (int h1 = 0; h1 < ACC1_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc1_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc1_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc1_receive_buff_type0(axis_stream_128& Acc1_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC1_M0/BUFF1_OUT_PARTA/NUM_PER_64][ACC1_N0/BUFF1_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf_out1_0[ACC1_M0/BUFF1_OUT_PARTA/NUM_PER_64][ACC1_N0/BUFF1_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        const int buffer_w2 = ACC1_N0/BUFF1_OUT_PARTA/(NUM_PER_128/NUM_PER_64);
        const int buffer_h1 = ACC1_M0/BUFF1_OUT_PARTA/NUM_PER_64;
        for (int x = 0; x < (ACC1_M0/ACC1_A/ACC1_H1); x++) { 
            for (int z = 0; z < (ACC1_N0/ACC1_C/ACC1_W2); z++) {          
                for (int h1 = 0; h1 < ACC1_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC1_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc1_sin0.read();
                        
                        int pos0=(w2+z*ACC1_RW2)%buffer_w2;
                        int pos1=(h1+x*ACC1_LH1)%buffer_h1;
                        int temp_c=(w2+z*ACC1_RW2)/buffer_w2;
                        int temp_a=(h1+x*ACC1_LH1)/buffer_h1;
                        if (temp_c==0&&temp_a==0){ 
                            buf_out0_0[pos1][pos0] = data_temp;
                        }
                        
                        else{
                            buf_out1_0[pos1][pos0] = data_temp;
                        }
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc1_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF1_OUT_PARTA*BUFF1_OUT_PARTC*ACC1_HEADS][ACC1_M0/BUFF1_OUT_PARTA/NUM_PER_64][ACC1_N0/BUFF1_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0, axis_stream_plio_128& Acc1_rxC0_2_heads0, axis_stream_plio_128& Acc1_rxC0_3_heads0, axis_stream_plio_128& Acc1_rxC0_4_heads0, axis_stream_plio_128& Acc1_rxC0_5_heads0,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc1_plio0_0_heads0_f0;axis_stream_128 Acc1_plio0_1_heads0_f0;axis_stream_128 Acc1_plio0_2_heads0_f0;axis_stream_128 Acc1_plio0_3_heads0_f0;axis_stream_128 Acc1_plio0_4_heads0_f0;axis_stream_128 Acc1_plio0_5_heads0_f0; 
    
    Acc1_receive_OUT<0>(Acc1_rxC0_0_heads0, Acc1_plio0_0_heads0_f0, enable); 
    Acc1_receive_OUT<1>(Acc1_rxC0_1_heads0, Acc1_plio0_1_heads0_f0, enable); 
    Acc1_receive_OUT<2>(Acc1_rxC0_2_heads0, Acc1_plio0_2_heads0_f0, enable); 
    Acc1_receive_OUT<3>(Acc1_rxC0_3_heads0, Acc1_plio0_3_heads0_f0, enable); 
    Acc1_receive_OUT<4>(Acc1_rxC0_4_heads0, Acc1_plio0_4_heads0_f0, enable); 
    Acc1_receive_OUT<5>(Acc1_rxC0_5_heads0, Acc1_plio0_5_heads0_f0, enable); 
    
    
    Acc1_receive_buff_type0<0>(Acc1_plio0_0_heads0_f0,buf_out[0],buf_out[6],enable);
    Acc1_receive_buff_type0<1>(Acc1_plio0_1_heads0_f0,buf_out[1],buf_out[7],enable);
    Acc1_receive_buff_type0<2>(Acc1_plio0_2_heads0_f0,buf_out[2],buf_out[8],enable);
    Acc1_receive_buff_type0<3>(Acc1_plio0_3_heads0_f0,buf_out[3],buf_out[9],enable);
    Acc1_receive_buff_type0<4>(Acc1_plio0_4_heads0_f0,buf_out[4],buf_out[10],enable);
    Acc1_receive_buff_type0<5>(Acc1_plio0_5_heads0_f0,buf_out[5],buf_out[11],enable);
    
}






template<int NC>
void Acc2_receive_OUT(axis_stream_plio_128& Acc2_rxC,axis_stream_128& Acc2_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC2_M0/ACC2_A/ACC2_H1); x++) { 
            for (int z = 0; z < (ACC2_N0/ACC2_C/ACC2_W2); z++) {
                for (int y = 0; y < (ACC2_K0/ACC2_B/ACC2_W1); y++) {
                    for (int h1 = 0; h1 < ACC2_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC2_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc2_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc2_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc2_receive_buff_type0(axis_stream_128& Acc2_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64][ACC2_N0/BUFF2_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf_out1_0[ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64][ACC2_N0/BUFF2_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        const int buffer_w2 = ACC2_N0/BUFF2_OUT_PARTA/(NUM_PER_128/NUM_PER_64);
        const int buffer_h1 = ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64;
        for (int x = 0; x < (ACC2_M0/ACC2_A/ACC2_H1); x++) { 
            for (int z = 0; z < (ACC2_N0/ACC2_C/ACC2_W2); z++) {          
                for (int h1 = 0; h1 < ACC2_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC2_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc2_sin0.read();
                        
                        int pos0=(w2+z*ACC2_RW2)%buffer_w2;
                        int pos1=(h1+x*ACC2_LH1)%buffer_h1;
                        int temp_c=(w2+z*ACC2_RW2)/buffer_w2;
                        int temp_a=(h1+x*ACC2_LH1)/buffer_h1;
                        if (temp_c==0&&temp_a==0){ 
                            buf_out0_0[pos1][pos0] = data_temp;
                        }
                        
                        else{
                            buf_out1_0[pos1][pos0] = data_temp;
                        }
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc2_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF2_OUT_PARTA*BUFF2_OUT_PARTC*ACC2_HEADS][ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64][ACC2_N0/BUFF2_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc2_plio0_0_heads0_f0;axis_stream_128 Acc2_plio0_1_heads0_f0;axis_stream_128 Acc2_plio0_2_heads0_f0;axis_stream_128 Acc2_plio0_3_heads0_f0;axis_stream_128 Acc2_plio0_4_heads0_f0;axis_stream_128 Acc2_plio0_5_heads0_f0; 
    
    Acc2_receive_OUT<0>(Acc2_rxC0_0_heads0, Acc2_plio0_0_heads0_f0, enable); 
    Acc2_receive_OUT<1>(Acc2_rxC0_1_heads0, Acc2_plio0_1_heads0_f0, enable); 
    Acc2_receive_OUT<2>(Acc2_rxC0_2_heads0, Acc2_plio0_2_heads0_f0, enable); 
    Acc2_receive_OUT<3>(Acc2_rxC0_3_heads0, Acc2_plio0_3_heads0_f0, enable); 
    Acc2_receive_OUT<4>(Acc2_rxC0_4_heads0, Acc2_plio0_4_heads0_f0, enable); 
    Acc2_receive_OUT<5>(Acc2_rxC0_5_heads0, Acc2_plio0_5_heads0_f0, enable); 
    
    
    Acc2_receive_buff_type0<0>(Acc2_plio0_0_heads0_f0,buf_out[0],buf_out[6],enable);
    Acc2_receive_buff_type0<1>(Acc2_plio0_1_heads0_f0,buf_out[1],buf_out[7],enable);
    Acc2_receive_buff_type0<2>(Acc2_plio0_2_heads0_f0,buf_out[2],buf_out[8],enable);
    Acc2_receive_buff_type0<3>(Acc2_plio0_3_heads0_f0,buf_out[3],buf_out[9],enable);
    Acc2_receive_buff_type0<4>(Acc2_plio0_4_heads0_f0,buf_out[4],buf_out[10],enable);
    Acc2_receive_buff_type0<5>(Acc2_plio0_5_heads0_f0,buf_out[5],buf_out[11],enable);
    
}

template<int NC>
void Acc3_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf_lhs0_0[ACC3_M0/BUFF3_LHS_PARTA/NUM_PER_64][ACC3_K0/BUFF3_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc3_txA0, 
                                      const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) { 
            for (int z = 0; z < (ACC3_N0/ACC3_C/ACC3_W2); z++) {
                for (int y = 0; y < (ACC3_K0/ACC3_B/ACC3_W1); y++) {
                    for (int h1 = 0; h1 < ACC3_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC3_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC3_LW1);
                            int pos1=(h1+x*ACC3_LH1);

                            data = buf_lhs0_0[pos1][pos0];
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc3_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc3_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_lhs[BUFF3_LHS_PARTA*BUFF3_LHS_PARTB*ACC3_HEADS][ACC3_M0/BUFF3_LHS_PARTA/NUM_PER_64][ACC3_K0/BUFF3_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc3_txA0_0_heads0, axis_stream_plio_128& Acc3_txA0_1_heads0,  
                                          axis_stream_plio_128& Acc3_txA1_0_heads0, axis_stream_plio_128& Acc3_txA1_1_heads0,  
                                          axis_stream_plio_128& Acc3_txA0_0_heads1, axis_stream_plio_128& Acc3_txA0_1_heads1,  
                                          axis_stream_plio_128& Acc3_txA1_0_heads1, axis_stream_plio_128& Acc3_txA1_1_heads1,  
                                          axis_stream_plio_128& Acc3_txA0_0_heads2, axis_stream_plio_128& Acc3_txA0_1_heads2,  
                                          axis_stream_plio_128& Acc3_txA1_0_heads2, axis_stream_plio_128& Acc3_txA1_1_heads2,  
                                          const bool enable){
    Acc3_send_LHS_type0<0>(buf_lhs[0],Acc3_txA0_0_heads0,enable);
    Acc3_send_LHS_type0<1>(buf_lhs[1],Acc3_txA0_1_heads0,enable);
    Acc3_send_LHS_type0<2>(buf_lhs[6],Acc3_txA1_0_heads0,enable);
    Acc3_send_LHS_type0<3>(buf_lhs[7],Acc3_txA1_1_heads0,enable);
    Acc3_send_LHS_type0<4>(buf_lhs[2],Acc3_txA0_0_heads1,enable);
    Acc3_send_LHS_type0<5>(buf_lhs[3],Acc3_txA0_1_heads1,enable);
    Acc3_send_LHS_type0<6>(buf_lhs[8],Acc3_txA1_0_heads1,enable);
    Acc3_send_LHS_type0<7>(buf_lhs[9],Acc3_txA1_1_heads1,enable);
    Acc3_send_LHS_type0<8>(buf_lhs[4],Acc3_txA0_0_heads2,enable);
    Acc3_send_LHS_type0<9>(buf_lhs[5],Acc3_txA0_1_heads2,enable);
    Acc3_send_LHS_type0<10>(buf_lhs[10],Acc3_txA1_0_heads2,enable);
    Acc3_send_LHS_type0<11>(buf_lhs[11],Acc3_txA1_1_heads2,enable);
      
}
template<int NC, int TEMP_B_New, int TEMP_C_New>
void Acc3_send_buff_type0(ap_uint<PLIO_WIDTH_128> buf_rhs[ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_128& Acc3_rhs0, const bool enable){
#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        
        const int buffer_w2 = ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64;
        const int buffer_w1 = ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64);
        
        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) { 
            for (int z = 0; z < (ACC3_N0/ACC3_C/ACC3_W2); z++) {
                for (int y = 0; y < (ACC3_K0/ACC3_B/ACC3_W1); y++) {
                    for (int w1 = 0; w1 < ACC3_RW1_TRANS/4; w1++) {
                        for (int w2 = 0; w2 < ACC3_RW2_TRANS; w2++) {
                            for (int i = 0; i < 4; i++) {
                            #pragma HLS PIPELINE II = 1
                                int pos0 = (w2 + z * ACC3_RW2_TRANS) % buffer_w2;
                                int pos1 = (i + w1 * 4 + y * (ACC3_RW1_TRANS * 4)) % buffer_w1;
                                int temp_c = (w2 + z * ACC3_RW2_TRANS) / buffer_w2;
                                int temp_b = (i + w1 * 4 + y * (ACC3_RW1_TRANS * 4)) / buffer_w1;
                                if (temp_b==TEMP_B_New&&temp_c==TEMP_C_New){   
                                    data = buf_rhs[pos0][pos1];
                                    Acc3_rhs0.write(data);
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
void Acc3_merge(axis_stream_128& Acc3_sin0_0,axis_stream_128& Acc3_sout, const bool enable){

#pragma HLS inline off

    if(enable){

        data_plio_128 data;
        
        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) { 
            for (int z = 0; z < (ACC3_N0/ACC3_C/ACC3_W2); z++) {
                for (int y = 0; y < (ACC3_K0/ACC3_B/ACC3_W1); y++) {
                    for (int w1 = 0; w1 < ACC3_RW1_TRANS/4; w1++) {
                        for (int w2 = 0; w2 < ACC3_RW2_TRANS; w2++) {
                            for (int i = 0; i < 4; i++) {
                            #pragma HLS PIPELINE II = 1

                                data = Acc3_sin0_0.read();
                                Acc3_sout.write(data);
                            }
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void Acc3_transpose(axis_stream_128& Acc3_sin, axis_stream_128& Acc3_sout, const bool enable){

#pragma HLS inline off
    if(enable){
        data_plio_128 temp_data;
        data_plio_128 data;

        ap_uint<8> data0[NUM_PER_64][NUM_PER_64];
        #pragma HLS ARRAY_PARTITION variable=data0 complete dim=0

        ap_uint<8> data1[NUM_PER_64][NUM_PER_64];
        #pragma HLS ARRAY_PARTITION variable=data1 complete dim=0

        int total_bound = (ACC3_K0/ACC3_B/ACC3_W1) * (ACC3_N0/ACC3_C/ACC3_W2) *  ACC3_RW1 * ACC3_RW2;
        
        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) {
            for (int i = 0; i < (total_bound+4); i++) {  
            #pragma HLS PIPELINE II = 1
                int pos0=i%4;
                int pos1=i/4;
                int pos=pos0*2;
                if(i<4){
                    data=Acc3_sin.read();
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
                    Acc3_sout.write(temp_data);
                }
                else if(pos1%2==1){
                    data=Acc3_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data1[un%8][pos+(un/8)]= data(7+un*8,un*8);
                        temp_data(7+un*8,un*8) = data0[pos+(un/8)][(un%8)];
                    }
                    Acc3_sout.write(temp_data);
                }
                else{
                    data=Acc3_sin.read();
                    for (int un=0; un<NUM_PER_128; un++){
                    #pragma HLS UNROLL factor=NUM_PER_128
                        data0[un%8][pos+(un/8)]= data(7+un*8,un*8);
                        temp_data(7+un*8,un*8) = data1[pos+(un/8)][(un%8)];
                    }
                    Acc3_sout.write(temp_data);
                }
            }
        }
    }
}

template<int NC>
void Acc3_send_RHS(axis_stream_128& Acc3_sin0, axis_stream_plio_128& Acc3_txB0, const bool enable){
#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) { 
            for (int z = 0; z < (ACC3_N0/ACC3_C/ACC3_W2); z++) {
                for (int y = 0; y < (ACC3_K0/ACC3_B/ACC3_W1); y++) {
                    for (int w1 = 0; w1 < ACC3_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC3_RW2; w2++) {
                            data = Acc3_sin0.read();
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc3_txB0.write(tmp);
                            
                        }
                    }
                }
            }
        }
    }
}

template<int NC>
void Acc3_send_RHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_rhs[BUFF0_OUT_PARTA*BUFF0_OUT_PARTC*ACC0_HEADS][ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
            axis_stream_plio_128& Acc3_txB0_0_heads0, axis_stream_plio_128& Acc3_txB1_0_heads0,  
            axis_stream_plio_128& Acc3_txB0_1_heads0, axis_stream_plio_128& Acc3_txB1_1_heads0,  
            axis_stream_plio_128& Acc3_txB0_0_heads1, axis_stream_plio_128& Acc3_txB1_0_heads1,  
            axis_stream_plio_128& Acc3_txB0_1_heads1, axis_stream_plio_128& Acc3_txB1_1_heads1,  
            axis_stream_plio_128& Acc3_txB0_0_heads2, axis_stream_plio_128& Acc3_txB1_0_heads2,  
            axis_stream_plio_128& Acc3_txB0_1_heads2, axis_stream_plio_128& Acc3_txB1_1_heads2,  
            const bool enable){


#pragma HLS dataflow

    axis_stream_128 Acc3_rhs0_0_p0_0_heads0;
    axis_stream_128 Acc3_rhs1_0_p0_0_heads0;
    axis_stream_128 Acc3_rhs0_1_p0_0_heads0;
    axis_stream_128 Acc3_rhs1_1_p0_0_heads0;
    axis_stream_128 Acc3_rhs0_0_p0_0_heads1;
    axis_stream_128 Acc3_rhs1_0_p0_0_heads1;
    axis_stream_128 Acc3_rhs0_1_p0_0_heads1;
    axis_stream_128 Acc3_rhs1_1_p0_0_heads1;
    axis_stream_128 Acc3_rhs0_0_p0_0_heads2;
    axis_stream_128 Acc3_rhs1_0_p0_0_heads2;
    axis_stream_128 Acc3_rhs0_1_p0_0_heads2;
    axis_stream_128 Acc3_rhs1_1_p0_0_heads2;
    axis_stream_128 Acc3_plio0_0_heads0; axis_stream_128 Acc3_plio0_1_heads0;  
    axis_stream_128 Acc3_plio1_0_heads0; axis_stream_128 Acc3_plio1_1_heads0;  
    axis_stream_128 Acc3_plio0_0_heads1; axis_stream_128 Acc3_plio0_1_heads1;  
    axis_stream_128 Acc3_plio1_0_heads1; axis_stream_128 Acc3_plio1_1_heads1;  
    axis_stream_128 Acc3_plio0_0_heads2; axis_stream_128 Acc3_plio0_1_heads2;  
    axis_stream_128 Acc3_plio1_0_heads2; axis_stream_128 Acc3_plio1_1_heads2;  
    

    axis_stream_128 Acc3_s0_0_heads0;
    axis_stream_128 Acc3_s0_1_heads0;
    axis_stream_128 Acc3_s1_0_heads0;
    axis_stream_128 Acc3_s1_1_heads0;
    axis_stream_128 Acc3_s0_0_heads1;
    axis_stream_128 Acc3_s0_1_heads1;
    axis_stream_128 Acc3_s1_0_heads1;
    axis_stream_128 Acc3_s1_1_heads1;
    axis_stream_128 Acc3_s0_0_heads2;
    axis_stream_128 Acc3_s0_1_heads2;
    axis_stream_128 Acc3_s1_0_heads2;
    axis_stream_128 Acc3_s1_1_heads2;
    
    Acc3_send_buff_type0<0,0,0>(buf_rhs[0], Acc3_rhs0_0_p0_0_heads0, enable);
    Acc3_send_buff_type0<1,0,0>(buf_rhs[1], Acc3_rhs1_0_p0_0_heads0, enable);
    Acc3_send_buff_type0<6,0,0>(buf_rhs[6], Acc3_rhs0_1_p0_0_heads0, enable);
    Acc3_send_buff_type0<7,0,0>(buf_rhs[7], Acc3_rhs1_1_p0_0_heads0, enable);
    Acc3_merge<0>(Acc3_rhs0_0_p0_0_heads0,Acc3_plio0_0_heads0, enable); 
    Acc3_transpose<0>(Acc3_plio0_0_heads0, Acc3_s0_0_heads0, enable);  
    Acc3_send_RHS<0>(Acc3_s0_0_heads0, Acc3_txB0_0_heads0, enable);
    Acc3_merge<1>(Acc3_rhs0_1_p0_0_heads0,Acc3_plio0_1_heads0, enable); 
    Acc3_transpose<1>(Acc3_plio0_1_heads0, Acc3_s0_1_heads0, enable);  
    Acc3_send_RHS<1>(Acc3_s0_1_heads0, Acc3_txB0_1_heads0, enable);
    Acc3_merge<2>(Acc3_rhs1_0_p0_0_heads0,Acc3_plio1_0_heads0, enable); 
    Acc3_transpose<2>(Acc3_plio1_0_heads0, Acc3_s1_0_heads0, enable);  
    Acc3_send_RHS<2>(Acc3_s1_0_heads0, Acc3_txB1_0_heads0, enable);
    Acc3_merge<3>(Acc3_rhs1_1_p0_0_heads0,Acc3_plio1_1_heads0, enable); 
    Acc3_transpose<3>(Acc3_plio1_1_heads0, Acc3_s1_1_heads0, enable);  
    Acc3_send_RHS<3>(Acc3_s1_1_heads0, Acc3_txB1_1_heads0, enable);
    
    Acc3_send_buff_type0<2,0,0>(buf_rhs[2], Acc3_rhs0_0_p0_0_heads1, enable);
    Acc3_send_buff_type0<3,0,0>(buf_rhs[3], Acc3_rhs1_0_p0_0_heads1, enable);
    Acc3_send_buff_type0<8,0,0>(buf_rhs[8], Acc3_rhs0_1_p0_0_heads1, enable);
    Acc3_send_buff_type0<9,0,0>(buf_rhs[9], Acc3_rhs1_1_p0_0_heads1, enable);
    Acc3_merge<4>(Acc3_rhs0_0_p0_0_heads1,Acc3_plio0_0_heads1, enable); 
    Acc3_transpose<4>(Acc3_plio0_0_heads1, Acc3_s0_0_heads1, enable);  
    Acc3_send_RHS<4>(Acc3_s0_0_heads1, Acc3_txB0_0_heads1, enable);
    Acc3_merge<5>(Acc3_rhs0_1_p0_0_heads1,Acc3_plio0_1_heads1, enable); 
    Acc3_transpose<5>(Acc3_plio0_1_heads1, Acc3_s0_1_heads1, enable);  
    Acc3_send_RHS<5>(Acc3_s0_1_heads1, Acc3_txB0_1_heads1, enable);
    Acc3_merge<6>(Acc3_rhs1_0_p0_0_heads1,Acc3_plio1_0_heads1, enable); 
    Acc3_transpose<6>(Acc3_plio1_0_heads1, Acc3_s1_0_heads1, enable);  
    Acc3_send_RHS<6>(Acc3_s1_0_heads1, Acc3_txB1_0_heads1, enable);
    Acc3_merge<7>(Acc3_rhs1_1_p0_0_heads1,Acc3_plio1_1_heads1, enable); 
    Acc3_transpose<7>(Acc3_plio1_1_heads1, Acc3_s1_1_heads1, enable);  
    Acc3_send_RHS<7>(Acc3_s1_1_heads1, Acc3_txB1_1_heads1, enable);
    
    Acc3_send_buff_type0<4,0,0>(buf_rhs[4], Acc3_rhs0_0_p0_0_heads2, enable);
    Acc3_send_buff_type0<5,0,0>(buf_rhs[5], Acc3_rhs1_0_p0_0_heads2, enable);
    Acc3_send_buff_type0<10,0,0>(buf_rhs[10], Acc3_rhs0_1_p0_0_heads2, enable);
    Acc3_send_buff_type0<11,0,0>(buf_rhs[11], Acc3_rhs1_1_p0_0_heads2, enable);
    Acc3_merge<8>(Acc3_rhs0_0_p0_0_heads2,Acc3_plio0_0_heads2, enable); 
    Acc3_transpose<8>(Acc3_plio0_0_heads2, Acc3_s0_0_heads2, enable);  
    Acc3_send_RHS<8>(Acc3_s0_0_heads2, Acc3_txB0_0_heads2, enable);
    Acc3_merge<9>(Acc3_rhs0_1_p0_0_heads2,Acc3_plio0_1_heads2, enable); 
    Acc3_transpose<9>(Acc3_plio0_1_heads2, Acc3_s0_1_heads2, enable);  
    Acc3_send_RHS<9>(Acc3_s0_1_heads2, Acc3_txB0_1_heads2, enable);
    Acc3_merge<10>(Acc3_rhs1_0_p0_0_heads2,Acc3_plio1_0_heads2, enable); 
    Acc3_transpose<10>(Acc3_plio1_0_heads2, Acc3_s1_0_heads2, enable);  
    Acc3_send_RHS<10>(Acc3_s1_0_heads2, Acc3_txB1_0_heads2, enable);
    Acc3_merge<11>(Acc3_rhs1_1_p0_0_heads2,Acc3_plio1_1_heads2, enable); 
    Acc3_transpose<11>(Acc3_plio1_1_heads2, Acc3_s1_1_heads2, enable);  
    Acc3_send_RHS<11>(Acc3_s1_1_heads2, Acc3_txB1_1_heads2, enable);
    
}



template<int NC>
void Acc3_receive_OUT(axis_stream_plio_128& Acc3_rxC,axis_stream_128& Acc3_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) { 
            for (int z = 0; z < (ACC3_N0/ACC3_C/ACC3_W2); z++) {
                for (int y = 0; y < (ACC3_K0/ACC3_B/ACC3_W1); y++) {
                    for (int h1 = 0; h1 < ACC3_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC3_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc3_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc3_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc3_receive_buff_type0(axis_stream_128& Acc3_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC3_M0/BUFF3_OUT_PARTA/NUM_PER_64][ACC3_N0/BUFF3_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        for (int x = 0; x < (ACC3_M0/ACC3_A/ACC3_H1); x++) { 
            for (int z = 0; z < (ACC3_N0/ACC3_C/ACC3_W2); z++) {          
                for (int h1 = 0; h1 < ACC3_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC3_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc3_sin0.read();
                        
                        int pos0=(w2+z*ACC3_RW2);
                        int pos1=(h1+x*ACC3_LH1);
                        buf_out0_0[pos1][pos0] = data_temp;
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc3_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF3_OUT_PARTA*BUFF3_OUT_PARTC*ACC3_HEADS][ACC3_M0/BUFF3_OUT_PARTA/NUM_PER_64][ACC3_N0/BUFF3_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc3_rxC0_0_heads0, axis_stream_plio_128& Acc3_rxC0_1_heads0,  
           axis_stream_plio_128& Acc3_rxC1_0_heads0, axis_stream_plio_128& Acc3_rxC1_1_heads0,  
           axis_stream_plio_128& Acc3_rxC0_0_heads1, axis_stream_plio_128& Acc3_rxC0_1_heads1,  
           axis_stream_plio_128& Acc3_rxC1_0_heads1, axis_stream_plio_128& Acc3_rxC1_1_heads1,  
           axis_stream_plio_128& Acc3_rxC0_0_heads2, axis_stream_plio_128& Acc3_rxC0_1_heads2,  
           axis_stream_plio_128& Acc3_rxC1_0_heads2, axis_stream_plio_128& Acc3_rxC1_1_heads2,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc3_plio0_0_heads0_f0;axis_stream_128 Acc3_plio0_1_heads0_f0; 
    axis_stream_128 Acc3_plio1_0_heads0_f0;axis_stream_128 Acc3_plio1_1_heads0_f0; 
    
    axis_stream_128 Acc3_plio0_0_heads1_f0;axis_stream_128 Acc3_plio0_1_heads1_f0; 
    axis_stream_128 Acc3_plio1_0_heads1_f0;axis_stream_128 Acc3_plio1_1_heads1_f0; 
    
    axis_stream_128 Acc3_plio0_0_heads2_f0;axis_stream_128 Acc3_plio0_1_heads2_f0; 
    axis_stream_128 Acc3_plio1_0_heads2_f0;axis_stream_128 Acc3_plio1_1_heads2_f0; 
    
    Acc3_receive_OUT<0>(Acc3_rxC0_0_heads0, Acc3_plio0_0_heads0_f0, enable); 
    Acc3_receive_OUT<1>(Acc3_rxC0_1_heads0, Acc3_plio0_1_heads0_f0, enable); 
    Acc3_receive_OUT<2>(Acc3_rxC1_0_heads0, Acc3_plio1_0_heads0_f0, enable); 
    Acc3_receive_OUT<3>(Acc3_rxC1_1_heads0, Acc3_plio1_1_heads0_f0, enable); 
    Acc3_receive_OUT<4>(Acc3_rxC0_0_heads1, Acc3_plio0_0_heads1_f0, enable); 
    Acc3_receive_OUT<5>(Acc3_rxC0_1_heads1, Acc3_plio0_1_heads1_f0, enable); 
    Acc3_receive_OUT<6>(Acc3_rxC1_0_heads1, Acc3_plio1_0_heads1_f0, enable); 
    Acc3_receive_OUT<7>(Acc3_rxC1_1_heads1, Acc3_plio1_1_heads1_f0, enable); 
    Acc3_receive_OUT<8>(Acc3_rxC0_0_heads2, Acc3_plio0_0_heads2_f0, enable); 
    Acc3_receive_OUT<9>(Acc3_rxC0_1_heads2, Acc3_plio0_1_heads2_f0, enable); 
    Acc3_receive_OUT<10>(Acc3_rxC1_0_heads2, Acc3_plio1_0_heads2_f0, enable); 
    Acc3_receive_OUT<11>(Acc3_rxC1_1_heads2, Acc3_plio1_1_heads2_f0, enable); 
    
    
    Acc3_receive_buff_type0<0>(Acc3_plio0_0_heads0_f0,buf_out[0],enable);
    Acc3_receive_buff_type0<1>(Acc3_plio0_1_heads0_f0,buf_out[1],enable);
    Acc3_receive_buff_type0<2>(Acc3_plio1_0_heads0_f0,buf_out[6],enable);
    Acc3_receive_buff_type0<3>(Acc3_plio1_1_heads0_f0,buf_out[7],enable);
    Acc3_receive_buff_type0<4>(Acc3_plio0_0_heads1_f0,buf_out[2],enable);
    Acc3_receive_buff_type0<5>(Acc3_plio0_1_heads1_f0,buf_out[3],enable);
    Acc3_receive_buff_type0<6>(Acc3_plio1_0_heads1_f0,buf_out[8],enable);
    Acc3_receive_buff_type0<7>(Acc3_plio1_1_heads1_f0,buf_out[9],enable);
    Acc3_receive_buff_type0<8>(Acc3_plio0_0_heads2_f0,buf_out[4],enable);
    Acc3_receive_buff_type0<9>(Acc3_plio0_1_heads2_f0,buf_out[5],enable);
    Acc3_receive_buff_type0<10>(Acc3_plio1_0_heads2_f0,buf_out[10],enable);
    Acc3_receive_buff_type0<11>(Acc3_plio1_1_heads2_f0,buf_out[11],enable);
    
}

template<int NC>
void Acc4_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf_lhs0_0[ACC4_M0/BUFF4_LHS_PARTA/NUM_PER_64][ACC4_K0/BUFF4_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc4_txA0, 
                                      const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC4_M0/ACC4_A/ACC4_H1); x++) { 
            for (int z = 0; z < (ACC4_N0/ACC4_C/ACC4_W2); z++) {
                for (int y = 0; y < (ACC4_K0/ACC4_B/ACC4_W1); y++) {
                    for (int h1 = 0; h1 < ACC4_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC4_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC4_LW1);
                            int pos1=(h1+x*ACC4_LH1);

                            data = buf_lhs0_0[pos1][pos0];
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc4_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc4_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_lhs[BUFF4_LHS_PARTA*BUFF4_LHS_PARTB*ACC4_HEADS][ACC4_M0/BUFF4_LHS_PARTA/NUM_PER_64][ACC4_K0/BUFF4_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc4_txA0_0_heads0, axis_stream_plio_128& Acc4_txA0_1_heads0,  
                                          axis_stream_plio_128& Acc4_txA1_0_heads0, axis_stream_plio_128& Acc4_txA1_1_heads0,  
                                          axis_stream_plio_128& Acc4_txA0_0_heads1, axis_stream_plio_128& Acc4_txA0_1_heads1,  
                                          axis_stream_plio_128& Acc4_txA1_0_heads1, axis_stream_plio_128& Acc4_txA1_1_heads1,  
                                          axis_stream_plio_128& Acc4_txA0_0_heads2, axis_stream_plio_128& Acc4_txA0_1_heads2,  
                                          axis_stream_plio_128& Acc4_txA1_0_heads2, axis_stream_plio_128& Acc4_txA1_1_heads2,  
                                          const bool enable){
    Acc4_send_LHS_type0<0>(buf_lhs[0],Acc4_txA0_0_heads0,enable);
    Acc4_send_LHS_type0<1>(buf_lhs[1],Acc4_txA0_1_heads0,enable);
    Acc4_send_LHS_type0<2>(buf_lhs[6],Acc4_txA1_0_heads0,enable);
    Acc4_send_LHS_type0<3>(buf_lhs[7],Acc4_txA1_1_heads0,enable);
    Acc4_send_LHS_type0<4>(buf_lhs[2],Acc4_txA0_0_heads1,enable);
    Acc4_send_LHS_type0<5>(buf_lhs[3],Acc4_txA0_1_heads1,enable);
    Acc4_send_LHS_type0<6>(buf_lhs[8],Acc4_txA1_0_heads1,enable);
    Acc4_send_LHS_type0<7>(buf_lhs[9],Acc4_txA1_1_heads1,enable);
    Acc4_send_LHS_type0<8>(buf_lhs[4],Acc4_txA0_0_heads2,enable);
    Acc4_send_LHS_type0<9>(buf_lhs[5],Acc4_txA0_1_heads2,enable);
    Acc4_send_LHS_type0<10>(buf_lhs[10],Acc4_txA1_0_heads2,enable);
    Acc4_send_LHS_type0<11>(buf_lhs[11],Acc4_txA1_1_heads2,enable);
      
}



template<int NC>
void Acc4_send_dummy_top(ap_uint<PLIO_WIDTH_128> buf_in[BUFF4_RHS_PARTB*BUFF4_RHS_PARTC*ACC4_HEADS][ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64][ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf_out[BUFF4_RHS_PARTB*BUFF4_RHS_PARTC*ACC4_HEADS][ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64][ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64)],const bool enable){
#pragma HLS inline off
    if(enable){
        for (int j = 0; j < ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64; j++) {
            for (int i = 0; i < ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64); i++) {
                #pragma HLS PIPELINE II = 1
                for (int k = 0; k < BUFF4_RHS_PARTB*BUFF4_RHS_PARTC*ACC4_HEADS; k++) {
                    buf_out[k][j][i]=buf_in[k][j][i];
                }
            }
        }
    }
}


template<int NC>
void Acc4_send_RHS_type0(ap_uint<PLIO_WIDTH_128> buf_rhs0_0[ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64][ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64)],axis_stream_plio_128& Acc4_txB0, const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC4_M0/ACC4_A/ACC4_H1); x++) { 
            for (int z = 0; z < (ACC4_N0/ACC4_C/ACC4_W2); z++) {
                for (int y = 0; y < (ACC4_K0/ACC4_B/ACC4_W1); y++) {
                    for (int w1 = 0; w1 < ACC4_RW1; w1++) {
                        for (int w2 = 0; w2 < ACC4_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w2+z*ACC4_RW2);
                            int pos1=(w1+y*ACC4_RW1);

                            data = buf_rhs0_0[pos1][pos0];
                            
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc4_txB0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc4_send_RHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_rhs[BUFF4_RHS_PARTB*BUFF4_RHS_PARTC*ACC4_HEADS][ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64][ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc4_txB0_0_heads0, axis_stream_plio_128& Acc4_txB0_1_heads0,  
                                          axis_stream_plio_128& Acc4_txB1_0_heads0, axis_stream_plio_128& Acc4_txB1_1_heads0,  
                                          axis_stream_plio_128& Acc4_txB0_0_heads1, axis_stream_plio_128& Acc4_txB0_1_heads1,  
                                          axis_stream_plio_128& Acc4_txB1_0_heads1, axis_stream_plio_128& Acc4_txB1_1_heads1,  
                                          axis_stream_plio_128& Acc4_txB0_0_heads2, axis_stream_plio_128& Acc4_txB0_1_heads2,  
                                          axis_stream_plio_128& Acc4_txB1_0_heads2, axis_stream_plio_128& Acc4_txB1_1_heads2,  
                                          const bool enable){
    Acc4_send_RHS_type0<0>(buf_rhs[0],Acc4_txB0_0_heads0,enable);
    Acc4_send_RHS_type0<1>(buf_rhs[1],Acc4_txB0_1_heads0,enable);
    Acc4_send_RHS_type0<2>(buf_rhs[6],Acc4_txB1_0_heads0,enable);
    Acc4_send_RHS_type0<3>(buf_rhs[7],Acc4_txB1_1_heads0,enable);
    Acc4_send_RHS_type0<4>(buf_rhs[2],Acc4_txB0_0_heads1,enable);
    Acc4_send_RHS_type0<5>(buf_rhs[3],Acc4_txB0_1_heads1,enable);
    Acc4_send_RHS_type0<6>(buf_rhs[8],Acc4_txB1_0_heads1,enable);
    Acc4_send_RHS_type0<7>(buf_rhs[9],Acc4_txB1_1_heads1,enable);
    Acc4_send_RHS_type0<8>(buf_rhs[4],Acc4_txB0_0_heads2,enable);
    Acc4_send_RHS_type0<9>(buf_rhs[5],Acc4_txB0_1_heads2,enable);
    Acc4_send_RHS_type0<10>(buf_rhs[10],Acc4_txB1_0_heads2,enable);
    Acc4_send_RHS_type0<11>(buf_rhs[11],Acc4_txB1_1_heads2,enable);
      
}


template<int NC>
void Acc4_receive_OUT(axis_stream_plio_128& Acc4_rxC,axis_stream_128& Acc4_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC4_M0/ACC4_A/ACC4_H1); x++) { 
            for (int z = 0; z < (ACC4_N0/ACC4_C/ACC4_W2); z++) {
                for (int y = 0; y < (ACC4_K0/ACC4_B/ACC4_W1); y++) {
                    for (int h1 = 0; h1 < ACC4_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC4_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc4_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc4_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc4_receive_buff_type0(axis_stream_128& Acc4_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC4_M0/BUFF4_OUT_PARTA/NUM_PER_64][ACC4_N0/BUFF4_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        for (int x = 0; x < (ACC4_M0/ACC4_A/ACC4_H1); x++) { 
            for (int z = 0; z < (ACC4_N0/ACC4_C/ACC4_W2); z++) {          
                for (int h1 = 0; h1 < ACC4_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC4_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc4_sin0.read();
                        
                        int pos0=(w2+z*ACC4_RW2);
                        int pos1=(h1+x*ACC4_LH1);
                        buf_out0_0[pos1][pos0] = data_temp;
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc4_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF4_OUT_PARTA*BUFF4_OUT_PARTC*ACC4_HEADS][ACC4_M0/BUFF4_OUT_PARTA/NUM_PER_64][ACC4_N0/BUFF4_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc4_rxC0_0_heads0, axis_stream_plio_128& Acc4_rxC0_1_heads0,  
           axis_stream_plio_128& Acc4_rxC1_0_heads0, axis_stream_plio_128& Acc4_rxC1_1_heads0,  
           axis_stream_plio_128& Acc4_rxC0_0_heads1, axis_stream_plio_128& Acc4_rxC0_1_heads1,  
           axis_stream_plio_128& Acc4_rxC1_0_heads1, axis_stream_plio_128& Acc4_rxC1_1_heads1,  
           axis_stream_plio_128& Acc4_rxC0_0_heads2, axis_stream_plio_128& Acc4_rxC0_1_heads2,  
           axis_stream_plio_128& Acc4_rxC1_0_heads2, axis_stream_plio_128& Acc4_rxC1_1_heads2,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc4_plio0_0_heads0_f0;axis_stream_128 Acc4_plio0_1_heads0_f0; 
    axis_stream_128 Acc4_plio1_0_heads0_f0;axis_stream_128 Acc4_plio1_1_heads0_f0; 
    
    axis_stream_128 Acc4_plio0_0_heads1_f0;axis_stream_128 Acc4_plio0_1_heads1_f0; 
    axis_stream_128 Acc4_plio1_0_heads1_f0;axis_stream_128 Acc4_plio1_1_heads1_f0; 
    
    axis_stream_128 Acc4_plio0_0_heads2_f0;axis_stream_128 Acc4_plio0_1_heads2_f0; 
    axis_stream_128 Acc4_plio1_0_heads2_f0;axis_stream_128 Acc4_plio1_1_heads2_f0; 
    
    Acc4_receive_OUT<0>(Acc4_rxC0_0_heads0, Acc4_plio0_0_heads0_f0, enable); 
    Acc4_receive_OUT<1>(Acc4_rxC0_1_heads0, Acc4_plio0_1_heads0_f0, enable); 
    Acc4_receive_OUT<2>(Acc4_rxC1_0_heads0, Acc4_plio1_0_heads0_f0, enable); 
    Acc4_receive_OUT<3>(Acc4_rxC1_1_heads0, Acc4_plio1_1_heads0_f0, enable); 
    Acc4_receive_OUT<4>(Acc4_rxC0_0_heads1, Acc4_plio0_0_heads1_f0, enable); 
    Acc4_receive_OUT<5>(Acc4_rxC0_1_heads1, Acc4_plio0_1_heads1_f0, enable); 
    Acc4_receive_OUT<6>(Acc4_rxC1_0_heads1, Acc4_plio1_0_heads1_f0, enable); 
    Acc4_receive_OUT<7>(Acc4_rxC1_1_heads1, Acc4_plio1_1_heads1_f0, enable); 
    Acc4_receive_OUT<8>(Acc4_rxC0_0_heads2, Acc4_plio0_0_heads2_f0, enable); 
    Acc4_receive_OUT<9>(Acc4_rxC0_1_heads2, Acc4_plio0_1_heads2_f0, enable); 
    Acc4_receive_OUT<10>(Acc4_rxC1_0_heads2, Acc4_plio1_0_heads2_f0, enable); 
    Acc4_receive_OUT<11>(Acc4_rxC1_1_heads2, Acc4_plio1_1_heads2_f0, enable); 
    
    
    Acc4_receive_buff_type0<0>(Acc4_plio0_0_heads0_f0,buf_out[0],enable);
    Acc4_receive_buff_type0<1>(Acc4_plio0_1_heads0_f0,buf_out[1],enable);
    Acc4_receive_buff_type0<2>(Acc4_plio1_0_heads0_f0,buf_out[6],enable);
    Acc4_receive_buff_type0<3>(Acc4_plio1_1_heads0_f0,buf_out[7],enable);
    Acc4_receive_buff_type0<4>(Acc4_plio0_0_heads1_f0,buf_out[2],enable);
    Acc4_receive_buff_type0<5>(Acc4_plio0_1_heads1_f0,buf_out[3],enable);
    Acc4_receive_buff_type0<6>(Acc4_plio1_0_heads1_f0,buf_out[8],enable);
    Acc4_receive_buff_type0<7>(Acc4_plio1_1_heads1_f0,buf_out[9],enable);
    Acc4_receive_buff_type0<8>(Acc4_plio0_0_heads2_f0,buf_out[4],enable);
    Acc4_receive_buff_type0<9>(Acc4_plio0_1_heads2_f0,buf_out[5],enable);
    Acc4_receive_buff_type0<10>(Acc4_plio1_0_heads2_f0,buf_out[10],enable);
    Acc4_receive_buff_type0<11>(Acc4_plio1_1_heads2_f0,buf_out[11],enable);
    
}

template<int NC>
void Acc5_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf_lhs0_0[ACC5_M0/BUFF5_LHS_PARTA/NUM_PER_64][ACC5_K0/BUFF5_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_128> buf_lhs1_0[ACC5_M0/BUFF5_LHS_PARTA/NUM_PER_64][ACC5_K0/BUFF5_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc5_txA0, 
                                      const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        const int buffer_w1 = ACC5_K0/BUFF5_LHS_PARTB/(NUM_PER_128/NUM_PER_64);
        const int buffer_h1 = ACC5_M0/BUFF5_LHS_PARTA/NUM_PER_64;
        for (int x = 0; x < (ACC5_M0/ACC5_A/ACC5_H1); x++) { 
            for (int z = 0; z < (ACC5_N0/ACC5_C/ACC5_W2); z++) {
                for (int y = 0; y < (ACC5_K0/ACC5_B/ACC5_W1); y++) {
                    for (int h1 = 0; h1 < ACC5_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC5_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC5_LW1)%buffer_w1;
                            int pos1=(h1+x*ACC5_LH1)%buffer_h1;
                            int temp_b=(w1+y*ACC5_LW1)/buffer_w1;
                            int temp_a=(h1+x*ACC5_LH1)/buffer_h1;
                            
                            if (temp_b==0&&temp_a==0){ 
                                data = buf_lhs0_0[pos1][pos0];
                            }else{
                                data = buf_lhs1_0[pos1][pos0];
                            }
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc5_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc5_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_lhs[BUFF5_LHS_PARTA*BUFF5_LHS_PARTB*ACC5_HEADS][ACC5_M0/BUFF5_LHS_PARTA/NUM_PER_64][ACC5_K0/BUFF5_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc5_txA0_0_heads0, axis_stream_plio_128& Acc5_txA0_1_heads0, axis_stream_plio_128& Acc5_txA0_2_heads0, axis_stream_plio_128& Acc5_txA0_3_heads0, axis_stream_plio_128& Acc5_txA0_4_heads0, axis_stream_plio_128& Acc5_txA0_5_heads0,  
                                          const bool enable){
    Acc5_send_LHS_type0<0>(buf_lhs[0],buf_lhs[6],Acc5_txA0_0_heads0,enable);
    Acc5_send_LHS_type0<1>(buf_lhs[1],buf_lhs[7],Acc5_txA0_1_heads0,enable);
    Acc5_send_LHS_type0<2>(buf_lhs[2],buf_lhs[8],Acc5_txA0_2_heads0,enable);
    Acc5_send_LHS_type0<3>(buf_lhs[3],buf_lhs[9],Acc5_txA0_3_heads0,enable);
    Acc5_send_LHS_type0<4>(buf_lhs[4],buf_lhs[10],Acc5_txA0_4_heads0,enable);
    Acc5_send_LHS_type0<5>(buf_lhs[5],buf_lhs[11],Acc5_txA0_5_heads0,enable);
      
}




template<int NC>
void Acc5_receive_OUT(axis_stream_plio_128& Acc5_rxC,axis_stream_128& Acc5_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC5_M0/ACC5_A/ACC5_H1); x++) { 
            for (int z = 0; z < (ACC5_N0/ACC5_C/ACC5_W2); z++) {
                for (int y = 0; y < (ACC5_K0/ACC5_B/ACC5_W1); y++) {
                    for (int h1 = 0; h1 < ACC5_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC5_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc5_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc5_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc5_receive_buff_type0(axis_stream_128& Acc5_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC5_M0/BUFF5_OUT_PARTA/NUM_PER_64][ACC5_N0/BUFF5_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        for (int x = 0; x < (ACC5_M0/ACC5_A/ACC5_H1); x++) { 
            for (int z = 0; z < (ACC5_N0/ACC5_C/ACC5_W2); z++) {          
                for (int h1 = 0; h1 < ACC5_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC5_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc5_sin0.read();
                        
                        int pos0=(w2+z*ACC5_RW2);
                        int pos1=(h1+x*ACC5_LH1);
                        buf_out0_0[pos1][pos0] = data_temp;
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc5_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF5_OUT_PARTA*BUFF5_OUT_PARTC*ACC5_HEADS][ACC5_M0/BUFF5_OUT_PARTA/NUM_PER_64][ACC5_N0/BUFF5_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc5_rxC0_0_heads0, axis_stream_plio_128& Acc5_rxC0_1_heads0, axis_stream_plio_128& Acc5_rxC0_2_heads0, axis_stream_plio_128& Acc5_rxC0_3_heads0,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc5_plio0_0_heads0_f0;axis_stream_128 Acc5_plio0_1_heads0_f0;axis_stream_128 Acc5_plio0_2_heads0_f0;axis_stream_128 Acc5_plio0_3_heads0_f0; 
    
    Acc5_receive_OUT<0>(Acc5_rxC0_0_heads0, Acc5_plio0_0_heads0_f0, enable); 
    Acc5_receive_OUT<1>(Acc5_rxC0_1_heads0, Acc5_plio0_1_heads0_f0, enable); 
    Acc5_receive_OUT<2>(Acc5_rxC0_2_heads0, Acc5_plio0_2_heads0_f0, enable); 
    Acc5_receive_OUT<3>(Acc5_rxC0_3_heads0, Acc5_plio0_3_heads0_f0, enable); 
    
    
    Acc5_receive_buff_type0<0>(Acc5_plio0_0_heads0_f0,buf_out[0],enable);
    Acc5_receive_buff_type0<1>(Acc5_plio0_1_heads0_f0,buf_out[1],enable);
    Acc5_receive_buff_type0<2>(Acc5_plio0_2_heads0_f0,buf_out[2],enable);
    Acc5_receive_buff_type0<3>(Acc5_plio0_3_heads0_f0,buf_out[3],enable);
    
}

template<int NC>
void Acc6_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf_lhs0_0[ACC6_M0/BUFF6_LHS_PARTA/NUM_PER_64][ACC6_K0/BUFF6_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc6_txA0, axis_stream_plio_128& Acc6_txA1, axis_stream_plio_128& Acc6_txA2, 
                                      const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC6_M0/ACC6_A/ACC6_H1); x++) { 
            for (int z = 0; z < (ACC6_N0/ACC6_C/ACC6_W2); z++) {
                for (int y = 0; y < (ACC6_K0/ACC6_B/ACC6_W1); y++) {
                    for (int h1 = 0; h1 < ACC6_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC6_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC6_LW1);
                            int pos1=(h1+x*ACC6_LH1);

                            data = buf_lhs0_0[pos1][pos0];
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc6_txA0.write(tmp);
                            Acc6_txA1.write(tmp);
                            Acc6_txA2.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc6_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_lhs[BUFF6_LHS_PARTA*BUFF6_LHS_PARTB*ACC6_HEADS][ACC6_M0/BUFF6_LHS_PARTA/NUM_PER_64][ACC6_K0/BUFF6_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc6_txA0_0_heads0, axis_stream_plio_128& Acc6_txA0_1_heads0, axis_stream_plio_128& Acc6_txA0_2_heads0, axis_stream_plio_128& Acc6_txA0_3_heads0,  
                                          axis_stream_plio_128& Acc6_txA1_0_heads0, axis_stream_plio_128& Acc6_txA1_1_heads0, axis_stream_plio_128& Acc6_txA1_2_heads0, axis_stream_plio_128& Acc6_txA1_3_heads0,  
                                          axis_stream_plio_128& Acc6_txA2_0_heads0, axis_stream_plio_128& Acc6_txA2_1_heads0, axis_stream_plio_128& Acc6_txA2_2_heads0, axis_stream_plio_128& Acc6_txA2_3_heads0,  
                                          const bool enable){
    Acc6_send_LHS_type0<0>(buf_lhs[0],Acc6_txA0_0_heads0,Acc6_txA1_0_heads0,Acc6_txA2_0_heads0,enable);
    Acc6_send_LHS_type0<1>(buf_lhs[1],Acc6_txA0_1_heads0,Acc6_txA1_1_heads0,Acc6_txA2_1_heads0,enable);
    Acc6_send_LHS_type0<2>(buf_lhs[2],Acc6_txA0_2_heads0,Acc6_txA1_2_heads0,Acc6_txA2_2_heads0,enable);
    Acc6_send_LHS_type0<3>(buf_lhs[3],Acc6_txA0_3_heads0,Acc6_txA1_3_heads0,Acc6_txA2_3_heads0,enable);
      
}




template<int NC>
void Acc6_receive_OUT(axis_stream_plio_128& Acc6_rxC,axis_stream_128& Acc6_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC6_M0/ACC6_A/ACC6_H1); x++) { 
            for (int z = 0; z < (ACC6_N0/ACC6_C/ACC6_W2); z++) {
                for (int y = 0; y < (ACC6_K0/ACC6_B/ACC6_W1); y++) {
                    for (int h1 = 0; h1 < ACC6_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC6_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc6_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc6_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc6_receive_buff_type0(axis_stream_128& Acc6_sin0,ap_uint<PLIO_WIDTH_128> buf_out0_0[ACC6_M0/BUFF6_OUT_PARTA/NUM_PER_64][ACC6_N0/BUFF6_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        for (int x = 0; x < (ACC6_M0/ACC6_A/ACC6_H1); x++) { 
            for (int z = 0; z < (ACC6_N0/ACC6_C/ACC6_W2); z++) {          
                for (int h1 = 0; h1 < ACC6_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC6_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc6_sin0.read();
                        
                        int pos0=(w2+z*ACC6_RW2);
                        int pos1=(h1+x*ACC6_LH1);
                        buf_out0_0[pos1][pos0] = data_temp;
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc6_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf_out[BUFF6_OUT_PARTA*BUFF6_OUT_PARTC*ACC6_HEADS][ACC6_M0/BUFF6_OUT_PARTA/NUM_PER_64][ACC6_N0/BUFF6_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc6_rxC0_0_heads0, axis_stream_plio_128& Acc6_rxC0_1_heads0, axis_stream_plio_128& Acc6_rxC0_2_heads0, axis_stream_plio_128& Acc6_rxC0_3_heads0, axis_stream_plio_128& Acc6_rxC0_4_heads0, axis_stream_plio_128& Acc6_rxC0_5_heads0, axis_stream_plio_128& Acc6_rxC0_6_heads0, axis_stream_plio_128& Acc6_rxC0_7_heads0, axis_stream_plio_128& Acc6_rxC0_8_heads0, axis_stream_plio_128& Acc6_rxC0_9_heads0, axis_stream_plio_128& Acc6_rxC0_10_heads0, axis_stream_plio_128& Acc6_rxC0_11_heads0, axis_stream_plio_128& Acc6_rxC0_12_heads0, axis_stream_plio_128& Acc6_rxC0_13_heads0, axis_stream_plio_128& Acc6_rxC0_14_heads0, axis_stream_plio_128& Acc6_rxC0_15_heads0, axis_stream_plio_128& Acc6_rxC0_16_heads0, axis_stream_plio_128& Acc6_rxC0_17_heads0, axis_stream_plio_128& Acc6_rxC0_18_heads0, axis_stream_plio_128& Acc6_rxC0_19_heads0, axis_stream_plio_128& Acc6_rxC0_20_heads0, axis_stream_plio_128& Acc6_rxC0_21_heads0, axis_stream_plio_128& Acc6_rxC0_22_heads0, axis_stream_plio_128& Acc6_rxC0_23_heads0,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc6_plio0_0_heads0_f0;axis_stream_128 Acc6_plio0_1_heads0_f0;axis_stream_128 Acc6_plio0_2_heads0_f0;axis_stream_128 Acc6_plio0_3_heads0_f0;axis_stream_128 Acc6_plio0_4_heads0_f0;axis_stream_128 Acc6_plio0_5_heads0_f0;axis_stream_128 Acc6_plio0_6_heads0_f0;axis_stream_128 Acc6_plio0_7_heads0_f0;axis_stream_128 Acc6_plio0_8_heads0_f0;axis_stream_128 Acc6_plio0_9_heads0_f0;axis_stream_128 Acc6_plio0_10_heads0_f0;axis_stream_128 Acc6_plio0_11_heads0_f0;axis_stream_128 Acc6_plio0_12_heads0_f0;axis_stream_128 Acc6_plio0_13_heads0_f0;axis_stream_128 Acc6_plio0_14_heads0_f0;axis_stream_128 Acc6_plio0_15_heads0_f0;axis_stream_128 Acc6_plio0_16_heads0_f0;axis_stream_128 Acc6_plio0_17_heads0_f0;axis_stream_128 Acc6_plio0_18_heads0_f0;axis_stream_128 Acc6_plio0_19_heads0_f0;axis_stream_128 Acc6_plio0_20_heads0_f0;axis_stream_128 Acc6_plio0_21_heads0_f0;axis_stream_128 Acc6_plio0_22_heads0_f0;axis_stream_128 Acc6_plio0_23_heads0_f0; 
    
    Acc6_receive_OUT<0>(Acc6_rxC0_0_heads0, Acc6_plio0_0_heads0_f0, enable); 
    Acc6_receive_OUT<1>(Acc6_rxC0_1_heads0, Acc6_plio0_1_heads0_f0, enable); 
    Acc6_receive_OUT<2>(Acc6_rxC0_2_heads0, Acc6_plio0_2_heads0_f0, enable); 
    Acc6_receive_OUT<3>(Acc6_rxC0_3_heads0, Acc6_plio0_3_heads0_f0, enable); 
    Acc6_receive_OUT<4>(Acc6_rxC0_4_heads0, Acc6_plio0_4_heads0_f0, enable); 
    Acc6_receive_OUT<5>(Acc6_rxC0_5_heads0, Acc6_plio0_5_heads0_f0, enable); 
    Acc6_receive_OUT<6>(Acc6_rxC0_6_heads0, Acc6_plio0_6_heads0_f0, enable); 
    Acc6_receive_OUT<7>(Acc6_rxC0_7_heads0, Acc6_plio0_7_heads0_f0, enable); 
    Acc6_receive_OUT<8>(Acc6_rxC0_8_heads0, Acc6_plio0_8_heads0_f0, enable); 
    Acc6_receive_OUT<9>(Acc6_rxC0_9_heads0, Acc6_plio0_9_heads0_f0, enable); 
    Acc6_receive_OUT<10>(Acc6_rxC0_10_heads0, Acc6_plio0_10_heads0_f0, enable); 
    Acc6_receive_OUT<11>(Acc6_rxC0_11_heads0, Acc6_plio0_11_heads0_f0, enable); 
    Acc6_receive_OUT<12>(Acc6_rxC0_12_heads0, Acc6_plio0_12_heads0_f0, enable); 
    Acc6_receive_OUT<13>(Acc6_rxC0_13_heads0, Acc6_plio0_13_heads0_f0, enable); 
    Acc6_receive_OUT<14>(Acc6_rxC0_14_heads0, Acc6_plio0_14_heads0_f0, enable); 
    Acc6_receive_OUT<15>(Acc6_rxC0_15_heads0, Acc6_plio0_15_heads0_f0, enable); 
    Acc6_receive_OUT<16>(Acc6_rxC0_16_heads0, Acc6_plio0_16_heads0_f0, enable); 
    Acc6_receive_OUT<17>(Acc6_rxC0_17_heads0, Acc6_plio0_17_heads0_f0, enable); 
    Acc6_receive_OUT<18>(Acc6_rxC0_18_heads0, Acc6_plio0_18_heads0_f0, enable); 
    Acc6_receive_OUT<19>(Acc6_rxC0_19_heads0, Acc6_plio0_19_heads0_f0, enable); 
    Acc6_receive_OUT<20>(Acc6_rxC0_20_heads0, Acc6_plio0_20_heads0_f0, enable); 
    Acc6_receive_OUT<21>(Acc6_rxC0_21_heads0, Acc6_plio0_21_heads0_f0, enable); 
    Acc6_receive_OUT<22>(Acc6_rxC0_22_heads0, Acc6_plio0_22_heads0_f0, enable); 
    Acc6_receive_OUT<23>(Acc6_rxC0_23_heads0, Acc6_plio0_23_heads0_f0, enable); 
    
    
    Acc6_receive_buff_type0<0>(Acc6_plio0_0_heads0_f0,buf_out[0],enable);
    Acc6_receive_buff_type0<1>(Acc6_plio0_1_heads0_f0,buf_out[1],enable);
    Acc6_receive_buff_type0<2>(Acc6_plio0_2_heads0_f0,buf_out[2],enable);
    Acc6_receive_buff_type0<3>(Acc6_plio0_3_heads0_f0,buf_out[3],enable);
    Acc6_receive_buff_type0<4>(Acc6_plio0_4_heads0_f0,buf_out[4],enable);
    Acc6_receive_buff_type0<5>(Acc6_plio0_5_heads0_f0,buf_out[5],enable);
    Acc6_receive_buff_type0<6>(Acc6_plio0_6_heads0_f0,buf_out[6],enable);
    Acc6_receive_buff_type0<7>(Acc6_plio0_7_heads0_f0,buf_out[7],enable);
    Acc6_receive_buff_type0<8>(Acc6_plio0_8_heads0_f0,buf_out[8],enable);
    Acc6_receive_buff_type0<9>(Acc6_plio0_9_heads0_f0,buf_out[9],enable);
    Acc6_receive_buff_type0<10>(Acc6_plio0_10_heads0_f0,buf_out[10],enable);
    Acc6_receive_buff_type0<11>(Acc6_plio0_11_heads0_f0,buf_out[11],enable);
    Acc6_receive_buff_type0<12>(Acc6_plio0_12_heads0_f0,buf_out[12],enable);
    Acc6_receive_buff_type0<13>(Acc6_plio0_13_heads0_f0,buf_out[13],enable);
    Acc6_receive_buff_type0<14>(Acc6_plio0_14_heads0_f0,buf_out[14],enable);
    Acc6_receive_buff_type0<15>(Acc6_plio0_15_heads0_f0,buf_out[15],enable);
    Acc6_receive_buff_type0<16>(Acc6_plio0_16_heads0_f0,buf_out[16],enable);
    Acc6_receive_buff_type0<17>(Acc6_plio0_17_heads0_f0,buf_out[17],enable);
    Acc6_receive_buff_type0<18>(Acc6_plio0_18_heads0_f0,buf_out[18],enable);
    Acc6_receive_buff_type0<19>(Acc6_plio0_19_heads0_f0,buf_out[19],enable);
    Acc6_receive_buff_type0<20>(Acc6_plio0_20_heads0_f0,buf_out[20],enable);
    Acc6_receive_buff_type0<21>(Acc6_plio0_21_heads0_f0,buf_out[21],enable);
    Acc6_receive_buff_type0<22>(Acc6_plio0_22_heads0_f0,buf_out[22],enable);
    Acc6_receive_buff_type0<23>(Acc6_plio0_23_heads0_f0,buf_out[23],enable);
    
}

template<int NC>
void Acc7_send_LHS_type0(ap_uint<PLIO_WIDTH_128> buf_lhs0_0[ACC7_M0/BUFF7_LHS_PARTA/NUM_PER_64][ACC7_K0/BUFF7_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                                      axis_stream_plio_128& Acc7_txA0, 
                                      const bool enable){

#pragma HLS inline off

    if(enable){
        data_plio_128 data;
        axis_pkt_128 tmp;
        for (int x = 0; x < (ACC7_M0/ACC7_A/ACC7_H1); x++) { 
            for (int z = 0; z < (ACC7_N0/ACC7_C/ACC7_W2); z++) {
                for (int y = 0; y < (ACC7_K0/ACC7_B/ACC7_W1); y++) {
                    for (int h1 = 0; h1 < ACC7_LH1; h1++) {
                        for (int w1 = 0; w1 < ACC7_LW1; w1++) {
                        #pragma HLS PIPELINE II = 1
                            int pos0=(w1+y*ACC7_LW1);
                            int pos1=(h1+x*ACC7_LH1);

                            data = buf_lhs0_0[pos1][pos0];
                            
                            tmp.data  =  data;
                            tmp.keep  =  -1;
                            Acc7_txA0.write(tmp);
                            
                        }
                    }
                }
            }   
        }
    }
}

template<int NC>
void Acc7_send_LHS_type0_top(ap_uint<PLIO_WIDTH_128> buf_lhs[BUFF7_LHS_PARTA*BUFF7_LHS_PARTB*ACC7_HEADS][ACC7_M0/BUFF7_LHS_PARTA/NUM_PER_64][ACC7_K0/BUFF7_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], 
                                          axis_stream_plio_128& Acc7_txA0_0_heads0, axis_stream_plio_128& Acc7_txA0_1_heads0, axis_stream_plio_128& Acc7_txA0_2_heads0, axis_stream_plio_128& Acc7_txA0_3_heads0, axis_stream_plio_128& Acc7_txA0_4_heads0, axis_stream_plio_128& Acc7_txA0_5_heads0, axis_stream_plio_128& Acc7_txA0_6_heads0, axis_stream_plio_128& Acc7_txA0_7_heads0, axis_stream_plio_128& Acc7_txA0_8_heads0, axis_stream_plio_128& Acc7_txA0_9_heads0, axis_stream_plio_128& Acc7_txA0_10_heads0, axis_stream_plio_128& Acc7_txA0_11_heads0, axis_stream_plio_128& Acc7_txA0_12_heads0, axis_stream_plio_128& Acc7_txA0_13_heads0, axis_stream_plio_128& Acc7_txA0_14_heads0, axis_stream_plio_128& Acc7_txA0_15_heads0, axis_stream_plio_128& Acc7_txA0_16_heads0, axis_stream_plio_128& Acc7_txA0_17_heads0, axis_stream_plio_128& Acc7_txA0_18_heads0, axis_stream_plio_128& Acc7_txA0_19_heads0, axis_stream_plio_128& Acc7_txA0_20_heads0, axis_stream_plio_128& Acc7_txA0_21_heads0, axis_stream_plio_128& Acc7_txA0_22_heads0, axis_stream_plio_128& Acc7_txA0_23_heads0,  
                                          const bool enable){
    Acc7_send_LHS_type0<0>(buf_lhs[0],Acc7_txA0_0_heads0,enable);
    Acc7_send_LHS_type0<1>(buf_lhs[1],Acc7_txA0_1_heads0,enable);
    Acc7_send_LHS_type0<2>(buf_lhs[2],Acc7_txA0_2_heads0,enable);
    Acc7_send_LHS_type0<3>(buf_lhs[3],Acc7_txA0_3_heads0,enable);
    Acc7_send_LHS_type0<4>(buf_lhs[4],Acc7_txA0_4_heads0,enable);
    Acc7_send_LHS_type0<5>(buf_lhs[5],Acc7_txA0_5_heads0,enable);
    Acc7_send_LHS_type0<6>(buf_lhs[6],Acc7_txA0_6_heads0,enable);
    Acc7_send_LHS_type0<7>(buf_lhs[7],Acc7_txA0_7_heads0,enable);
    Acc7_send_LHS_type0<8>(buf_lhs[8],Acc7_txA0_8_heads0,enable);
    Acc7_send_LHS_type0<9>(buf_lhs[9],Acc7_txA0_9_heads0,enable);
    Acc7_send_LHS_type0<10>(buf_lhs[10],Acc7_txA0_10_heads0,enable);
    Acc7_send_LHS_type0<11>(buf_lhs[11],Acc7_txA0_11_heads0,enable);
    Acc7_send_LHS_type0<12>(buf_lhs[12],Acc7_txA0_12_heads0,enable);
    Acc7_send_LHS_type0<13>(buf_lhs[13],Acc7_txA0_13_heads0,enable);
    Acc7_send_LHS_type0<14>(buf_lhs[14],Acc7_txA0_14_heads0,enable);
    Acc7_send_LHS_type0<15>(buf_lhs[15],Acc7_txA0_15_heads0,enable);
    Acc7_send_LHS_type0<16>(buf_lhs[16],Acc7_txA0_16_heads0,enable);
    Acc7_send_LHS_type0<17>(buf_lhs[17],Acc7_txA0_17_heads0,enable);
    Acc7_send_LHS_type0<18>(buf_lhs[18],Acc7_txA0_18_heads0,enable);
    Acc7_send_LHS_type0<19>(buf_lhs[19],Acc7_txA0_19_heads0,enable);
    Acc7_send_LHS_type0<20>(buf_lhs[20],Acc7_txA0_20_heads0,enable);
    Acc7_send_LHS_type0<21>(buf_lhs[21],Acc7_txA0_21_heads0,enable);
    Acc7_send_LHS_type0<22>(buf_lhs[22],Acc7_txA0_22_heads0,enable);
    Acc7_send_LHS_type0<23>(buf_lhs[23],Acc7_txA0_23_heads0,enable);
      
}




template<int NC>
void Acc7_receive_OUT(axis_stream_plio_128& Acc7_rxC,axis_stream_128& Acc7_sout0,const bool enable){

#pragma HLS inline off
    if(enable){
        ap_int<8>  data_temp0[NUM_PER_128];
        #pragma HLS ARRAY_PARTITION variable=data_temp0 complete dim=0

        data_plio_128  data_temp;
        axis_pkt_128 tmp;

        for (int x = 0; x < (ACC7_M0/ACC7_A/ACC7_H1); x++) { 
            for (int z = 0; z < (ACC7_N0/ACC7_C/ACC7_W2); z++) {
                for (int y = 0; y < (ACC7_K0/ACC7_B/ACC7_W1); y++) {
                    for (int h1 = 0; h1 < ACC7_LH1; h1++) {
                        for (int w2 = 0; w2 < ACC7_RW2; w2++) {
                        #pragma HLS PIPELINE II = 1
                        
                            tmp   =  Acc7_rxC.read();
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp0[un]   =  tmp.data(un*8+7,un*8);
                            }
                            
                            for (int un=0; un<NUM_PER_128; un++){
                            #pragma HLS UNROLL factor=NUM_PER_128
                                data_temp(un*8+7,un*8) = data_temp0[un];
                            }
                            
                            Acc7_sout0.write(data_temp);
                            
                            
                        }
                    }
                }  
            }
        }
    }
}

template<int NC>
void Acc7_receive_buff_type0(axis_stream_128& Acc7_sin0,ap_uint<PLIO_WIDTH_128> buf0_out0_0[ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          ap_uint<PLIO_WIDTH_128> buf1_out0_0[ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                                          const bool enable){
#pragma HLS inline off
    if(enable){
        
        data_plio_128  data_temp;

        for (int x = 0; x < (ACC7_M0/ACC7_A/ACC7_H1); x++) { 
            for (int z = 0; z < (ACC7_N0/ACC7_C/ACC7_W2); z++) {          
                for (int h1 = 0; h1 < ACC7_LH1; h1++) {
                    for (int w2 = 0; w2 < ACC7_RW2; w2++) {
                    #pragma HLS PIPELINE II = 1
                        data_temp = Acc7_sin0.read();
                        
                        int pos0=(w2+z*ACC7_RW2);
                        int pos1=(h1+x*ACC7_LH1);
                        buf0_out0_0[pos1][pos0] = data_temp;
                        buf1_out0_0[pos1][pos0] = data_temp;
                        
                    }
                } 
            }
        }
    }
}

template<int NC>
void Acc7_receive_OUT_type0_top(ap_uint<PLIO_WIDTH_128> buf0_out[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)],
            ap_uint<PLIO_WIDTH_128> buf1_out[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
           axis_stream_plio_128& Acc7_rxC0_0_heads0, axis_stream_plio_128& Acc7_rxC0_1_heads0, axis_stream_plio_128& Acc7_rxC0_2_heads0, axis_stream_plio_128& Acc7_rxC0_3_heads0,  
           const bool enable){

#pragma HLS dataflow
    
    axis_stream_128 Acc7_plio0_0_heads0_f0;axis_stream_128 Acc7_plio0_1_heads0_f0;axis_stream_128 Acc7_plio0_2_heads0_f0;axis_stream_128 Acc7_plio0_3_heads0_f0; 
    
    Acc7_receive_OUT<0>(Acc7_rxC0_0_heads0, Acc7_plio0_0_heads0_f0, enable); 
    Acc7_receive_OUT<1>(Acc7_rxC0_1_heads0, Acc7_plio0_1_heads0_f0, enable); 
    Acc7_receive_OUT<2>(Acc7_rxC0_2_heads0, Acc7_plio0_2_heads0_f0, enable); 
    Acc7_receive_OUT<3>(Acc7_rxC0_3_heads0, Acc7_plio0_3_heads0_f0, enable); 
    
    
    Acc7_receive_buff_type0<0>(Acc7_plio0_0_heads0_f0,buf0_out[0],buf1_out[0],enable);
    Acc7_receive_buff_type0<1>(Acc7_plio0_1_heads0_f0,buf0_out[1],buf1_out[1],enable);
    Acc7_receive_buff_type0<2>(Acc7_plio0_2_heads0_f0,buf0_out[2],buf1_out[2],enable);
    Acc7_receive_buff_type0<3>(Acc7_plio0_3_heads0_f0,buf0_out[3],buf1_out[3],enable);
    
}

template<int NC>
void m2vector(ap_uint<PLIO_WIDTH_128> buff0[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)],ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64],const bool enable){
#pragma HLS inline off

    if (enable){
        data_plio_64 temp_out0;
        data_plio_64 temp_out1;
        data_plio_128 data0;
        data_plio_128 data1;
        ap_uint<8> temp_data[2][8];
        #pragma HLS ARRAY_PARTITION variable=temp_data complete dim=1
        #pragma HLS ARRAY_PARTITION variable=temp_data complete dim=2

        for (int p=0;p<BUFF7_OUT_PARTC/FC_B;p++){
            for (int j=0; j<(FC_K/BUFF7_OUT_PARTC/NUM_PER_64);j++){
                for (int i=0; i < 4;i++){
                #pragma HLS PIPELINE II = 1
                    int pos0= i + j * 4;
                    data0 = buff0[p+(BUFF7_OUT_PARTC*(BUFF7_OUT_PARTA-1))][(ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64-1)][pos0];
                    data1 = buff0[p+(BUFF7_OUT_PARTC*(BUFF7_OUT_PARTA-1)+(BUFF7_OUT_PARTC/FC_B))][(ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64-1)][pos0];
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
void Acc7_m2vector(ap_uint<PLIO_WIDTH_128> buf0_in[BUFF7_LHS_PARTA*BUFF7_LHS_PARTB*ACC7_HEADS][ACC7_M0/BUFF7_LHS_PARTA/NUM_PER_64][ACC7_K0/BUFF7_LHS_PARTB/(NUM_PER_128/NUM_PER_64)],
                         ap_uint<PLIO_WIDTH_128> buf0_out[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)], 
                        axis_stream_plio_128& Acc7_txA0_0_heads0, axis_stream_plio_128& Acc7_txA0_1_heads0, axis_stream_plio_128& Acc7_txA0_2_heads0, axis_stream_plio_128& Acc7_txA0_3_heads0, axis_stream_plio_128& Acc7_txA0_4_heads0, axis_stream_plio_128& Acc7_txA0_5_heads0, axis_stream_plio_128& Acc7_txA0_6_heads0, axis_stream_plio_128& Acc7_txA0_7_heads0, axis_stream_plio_128& Acc7_txA0_8_heads0, axis_stream_plio_128& Acc7_txA0_9_heads0, axis_stream_plio_128& Acc7_txA0_10_heads0, axis_stream_plio_128& Acc7_txA0_11_heads0, axis_stream_plio_128& Acc7_txA0_12_heads0, axis_stream_plio_128& Acc7_txA0_13_heads0, axis_stream_plio_128& Acc7_txA0_14_heads0, axis_stream_plio_128& Acc7_txA0_15_heads0, axis_stream_plio_128& Acc7_txA0_16_heads0, axis_stream_plio_128& Acc7_txA0_17_heads0, axis_stream_plio_128& Acc7_txA0_18_heads0, axis_stream_plio_128& Acc7_txA0_19_heads0, axis_stream_plio_128& Acc7_txA0_20_heads0, axis_stream_plio_128& Acc7_txA0_21_heads0, axis_stream_plio_128& Acc7_txA0_22_heads0, axis_stream_plio_128& Acc7_txA0_23_heads0,  
                        axis_stream_plio_128& Acc7_rxC0_0_heads0, axis_stream_plio_128& Acc7_rxC0_1_heads0, axis_stream_plio_128& Acc7_rxC0_2_heads0, axis_stream_plio_128& Acc7_rxC0_3_heads0,  
                        ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64], const bool enable_0,const bool enable_1){
    
    ap_uint<PLIO_WIDTH_128> buf1_out[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buf1_out type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buf1_out complete dim=1
    
    for(int i=0 ; i < 2; i++){
        if(i==0){
            Acc7_send_LHS_type0_top<0>(buf0_in, Acc7_txA0_0_heads0, Acc7_txA0_1_heads0, Acc7_txA0_2_heads0, Acc7_txA0_3_heads0, Acc7_txA0_4_heads0, Acc7_txA0_5_heads0, Acc7_txA0_6_heads0, Acc7_txA0_7_heads0, Acc7_txA0_8_heads0, Acc7_txA0_9_heads0, Acc7_txA0_10_heads0, Acc7_txA0_11_heads0, Acc7_txA0_12_heads0, Acc7_txA0_13_heads0, Acc7_txA0_14_heads0, Acc7_txA0_15_heads0, Acc7_txA0_16_heads0, Acc7_txA0_17_heads0, Acc7_txA0_18_heads0, Acc7_txA0_19_heads0, Acc7_txA0_20_heads0, Acc7_txA0_21_heads0, Acc7_txA0_22_heads0, Acc7_txA0_23_heads0,  enable_0);
            Acc7_receive_OUT_type0_top<0>(buf0_out, buf1_out ,Acc7_rxC0_0_heads0, Acc7_rxC0_1_heads0, Acc7_rxC0_2_heads0, Acc7_rxC0_3_heads0, enable_0);
        }
        else{
            m2vector<0>(buf1_out, buff_fc0, enable_1);
        }
    }
}



template<int NC>
void send_patch_wrapper1(ap_uint<PLIO_WIDTH_128> a_buf[PE_KHP][PE_KH][PE_SEQ_ALL],axis_stream_plio_128& txA0,axis_stream_plio_128& txA1,axis_stream_plio_128& txA2,axis_stream_plio_128& txA3,const bool enable){
    send_patch_wrapper0<0>(a_buf[0], txA0, enable);
    send_patch_wrapper0<1>(a_buf[1], txA1, enable);
    send_patch_wrapper0<2>(a_buf[2], txA2, enable);
    send_patch_wrapper0<3>(a_buf[3], txA3, enable);
    
}

template<int NC>
void send_patch_top(ap_uint<PLIO_WIDTH_128> a_buf[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL],
                    axis_stream_plio_128& txA0, axis_stream_plio_128& txA1, axis_stream_plio_128& txA2, axis_stream_plio_128& txA3, 
                    axis_stream_plio_128& txA4, axis_stream_plio_128& txA5, axis_stream_plio_128& txA6, axis_stream_plio_128& txA7, 
                    axis_stream_plio_128& txA8, axis_stream_plio_128& txA9, axis_stream_plio_128& txA10, axis_stream_plio_128& txA11, const bool enable){
    send_patch_wrapper1<0>(a_buf[0],txA0,txA1,txA2,txA3,enable);
    send_patch_wrapper1<1>(a_buf[1],txA4,txA5,txA6,txA7,enable);
    send_patch_wrapper1<2>(a_buf[2],txA8,txA9,txA10,txA11,enable);
    
}

template<int NC>
void load_image_top(axis_stream_A& dataA_in0, axis_stream_A& dataA_in1, axis_stream_A& dataA_in2, ap_uint<PLIO_WIDTH_128> a_buf[PE_INC][PE_KHP][PE_KH][PE_SEQ_ALL],const bool enable){
    load_image<0>(dataA_in0, a_buf[0], enable);
    load_image<1>(dataA_in1, a_buf[1], enable);
    load_image<2>(dataA_in2, a_buf[2], enable);
}

template<int NC>
void receive_Patch_buff(axis_stream_128& s_in0,
                        const bool enable){
#pragma HLS inline off
    if(enable){
        data_plio_128 data_temp;
        for (int h1 = 0; h1 < (PE_SEQ_ALL/NUM_PER_64); h1++) {
            for (int w2 = 0; w2 < (PE_OC/2); w2++) {
            #pragma HLS PIPELINE II = 1
                data_temp=s_in0.read();
                
            }
        }
    }
}

template<int NC>
void receive_Patch_top(axis_stream_plio_128& rxC0,axis_stream_plio_128& rxC1,axis_stream_plio_128& rxC2,axis_stream_plio_128& rxC3,ap_uint<PLIO_WIDTH_128> buff0[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB][(ACC0_M0/BUFF0_LHS_PARTA/NUM_PER_64)][ACC0_K0/BUFF0_LHS_PARTB/(NUM_PER_128/NUM_PER_64)], const bool enable){
#pragma HLS dataflow
    
    
    axis_stream_128 s_out0;
    axis_stream_128 s_out1;
    axis_stream_128 s_out2;
    axis_stream_128 s_out3;
    
    receive_Patch_transpose<0>(rxC0, s_out0, enable);
    receive_Patch_buff<0>(s_out0,enable);
    
    receive_Patch_transpose<1>(rxC1, s_out1, enable);
    receive_Patch_buff<1>(s_out1,enable);
    
    receive_Patch_transpose<2>(rxC2, s_out2, enable);
    receive_Patch_buff<2>(s_out2,enable);
    
    receive_Patch_transpose<3>(rxC3, s_out3, enable);
    receive_Patch_buff<3>(s_out3,enable);
    
    
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
                     axis_stream_plio_128& Acc0_rxC0_0_heads0, axis_stream_plio_128& Acc0_rxC0_1_heads0, axis_stream_plio_128& Acc0_rxC0_2_heads0, axis_stream_plio_128& Acc0_rxC0_3_heads0, axis_stream_plio_128& Acc0_rxC0_4_heads0, axis_stream_plio_128& Acc0_rxC0_5_heads0,  
                     axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0, axis_stream_plio_128& Acc1_txA0_2_heads0, axis_stream_plio_128& Acc1_txA0_3_heads0,  
                     axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0, axis_stream_plio_128& Acc1_rxC0_2_heads0, axis_stream_plio_128& Acc1_rxC0_3_heads0, axis_stream_plio_128& Acc1_rxC0_4_heads0, axis_stream_plio_128& Acc1_rxC0_5_heads0,  
                     axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0,  
                     axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
                     axis_stream_plio_128& Acc3_txA0_0_heads0, axis_stream_plio_128& Acc3_txA0_1_heads0,  
                     axis_stream_plio_128& Acc3_txA1_0_heads0, axis_stream_plio_128& Acc3_txA1_1_heads0,  
                     axis_stream_plio_128& Acc3_txA0_0_heads1, axis_stream_plio_128& Acc3_txA0_1_heads1,  
                     axis_stream_plio_128& Acc3_txA1_0_heads1, axis_stream_plio_128& Acc3_txA1_1_heads1,  
                     axis_stream_plio_128& Acc3_txA0_0_heads2, axis_stream_plio_128& Acc3_txA0_1_heads2,  
                     axis_stream_plio_128& Acc3_txA1_0_heads2, axis_stream_plio_128& Acc3_txA1_1_heads2,  
                     axis_stream_plio_128& Acc3_txB0_0_heads0, axis_stream_plio_128& Acc3_txB1_0_heads0,  
                     axis_stream_plio_128& Acc3_txB0_1_heads0, axis_stream_plio_128& Acc3_txB1_1_heads0,  
                     axis_stream_plio_128& Acc3_txB0_0_heads1, axis_stream_plio_128& Acc3_txB1_0_heads1,  
                     axis_stream_plio_128& Acc3_txB0_1_heads1, axis_stream_plio_128& Acc3_txB1_1_heads1,  
                     axis_stream_plio_128& Acc3_txB0_0_heads2, axis_stream_plio_128& Acc3_txB1_0_heads2,  
                     axis_stream_plio_128& Acc3_txB0_1_heads2, axis_stream_plio_128& Acc3_txB1_1_heads2,  
                     axis_stream_plio_128& Acc3_rxC0_0_heads0, axis_stream_plio_128& Acc3_rxC0_1_heads0,  
                     axis_stream_plio_128& Acc3_rxC1_0_heads0, axis_stream_plio_128& Acc3_rxC1_1_heads0,  
                     axis_stream_plio_128& Acc3_rxC0_0_heads1, axis_stream_plio_128& Acc3_rxC0_1_heads1,  
                     axis_stream_plio_128& Acc3_rxC1_0_heads1, axis_stream_plio_128& Acc3_rxC1_1_heads1,  
                     axis_stream_plio_128& Acc3_rxC0_0_heads2, axis_stream_plio_128& Acc3_rxC0_1_heads2,  
                     axis_stream_plio_128& Acc3_rxC1_0_heads2, axis_stream_plio_128& Acc3_rxC1_1_heads2,  
                     axis_stream_plio_128& Acc4_txA0_0_heads0, axis_stream_plio_128& Acc4_txA0_1_heads0,  
                     axis_stream_plio_128& Acc4_txA1_0_heads0, axis_stream_plio_128& Acc4_txA1_1_heads0,  
                     axis_stream_plio_128& Acc4_txA0_0_heads1, axis_stream_plio_128& Acc4_txA0_1_heads1,  
                     axis_stream_plio_128& Acc4_txA1_0_heads1, axis_stream_plio_128& Acc4_txA1_1_heads1,  
                     axis_stream_plio_128& Acc4_txA0_0_heads2, axis_stream_plio_128& Acc4_txA0_1_heads2,  
                     axis_stream_plio_128& Acc4_txA1_0_heads2, axis_stream_plio_128& Acc4_txA1_1_heads2,  
                     axis_stream_plio_128& Acc4_txB0_0_heads0, axis_stream_plio_128& Acc4_txB1_0_heads0,  
                     axis_stream_plio_128& Acc4_txB0_1_heads0, axis_stream_plio_128& Acc4_txB1_1_heads0,  
                     axis_stream_plio_128& Acc4_txB0_0_heads1, axis_stream_plio_128& Acc4_txB1_0_heads1,  
                     axis_stream_plio_128& Acc4_txB0_1_heads1, axis_stream_plio_128& Acc4_txB1_1_heads1,  
                     axis_stream_plio_128& Acc4_txB0_0_heads2, axis_stream_plio_128& Acc4_txB1_0_heads2,  
                     axis_stream_plio_128& Acc4_txB0_1_heads2, axis_stream_plio_128& Acc4_txB1_1_heads2,  
                     axis_stream_plio_128& Acc4_rxC0_0_heads0, axis_stream_plio_128& Acc4_rxC0_1_heads0,  
                     axis_stream_plio_128& Acc4_rxC1_0_heads0, axis_stream_plio_128& Acc4_rxC1_1_heads0,  
                     axis_stream_plio_128& Acc4_rxC0_0_heads1, axis_stream_plio_128& Acc4_rxC0_1_heads1,  
                     axis_stream_plio_128& Acc4_rxC1_0_heads1, axis_stream_plio_128& Acc4_rxC1_1_heads1,  
                     axis_stream_plio_128& Acc4_rxC0_0_heads2, axis_stream_plio_128& Acc4_rxC0_1_heads2,  
                     axis_stream_plio_128& Acc4_rxC1_0_heads2, axis_stream_plio_128& Acc4_rxC1_1_heads2,  
                     axis_stream_plio_128& Acc5_txA0_0_heads0, axis_stream_plio_128& Acc5_txA0_1_heads0, axis_stream_plio_128& Acc5_txA0_2_heads0, axis_stream_plio_128& Acc5_txA0_3_heads0, axis_stream_plio_128& Acc5_txA0_4_heads0, axis_stream_plio_128& Acc5_txA0_5_heads0,  
                     axis_stream_plio_128& Acc5_rxC0_0_heads0, axis_stream_plio_128& Acc5_rxC0_1_heads0, axis_stream_plio_128& Acc5_rxC0_2_heads0, axis_stream_plio_128& Acc5_rxC0_3_heads0,  
                     axis_stream_plio_128& Acc6_txA0_0_heads0, axis_stream_plio_128& Acc6_txA0_1_heads0, axis_stream_plio_128& Acc6_txA0_2_heads0, axis_stream_plio_128& Acc6_txA0_3_heads0,  
                     axis_stream_plio_128& Acc6_txA1_0_heads0, axis_stream_plio_128& Acc6_txA1_1_heads0, axis_stream_plio_128& Acc6_txA1_2_heads0, axis_stream_plio_128& Acc6_txA1_3_heads0,  
                     axis_stream_plio_128& Acc6_txA2_0_heads0, axis_stream_plio_128& Acc6_txA2_1_heads0, axis_stream_plio_128& Acc6_txA2_2_heads0, axis_stream_plio_128& Acc6_txA2_3_heads0,  
                     axis_stream_plio_128& Acc6_rxC0_0_heads0, axis_stream_plio_128& Acc6_rxC0_1_heads0, axis_stream_plio_128& Acc6_rxC0_2_heads0, axis_stream_plio_128& Acc6_rxC0_3_heads0, axis_stream_plio_128& Acc6_rxC0_4_heads0, axis_stream_plio_128& Acc6_rxC0_5_heads0, axis_stream_plio_128& Acc6_rxC0_6_heads0, axis_stream_plio_128& Acc6_rxC0_7_heads0, axis_stream_plio_128& Acc6_rxC0_8_heads0, axis_stream_plio_128& Acc6_rxC0_9_heads0, axis_stream_plio_128& Acc6_rxC0_10_heads0, axis_stream_plio_128& Acc6_rxC0_11_heads0, axis_stream_plio_128& Acc6_rxC0_12_heads0, axis_stream_plio_128& Acc6_rxC0_13_heads0, axis_stream_plio_128& Acc6_rxC0_14_heads0, axis_stream_plio_128& Acc6_rxC0_15_heads0, axis_stream_plio_128& Acc6_rxC0_16_heads0, axis_stream_plio_128& Acc6_rxC0_17_heads0, axis_stream_plio_128& Acc6_rxC0_18_heads0, axis_stream_plio_128& Acc6_rxC0_19_heads0, axis_stream_plio_128& Acc6_rxC0_20_heads0, axis_stream_plio_128& Acc6_rxC0_21_heads0, axis_stream_plio_128& Acc6_rxC0_22_heads0, axis_stream_plio_128& Acc6_rxC0_23_heads0,  
                     axis_stream_plio_128& Acc7_txA0_0_heads0, axis_stream_plio_128& Acc7_txA0_1_heads0, axis_stream_plio_128& Acc7_txA0_2_heads0, axis_stream_plio_128& Acc7_txA0_3_heads0, axis_stream_plio_128& Acc7_txA0_4_heads0, axis_stream_plio_128& Acc7_txA0_5_heads0, axis_stream_plio_128& Acc7_txA0_6_heads0, axis_stream_plio_128& Acc7_txA0_7_heads0, axis_stream_plio_128& Acc7_txA0_8_heads0, axis_stream_plio_128& Acc7_txA0_9_heads0, axis_stream_plio_128& Acc7_txA0_10_heads0, axis_stream_plio_128& Acc7_txA0_11_heads0, axis_stream_plio_128& Acc7_txA0_12_heads0, axis_stream_plio_128& Acc7_txA0_13_heads0, axis_stream_plio_128& Acc7_txA0_14_heads0, axis_stream_plio_128& Acc7_txA0_15_heads0, axis_stream_plio_128& Acc7_txA0_16_heads0, axis_stream_plio_128& Acc7_txA0_17_heads0, axis_stream_plio_128& Acc7_txA0_18_heads0, axis_stream_plio_128& Acc7_txA0_19_heads0, axis_stream_plio_128& Acc7_txA0_20_heads0, axis_stream_plio_128& Acc7_txA0_21_heads0, axis_stream_plio_128& Acc7_txA0_22_heads0, axis_stream_plio_128& Acc7_txA0_23_heads0,  
                     axis_stream_plio_128& Acc7_rxC0_0_heads0, axis_stream_plio_128& Acc7_rxC0_1_heads0, axis_stream_plio_128& Acc7_rxC0_2_heads0, axis_stream_plio_128& Acc7_rxC0_3_heads0,  
                     axis_stream_plio_128& Acc8_txA0_0, axis_stream_plio_128& Acc8_txA0_1, axis_stream_plio_128& Acc8_txA0_2, axis_stream_plio_128& Acc8_txA0_3, axis_stream_plio_128& Acc8_txA0_4, axis_stream_plio_128& Acc8_txA0_5, axis_stream_plio_128& Acc8_txA0_6, axis_stream_plio_128& Acc8_txA0_7, axis_stream_plio_128& Acc8_txA0_8, axis_stream_plio_128& Acc8_txA0_9, axis_stream_plio_128& Acc8_txA0_10, axis_stream_plio_128& Acc8_txA0_11,  
                     axis_stream_plio_128& Acc8_rxC0_0, axis_stream_plio_128& Acc8_rxC0_1, axis_stream_plio_128& Acc8_rxC0_2, axis_stream_plio_128& Acc8_rxC0_3,  
                     axis_stream_plio_128& Acc9_txA0_0, axis_stream_plio_128& Acc9_txA0_1,  
                     axis_stream_plio_128& Acc9_rxC0_0, axis_stream_plio_128& Acc9_rxC0_1, axis_stream_plio_128& Acc9_rxC0_2, axis_stream_plio_128& Acc9_rxC0_3, axis_stream_plio_128& Acc9_rxC0_4,  
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
    
    ap_uint<PLIO_WIDTH_128> buff_pe_0[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/ACC0_A/NUM_PER_64][ACC0_K0/ACC0_B/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff_pe_0 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_pe_0 complete dim=1

    ap_uint<PLIO_WIDTH_128> buff_pe_1[BUFF0_LHS_PARTA*BUFF0_LHS_PARTB*ACC0_HEADS][ACC0_M0/ACC0_A/NUM_PER_64][ACC0_K0/ACC0_B/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff_pe_1 type=RAM_T2P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_pe_1 complete dim=1


    ap_uint<PLIO_WIDTH_128> buff0_0[BUFF0_OUT_PARTA*BUFF0_OUT_PARTC*ACC0_HEADS][ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff0_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff0_1[BUFF0_OUT_PARTA*BUFF0_OUT_PARTC*ACC0_HEADS][ACC0_M0/BUFF0_OUT_PARTA/NUM_PER_64][ACC0_N0/BUFF0_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff0_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff0_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff1_0[BUFF1_OUT_PARTA*BUFF1_OUT_PARTC*ACC1_HEADS][ACC1_M0/BUFF1_OUT_PARTA/NUM_PER_64][ACC1_N0/BUFF1_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff1_1[BUFF1_OUT_PARTA*BUFF1_OUT_PARTC*ACC1_HEADS][ACC1_M0/BUFF1_OUT_PARTA/NUM_PER_64][ACC1_N0/BUFF1_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff1_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff1_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff2_0[BUFF2_OUT_PARTA*BUFF2_OUT_PARTC*ACC2_HEADS][ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64][ACC2_N0/BUFF2_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff2_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff2_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff2_1[BUFF2_OUT_PARTA*BUFF2_OUT_PARTC*ACC2_HEADS][ACC2_M0/BUFF2_OUT_PARTA/NUM_PER_64][ACC2_N0/BUFF2_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff2_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff2_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff3_0[BUFF3_OUT_PARTA*BUFF3_OUT_PARTC*ACC3_HEADS][ACC3_M0/BUFF3_OUT_PARTA/NUM_PER_64][ACC3_N0/BUFF3_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff3_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff3_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff3_1[BUFF3_OUT_PARTA*BUFF3_OUT_PARTC*ACC3_HEADS][ACC3_M0/BUFF3_OUT_PARTA/NUM_PER_64][ACC3_N0/BUFF3_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff3_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff3_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff4_0[BUFF4_OUT_PARTA*BUFF4_OUT_PARTC*ACC4_HEADS][ACC4_M0/BUFF4_OUT_PARTA/NUM_PER_64][ACC4_N0/BUFF4_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff4_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff4_0 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff_dummy_0[BUFF4_RHS_PARTB*BUFF4_RHS_PARTC*ACC4_HEADS][ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64][ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff_dummy_0 type=RAM_1P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_dummy_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff4_1[BUFF4_OUT_PARTA*BUFF4_OUT_PARTC*ACC4_HEADS][ACC4_M0/BUFF4_OUT_PARTA/NUM_PER_64][ACC4_N0/BUFF4_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff4_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff4_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff_dummy_1[BUFF4_RHS_PARTB*BUFF4_RHS_PARTC*ACC4_HEADS][ACC4_K0/BUFF4_RHS_PARTB/NUM_PER_64][ACC4_N0/BUFF4_RHS_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff_dummy_1 type=RAM_1P impl=URAM
    #pragma HLS ARRAY_PARTITION variable=buff_dummy_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff5_0[BUFF5_OUT_PARTA*BUFF5_OUT_PARTC*ACC5_HEADS][ACC5_M0/BUFF5_OUT_PARTA/NUM_PER_64][ACC5_N0/BUFF5_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff5_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff5_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff5_1[BUFF5_OUT_PARTA*BUFF5_OUT_PARTC*ACC5_HEADS][ACC5_M0/BUFF5_OUT_PARTA/NUM_PER_64][ACC5_N0/BUFF5_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff5_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff5_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff6_0[BUFF6_OUT_PARTA*BUFF6_OUT_PARTC*ACC6_HEADS][ACC6_M0/BUFF6_OUT_PARTA/NUM_PER_64][ACC6_N0/BUFF6_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff6_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff6_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff6_1[BUFF6_OUT_PARTA*BUFF6_OUT_PARTC*ACC6_HEADS][ACC6_M0/BUFF6_OUT_PARTA/NUM_PER_64][ACC6_N0/BUFF6_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff6_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff6_1 complete dim=1
    
    ap_uint<PLIO_WIDTH_128> buff7_0[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff7_0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff7_0 complete dim=1
    ap_uint<PLIO_WIDTH_128> buff7_1[BUFF7_OUT_PARTA*BUFF7_OUT_PARTC*ACC7_HEADS][ACC7_M0/BUFF7_OUT_PARTA/NUM_PER_64][ACC7_N0/BUFF7_OUT_PARTC/(NUM_PER_128/NUM_PER_64)];
    #pragma HLS bind_storage variable=buff7_1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff7_1 complete dim=1
    
    

    ap_uint<PLIO_WIDTH_64> buff_fc0[FC_B][FC_K/FC_B/NUM_PER_64];
    #pragma HLS bind_storage variable=buff_fc0 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff_fc0 complete dim=1 

    ap_uint<PLIO_WIDTH_64> buff_fc1[FC_B][FC_K/FC_B/NUM_PER_64];
    #pragma HLS bind_storage variable=buff_fc1 type=RAM_1P impl=BRAM
    #pragma HLS ARRAY_PARTITION variable=buff_fc1 complete dim=1

    bool enable_load,enable_pe,enable_s0,enable_sel,enable_s1,enable_s2,enable_s3,enable_s4,enable_s5,enable_m2v,enable_fc;
    int batch_iter = (batch-1)/6 + 1;
    int round = batch_iter * 6 * block + 2;
    int round_all = round + batch + 1;
    int rd_all = 0 ;
    for (int iter = 0; iter < iteration; iter++) {
        for (int rd=0; rd<round_all; rd++){
            enable_load=(rd<batch);
            enable_pe =( rd>0 && rd<(batch+1) );
            enable_s0 =( (rd>1) && (rd<round) && (((rd-2)%6)<batch) );
            enable_sel=( (rd>batch+1) && (rd<round) && (((rd-2)%6)<batch) );
            enable_s1 =( (rd>2) && (rd<round+1) && (((rd-3)%6)<batch) );
            enable_s2 =( (rd>3) && (rd<round+2) && (((rd-4)%6)<batch) );
            enable_s3 =( (rd>4) && (rd<round+3) && (((rd-5)%6)<batch) );
            enable_s4 =( (rd>5) && (rd<round+4) && (((rd-6)%6)<batch) );
            enable_s5 =( (rd>6) && (rd<round+5) && (((rd-7)%6)<batch) );
            enable_m2v=( rd>(round-1) && (rd<round_all-1) );
            enable_fc =( rd>(round) && (rd<round_all) );

            
            if(rd%2==0){
                load_image_top<0>(dataA_in0, dataA_in1, dataA_in2, buff_image0, enable_load);
                send_patch_top<0>(buff_image1, Acc8_txA0_0,Acc8_txA0_1,Acc8_txA0_2,Acc8_txA0_3,Acc8_txA0_4,Acc8_txA0_5,Acc8_txA0_6,Acc8_txA0_7,Acc8_txA0_8,Acc8_txA0_9,Acc8_txA0_10,Acc8_txA0_11,enable_pe);
                receive_Patch_top<0>(Acc8_rxC0_0,Acc8_rxC0_1,Acc8_rxC0_2,Acc8_rxC0_3,buff_pe_0, enable_pe);
                Acc0_send_LHS_type0_top<0>(buff_pe_1, buff7_1,Acc0_txA0_0_heads0, Acc0_txA0_1_heads0, Acc0_txA0_2_heads0, Acc0_txA0_3_heads0,  Acc1_txA0_0_heads0, Acc1_txA0_1_heads0, Acc1_txA0_2_heads0, Acc1_txA0_3_heads0,  Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0,  enable_sel, enable_s0);
                Acc0_receive_OUT_type0_top<0>(buff0_0,Acc0_rxC0_0_heads0, Acc0_rxC0_1_heads0, Acc0_rxC0_2_heads0, Acc0_rxC0_3_heads0, Acc0_rxC0_4_heads0, Acc0_rxC0_5_heads0, enable_s0);
                Acc1_receive_OUT_type0_top<0>(buff1_0,Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0, Acc1_rxC0_2_heads0, Acc1_rxC0_3_heads0, Acc1_rxC0_4_heads0, Acc1_rxC0_5_heads0, enable_s0);
                Acc2_receive_OUT_type0_top<0>(buff2_0,Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0, enable_s0);
                Acc3_send_LHS_type0_top<0>(buff0_1, Acc3_txA0_0_heads0, Acc3_txA0_1_heads0,  Acc3_txA1_0_heads0, Acc3_txA1_1_heads0,  Acc3_txA0_0_heads1, Acc3_txA0_1_heads1,  Acc3_txA1_0_heads1, Acc3_txA1_1_heads1,  Acc3_txA0_0_heads2, Acc3_txA0_1_heads2,  Acc3_txA1_0_heads2, Acc3_txA1_1_heads2,  enable_s1);
                Acc4_send_dummy_top<0>(buff2_1,buff_dummy_0,enable_s1);
                Acc3_send_RHS_type0_top<0>(buff1_1,Acc3_txB0_0_heads0, Acc3_txB0_1_heads0, Acc3_txB1_0_heads0, Acc3_txB1_1_heads0, Acc3_txB0_0_heads1, Acc3_txB0_1_heads1, Acc3_txB1_0_heads1, Acc3_txB1_1_heads1, Acc3_txB0_0_heads2, Acc3_txB0_1_heads2, Acc3_txB1_0_heads2, Acc3_txB1_1_heads2, enable_s1);
                Acc3_receive_OUT_type0_top<0>(buff3_0,Acc3_rxC0_0_heads0, Acc3_rxC0_1_heads0, Acc3_rxC1_0_heads0, Acc3_rxC1_1_heads0, Acc3_rxC0_0_heads1, Acc3_rxC0_1_heads1, Acc3_rxC1_0_heads1, Acc3_rxC1_1_heads1, Acc3_rxC0_0_heads2, Acc3_rxC0_1_heads2, Acc3_rxC1_0_heads2, Acc3_rxC1_1_heads2, enable_s1);
                Acc4_send_LHS_type0_top<0>(buff3_1, Acc4_txA0_0_heads0, Acc4_txA0_1_heads0,  Acc4_txA1_0_heads0, Acc4_txA1_1_heads0,  Acc4_txA0_0_heads1, Acc4_txA0_1_heads1,  Acc4_txA1_0_heads1, Acc4_txA1_1_heads1,  Acc4_txA0_0_heads2, Acc4_txA0_1_heads2,  Acc4_txA1_0_heads2, Acc4_txA1_1_heads2,  enable_s2);
                Acc4_send_RHS_type0_top<0>(buff_dummy_1,Acc4_txB0_0_heads0, Acc4_txB0_1_heads0, Acc4_txB1_0_heads0, Acc4_txB1_1_heads0, Acc4_txB0_0_heads1, Acc4_txB0_1_heads1, Acc4_txB1_0_heads1, Acc4_txB1_1_heads1, Acc4_txB0_0_heads2, Acc4_txB0_1_heads2, Acc4_txB1_0_heads2, Acc4_txB1_1_heads2, enable_s2);
                Acc4_receive_OUT_type0_top<0>(buff4_0,Acc4_rxC0_0_heads0, Acc4_rxC0_1_heads0, Acc4_rxC1_0_heads0, Acc4_rxC1_1_heads0, Acc4_rxC0_0_heads1, Acc4_rxC0_1_heads1, Acc4_rxC1_0_heads1, Acc4_rxC1_1_heads1, Acc4_rxC0_0_heads2, Acc4_rxC0_1_heads2, Acc4_rxC1_0_heads2, Acc4_rxC1_1_heads2, enable_s2);
                Acc5_send_LHS_type0_top<0>(buff4_1, Acc5_txA0_0_heads0, Acc5_txA0_1_heads0, Acc5_txA0_2_heads0, Acc5_txA0_3_heads0, Acc5_txA0_4_heads0, Acc5_txA0_5_heads0,  enable_s3);
                Acc5_receive_OUT_type0_top<0>(buff5_0,Acc5_rxC0_0_heads0, Acc5_rxC0_1_heads0, Acc5_rxC0_2_heads0, Acc5_rxC0_3_heads0, enable_s3);
                Acc6_send_LHS_type0_top<0>(buff5_1, Acc6_txA0_0_heads0, Acc6_txA0_1_heads0, Acc6_txA0_2_heads0, Acc6_txA0_3_heads0,  Acc6_txA1_0_heads0, Acc6_txA1_1_heads0, Acc6_txA1_2_heads0, Acc6_txA1_3_heads0,  Acc6_txA2_0_heads0, Acc6_txA2_1_heads0, Acc6_txA2_2_heads0, Acc6_txA2_3_heads0,  enable_s4);
                Acc6_receive_OUT_type0_top<0>(buff6_0,Acc6_rxC0_0_heads0, Acc6_rxC0_1_heads0, Acc6_rxC0_2_heads0, Acc6_rxC0_3_heads0, Acc6_rxC0_4_heads0, Acc6_rxC0_5_heads0, Acc6_rxC0_6_heads0, Acc6_rxC0_7_heads0, Acc6_rxC0_8_heads0, Acc6_rxC0_9_heads0, Acc6_rxC0_10_heads0, Acc6_rxC0_11_heads0, Acc6_rxC0_12_heads0, Acc6_rxC0_13_heads0, Acc6_rxC0_14_heads0, Acc6_rxC0_15_heads0, Acc6_rxC0_16_heads0, Acc6_rxC0_17_heads0, Acc6_rxC0_18_heads0, Acc6_rxC0_19_heads0, Acc6_rxC0_20_heads0, Acc6_rxC0_21_heads0, Acc6_rxC0_22_heads0, Acc6_rxC0_23_heads0, enable_s4);
                Acc7_m2vector<0>(buff6_1, buff7_0, Acc7_txA0_0_heads0, Acc7_txA0_1_heads0, Acc7_txA0_2_heads0, Acc7_txA0_3_heads0, Acc7_txA0_4_heads0, Acc7_txA0_5_heads0, Acc7_txA0_6_heads0, Acc7_txA0_7_heads0, Acc7_txA0_8_heads0, Acc7_txA0_9_heads0, Acc7_txA0_10_heads0, Acc7_txA0_11_heads0, Acc7_txA0_12_heads0, Acc7_txA0_13_heads0, Acc7_txA0_14_heads0, Acc7_txA0_15_heads0, Acc7_txA0_16_heads0, Acc7_txA0_17_heads0, Acc7_txA0_18_heads0, Acc7_txA0_19_heads0, Acc7_txA0_20_heads0, Acc7_txA0_21_heads0, Acc7_txA0_22_heads0, Acc7_txA0_23_heads0,  Acc7_rxC0_0_heads0, Acc7_rxC0_1_heads0, Acc7_rxC0_2_heads0, Acc7_rxC0_3_heads0,  buff_fc0, enable_s5, enable_m2v);
                send_FC<0>(buff_fc1, Acc9_txA0_0,Acc9_txA0_1,enable_fc);
                receive_FC<0>(dataC_out, Acc9_rxC0_0,Acc9_rxC0_1,Acc9_rxC0_2,Acc9_rxC0_3,Acc9_rxC0_4,enable_fc);
            }
            else{
                load_image_top<0>(dataA_in0, dataA_in1, dataA_in2, buff_image1, enable_load);
                send_patch_top<0>(buff_image0, Acc8_txA0_0,Acc8_txA0_1,Acc8_txA0_2,Acc8_txA0_3,Acc8_txA0_4,Acc8_txA0_5,Acc8_txA0_6,Acc8_txA0_7,Acc8_txA0_8,Acc8_txA0_9,Acc8_txA0_10,Acc8_txA0_11,enable_pe);
                receive_Patch_top<0>(Acc8_rxC0_0,Acc8_rxC0_1,Acc8_rxC0_2,Acc8_rxC0_3,buff_pe_1, enable_pe);
                Acc0_send_LHS_type0_top<0>(buff_pe_0, buff7_0,Acc0_txA0_0_heads0, Acc0_txA0_1_heads0, Acc0_txA0_2_heads0, Acc0_txA0_3_heads0,  Acc1_txA0_0_heads0, Acc1_txA0_1_heads0, Acc1_txA0_2_heads0, Acc1_txA0_3_heads0,  Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0,  enable_sel, enable_s0);
                Acc0_receive_OUT_type0_top<0>(buff0_1,Acc0_rxC0_0_heads0, Acc0_rxC0_1_heads0, Acc0_rxC0_2_heads0, Acc0_rxC0_3_heads0, Acc0_rxC0_4_heads0, Acc0_rxC0_5_heads0, enable_s0);
                Acc1_receive_OUT_type0_top<0>(buff1_1,Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0, Acc1_rxC0_2_heads0, Acc1_rxC0_3_heads0, Acc1_rxC0_4_heads0, Acc1_rxC0_5_heads0, enable_s0);
                Acc2_receive_OUT_type0_top<0>(buff2_1,Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0, enable_s0);
                Acc3_send_LHS_type0_top<0>(buff0_0, Acc3_txA0_0_heads0, Acc3_txA0_1_heads0,  Acc3_txA1_0_heads0, Acc3_txA1_1_heads0,  Acc3_txA0_0_heads1, Acc3_txA0_1_heads1,  Acc3_txA1_0_heads1, Acc3_txA1_1_heads1,  Acc3_txA0_0_heads2, Acc3_txA0_1_heads2,  Acc3_txA1_0_heads2, Acc3_txA1_1_heads2,  enable_s1);
                Acc4_send_dummy_top<0>(buff2_0,buff_dummy_1,enable_s1);
                Acc3_send_RHS_type0_top<0>(buff1_0,Acc3_txB0_0_heads0, Acc3_txB0_1_heads0, Acc3_txB1_0_heads0, Acc3_txB1_1_heads0, Acc3_txB0_0_heads1, Acc3_txB0_1_heads1, Acc3_txB1_0_heads1, Acc3_txB1_1_heads1, Acc3_txB0_0_heads2, Acc3_txB0_1_heads2, Acc3_txB1_0_heads2, Acc3_txB1_1_heads2, enable_s1);
                Acc3_receive_OUT_type0_top<0>(buff3_1,Acc3_rxC0_0_heads0, Acc3_rxC0_1_heads0, Acc3_rxC1_0_heads0, Acc3_rxC1_1_heads0, Acc3_rxC0_0_heads1, Acc3_rxC0_1_heads1, Acc3_rxC1_0_heads1, Acc3_rxC1_1_heads1, Acc3_rxC0_0_heads2, Acc3_rxC0_1_heads2, Acc3_rxC1_0_heads2, Acc3_rxC1_1_heads2, enable_s1);
                Acc4_send_LHS_type0_top<0>(buff3_0, Acc4_txA0_0_heads0, Acc4_txA0_1_heads0,  Acc4_txA1_0_heads0, Acc4_txA1_1_heads0,  Acc4_txA0_0_heads1, Acc4_txA0_1_heads1,  Acc4_txA1_0_heads1, Acc4_txA1_1_heads1,  Acc4_txA0_0_heads2, Acc4_txA0_1_heads2,  Acc4_txA1_0_heads2, Acc4_txA1_1_heads2,  enable_s2);
                Acc4_send_RHS_type0_top<0>(buff_dummy_0,Acc4_txB0_0_heads0, Acc4_txB0_1_heads0, Acc4_txB1_0_heads0, Acc4_txB1_1_heads0, Acc4_txB0_0_heads1, Acc4_txB0_1_heads1, Acc4_txB1_0_heads1, Acc4_txB1_1_heads1, Acc4_txB0_0_heads2, Acc4_txB0_1_heads2, Acc4_txB1_0_heads2, Acc4_txB1_1_heads2, enable_s2);
                Acc4_receive_OUT_type0_top<0>(buff4_1,Acc4_rxC0_0_heads0, Acc4_rxC0_1_heads0, Acc4_rxC1_0_heads0, Acc4_rxC1_1_heads0, Acc4_rxC0_0_heads1, Acc4_rxC0_1_heads1, Acc4_rxC1_0_heads1, Acc4_rxC1_1_heads1, Acc4_rxC0_0_heads2, Acc4_rxC0_1_heads2, Acc4_rxC1_0_heads2, Acc4_rxC1_1_heads2, enable_s2);
                Acc5_send_LHS_type0_top<0>(buff4_0, Acc5_txA0_0_heads0, Acc5_txA0_1_heads0, Acc5_txA0_2_heads0, Acc5_txA0_3_heads0, Acc5_txA0_4_heads0, Acc5_txA0_5_heads0,  enable_s3);
                Acc5_receive_OUT_type0_top<0>(buff5_1,Acc5_rxC0_0_heads0, Acc5_rxC0_1_heads0, Acc5_rxC0_2_heads0, Acc5_rxC0_3_heads0, enable_s3);
                Acc6_send_LHS_type0_top<0>(buff5_0, Acc6_txA0_0_heads0, Acc6_txA0_1_heads0, Acc6_txA0_2_heads0, Acc6_txA0_3_heads0,  Acc6_txA1_0_heads0, Acc6_txA1_1_heads0, Acc6_txA1_2_heads0, Acc6_txA1_3_heads0,  Acc6_txA2_0_heads0, Acc6_txA2_1_heads0, Acc6_txA2_2_heads0, Acc6_txA2_3_heads0,  enable_s4);
                Acc6_receive_OUT_type0_top<0>(buff6_1,Acc6_rxC0_0_heads0, Acc6_rxC0_1_heads0, Acc6_rxC0_2_heads0, Acc6_rxC0_3_heads0, Acc6_rxC0_4_heads0, Acc6_rxC0_5_heads0, Acc6_rxC0_6_heads0, Acc6_rxC0_7_heads0, Acc6_rxC0_8_heads0, Acc6_rxC0_9_heads0, Acc6_rxC0_10_heads0, Acc6_rxC0_11_heads0, Acc6_rxC0_12_heads0, Acc6_rxC0_13_heads0, Acc6_rxC0_14_heads0, Acc6_rxC0_15_heads0, Acc6_rxC0_16_heads0, Acc6_rxC0_17_heads0, Acc6_rxC0_18_heads0, Acc6_rxC0_19_heads0, Acc6_rxC0_20_heads0, Acc6_rxC0_21_heads0, Acc6_rxC0_22_heads0, Acc6_rxC0_23_heads0, enable_s4);
                Acc7_m2vector<0>(buff6_0, buff7_1, Acc7_txA0_0_heads0, Acc7_txA0_1_heads0, Acc7_txA0_2_heads0, Acc7_txA0_3_heads0, Acc7_txA0_4_heads0, Acc7_txA0_5_heads0, Acc7_txA0_6_heads0, Acc7_txA0_7_heads0, Acc7_txA0_8_heads0, Acc7_txA0_9_heads0, Acc7_txA0_10_heads0, Acc7_txA0_11_heads0, Acc7_txA0_12_heads0, Acc7_txA0_13_heads0, Acc7_txA0_14_heads0, Acc7_txA0_15_heads0, Acc7_txA0_16_heads0, Acc7_txA0_17_heads0, Acc7_txA0_18_heads0, Acc7_txA0_19_heads0, Acc7_txA0_20_heads0, Acc7_txA0_21_heads0, Acc7_txA0_22_heads0, Acc7_txA0_23_heads0,  Acc7_rxC0_0_heads0, Acc7_rxC0_1_heads0, Acc7_rxC0_2_heads0, Acc7_rxC0_3_heads0,  buff_fc1, enable_s5, enable_m2v);
                send_FC<0>(buff_fc0, Acc9_txA0_0,Acc9_txA0_1,enable_fc);
                receive_FC<0>(dataC_out, Acc9_rxC0_0,Acc9_rxC0_1,Acc9_rxC0_2,Acc9_rxC0_3,Acc9_rxC0_4,enable_fc);
            }

        }
    }
}

void pl_engine( ap_uint<AXI_WIDTH_A>* ina0, ap_uint<AXI_WIDTH_A>* ina1, ap_uint<AXI_WIDTH_A>* ina2, ap_uint<AXI_WIDTH_C>* out0, 
               axis_stream_plio_128& Acc0_txA0_0_heads0, axis_stream_plio_128& Acc0_txA0_1_heads0, axis_stream_plio_128& Acc0_txA0_2_heads0, axis_stream_plio_128& Acc0_txA0_3_heads0,  
               axis_stream_plio_128& Acc0_rxC0_0_heads0, axis_stream_plio_128& Acc0_rxC0_1_heads0, axis_stream_plio_128& Acc0_rxC0_2_heads0, axis_stream_plio_128& Acc0_rxC0_3_heads0, axis_stream_plio_128& Acc0_rxC0_4_heads0, axis_stream_plio_128& Acc0_rxC0_5_heads0,  
               axis_stream_plio_128& Acc1_txA0_0_heads0, axis_stream_plio_128& Acc1_txA0_1_heads0, axis_stream_plio_128& Acc1_txA0_2_heads0, axis_stream_plio_128& Acc1_txA0_3_heads0,  
               axis_stream_plio_128& Acc1_rxC0_0_heads0, axis_stream_plio_128& Acc1_rxC0_1_heads0, axis_stream_plio_128& Acc1_rxC0_2_heads0, axis_stream_plio_128& Acc1_rxC0_3_heads0, axis_stream_plio_128& Acc1_rxC0_4_heads0, axis_stream_plio_128& Acc1_rxC0_5_heads0,  
               axis_stream_plio_128& Acc2_txA0_0_heads0, axis_stream_plio_128& Acc2_txA0_1_heads0, axis_stream_plio_128& Acc2_txA0_2_heads0, axis_stream_plio_128& Acc2_txA0_3_heads0,  
               axis_stream_plio_128& Acc2_rxC0_0_heads0, axis_stream_plio_128& Acc2_rxC0_1_heads0, axis_stream_plio_128& Acc2_rxC0_2_heads0, axis_stream_plio_128& Acc2_rxC0_3_heads0, axis_stream_plio_128& Acc2_rxC0_4_heads0, axis_stream_plio_128& Acc2_rxC0_5_heads0,  
               axis_stream_plio_128& Acc3_txA0_0_heads0, axis_stream_plio_128& Acc3_txA0_1_heads0,  
               axis_stream_plio_128& Acc3_txA1_0_heads0, axis_stream_plio_128& Acc3_txA1_1_heads0,  
               axis_stream_plio_128& Acc3_txA0_0_heads1, axis_stream_plio_128& Acc3_txA0_1_heads1,  
               axis_stream_plio_128& Acc3_txA1_0_heads1, axis_stream_plio_128& Acc3_txA1_1_heads1,  
               axis_stream_plio_128& Acc3_txA0_0_heads2, axis_stream_plio_128& Acc3_txA0_1_heads2,  
               axis_stream_plio_128& Acc3_txA1_0_heads2, axis_stream_plio_128& Acc3_txA1_1_heads2,  
               axis_stream_plio_128& Acc3_txB0_0_heads0, axis_stream_plio_128& Acc3_txB1_0_heads0,  
               axis_stream_plio_128& Acc3_txB0_1_heads0, axis_stream_plio_128& Acc3_txB1_1_heads0,  
               axis_stream_plio_128& Acc3_txB0_0_heads1, axis_stream_plio_128& Acc3_txB1_0_heads1,  
               axis_stream_plio_128& Acc3_txB0_1_heads1, axis_stream_plio_128& Acc3_txB1_1_heads1,  
               axis_stream_plio_128& Acc3_txB0_0_heads2, axis_stream_plio_128& Acc3_txB1_0_heads2,  
               axis_stream_plio_128& Acc3_txB0_1_heads2, axis_stream_plio_128& Acc3_txB1_1_heads2,  
               axis_stream_plio_128& Acc3_rxC0_0_heads0, axis_stream_plio_128& Acc3_rxC0_1_heads0,  
               axis_stream_plio_128& Acc3_rxC1_0_heads0, axis_stream_plio_128& Acc3_rxC1_1_heads0,  
               axis_stream_plio_128& Acc3_rxC0_0_heads1, axis_stream_plio_128& Acc3_rxC0_1_heads1,  
               axis_stream_plio_128& Acc3_rxC1_0_heads1, axis_stream_plio_128& Acc3_rxC1_1_heads1,  
               axis_stream_plio_128& Acc3_rxC0_0_heads2, axis_stream_plio_128& Acc3_rxC0_1_heads2,  
               axis_stream_plio_128& Acc3_rxC1_0_heads2, axis_stream_plio_128& Acc3_rxC1_1_heads2,  
               axis_stream_plio_128& Acc4_txA0_0_heads0, axis_stream_plio_128& Acc4_txA0_1_heads0,  
               axis_stream_plio_128& Acc4_txA1_0_heads0, axis_stream_plio_128& Acc4_txA1_1_heads0,  
               axis_stream_plio_128& Acc4_txA0_0_heads1, axis_stream_plio_128& Acc4_txA0_1_heads1,  
               axis_stream_plio_128& Acc4_txA1_0_heads1, axis_stream_plio_128& Acc4_txA1_1_heads1,  
               axis_stream_plio_128& Acc4_txA0_0_heads2, axis_stream_plio_128& Acc4_txA0_1_heads2,  
               axis_stream_plio_128& Acc4_txA1_0_heads2, axis_stream_plio_128& Acc4_txA1_1_heads2,  
               axis_stream_plio_128& Acc4_txB0_0_heads0, axis_stream_plio_128& Acc4_txB1_0_heads0,  
               axis_stream_plio_128& Acc4_txB0_1_heads0, axis_stream_plio_128& Acc4_txB1_1_heads0,  
               axis_stream_plio_128& Acc4_txB0_0_heads1, axis_stream_plio_128& Acc4_txB1_0_heads1,  
               axis_stream_plio_128& Acc4_txB0_1_heads1, axis_stream_plio_128& Acc4_txB1_1_heads1,  
               axis_stream_plio_128& Acc4_txB0_0_heads2, axis_stream_plio_128& Acc4_txB1_0_heads2,  
               axis_stream_plio_128& Acc4_txB0_1_heads2, axis_stream_plio_128& Acc4_txB1_1_heads2,  
               axis_stream_plio_128& Acc4_rxC0_0_heads0, axis_stream_plio_128& Acc4_rxC0_1_heads0,  
               axis_stream_plio_128& Acc4_rxC1_0_heads0, axis_stream_plio_128& Acc4_rxC1_1_heads0,  
               axis_stream_plio_128& Acc4_rxC0_0_heads1, axis_stream_plio_128& Acc4_rxC0_1_heads1,  
               axis_stream_plio_128& Acc4_rxC1_0_heads1, axis_stream_plio_128& Acc4_rxC1_1_heads1,  
               axis_stream_plio_128& Acc4_rxC0_0_heads2, axis_stream_plio_128& Acc4_rxC0_1_heads2,  
               axis_stream_plio_128& Acc4_rxC1_0_heads2, axis_stream_plio_128& Acc4_rxC1_1_heads2,  
               axis_stream_plio_128& Acc5_txA0_0_heads0, axis_stream_plio_128& Acc5_txA0_1_heads0, axis_stream_plio_128& Acc5_txA0_2_heads0, axis_stream_plio_128& Acc5_txA0_3_heads0, axis_stream_plio_128& Acc5_txA0_4_heads0, axis_stream_plio_128& Acc5_txA0_5_heads0,  
               axis_stream_plio_128& Acc5_rxC0_0_heads0, axis_stream_plio_128& Acc5_rxC0_1_heads0, axis_stream_plio_128& Acc5_rxC0_2_heads0, axis_stream_plio_128& Acc5_rxC0_3_heads0,  
               axis_stream_plio_128& Acc6_txA0_0_heads0, axis_stream_plio_128& Acc6_txA0_1_heads0, axis_stream_plio_128& Acc6_txA0_2_heads0, axis_stream_plio_128& Acc6_txA0_3_heads0,  
               axis_stream_plio_128& Acc6_txA1_0_heads0, axis_stream_plio_128& Acc6_txA1_1_heads0, axis_stream_plio_128& Acc6_txA1_2_heads0, axis_stream_plio_128& Acc6_txA1_3_heads0,  
               axis_stream_plio_128& Acc6_txA2_0_heads0, axis_stream_plio_128& Acc6_txA2_1_heads0, axis_stream_plio_128& Acc6_txA2_2_heads0, axis_stream_plio_128& Acc6_txA2_3_heads0,  
               axis_stream_plio_128& Acc6_rxC0_0_heads0, axis_stream_plio_128& Acc6_rxC0_1_heads0, axis_stream_plio_128& Acc6_rxC0_2_heads0, axis_stream_plio_128& Acc6_rxC0_3_heads0, axis_stream_plio_128& Acc6_rxC0_4_heads0, axis_stream_plio_128& Acc6_rxC0_5_heads0, axis_stream_plio_128& Acc6_rxC0_6_heads0, axis_stream_plio_128& Acc6_rxC0_7_heads0, axis_stream_plio_128& Acc6_rxC0_8_heads0, axis_stream_plio_128& Acc6_rxC0_9_heads0, axis_stream_plio_128& Acc6_rxC0_10_heads0, axis_stream_plio_128& Acc6_rxC0_11_heads0, axis_stream_plio_128& Acc6_rxC0_12_heads0, axis_stream_plio_128& Acc6_rxC0_13_heads0, axis_stream_plio_128& Acc6_rxC0_14_heads0, axis_stream_plio_128& Acc6_rxC0_15_heads0, axis_stream_plio_128& Acc6_rxC0_16_heads0, axis_stream_plio_128& Acc6_rxC0_17_heads0, axis_stream_plio_128& Acc6_rxC0_18_heads0, axis_stream_plio_128& Acc6_rxC0_19_heads0, axis_stream_plio_128& Acc6_rxC0_20_heads0, axis_stream_plio_128& Acc6_rxC0_21_heads0, axis_stream_plio_128& Acc6_rxC0_22_heads0, axis_stream_plio_128& Acc6_rxC0_23_heads0,  
               axis_stream_plio_128& Acc7_txA0_0_heads0, axis_stream_plio_128& Acc7_txA0_1_heads0, axis_stream_plio_128& Acc7_txA0_2_heads0, axis_stream_plio_128& Acc7_txA0_3_heads0, axis_stream_plio_128& Acc7_txA0_4_heads0, axis_stream_plio_128& Acc7_txA0_5_heads0, axis_stream_plio_128& Acc7_txA0_6_heads0, axis_stream_plio_128& Acc7_txA0_7_heads0, axis_stream_plio_128& Acc7_txA0_8_heads0, axis_stream_plio_128& Acc7_txA0_9_heads0, axis_stream_plio_128& Acc7_txA0_10_heads0, axis_stream_plio_128& Acc7_txA0_11_heads0, axis_stream_plio_128& Acc7_txA0_12_heads0, axis_stream_plio_128& Acc7_txA0_13_heads0, axis_stream_plio_128& Acc7_txA0_14_heads0, axis_stream_plio_128& Acc7_txA0_15_heads0, axis_stream_plio_128& Acc7_txA0_16_heads0, axis_stream_plio_128& Acc7_txA0_17_heads0, axis_stream_plio_128& Acc7_txA0_18_heads0, axis_stream_plio_128& Acc7_txA0_19_heads0, axis_stream_plio_128& Acc7_txA0_20_heads0, axis_stream_plio_128& Acc7_txA0_21_heads0, axis_stream_plio_128& Acc7_txA0_22_heads0, axis_stream_plio_128& Acc7_txA0_23_heads0,  
               axis_stream_plio_128& Acc7_rxC0_0_heads0, axis_stream_plio_128& Acc7_rxC0_1_heads0, axis_stream_plio_128& Acc7_rxC0_2_heads0, axis_stream_plio_128& Acc7_rxC0_3_heads0,  
               axis_stream_plio_128& Acc8_txA0_0, axis_stream_plio_128& Acc8_txA0_1, axis_stream_plio_128& Acc8_txA0_2, axis_stream_plio_128& Acc8_txA0_3, axis_stream_plio_128& Acc8_txA0_4, axis_stream_plio_128& Acc8_txA0_5, axis_stream_plio_128& Acc8_txA0_6, axis_stream_plio_128& Acc8_txA0_7, axis_stream_plio_128& Acc8_txA0_8, axis_stream_plio_128& Acc8_txA0_9, axis_stream_plio_128& Acc8_txA0_10, axis_stream_plio_128& Acc8_txA0_11,  
               axis_stream_plio_128& Acc8_rxC0_0, axis_stream_plio_128& Acc8_rxC0_1, axis_stream_plio_128& Acc8_rxC0_2, axis_stream_plio_128& Acc8_rxC0_3,  
               axis_stream_plio_128& Acc9_txA0_0, axis_stream_plio_128& Acc9_txA0_1,  
               axis_stream_plio_128& Acc9_rxC0_0, axis_stream_plio_128& Acc9_rxC0_1, axis_stream_plio_128& Acc9_rxC0_2, axis_stream_plio_128& Acc9_rxC0_3, axis_stream_plio_128& Acc9_rxC0_4,  
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
    #pragma HLS interface axis port=Acc1_txA0_0_heads0
    #pragma HLS interface axis port=Acc1_txA0_1_heads0
    #pragma HLS interface axis port=Acc1_txA0_2_heads0
    #pragma HLS interface axis port=Acc1_txA0_3_heads0
    #pragma HLS interface axis port=Acc1_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_3_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_4_heads0  
    #pragma HLS interface axis port=Acc1_rxC0_5_heads0  
    #pragma HLS interface axis port=Acc2_txA0_0_heads0
    #pragma HLS interface axis port=Acc2_txA0_1_heads0
    #pragma HLS interface axis port=Acc2_txA0_2_heads0
    #pragma HLS interface axis port=Acc2_txA0_3_heads0
    #pragma HLS interface axis port=Acc2_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_3_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_4_heads0  
    #pragma HLS interface axis port=Acc2_rxC0_5_heads0  
    #pragma HLS interface axis port=Acc3_txA0_0_heads0
    #pragma HLS interface axis port=Acc3_txA0_1_heads0
    #pragma HLS interface axis port=Acc3_txA1_0_heads0
    #pragma HLS interface axis port=Acc3_txA1_1_heads0
    #pragma HLS interface axis port=Acc3_txA0_0_heads1
    #pragma HLS interface axis port=Acc3_txA0_1_heads1
    #pragma HLS interface axis port=Acc3_txA1_0_heads1
    #pragma HLS interface axis port=Acc3_txA1_1_heads1
    #pragma HLS interface axis port=Acc3_txA0_0_heads2
    #pragma HLS interface axis port=Acc3_txA0_1_heads2
    #pragma HLS interface axis port=Acc3_txA1_0_heads2
    #pragma HLS interface axis port=Acc3_txA1_1_heads2
    #pragma HLS interface axis port=Acc3_txB0_0_heads0 
    #pragma HLS interface axis port=Acc3_txB1_0_heads0 
    #pragma HLS interface axis port=Acc3_txB0_1_heads0 
    #pragma HLS interface axis port=Acc3_txB1_1_heads0 
    #pragma HLS interface axis port=Acc3_txB0_0_heads1 
    #pragma HLS interface axis port=Acc3_txB1_0_heads1 
    #pragma HLS interface axis port=Acc3_txB0_1_heads1 
    #pragma HLS interface axis port=Acc3_txB1_1_heads1 
    #pragma HLS interface axis port=Acc3_txB0_0_heads2 
    #pragma HLS interface axis port=Acc3_txB1_0_heads2 
    #pragma HLS interface axis port=Acc3_txB0_1_heads2 
    #pragma HLS interface axis port=Acc3_txB1_1_heads2 
    #pragma HLS interface axis port=Acc3_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc3_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc3_rxC1_0_heads0  
    #pragma HLS interface axis port=Acc3_rxC1_1_heads0  
    #pragma HLS interface axis port=Acc3_rxC0_0_heads1  
    #pragma HLS interface axis port=Acc3_rxC0_1_heads1  
    #pragma HLS interface axis port=Acc3_rxC1_0_heads1  
    #pragma HLS interface axis port=Acc3_rxC1_1_heads1  
    #pragma HLS interface axis port=Acc3_rxC0_0_heads2  
    #pragma HLS interface axis port=Acc3_rxC0_1_heads2  
    #pragma HLS interface axis port=Acc3_rxC1_0_heads2  
    #pragma HLS interface axis port=Acc3_rxC1_1_heads2  
    #pragma HLS interface axis port=Acc4_txA0_0_heads0
    #pragma HLS interface axis port=Acc4_txA0_1_heads0
    #pragma HLS interface axis port=Acc4_txA1_0_heads0
    #pragma HLS interface axis port=Acc4_txA1_1_heads0
    #pragma HLS interface axis port=Acc4_txA0_0_heads1
    #pragma HLS interface axis port=Acc4_txA0_1_heads1
    #pragma HLS interface axis port=Acc4_txA1_0_heads1
    #pragma HLS interface axis port=Acc4_txA1_1_heads1
    #pragma HLS interface axis port=Acc4_txA0_0_heads2
    #pragma HLS interface axis port=Acc4_txA0_1_heads2
    #pragma HLS interface axis port=Acc4_txA1_0_heads2
    #pragma HLS interface axis port=Acc4_txA1_1_heads2
    #pragma HLS interface axis port=Acc4_txB0_0_heads0 
    #pragma HLS interface axis port=Acc4_txB1_0_heads0 
    #pragma HLS interface axis port=Acc4_txB0_1_heads0 
    #pragma HLS interface axis port=Acc4_txB1_1_heads0 
    #pragma HLS interface axis port=Acc4_txB0_0_heads1 
    #pragma HLS interface axis port=Acc4_txB1_0_heads1 
    #pragma HLS interface axis port=Acc4_txB0_1_heads1 
    #pragma HLS interface axis port=Acc4_txB1_1_heads1 
    #pragma HLS interface axis port=Acc4_txB0_0_heads2 
    #pragma HLS interface axis port=Acc4_txB1_0_heads2 
    #pragma HLS interface axis port=Acc4_txB0_1_heads2 
    #pragma HLS interface axis port=Acc4_txB1_1_heads2 
    #pragma HLS interface axis port=Acc4_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc4_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc4_rxC1_0_heads0  
    #pragma HLS interface axis port=Acc4_rxC1_1_heads0  
    #pragma HLS interface axis port=Acc4_rxC0_0_heads1  
    #pragma HLS interface axis port=Acc4_rxC0_1_heads1  
    #pragma HLS interface axis port=Acc4_rxC1_0_heads1  
    #pragma HLS interface axis port=Acc4_rxC1_1_heads1  
    #pragma HLS interface axis port=Acc4_rxC0_0_heads2  
    #pragma HLS interface axis port=Acc4_rxC0_1_heads2  
    #pragma HLS interface axis port=Acc4_rxC1_0_heads2  
    #pragma HLS interface axis port=Acc4_rxC1_1_heads2  
    #pragma HLS interface axis port=Acc5_txA0_0_heads0
    #pragma HLS interface axis port=Acc5_txA0_1_heads0
    #pragma HLS interface axis port=Acc5_txA0_2_heads0
    #pragma HLS interface axis port=Acc5_txA0_3_heads0
    #pragma HLS interface axis port=Acc5_txA0_4_heads0
    #pragma HLS interface axis port=Acc5_txA0_5_heads0
    #pragma HLS interface axis port=Acc5_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc5_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc5_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc5_rxC0_3_heads0  
    #pragma HLS interface axis port=Acc6_txA0_0_heads0
    #pragma HLS interface axis port=Acc6_txA0_1_heads0
    #pragma HLS interface axis port=Acc6_txA0_2_heads0
    #pragma HLS interface axis port=Acc6_txA0_3_heads0
    #pragma HLS interface axis port=Acc6_txA1_0_heads0
    #pragma HLS interface axis port=Acc6_txA1_1_heads0
    #pragma HLS interface axis port=Acc6_txA1_2_heads0
    #pragma HLS interface axis port=Acc6_txA1_3_heads0
    #pragma HLS interface axis port=Acc6_txA2_0_heads0
    #pragma HLS interface axis port=Acc6_txA2_1_heads0
    #pragma HLS interface axis port=Acc6_txA2_2_heads0
    #pragma HLS interface axis port=Acc6_txA2_3_heads0
    #pragma HLS interface axis port=Acc6_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_3_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_4_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_5_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_6_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_7_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_8_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_9_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_10_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_11_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_12_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_13_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_14_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_15_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_16_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_17_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_18_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_19_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_20_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_21_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_22_heads0  
    #pragma HLS interface axis port=Acc6_rxC0_23_heads0  
    #pragma HLS interface axis port=Acc7_txA0_0_heads0
    #pragma HLS interface axis port=Acc7_txA0_1_heads0
    #pragma HLS interface axis port=Acc7_txA0_2_heads0
    #pragma HLS interface axis port=Acc7_txA0_3_heads0
    #pragma HLS interface axis port=Acc7_txA0_4_heads0
    #pragma HLS interface axis port=Acc7_txA0_5_heads0
    #pragma HLS interface axis port=Acc7_txA0_6_heads0
    #pragma HLS interface axis port=Acc7_txA0_7_heads0
    #pragma HLS interface axis port=Acc7_txA0_8_heads0
    #pragma HLS interface axis port=Acc7_txA0_9_heads0
    #pragma HLS interface axis port=Acc7_txA0_10_heads0
    #pragma HLS interface axis port=Acc7_txA0_11_heads0
    #pragma HLS interface axis port=Acc7_txA0_12_heads0
    #pragma HLS interface axis port=Acc7_txA0_13_heads0
    #pragma HLS interface axis port=Acc7_txA0_14_heads0
    #pragma HLS interface axis port=Acc7_txA0_15_heads0
    #pragma HLS interface axis port=Acc7_txA0_16_heads0
    #pragma HLS interface axis port=Acc7_txA0_17_heads0
    #pragma HLS interface axis port=Acc7_txA0_18_heads0
    #pragma HLS interface axis port=Acc7_txA0_19_heads0
    #pragma HLS interface axis port=Acc7_txA0_20_heads0
    #pragma HLS interface axis port=Acc7_txA0_21_heads0
    #pragma HLS interface axis port=Acc7_txA0_22_heads0
    #pragma HLS interface axis port=Acc7_txA0_23_heads0
    #pragma HLS interface axis port=Acc7_rxC0_0_heads0  
    #pragma HLS interface axis port=Acc7_rxC0_1_heads0  
    #pragma HLS interface axis port=Acc7_rxC0_2_heads0  
    #pragma HLS interface axis port=Acc7_rxC0_3_heads0  
    
                
    #pragma HLS interface axis port=Acc8_txA0_0
    #pragma HLS interface axis port=Acc8_txA0_1
    #pragma HLS interface axis port=Acc8_txA0_2
    #pragma HLS interface axis port=Acc8_txA0_3
    #pragma HLS interface axis port=Acc8_txA0_4
    #pragma HLS interface axis port=Acc8_txA0_5
    #pragma HLS interface axis port=Acc8_txA0_6
    #pragma HLS interface axis port=Acc8_txA0_7
    #pragma HLS interface axis port=Acc8_txA0_8
    #pragma HLS interface axis port=Acc8_txA0_9
    #pragma HLS interface axis port=Acc8_txA0_10
    #pragma HLS interface axis port=Acc8_txA0_11
     
    #pragma HLS interface axis port=Acc8_rxC0_0
    #pragma HLS interface axis port=Acc8_rxC0_1
    #pragma HLS interface axis port=Acc8_rxC0_2
    #pragma HLS interface axis port=Acc8_rxC0_3
     
    #pragma HLS interface axis port=Acc9_txA0_0
    #pragma HLS interface axis port=Acc9_txA0_1
     
    #pragma HLS interface axis port=Acc9_rxC0_0
    #pragma HLS interface axis port=Acc9_rxC0_1
    #pragma HLS interface axis port=Acc9_rxC0_2
    #pragma HLS interface axis port=Acc9_rxC0_3
    #pragma HLS interface axis port=Acc9_rxC0_4
     
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
                    Acc0_rxC0_0_heads0, Acc0_rxC0_1_heads0, Acc0_rxC0_2_heads0, Acc0_rxC0_3_heads0, Acc0_rxC0_4_heads0, Acc0_rxC0_5_heads0,  
                    Acc1_txA0_0_heads0, Acc1_txA0_1_heads0, Acc1_txA0_2_heads0, Acc1_txA0_3_heads0,  
                    Acc1_rxC0_0_heads0, Acc1_rxC0_1_heads0, Acc1_rxC0_2_heads0, Acc1_rxC0_3_heads0, Acc1_rxC0_4_heads0, Acc1_rxC0_5_heads0,  
                    Acc2_txA0_0_heads0, Acc2_txA0_1_heads0, Acc2_txA0_2_heads0, Acc2_txA0_3_heads0,  
                    Acc2_rxC0_0_heads0, Acc2_rxC0_1_heads0, Acc2_rxC0_2_heads0, Acc2_rxC0_3_heads0, Acc2_rxC0_4_heads0, Acc2_rxC0_5_heads0,  
                    Acc3_txA0_0_heads0, Acc3_txA0_1_heads0,  
                    Acc3_txA1_0_heads0, Acc3_txA1_1_heads0,  
                    Acc3_txA0_0_heads1, Acc3_txA0_1_heads1,  
                    Acc3_txA1_0_heads1, Acc3_txA1_1_heads1,  
                    Acc3_txA0_0_heads2, Acc3_txA0_1_heads2,  
                    Acc3_txA1_0_heads2, Acc3_txA1_1_heads2,  
                    Acc3_txB0_0_heads0, Acc3_txB1_0_heads0,  
                    Acc3_txB0_1_heads0, Acc3_txB1_1_heads0,  
                    Acc3_txB0_0_heads1, Acc3_txB1_0_heads1,  
                    Acc3_txB0_1_heads1, Acc3_txB1_1_heads1,  
                    Acc3_txB0_0_heads2, Acc3_txB1_0_heads2,  
                    Acc3_txB0_1_heads2, Acc3_txB1_1_heads2,  
                    Acc3_rxC0_0_heads0, Acc3_rxC0_1_heads0,  
                    Acc3_rxC1_0_heads0, Acc3_rxC1_1_heads0,  
                    Acc3_rxC0_0_heads1, Acc3_rxC0_1_heads1,  
                    Acc3_rxC1_0_heads1, Acc3_rxC1_1_heads1,  
                    Acc3_rxC0_0_heads2, Acc3_rxC0_1_heads2,  
                    Acc3_rxC1_0_heads2, Acc3_rxC1_1_heads2,  
                    Acc4_txA0_0_heads0, Acc4_txA0_1_heads0,  
                    Acc4_txA1_0_heads0, Acc4_txA1_1_heads0,  
                    Acc4_txA0_0_heads1, Acc4_txA0_1_heads1,  
                    Acc4_txA1_0_heads1, Acc4_txA1_1_heads1,  
                    Acc4_txA0_0_heads2, Acc4_txA0_1_heads2,  
                    Acc4_txA1_0_heads2, Acc4_txA1_1_heads2,  
                    Acc4_txB0_0_heads0, Acc4_txB1_0_heads0,  
                    Acc4_txB0_1_heads0, Acc4_txB1_1_heads0,  
                    Acc4_txB0_0_heads1, Acc4_txB1_0_heads1,  
                    Acc4_txB0_1_heads1, Acc4_txB1_1_heads1,  
                    Acc4_txB0_0_heads2, Acc4_txB1_0_heads2,  
                    Acc4_txB0_1_heads2, Acc4_txB1_1_heads2,  
                    Acc4_rxC0_0_heads0, Acc4_rxC0_1_heads0,  
                    Acc4_rxC1_0_heads0, Acc4_rxC1_1_heads0,  
                    Acc4_rxC0_0_heads1, Acc4_rxC0_1_heads1,  
                    Acc4_rxC1_0_heads1, Acc4_rxC1_1_heads1,  
                    Acc4_rxC0_0_heads2, Acc4_rxC0_1_heads2,  
                    Acc4_rxC1_0_heads2, Acc4_rxC1_1_heads2,  
                    Acc5_txA0_0_heads0, Acc5_txA0_1_heads0, Acc5_txA0_2_heads0, Acc5_txA0_3_heads0, Acc5_txA0_4_heads0, Acc5_txA0_5_heads0,  
                    Acc5_rxC0_0_heads0, Acc5_rxC0_1_heads0, Acc5_rxC0_2_heads0, Acc5_rxC0_3_heads0,  
                    Acc6_txA0_0_heads0, Acc6_txA0_1_heads0, Acc6_txA0_2_heads0, Acc6_txA0_3_heads0,  
                    Acc6_txA1_0_heads0, Acc6_txA1_1_heads0, Acc6_txA1_2_heads0, Acc6_txA1_3_heads0,  
                    Acc6_txA2_0_heads0, Acc6_txA2_1_heads0, Acc6_txA2_2_heads0, Acc6_txA2_3_heads0,  
                    Acc6_rxC0_0_heads0, Acc6_rxC0_1_heads0, Acc6_rxC0_2_heads0, Acc6_rxC0_3_heads0, Acc6_rxC0_4_heads0, Acc6_rxC0_5_heads0, Acc6_rxC0_6_heads0, Acc6_rxC0_7_heads0, Acc6_rxC0_8_heads0, Acc6_rxC0_9_heads0, Acc6_rxC0_10_heads0, Acc6_rxC0_11_heads0, Acc6_rxC0_12_heads0, Acc6_rxC0_13_heads0, Acc6_rxC0_14_heads0, Acc6_rxC0_15_heads0, Acc6_rxC0_16_heads0, Acc6_rxC0_17_heads0, Acc6_rxC0_18_heads0, Acc6_rxC0_19_heads0, Acc6_rxC0_20_heads0, Acc6_rxC0_21_heads0, Acc6_rxC0_22_heads0, Acc6_rxC0_23_heads0,  
                    Acc7_txA0_0_heads0, Acc7_txA0_1_heads0, Acc7_txA0_2_heads0, Acc7_txA0_3_heads0, Acc7_txA0_4_heads0, Acc7_txA0_5_heads0, Acc7_txA0_6_heads0, Acc7_txA0_7_heads0, Acc7_txA0_8_heads0, Acc7_txA0_9_heads0, Acc7_txA0_10_heads0, Acc7_txA0_11_heads0, Acc7_txA0_12_heads0, Acc7_txA0_13_heads0, Acc7_txA0_14_heads0, Acc7_txA0_15_heads0, Acc7_txA0_16_heads0, Acc7_txA0_17_heads0, Acc7_txA0_18_heads0, Acc7_txA0_19_heads0, Acc7_txA0_20_heads0, Acc7_txA0_21_heads0, Acc7_txA0_22_heads0, Acc7_txA0_23_heads0,  
                    Acc7_rxC0_0_heads0, Acc7_rxC0_1_heads0, Acc7_rxC0_2_heads0, Acc7_rxC0_3_heads0,  
                    Acc8_txA0_0, Acc8_txA0_1, Acc8_txA0_2, Acc8_txA0_3, Acc8_txA0_4, Acc8_txA0_5, Acc8_txA0_6, Acc8_txA0_7, Acc8_txA0_8, Acc8_txA0_9, Acc8_txA0_10, Acc8_txA0_11,  
                    Acc8_rxC0_0, Acc8_rxC0_1, Acc8_rxC0_2, Acc8_rxC0_3,  
                    Acc9_txA0_0, Acc9_txA0_1,  
                    Acc9_rxC0_0, Acc9_rxC0_1, Acc9_rxC0_2, Acc9_rxC0_3, Acc9_rxC0_4,  
                    dataA_in0, dataA_in1, dataA_in2, dataC_out, batch, block, iteration);

    address_C_ddr(addrC_in,batch,iteration);
    storeC_ddr(out0,addrC_in,dataC_out,batch,iteration);
}