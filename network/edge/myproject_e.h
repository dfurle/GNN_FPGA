#ifndef MYPROJECT_H_EDGE
#define MYPROJECT_H_EDGE

#include "defines_e.h"
#include "projectDefines.h"

extern "C"{

// Prototype of top level function for C-synthesis
void edge_network(par_t H[NHITS], ei_t edge_index, data_t e[NEDGES], int valid_edges);

}

#endif
