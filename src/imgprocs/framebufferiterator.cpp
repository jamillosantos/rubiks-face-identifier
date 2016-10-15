/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include "framebufferiterator.h"

mote::imgprocs::FrameBufferIterator::FrameBufferIterator(unsigned int bytesPerPixel, uint8_t *begin, unsigned int width,
	unsigned int height, unsigned int subsample)
	: _bytesPerPixel(bytesPerPixel), _bytesPerLine(width * _bytesPerPixel), _cursor(-1, 0), _begin(begin),
	_width(width), _height(height), _subsample(subsample)
{}

const cv::Point2i& mote::imgprocs::FrameBufferIterator::cursor() const
{
	return this->_cursor;
}

bool mote::imgprocs::FrameBufferIterator::inBounds(const unsigned int x, const unsigned int y)
{
	return (x >= 0) && (x < this->_width) && (y >= 0) && (y < this->_height);
}

bool mote::imgprocs::FrameBufferIterator::goNext()
{
	this->_cursor.x += this->_subsample;
	if (this->_cursor.x >= this->_width)
	{
		this->_cursor.x = 0;
		this->_cursor.y += this->_subsample;
	}
	return (this->_cursor.y < this->_height);
}

bool mote::imgprocs::FrameBufferIterator::go(const unsigned int x, const unsigned int y)
{
	if (this->inBounds(x, y))
	{
		this->_cursor.x = x;
		this->_cursor.y = y;
		return true;
	}
	return false;
}

bool mote::imgprocs::FrameBufferIterator::goLeft(const unsigned int amount)
{
	return this->go(this->_cursor.x + this->_subsample*amount, this->_cursor.y);
}

bool mote::imgprocs::FrameBufferIterator::goRight(const unsigned int amount)
{
	return this->go(this->_cursor.x - this->_subsample*amount, this->_cursor.y);
}

bool mote::imgprocs::FrameBufferIterator::goUp(const unsigned int amount)
{
	return this->go(this->_cursor.x, this->_cursor.y - this->_subsample*amount);
}

bool mote::imgprocs::FrameBufferIterator::goDown(const unsigned int amount)
{
	return this->go(this->_cursor.x, this->_cursor.y + this->_subsample*amount);
}

bool mote::imgprocs::FrameBufferIterator::get(mote::data::Pixel *pixel)
{
	this->getPixel(pixel, this->_cursor.x, this->_cursor.y);
}

bool mote::imgprocs::FrameBufferIterator::get(mote::data::Pixel *pixel, const int offsetX, const int offsetY)
{
	return this->getPixel(pixel, this->_cursor.x + offsetX, this->_cursor.y + offsetY);
}

bool mote::imgprocs::FrameBufferIterator::get(mote::data::Pixel &pixel)
{
	return this->getPixel(pixel, this->_cursor.x, this->_cursor.y);
}

bool mote::imgprocs::FrameBufferIterator::get(mote::data::Pixel &pixel, const int offsetX, const int offsetY)
{
	return this->getPixel(pixel, this->_cursor.x + offsetX, this->_cursor.y + offsetY);
}

mote::imgprocs::FramBufferIteratorRGB24::FramBufferIteratorRGB24(uint8_t *begin,
	unsigned int width, unsigned int height, unsigned int subsample)
	: FrameBufferIterator(sizeof(mote::data::Pixel), begin, width, height, subsample)
{ }

uint8_t *mote::imgprocs::FramBufferIteratorRGB24::getAddress(const unsigned int x, const unsigned int y)
{
	if (this->inBounds(x, y))
		return this->_begin + (this->_bytesPerLine * y) + (this->_bytesPerPixel * x);
	return nullptr;
}

bool mote::imgprocs::FramBufferIteratorRGB24::getPixel(mote::data::Pixel *px, const unsigned int x, const unsigned int y)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		px->b = *p++;
		px->g = *p++;
		px->r = *p;
		return true;
	}
	return false;
}

bool mote::imgprocs::FramBufferIteratorRGB24::getPixel(mote::data::Pixel &px, const unsigned int x, const unsigned int y)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		px.b = *p++;
		px.g = *p++;
		px.r = *p;
		return true;
	}
	return false;
}

bool mote::imgprocs::FramBufferIteratorRGB24::setPixel(const unsigned int x, const unsigned int y,
	mote::data::Pixel *px)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		*p++ = px->b;
		*p++ = px->g;
		*p = px->r;
		return true;
	}
	return false;
}

bool mote::imgprocs::FramBufferIteratorRGB24::setPixel(const unsigned int x, const unsigned int y,
	const mote::data::Pixel &px)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		*p++ = px.b;
		*p++ = px.g;
		*p = px.r;
		return true;
	}
	return false;
}

bool mote::imgprocs::FramBufferIteratorRGB24::setPixel(mote::data::Pixel *px)
{
	return this->setPixel(this->_cursor.x, this->_cursor.y, px);
}

bool mote::imgprocs::FramBufferIteratorRGB24::setPixel(const mote::data::Pixel &px)
{
	return this->setPixel(this->_cursor.x, this->_cursor.y, px);
}
