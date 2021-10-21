#!/usr/bin/env bash

NP=8   # threads for compile

ORIGIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

FFT3D_PATH=fft3d
GENERATE_PATH=generate
QAVER_PATH=qfft3daverage

FFT3D_TMP=${FFT3D_PATH}/build
GENERATE_TMP=${GENERATE_PATH}/build
QAVER_TMP=${QAVER_PATH}/build

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

function Build() {
    # fft3d build
    cd ${ORIGIN_DIR}
    mkdir -p ${FFT3D_TMP}
    cd ${FFT3D_TMP}
    CC=${CC} CXX=${CXX} cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../
    mv compile_commands.json ../
    make -j${NP} CC=${CC} CXX=${CXX}

    # generate build
    cd ${ORIGIN_DIR}
    mkdir -p ${GENERATE_TMP}
    cd ${GENERATE_TMP}
    CC=${CC} CXX=${CXX} cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../
    mv compile_commands.json ../
    make -j${NP} CC=${CC} CXX=${CXX}

    # qfft3daverage build
    cd ${ORIGIN_DIR}
    mkdir -p ${QAVER_TMP}
    cd ${QAVER_TMP}
    qmake ../
    bear -- make -j${NP} CC=${CC} CXX=${CXX}
    mv compile_commands.json ../
}

function Clean(){
    cd ${ORIGIN_DIR}/${FFT3D_TMP} && make clean
    cd ${ORIGIN_DIR}/${GENERATE_TMP} && make clean
    cd ${ORIGIN_DIR}/${QAVER_TMP} && make clean
    cd ${ORIGIN_DIR}
    rm -vrf ${BIN_PATH}
    rm -vrf ${LIB_PATH}
    rm -vrf ${FFT3D_TMP}
    rm -vrf ${GENERATE_TMP}
    rm -vrf ${QAVER_TMP}
}


if [[ "$1" == "clean" ]]
then
    Clean
    exit 0
fi

if [[ "$1" == "rebuild" ]]
then
    Clean
    Build
    exit 0
fi

Build
