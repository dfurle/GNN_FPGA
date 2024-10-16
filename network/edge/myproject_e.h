#ifndef MYPROJECT_H_EDGE
#define MYPROJECT_H_EDGE

#include "defines_e.h"
#include "projectDefines.h"

extern "C"{

// Prototype of top level function for C-synthesis
void edge_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], int valid_edges, par_t inbound[NEDGES], par_t outbound[NEDGES]);

}

#endif
