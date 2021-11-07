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
	setIteration(iteration);
}

void DaVince3D::element(FFT3D::acoord pos,
			 int size,
			 int age) {

	int shift = size * SizeElement(age-1);
	int c_size = size * SizeCentral(age);

	if(age == 2) {
		std::vector<Primitives::Cube> cubes;
		for(int i=0; i < 8; i++){
			Primitives::Cube cube(_data);
			cube.setSize(size);
			cubes.push_back(cube);
		}
		cubes[0].setKeyPosition(pos);
		cubes[1].setKeyPosition({
				pos.i + c_size + shift,
				pos.j,
				pos.k,
			});
		cubes[2].setKeyPosition({
				pos.i,
				pos.j + c_size + shift,
				pos.k,
			});
		cubes[3].setKeyPosition({
				pos.i + c_size + shift,
				pos.j + c_size + shift,
				pos.k,
			});
		cubes[4].setKeyPosition({
				pos.i,
				pos.j,
				pos.k + c_size + shift,
			});
		cubes[5].setKeyPosition({
				pos.i + c_size + shift,
				pos.j,
				pos.k + c_size + shift,
			});
		cubes[6].setKeyPosition({
				pos.i,
				pos.j + c_size + shift,
				pos.k + c_size + shift,
			});
		cubes[7].setKeyPosition({
				pos.i + c_size + shift,
				pos.j + c_size + shift,
				pos.k + c_size + shift,
			});
		for(int i=0; i<8; i++) cubes[i].paint();
	}

	if(age > 2) {
		element(pos, size, age-1);
		element({
				pos.i + c_size + shift,
				pos.j,
				pos.k,
			}, size, age-1);
		element({
				pos.i,
				pos.j + c_size + shift,
				pos.k,
			}, size, age-1);
		element({
				pos.i + c_size + shift,
				pos.j + c_size + shift,
				pos.k,
			}, size, age-1);
		element({
				pos.i,
				pos.j,
				pos.k + c_size + shift,
			}, size, age-1);
		element({
				pos.i + c_size + shift,
				pos.j,
				pos.k + c_size + shift,
			}, size, age-1);
		element({
				pos.i,
				pos.j + c_size + shift,
				pos.k + c_size + shift,
			}, size, age-1);
		element({
				pos.i + c_size + shift,
				pos.j + c_size + shift,
				pos.k + c_size + shift,
			}, size, age-1);
	}


	Primitives::Cube central_cube(_data);
	central_cube.setSize(c_size);
	central_cube.setKeyPosition({
			pos.i + shift,
			pos.j + shift,
			pos.k + shift,
		});
	central_cube.paint();
}

void DaVince3D::generate() {
	element(position,
		minimum_size_element,
		_iteration);
}

void DaVince3D::setIteration(int itteration) {
	_iteration = itteration;
}

int DaVince3D::getIteration() {
	return _iteration;
}

void DaVince3D::setMaximumItteration() {
	_iteration = 1;
	int size = _data->size_x();
	while(size >= minimum_size_element * SizeElement(_iteration)){
		_iteration ++;
	}
	_iteration --;
}

int DaVince3D::getSizeFromIteration(int iteration) {
	return SizeElement(iteration);
}

/* return size of element (central cube + 8 elements (n-1) step)
   from step iteration "age" */
int DaVince3D::SizeElement(int age) {
	if(age <= 0) return 0;
	if(age == 1) return 1;
	return SizeCentral(age) + 2 * SizeElement(age-1);
}

/* return size of central cube from step iteration "age" */
int DaVince3D::SizeCentral(int age) {
	if(age <= 1) return 1;
	return _speed * SizeCentral(age-1);
}

void DaVince3D::setMinimumSizeElement(int size) {
	minimum_size_element = size;
}

void DaVince3D::setPosition(FFT3D::acoord pos) {
	position = pos;
}

void DaVince3D::setSpeed(int speed) {
	_speed = speed;
}
