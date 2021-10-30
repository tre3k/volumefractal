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


#include <iostream>
#include <complex>
#include <chrono>
#include <string>

#include <getopt.h>

#include "fastfouriertransform3d.h"

void version(){
	std::cout << "3D fast fourier transform, (volumefractals) v0.9 betta"
		  << std::endl;
	std::cout << "GPLv3 (c) Copyright (c) 2020-2021 NRC KI PNPI, "
		"Gatchina, LO, 188300 Russia" <<
		std::endl;
	std::cout << "\tAuthor: Kirill Pshenichnyi <pshcyrill@mail.ru>" <<
		std::endl << std::endl;

	std::cout << "Source code: https://github.com/tre3k/volumefractal" <<
		std::endl << std::endl;
}

void help(std::string prg) {
	version();
	std::cout << prg << " [options]" << std::endl;
	std::cout << "options: " << std::endl;
	std::cout << "\t-h, --help\r\t\t\t\t" <<
		"Show this message" << std::endl;
	std::cout << "\t-v, --version\r\t\t\t\t" <<
		"Show version" << std::endl;
	std::cout << "\t-i, --input=<filename>\r\t\t\t\t" <<
		"Input *.raw filename" <<
		std::endl;
	std::cout << "\t-o, --output=<filename>\r\t\t\t\t" <<
		"Output *.raw filename" <<
		std::endl;
	std::cout << "\t-j, --jobs=<num>\r\t\t\t\t" <<
		"Number of threads" <<
		std::endl;
	std::cout << "\t--noconfirm\r\t\t\t\t" <<
		"Do not ask for confirmation" <<
		std::endl;
	std::cout << "\t-b, --benchmark=<size>\r\t\t\t\t" <<
		"Benchmark for size x size x size" <<
		std::endl << std::endl;

}

void benchmark(unsigned int size,
	       unsigned int threads,
	       std::string filename,
	       std::string in_filename
	) {
	std::cout << std::endl << "Benchmark..." << std::endl;
	FFT3D::Data data(size);
	std::chrono::time_point <std::chrono::high_resolution_clock>
		start_time,
		midle_time,
		end_time;

	std::complex<DATA_TYPE> value;

	start_time = std::chrono::high_resolution_clock::now();
	for(int k=0;k<data.size_z();k++){
		for(int j=0;j<data.size_y();j++){
			for(int i=0;i<data.size_x();i++){
				value = {cos(2*M_PI*i*0.1 +
					     2*M_PI*j*0.0 +
					     2*M_PI*k*0.0) +
					cos(2*M_PI*i*0.0 +
					    2*M_PI*j*0.2 +
					    2*M_PI*k*0.0) +
					cos(2*M_PI*i*0.0 +
					    2*M_PI*j*0.0 +
					    2*M_PI*k*0.3), 0};

				data.setValue(i,j,k,value);
			}
		}
	}
	end_time = std::chrono::high_resolution_clock::now();
	std::cout << "Initial array: " <<
		(double)(end_time-start_time).count()/1000000.0 <<
		" ms" << std::endl << std::endl;

	if(in_filename != ""){
		std::cout << "Write to file " << in_filename << "..." <<
			std::endl;
		start_time = std::chrono::high_resolution_clock::now();
		data.WriteToRawFile(in_filename);
		end_time = std::chrono::high_resolution_clock::now();
		double speed =
			data.FileSize() /
			(double)(end_time-start_time).count() / 1e-9;
		std::cout << "Write file time: " <<
			(double)(end_time-start_time).count() / 1000000.0 <<
			" ms ("<< FFT3D::Data::human_size(speed) << "/s)" <<
			std::endl << std::endl;
	}

	start_time = std::chrono::high_resolution_clock::now();
	auto fft = new FFT3D::FastFourierTransform3D(&data);
	fft->setNumberOfThreads(threads);
	fft->GenerateFFTConsts();
	fft->GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);

	midle_time = std::chrono::high_resolution_clock::now();
	fft->calculate();
	end_time = std::chrono::high_resolution_clock::now();

	std::cout << std::endl << "Initial Time: " <<
		(double)(midle_time-start_time).count()/1000000.0 <<
		" ms" << std::endl;
	std::cout << "Calculation Time: " <<
		(double)(end_time-start_time).count() / 1000000.0 <<
		" ms" << std::endl << std::endl;

	if(filename != ""){
		std::cout << "Write to file " <<
			filename << "..." << std::endl;
		start_time = std::chrono::high_resolution_clock::now();
		data.WriteToRawFile(filename);
		end_time = std::chrono::high_resolution_clock::now();
		double speed = data.FileSize() /
			(double)(end_time-start_time).count()/1e-9;
		std::cout << "Write file time: " <<
			(double)(end_time-start_time).count()/1000000.0 <<
			" ms ("<< FFT3D::Data::human_size(speed) << "/s)" <<
			std::endl << std::endl;
	}

	delete fft;
}

