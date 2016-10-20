/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 26, 2016
 */

#include "floodfillstate.h"

procs::FloodFillState::FloodFillState()
	: _size(0), _sumX(0), _sumY(0), _bBox(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), 0, 0),
	  _sumRed(0), _sumGreen(0), _sumBlue(0)
{}

void procs::FloodFillState::addPoint(const unsigned int x, const unsigned int y, const cv::Vec3b &pixel)
{
	if ((signed)x < this->_bBox.x)
	{
		if (this->bBox().x == std::numeric_limits<int>::max())
			this->_bBox.x = x;
		else
		{
			this->_bBox.width -= (x - this->_bBox.x);
			this->_bBox.x = x;
		}
	}

	if ((signed)y < this->_bBox.y)
	{
		if (this->bBox().y == std::numeric_limits<int>::max())
			this->_bBox.y = y;
		else
		{
			this->_bBox.height -= (y - this->_bBox.y);
			this->_bBox.y = y;
		}
	}

	if ((signed)x > (this->_bBox.x + this->_bBox.width))
	{
		this->_bBox.width = (x - this->_bBox.x);
	}

	if ((signed)y > (this->_bBox.y + this->_bBox.height))
		this->_bBox.height = (y - this->_bBox.y);

	this->_sumRed = this->_sumRed + pixel.val[2];
	this->_sumGreen = this->_sumGreen + pixel.val[1];
	this->_sumBlue = this->_sumBlue + pixel.val[0];

	++this->_size;
	this->_sumX = this->_sumX + x;
	this->_sumY = this->_sumY + y;
}

void procs::FloodFillState::addPoint(const cv::Point2f &p, const cv::Vec3b &pixel)
{
	this->addPoint(p.x, p.y, pixel);
}

cv::Rect2i& procs::FloodFillState::bBox()
{
	return this->_bBox;
}

std::size_t procs::FloodFillState::size() const
{
	return this->_size;
}

procs::FloodFillState &procs::FloodFillState::size(std::size_t size)
{
	this->_size = size;
	return *this;
}

unsigned int procs::FloodFillState::sumX() const
{
	return this->_sumX;
}

procs::FloodFillState &procs::FloodFillState::sumX(unsigned int sumX)
{
	this->_sumX = sumX;
	return *this;
}

unsigned int procs::FloodFillState::sumY() const
{
	return this->_sumY;
}

procs::FloodFillState &procs::FloodFillState::sumY(unsigned int sumY)
{
	this->_sumY = sumY;
	return *this;
}

int procs::FloodFillState::x() const
{
	int result = -1;
	if (this->_size > 0)
		result = this->_sumX / this->_size;
	return result;
}

int procs::FloodFillState::y() const
{
	int result = -1;
	if (this->_size > 0)
		result = this->_sumY / this->_size;
	return result;
}

cv::Vec3b procs::FloodFillState::averageColour() const
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

	return cv::Vec3b(
		boost::algorithm::clamp(blue, 0, 255),
		boost::algorithm::clamp(green, 0, 255),
		boost::algorithm::clamp(red, 0, 255)
	);
}

void procs::FloodFillState::clear()
{
	this->_size = this->_sumX = this->_sumY = 0;

	this->_bBox.x = this->_bBox.y = std::numeric_limits<int>::max();
	this->_bBox.width = this->_bBox.height = 0;

	this->_sumRed = this->_sumGreen = this->_sumBlue = 0;
}
