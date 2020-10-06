/*
 * Copyright (c) 2020 Kirill Pshenichnyi pshcyrill@mail.ru
 * 3D Fast Fourier Transform, License: GPLv3
 */

#include "fft3ddata.h"

using namespace FFT3D;

Data::Data(unsigned long int size) {
    _size_x = size;
    _size_y = _size_x;
    _size_z = _size_x;
    _data = new std::complex<DATA_TYPE> [(unsigned long int) _size_x*_size_y*_size_z];
}