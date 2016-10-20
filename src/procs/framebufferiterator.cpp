/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include <boost/log/trivial.hpp>
#include "framebufferiterator.h"

procs::FrameBufferIterator::FrameBufferIterator(unsigned int bytesPerPixel, uint8_t *begin, unsigned int width,
	unsigned int height, unsigned int subsample)
	: _bytesPerPixel(bytesPerPixel), _bytesPerLine(width * _bytesPerPixel), _cursor(-1, 0), _begin(begin),
	_width(width), _height(height), _subsample(subsample)
{ }

const cv::Point2i& procs::FrameBufferIterator::cursor() const
{
	return this->_cursor;
}

bool procs::FrameBufferIterator::inBounds(const unsigned int x, const unsigned int y)
{
	return (x >= 0) && (x < this->_width) && (y >= 0) && (y < this->_height);
}

bool procs::FrameBufferIterator::goNext()
{
	this->_cursor.x += this->_subsample;
	if (this->_cursor.x >= this->_width)
	{
		this->_cursor.x = 0;
		this->_cursor.y += this->_subsample;
	}
	return (this->_cursor.y < this->_height);
}

bool procs::FrameBufferIterator::go(const unsigned int x, const unsigned int y)
{
	if (this->inBounds(x, y))
	{
		this->_cursor.x = x;
		this->_cursor.y = y;
		return true;
	}
	return false;
}

bool procs::FrameBufferIterator::goLeft(const unsigned int amount)
{
	return this->go(this->_cursor.x + this->_subsample*amount, this->_cursor.y);
}

bool procs::FrameBufferIterator::goRight(const unsigned int amount)
{
	return this->go(this->_cursor.x - this->_subsample*amount, this->_cursor.y);
}

bool procs::FrameBufferIterator::goUp(const unsigned int amount)
{
	return this->go(this->_cursor.x, this->_cursor.y - this->_subsample*amount);
}

bool procs::FrameBufferIterator::goDown(const unsigned int amount)
{
	return this->go(this->_cursor.x, this->_cursor.y + this->_subsample*amount);
}

bool procs::FrameBufferIterator::get(cv::Vec3b *pixel)
{
	this->getPixel(pixel, this->_cursor.x, this->_cursor.y);
}

bool procs::FrameBufferIterator::get(cv::Vec3b *pixel, const int offsetX, const int offsetY)
{
	return this->getPixel(pixel, this->_cursor.x + offsetX, this->_cursor.y + offsetY);
}

bool procs::FrameBufferIterator::get(cv::Vec3b &pixel)
{
	return this->getPixel(pixel, this->_cursor.x, this->_cursor.y);
}

bool procs::FrameBufferIterator::get(cv::Vec3b &pixel, const int offsetX, const int offsetY)
{
	return this->getPixel(pixel, this->_cursor.x + offsetX, this->_cursor.y + offsetY);
}

procs::FramBufferIteratorRGB24::FramBufferIteratorRGB24(uint8_t *begin,
	unsigned int width, unsigned int height, unsigned int subsample)
	: FrameBufferIterator(sizeof(cv::Vec3b), begin, width, height, subsample)
{ }

uint8_t *procs::FramBufferIteratorRGB24::getAddress(const unsigned int x, const unsigned int y)
{
	if (this->inBounds(x, y))
		return this->_begin + (this->_bytesPerLine * y) + (this->_bytesPerPixel * x);
	return nullptr;
}

bool procs::FramBufferIteratorRGB24::getPixel(cv::Vec3b *px, const unsigned int x, const unsigned int y)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		px->val[0] = *p++;
		px->val[1] = *p++;
		px->val[2] = *p;
		return true;
	}
	return false;
}

bool procs::FramBufferIteratorRGB24::getPixel(cv::Vec3b &px, const unsigned int x, const unsigned int y)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		px.val[0] = *p++;
		px.val[1] = *p++;
		px.val[2] = *p;
		return true;
	}
	return false;
}

bool procs::FramBufferIteratorRGB24::setPixel(const unsigned int x, const unsigned int y,
	cv::Vec3b *px)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		*p++ = px->val[0];
		*p++ = px->val[1];
		*p = px->val[2];
		return true;
	}
	return false;
}

bool procs::FramBufferIteratorRGB24::setPixel(const unsigned int x, const unsigned int y,
	const cv::Vec3b &px)
{
	uint8_t *p = this->getAddress(x, y);
	if (p)
	{
		*p++ = px.val[0];
		*p++ = px.val[1];
		*p = px.val[2];
		return true;
	}
	return false;
}

bool procs::FramBufferIteratorRGB24::setPixel(cv::Vec3b *px)
{
	return this->setPixel(this->_cursor.x, this->_cursor.y, px);
}

bool procs::FramBufferIteratorRGB24::setPixel(const cv::Vec3b &px)
{
	return this->setPixel(this->_cursor.x, this->_cursor.y, px);
}
