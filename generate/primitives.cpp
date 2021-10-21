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
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
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

}

void Cube::calculateCenter() {

}
