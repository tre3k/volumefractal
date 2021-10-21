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

void help(char *progname) {
	std::cout << "Usage: " << progname << " [options]" << std::endl;
	std::cout << "Options: " << std::endl;

	std::cout << "\t-h, --help\r\t\t\t\t\tThis message" <<
		std::endl;
	std::cout << "\t-s, --size=<size>\r\t\t\t\t\t"
		"Size of data" <<
		std::endl;
	std::cout << "\t-o, --output=<filename>\r\t\t\t\t\t"
		"Output filename" <<
		std::endl;
	std::cout << "\t-i, --itteration=<num>\r\t\t\t\t\t"
		"Step itteration count" <<
		std::endl;
	std::cout << "\t-f, --fractal=<fractal>\r\t\t\t\t\t"
		"select fractal like as:" << std::endl <<
		"\r\t\t\t\t\t\"davinci\" - Da Vinci 3D tree" <<
		std::endl;


	std::cout << std::endl;
}

void generateDaVinci(bool default_size,
		    int size,
		    bool default_intteration,
		    int iteration,
		    std::string output_file_name) {

	std::cout << "Da Vinci 3D tree!\n";
	if(default_size)
		size = Fractals::DaVince3D::getSizeFromItteration(
			iteration-1
			);

	std::cout << "size: " <<
		size << "x" << size << "x" << size <<
		std::endl;

	FFT3D::Data data(size);

	Fractals::DaVince3D davince3d(&data, iteration-1);
	if (default_intteration && !default_size)
		davince3d.setMaximumItteration();
	std::cout << "Start generate the tree of da Vinci: " <<
		davince3d.getIteration()+1 << " steps iteration" << std::endl;

	davince3d.generate();
	std::cout << "write to output file: " <<
		output_file_name <<
		" (" << FFT3D::Data::human_size(data.FileSize()) << ")" <<
		std::endl;
	data.WriteToRawFile(output_file_name);
	std::cout << "done." << std::endl;
}

int main(int argc, char *argv[]) {

	std::cout <<
		"GPLv3 (c) Copyright (c) 2020-2021 NRC KI PNPI, "
		"Gatchina, LO, 188300 Russia" <<
		std::endl;
	std::cout << "Author: Kirill Pshenichnyi <pshcyrill@mail.ru>" <<
		std::endl;

	/* default options */
	int size = 64;
	bool default_size {true};

	int iteration = 3;
	bool default_intteration {true};

	std::string output_file_name = "out.raw";
	bool default_output_file_name {true};

	std::string str_fractal;
	int fractal {0};

	int opt;
	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"size", required_argument, 0, 's'},
		{"output", required_argument, 0, 'o'},
		{"itteration", required_argument, 0, 'i'},
		{"fractal", required_argument, 0, 'f'},
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
			if(str_fractal == "davinci") {
				fractal = Fractals::DAVINCI;
			}
			break;

		case '?':

			break;

		}
	}

	switch(fractal){
	case Fractals::DAVINCI:
		generateDaVinci(default_size,
			       size,
			       default_intteration,
			       iteration,
			       output_file_name);
		break;
	}

	if(str_fractal == "") {
		help(argv[0]);
		std::cout << "Please set option --fractal!" << std::endl;
		exit(0);
	}

	return 0;
}
