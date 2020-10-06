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

void Data::WriteToRawFile(std::string filename) {
    std::FILE *f = std::fopen(filename.c_str(),"w");

    s_raw_file_header header;
    header.size = _size_x;
    header.data_type=1;
    std::fwrite(&header,1,sizeof(s_raw_file_header),f);
    std::fwrite(_data,sizeof(std::complex<DATA_TYPE>),header.size*header.size*header.size,f);

    std::fclose(f);
}