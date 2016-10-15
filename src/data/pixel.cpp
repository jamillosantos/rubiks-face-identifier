/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include "pixel.h"

mote::data::Pixel::Pixel()
{ }

mote::data::Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b)
	: b(b), g(g), r(r)
{ }


mote::data::Pixel::Pixel(const mote::data::Pixel &px)
	: b(px.b), g(px.g), r(px.r)
{ }

bool mote::data::Pixel::is(uint8_t r, uint8_t g, uint8_t b)
{
	return
		(this->r == r)
		&& (this->g == g)
		&& (this->b == b);
}

double mote::data::Pixel::intensity() const
{
	return (this->r + this->g + this->b) / 3.0;
}

double mote::data::Pixel::diffIntensity(const mote::data::Pixel &p) const
{
	double i1 = this->intensity();
	double i2 = p.intensity();
	double diff = i1 - i2;
	return diff * diff;
}
