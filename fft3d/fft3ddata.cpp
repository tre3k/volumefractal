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
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#include "fft3ddata.h"

using namespace FFT3D;

Data::Data(unsigned long int size) {
	_size_x = size;
	_size_y = _size_x;
	_size_z = _size_x;
	_data = new std::complex<DATA_TYPE>
		[(unsigned long) _size_x*_size_y*_size_z];
}

void Data::WriteToRawFile(std::string filename) {
	std::FILE *f = std::fopen(filename.c_str(), "w");

	s_raw_file_header header;
	header.size = _size_x;
	header.hash_data_type = typeid(DATA_TYPE).hash_code();

	std::fwrite(&header, 1, sizeof(s_raw_file_header), f);
	std::fwrite(_data,
		    sizeof(std::complex<DATA_TYPE>),
		    header.size*header.size*header.size, f);

	std::fclose(f);
}

void Data::ReadFromRawFile(std::string filename){
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	_size_x = header.size;
	_size_y = _size_x;
	_size_z = _size_x;

	if(_data!=nullptr) delete _data;

	_data = new std::complex<DATA_TYPE>[
		header.size*header.size*header.size
		];
	std::fread(_data,
		   sizeof(std::complex<DATA_TYPE>),
		   header.size*header.size*header.size, f);
	std::fclose(f);
}

void Data::ReadOnlyHeader(std::string filename) {
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header, 1, sizeof(s_raw_file_header), f);
	_size_x = header.size;
	_size_y = _size_x;
	_size_z = _size_x;

	std::fclose(f);
}

void Data::Read2DLayerDepthFromFile(std::string filename,
				    Data2D *data,
				    unsigned long num_depth) {
	std::FILE *f = std::fopen(filename.c_str(), "r");

	s_raw_file_header header;
	std::fread(&header, 1, sizeof(s_raw_file_header), f);
	data->Init(header.size);
	std::fseek(f, num_depth*header.size*header.size *
		   sizeof(std::complex<DATA_TYPE>),
		   SEEK_CUR);
	std::fread(data->data(),
		   sizeof(std::complex<DATA_TYPE>),
		   header.size*header.size, f);

	std::fclose(f);
}

void Data::Read2DLayerSphereFromFile(std::string filename,
				     Data2D *data,
				     double r) {
	std::FILE *f = std::fopen(filename.c_str(),"r");
	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	fclose(f);
	data->Init(header.size);

	double dphi = 360.0/header.size;
	double dtheta = 360.0/header.size;

	std::complex<DATA_TYPE> value;

	int i = 0, j = 0;

	for(double phi = -180; phi < 180; phi += dphi){
		for(double theta = -180; theta < 180; theta += dphi){
			value = ReadValueSphere(filename,r,phi,theta);
			data->setValue(i,j,value);
			j++;
		}
		i++; j=0;
	}
}

void Data::ReadColumnFromFile(std::string filename,
			      Data1D *data,
			      unsigned int row,
			      unsigned int depth){
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	data->Init(header.size);
	unsigned long index = 0;
	std::complex<DATA_TYPE> tmp;
	for(int i=0; i<header.size; i++) {
		index = row + i*header.size + depth*header.size*header.size;
		std::fseek(f, sizeof(s_raw_file_header) +
			   index*sizeof(std::complex<DATA_TYPE>), SEEK_SET);
		std::fread(&tmp,sizeof(std::complex<DATA_TYPE>),1,f);
		data->setData(i,tmp);
	}

	std::fclose(f);
}

std::complex<DATA_TYPE> Data::ReadValueFromFile(std::string filename,
						unsigned int row,
						unsigned int column,
						unsigned int depth){
	std::complex<DATA_TYPE> retval {0.0,0.0};

	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	if(row > header.size || column > header.size || depth > header.size) return retval;
	if(row < 0 || column < 0 || depth < 0) return retval;

	unsigned int index = row + column*header.size + depth*header.size*header.size;
	std::fseek(f,sizeof(s_raw_file_header)+index*sizeof(std::complex<DATA_TYPE>),SEEK_SET);
	std::fread(&retval,sizeof(std::complex<DATA_TYPE>),1,f);

	std::fclose(f);
	return retval;
}

std::complex<DATA_TYPE> Data::ReadValueFromFileInter(std::string filename,
						     double i,
						     double j,
						     double k,
						     unsigned int size){
	std::complex<DATA_TYPE> retval {0.0,0.0};
	double d_row, d_column, d_depth;

	/* denormalize */
	d_row = i*size + 0.5*size;
	d_column = j*size + 0.5*size;
	d_depth = k*size + 0.5*size;

	/* Trilinear interpolation */
	// rounding to integers
	double x1 = double ((int) d_row);
	double x2 = x1+1;
	double y1 = double ((int) d_column);
	double y2 = y1+1;
	double z1 = double ((int) d_depth);
	double z2 = z1+1;

	// std::cout << d_row << " " << d_column << " " << d_depth << "\n";
	// std::cout << "x1: " << x1 << " x2: " << x2 << "\n";
	// std::cout << "y1: " << y1 << " y2: " << y2 << "\n";
	// std::cout << "z1: " << z1 << " z2: " << z2 << "\n";

	auto p1 = ReadValueFromFile(filename,x1,y1,z1);
	auto p2 = ReadValueFromFile(filename,x2,y1,z1);
	auto p3 = ReadValueFromFile(filename,x2,y1,z2);
	auto p4 = ReadValueFromFile(filename,x2,y2,z1);
	auto p5 = ReadValueFromFile(filename,x2,y2,z2);

	retval =
		p1*(x2-d_row)*(y2-d_column)*(z2-d_depth) +
		p2*(d_row-x1)*(y2-d_column)*(z2-d_depth) +
		p3*(d_row-x1)*(y2-d_column)*(d_depth-z1) +
		p4*(d_row-x1)*(d_column-y1)*(z2-d_depth) +
		p5*(d_row-x1)*(d_column-y1)*(d_depth-z1);

	return retval;
}

std::complex<DATA_TYPE> Data::ReadValueSphere(std::string filename,
					      double r,
					      double phi,
					      double theta){
	double i, j, k;
	int size;

	std::FILE *f = std::fopen(filename.c_str(),"r");
	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	fclose(f);

	i = r*sin(M_PI*theta/180.0)*cos(M_PI*phi/180.0);
	j = r*sin(M_PI*theta/180.0)*sin(M_PI*phi/180.0);
	k = r*cos(M_PI*theta/180.0);

	// std::cout << i << " " << j << " " << k << " " << "\n";

	return Data::ReadValueFromFileInter(filename,i,j,k,header.size);
}


// This function out the size in human format
// long int -> string
std::string Data::human_size(unsigned long size){
	std::string retval;
	std::string postfix;
	int count = 0;

	double h = (double) size;

	while(h>=1024){
		h /= 1024;
		count ++;
	}

	switch (count) {
        case 0:
		postfix = " bytes";
		break;
        case 1:
		postfix = " KB";
		break;
        case 2:
		postfix = " MB";
		break;
        case 3:
		postfix = " GB";
		break;
        case 4:
		postfix = " TB";
		break;
        case 5:
		postfix = " PB";
		break;
	}

	retval = std::to_string(h).
		substr(0, std::to_string(h).find(".") + 3 + 1) + postfix;

	return retval;
}
