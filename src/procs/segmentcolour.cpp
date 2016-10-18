/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include <boost/log/trivial.hpp>
#include <cv.hpp>
#include "segmentcolour.h"
#include "framebufferiterator.h"

mote::procs::SegmentColour::SegmentColour(unsigned int stackSize)
	: _stackSize(stackSize), _stack(new cv::Point[stackSize]), _stackLength(0)
{ }

mote::procs::SegmentColour::~SegmentColour()
{
	delete this->_stack;
}

void mote::procs::SegmentColour::pushToStack(const unsigned int x, const unsigned int y)
{
	this->_stack[this->_stackLength].x = x;
	this->_stack[this->_stackLength++].y = y;
	BOOST_LOG_TRIVIAL(trace) << "Q << " << x << ":" << y;
}

void mote::procs::SegmentColour::pushToStack(const cv::Point2i &point)
{
	this->_stack[this->_stackLength++] = point;
	BOOST_LOG_TRIVIAL(trace) << "Q << " << point.x << ":" << point.y;
}

cv::Point2i *mote::procs::SegmentColour::popFromStack()
{
	if (this->_stackLength > 0)
	{
		BOOST_LOG_TRIVIAL(trace) << "Q >>cd te	 " << this->_stack[this->_stackLength-1].x << ":" << this->_stack[this->_stackLength-1].y;
		return &this->_stack[--this->_stackLength];
	}
	return nullptr;
}

mote::procs::SegmentColour::ErrorCode mote::procs::SegmentColour::doFloodFill(cv::Mat &in, cv::Mat &out, cv::Point2i p,
	cv::Vec3b &seed, unsigned int threshold, const mote::procs::SegmentColourVisitor *target, unsigned int subsample,
	mote::procs::FloodFillState *state)
{
	using namespace mote::procs;

	cv::Vec3b pixel;
	cv::Vec3b outPixel;
	cv::Vec3b neighbour;

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

	if ((seed.val[0] == 0) && (seed.val[1] == 0) && (seed.val[2] == 0))
	{
		// Fudge the seed to be not all black.
		seed.val[0] = 1;
	}

	if (!(iter.go(p.x, p.y) && outIter.go(p.x, p.y)))
		return INVALID_SEED_POINT;

	// Push the initial point onto the stack

	this->_stack[stackIndex++] = p;
	while (stackIndex > 1)
	{
		if (stackIndex >= this->_stackSize - 4)
		{
			// TODO: Uncomment
			// ERROR("possible stack overflow " << stackIndex);
			return STACK_OVERFLOW;
		}
		c = this->_stack[--stackIndex];

		if (!(iter.go(c.x, c.y) && outIter.go(c.x, c.y)))
			return OUT_OF_BOUNDS;

		iter.get(&pixel);
		outIter.get(&outPixel);

		if ((outPixel.val[0] == 0) && (outPixel.val[1] == 0) && (outPixel.val[2] == 0))
		{
			if (((pixel.val[2] < BRIGHT_PIXEL) && (pixel.val[2] > DARK_PIXEL)) && (pixel.val[1] > DARK_PIXEL) && (pixel.val[0] > DARK_PIXEL))
			{
				state->addPoint(c, pixel);

				outIter.setPixel(seed);
				iter.setPixel(seed);
				if (c.x >= subsample)
				{
					iter.get(&neighbour, -subsample, 0);
					diff = this->diffIntensity(pixel, neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->matches(pixel, neighbour)))
					{
						this->_stack[stackIndex].x = c.x - subsample;
						this->_stack[stackIndex++].y = c.y;
					}
				}

				if (c.x < (in.cols - 1 - subsample))
				{
					iter.get(&neighbour, subsample, 0);
					diff = this->diffIntensity(pixel, neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->matches(pixel, neighbour)))
					{
						this->_stack[stackIndex].x = c.x + subsample;
						this->_stack[stackIndex++]. y = c.y;
					}
				}

				if (c.y >= subsample)
				{
					iter.get(&neighbour, 0, -subsample);
					diff = this->diffIntensity(pixel, neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->matches(pixel, neighbour)))
					{
						this->_stack[stackIndex].x = c.x;
						this->_stack[stackIndex++].y = c.y - subsample;
					}
				}

				if (c.y < (in.rows - 1 - subsample))
				{
					iter.get(&neighbour, 0, +subsample);
					diff = this->diffIntensity(pixel, neighbour);
					if ((diff <= threshold) && ((target == nullptr) || target->matches(pixel, neighbour)))
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

double mote::procs::SegmentColour::diffIntensity(const cv::Vec3b &base, const cv::Vec3b &from)
{
	double i1 = this->pxIntensity(base) / 3.0;
	double i2 = this->pxIntensity(from) / 3.0;
	double diff = i1 - i2;
	return diff * diff;
}

double mote::procs::SegmentColour::pxIntensity(const cv::Vec3b &pixel)
{
	return (pixel.val[0] + pixel.val[1] + pixel.val[2]) / 3.0;
}

void mote::procs::SegmentColour::action(cv::Mat &in, cv::Mat &out, unsigned int threshold, unsigned int minLength,
	unsigned int minSize, unsigned int subsample, const mote::procs::SegmentColourVisitor &colourDefinition,
	std::vector<mote::procs::SegmentationColourObject>& results)
{
	MatIterator<FramBufferIteratorRGB24>
		it(in, subsample),
		oit(out, subsample);
	cv::Vec3b
		cPixel,
		lastCPixel,
		oPixel;
	FloodFillState state;
	unsigned int count;

	for (unsigned int row = 0; row < in.rows; row = row + subsample)
	{
		it.go(subsample, row);
		oit.go(subsample, row);

		count = 0;
		it.get(lastCPixel);
		for (unsigned int col = subsample; col < in.cols; col = col + subsample, it.goLeft(subsample), oit.goLeft(subsample))
		{
			oit.get(oPixel);
			if ((oPixel.val[0] == 0) && (oPixel.val[1] == 0) && (oPixel.val[2] == 0))
			{
				it.get(cPixel);
				// oit.setPixel(highlightPixel);

				if (colourDefinition.matches(lastCPixel, cPixel))
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
				lastCPixel.val[0] = cPixel.val[0];
				lastCPixel.val[1] = cPixel.val[1];
				lastCPixel.val[2] = cPixel.val[2];
			}
			else
				count = 0;
		}
	}
}
