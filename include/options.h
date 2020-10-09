//
// Created by kirill on 09.10.2020.
//

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
