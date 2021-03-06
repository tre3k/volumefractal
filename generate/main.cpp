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
	std::cout << "generate 3D fractals, v0.9.1" << std::endl;
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
	std::cout << "\t-f, --fractal=<fractal>\r\t\t\t\t\t"
		"Selection of a fractal, such as:" << std::endl <<
		"\r\t\t\t\t\t" << std::to_string(Fractals::PINHOLL) <<
		". \"pinholl\" - Just one pixel in center" <<
		std::endl <<
		"\r\t\t\t\t\t" << std::to_string(Fractals::SPHERA) <<
		". \"sphera\" - Draw sphera" <<
		std::endl <<
		"\r\t\t\t\t\t" << std::to_string(Fractals::DAVINCI) <<
		". \"davinci\" - Da Vinci 3D tree" <<
		std::endl <<
		"\r\t\t\t\t\t" << std::to_string(Fractals::SDAVINCI) <<
		". \"sdavinci\" - Spherical tree Da Vinci 3D" <<
		std::endl <<
		"\r\t\t\t\t\t" << std::to_string(Fractals::VISHEK) <<
		". \"vishek\" - Vishek fractal" <<
		std::endl <<
		"\r\t\t\t\t\t" << std::to_string(Fractals::VMULD) <<
		". \"vmuld\" - Vishek multiplication with Davinchi3D" <<
		std::endl;


	std::cout << "\t--noconfirm\r\t\t\t\t\t" <<
		"Do not ask for any confirmation" << std::endl;
	std::cout << "\t--minimum-size=<size>\r\t\t\t\t\t" <<
		"Size of minimum element (for \"davinci\" fractal)" <<
		std::endl <<
		"\r\t\t\t\t\tDiameter for \"sphere\"" <<
		std::endl;
	std::cout << "\t--speed=<value>\r\t\t\t\t\t" <<
		"the rate of growth of the fractal, the rule of increase" <<
		std::endl <<
		"\r\t\t\t\t\twith each iteration step" <<
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

	std::string output_file_name = "";
	bool default_output_file_name {true};

	std::string str_fractal;
	int fractal {0};
	int speed {0};

	int minimum_size_element {10};

	FFT3D::Data *data;
	Fractals::DaVinci3D *davinci3d;
	Fractals::SDaVinci3D *sdavinci3d;
	Fractals::Vishek3D *vishek3d;
	Fractals::VmulD3D *vmuld3d;

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
		{"iteration", required_argument, 0, 'i'},
		{"fractal", required_argument, 0, 'f'},
		{"noconfirm", no_argument, &no_confirm_flag, 1},
		{"minimum-size", required_argument, 0, 0},
		{"speed", required_argument, 0, 0},
		{0, 0 ,0 ,0}
	};

	int option_index {0};

	if(argc < 2) {
		help(argv[0]);
		exit(0);
	}

	while(1) {
		opt = getopt_long(argc, argv, "vhs:o:i:f:",
				  long_options, &option_index);

		if(opt < 0) break;
		switch(opt) {
		case 0:
			if(std::string(long_options[option_index].name) ==
			   std::string("minimum-size")) {
				minimum_size_element = std::atoi(optarg);
			}
			if(std::string(long_options[option_index].name) ==
			   std::string("speed")) {
				speed = std::atoi(optarg);
			}

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
			if(str_fractal == "davinci" ||
			   str_fractal == std::to_string(Fractals::DAVINCI)) {
				str_fractal = "davinci";
				fractal = Fractals::DAVINCI;
			}

			if(str_fractal == "pinholl" ||
			   str_fractal == std::to_string(Fractals::PINHOLL)){
				str_fractal = "pinholl";
				fractal = Fractals::PINHOLL;
			}

			if(str_fractal == "sphera" ||
			   str_fractal == std::to_string(Fractals::SPHERA)) {
				str_fractal = "sphera";
				fractal = Fractals::SPHERA;
			}

			if(str_fractal == "sdavinci" ||
			   str_fractal == std::to_string(Fractals::SDAVINCI)) {
				str_fractal = "sdavinci";
				fractal = Fractals::SDAVINCI;
			}


			// ?????????????? ?????????????? Int->String ?? ???????????????????? ????
			// ?????????? ?????? ?????? ????????????????
			if(str_fractal == "vishek" ||
			   str_fractal == std::to_string(Fractals::VISHEK)) {
				str_fractal = "vishek";
				fractal = Fractals::VISHEK;
			}

			if(str_fractal == "vmuld" ||
			   str_fractal == std::to_string(Fractals::VMULD)) {
				str_fractal = "vmuld";
				fractal = Fractals::VMULD;
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
						minimum_size_element/2);
		sphera->paint();
		break;

	case Fractals::PINHOLL:
		confirm(no_confirm_flag, size);
		data = new FFT3D::Data(size);
		pinholl = new Primitives::Pinholl(data, center);
		pinholl->paint();
		break;

	case Fractals::SDAVINCI:
		std::cout << "Spherical tree Da Vinci 3D!" << std::endl;
		confirm(no_confirm_flag, size);

		data = new FFT3D::Data(size);
		sdavinci3d = new Fractals::SDaVinci3D(data, iteration);
		sdavinci3d->setMinimumSizeElement(minimum_size_element/2);
		if(speed != 0) sdavinci3d->setSpeed(speed);
		if(default_intteration && !default_size)
			sdavinci3d->setMaximumIteration();
		iteration = sdavinci3d->getIteration();
		std::cout << "Minimum sphere radius: " <<
			int(minimum_size_element/2) <<
			" pix " <<
			"(diamenter: " <<
			int(minimum_size_element) <<
			" pix)" << std::endl;
		std::cout << "Start generate the spherical tree" <<
			"of da Vinci: " <<
			iteration << " steps iteration" <<
			std::endl;
		sdavinci3d->setPosition(
			{data->size_x()/2,
			 data->size_y()/2,
			 data->size_z()/2}
			);

		sdavinci3d->generate();
		break;

	case Fractals::DAVINCI:
		std::cout << "Da Vinci 3D tree!" << std::endl;
		confirm(no_confirm_flag, size);

		data = new FFT3D::Data(size);
		davinci3d = new Fractals::DaVinci3D(data, iteration);
		davinci3d->setMinimumSizeElement(minimum_size_element);
		if(speed != 0) davinci3d->setSpeed(speed);
		if(default_intteration && !default_size)
			davinci3d->setMaximumIteration();
		iteration = davinci3d->getIteration();
		std::cout << "Minims element size: " << minimum_size_element <<
			" pix" << std::endl;
		std::cout << "Start generate the tree of da Vinci: " <<
			iteration << " steps iteration" <<
			std::endl;
		davinci3d->setPosition(
			{data->size_x()/2 -
			 minimum_size_element *
			 davinci3d->SizeElement(iteration)/2,
			 data->size_y()/2 -
			 minimum_size_element *
			 davinci3d->SizeElement(iteration)/2,
			 data->size_z()/2 -
			 minimum_size_element *
			 davinci3d->SizeElement(iteration)/2});
		davinci3d->generate();
		break;

	case Fractals::VMULD:
		std::cout << "generate Vishek multiplication DaVinci"
			  << std::endl;
		confirm(no_confirm_flag, size);

		data = new FFT3D::Data(size);
		vmuld3d = new Fractals::VmulD3D(data, iteration);
		vmuld3d->setMinimumSizeElement(minimum_size_element);
		if(speed != 0) vmuld3d->setSpeed(speed);
		if(default_intteration && !default_size)
			vmuld3d->setMaximumIteration();
		iteration = vmuld3d->getIteration();
		std::cout << "Minims element size: " << minimum_size_element <<
			" pix" << std::endl;
		std::cout << "Start generate Vishek and da Vinci: " <<
			iteration << " steps iteration" <<
			std::endl;
		vmuld3d->setPosition(
			{data->size_x()/2 -
			 minimum_size_element *
			 vmuld3d->SizeElement(iteration)/2,
			 data->size_y()/2 -
			 minimum_size_element *
			 vmuld3d->SizeElement(iteration)/2,
			 data->size_z()/2 -
			 minimum_size_element *
			 vmuld3d->SizeElement(iteration)/2});
		vmuld3d->generate();

		break;

	case Fractals::VISHEK:
		data = new FFT3D::Data(size);
		vishek3d = new Fractals::Vishek3D(data, iteration);
		int ps = minimum_size_element *
			Fractals::Vishek3D::permitedSizes[iteration];
		vishek3d->setSize(ps);
		vishek3d->setPosition({
				(int)((size - ps) / 2),
				(int)((size - ps) / 2),
				(int)((size - ps) / 2)
			});
		vishek3d->generate();
		break;

	}

	std::cout << "write to output file: " <<
		output_file_name <<
		" (" << FFT3D::Data::human_size(data->FileSize()) << ")..." <<
		std::endl;
	if(default_output_file_name) {
		output_file_name = str_fractal + "-" +
			std::to_string(iteration) + "-" +
			std::to_string(size) + ".raw";
	}
	data->WriteToRawFile(output_file_name);
	std::cout << "done." << std::endl;

	return 0;
}
