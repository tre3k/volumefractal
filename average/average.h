#ifndef AVERAGE_H
#define AVERAGE_H

#include <vector>
#include <complex>

#include <fft3ddata.h>

class Average {
private:
	FFT3D::Data *_data {nullptr};
	int doubleToInt(double val);

public:
	Average(FFT3D::Data *data = nullptr);
	void setData(FFT3D::Data *data);
	/* search mass of center for intencity abs*abs */
	FFT3D::acoord findCenterMass(void);

	std::complex<DATA_TYPE> getPointFrom2DAverage(std::string filename,
						      unsigned long num_depth,
						      double r);


	struct s_averages{
		std::vector<std::complex<DATA_TYPE>> value;
		std::vector<double> r;
	} typedef averages;

	averages average(FFT3D::acoord center);
};

#endif
