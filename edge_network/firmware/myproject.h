#ifndef MYPROJECT_H_EDGE
#define MYPROJECT_H_EDGE


#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "defines.h"

#include "projectDefines.h"

extern "C"{

// Prototype of top level function for C-synthesis
// void edge_network(data_t H[NHITS * NPARHID], data_t Ro[NHITS * NEDGES], data_t Ri[NHITS * NEDGES], data_t e[NEDGES]);
void edge_network(hls::stream<H_t>& H_stream, hls::stream<R_t>& Ro_stream, hls::stream<R_t>& Ri_stream, hls::stream<e_t>& e_stream);

}

#endif
