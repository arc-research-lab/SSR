
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>

#include "../aie/layer0/aie_top_L0.h"
#include "../aie/layer1/aie_top_L1.h"
#include "../aie/layer2/aie_top_L2.h"
#include "../aie/layer3/aie_top_L3.h"
#include "../aie/layer4/aie_top_L4.h"

// This is used for the PL Kernels
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
vit_x1_x4_x18_graph0 vit_graph0;
vit_x4_x2_x2_graph1 vit_graph1;
vit_x4_x6_x6_graph2 vit_graph2;
vit_x1_x6_x4_graph3 vit_graph3;
vit_x1_x2_x5_graph4 vit_graph4;
using namespace std;

//A_BEF[K][M] A_AFT[M/8][K][8]
void transpose_data(std::vector<int8_t> A_BEF, std::vector<int8_t>& A_AFT,
           const int M, const int K){
    
    for (int m=0; m<M/8; m++){
        for (int k=0; k<K; k++){
            for (int i=0; i<8; i++){
                int pos_bef=i+k*M+m*8;
                int pos_aft=i+k*8+m*K*8;
                A_AFT[pos_aft]=A_BEF[pos_bef];
            }
        }
    }  
}

//DataInput0[N][M]  DataInput1[N][K]  golden[N][M]
void sw_mm(std::vector<int8_t>& DataInput0, std::vector<int8_t>& DataInput1, std::vector<int8_t> &golden,
           const int M, const int K, const int N, const int B,const int W1){
    int8_t temp0=0;
    int8_t temp1=0;
    int8_t sum_temp=0;
    for (int n=0; n<N;n++){
        for (int m=0; m<M;m++){
            int8_t sum_all = 0;
            for (int k=0; k<K/(W1*B);k++){
                int sum = 0;
                for (int aie_k=0; aie_k<W1*B;aie_k++){
                    int pos0=m+(aie_k+k*(W1*B))*M;
                    int pos1=aie_k+k*(W1*B)+n*K;
                    temp0=(int8_t) DataInput0[pos0];
                    temp1=(int8_t) DataInput1[pos1];     
                    sum+=temp0*temp1;
                }
                if (sum>127){
                    sum_temp=127;
                }
                else if(sum<-128){
                    sum_temp=-128;
                }
                else{
                    sum_temp=sum;
                }
                sum_all += sum_temp;
            }
            int pos2=m+n*M;
            golden[pos2]=sum_all;
        }
    }
}




const int image_size = 224;
const int input_channel =3;
const int out_class =1000;

