/*
 * Copyright (c) 2020 Kirill Pshenichnyi pshcyrill@mail.ru & fsbi NRC KI PNPI, LO, Russia
 * 3D Fast Fourier Transform, License: GPLv3
 */


#include <iostream>
#include <complex>
#include <chrono>
#include <string>

#include "options.h"
#include "fastfouriertransform3d.h"

void output_cube(FFT3D::Data *data, bool magnitude = false){
	for(int k=0;k<data->size_z();k++){
		std::cout << std::endl << "Depth: " << k << std::endl;
		for(int j=0;j<data->size_y();j++){
			for(int i=0;i<data->size_x();i++){
				if(magnitude)
					std::cout << abs(data->getValue(i,j,k));
				else
					std::cout << data->getValue(i,j,k);
				std::cout << "\r";
				for(int l=0;l<i+1;l++) std::cout << "\t\t\t";
			}
			std::cout << std::endl;
		}
	}
}

std::string human_size(unsigned long size){
	std::string retval;
	std::string postfix;
	int count = 0;

	double h = (double)size;

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

	retval = std::to_string(h).substr(0, std::to_string(h).find(".") + 3 + 1) + postfix;

	return retval;
}

void benchmark(unsigned int size, unsigned int threads,std::string filename,std::string in_filename){
	std::cout << std::endl << "Benchmark..." << std::endl;
	FFT3D::Data data(size);
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time,midle_time,end_time;

	std::complex<DATA_TYPE> value;

	start_time = std::chrono::high_resolution_clock::now();
	for(int k=0;k<data.size_z();k++){
		for(int j=0;j<data.size_y();j++){
			for(int i=0;i<data.size_x();i++){
				/*
				if((cos(2*M_PI*0.025*i)+cos(2*M_PI*0.025*j)+cos(2*M_PI*0.025*k))>=0){
					value = {1,0};
				}else{
					value = {-1,0};
				}
				*/
				// value = {cos(2*M_PI*i*0.123+2*M_PI*j*0.123+2*M_PI*k*0.123),0};
				value = {1.0/(i+1)*cos(2*M_PI*i*0.123+2*M_PI*j*0.0+2*M_PI*k*0.0)+
					 1.0/(j+1)*cos(2*M_PI*i*0.0+2*M_PI*j*0.133+2*M_PI*k*0.0)+
					 1.0/(k+1)*cos(2*M_PI*i*0.0+2*M_PI*j*0.0+2*M_PI*k*0.143),0};
				data.setValue(i,j,k,value);			
			}
		}
	}
	end_time = std::chrono::high_resolution_clock::now();
	std::cout << "Initial array: " << (double)(end_time-start_time).count()/1000000.0 << " ms" << std::endl << std::endl;

	if(in_filename!=""){
		std::cout << "Write to file " << in_filename << "..." << std::endl;
		start_time = std::chrono::high_resolution_clock::now();
		data.WriteToRawFile(in_filename);
		end_time = std::chrono::high_resolution_clock::now();
		double speed = data.FileSize()/(double)(end_time-start_time).count()/1e-9;
		std::cout << "Write file time: " << (double)(end_time-start_time).count()/1000000.0 << " ms ("<< human_size(speed) << "/s)" << std::endl << std::endl;
	}

	start_time = std::chrono::high_resolution_clock::now();
	FFT3D::FastFourierTransform3D *fft = new FFT3D::FastFourierTransform3D(&data);
	fft->setNumberOfThreads(threads);
	fft->GenerateFFTConsts();
	fft->GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);

	midle_time = std::chrono::high_resolution_clock::now();
	fft->calculate();
	end_time = std::chrono::high_resolution_clock::now();

	std::cout << std::endl << "Initial Time: " << (double)(midle_time-start_time).count()/1000000.0 << " ms" << std::endl;
	std::cout << "Calculation Time: " << (double)(end_time-start_time).count()/1000000.0 << " ms" << std::endl << std::endl;

	if(filename!=""){
		std::cout << "Write to file " << filename << "..." << std::endl;
		start_time = std::chrono::high_resolution_clock::now();
		data.WriteToRawFile(filename);
		end_time = std::chrono::high_resolution_clock::now();
		double speed = data.FileSize()/(double)(end_time-start_time).count()/1e-9;
		std::cout << "Write file time: " << (double)(end_time-start_time).count()/1000000.0 << " ms ("<< human_size(speed) << "/s)" << std::endl << std::endl;
	}

	delete fft;
}

