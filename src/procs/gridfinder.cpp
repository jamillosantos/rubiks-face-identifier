/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 19, 2016
 */

#include <iostream>
#include <deque>
#include "gridfinder.h"

bool procs::GridFinder::findBy(procs::SegmentationColourObject *seed,
	std::list<procs::SegmentationColourObject*> objects, std::vector<procs::SegmentationColourObject*> &grid)
{
	procs::SegmentationColourObject *obj;
	while (!objects.empty())
	{
		obj = objects.back();
		objects.pop_back();
		double intersectFactor = procs::verticalIntersectionFactor(obj->bBox, seed->bBox);
		if (
			(
				// Check similarity by width and height
				((std::abs(obj->bBox.width - seed->bBox.width)/(double)seed->bBox.width) < 0.3)
				&& ((std::abs(obj->bBox.height - seed->bBox.height)/(double)seed->bBox.height) < 0.3)
			)
			&& (intersectFactor > 0.8)
		)
		{
			grid.push_back(obj);
		}
	}
}

void procs::GridFinder::find(std::vector<procs::SegmentationColourObject> &objects,
	std::vector<procs::SegmentationColourObject*> &grid)
{
	std::vector<procs::SegmentationColourObject*> queue(objects.size());
	std::list<procs::SegmentationColourObject*> os;
	for (procs::SegmentationColourObject &obj : objects)
		os.push_back(&obj);


	grid.push_back(&objects[27]);
	this->findBy(&objects[27], os, grid);
	grid.push_back(&objects[11]);
	this->findBy(&objects[11], os, grid);
	/*
	for (procs::SegmentationColourObject *obj : os)
	{
		grid.push_back(obj);
		if (this->findBy(obj, os, grid))
			return;
		else
			grid.pop_back();
	}
	 */
}
