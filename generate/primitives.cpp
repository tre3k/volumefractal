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


#include "primitives.h"
using namespace Primitives;


Primitive::Primitive(FFT3D::Data *data,
		     FFT3D::acoord center
	) {
	_data = data;
	_center = center;
}


Cube::Cube(FFT3D::Data *data,
	   int a,
	   FFT3D::acoord center
	) : Primitive(data, center) {

	setSize(a);
	setCenterPosition(center);
}

void Cube::setSize(int a){
	_a = a;
}

void Cube::setKeyPosition(FFT3D::acoord key) {
	_key = key;
	calculateCenter();
}

void Cube::setCenterPosition(FFT3D::acoord center) {
	_center = center;
	calculateKey();
}

void Cube::calculateKey() {
	_key.i = _center.i - _a/2;
	_key.j = _center.j - _a/2;
	_key.k = _center.k - _a/2;
}

void Cube::calculateCenter() {
	_center.i = _key.i + _a/2;
	_center.j = _key.j + _a/2;
	_center.k = _key.k + _a/2;
}

void Cube::paint() {
	for(int i=_key.i; i<_a+_key.i; i++) {
		for(int j=_key.j; j<_a+_key.j; j++) {
			for(int k=_key.k; k<_a+_key.k; k++) {
				_data->setValue(i, j, k, {1.0,0});
			}
		}
	}
}


Sphera::Sphera(FFT3D::Data *data,
		    FFT3D::acoord center,
		    int radius
	) : Primitive(data, center) {
	setRadius(radius);
}

void Sphera::setRadius(int r) {
	_r = r;
}

void Sphera::paint() {
	double dr = 1;
	double dphi;
	double dtheta;

	double r, theta, phi;
	double x {0.0}, y {0.0}, z {0.0};
	int ix {0}, iy {0}, iz {0};

	_data->setValue(_center.i,
			_center.j,
			_center.k,
			{1.0,0.0});

	for(r = dr; r < _r; r += dr) {
		for(theta = 0; theta < M_PI; theta += dtheta) {
			for(phi = -M_PI; phi < M_PI; phi += dphi){
				dphi = atan(0.1/r);
				dtheta = atan(0.1/r);

				_data->fromSphere(&x, &y, &z,
						  r, theta, phi);

				ix = Average::doubleToInt(x) + _center.i;
				iy = Average::doubleToInt(y) + _center.j;
				iz = Average::doubleToInt(z) + _center.k;

				_data->setValue(ix, iy, iz,
						{1.0, 0.0});
				if(theta == 0) break;
			}
		}
	}
}



Pinholl::Pinholl(FFT3D::Data *data,
		 FFT3D::acoord center
	) : Primitive(data, center) {
}

void Pinholl::paint() {
	_data->setValue(_center.i,
			_center.j,
			_center.k,
			{1.0, 0.0});
}
