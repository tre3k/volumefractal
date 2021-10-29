/*
 *  Copyright (c) 2020-2021 NRC KI PNPI, Gatchina, LO, 188300 Russia
 *
 *  This file is part of volumefractal (average).
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


#ifndef AVERAGE_H
#define AVERAGE_H

#include <vector>
#include <complex>

#include <fft3ddata.h>

class Average {
private:
	FFT3D::Data *_data {nullptr};

public:
	Average(FFT3D::Data *data = nullptr);
	void setData(FFT3D::Data *data);
	/* search mass of center for intencity abs*abs */
	FFT3D::acoord findCenterMass(void);

	std::complex<DATA_TYPE> getPointFrom2DAverage(std::string filename,
						      unsigned long num_depth,
						      double r);

	struct s_averages{
		std::vector<std::complex<DATA_TYPE>> value;
		std::vector<double> r;
	} typedef averages;

	averages average(FFT3D::acoord center);

	static int doubleToInt(double val);
};

#endif
