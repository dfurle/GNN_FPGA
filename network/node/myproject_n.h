#ifndef MYPROJECT_H_NODE
#define MYPROJECT_H_NODE

#include "projectDefines.h"
#include "defines_n.h"

extern "C"{

// Prototype of top level function for C-synthesis
void node_network(par_t H[NHITS], ei_t edge_index, data_t e[NEDGES], par_t Hout[NHITS]);
}

#endif
