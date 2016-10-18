/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include "segmentationcolourobject.h"

mote::procs::SegmentationColourObject::SegmentationColourObject()
{ }

mote::procs::SegmentationColourObject::SegmentationColourObject(const cv::Rect2i &bBox,
	const cv::Point2i &center, unsigned int size, const cv::Vec3b &average, const cv::Vec3b &seed)
	: bBox(bBox), center(center), size(size), average(average), seed(seed)
{ }
