/**
 * @author J. Santos <jamillo@gmail.com>
 * @date July 25, 2016
 */

#include <limits>
#include "colour_definition.h"

mote::data::ColourRange::ColourRange(const uint8_t r, const uint8_t g, const uint8_t b, const int redGreen,
	const int redBlue, const int greenBlue)
	: RGBColour(r, g, b), redGreen(redGreen), redBlue(redBlue), greenBlue(greenBlue)
{ }

mote::data::ColourRange::ColourRange(uint8_t r, uint8_t g, uint8_t b)
	: RGBColour(r, g, b)
{
	this->update();
}

mote::data::ColourRange::ColourRange(const mote::data::Pixel &pixel)
	: RGBColour(pixel.r, pixel.g, pixel.b)
{ }

mote::data::ColourRange::ColourRange(const mote::data::ColourRange &colour)
	: ColourRange(colour.pixel()->r, colour.pixel()->g, colour.pixel()->b, colour.redGreen, colour.redBlue,
	  colour.greenBlue)
{ }

mote::data::ColourRange::ColourRange(const mote::data::RGBColour &colour)
	: ColourRange(colour.pixel()->r, colour.pixel()->g, colour.pixel()->b)
{ }

mote::data::ColourRange::ColourRange()
	: ColourRange(0, 0, 0)
{ }

void mote::data::ColourRange::update()
{
	this->redGreen = this->pixel()->r - this->pixel()->g;
	this->redBlue = this->pixel()->r - this->pixel()->b;
	this->greenBlue = this->pixel()->g - this->pixel()->b;
}

mote::data::RGBColour &mote::data::ColourRange::minimum(const mote::data::RGBColour &colour)
{
	RGBColour::minimum(colour);
	const ColourRange *cr = dynamic_cast<const ColourRange*>(&colour);
	if (cr)
	{
		if (cr->redGreen < this->redGreen)
			this->redGreen = cr->redGreen;
		if (cr->redBlue < this->redBlue)
			this->redBlue = cr->redBlue;
		if (cr->greenBlue < this->greenBlue)
			this->greenBlue = cr->greenBlue;
	}
	return *this;
}

mote::data::RGBColour &mote::data::ColourRange::maximum(const mote::data::RGBColour &colour)
{
	RGBColour::maximum(colour);
	const ColourRange *cr = dynamic_cast<const ColourRange*>(&colour);
	if (cr)
	{
		if (cr->redGreen > this->redGreen)
			this->redGreen = cr->redGreen;
		if (cr->redBlue > this->redBlue)
			this->redBlue = cr->redBlue;
		if (cr->greenBlue > this->greenBlue)
			this->greenBlue = cr->greenBlue;
	}
	return *this;
}

mote::data::ColourRange mote::data::ColourRange::operator+(const mote::data::ColourRange &colour)
{
	return mote::data::ColourRange(this->pixel()->r + colour.pixel()->r, this->pixel()->g + colour.pixel()->g, this->pixel()->b + colour.pixel()->b);
}

mote::data::ColourRange mote::data::ColourRange::operator-(const mote::data::ColourRange &colour)
{
	return mote::data::ColourRange(this->pixel()->r - colour.pixel()->r, this->pixel()->g - colour.pixel()->g, this->pixel()->b - colour.pixel()->b);
}

mote::data::ColourRange mote::data::ColourRange::operator*(const mote::data::ColourRange &colour)
{
	return mote::data::ColourRange(this->pixel()->r * colour.pixel()->r, this->pixel()->g * colour.pixel()->g, this->pixel()->b * colour.pixel()->b);
}

mote::data::ColourRange mote::data::ColourRange::operator/(const mote::data::ColourRange &colour)
{
	return mote::data::ColourRange(this->pixel()->r / colour.pixel()->r, this->pixel()->g / colour.pixel()->g, this->pixel()->b / colour.pixel()->b);
}

mote::data::ColourRange &mote::data::ColourRange::operator+=(const mote::data::ColourRange &colour)
{
	this->pixel()->r += colour.pixel()->r;
	this->pixel()->g += colour.pixel()->g;
	this->pixel()->b += colour.pixel()->b;
	this->update();
	return *this;
}

