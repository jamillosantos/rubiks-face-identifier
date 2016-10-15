/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include <data/floodfillstate.h>
#include <cv.hpp>
#include "segmentcolour.h"
#include "framebufferiterator.h"

mote::imgprocs::SegmentColour::SegmentColour(unsigned int stackSize)
	: _stackSize(stackSize), _stack(new cv::Point[stackSize]), _stackLength(0)
{ }

mote::imgprocs::SegmentColour::~SegmentColour()
{
	delete this->_stack;
}

void mote::imgprocs::SegmentColour::pushToStack(const unsigned int x, const unsigned int y)
{
	this->_stack[this->_stackLength].x = x;
	this->_stack[this->_stackLength++].y = y;
}

void mote::imgprocs::SegmentColour::pushToStack(const cv::Point2i &point)
{
	this->_stack[this->_stackLength++] = point;
}

cv::Point2i *mote::imgprocs::SegmentColour::popFromStack()
{
	if (this->_stackLength > 0)
	{
		return &this->_stack[--this->_stackLength];
	}
	return nullptr;
}

mote::imgprocs::SegmentColour::ErrorCode mote::imgprocs::SegmentColour::doFloodFill(cv::Mat &in, cv::Mat &out, cv::Point2i p,
	mote::data::Pixel &seed, unsigned int threshold, const mote::data::ColourDefinition *target, unsigned int subsample,
	mote::data::FloodFillState *state)
{
	using namespace mote::data;

	Pixel pixel;
	Pixel outPixel;
	Pixel neighbour;

	unsigned int diff;

	MatIterator<FramBufferIteratorRGB24> iter(in);
	MatIterator<FramBufferIteratorRGB24> outIter(out);
	cv::Point &c = p;

	enum ErrorCode err = NO_ERROR;

	// Use stackIndex = 0 as underflow check
	unsigned int stackIndex = 1;

	//  state->initialize();

	// outFrame->fill( RawPixel(0,0,0) ); // Set to black.

	// If the seed is black, the flood fill will get stuck in an
	// infinite loop.  So, we skip floodfills on black seeds.

	if ((seed.r == 0) && (seed.g == 0) && (seed.b == 0))
	{
		// Fudge the seed to be not all black.
		seed.b = 1;
	}

	if (!(iter.go(p.x, p.y) && outIter.go(p.x, p.y)))
		return INVALID_SEED_POINT;

	// Push the initial point onto the stack

	this->_stack[stackIndex++] = p;
	while (stackIndex > 1)
	{
		if (stackIndex >= this->_stackSize - 4)
		{
			return STACK_OVERFLOW;
		}
		c = this->_stack[--stackIndex];
#ifndef Q_NODEBUG
		//      std::cout << " processing point (" << c.x() << ',' << c.y() << ")" << std::endl;
#endif

		if (!(iter.go(c.x, c.y) && outIter.go(c.x, c.y)))
			return OUT_OF_BOUNDS;

		iter.get(&pixel);
		outIter.get(&outPixel);

		//      if ( ! ImageProcessing::isChecked( outPixel ) )
		if ((outPixel.r == 0) && (outPixel.g == 0) && (outPixel.b == 0))
		{
			if (((pixel.r < BRIGHT_PIXEL) && (pixel.r > DARK_PIXEL)) && (pixel.g > DARK_PIXEL) && (pixel.b > DARK_PIXEL))
			{
				state->addPoint(c, pixel);

				outIter.setPixel(seed);
				iter.setPixel(seed);
				if (c.x >= subsample)
				{
					iter.get(&neighbour, -subsample, 0);
					diff = pixel.diffIntensity(neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->isMatch(pixel)))
					{
						this->_stack[stackIndex].x = c.x - subsample;
						this->_stack[stackIndex++].y = c.y;
					}
				}

				if (c.x < (in.cols - 1 - subsample))
				{
					iter.get(&neighbour, subsample, 0);
					diff = pixel.diffIntensity(neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->isMatch(pixel)))
					{
						this->_stack[stackIndex].x = c.x + subsample;
						this->_stack[stackIndex++]. y = c.y;
					}
				}

				if (c.y >= subsample)
				{
					iter.get(&neighbour, 0, -subsample);
					diff = pixel.diffIntensity(neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->isMatch(pixel)))
					{
						this->_stack[stackIndex].x = c.x;
						this->_stack[stackIndex++].y = c.y - subsample;
					}
				}

				if (c.y < (in.rows - 1 - subsample))
				{
					iter.get(&neighbour, 0, +subsample);
					diff = pixel.diffIntensity(neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->isMatch(pixel)))
					{
						this->_stack[stackIndex].x = c.x;
						this->_stack[stackIndex++].y = c.y + subsample;
					}
				}
			}
			else
			{
				// mark the pixel as checked
				outIter.setPixel(seed);
				iter.setPixel(seed);
			}
		}
	}
	state->
		sumX(state->sumX() * subsample * subsample)
		.sumY(state->sumY() * subsample * subsample)
		.size(state->size() * subsample * subsample);

	return err;
}

void mote::imgprocs::SegmentColour::action(cv::Mat &in, cv::Mat &out, unsigned int threshold, unsigned int minLength,
	unsigned int minSize, unsigned int subsample, const mote::data::ColourDefinition &colourDefinition,
	std::vector<mote::data::SegmentationColourObject>& results)
{
	using namespace mote::data;
	MatIterator<FramBufferIteratorRGB24>
		it(in, subsample),
		oit(out, subsample);
	Pixel
		cPixel,
		oPixel;
	FloodFillState state;
	unsigned int count;

	for (unsigned int row = 0; row < in.rows; row = row + subsample)
	{
		it.go(subsample, row);
		oit.go(subsample, row);

		count = 0;
		for (unsigned int col = subsample; col < in.cols; col = col + subsample, it.goLeft(subsample), oit.goLeft(subsample))
		{
			oit.get(oPixel);
			if (oPixel.is(0, 0, 0))
			{
				it.get(cPixel);
				// oit.setPixel(highlightPixel);

				if (colourDefinition.isMatch(cPixel))
					count++;
				else
					count = 0;

				if (count >= minLength)
				{
					state.clear();
					this->doFloodFill(in, out, cv::Point2i(col, row), cPixel, threshold, &colourDefinition, subsample, &state);

					if (state.size() > minSize)
					{
						cv::rectangle(out, state.bBox().tl(), state.bBox().br(), cv::Scalar(0, 0, 255));

						std::vector<SegmentationColourObject>::iterator i;
						for (i = results.begin(); i != results.end(); ++i)
						{
							if ((*i).size < state.size())
								break;
						}
						results.emplace(i, state.bBox(), cv::Point2i(state.x(), state.y()), state.size(), state.averageColour(), cPixel);
					}
					count = 0;
				}
			}
			else
				count = 0;
		}
	}
}
