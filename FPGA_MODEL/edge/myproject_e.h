#pragma once
#include "projectDefines.h"
#include "defines_e.h"

extern "C"{
  void edge_network(par_t H[NHITS], i_data_t edge_index[NEDGES*2], par_t inbound[NHITS], par_t outbound[NHITS]);
  void edge_predictor(int valid_edges, par_t H[NHITS], i_data_t edge_index[NEDGES*2], data_t e[NEDGES]);
}

