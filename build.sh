#!/usr/bin/env bash

NP=8   # threads for compile

ORIGIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

FFT3D_PATH=fft3d
AVER_PATH=fft3daverage

FFT3D_TMP=${FFT3D_PATH}/build
AVER_TMP=${AVER_PATH}/build

BIN_PATH=bin
LIB_PATH=lib

CC=gcc
CXX=g++

if [[ "$1" == "clang" ]]
then
    CC=clang
    CXX=clang++
fi

export CC=${CC}
export CXX=${CXX}

if [[ "$1" == "clean" ]]
then
    cd ${ORIGIN_DIR}/${FFT3D_TMP} && make clean
    cd ${ORIGIN_DIR}/${AVER_TMP} && make clean
    cd ${ORIGIN_DIR}
    rm -vrf ${BIN_PATH}
    rm -vrf ${LIB_PATH}
    rm -vrf ${FFT3D_TMP}
    rm -vrf ${AVER_TMP}
    exit 0
fi

# fft3d build
mkdir -p ${FFT3D_TMP}
cd ${FFT3D_TMP}
CC=${CC} CXX=${CXX} cmake ../
make -j${NP} CC=${CC} CXX=${CXX}


# fft3daverage build
cd ${ORIGIN_DIR}
mkdir -p ${AVER_TMP}
cd ${AVER_TMP}
qmake ../
make -j${NP} CC=${CC} CXX=${CXX}
