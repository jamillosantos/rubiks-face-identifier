/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 26, 2016
 */

#include "floodfillstate.h"

mote::data::FloodFillState::FloodFillState()
	: _size(0), _sumX(0), _sumY(0), _bBox(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), 0, 0),
	  _sumRed(0), _sumGreen(0), _sumBlue(0)
{}

void mote::data::FloodFillState::addPoint(const unsigned int x, const unsigned int y, const mote::data::Pixel &pixel)
{
	if ((signed)x < this->_bBox.x)
	{
		if (this->bBox().x == std::numeric_limits<int>::max())
			this->_bBox.x = x;
		else
			this->_bBox.l(x);
	}

	if ((signed)y < this->_bBox.y)
	{
		if (this->bBox().y == std::numeric_limits<int>::max())
			this->_bBox.y = y;
		else
			this->_bBox.t(y);
	}

	if ((signed)x > this->_bBox.r())
		this->_bBox.r(x);

	if ((signed)y > this->_bBox.b())
		this->_bBox.b(y);

	this->_sumRed = this->_sumRed + pixel.r;
	this->_sumGreen = this->_sumGreen + pixel.g;
	this->_sumBlue = this->_sumBlue + pixel.b;

	++this->_size;
	this->_sumX = this->_sumX + x;
	this->_sumY = this->_sumY + y;
}

void mote::data::FloodFillState::addPoint(const cv::Point2f &p, const mote::data::Pixel &pixel)
{
	this->addPoint(p.x, p.y, pixel);
}

mote::data::Rect<int>& mote::data::FloodFillState::bBox()
{
	return this->_bBox;
}

std::size_t mote::data::FloodFillState::size() const
{
	return this->_size;
}

mote::data::FloodFillState &mote::data::FloodFillState::size(std::size_t size)
{
	this->_size = size;
	return *this;
}

unsigned int mote::data::FloodFillState::sumX() const
{
	return this->_sumX;
}

mote::data::FloodFillState &mote::data::FloodFillState::sumX(unsigned int sumX)
{
	this->_sumX = sumX;
	return *this;
}

unsigned int mote::data::FloodFillState::sumY() const
{
	return this->_sumY;
}

mote::data::FloodFillState &mote::data::FloodFillState::sumY(unsigned int sumY)
{
	this->_sumY = sumY;
	return *this;
}

int mote::data::FloodFillState::x() const
{
	int result = -1;
	if (this->_size > 0)
		result = this->_sumX / this->_size;
	return result;
}

int mote::data::FloodFillState::y() const
{
	int result = -1;
	if (this->_size > 0)
		result = this->_sumY / this->_size;
	return result;
}

mote::data::Pixel mote::data::FloodFillState::averageColour() const
{
	unsigned int red;
	unsigned int green;
	unsigned int blue;

	if (this->_size > 0)
	{
		red = this->_sumRed / this->_size;
		green = this->_sumGreen / this->_size;
		blue = this->_sumBlue / this->_size;
	}
	else
	{
		red = 0;
		green = 0;
		blue = 0;
	}

	return mote::data::Pixel(boost::algorithm::clamp(red, 0, 255), boost::algorithm::clamp(green, 0, 255),
		boost::algorithm::clamp(blue, 0, 255));
}

void mote::data::FloodFillState::clear()
{
	this->_size = this->_sumX = this->_sumY = 0;

	this->_bBox.x = this->_bBox.y = std::numeric_limits<int>::max();
	this->_bBox.width = this->_bBox.height = 0;

	this->_sumRed = this->_sumGreen = this->_sumBlue = 0;
}
