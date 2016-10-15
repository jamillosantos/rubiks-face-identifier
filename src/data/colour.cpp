/**
 * @author J. Santos <jamillo@gmail.com>
 * @date July 24, 2016
 */

#include "colour.h"

mote::data::RGBColour mote::data::RGBColour::red(255, 0, 0);
mote::data::RGBColour mote::data::RGBColour::green(0, 255, 0);
mote::data::RGBColour mote::data::RGBColour::blue(0, 0, 255);
mote::data::RGBColour mote::data::RGBColour::white(255, 255, 255);
mote::data::RGBColour mote::data::RGBColour::black(0, 0, 0);
mote::data::RGBColour mote::data::RGBColour::yellow(255, 255, 0);
mote::data::RGBColour mote::data::RGBColour::cyan(0, 255, 255);
mote::data::RGBColour mote::data::RGBColour::magenta(255, 0, 255);

mote::data::RGBColour::RGBColour(uint8_t r, uint8_t g, uint8_t b)
	: _pixel(new mote::data::Pixel(r, g, b)), _ownsPixel(true)
{ }

mote::data::RGBColour::RGBColour(mote::data::Pixel *pixel)
	: _pixel(pixel), _ownsPixel(false)
{ }

mote::data::RGBColour::RGBColour(const mote::data::RGBColour &colour)
	: mote::data::RGBColour::RGBColour(colour.pixel()->r, colour.pixel()->g, colour.pixel()->b)
{ }

mote::data::RGBColour::RGBColour()
	: mote::data::RGBColour::RGBColour(0, 0, 0)
{ }

mote::data::RGBColour::~RGBColour()
{
	if (this->_ownsPixel && this->_pixel)
		delete this->_pixel;
}

mote::data::Pixel* mote::data::RGBColour::pixel()
{
	return this->_pixel;
}

mote::data::Pixel* mote::data::RGBColour::pixel() const
{
	return this->_pixel;
}

mote::data::RGBColour &mote::data::RGBColour::pixel(mote::data::Pixel *pixel, bool own)
{
	if (this->_ownsPixel && (this->_ownsPixel))
		delete this->_pixel;

	this->_pixel = pixel;
	this->_ownsPixel = own;

	return *this;
}

double mote::data::RGBColour::intensity()
{
	return (this->pixel()->r + this->pixel()->g + this->pixel()->b) / 3.0;
}

mote::data::RGBColour &mote::data::RGBColour::minimum(const mote::data::RGBColour &colour)
{
	if (colour.pixel()->r < this->pixel()->r)
		this->pixel()->r = colour.pixel()->r;
	if (colour.pixel()->g < this->pixel()->g)
		this->pixel()->g = colour.pixel()->g;
	if (colour.pixel()->b < this->pixel()->b)
		this->pixel()->b = colour.pixel()->b;
	return *this;
}

mote::data::RGBColour &mote::data::RGBColour::maximum(const mote::data::RGBColour &colour)
{
	if (colour.pixel()->r > this->pixel()->r)
		this->pixel()->r = colour.pixel()->r;
	if (colour.pixel()->g > this->pixel()->g)
		this->pixel()->g = colour.pixel()->g;
	if (colour.pixel()->b > this->pixel()->b)
		this->pixel()->b = colour.pixel()->b;
	return *this;
}

mote::data::RGBColour &mote::data::RGBColour::operator=(const mote::data::RGBColour &colour)
{
	this->pixel()->r = colour.pixel()->r;
	this->pixel()->g = colour.pixel()->g;
	this->pixel()->b = colour.pixel()->b;
	return *this;
}

bool mote::data::RGBColour::operator==(const mote::data::RGBColour &colour) const
{
	return
		(colour.pixel()->r == this->pixel()->r) &&
		(colour.pixel()->g == this->pixel()->g) &&
		(colour.pixel()->b == this->pixel()->b);
}

bool mote::data::RGBColour::operator!=(const mote::data::RGBColour &colour) const
{
	return
		(colour.pixel()->r != this->pixel()->r) ||
		(colour.pixel()->g != this->pixel()->g) ||
		(colour.pixel()->b != this->pixel()->b);
}

mote::data::RGBColour mote::data::RGBColour::operator+(const mote::data::RGBColour &colour)
{
	return mote::data::RGBColour(this->pixel()->r + colour.pixel()->r, this->pixel()->g + colour.pixel()->g, this->pixel()->b + colour.pixel()->b);
}

mote::data::RGBColour mote::data::RGBColour::operator-(const mote::data::RGBColour &colour)
{
	return mote::data::RGBColour(this->pixel()->r - colour.pixel()->r, this->pixel()->g - colour.pixel()->g, this->pixel()->b - colour.pixel()->b);
}

mote::data::RGBColour mote::data::RGBColour::operator*(const mote::data::RGBColour &colour)
{
	return mote::data::RGBColour(this->pixel()->r * colour.pixel()->r, this->pixel()->g * colour.pixel()->g, this->pixel()->b * colour.pixel()->b);
}

mote::data::RGBColour mote::data::RGBColour::operator/(const mote::data::RGBColour &colour)
{
	return mote::data::RGBColour(this->pixel()->r / colour.pixel()->r, this->pixel()->g / colour.pixel()->g, this->pixel()->b / colour.pixel()->b);
}

mote::data::RGBColour &mote::data::RGBColour::operator+=(const mote::data::RGBColour &colour)
{
	this->pixel()->r += colour.pixel()->r;
	this->pixel()->g += colour.pixel()->g;
	this->pixel()->b += colour.pixel()->b;
	return *this;
}

mote::data::RGBColour &mote::data::RGBColour::operator-=(const mote::data::RGBColour &colour)
{
	this->pixel()->r -= colour.pixel()->r;
	this->pixel()->g -= colour.pixel()->g;
	this->pixel()->b -= colour.pixel()->b;
	return *this;
}

mote::data::RGBColour &mote::data::RGBColour::operator*=(const mote::data::RGBColour &colour)
{
	this->pixel()->r *= colour.pixel()->r;
	this->pixel()->g *= colour.pixel()->g;
	this->pixel()->b *= colour.pixel()->b;
	return *this;
}

mote::data::RGBColour &mote::data::RGBColour::operator/=(const mote::data::RGBColour &colour)
{
	this->pixel()->r /= colour.pixel()->r;
	this->pixel()->g /= colour.pixel()->g;
	this->pixel()->b /= colour.pixel()->b;
	return *this;
}

void mote::data::RGBColour::fromJson(const Json::Value &json)
{
	this->pixel()->r = json["r"].asUInt();
	this->pixel()->g = json["g"].asUInt();
	this->pixel()->b = json["b"].asUInt();
}

void mote::data::RGBColour::toJson(Json::Value &json) const
{
	json["r"] = this->pixel()->r;
	json["g"] = this->pixel()->g;
	json["b"] = this->pixel()->b;
}

bool mote::data::RGBColour::ownsPixel() const
{
	return this->_ownsPixel;
}
