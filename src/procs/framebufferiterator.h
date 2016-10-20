/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#ifndef PROCS_FRAMEBUFFERITERATOR_H
#define PROCS_FRAMEBUFFERITERATOR_H

#include <cstdint>
#include <opencv2/core/mat.hpp>

namespace procs
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

	virtual bool getPixel(cv::Vec3b *px, const unsigned int x, const unsigned int y) = 0;
	virtual bool getPixel(cv::Vec3b &px, const unsigned int x, const unsigned int y) = 0;

	virtual bool setPixel(cv::Vec3b *px) = 0;
	virtual bool setPixel(const cv::Vec3b &px) = 0;
	virtual bool setPixel(const unsigned int x, const unsigned int y, cv::Vec3b *px) = 0;
	virtual bool setPixel(const unsigned int x, const unsigned int y, const cv::Vec3b &px) = 0;

	virtual bool goNext();
	virtual bool go(const unsigned int x, const unsigned int y);
	virtual bool goLeft(const unsigned int amount = 1);
	virtual bool goRight(const unsigned int amount = 1);
	virtual bool goUp(const unsigned int amount = 1);
	virtual bool goDown(const unsigned int amount = 1);

	virtual bool get(cv::Vec3b *pixel);
	virtual bool get(cv::Vec3b *pixel, const int offsetX, const int offsetY);
	virtual bool get(cv::Vec3b &pixel);
	virtual bool get(cv::Vec3b &pixel, const int offsetX, const int offsetY);
};

class FramBufferIteratorRGB24
	: public FrameBufferIterator
{
public:
	FramBufferIteratorRGB24(uint8_t *begin, unsigned int width, unsigned int height, unsigned int subsample);

	virtual uint8_t *getAddress(const unsigned int x, const unsigned int y) override;

	virtual bool getPixel(cv::Vec3b *px, const unsigned int x, const unsigned int y) override;
	virtual bool getPixel(cv::Vec3b &px, const unsigned int x, const unsigned int y) override;

	virtual bool setPixel(cv::Vec3b *px) override;
	virtual bool setPixel(const cv::Vec3b &px) override;
	virtual bool setPixel(const unsigned int x, const unsigned int y, cv::Vec3b *px) override;
	virtual bool setPixel(const unsigned int x, const unsigned int y, const cv::Vec3b &px) override;
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

#endif //PROCS_FRAMEBUFFERITERATOR_H
