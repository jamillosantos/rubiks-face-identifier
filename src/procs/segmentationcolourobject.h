/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#ifndef PROCS_SEGMENTATIONCOLOUROBJECT_H
#define PROCS_SEGMENTATIONCOLOUROBJECT_H

#include <opencv2/core/types.hpp>

namespace procs
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
	cv::Rect2i bBox;

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
	cv::Vec3b average;

	/**
	 * Seed colour used in the flood fill algorithm.
	 */
	cv::Vec3b seed;

	SegmentationColourObject();

	SegmentationColourObject(const cv::Rect2i &bBox, const cv::Point2i &center, unsigned int size,
		const cv::Vec3b &average, const cv::Vec3b &seed);

	SegmentationColourObject& operator=(const SegmentationColourObject& obj);
};
}

#endif //PROCS_SEGMENTATIONCOLOUROBJECT_H
