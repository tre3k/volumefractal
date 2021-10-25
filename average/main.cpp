/*
 *  Copyright (c) 2020-2021 NRC KI PNPI, Gatchina, LO, 188300 Russia
 *
 *  This file is part of volumefractal (average).
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

#include "average.h"

void help(char *prgname) {
	std::cout << "Usage: " << prgname << "[options]" << std::endl;
	std::cout << "Options: " << std::endl;
}

int main(int argc, char *argv[]) {

	std::string out_file {"out.dat"};
	std::string in_file {""};
	int no_confirm_flag {0};

	std::string status;

	int opt;
	static struct option long_options[] = {
		{"version", no_argument, 0, 'v'},
		{"help", no_argument, 0, 'h'},
		{"output", required_argument, 0, 'o'},
		{"input", required_argument, 0, 'i'},
		{"noconfirm", no_argument, &no_confirm_flag, 1},
		{0, 0 ,0 ,0}
	};

	int option_index {0};

	if(argc < 2) {
		help(argv[0]);
		return 0;
	}


	while(1) {
		opt = getopt_long(argc, argv, "hvo:i:",
			     long_options, &option_index);
		if(opt < 0) break;
		switch(opt) {
		case 'h':
			help(argv[0]);
			return 0;
			break;

		case 'i':
			in_file = std::string(optarg);
			break;

		case 'o':
			out_file = std::string(optarg);
			break;
		}

	}

	if(in_file == "") {
		std::cout << "Please set input file!" <<
			" See --help for --input option." <<
			std::endl;
		return 0;
	}

	std::cout << "load file " << in_file << std::endl;
	FFT3D::Data data(0);
	data.ReadOnlyHeader(in_file);
	std::cout << "size: " << data.size_x() << "x" <<
		data.size_y() << "x" << data.size_z() <<
		" (" << FFT3D::Data::human_size(
			sizeof(std::complex<DATA_TYPE>) *
			data.size_x() *
			data.size_y() *
			data.size_z()) <<
		")" << std::endl;

	if(!no_confirm_flag) {
		status = "";
		std::cout << "do you want to continue? ";
		while(status != "y"){
			std::cout << "y/n: ";
			std::cin >> status;
			if(status == "n" || status == "q") return 0;
		}

	}
	std::cout << "load file..." << std::endl;
	data.ReadFromRawFile(in_file);
	std::cout << "ok." << std::endl;

	Average average(&data);
	auto tmp = average.findCenterMass();
	std::cout << tmp.di << " " << tmp.dj << " " << tmp.dk << std::endl;

	return 0;
}