void help(std::string prg){
	std::cout << prg << " [options]" << std::endl;
	std::cout << "options: " << std::endl;
	std::cout << "\t-in <filename>\r\t\t\t\tinput *.raw filename" << std::endl;
	std::cout << "\t-out <filename>\r\t\t\t\toutput *.raw filename" << std::endl;
	std::cout << "\t-j <num>\r\t\t\t\tnumber of threads" << std::endl;
	std::cout << "\t-q\r\t\t\t\tdo not ask for confirmation" << std::endl;
	std::cout << "\t-b <size>\r\t\t\t\tbenchmark for size x size x size" << std::endl;

}

int main(int argc,char *argv[]){
	std::cout << "GPLv3 (c) 2020 Kirill Pshenichnyi & fsbi NRC KI PNPI, LO, Russia" << std::endl;
	unsigned long size{0};
	unsigned long opt_benchmark{0};
	unsigned int opt_threads{1};
	std::string opt_in_filename{""},opt_out_filename{""};
	bool opt_help = false;
	bool opt_d_accept = false;

	std::string status;

	FFT3D::Data data(0);

	CmdLine::Options opt(argc,argv);
	opt.parse("-j",CmdLine::T_INT,&opt_threads);
	opt.parse("-b",CmdLine::T_INT,&opt_benchmark);
	opt.parse("-q",CmdLine::T_BOOL,&opt_d_accept);
	opt.parse("-h",CmdLine::T_BOOL,&opt_help);
	opt.parse("-out",CmdLine::T_STRING,&opt_out_filename);
	opt.parse("-in",CmdLine::T_STRING,&opt_in_filename);

	if(argc < 2 || opt_help){
		help(argv[0]); return 0;
	}

	if(opt_benchmark!=0) size = opt_benchmark;

	if(opt_out_filename==""){
		std::cout << "no output file specified! (see -out option)" << std::endl;
		if(!opt_d_accept){
			status = "";
			std::cout << "do you want to continue? ";
			while(status!="y"){
				std::cout << "y/n: ";
				std::cin >> status;
				if(status=="n" || status=="q") return 0;
			}
		}
	}

	if(opt_in_filename=="" && opt_benchmark==0){
		std::cout << "no input file specified (see -in option)" << std::endl;
		std::cout << "nothing to do. Exiting." << std::endl;
		return 0;
	}

	if(opt_benchmark==0){
		data.ReadOnlyHeader(opt_in_filename);
		size = data.size_x();
	}
	std::cout << "Size: " << size << "x" << size << "x" << size << std::endl;
	std::cout << "\033[1mThreads: " << opt_threads << "\033[0m\n";
	std::cout << "On current system DATA_TYPE size: " << sizeof(DATA_TYPE) << " bytes or " << sizeof(DATA_TYPE)*8 << " bits" << std::endl;
	unsigned long long ram_size = sizeof(std::complex<DATA_TYPE>)*(size*size*size+size*opt_threads);
	std::cout << "you need RAM size: " << ram_size << " bytes (" << human_size(ram_size) << ")" << std::endl;
	if(opt_out_filename!=""){
		unsigned long long disk_space = sizeof(FFT3D::Data::s_raw_file_header)+size*size*size*sizeof(std::complex<DATA_TYPE>);
		std::cout << "you need disk space size: " << disk_space << " bytes (" << human_size(disk_space) << ") for " << opt_out_filename << std::endl;
	}
	if(!opt_d_accept){
		status = "";
		while(status!="y"){
			std::cout << "y/n: ";
			std::cin >> status;
			if(status=="n" || status=="q") return 0;
		}
	}
	if(opt_benchmark!=0){
		benchmark(size,opt_threads,opt_out_filename,opt_in_filename);
		std::cout << "Exiting." << std::endl;
		return 0;
	}

	data.ReadFromRawFile(opt_in_filename);
	FFT3D::FastFourierTransform3D *fft = new FFT3D::FastFourierTransform3D(&data);
	fft->setNumberOfThreads(opt_threads);
	fft->GenerateFFTConsts();
	fft->GeneratePermutation(FFT3D::Permutations::P_CENTER_ZERO);
	std::cout << "calculation..." << std::endl;
	fft->calculate();
	std::cout << "Write results..." << std::endl;
	data.WriteToRawFile(opt_out_filename);
	std::cout << "Done." << std::endl;

	return 0;
}

