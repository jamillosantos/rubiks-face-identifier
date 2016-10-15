/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#ifndef MOTE_VISION_DATA_SEGMENTATIONCOLOUROBJECT_H
#define MOTE_VISION_DATA_SEGMENTATIONCOLOUROBJECT_H

#include <opencv2/core/types.hpp>
#include "colour.h"
#include "rect.h"

namespace mote
{
namespace data
{
/**
 * This class represents the objects found by the segmentation colour algorithm.
 */
class SegmentationColourObject
{
public:
	/**
	 * Boundary box of the blob.
	 */
	mote::data::Rect<int> bBox;

	/**
	 * This keeps the center of the blob.
	 */
	cv::Point2i center;

	/**
	 * Size of the blob.
	 */
	unsigned int size;

	/**
	 * RGB of the average colour of the blob.
	 */
	mote::data::Pixel average;
	/**
	 * Seed colour used in the flood fill algorithm.
	 */
	mote::data::Pixel seed;

	SegmentationColourObject();

	SegmentationColourObject(const Rect<int> &bBox, const cv::Point2i &center, unsigned int size,
		const Pixel &average, const Pixel &seed);
};
}
}

#endif //MOTE_VISION_DATA_SEGMENTATIONCOLOUROBJECT_H
