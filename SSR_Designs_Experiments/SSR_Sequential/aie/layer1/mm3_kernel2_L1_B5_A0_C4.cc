#include <adf.h>
#include <stdio.h>
#include "para_L1.h"
#include "parameter_L1_B5_A0_C4.h"

void mm3_kernel2_L1_B5_A0_C4(input_window_int8* __restrict matA, 
		input_stream_acc48* __restrict acc_in,
		output_window_int8* __restrict matC){
    
    set_sat();
    v32int8 *restrict matB = (v32int8 *)matB_LUT0;

	v64int8 chess_storage(xa) buf_matA0=undef_v64int8(); //
	v64int8 chess_storage(xb) buf_matA1=undef_v64int8(); //
			
	v32int8 chess_storage(wc0) buf_matB0=undef_v32int8(); // 
	v32int8 chess_storage(wc1) buf_matB1=undef_v32int8(); // 
    
    for (int seq=0;seq<L1_bound_seq1;seq++)
	    chess_prepare_for_pipelining
	    chess_loop_range(L1_bound_seq1,)
    {
        for (int seq_t2=0;seq_t2<2;seq_t2++)
	    chess_prepare_for_pipelining
	    chess_loop_range(2,)
        {
            for (int oc=0;oc<L1_bound_oc;oc++)
	        chess_prepare_for_pipelining
	        chess_loop_range(L1_bound_oc,)
            { //OC/4

                v8acc48 acc0=readincr_v8(acc_in);
                v8acc48 acc1=readincr_v8(acc_in);

                buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;

	            buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);

                //K iteration 0
                acc0=mac8(acc0,buf_matA0,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); 
                buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA0,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210); 
                buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;


                //K iteration 1
                acc0=mac8(acc0,buf_matA1,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); 
                buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA1,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210);
                buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;
                //K iteration 2
                acc0=mac8(acc0,buf_matA0,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); 
                buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA0,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210); 
                buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;


                //K iteration 3
                acc0=mac8(acc0,buf_matA1,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); 
                buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA1,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210);
                buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;
                //K iteration 4
                acc0=mac8(acc0,buf_matA0,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); 
                buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA0,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210); 
                buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;


                //K iteration 5
                acc0=mac8(acc0,buf_matA1,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); 
                buf_matA0=upd_w(buf_matA0,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA1,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210);
                buf_matA0=upd_w(buf_matA0,1,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;
                
                //K iteration 6
                acc0=mac8(acc0,buf_matA0,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); // Seq:0-3, KW:0-15, OC:0-1, KH 14
                buf_matA1=upd_w(buf_matA1,0,window_read_v32(matA));
	            window_incr(matA,L1_jumpA0);
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA0,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210); // Seq:0-3, KW:0-15, OC:2-3, KH 14
                buf_matA1=upd_w(buf_matA1,1,window_read_v32(matA));
	            window_decr(matA,L1_jumpA2);
	            buf_matB0=*matB;
                matB +=L1_jumpB0/32;


                //K iteration 7
                acc0=mac8(acc0,buf_matA1,0,0x0000,8,0x3120,buf_matB0,0,0x8800,2,0x3210); // Seq:0-3, KW:0-15, OC:0-1, KH 15
	            buf_matB1=*matB;
                matB +=L1_jumpB0/32;

                acc1=mac8(acc1,buf_matA1,0,0x0000,8,0x3120,buf_matB1,0,0x8800,2,0x3210); // Seq:0-3, KW:0-15, OC:2-3, KH 15

                v16int8 temp=bsrs(concat(acc0,acc1),0);
                window_write(matC,temp);
	            window_incr(matC,L1_jumpC0);
            }
            window_incr(matA,L1_jumpA1);
            matB -=L1_jumpB3/32;
            window_decr(matC,L1_jumpC1);
        }
        window_incr(matC,L1_jumpC2);
    }
}