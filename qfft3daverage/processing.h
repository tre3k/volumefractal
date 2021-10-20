#ifndef PROCESSING_H
#define PROCESSING_H

#include <iostream>
#include "fft3ddata.h"


class Processing
{
public:
        Processing();
        static void cutLayerOz(FFT3D::Data2D *result,std::string filename, double angle);
};

#endif // PROCESSING_H
