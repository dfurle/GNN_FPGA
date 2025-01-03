#pragma once
#include "projectDefines.h"
#include "defines_n.h"

extern "C"{
  void node_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES], par_t Hout[NHITS], par_t inbound[NEDGES], par_t outbound[NEDGES]);
}

