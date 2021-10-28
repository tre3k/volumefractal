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


#include <complex>
#include "average.h"

Average::Average(FFT3D::Data *data) {
	setData(data);
}

void Average::setData(FFT3D::Data *data) {
	_data = data;
}

int Average::doubleToInt(double val) {
	return (int)(val+0.5);
}


FFT3D::acoord Average::findCenterMass(){
	FFT3D::acoord retVal;

	double Sm {0.0};
	double Sx {0.0};
	double Sy {0.0};
	double Sz {0.0};
	double mod {0.0};

	auto sx = _data->size_x();
	auto sy = _data->size_y();
	auto sz = _data->size_z();

	for(int i = 0; i < sx; i++) {
		for(int j = 0; j < sy; j++) {
			for(int k = 0; k < sz; k++) {
				mod = abs(_data->getValue(i,j,k));
				Sx += mod*mod*i;
				Sy += mod*mod*j;
				Sz += mod*mod*k;
				Sm += mod*mod;
			}
		}
	}

	retVal.di = Sx/Sm;
	retVal.dj = Sy/Sm;
	retVal.dk = Sz/Sm;
	retVal.double_flag = true;

	return retVal;
}



/* function for azim average (2pi) just only one r (recive one point) */
std::complex<DATA_TYPE> Average::getPointFrom2DAverage(std::string filename,
						       unsigned long num_depth,
						       double r) {
	std::complex<DATA_TYPE> retVal {0.0, 0.0};
	double dr = sqrt(2);
	double dphi = 0.1;


	FFT3D::Data2D data2d(_data->size_x());
	_data->Read2DLayerDepthFromFile(filename,
					&data2d,
					num_depth);

	dphi = atan(1.0/r);


	return retVal;
}

Average::averages Average::average(FFT3D::acoord center){
	averages retVal;

	std::complex<DATA_TYPE> S {0.0, 0.0};
	int count {0};

	double r, theta, phi;
	double x, y, z;
	int ix {0}, iy {0}, iz {0};

	// тут необходимо посчитать максимальный радиусвектор
	double mr = (double) _data->size_x()/2;

	FFT3D::Data::syncAccord(&center);

	double dr = sqrt(3);
	double dphi;
	double dtheta;


	r = 3;
	theta = -M_PI/2;
	phi = M_PI/2;

	// -pi <= phi < pi
	// 0 <= theta < pi

	/* Стартовая точка, при нулевом радиус-векторе - центр  */
	retVal.r.push_back(0);
	retVal.value.push_back(
		_data->getValue(center.i,
				center.j,
				center.k)
		);

	for(r = dr; r < mr; r += dr) {
		for(theta = 0; theta < M_PI; theta += dtheta) {
			for(phi = -M_PI; phi < M_PI; phi += dphi){

				dphi = atan(1.0/r);
				dtheta = atan(1.0/r);

				_data->fromSphere(&x, &y, &z,
						  r, theta, phi);

				ix = doubleToInt(x) + center.i;
				iy = doubleToInt(y) + center.j;
				iz = doubleToInt(z) + center.k;

				S += _data->getValue(ix, iy, iz);
				count ++;

				/* если theta = 0, то мы крутимся на одном
				   месте в полюсе поэтому достаточно взять
				   одну точку */
				if(theta == 0) break;
			}
		}

		S = S / (double) count;

		retVal.r.push_back(r);
		retVal.value.push_back(S);
		S = {0.0, 0.0};
		count = 0;
	}

	return retVal;
}