int main(int argc,char *argv[]) {
	unsigned long size {0};
	unsigned long size_for_benchmark {0};
	unsigned int threads {1};
	std::string in_filename {""}, out_filename {""};

	static int no_confirm_flag;

	std::string status;

	FFT3D::Data data(0);

	int opt;
	int option_index {0};

	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"version", no_argument, 0, 'v'},
		{"jobs", required_argument, 0, 'j'},
		{"benchmark", required_argument, 0, 'b'},
		{"noconfirm", no_argument, &no_confirm_flag, 1},
		{"output", required_argument, 0, 'o'},
		{"input", required_argument, 0, 'i'},
		{0, 0, 0, 0}
	};

	if(argc < 2){
		help(argv[0]);
		return 0;
	}

	while(1) {
		opt = getopt_long(argc, argv, "hvj:b:o:i:",
				  long_options, &option_index);

		if(opt < 0) break;

		switch(opt) {
		case 'h':
			help(argv[0]);
			return 0;
			break;

		case 'v':
			version();
			return 0;
			break;

		case 'b':
			size_for_benchmark = std::atoi(optarg);
			size = size_for_benchmark;
			break;

		case 'i':
			in_filename = std::string(optarg);
			break;

		case 'o':
			out_filename = std::string(optarg);
			break;

		case 'j':
			threads = std::atoi(optarg);
			break;

		}

	}

	if(out_filename == ""){
		std::cout << "no output file specified! "
			"(see --output option)" <<
			std::endl;
		if(!no_confirm_flag){
			status = "";
			std::cout << "do you want to continue? ";
			while(status != "y"){
				std::cout << "y/n: ";
				std::cin >> status;
				if(status == "n" || status == "q") return 0;
			}
		}
	}

	if(in_filename == "" && size_for_benchmark == 0){
		std::cout << "no input file specified (see --input option)"
			  << std::endl;
		std::cout << "nothing to do. Exiting." << std::endl;
		return 0;
	}

	if(size_for_benchmark == 0){
		data.ReadOnlyHeader(in_filename);
		size = data.size_x();
	}

	std::cout << "Size: " << size << "x" << size << "x" << size <<
		std::endl;
	std::cout << "\033[1mThreads: " << threads << "\033[0m\n";
	std::cout << "On current system DATA_TYPE size: " <<
		sizeof(DATA_TYPE) <<
		" bytes or " <<
		sizeof(DATA_TYPE) * 8 << " bits" << std::endl;

	unsigned long long ram_size =
		sizeof(std::complex<DATA_TYPE>) *
		(size * size * size + size * threads);
	std::cout << "you need RAM size: " <<
		ram_size << " bytes (" <<
		FFT3D::Data::human_size(ram_size) << ")" <<
		std::endl;

	if(out_filename != "") {
		unsigned long long disk_space =
			sizeof(FFT3D::Data::s_raw_file_header) +
			size * size * size * sizeof(std::complex<DATA_TYPE>);
		std::cout << "you need disk space size: " <<
			disk_space << " bytes (" <<
			FFT3D::Data::human_size(disk_space) <<
			") for " << out_filename << std::endl;
	}
	if(!no_confirm_flag) {
		status = "";
		while(status!="y"){
			std::cout << "y/n: ";
			std::cin >> status;
			if(status == "n" || status == "q") return 0;
		}
	}
	if(size_for_benchmark != 0) {
		benchmark(size, threads, out_filename, in_filename);
		std::cout << "Exiting." << std::endl;
		return 0;
	}

	if(out_filename == "") out_filename = "out.raw";

	data.ReadFromRawFile(in_filename);
	auto fft = new FFT3D::FastFourierTransform3D(&data);
	fft->setNumberOfThreads(threads);
	fft->GenerateFFTConsts();
	fft->GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);
	std::cout << "calculation..." << std::endl;
	fft->calculate();
	std::cout << "Write results to " << out_filename <<
		"..." << std::endl;
	data.WriteToRawFile(out_filename);
	std::cout << "Done." << std::endl;

	return 0;
}
