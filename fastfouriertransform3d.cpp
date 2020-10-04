#include "fastfouriertransform3d.h"

using namespace FFT3D;

FastFourierTransform3D::FastFourierTransform3D(Data *data)
{
    setData(data);
}

void FastFourierTransform3D::ThreadRows(unsigned int id){
    unsigned int n_rows = _data->size_y()*_data->size_z();

    /* FFT for all row */
    for(int row=id;row<n_rows;row+=_n_threads){
        _data->setRow(2,row,std::complex<DATA_TYPE>(2,2));
    }
}

void FastFourierTransform3D::ThreadColumns(unsigned int id){
    unsigned int n_columns = _data->size_x()*_data->size_z();

    /* FFT for all column */
    for(int column=id;column<n_columns;column+=_n_threads){
        _data->setColumn(1,column,std::complex<DATA_TYPE>(3,3));
    }

}

void FastFourierTransform3D::ThreadDepth(unsigned int id){
    unsigned int n_depth =_data->size_x()*_data->size_y();

    /* FFT for all depth */
    for(int depth=id;depth<n_depth;depth+=_n_threads){
        _data->setDepth(0,depth,std::complex<DATA_TYPE>(4,4));
    }
}

void FastFourierTransform3D::calculate(){
    KillAllThreads();
    std::thread *th;

    /* Calculate FFT for all rows */
    for(int i=0;i<_n_threads;i++){
        th = new std::thread(&FastFourierTransform3D::ThreadRows,this,i);
        threads.push_back(th);
    }

    // wait for complete
    for(auto thread : threads) thread->join();
    threads.clear();

    /* Calculate FFT for all columns */
    for(int i=0;i<_n_threads;i++){
        th = new std::thread(&FastFourierTransform3D::ThreadColumns,this,i);
        threads.push_back(th);
    }

    // wait for all threads complete
    for(auto thread : threads) thread->join();
    threads.clear();

    /* Calculate FFT for all depth */
    for(int i=0;i<_n_threads;i++){
        th = new std::thread(&FastFourierTransform3D::ThreadDepth,this,i);
        threads.push_back(th);
    }

    // wait for complete
    for(auto thread : threads) thread->join();
    threads.clear();

}
