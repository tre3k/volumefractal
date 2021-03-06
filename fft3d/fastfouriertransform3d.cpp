/*
 *  Copyright (c) 2020-2021 NRC KI PNPI, Gatchina, LO, 188300 Russia
 *
 *  This file is part of volumefractal (fft3d).
 *
 *  volumefractal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  volumefractal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with volumefractal.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#include "fastfouriertransform3d.h"

using namespace FFT3D;

FastFourierTransform3D::FastFourierTransform3D(Data *data) {
	setData(data);

	_p = new unsigned int [_size];
	for(int i=0;i<_size;i++) _p[i] = 0;
	_w = new std::complex<DATA_TYPE> [_size/2];
}

/* Threads for rows columns and depth */
void FastFourierTransform3D::ThreadRows(unsigned int id) {
	unsigned int n_rows = _data->size_y()*_data->size_z();
	/* FFT for all row */
	for(int row=id;row<n_rows;row+=_n_threads)
		fft(Type::FFT_ROW,row);
}

void FastFourierTransform3D::ThreadColumns(unsigned int id) {
	unsigned int n_columns = _data->size_x()*_data->size_z();
	/* FFT for all column */
	for(int column=id;column<n_columns;column+=_n_threads)
		fft(Type::FFT_COLUMN,column);
}

void FastFourierTransform3D::ThreadDepth(unsigned int id) {
	unsigned int n_depth =_data->size_x()*_data->size_y();
	/* FFT for all depth */
	for(int depth=id;depth<n_depth;depth+=_n_threads)
		fft(Type::FFT_DEPTH,depth);
}

void FastFourierTransform3D::calculate() {
	KillAllThreads();
	std::thread *th;

	/* Calculate FFT for all rows */
	auto t_start = std::chrono::high_resolution_clock::now();
	for(int i=0;i<_n_threads;i++){
		th = new std::thread(&FastFourierTransform3D::ThreadRows,
				     this, i);
		threads.push_back(th);
	}
	// wait for complete
	for(auto thread : threads) thread->join();
	auto t_stop = std::chrono::high_resolution_clock::now();
	threads.clear();
	std::cout << "FFT for rows is complete. "
		  << (t_stop-t_start).count()/1000
		  << " us." << std::endl;

	/* Calculate FFT for all columns */
	t_start = std::chrono::high_resolution_clock::now();
	for(int i=0;i<_n_threads;i++){
		th = new std::thread(&FastFourierTransform3D::ThreadColumns,
				     this, i);
		threads.push_back(th);
	}

	// wait for all threads complete
	for(auto thread : threads) thread->join();
	t_stop = std::chrono::high_resolution_clock::now();
	threads.clear();
	std::cout << "FFT for columns is complete. "
		  << (t_stop-t_start).count()/1000
		  << " us." << std::endl;

	/* Calculate FFT for all depth */
	t_start = std::chrono::high_resolution_clock::now();
	for(int i=0;i<_n_threads;i++){
		th = new std::thread(&FastFourierTransform3D::ThreadDepth,
				     this, i);
		threads.push_back(th);
	}
	// wait for complete
	for(auto thread : threads) thread->join();
	t_stop = std::chrono::high_resolution_clock::now();
	threads.clear();
	std::cout << "FFT for depth is complete. "
		  << (t_stop-t_start).count()/1000
		  << " us." << std::endl;
}

/* Just 1D FFT */
void FastFourierTransform3D::fft(unsigned int type, unsigned int index) {
	int n2 = _size/2;
	int step = 1;
	int shift = 0, p = 0, base = 0;
	int factor = 0;

	int i1,i2;

	std::complex<DATA_TYPE> temp {0, 0};
	std::complex<DATA_TYPE> v_temp[_size];

	/* copy data */
	for(int i=0;i<_size;i++){
		switch(type){
		case Type::FFT_ROW:
			v_temp[i] = _data->getRow(i,index);
			break;

		case Type::FFT_COLUMN:
			v_temp[i] = _data->getColumn(i,index);
			break;

		case Type::FFT_DEPTH:
			v_temp[i] = _data->getDepth(i,index);
			break;
		}
	}

	while(step < _bits+1){
		for(int l=0; l<(1<<(step-1)); l++){
			shift = _bits-step+1;
			base = (1<<shift);
			// base - ?????????????????? (W^p)_base
			factor = _size/base;

			for(int i=0;i<n2;i++){
				i1 = i+l*base;
				i2 = n2+i+l*base;
				// factor - ??????????????????
				p = i*factor;

				temp = v_temp[i1];
				v_temp[i1] = v_temp[i1] + v_temp[i2];
				v_temp[i2] = _w[p]*(temp - v_temp[i2]);
			}
		}
		n2 /= 2;
		step++;
	}

	/* copy with premutation */
	for(int i=0;i<_size;i++){
		switch(type){
		case Type::FFT_ROW:
			_data->setRow(i,index,_coeff*v_temp[_p[i]]);
			break;

		case Type::FFT_COLUMN:
			_data->setColumn(i,index,_coeff*v_temp[_p[i]]);
			break;

		case Type::FFT_DEPTH:
			_data->setDepth(i,index,_coeff*v_temp[_p[i]]);
			break;
		}
	}

}

void FastFourierTransform3D::GenerateFFTConsts(bool inverse) {
	if(!inverse){
		_coeff = {(DATA_TYPE)(1.0/_size),0};
		for(int i=0;i<_size/2;i++)
			_w[i] = std::complex<DATA_TYPE>
				{cos(-2.0 * M_PI * i / _size),
				 sin(-2.0 * M_PI * i / _size)};
	}else{
		_coeff = {(DATA_TYPE)1,0};
		for(int i=0;i<_size/2;i++)
			_w[i] = std::complex<DATA_TYPE>
				{cos(2.0 * M_PI * i / _size),
				 sin(2.0 * M_PI * i / _size)};
	}
}

void FastFourierTransform3D::GeneratePermutation(int type) {
	unsigned int temp {0};
	int size, bits;

	size = _size;
	bits = _bits;
	for(int i=0;i<_size;i++) _p[i] = 0;

	switch (type) {
        case Permutations::P_CLASSIC:
		// this for loop just reverse bits: like 0b11001 -> 0b10011
		for(int i=0; i<size; i++){
			temp = i;
			for(int j=0; j<bits; j++)
				_p[i] |= ((temp&(1<<j))>>j)<<(bits-j-1); // ;)
		}
		break;

        case Permutations::P_CENTER_ZERO:
		GeneratePermutation(Permutations::P_CLASSIC);
		for(int i=0;i<size/2;i++){
			temp = _p[i];
			_p[i] = _p[i+size/2];
			_p[i+size/2] = temp;
		}
		break;
	}
}
