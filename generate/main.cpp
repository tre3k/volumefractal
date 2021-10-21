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
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     Author: Kirill Pshenichnyi <pshcyrill@mail.ru>
 */


#include <iostream>
#include <getopt.h>
#include <fft3ddata.h>
#include <string>

#include "primitives.h"

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

	std::cout << std::endl;
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
	std::string output_file_name = "out.raw";
	int itteration = 3;

	int opt;
	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"size", required_argument, 0, 's'},
		{"output", required_argument, 0, 'o'},
		{"itteration", required_argument, 0, 'i'},
		{0, 0 ,0 ,0}
	};

	int option_index {0};

	if(argc < 2) {
		help(argv[0]);
		exit(0);
	}

	while(1) {
		opt = getopt_long(argc, argv, "hs:o:i:",
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
			break;

		case 'o':
			output_file_name = std::string(optarg);
			break;

		case 'i':
			itteration = std::atoi(optarg);
			break;

		case '?':

			break;

		}
	}

	FFT3D::Data data(size);

	std::cout << "size: " <<
		size << "x" << size << "x" << size <<
		std::endl;

	Primitives::Cube cube(&data);
	cube.setSize(30);
	cube.setKeyPosition({60, 60, 40});
	cube.paint();

	std::cout << "write to output file: " <<
		output_file_name <<
		" (" << FFT3D::Data::human_size(data.FileSize()) << ")" <<
		std::endl;
	data.WriteToRawFile(output_file_name);
	std::cout << "done." << std::endl;

	return 0;
}
