#include <adf.h>
#include <stdio.h>
#include "para_L4.h"

void mm1_kernel3_L4(input_window_int8* __restrict matA,
        input_window_int8* __restrict matB,
		output_stream_int8* __restrict matC){
    
    set_sat();
	v64int8 chess_storage(xa) buf_matA0=undef_v64int8();
	v64int8 chess_storage(xb) buf_matA1=undef_v64int8();
			
	v32int8 chess_storage(wc0) buf_matB0=undef_v32int8(); 
	v32int8 chess_storage(wc1) buf_matB1=undef_v32int8(); 
	 
    for (int i=0;i<L4_boundry_i;i++)
	chess_prepare_for_pipelining
	chess_loop_range(L4_boundry_i,)
	{ //h1/8
        for (int j=0;j<L4_boundry_j;j++)
	    chess_prepare_for_pipelining
	    chess_loop_range(L4_boundry_j,)
        { //W2/8

            v16acc48 acc0=null_v16acc48();
            v16acc48 acc1=null_v16acc48();
            buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
	        buf_matB0=window_read_v32(matB);
            window_incr(matB,L4_jumpB1);

	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);

            //K iteration 0
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L4_jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,L4_jumpB1);
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            buf_matB0=window_read_v32(matB); 
            window_incr(matB,L4_jumpB1);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            //K iteration 1
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L4_jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,L4_jumpB1);
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            buf_matB0=window_read_v32(matB); 
            window_incr(matB,L4_jumpB1);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            //K iteration 2
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L4_jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,L4_jumpB1);
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            buf_matB0=window_read_v32(matB); 
            window_incr(matB,L4_jumpB1);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            //K iteration 3
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L4_jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,L4_jumpB1);
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            buf_matB0=window_read_v32(matB); 
            window_incr(matB,L4_jumpB1);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            //K iteration 4
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L4_jumpA0);
            buf_matB1=window_read_v32(matB);
            window_incr(matB,L4_jumpB1);
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            buf_matB0=window_read_v32(matB); 
            window_incr(matB,L4_jumpB1);

            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
	        buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA0);
            
            //K iteration 5
            acc0=mac16(acc0,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0x44440000,2,0x3210); 
            buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
            window_incr(matA,L4_jumpA0);
            buf_matB1=window_read_v32(matB);
            window_decr(matB,L4_jumpB2);
            
            acc1=mac16(acc1,buf_matA0,0,0x11101110,16,0x3120,buf_matB0,0,0xCCCC8888,2,0x3210); 
	        buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	        window_incr(matA,L4_jumpA2);
            
            acc0=mac16(acc0,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0x44440000,2,0x3210); 
            writeincr_v16(matC,bsrs(acc0,0));
            
            acc1=mac16(acc1,buf_matA1,0,0x11101110,16,0x3120,buf_matB1,0,0xCCCC8888,2,0x3210); 
            writeincr_v16(matC,bsrs(acc1,0));

        }
        window_decr(matA,L4_jumpA1);
        window_decr(matB,L4_jumpB1);
    }

}