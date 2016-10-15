/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#ifndef MOTE_VISION_IMGPROCS_SEGMENTCOLOUR_H
#define MOTE_VISION_IMGPROCS_SEGMENTCOLOUR_H

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

#include <data/colour_definition.h>
#include <data/segmentationcolourobject.h>
#include <data/floodfillstate.h>

#define BRIGHT_PIXEL 256
#define DARK_PIXEL 4

namespace mote
{
namespace imgprocs
{
/**
 * Performs the segmentation colour detection using the `data::ColourDefinition`.
 *
 * This methods is fully based on the University of Manitoba Autonomous Agent Laboratory implementation. You can find
 * more about it at: http://aalab.cs.umanitoba.ca and https://gitlab.com/aalab/autman
 */
class SegmentColour
{
public:
	enum ErrorCode
	{
		NO_ERROR = 0,
		OUT_OF_BOUNDS,
		INVALID_SEED_POINT,
		STACK_OVERFLOW
	};
private:
	cv::Point* _stack;
	int _stackSize;
	int _stackLength;
protected:
	void pushToStack(const unsigned int x, const unsigned int y);
	void pushToStack(const cv::Point2i& point);
	cv::Point2i* popFromStack();

	/**
	 * Performs the flood fill algorithm.
	 *
	 * @param in Input frame buffer
	 * @param out Output frame buffer
	 * @param p The origin point of the flood fill
	 * @param seed The seed colour used to fill the output buffer (for checking visited pixels)
	 * @param threshold The maximum quad intensity difference between colours
	 * @param target Colour restrinctions
	 * @param subsample Subsample used for the detection
	 * @param state The state of the flood fill algorithm
	 * @return If fails it will return the code of the error
	 */
	ErrorCode doFloodFill(cv::Mat &in, cv::Mat &out, cv::Point2i p, mote::data::Pixel &seed, unsigned int threshold,
		const mote::data::ColourDefinition *target, unsigned int subsample, mote::data::FloodFillState *state);
public:
	SegmentColour(unsigned int stackSize);
	virtual ~SegmentColour();

	/**
	 * Performs the detection.
	 *
	 * @param in Input frame buffer
	 * @param out Output frame buffer
	 * @param threshold The maximum quad intensity difference between colours
	 * @param minLength Minimum length of a segment
	 * @param minSize Minimum size of a segment
	 * @param subsample Subsample used for the detection
	 * @param colourDefinition Colour restrinctions
	 * @param results Vector of the objects found on the process
	 */
	void action(cv::Mat &in, cv::Mat &out, unsigned int threshold, unsigned int minLength, unsigned int minSize,
		unsigned int subsample, const mote::data::ColourDefinition &colourDefinition,
		std::vector<mote::data::SegmentationColourObject> &results);
};
}
}


#endif //MOTE_VISION_IMGPROCS_SEGMENTCOLOUR_H
