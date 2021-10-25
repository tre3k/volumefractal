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
 *  volumefractal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with volumefractal.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#ifndef FFT3DDATA_H
#define FFT3DDATA_H

#include <complex>
#include <string>
#include <iostream>
#include <math.h>

namespace FFT3D {
#define DATA_TYPE double

	// absolute coordinates 0 .. size
	typedef struct s_acoord {
		unsigned long i, j, k;
		double di, dj, dk;
		bool double_flag = false;
	} acoord;

	// reletive coordinates -0.5 .. 0.5
	typedef struct s_rcoord {
		double i, j, k;
	} rcoord;

	class Data1D {
	private:
		unsigned long _size{0};
		std::complex<DATA_TYPE> *_data {nullptr};
	public:
		Data1D(unsigned long int size){Init(size);}
		~Data1D(void){delete [] _data;}
		void Init(unsigned int size){
			_size = size;
			if(_data!= nullptr) delete _data;
			_data = new std::complex<DATA_TYPE>[size];
		}
		unsigned long size(void){return _size;}
		std::complex<DATA_TYPE>& operator[](unsigned long val) {
			return _data[val];
		}
		void setData(unsigned long index,std::complex<DATA_TYPE> val) {
			_data[index] = val;
		}

	};

	class Data2D {
	private:
		unsigned long _size {0};
		std::complex<DATA_TYPE> *_data {nullptr};
	public:
		Data2D(unsigned long size){Init(size);}
		std::complex<DATA_TYPE> *data(){return _data;}
		void Init(unsigned long size){
			_size = size;
			if(_data!= nullptr) delete _data;
			_data = new std::complex<DATA_TYPE>[size*size];
		}

		unsigned long size(void){return _size;}

		std::complex<DATA_TYPE> getValue(unsigned long i,
						 unsigned long j) {
			unsigned long index = i + j*_size;
			return _data[index];
		}

		void setValue(unsigned long i,
			      unsigned long j,
			      std::complex<DATA_TYPE> value) {
			unsigned long index = i + j*_size;
			_data[index] = value;
			return;
		}

	};

	class Data {
	private:
		std::complex<DATA_TYPE> *_data {nullptr};

	protected:
		unsigned long _size_x {0}, _size_y {0}, _size_z {0};

	public:
		Data(unsigned long size);
		~Data(){
			delete [] _data;
		}

		struct s_raw_file_header{
			unsigned long size;
			size_t hash_data_type;
		};

		unsigned int getIndex(unsigned long i,
				      unsigned long j,
				      unsigned long k) {
			unsigned long index {0};
			index = i + j*_size_y + k*_size_y*_size_z;
			return index;
		}

		// get value without load (for next future)
		// std::complex<DATA_TYPE> getValueFromFile(std::string filename,
		// 					 unsigned long i,
		// 					 unsigned long j,
		// 					 unsigned long k);

		std::complex<DATA_TYPE> getValue(unsigned long i,
						 unsigned long j,
						 unsigned long k) {
			return _data[getIndex(i,j,k)];
		}

		// set Value without load (for next future)
		// void setValueFromFile(std::string filename,
		// 		      unsigned long i,
		// 		      unsigned long j,
		// 		      unsigned long k,
		// 		      std::complex<DATA_TYPE> val);

		void setValue(unsigned long i,
			      unsigned long j,
			      unsigned long k,
			      std::complex<DATA_TYPE> val) {
			_data[getIndex(i,j,k)] = val;
		}

		std::complex<DATA_TYPE> getRow(unsigned int i,
					       unsigned int num) {
			unsigned long k = (int)(num/_size_y);
			unsigned long j = num - k*_size_y;
			return getValue(i,j,k);
		}

		void setRow(unsigned int i,
			    unsigned int num,std::complex<DATA_TYPE> val) {
			unsigned long k = (int)(num/_size_y);
			unsigned long j = num - k*_size_y;
			return setValue(i,j,k,val);
		}

		std::complex<DATA_TYPE> getColumn(unsigned long j,
						  unsigned long num) {
			unsigned int k = (int)(num/_size_x);
			unsigned int i = num - k*_size_x;
			return getValue(i,j,k);
		}

		void setColumn(unsigned long j,
			       unsigned long num,
			       std::complex<DATA_TYPE> val) {
			unsigned int k = (int)(num/_size_x);
			unsigned int i = num - k*_size_x;
			return setValue(i,j,k,val);
		}

		std::complex<DATA_TYPE> getDepth(unsigned long k,
						 unsigned long num) {
			unsigned int j = (int)(num/_size_x);
			unsigned int i = num - j*_size_x;
			return getValue(i,j,k);
		}

		void setDepth(unsigned long k,
			      unsigned long num,
			      std::complex<DATA_TYPE> val) {
			unsigned int j = (int)(num/_size_x);
			unsigned int i = num - j*_size_x;
			return setValue(i,j,k,val);
		}

		std::complex<DATA_TYPE>& operator[](unsigned long val){
			return _data[val];
		}

		unsigned long size_x(){return _size_x;}
		unsigned long size_y(){return _size_y;}
		unsigned long size_z(){return _size_z;}

		void WriteToRawFile(std::string filename);
		void ReadFromRawFile(std::string filename);
		void ReadOnlyHeader(std::string filename);
		static void Read2DLayerDepthFromFile(std::string filename,
						     Data2D *data,
						     unsigned long num_depth);
		static void Read2DLayerSphereFromFile(std::string filename,
						      Data2D *data,
						      double r);
		static void ReadColumnFromFile(std::string filename,
					       Data1D *data,
					       unsigned int row,
					       unsigned int depth);

		static std::string human_size(unsigned long size);

		rcoord aToRcoord(acoord coord);

		static std::complex<DATA_TYPE>
		ReadValueFromFile(std::string filename,
				  unsigned int row,
				  unsigned int column,
				  unsigned int depth);
		/* Interpolation  i,j and k range from -0.5 to 0.5 */
		static std::complex<DATA_TYPE>
		ReadValueFromFileInter(std::string filename,
				       double i,
				       double j,
				       double k,
				       unsigned int size);
		/* -0.5 < r < 0.5, angles in degree */
		static std::complex<DATA_TYPE>
		ReadValueSphere(std::string filename,
				double r,
				double phi,
				double theta);

		unsigned long long FileSize(void){
			return sizeof(s_raw_file_header) +
				_size_x*_size_y*_size_z *
				sizeof(std::complex<DATA_TYPE>);
		}

		void output_cube(bool magnitude = false);
		static void syncAccord(acoord *coord);

	};

}
#endif
