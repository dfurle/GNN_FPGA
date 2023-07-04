#ifndef MYPROJECT_H_INPUT
#define MYPROJECT_H_INPUT

#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"

#include "projectDefines.h"

#include "defines.h"

extern "C"{

// Prototype of top level function for C-synthesis
void input_network(hls::stream<X_t>& X_stream, hls::stream<H_t>& H_stream);
// void input_network(data_t X[NHITS * NPARAMS], data_t H[NHITS * NPARHID]);

}

#endif
