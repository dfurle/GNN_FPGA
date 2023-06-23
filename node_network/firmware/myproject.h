#ifndef MYPROJECT_H_
#define MYPROJECT_H_

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "projectDefines.h"

#include "defines.h"

extern "C"{

// Prototype of top level function for C-synthesis
void node_network(data_t H[NHITS * NPARHID], data_t Ro[NHITS * NEDGES], data_t Ri[NHITS * NEDGES], data_t e[NEDGES], data_t outH[NHITS * NPARHID]);

}

#endif
