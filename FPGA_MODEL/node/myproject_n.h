#pragma once
#include "projectDefines.h"
#include "defines_n.h"

extern "C"{
  void node_network(par_t H[NHITS], par_t Hout[NHITS], par_t inbound[NHITS], par_t outbound[NHITS]);
}

