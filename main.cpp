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


void benchmark(unsigned int size, unsigned int threads){
    FFT3D::Data data(size);
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time,midle_time,end_time;

    start_time = std::chrono::high_resolution_clock::now();
    for(int k=0;k<data.size_z();k++){
        for(int j=0;j<data.size_y();j++){
            for(int i=0;i<data.size_x();i++){
                data.setValue(i,j,k,std::complex<DATA_TYPE>
                              (cos(2*M_PI*i*0.25+0.1),0)
                              //(0,0)
                              );
            }
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Initial array: " << (double)(end_time-start_time).count()/1000000.0 << " ms" << std::endl << std::endl;

    std::cout << "\033[1mThreads: " << threads << "\033[0m\n";
    start_time = std::chrono::high_resolution_clock::now();

    FFT3D::FastFourierTransform3D *fft = new FFT3D::FastFourierTransform3D(&data);
    fft->setNumberOfThreads(threads);
    fft->GenerateFFTConsts();
    fft->GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);

    midle_time = std::chrono::high_resolution_clock::now();
    fft->calculate();
    end_time = std::chrono::high_resolution_clock::now();

    std::cout << std::endl << "Initial Time: " << (double)(midle_time-start_time).count()/1000000.0 << " ms" << std::endl;
    std::cout << "Calculation Time: " << (double)(end_time-start_time).count()/1000000.0 << " ms" << std::endl << std::endl;

    //output_cube(&data,true);
    delete fft;
}


int main(int argc,char *argv[]){
    unsigned int opt_size{8};
    unsigned int opt_threads{6};

    unsigned long int ram_size = sizeof(DATA_TYPE)*2*(opt_size*opt_size*opt_size+opt_size*opt_threads);

    std::cout << "On current system DATA_TYPE size: " << sizeof(DATA_TYPE) << " bytes or " << sizeof(DATA_TYPE)*8 << " bits" << std::endl;
    std::cout << "you need RAM size: " << ram_size << " bytes" << std::endl << std::endl;

    benchmark(opt_size,opt_threads);

    return 0;
}
