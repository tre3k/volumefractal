/*
 * Copyright (c) 2020 Kirill Pshenichnyi pshcyrill@mail.ru
 * 3D Fast Fourier Transform, License: GPLv3
 */

#ifndef FFT3DDATA_H
#define FFT3DDATA_H

#include <complex>
#include <string>
#include <iostream>

namespace FFT3D {
#define DATA_TYPE double

    class Data2D{
    private:
        unsigned long _size {0};
        std::complex<DATA_TYPE> *_data {nullptr};
    public:
        Data2D(unsigned long size){Init(size);}
        std::complex<DATA_TYPE> *data(){return _data;}
        void Init(unsigned long size){
            _size = size;
            if(_data!= nullptr) delete _data;
            _data = new std::complex<DATA_TYPE>[size*size];
        }
        unsigned long size(void){return _size;}
        std::complex<DATA_TYPE> getValue(unsigned long i,unsigned long j){
            unsigned long index = j+i*_size;
            return _data[index];
        }

    };

    class Data{
    private:
        std::complex<DATA_TYPE> *_data {nullptr};

    protected:
        unsigned long _size_x {0}, _size_y {0}, _size_z {0};

    public:
        Data(unsigned long size);
        ~Data(){
            delete [] _data;
        }

        struct s_raw_file_header{
            unsigned long size;
            size_t hash_data_type;
        };

        unsigned int getIndex(unsigned long i,unsigned long j,unsigned long k){
            unsigned long index {0};
            index = i + j*_size_y + k*_size_y*_size_z;
            return index;
        }

        std::complex<DATA_TYPE> getValue(unsigned long i,unsigned long j,unsigned long k){ return _data[getIndex(i,j,k)];}
        void setValue(unsigned long i,unsigned long j,unsigned long k,std::complex<DATA_TYPE> val){ _data[getIndex(i,j,k)] = val;}

        std::complex<DATA_TYPE> getRow(unsigned int i,unsigned int num){
            unsigned long k = (int)(num/_size_y);
            unsigned long j = num - k*_size_y;
            return getValue(i,j,k);
        }

        void setRow(unsigned int i,unsigned int num,std::complex<DATA_TYPE> val){
            unsigned long k = (int)(num/_size_y);
            unsigned long j = num - k*_size_y;
            return setValue(i,j,k,val);
        }

        std::complex<DATA_TYPE> getColumn(unsigned long j,unsigned long num){
            unsigned int k = (int)(num/_size_x);
            unsigned int i = num - k*_size_x;
            return getValue(i,j,k);
        }
        void setColumn(unsigned long j,unsigned long num,std::complex<DATA_TYPE> val){
            unsigned int k = (int)(num/_size_x);
            unsigned int i = num - k*_size_x;
            return setValue(i,j,k,val);
        }

        std::complex<DATA_TYPE> getDepth(unsigned long k,unsigned long num){
            unsigned int j = (int)(num/_size_x);
            unsigned int i = num - j*_size_x;
            return getValue(i,j,k);
        }
        void setDepth(unsigned long k,unsigned long num,std::complex<DATA_TYPE> val){
            unsigned int j = (int)(num/_size_x);
            unsigned int i = num - j*_size_x;
            return setValue(i,j,k,val);
        }

        std::complex<DATA_TYPE>& operator[](unsigned long val){
            return _data[val];
        }

        unsigned long size_x(){return _size_x;}
        unsigned long size_y(){return _size_y;}
        unsigned long size_z(){return _size_z;}

        void WriteToRawFile(std::string filename);
        void ReadFromRawFile(std::string filename);
        void ReadOnlyHeader(std::string filename);
        static void ReadDepthFromFile(std::string filename, Data2D *data, unsigned long num_depth);
        unsigned long long FileSize(void){
            return sizeof(s_raw_file_header)+_size_x*_size_y*_size_z*sizeof(std::complex<DATA_TYPE>);
        }

    };



}
#endif
