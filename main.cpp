/*
 * Copyright (c) 2020 Kirill Pshenichnyi pshcyrill@mail.ru
 * 3D Fast Fourier Transform, License: GPLv3
 */

#include <iostream>
#include <complex>
#include <chrono>

#include "fastfouriertransform3d.h"

void output_cube(FFT3D::Data *data, bool magnitude = false){
    for(int k=0;k<data->size_z();k++){
        std::cout << std::endl << "Depth: " << k << std::endl;
        for(int j=0;j<data->size_y();j++){
            for(int i=0;i<data->size_x();i++){
                if(magnitude)
                    std::cout << abs(data->getValue(i,j,k));
                else
                    std::cout << data->getValue(i,j,k);
                std::cout << "\r";
                for(int l=0;l<i+1;l++) std::cout << "\t\t\t";
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc,char *argv[]){
    FFT3D::Data data(1024,1024,1024);

    /* set array */
    for(int k=0;k<data.size_z();k++){
        for(int j=0;j<data.size_y();j++){
            for(int i=0;i<data.size_x();i++){
                data.setValue(i,j,k,std::complex<DATA_TYPE>
                              (cos(2*M_PI*k*0.25+0.1),0)
                              //(0,0)
                              );
            }
        }
    }

    FFT3D::FastFourierTransform3D fft3d(&data);
    fft3d.setNumberOfThreads(8);

    fft3d.GenerateFFTConsts();
    fft3d.GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);
    //fft3d.GeneratePermutation(FFT3D::Permutations::P_CLASSIC);

    auto t_start = std::chrono::high_resolution_clock::now();
    fft3d.calculate();
    auto t_stop = std::chrono::high_resolution_clock::now();
    std::cout << "total time: " << (t_stop-t_start).count()/1000 << " us\n";

    //output_cube(&data,true);
    return 0;
}
