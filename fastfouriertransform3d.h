#ifndef FASTFOURIERTRANSFORM3D_H
#define FASTFOURIERTRANSFORM3D_H

#include <complex>
#include <thread>

namespace FFT3D {
#define DATA_TYPE long double

class Data{
private:
    std::complex<DATA_TYPE> *_data {nullptr};

protected:
    unsigned int _size_x {0}, _size_y {0}, _size_z {0};


public:
    explicit Data(unsigned int size_x, unsigned int size_y, unsigned int size_z){
        _data = new std::complex<DATA_TYPE> [size_x*size_y*size_z];
        _size_x = size_x; _size_y = size_y; _size_z = size_z;
    }
    ~Data(){
        delete [] _data;
    }

    unsigned int getIndex(unsigned int i,unsigned int j,unsigned int k){
        unsigned int index {0};
        index = i + j*_size_y + k*_size_y*_size_z;
        /*
        if(index >= _size_x*_size_y*_size_z){
            // and set error
            index = 0;
        }
        */
        return index;
    }

    std::complex<DATA_TYPE> getValue(unsigned int i,unsigned int j,unsigned int k){ return _data[getIndex(i,j,k)];}
    void setValue(unsigned int i,unsigned int j,unsigned int k,std::complex<DATA_TYPE> val){ _data[getIndex(i,j,k)] = val;}

    std::complex<DATA_TYPE>& operator[](unsigned int val){
        return _data[val];
    }

    unsigned int size_x(){return _size_x;}
    unsigned int size_y(){return _size_y;}
    unsigned int size_z(){return _size_z;}

};

class FastFourierTransform3D
{
private:
    Data *_data {nullptr};
public:

    FastFourierTransform3D(Data *data = nullptr);
    ~FastFourierTransform3D(){

    }

    void setData(Data *data = nullptr){
        _data = data;
    }

};
}



#endif // FASTFOURIERTRANSFORM3D_H
