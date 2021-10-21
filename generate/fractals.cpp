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


#include <vector>
#include "fractals.h"

using namespace Fractals;

DaVince3D::DaVince3D(FFT3D::Data *data, int iteration) {
	_data = data;
	/* set maximum size of element from size of array _data,
	   we can place 4 elements in space */
	start_size_element = _data->size_x()/4;
	setIteration(iteration);
}

void DaVince3D::element(FFT3D::acoord pos,
			 int size,
			 int age) {
	if(age == 1) {
		std::vector<Primitives::Cube> cubes;
		for(int i=0; i<8; i++){
			Primitives::Cube cube(_data);
			cube.setSize(size);
			cubes.push_back(cube);
		}
		cubes[0].setKeyPosition(pos);
		cubes[1].setKeyPosition({
				pos.i + (int)(3 * size),
				pos.j,
				pos.k,
			});
		cubes[2].setKeyPosition({
				pos.i,
				pos.j + (int)(3 * size),
				pos.k,
			});
		cubes[3].setKeyPosition({
				pos.i + (int)(3 * size),
				pos.j + (int)(3 * size),
				pos.k,
			});
		cubes[4].setKeyPosition({
				pos.i,
				pos.j,
				pos.k + (int)(3 * size),
			});
		cubes[5].setKeyPosition({
				pos.i + (int)(3 * size),
				pos.j,
				pos.k + (int)(3 * size),
			});
		cubes[6].setKeyPosition({
				pos.i,
				pos.j + (int)(3 * size),
				pos.k + (int)(3 * size),
			});
		cubes[7].setKeyPosition({
				pos.i + (int)(3 * size),
				pos.j + (int)(3 * size),
				pos.k + (int)(3 * size),
			});
		for(int i=0; i<8; i++) cubes[i].paint();
	}

	if(age > 1) {
		element(pos, size/4, age-1);
		element({
				pos.i + (int)(3 * size),
				pos.j,
				pos.k,
			}, size/4, age-1);
		element({
				pos.i,
				pos.j + (int)(3 * size),
				pos.k,
			}, size/4, age-1);
		element({
				pos.i + (int)(3 * size),
				pos.j + (int)(3 * size),
				pos.k,
			}, size/4, age-1);
		element({
				pos.i,
				pos.j,
				pos.k + (int)(3 * size),
			}, size/4, age-1);
		element({
				pos.i + (int)(3 * size),
				pos.j,
				pos.k + (int)(3 * size),
			}, size/4, age-1);
		element({
				pos.i,
				pos.j + (int)(3 * size),
				pos.k + (int)(3 * size),
			}, size/4, age-1);
		element({
				pos.i + (int)(3 * size),
				pos.j + (int)(3 * size),
				pos.k + (int)(3 * size),
			}, size/4, age-1);
	}

	Primitives::Cube central_cube(_data);
	central_cube.setSize(2*size);
	central_cube.setKeyPosition({
			pos.i + size,
			pos.j + size,
			pos.k + size,
		});
	central_cube.paint();
}

void DaVince3D::generate() {
	element({0, 0, 0},
		start_size_element,
		_iteration);
}

void DaVince3D::setIteration(int itteration) {
	_iteration = itteration;
}

int DaVince3D::getIteration() {
	return _iteration;
}

/* size = 4^iteration */
void DaVince3D::setMaximumItteration() {
	_iteration = log(_data->size_x())/log(4);
}

int DaVince3D::getSizeFromItteration(int intteration) {
	return pow(4, intteration);
}
