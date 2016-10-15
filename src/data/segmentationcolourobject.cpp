/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include "segmentationcolourobject.h"

mote::data::SegmentationColourObject::SegmentationColourObject()
{ }

mote::data::SegmentationColourObject::SegmentationColourObject(const mote::data::Rect<int> &bBox,
	const cv::Point2i &center, unsigned int size, const mote::data::Pixel &average, const mote::data::Pixel &seed)
	: bBox(bBox), center(center), size(size), average(average), seed(seed)
{ }