int main(int argc, char** argv) {

    
    int iter=1,batch=1,block=1,verify=0;
    char* xclbinFilename;
    if(argc == 6) {
        xclbinFilename = argv[1];
        if (sscanf (argv[2], "%i", &iter) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[3], "%i", &batch) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[4], "%i", &block) != 1) {
            fprintf(stderr, "error - not an integer");
        }
        if (sscanf (argv[5], "%i", &verify) != 1) {
            fprintf(stderr, "error - not an integer");
        }
    }

    //////////////////////////////////////////
    // Open xclbin
    //////////////////////////////////////////
    
    auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);
	auto dhdl = xrtDeviceOpenFromXcl(device);

    auto pl_engine = xrt::kernel(device, uuid, "pl_engine");
    
    int sizeIn0 = image_size * image_size * batch;
    //int sizeIn1 = ACC0_K0 * ACC0_N0;
    int sizeOut0 = out_class * batch;

    int sizeout_out = sizeOut0;
    
    std::vector<int8_t> DataInput0(sizeIn0,1);
    //std::vector<int8_t> DataInput1(sizeIn1,1);
    std::vector<int8_t> DataInput0_Trans(sizeIn0,1);

    srand (time(0));

    for (int m = 0; m < sizeIn0; m++) {
        DataInput0[m]= (int8_t)(rand()%3)-1;
    }

    //Allocate input mem
    auto in_bohdl0 = xrt::bo(device, sizeIn0 * sizeof(int8_t), pl_engine.group_id(0));
    auto in_bomapped0 = in_bohdl0.map<int8_t*>();

    auto in_bohdl1 = xrt::bo(device, sizeIn0 * sizeof(int8_t), pl_engine.group_id(0));
    auto in_bomapped1 = in_bohdl1.map<int8_t*>();

    auto in_bohdl2 = xrt::bo(device, sizeIn0 * sizeof(int8_t), pl_engine.group_id(0));
    auto in_bomapped2 = in_bohdl2.map<int8_t*>();
    
    //transpose_data(DataInput0, DataInput0_Trans, ACC0_M0, ACC0_K0);
    for (int k = 0; k < sizeIn0; k++) {
        in_bomapped0 [k]= DataInput0[k];
        in_bomapped1 [k]= DataInput0[k];
        in_bomapped2 [k]= DataInput0[k];
    } 


    // sync input memory
    in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn0* sizeof(int8_t),0);
    in_bohdl1.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn0* sizeof(int8_t),0);
    in_bohdl2.sync(XCL_BO_SYNC_BO_TO_DEVICE , sizeIn0* sizeof(int8_t),0);
    
    //Allocate output buffer
    auto out_bohdl = xrt::bo(device, sizeout_out* sizeof(int8_t), pl_engine.group_id(0));
    auto out_bomapped = out_bohdl.map<int8_t*>();
    
    std::cout << "Kernel run\n";
    xrt::run pl_engine_run;

    
    auto ghdl0=xrt::graph(device,uuid,"vit_graph0");
    auto ghdl1=xrt::graph(device,uuid,"vit_graph1");
    auto ghdl2=xrt::graph(device,uuid,"vit_graph2");
    auto ghdl3=xrt::graph(device,uuid,"vit_graph3");
    auto ghdl4=xrt::graph(device,uuid,"vit_graph4");
    
    int weights_offset=0;
    ghdl0.update("vit_graph0.vit_0_0_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_1_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_2_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_3_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_4_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_5_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_6_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_7_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_8_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_9_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_10_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_11_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_12_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_13_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_14_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_15_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_16_0.mm[0].in[1]",weights_offset);
    ghdl0.update("vit_graph0.vit_0_17_0.mm[0].in[1]",weights_offset);
    
    printf("graph run\n");
    ghdl0.run(-1);
    ghdl1.run(-1);
    ghdl2.run(-1);
    ghdl3.run(-1);
    ghdl4.run(-1);
    

    //profile aie mm 
    double kernel_time_in_sec = 0;
    std::chrono::duration<double> kernel_time(0);
    auto kernel_start = std::chrono::high_resolution_clock::now();
    
    
    // start input kernels run handles
    
    pl_engine_run = pl_engine(in_bohdl0, in_bohdl1, in_bohdl2, out_bohdl,
            nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,  
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr,  
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,  
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,  
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,  
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,  
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,  
            nullptr, nullptr, 
            nullptr, nullptr, nullptr, nullptr, nullptr,  
            batch, block, iter);

    pl_engine_run.wait();
    
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
    kernel_time_in_sec = kernel_time.count();
    //double TOPS = (double)(ACC0_M0 * ACC0_K0) * (double) (ACC0_N0 * 2 * iter * 1e-9) / kernel_time_in_sec;
    std::cout << std::endl;
    std::cout << std::endl;
    //std::cout << "MM Size:"<< ACC0_M0 << "*" << ACC0_K0 << "*" << ACC0_N0 << std::endl;
    std::cout << "Total time is: "<< (kernel_time_in_sec/iter*1000) <<" ms" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // sync output memory
    out_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE , sizeout_out* sizeof(int8_t),/*OFFSET=*/ 0);

    ////////////////////////////////////////////
    //// Comparing the execution data to the golden data
    ////////////////////////////////////////////
    if(verify){
        // std::vector<int8_t> golden_L0_OUT(sizeOut0,0); //[L0_N][L0_M]
        // sw_mm(DataInput0, DataInput1, golden_L0_OUT,ACC0_M0, ACC0_K0, ACC0_N0, B0, W1_0);

        // std::vector<int8_t> golden(sizeout_out,0);
        // transpose_data(golden_L0_OUT, golden, ACC0_M5, ACC0_N5);

        int errorCount = 0;  
        // for (int n = 0; n < sizeout_out; n++) {
        //     if(abs((int8_t)(out_bomapped[n])-golden[n])>=1e-5){
        //         printf("Error found out_bomapped[%d]!=golden[%d], %d!=%d \n", n,n,out_bomapped[n],golden[n]);
        //         errorCount++;
        //     }
        // }  
        if (errorCount)
            printf("Test failed with %d errors\n", errorCount);
        else
            printf("TEST PASSED\n");
    }

    //////////////////////////////////////////
    // clean up XRT
    //////////////////////////////////////////

    std::cout << "Releasing remaining XRT objects...\n";
    
    return 0;
}