mote::data::ColourRange &mote::data::ColourRange::operator-=(const mote::data::ColourRange &colour)
{
	this->pixel()->r -= colour.pixel()->r;
	this->pixel()->g -= colour.pixel()->g;
	this->pixel()->b -= colour.pixel()->b;
	this->update();
	return *this;
}

mote::data::ColourRange &mote::data::ColourRange::operator*=(const mote::data::ColourRange &colour)
{
	this->pixel()->r *= colour.pixel()->r;
	this->pixel()->g *= colour.pixel()->g;
	this->pixel()->b *= colour.pixel()->b;
	this->update();
	return *this;
}

mote::data::ColourRange &mote::data::ColourRange::operator/=(const mote::data::ColourRange &colour)
{
	this->pixel()->r /= colour.pixel()->r;
	this->pixel()->g /= colour.pixel()->g;
	this->pixel()->b /= colour.pixel()->b;
	this->update();
	return *this;
}

void mote::data::ColourRange::toJson(Json::Value &json) const
{
	RGBColour::toJson(json);
	json["redGreen"] = this->redGreen;
	json["redBlue"] = this->redBlue;
	json["greenBlue"] = this->greenBlue;
}

void mote::data::ColourRange::fromJson(const Json::Value &json)
{
	RGBColour::fromJson(json);
	this->update();
	if (json["redGreen"].isUInt())
		this->redGreen = json["redGreen"].asUInt();
	if (json["redBlue"].isUInt())
		this->redBlue = json["redBlue"].asUInt();
	if (json["greenBlue"].isUInt())
		this->greenBlue = json["greenBlue"].asUInt();
}


mote::data::ColourDefinition::ColourDefinition()
	: min(std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max(),
	  std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max()), max(
	  std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::min(),
	  std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), std::numeric_limits<int>::min())
{ }

mote::data::ColourDefinition::ColourDefinition(const mote::data::ColourDefinition &definition)
	: min(definition.min), max(definition.max)
{ }

mote::data::ColourDefinition::ColourDefinition(const mote::data::ColourRange &min, const mote::data::ColourRange &max)
	: min(min), max(max)
{ }

mote::data::ColourDefinition::ColourDefinition(const mote::data::ColourRange&& min, const mote::data::ColourRange&& max)
	: min(min), max(max)
{ }

bool mote::data::ColourDefinition::operator==(const mote::data::ColourDefinition &colourDefinition) const
{
	return
		(this->min == colourDefinition.min) && (this->max == colourDefinition.max);
}

bool mote::data::ColourDefinition::operator!=(const mote::data::ColourDefinition &colourDefinition) const
{
	return !(colourDefinition == *this);
}

bool mote::data::ColourDefinition::isMatch(const mote::data::Pixel &p) const
{
	return
		(p.r >= this->min.pixel()->r)
		&& (p.g >= this->min.pixel()->g)
		&& (p.b >= this->min.pixel()->b)
		&& ((p.r - (int)p.g) >= this->min.redGreen)
		&& ((p.r - (int)p.b) >= this->min.redBlue)
		&& ((p.g - (int)p.b) >= this->min.greenBlue)

		&& (p.r <= this->max.pixel()->r)
		&& (p.g <= this->max.pixel()->g)
		&& (p.b <= this->max.pixel()->b)
		&& ((p.r - (int)p.g) <= this->max.redGreen)
		&& ((p.r - (int)p.b) <= this->max.redBlue)
		&& ((p.g - (int)p.b) <= this->max.greenBlue);
}

bool mote::data::ColourDefinition::isMatch(const mote::data::RGBColour &colour) const
{
	return this->isMatch(*colour.pixel());
}

void mote::data::ColourDefinition::addPixel(const mote::data::Pixel& pixel)
{
	mote::data::Pixel p(pixel);
	mote::data::ColourRange colour(&p);
	this->min.minimum(colour);
	this->max.maximum(colour);
}

void mote::data::ColourDefinition::addPixel(const mote::data::RGBColour &colour)
{
	this->min.minimum(colour);
	this->max.maximum(colour);
}

void mote::data::ColourDefinition::fromJson(const Json::Value &json)
{
	this->min.fromJson(json["min"]);
	this->max.fromJson(json["max"]);
}

void mote::data::ColourDefinition::toJson(Json::Value &json) const
{
	this->min.toJson(json["min"]);
	this->max.toJson(json["max"]);
}
