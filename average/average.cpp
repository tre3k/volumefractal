#include "average.h"

Average::Average(FFT3D::Data *data) {
	setData(data);
}

void Average::setData(FFT3D::Data *data) {
	_data = data;
}

int Average::doubleToInt(double val) {
	return (int)(val+0.5);
}


FFT3D::acoord Average::findCenterMass(){
	FFT3D::acoord retVal;

	double Sm {0.0};
	double Sx {0.0};
	double Sy {0.0};
	double Sz {0.0};
	double mod {0.0};

	auto sx = _data->size_x();
	auto sy = _data->size_y();
	auto sz = _data->size_z();

	for(int i = 0; i < sx; i++) {
		for(int j = 0; j < sy; j++) {
			for(int k = 0; k < sz; k++) {
				mod = abs(_data->getValue(i,j,k));
				Sx += mod*mod*i;
				Sy += mod*mod*j;
				Sz += mod*mod*k;
				Sm += mod*mod;
			}
		}
	}

	retVal.di = Sx/Sm;
	retVal.dj = Sy/Sm;
	retVal.dk = Sz/Sm;
	retVal.double_flag = true;

	return retVal;
}



/* function for azim average (2pi) just only one r (recive one point) */
std::complex<DATA_TYPE> Average::getPointFrom2DAverage(std::string filename,
						       unsigned long num_depth,
						       double r) {
	std::complex<DATA_TYPE> retVal {0.0, 0.0};
	double dr = sqrt(2);
	double dphi = 0.1;


	FFT3D::Data2D data2d(_data->size_x());
	_data->Read2DLayerDepthFromFile(filename,
					&data2d,
					num_depth);

	dphi = atan(1.0/r);


	return retVal;
}

Average::averages Average::average(FFT3D::acoord center){
	std::complex<DATA_TYPE> S {0.0, 0.0};
	averages retVal;


	double x, y, z;
	int ix, iy, iz;



	return retVal;
}
