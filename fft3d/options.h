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


#ifndef FFT3D_OPTIONS_H
#define FFT3D_OPTIONS_H

#include <vector>
#include <string>

namespace CmdLine {
	enum Type{
		T_BOOL,
		T_INT,
		T_DOUBLE,
		T_FLOAT,
		T_STRING
	};

	class Options {
	private:
		std::vector<std::string> _argv;

	public:
		Options(int argc,char **argv);
		void parse(std::string opt,unsigned int type, void *val);

	};
}


#endif //FFT3D_OPTIONS_H
