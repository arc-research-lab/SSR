#include <adf.h>
#include <stdio.h>
#include "para_L6.h"
#include "parameter_L6_B0_A0_C16.h"

void mm0_kernel0_L6_B0_A0_C16(input_window_int8* __restrict matA,
        const int32 weights_offset,
		output_stream_acc48* __restrict matC){
    
    v32int8 *restrict matB = (v32int8 *)matB_LUT0;

	v64int8 chess_storage(xa) buf_matA0=undef_v64int8();
	v64int8 chess_storage(xb) buf_matA1=undef_v64int8();
			
	v32int8 chess_storage(wc0) buf_matB0=undef_v32int8(); 
	v32int8 chess_storage(wc1) buf_matB1=undef_v32int8(); 
	
    v8int32 temp=upd_elem(undef_v8int32(),0,weights_offset);
    v8acc48 temp_acc=ups(temp,0);
    writeincr_v8(matC,temp_acc);
    matB  += weights_offset;

    for (int i=0;i<L6_boundry_i;i++)
	chess_prepare_for_pipelining
	chess_loop_range(L6_boundry_i,)
	{ //h1/8
        for (int j=0;j<L6_boundry_j;j++)
	    chess_prepare_for_pipelining
	    chess_loop_range(L6_boundry_j,)
        { //W2/8

            v16acc48 acc0=null_v16acc48();
            v16acc48 acc1=null_v16acc48();
            buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
	        buf_matB0=*matB;
            matB +=L6_jumpB1/32;

	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            
            //K iteration 0
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L6_jumpA0);
            buf_matB1=*matB;
            matB +=L6_jumpB1/32;
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            buf_matB0=*matB; 
            matB +=L6_jumpB1/32;

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            //K iteration 1
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L6_jumpA0);
            buf_matB1=*matB;
            matB +=L6_jumpB1/32;
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            buf_matB0=*matB; 
            matB +=L6_jumpB1/32;

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210);
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L6_jumpA0);
            
            //K iteration 2
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210);
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L6_jumpA0);
            buf_matB1=*matB;
            matB -=L6_jumpB2/32;
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210);
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L6_jumpA2);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210);
            writeincr_v8(matC,ext_lo(acc0));
			writeincr_v8(matC,ext_hi(acc0));

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210);
            writeincr_v8(matC,ext_lo(acc1));
			writeincr_v8(matC,ext_hi(acc1));

        }
        window_decr(matA,L6_jumpA1);
        matB -=L6_jumpB1/32;
    }

}