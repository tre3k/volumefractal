/*
 * Copyright (c) 2020 Kirill Pshenichnyi pshcyrill@mail.ru & fsbi NRC KI PNPI, LO, Russia
 * 3D Fast Fourier Transform, License: GPLv3
 */

#include "fft3ddata.h"

using namespace FFT3D;

Data::Data(unsigned long int size) {
	_size_x = size;
	_size_y = _size_x;
	_size_z = _size_x;
	_data = new std::complex<DATA_TYPE> [(unsigned long) _size_x*_size_y*_size_z];
}

void Data::WriteToRawFile(std::string filename) {
	std::FILE *f = std::fopen(filename.c_str(),"w");

	s_raw_file_header header;
	header.size = _size_x;
	header.hash_data_type = typeid(DATA_TYPE).hash_code();

	std::fwrite(&header,1,sizeof(s_raw_file_header),f);
	std::fwrite(_data,sizeof(std::complex<DATA_TYPE>),header.size*header.size*header.size,f);

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

	_data = new std::complex<DATA_TYPE>[header.size*header.size*header.size];
	std::fread(_data,sizeof(std::complex<DATA_TYPE>),header.size*header.size*header.size,f);
	std::fclose(f);
}

void Data::ReadOnlyHeader(std::string filename) {
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	_size_x = header.size;
	_size_y = _size_x;
	_size_z = _size_x;

	std::fclose(f);

}

void Data::Read2DLayerDepthFromFile(std::string filename, Data2D *data, unsigned long num_depth) {
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	data->Init(header.size);
	std::fseek(f,num_depth*header.size*header.size*sizeof(std::complex<DATA_TYPE>),SEEK_CUR);
	std::fread(data->data(),sizeof(std::complex<DATA_TYPE>),header.size*header.size,f);

	std::fclose(f);
}

void Data::ReadColumnFromFile(std::string filename, Data1D *data, unsigned int row, unsigned int depth){
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	data->Init(header.size);
	unsigned long index = 0;
	std::complex<DATA_TYPE> tmp;
	for(int i=0;i<header.size;i++){
		index = row + i*header.size + depth*header.size*header.size;
		std::fseek(f,sizeof(s_raw_file_header)+index*sizeof(std::complex<DATA_TYPE>),SEEK_SET);
		std::fread(&tmp,sizeof(std::complex<DATA_TYPE>),1,f);
		data->setData(i,tmp);
	}

	std::fclose(f);
}
