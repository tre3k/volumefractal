#include <iostream>
#include <complex>

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
    FFT3D::Data data(8,8,8);

    /* set array */
    for(int k=0;k<data.size_z();k++){
        for(int j=0;j<data.size_y();j++){
            for(int i=0;i<data.size_x();i++){
                data.setValue(i,j,k,std::complex<DATA_TYPE>
                              (cos(2*M_PI*i*0.123+0.1),0)
                              );
            }
        }
    }


    FFT3D::FastFourierTransform3D fft3d ;

    output_cube(&data,false);

    return 0;
}
