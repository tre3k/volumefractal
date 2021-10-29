/*
 *  Copyright (c) 2020-2021 NRC KI PNPI, Gatchina, LO, 188300 Russia
 *
 *  This file is part of volumefractal (generate).
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


#ifndef FRACTALS_H
#define FRACTALS_H

#include <fft3ddata.h>
#include <iostream>
#include "primitives.h"

namespace Fractals {
	enum FRACTALS {
		FNULL,
		PINHOLL,           // from Primitives namespace
		SPHERA,
		DAVINCI
	};

	class DaVince3D {
	protected:
		FFT3D::Data *_data;

		long int start_size_element {1};
		int current_size_element = start_size_element;

		int _iteration {0};


		void element(FFT3D::acoord pos,
			     int size,
			     int age);

	public:
		DaVince3D(FFT3D::Data *data, int iteration = 1);
		void setIteration(int iteration);
		int getIteration(void);
		void generate(void);

		/* This function determines the maximum number
		   of iterations over the size of the array _data */
		void setMaximumItteration(void);
		/* This function returns the size of the array for
		   a given number of iterations. */
		static int getSizeFromItteration(int);
	};
};


#endif
