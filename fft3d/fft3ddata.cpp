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

void Data::Read2DLayerSphereFromFile(std::string filename, Data2D *data, double phi, double theta){
	std::FILE *f = std::fopen(filename.c_str(),"r");
	std::cout << "call Read2DLayerSphereFromFile\n";

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	fclose(f);
	data->Init(header.size);

	double i,j,k;
	
	auto value = ReadValueFromFileInter(filename, i, j, k, header.size);
	
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

std::complex<DATA_TYPE> Data::ReadValueFromFile(std::string filename, unsigned int row, unsigned int column, unsigned int depth){
	std::complex<DATA_TYPE> retval {0.0,0.0};
	
	std::FILE *f = std::fopen(filename.c_str(),"r");

	s_raw_file_header header;
	std::fread(&header,1,sizeof(s_raw_file_header),f);
	if(row >= header.size || column >= header.size || depth >= header.size) return retval;
	if(row < 0 || column < 0 || depth < 0) return retval;
	
	unsigned int index = row + column*header.size + depth*header.size*header.size;
	std::fseek(f,sizeof(s_raw_file_header)+index*sizeof(std::complex<DATA_TYPE>),SEEK_SET);
	std::fread(&retval,sizeof(std::complex<DATA_TYPE>),1,f);
	
	std::fclose(f);
	return retval;
}

std::complex<DATA_TYPE> Data::ReadValueFromFileInter(std::string filename, double i, double j, double k, unsigned int size){
	std::complex<DATA_TYPE> retval {0.0,0.0};

	double d_row,d_column,d_depth;
	d_row = i*size + 0.5*size;
	d_column = j*size + 0.5*size;
	d_depth = k*size + 0.5*size;

	std::cout << d_row << " " << d_column << " " << d_depth << "\n";
	
	//retval = ReadValueFromFile(filename,512,512,512);
	
	return retval;
}

