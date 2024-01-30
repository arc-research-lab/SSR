#include <adf.h>
#include <stdio.h>
#include "para_L7.h"
#include "parameter_L7_B13_A0_C1.h"

void mm0_kernel1_L7_B13_A0_C1(input_window_int8* __restrict matA, 
        input_stream_acc48* __restrict acc_in,
		output_stream_acc48* __restrict matC){
    
    v32int8 *restrict matB = (v32int8 *)matB_LUT0;

	v64int8 chess_storage(xa) buf_matA0=undef_v64int8();
	v64int8 chess_storage(xb) buf_matA1=undef_v64int8();
			
	v32int8 chess_storage(wc0) buf_matB0=undef_v32int8(); 
	v32int8 chess_storage(wc1) buf_matB1=undef_v32int8(); 
	
    int weights_offset;
    v8acc48 temp_acc=readincr_v8(acc_in);
    writeincr_v8(matC,temp_acc);
    v8int32 temp=lsrs(temp_acc,0);
    weights_offset=ext_elem(temp,0);
    matB += weights_offset;
    
    for (int i=0;i<L7_boundry_i;i++)
	chess_prepare_for_pipelining
	chess_loop_range(L7_boundry_i,)
	{ //h1/8
        for (int j=0;j<L7_boundry_j;j++)
	    chess_prepare_for_pipelining
	    chess_loop_range(L7_boundry_j,)
        { //W2/8

            v16acc48 acc0=concat(readincr_v8(acc_in),readincr_v8(acc_in));//chess_storage(bm0) 
	        v16acc48 acc1=concat(readincr_v8(acc_in),readincr_v8(acc_in));//chess_storage(bm1) 
            buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L7_jumpA0);
	        buf_matB0=*matB;
            matB +=L7_jumpB1/32;

	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L7_jumpA0);

            //K iteration 0
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L7_jumpA0);
            buf_matB1=*matB;
            matB +=L7_jumpB1/32;
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L7_jumpA0);
            
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L7_jumpA0);
            buf_matB0=*matB; 
            matB +=L7_jumpB1/32;

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L7_jumpA0);
            
            //K iteration 1
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L7_jumpA0);
            buf_matB1=*matB;
            matB -=L7_jumpB2/32;
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L7_jumpA2);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
            writeincr_v8(matC,ext_lo(acc0));
			writeincr_v8(matC,ext_hi(acc0));

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
            writeincr_v8(matC,ext_lo(acc1));
			writeincr_v8(matC,ext_hi(acc1));

        }
        window_decr(matA,L7_jumpA1);
        matB -=L7_jumpB1/32;
    }

}