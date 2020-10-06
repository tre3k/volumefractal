/*
 * Copyright (c) 2020 Kirill Pshenichnyi pshcyrill@mail.ru
 * 3D Fast Fourier Transform, License: GPLv3
 */

#ifndef FFT3DDATA_H
#define FFT3DDATA_H

#include <complex>

namespace FFT3D {
#define DATA_TYPE double

    class Data{
    private:
        std::complex<DATA_TYPE> *_data {nullptr};

    protected:
        unsigned long int _size_x {0}, _size_y {0}, _size_z {0};

    public:
        explicit Data(unsigned long int size);
        ~Data(){
            delete [] _data;
        }

        unsigned int getIndex(unsigned long int i,unsigned long int j,unsigned long int k){
            unsigned long int index {0};
            index = i + j*_size_y + k*_size_y*_size_z;
            /*
            if(index >= _size_x*_size_y*_size_z){
                // and set error
                index = 0;
            }
            */
            return index;
        }

        std::complex<DATA_TYPE> getValue(unsigned long int i,unsigned long int j,unsigned long int k){ return _data[getIndex(i,j,k)];}
        void setValue(unsigned int long i,unsigned long int j,unsigned long int k,std::complex<DATA_TYPE> val){ _data[getIndex(i,j,k)] = val;}

        std::complex<DATA_TYPE> getRow(unsigned int i,unsigned int num){
            unsigned int k = (int)(num/_size_y);
            unsigned int j = num - k*_size_y;
            return getValue(i,j,k);
        }
        void setRow(unsigned int i,unsigned int num,std::complex<DATA_TYPE> val){
            unsigned int k = (int)(num/_size_y);
            unsigned int j = num - k*_size_y;
            return setValue(i,j,k,val);
        }

        std::complex<DATA_TYPE> getColumn(unsigned int j,unsigned int num){
            unsigned int k = (int)(num/_size_x);
            unsigned int i = num - k*_size_x;
            return getValue(i,j,k);
        }
        void setColumn(unsigned int j,unsigned int num,std::complex<DATA_TYPE> val){
            unsigned int k = (int)(num/_size_x);
            unsigned int i = num - k*_size_x;
            return setValue(i,j,k,val);
        }

        std::complex<DATA_TYPE> getDepth(unsigned int k,unsigned int num){
            unsigned int j = (int)(num/_size_x);
            unsigned int i = num - j*_size_x;
            return getValue(i,j,k);
        }
        void setDepth(unsigned int k,unsigned int num,std::complex<DATA_TYPE> val){
            unsigned int j = (int)(num/_size_x);
            unsigned int i = num - j*_size_x;
            return setValue(i,j,k,val);
        }

        std::complex<DATA_TYPE>& operator[](unsigned int val){
            return _data[val];
        }

        unsigned int size_x(){return _size_x;}
        unsigned int size_y(){return _size_y;}
        unsigned int size_z(){return _size_z;}
    };
}
#endif
