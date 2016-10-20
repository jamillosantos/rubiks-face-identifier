/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 19, 2016
 */

#ifndef RUBIKS_PROCS_CLASSIFIER_H
#define RUBIKS_PROCS_CLASSIFIER_H

#include <list>
#include "segmentationcolourobject.h"

namespace procs
{

template <typename T>
double verticalIntersectionFactor(const cv::Rect_<T> &a, const cv::Rect_<T> &b)
{
	return std::max((
		std::min(a.y + a.height, b.y + b.height)
		- std::max(a.y, b.y)
	) / (double)std::min(a.height, b.height), 0.0);
}

class SementGroup
{
public:
	std::vector<SegmentationColourObject> objects;
};

class GridFinder
{
private:
	bool findBy(procs::SegmentationColourObject *seed, std::list<SegmentationColourObject*> objects,
		std::vector<procs::SegmentationColourObject*> &grid);
public:
	void find(std::vector<SegmentationColourObject> &objects, std::vector<procs::SegmentationColourObject*> &grid);
};
}


#endif //RUBIKS_PROCS_CLASSIFIER_H
