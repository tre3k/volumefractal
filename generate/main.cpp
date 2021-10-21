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


void help(char *progname) {

	std::cout << "Usage: " << progname << " [options]" << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\t-h, --help\r\t\t\t\tThis message" << std::endl;

	std::cout << "\t-s, --size=<size>" << std::endl;

	std::cout << std::endl;

}


int main(int argc, char *argv[]) {

	std::cout <<
		"GPLv3 (c) Copyright (c) 2020-2021 NRC KI PNPI, "
		"Gatchina, LO, 188300 Russia" <<
		std::endl;
	std::cout << "Author: Kirill Pshenichnyi <pshcyrill@mail.ru>" <<
		std::endl;

	int size = 64;


	int opt;
	static struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"size", required_argument, 0, 's'},
		{0, 0 ,0 ,0}
	};

	int option_index {0};

	if(argc < 2) {
		help(argv[0]);
		exit(0);
	}

	while(1) {
		opt = getopt_long(argc, argv, "hs:",
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

		case '?':

			break;

		}
	}

	std::cout << "size: " << size << "x" <<
		size << "x" << size << std::endl;


	return 0;
}
