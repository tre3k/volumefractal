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
		DAVINCI,
		SDAVINCI,
		VISHEK,
		VMULD
	};

	class DaVinci3D {
	protected:
		FFT3D::Data *_data;

		/* minimum of element size,
		   need calculate for space size */
		int minimum_size_element {1};

		int _iteration {0};
		int _speed {2};
		FFT3D::acoord position {0, 0, 0};


		virtual void element(FFT3D::acoord pos,
			     int size,             // size of minimum element
			     int age);
		/* if you want know full size,
		   you need: size * size_element(age) */


	public:
		DaVinci3D(FFT3D::Data *data, int iteration = 1);
		void setIteration(int iteration);
		int getIteration(void);
		void generate(void);

		/* This function determines the maximum number
		   of iterations over the size of the array _data */
		void setMaximumIteration(void);
		/* This function returns the size of the array for
		   a given number of iterations. */
		int getSizeFromIteration(int);

		void setMinimumSizeElement(int);
		void setPosition(FFT3D::acoord pos);
		void setSpeed(int speed);

		int SizeElement(int age);
		int SizeCentral(int age);
	};

	class SDaVinci3D : public DaVinci3D {
	public:
		SDaVinci3D(FFT3D::Data *data, int iteration = 1);

	protected:
		void element(FFT3D::acoord pos,
			     int size,
			     int age) override;
	};

	class Vishek3D {
	public:
		Vishek3D(FFT3D::Data *data, int iteration = 1);
		void generate(void);
		void setPosition(FFT3D::acoord pos);
		void setSize(int size);
		void setIteration(int iteration);
		static const int permitedSizes[];

	protected:
		FFT3D::Data *_data;

		FFT3D::acoord position_ {0, 0, 0};
		int size_ {1};
		int iteration_ {1};

		void generateWhiteCubes(FFT3D::acoord pos,
					int size);
		void element(FFT3D::acoord pos,
			     int size,
			     int age);

	};

	/* Vishek 3D multiplication with Davinci */
	class VmulD3D : public DaVinci3D {
	public:
		VmulD3D(FFT3D::Data *data, int iteration = 1);
		void setVishekIteration(int iteration = 2);
	protected:
		int vishekIteraption_ {3};
		void element(FFT3D::acoord pos,
			     int size,
			     int age) override;

	};

};


#endif
