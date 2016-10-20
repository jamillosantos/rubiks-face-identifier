/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#include "segmentationcolourobject.h"

procs::SegmentationColourObject::SegmentationColourObject()
{ }

procs::SegmentationColourObject::SegmentationColourObject(const cv::Rect2i &bBox,
	const cv::Point2i &center, unsigned int size, const cv::Vec3b &average, const cv::Vec3b &seed)
	: bBox(bBox), center(center), size(size), average(average), seed(seed)
{ }

procs::SegmentationColourObject &procs::SegmentationColourObject::operator=(const procs::SegmentationColourObject &obj)
{
	this->bBox = obj.bBox;
	this->center = obj.center;
	this->size = obj.size;
	this->average = obj.average;
	this->seed = obj.seed;
	return *this;
}
