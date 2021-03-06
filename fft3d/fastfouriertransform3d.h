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


#ifndef FASTFOURIERTRANSFORM3D_H
#define FASTFOURIERTRANSFORM3D_H

#include <complex>
#include <thread>
#include <vector>
#include <iostream>

#include "fft3ddata.h"

namespace FFT3D {

// P_CLASSIC:       0 ..... fd/2 ....... fd
// P_CENTER_ZERO:  -fd/2 ..... 0 ..... fd/2
//                 [fd -  freq. of discret.]

	enum Permutations{
		P_CLASSIC,
		P_CENTER_ZERO
	};

	enum Direction{
		T_DIRECT_FFT,
		T_INVERSE_FFT
	};

	enum Type{
		FFT_ROW,
		FFT_COLUMN,
		FFT_DEPTH
	};

	class FastFourierTransform3D
	{
	private:
		Data* _data {nullptr};
		std::vector<std::thread *> threads;
		unsigned int _n_threads {1};
		std::complex<DATA_TYPE> *_w {nullptr};    // Fourier constants
		unsigned int *_p {nullptr};
		std::complex<DATA_TYPE> _coeff {1,0};

		unsigned int _size {0}, _bits{0};

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
			_size = data->size_x();
			_bits = log2(_size);
		}

		void setNumberOfThreads(unsigned int n_threads){
			_n_threads = n_threads;
		}

		void calculate(void);
		void fft(unsigned int type = Type::FFT_ROW,
			 unsigned int index = 0);


		void GenerateFFTConsts(bool inverse = false);
		void GeneratePermutation(int type);

	};

}

#endif // FASTFOURIERTRANSFORM3D_H
