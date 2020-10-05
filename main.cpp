#include <iostream>
#include <complex>
#include <thread>
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

int main()
{
    FFT3D::Data data(4,4,4);

    /* set array */
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



    FFT3D::FastFourierTransform3D fft3d(&data);
    fft3d.setNumberOfThreads(1);

    fft3d.GenerateFFTConsts();
    fft3d.GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);
    //fft3d.GeneratePermutation(FFT3D::Permutations::P_CLASSIC);

    fft3d.calculate();

    //std::this_thread::sleep_for((std::chrono::duration<int>)2);

    output_cube(&data,true);

    return 0;
}
