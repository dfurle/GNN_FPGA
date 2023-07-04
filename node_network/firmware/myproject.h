#ifndef MYPROJECT_H_NODE
#define MYPROJECT_H_NODE

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "projectDefines.h"

#include "defines.h"

extern "C"{

// Prototype of top level function for C-synthesis
#ifdef STREAM
void node_network(hls::stream<H_t>& Hin_stream, hls::stream<R_t>& Ro_stream, hls::stream<R_t>& Ri_stream, hls::stream<e_t>& e_stream, hls::stream<H_t>& Hout_stream);
#endif
#ifdef VECTOR
void node_network(H_t Hin_stream, R_t Ro_stream, R_t Ri_stream, e_t e_stream, H_t Hout_stream);
#endif
#ifdef ARRAY
void node_network(data_t H[NHITS * NPARHID], data_t Ro[NHITS * NEDGES], data_t Ri[NHITS * NEDGES], data_t e[NEDGES], data_t Hout[NHITS * NPARHID]);
#endif
}

#endif
