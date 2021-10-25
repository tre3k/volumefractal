#!/usr/bin/env bash

NP=8   # threads for compile

ORIGIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BUILD_CMAKE=${ORIGIN_DIR}/build

QAVER_PATH=qfft3daverage
QAVER_TMP=${QAVER_PATH}/build

BIN_PATH=bin
LIB_PATH=lib


function Build() {
    mkdir -p ${BUILD_CMAKE}
    cd ${BUILD_CMAKE}
    CC=${CC} CXX=${CCX} cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../
    mv compile_commands.json ../
    make -j${NP} CC=${CC} CXX=${CXX}
}

function Clean(){
    cd ${BUILD_CMAKE}
    make clean
    rm -rf ${ORIGIN_DIR}/bin
    rm -rf ${ORIGIN_DIR}/lib
    rm -rf ${ORIGIN_DIR}/build
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
