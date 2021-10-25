#ifndef AVERAGE_H
#define AVERAGE_H

#include <fft3ddata.h>


class Average {
private:
	FFT3D::Data *_data {nullptr};

public:
	Average(FFT3D::Data *data = nullptr);
	void setData(FFT3D::Data *data);
	/* search mass of center for intencity abs*abs */
	FFT3D::acoord findCenterMass(void);
};

#endif
