
# name of .xcl file
HLS_NAME := GraphNetwork
# xilinx part to be used
PART := xilinx_u200_gen3x16_xdma_2_202110_1
# emulation mode (sw_emu, hw_emu, hw)
EMU_MODE := hw_emu

########

WORK := $(realpath ../)
LDFLAGS := 
# LDFLAGS += -lxrt_coreutil  # used in xrt instead of opencl implementation, not needed
LDFLAGS += -lxilinxopencl
# LDFLAGS += -pthread   #doesnt seem to be needed, but if anything breaks try uncomment this first!

main: ${HLS_NAME}.xclbin ./host.exe export.sh

.PHONY: host compile link clean all

all: host compile link

HOST_SRC := ${WORK}/host.cpp
host.exe: $(HOST_SRC)
	g++ -g -std=c++17 -Wall -O0 $(HOST_SRC) -o ./host.exe -I${XILINX_XRT}/include/ -I${XILINX_HLS}/include/ -L${XILINX_XRT}/lib ${LDFLAGS}
	
INPUT_DIR := ${WORK}/input_network
INPUT_SRC := ${INPUT_DIR}/firmware/myproject.cpp
INPUT_INCLUDE := -I${INPUT_DIR} -I${WORK} -I${INPUT_DIR}/firmware
INPUT_KERNEL := input_network
$(INPUT_KERNEL).xo: ${INPUT_SRC}
	mkdir -p ./kernels
	cd ./kernels; \
	v++ -c -t $(EMU_MODE) --platform $(PART) --config ${INPUT_DIR}/config.cfg -k $(INPUT_KERNEL) $(INPUT_INCLUDE) ${INPUT_SRC} -o $(INPUT_KERNEL).xo; \
	mv ${INPUT_KERNEL}.xo ..
	
EDGE_DIR := ${WORK}/edge_network
EDGE_SRC := ${EDGE_DIR}/firmware/myproject.cpp
EDGE_INCLUDE := -I${EDGE_DIR} -I${WORK} -I${EDGE_DIR}/firmware
EDGE_KERNEL := edge_network
${EDGE_KERNEL}.xo: ${EDGE_SRC}
	mkdir -p ./kernels
	cd ./kernels; \
	v++ -c -t $(EMU_MODE) --platform $(PART) --config ${EDGE_DIR}/config.cfg -k ${EDGE_KERNEL} $(EDGE_INCLUDE) ${EDGE_SRC} -o ${EDGE_KERNEL}.xo; \
	mv ${EDGE_KERNEL}.xo ..

NODE_DIR := ${WORK}/node_network
NODE_SRC := ${NODE_DIR}/firmware/myproject.cpp
NODE_INCLUDE := -I${NODE_DIR} -I${WORK} -I${NODE_DIR}/firmware
NODE_KERNEL := node_network
${NODE_KERNEL}.xo: ${NODE_SRC}
	mkdir -p ./kernels
	cd ./kernels; \
	v++ -c -t $(EMU_MODE) --platform $(PART) --config ${NODE_DIR}/config.cfg -k ${NODE_KERNEL} $(NODE_INCLUDE) ${NODE_SRC} -o ${NODE_KERNEL}.xo; \
	mv ${NODE_KERNEL}.xo ..

${HLS_NAME}.xclbin: $(INPUT_KERNEL).xo ${EDGE_KERNEL}.xo ${NODE_KERNEL}.xo emconfig.json
	mkdir -p ./xclbin
	cd ./xclbin; \
	v++ -l -t $(EMU_MODE) --platform $(PART) --config ${WORK}/config.cfg ../$(INPUT_KERNEL).xo ../${NODE_KERNEL}.xo ../${EDGE_KERNEL}.xo -o $(HLS_NAME).xclbin; \
	mv ${HLS_NAME}.xclbin ..

export.sh: ${HLS_NAME}.xclbin
	@echo "export XCL_EMULATION_MODE=$(EMU_MODE)\n" > export.sh
	echo "do 'source export.sh' if EMU_MODE was changed"

emconfig.json:
	emconfigutil --platform $(PART)

host:
	rm -f host.exe
	${MAKE}

link:
	rm -f ${HLS_NAME}.xclbin
	${MAKE}

compile:
	rm -f  $(INPUT_KERNEL).xo ${EDGE_KERNEL}.xo ${NODE_KERNEL}.xo
	${MAKE}

clean:
	rm -rf ./.Xil/
	rm -rf ./.run/
	rm -rf ./kernels
	rm -rf ./xclbin
	rm -f *.xo
	rm -f *.xclbin
	rm -f *.exe
	rm -f emconfig.json
	rm -f export.sh
	rm -f host.exe

