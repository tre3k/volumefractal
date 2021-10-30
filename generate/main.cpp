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


#include <iostream>
#include <getopt.h>
#include <fft3ddata.h>
#include <string>

#include "fractals.h"

void version(){
	std::cout << "generate 3D fractals, v0.9 betta" << std::endl;
	std::cout <<
		"GPLv3 (c) Copyright (c) 2020-2021 NRC KI PNPI, "
		"Gatchina, LO, 188300 Russia" <<
		std::endl;
	std::cout << "\tAuthor: Kirill Pshenichnyi <pshcyrill@mail.ru>" <<
		std::endl;

	std::cout << "Source code: https://github.com/tre3k/volumefractal" <<
		std::endl << std::endl;

}

void help(char *progname) {
	version();
	std::cout << "Usage: " << progname << " [options]" << std::endl;
	std::cout << "Options: " << std::endl;

	std::cout << "\t-h, --help\r\t\t\t\t\tThis message" <<
		std::endl;
	std::cout << "\t-v, --version\r\t\t\t\t\tShow version" <<
		std::endl;
	std::cout << "\t-s, --size=<size>\r\t\t\t\t\t"
		"Size of the array data <size>x<size>x<size>" <<
		std::endl;
	std::cout << "\t-o, --output=<filename>\r\t\t\t\t\t"
		"Output filename" <<
		std::endl;
	std::cout << "\t-i, --iteration=<num>\r\t\t\t\t\t"
		"Number of iteration steps" <<
		std::endl;
	std::cout << "\t--noconfirm\r\t\t\t\t\t" <<
		"Do not ask for any confirmation" << std::endl;
	std::cout << "\t-f, --fractal=<fractal>\r\t\t\t\t\t"
		"Selection of a fractal, such as:" << std::endl <<
		"\r\t\t\t\t\t1. \"davinci\" - Da Vinci 3D tree" <<
		std::endl <<
		"\r\t\t\t\t\t2. \"pinholl\" - Just one pixel in center" <<
		std::endl <<
		"\r\t\t\t\t\t3. \"sphera\" - Draw sphera" <<
		std::endl;

	std::cout << std::endl;
}

void confirm(int flag, unsigned long size) {
	std::cout << "size: " << size << "x" <<
		size << "x" << size <<
		" (" << FFT3D::Data::human_size(
			sizeof(std::complex<DATA_TYPE>) *
			size * size * size) <<
		")" << std::endl;

	std::string status;
	if(!flag) {
		status = "";
		std::cout << "do you want to continue? ";
		while(status != "y"){
			std::cout << "y/n: ";
			std::cin >> status;
			if(status == "n" || status == "q") exit(0);
		}
	}

}

int main(int argc, char *argv[]) {
	/* default options */
	int size = 64;
	bool default_size {true};

	int iteration = 3;
	bool default_intteration {true};

	std::string output_file_name = "out.raw";
	bool default_output_file_name {true};

	std::string str_fractal;
	int fractal {0};


	FFT3D::Data *data;
	Fractals::DaVince3D *davince3d;

	Primitives::Pinholl *pinholl;
	Primitives::Sphera *sphera;

	int no_confirm_flag {0};

	/* for opt = getopt(...) */
	int opt;
	static struct option long_options[] = {
		{"version", no_argument, 0, 'v'},
		{"help", no_argument, 0, 'h'},
		{"size", required_argument, 0, 's'},
		{"output", required_argument, 0, 'o'},
		{"itteration", required_argument, 0, 'i'},
		{"fractal", required_argument, 0, 'f'},
		{"noconfirm", no_argument, &no_confirm_flag, 1},
		{0, 0 ,0 ,0}
	};

	int option_index {0};

	if(argc < 2) {
		help(argv[0]);
		exit(0);
	}

	while(1) {
		opt = getopt_long(argc, argv, "hs:o:i:f:",
				  long_options, &option_index);

		if(opt < 0) break;
		switch(opt) {
		case 0:

			break;

		case 'v':
			version();
			exit(0);
			break;

		case 'h':
			help(argv[0]);
			exit(0);
			break;

		case 's':
			size = std::atoi(optarg);
			default_size = false;
			break;

		case 'o':
			output_file_name = std::string(optarg);
			default_output_file_name = false;
			break;

		case 'i':
			iteration = std::atoi(optarg);
			default_intteration = false;
			break;

		case 'f':
			str_fractal = std::string(optarg);
			if(str_fractal == "davinci" || str_fractal == "1") {
				str_fractal = "davinci";
				fractal = Fractals::DAVINCI;
			}

			if(str_fractal == "pinholl" || str_fractal == "2"){
				str_fractal = "pinholl";
				fractal = Fractals::PINHOLL;
			}

			if(str_fractal == "sphera" || str_fractal == "3") {
				str_fractal = "sphera";
				fractal = Fractals::SPHERA;
			}

			break;

		case '?':

			break;

		}
	}

	if(str_fractal == "") {
		help(argv[0]);
		std::cout << "Please set option -f, --fractal!" << std::endl;
		return 0;
	}

	std::cout << "generate " << str_fractal << " object." << std::endl;


	FFT3D::acoord center = {
		(unsigned long) size/2,
		(unsigned long) size/2,
		(unsigned long) size/2
	};

	switch(fractal){
	default:
		std::cout << "Please set correct option for fractal " <<
			"see --help" << std::endl;
		return 0;
		break;

	case Fractals::SPHERA:
		confirm(no_confirm_flag, size);
		data = new FFT3D::Data(size);
		sphera = new Primitives::Sphera(data,
						center,
						size/2/iteration);
		sphera->paint();
		break;

	case Fractals::PINHOLL:
		confirm(no_confirm_flag, size);
		data = new FFT3D::Data(size);
		pinholl = new Primitives::Pinholl(data, center);
		pinholl->paint();
		break;

	case Fractals::DAVINCI:
		std::cout << "Da Vinci 3D tree!\n";
		if(default_size)
			size = Fractals::DaVince3D::getSizeFromItteration(
				iteration-1
				);

		confirm(no_confirm_flag, size);

		data = new FFT3D::Data(size);
		davince3d = new Fractals::DaVince3D(data, iteration-1);
		if (default_intteration && !default_size)
			davince3d->setMaximumItteration();
		std::cout << "Start generate the tree of da Vinci: " <<
			davince3d->getIteration()+1 << " steps iteration" <<
			std::endl;
		davince3d->generate();
		break;

	}

	std::cout << "write to output file: " <<
		output_file_name <<
		" (" << FFT3D::Data::human_size(data->FileSize()) << ")..." <<
		std::endl;
	data->WriteToRawFile(output_file_name);
	std::cout << "done." << std::endl;

	return 0;
}
