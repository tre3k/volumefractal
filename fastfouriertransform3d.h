#ifndef FASTFOURIERTRANSFORM3D_H
#define FASTFOURIERTRANSFORM3D_H

#include <complex>
#include <thread>
#include <vector>
#include <iostream>

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
        unsigned int j = (int)(num/_size_z);
        unsigned int i = num - k*_size_z;
        return getValue(i,j,k);
    }
    void setDepth(unsigned int k,unsigned int num,std::complex<DATA_TYPE> val){
        unsigned int i = (int)(num/_size_y);
        unsigned int j = num - i*_size_y;
        return setValue(i,j,k,val);
    }

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
    std::vector<std::thread *> threads;
    unsigned int _n_threads {1};

    void ThreadRows(unsigned int id);
    void ThreadColumns(unsigned int id);
    void ThreadDepth(unsigned int id);
    void KillAllThreads(void){
        for(auto thread : threads){
            thread->detach();
            delete thread;
        }
        threads.clear();
    }

public:

    FastFourierTransform3D(Data *data = nullptr);
    ~FastFourierTransform3D(){
        return;
    }

    void setData(Data *data = nullptr){
        _data = data;
    }

    void setNumberOfThreads(unsigned int n_threads){_n_threads = n_threads;}

    void calculate(void);
};


}



#endif // FASTFOURIERTRANSFORM3D_H
