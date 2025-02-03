#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_vector.h"
#include "hls_stream.h"


#define NHITS  150
// #define NEDGES 256
#define NEDGES 250
// #define NHITS  21
// #define NEDGES 42
#define NPARAMS  3

#define NHIDDEN  8 // original
// #define NHIDDEN  9
// #define NHIDDEN  16
// #define NHIDDEN  24
// #define NHIDDEN  128

// #define STREAM
// #define VECTOR
#define ARRAY

// #define DISABLE_INPUT
// #define DISABLE_EDGE
// #define DISABLE_NODE

// dont forget to update if NPARAMS or NHIDDEN changed

#define NPARHID  11 //NPARAMS + NHIDDEN
#define NPARHID2 22 //NPARHID + NPARHID
#define NPARHID3 33 //NPARHID + NPARHID + NPARHID
// took 1927.84s to compile, ie 32 minutes, total 36m 21s
// 24,841 Compile/Link
// 2,023,874 Unroll/Inline
// 1,259,731 Step 2
// DSP 6002 (87%)|  FF 470825 (19%)|  LUT 388181 (32%)

// #define NPARHID  12 //NPARAMS + NHIDDEN
// #define NPARHID2 24 //NPARHID + NPARHID
// #define NPARHID3 36 //NPARHID + NPARHID + NPARHID
// // took 2237.78s to compile, ie 37 minutes, total 44m 27s
// // 24,851 Compile/Link
// // 2,221,253 Unroll/Inline
// // 1,375,393 Step 2
// // DSP 6601 (96%)|  FF 511333 (21%)|  LUT 422455 (35%)

// #define NPARHID  19 //NPARAMS + NHIDDEN
// #define NPARHID2 38 //NPARHID + NPARHID
// #define NPARHID3 57 //NPARHID + NPARHID + NPARHID

// #define NPARHID  27 //NPARAMS + NHIDDEN
// #define NPARHID2 54 //NPARHID + NPARHID
// #define NPARHID3 81 //NPARHID + NPARHID + NPARHID

// #define NPARHID  131 //NPARAMS + NHIDDEN
// #define NPARHID2 262 //NPARHID + NPARHID
// #define NPARHID3 393 //NPARHID + NPARHID + NPARHID


typedef ap_fixed<16,6> data_t;
// typedef ap_fixed<24, 12> data_t;
typedef ap_uint<8> i_data_t; // max nodes: 256

typedef hls::vector<data_t, NPARHID> par_t;
typedef hls::vector<par_t, 2> par2_t;
// typedef hls::vector<par_t, 3> par3_t;


extern "C"{

void runGraphNetwork(hls::stream<data_t>& X_stream, hls::stream<i_data_t>& ei_stream, hls::stream<data_t>& e_stream);
// void runGraphNetwork(data_t X_arr[NHITS * NPARAMS], i_data_t edge_i_arr[NEDGES * 2], data_t e_arr[NEDGES]);

}

#endif