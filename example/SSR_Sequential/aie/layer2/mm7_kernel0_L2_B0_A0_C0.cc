#include <adf.h>
#include <stdio.h>
#include "para_L2.h"
#include "parameter_L2_B0_A0_C0.h"

void mm7_kernel0_L2_B0_A0_C0(input_window_int8* __restrict matA,
		output_stream_acc48* __restrict matC){
    
    v32int8 *restrict matB = (v32int8 *)matB_LUT0;

	v64int8 chess_storage(xa) buf_matB0=undef_v64int8();
	v64int8 chess_storage(xb) buf_matB1=undef_v64int8();
			
	v32int8 chess_storage(wc0) buf_matA0=undef_v32int8(); 
	v32int8 chess_storage(wc1) buf_matA1=undef_v32int8(); 

  
    for (int j=0;j<L2_boundry_j;j++)
	chess_prepare_for_pipelining
	chess_loop_range(L2_boundry_j,)
    { //W2/8

        v16acc48 acc0=null_v16acc48();

        buf_matA0=window_read_v32(matA); 
        window_incr(matA,L2_jumpA0);
        buf_matB0=upd_w(buf_matB0,0,*matB);
        matB +=L2_jumpB0/32;
	    buf_matB0=upd_w(buf_matB0,1,*matB);
	    matB +=L2_jumpB0/32;
        
        //K iteration 0
        acc0=mac16(acc0,buf_matB0,0,0x11101110,16,0x3120,buf_matA0,0,0x44440000,2,0x3210);
        buf_matB1=upd_w(buf_matB1,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB1=upd_w(buf_matB1,1,*matB);
        matB +=L2_jumpB0/32;
        buf_matA1=window_read_v32(matA); 
        window_incr(matA,L2_jumpA0);

        //K iteration 1
        acc0=mac16(acc0,buf_matB1,0,0x11101110,16,0x3120,buf_matA0,0,0xCCCC8888,2,0x3210);
	    buf_matB0=upd_w(buf_matB0,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB0=upd_w(buf_matB0,1,*matB);
        matB +=L2_jumpB0/32;

        //K iteration 2
        acc0=mac16(acc0,buf_matB0,0,0x11101110,16,0x3120,buf_matA1,0,0x44440000,2,0x3210);
        buf_matB1=upd_w(buf_matB1,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB1=upd_w(buf_matB1,1,*matB);
        matB +=L2_jumpB0/32;
        buf_matA0=window_read_v32(matA); 
        window_incr(matA,L2_jumpA0);

        //K iteration 3
        acc0=mac16(acc0,buf_matB1,0,0x11101110,16,0x3120,buf_matA1,0,0xCCCC8888,2,0x3210);
	    buf_matB0=upd_w(buf_matB0,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB0=upd_w(buf_matB0,1,*matB);
        matB +=L2_jumpB0/32;
        //K iteration 4
        acc0=mac16(acc0,buf_matB0,0,0x11101110,16,0x3120,buf_matA0,0,0x44440000,2,0x3210);
        buf_matB1=upd_w(buf_matB1,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB1=upd_w(buf_matB1,1,*matB);
        matB +=L2_jumpB0/32;
        buf_matA1=window_read_v32(matA); 
        window_incr(matA,L2_jumpA0);

        //K iteration 5
        acc0=mac16(acc0,buf_matB1,0,0x11101110,16,0x3120,buf_matA0,0,0xCCCC8888,2,0x3210);
	    buf_matB0=upd_w(buf_matB0,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB0=upd_w(buf_matB0,1,*matB);
        matB +=L2_jumpB0/32;

        //K iteration 6
        acc0=mac16(acc0,buf_matB0,0,0x11101110,16,0x3120,buf_matA1,0,0x44440000,2,0x3210);
        buf_matB1=upd_w(buf_matB1,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB1=upd_w(buf_matB1,1,*matB);
        matB +=L2_jumpB0/32;
        buf_matA0=window_read_v32(matA); 
        window_incr(matA,L2_jumpA0);

        //K iteration 7
        acc0=mac16(acc0,buf_matB1,0,0x11101110,16,0x3120,buf_matA1,0,0xCCCC8888,2,0x3210);
	    buf_matB0=upd_w(buf_matB0,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB0=upd_w(buf_matB0,1,*matB);
        matB +=L2_jumpB0/32;
        
        //K iteration 8
        acc0=mac16(acc0,buf_matB0,0,0x11101110,16,0x3120,buf_matA0,0,0x44440000,2,0x3210); // W2:0-7, W1:64-71, H1:0-1
        buf_matB1=upd_w(buf_matB1,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB1=upd_w(buf_matB1,1,*matB);
        matB +=L2_jumpB0/32;
        buf_matA1=window_read_v32(matA); 
        window_incr(matA,L2_jumpA0);

        //K iteration 9
        acc0=mac16(acc0,buf_matB1,0,0x11101110,16,0x3120,buf_matA0,0,0xCCCC8888,2,0x3210); // W2:0-7, W1:72-79, H1:0-1
	    buf_matB0=upd_w(buf_matB0,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB0=upd_w(buf_matB0,1,*matB);
        matB +=L2_jumpB0/32;

        //K iteration 10
        acc0=mac16(acc0,buf_matB0,0,0x11101110,16,0x3120,buf_matA1,0,0x44440000,2,0x3210); // W2:0-7, W1:80-87, H1:0-1
        buf_matB1=upd_w(buf_matB1,0,*matB);
        matB +=L2_jumpB0/32;
        buf_matB1=upd_w(buf_matB1,1,*matB);
        matB +=L2_jumpB0/32;

        //K iteration 11
        acc0=mac16(acc0,buf_matB1,0,0x11101110,16,0x3120,buf_matA1,0,0xCCCC8888,2,0x3210); // W2:0-7, W1:88-95, H1:0-1
        writeincr_v8(matC,ext_lo(acc0));
		writeincr_v8(matC,ext_hi(acc0));

    }
}