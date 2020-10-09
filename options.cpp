//
// Created by kirill on 09.10.2020.
//

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