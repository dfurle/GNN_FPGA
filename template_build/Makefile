
# name of .xcl file
HLS_NAME := GraphNetwork
# xilinx part to be used
PART := xilinx_u200_gen3x16_xdma_2_202110_1
# emulation mode (sw_emu, hw_emu, hw)
EMU_MODE := hw_emu

########

WORK := $(realpath ../)
LDFLAGS := -L${XILINX_XRT}/lib
LDFLAGS += -lxilinxopencl
# LDFLAGS += -pthread   #doesnt seem to be needed, but if anything breaks try uncomment this first!

main: ${HLS_NAME}.xclbin ./host.exe export.sh

.PHONY: host compile link clean all

all: host compile link

HOST_SRC := ${WORK}/host.cpp
HOST_INCLUDE := -I${XILINX_XRT}/include/
HOST_INCLUDE += -I${XILINX_HLS}/include/
HOST_INCLUDE += -I${WORK}/runner/
host.exe: $(HOST_SRC)
	g++ -g -std=c++17 -Wall -O0 $(HOST_SRC) ${HOST_INCLUDE} -o ./host.exe  ${LDFLAGS}
	
INPUT_DIR := ${WORK}/network/input
INPUT_SRC := ${INPUT_DIR}/myproject_i.cpp
INPUT_INCLUDE := -I${INPUT_DIR}
	
EDGE_DIR := ${WORK}/network/edge
EDGE_SRC := ${EDGE_DIR}/myproject_e.cpp
EDGE_INCLUDE := -I${EDGE_DIR}

NODE_DIR := ${WORK}/network/node
NODE_SRC := ${NODE_DIR}/myproject_n.cpp
NODE_INCLUDE := -I${NODE_DIR}

RUNNER_SRC := ${WORK}/runner/runner.cpp ${INPUT_SRC} ${NODE_SRC} ${EDGE_SRC}
RUNNER_INCLUDE := -I${WORK} -I${WORK}/network/firmware ${INPUT_INCLUDE} ${EDGE_INCLUDE} ${NODE_INCLUDE}
RUNNER_KERNEL := runner
KERNEL_CFG := ${WORK}/runner/hls_config.cfg
${RUNNER_KERNEL}.xo: ${RUNNER_SRC}
	mkdir -p ./kernels
	cd ./kernels; \
	v++ -c --mode hls --config ${KERNEL_CFG}; \
	mv ${RUNNER_KERNEL}/${RUNNER_KERNEL}.xo ..


${HLS_NAME}.xclbin: ${RUNNER_KERNEL}.xo emconfig.json
	mkdir -p ./xclbin
	cd ./xclbin; \
	v++ -l -t $(EMU_MODE) --platform $(PART) --config ${WORK}/runner/config.cfg ../${RUNNER_KERNEL}.xo -o $(HLS_NAME).xclbin; \
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
	rm -f  ${RUNNER_KERNEL}.xo
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

