#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_vector.h"
#include "hls_stream.h"


#define NHITS  150
#define NEDGES 256
// #define NHITS  21
// #define NEDGES 42
#define NPARAMS  2
#define NHIDDEN  8

// #define STREAM
// #define VECTOR
#define ARRAY

// #define DISABLE_INPUT
// #define DISABLE_EDGE
// #define DISABLE_NODE

// dont forget to update if NPARAMS or NHIDDEN changed
#define NPARHID  10 //NPARAMS + NHIDDEN
#define NPARHID2 20 //NPARHID + NPARHID
#define NPARHID3 30 //NPARHID + NPARHID + NPARHID

typedef ap_fixed<16,6> data_t;
typedef ap_uint<8> i_data_t; // max nodes: 256

typedef hls::vector<data_t, NPARHID> par_t;
typedef hls::vector<par_t, 2> par2_t;
typedef hls::vector<par_t, 3> par3_t;
typedef hls::vector<i_data_t, NEDGES*2> ei_t;


extern "C"{

void runGraphNetwork(hls::stream<data_t>& X_stream, hls::stream<i_data_t>& ei_stream, hls::stream<data_t>& e_stream);
// void runGraphNetwork(data_t X_arr[NHITS * NPARAMS], i_data_t edge_i_arr[NEDGES * 2], data_t e_arr[NEDGES]);

}

#endif