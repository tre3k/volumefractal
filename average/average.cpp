#include "average.h"

Average::Average(FFT3D::Data *data) {
	setData(data);
}

void Average::setData(FFT3D::Data *data) {
	_data = data;
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
