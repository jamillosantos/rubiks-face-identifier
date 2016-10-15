/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#ifndef MOTE_VISION_MATCVITERATOR_H
#define MOTE_VISION_MATCVITERATOR_H

#include <cstdint>
#include <data/pixel.h>
#include <opencv2/core/mat.hpp>

namespace mote
{
namespace imgprocs
{
class FrameBufferIterator
{
protected:
	unsigned int _bytesPerPixel;
	unsigned int _bytesPerLine;

	cv::Point2i _cursor;

	uint8_t* _begin;
	unsigned int _width;
	unsigned int _height;

	unsigned int _subsample;
public:
	FrameBufferIterator(unsigned int bytesPerPixel, uint8_t *begin, unsigned int width, unsigned int height,
		unsigned int subsample);

	const cv::Point2i& cursor() const;

	virtual uint8_t* getAddress(const unsigned int x, const unsigned int y) = 0;

	virtual bool inBounds(const unsigned int x, const unsigned int y);

	virtual bool getPixel(mote::data::Pixel *px, const unsigned int x, const unsigned int y) = 0;
	virtual bool getPixel(mote::data::Pixel &px, const unsigned int x, const unsigned int y) = 0;

	virtual bool setPixel(mote::data::Pixel *px) = 0;
	virtual bool setPixel(const mote::data::Pixel &px) = 0;
	virtual bool setPixel(const unsigned int x, const unsigned int y, mote::data::Pixel *px) = 0;
	virtual bool setPixel(const unsigned int x, const unsigned int y, const mote::data::Pixel &px) = 0;

	virtual bool goNext();
	virtual bool go(const unsigned int x, const unsigned int y);
	virtual bool goLeft(const unsigned int amount = 1);
	virtual bool goRight(const unsigned int amount = 1);
	virtual bool goUp(const unsigned int amount = 1);
	virtual bool goDown(const unsigned int amount = 1);

	virtual bool get(mote::data::Pixel *pixel);
	virtual bool get(mote::data::Pixel *pixel, const int offsetX, const int offsetY);
	virtual bool get(mote::data::Pixel &pixel);
	virtual bool get(mote::data::Pixel &pixel, const int offsetX, const int offsetY);
};

class FramBufferIteratorRGB24
	: public FrameBufferIterator
{
public:
	FramBufferIteratorRGB24(uint8_t *begin, unsigned int width, unsigned int height, unsigned int subsample);

	virtual uint8_t *getAddress(const unsigned int x, const unsigned int y) override;

	virtual bool getPixel(mote::data::Pixel *px, const unsigned int x, const unsigned int y) override;
	virtual bool getPixel(mote::data::Pixel &px, const unsigned int x, const unsigned int y) override;

	virtual bool setPixel(mote::data::Pixel *px) override;
	virtual bool setPixel(const mote::data::Pixel &px) override;
	virtual bool setPixel(const unsigned int x, const unsigned int y, mote::data::Pixel *px) override;
	virtual bool setPixel(const unsigned int x, const unsigned int y, const mote::data::Pixel &px) override;
};

template <typename T>
class MatIterator
	: public T
{
public:
	MatIterator(cv::Mat &mat, const unsigned int subsample = 1)
		: T(mat.data, mat.cols, mat.rows, subsample)
	{ }
};

}
}

#endif //MOTE_VISION_MATCVITERATOR_H
