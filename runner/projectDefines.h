#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_vector.h"
#include "hls_stream.h"


// #define NHITS  100
// #define NEDGES 300
#define NHITS  21
#define NEDGES 42
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
typedef ap_uint<2> R_data_t;



// typedef hls::vector<data_t, NPARAMS> node_t;
// typedef hls::vector<data_t, NPARHID> hidden_t;
// typedef hls::vector<data_t, NPARHID2> hidden2_t;
// typedef hls::vector<data_t, NPARHID3> hidden3_t;
// typedef hls::vector<R_data_t, NEDGES> Redges_t;
// typedef hls::vector<data_t, NEDGES> e_t;


typedef hls::vector<hls::vector<data_t,NPARAMS>, NHITS> X_t;
typedef hls::vector<hls::vector<data_t,NPARHID>, NHITS> H_t;
typedef hls::vector<hls::vector<data_t,NPARHID2>, NHITS> H2_t;
typedef hls::vector<hls::vector<data_t,NPARHID3>, NHITS> H3_t;
typedef hls::vector<hls::vector<data_t,NPARHID>, NHITS> inner_t;
typedef hls::vector<hls::vector<R_data_t,NEDGES>, NHITS> R_t;

typedef hls::vector<hls::vector<R_data_t,NHITS>, NEDGES> R_tr_t;

typedef hls::vector<hls::vector<data_t,NPARHID>, NEDGES> b_t;
typedef hls::vector<hls::vector<data_t,NPARHID2>, NEDGES> B_t;

typedef hls::vector<data_t, NEDGES> e_t;

#endif