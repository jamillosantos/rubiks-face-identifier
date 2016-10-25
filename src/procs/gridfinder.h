/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 19, 2016
 */

#ifndef RUBIKS_PROCS_CLASSIFIER_H
#define RUBIKS_PROCS_CLASSIFIER_H

#include <iostream>
#include <list>
#include "segmentationcolourobject.h"

namespace procs
{

template <typename T>
double verticalIntersectionFactor(const cv::Rect_<T> &a, const cv::Rect_<T> &b)
{
	return std::max((
		std::min((double)(a.y + a.height), (double)(b.y + b.height))
		- std::max((double)a.y, (double)b.y)
	) / std::min((double)a.height, (double)b.height), (double)0.0);
}

class SementGroup
{
public:
	std::vector<SegmentationColourObject> objects;
};

class GridFinder
{
private:
	void groupByLines(std::list<procs::SegmentationColourObject*> &objects,
		std::vector<std::vector<procs::SegmentationColourObject*>> &lines);
public:
	void find(std::vector<SegmentationColourObject*> &objects, std::vector<procs::SegmentationColourObject*> &grid);

	void findLine(std::vector<SegmentationColourObject *> line, std::vector<SegmentationColourObject *> objects);
};
}


#endif //RUBIKS_PROCS_CLASSIFIER_H
