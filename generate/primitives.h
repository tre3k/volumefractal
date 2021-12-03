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


#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <fft3ddata.h>
#include <average.h>

namespace Primitives {
	/* Abstract class for shared properties */
	class Primitive {
	public:
		Primitive(FFT3D::Data *data = nullptr,
			  FFT3D::acoord center = {0, 0, 0}
			);

		virtual void paint(void) {};

	protected:
		FFT3D::Data *_data;
		// this is coordinate of the figure
		FFT3D::acoord _center;
		// this is coordinate of key pixel
		FFT3D::acoord _key;

	};

	/* Cube class for paint cube =) */
	class Cube : public Primitive {
	protected:
		int _a;
	public:
		Cube(FFT3D::Data *data = nullptr,
		     int a = 1,
		     FFT3D::acoord center = {0, 0, 0}
			);

		void setSize(int a);

		void setKeyPosition(FFT3D::acoord key);
		/* If I know center, I can calculate Key position */
		void calculateKey(void);

		void setCenterPosition(FFT3D::acoord center);
		/* If I know key pos., I can calculate center position */
		void calculateCenter(void);

		void paint(void);

	};

	class CubeWhite : public Cube {
	public:
		CubeWhite(FFT3D::Data *data = nullptr);
		void paint(void);
	};


	class Sphera : public Primitive {
	protected:
		int _r {5};           // radius
	public:
		Sphera(FFT3D::Data *data,
		       FFT3D::acoord center = {6, 6, 6},
		       int radius = 5);

		void setRadius(int r);
		void setPosition(FFT3D::acoord pos);
		void paint(void);
	};

	class Pinholl : public Primitive {
	public:
		Pinholl(FFT3D::Data *data = nullptr,
			FFT3D::acoord center = {0, 0, 0});
		void paint(void);
	};
};

#endif
