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


#include "options.h"

using namespace CmdLine;

Options::Options(int argc, char **argv) {
	for(int i=0;i<argc;i++){
		_argv.push_back(argv[i]);
	}
}

void Options::parse(std::string opt, unsigned int type, void *val) {
	bool *t_bool;
	int *t_int;
	double *t_double;
	float *t_float;
	std::string *t_string;

	int index = 0;

	for(auto o : _argv){
		if(o==opt){
			switch (type) {
			case T_BOOL:
				t_bool = (bool *)val;
				*t_bool = true;
				break;

			case T_INT:
				t_int = (int *)val;
				if(index+1>=_argv.size()){
					t_int = 0;
					break;
				}
				*t_int = std::stoi(_argv.at(index+1));
				break;

			case T_DOUBLE:
				t_double = (double *)val;
				*t_double = std::stod(_argv.at(index+1));
				break;

			case T_FLOAT:
				t_float = (float *)val;
				*t_float = std::stof(_argv.at(index+1));
				break;

			case T_STRING:
				t_string = (std::string *)val;
				*t_string = _argv.at(index+1);
				break;
			}
		}
		index ++;
	}
}
