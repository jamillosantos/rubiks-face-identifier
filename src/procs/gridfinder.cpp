/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 19, 2016
 */

#include "gridfinder.h"

void procs::GridFinder::groupByLines(std::list<procs::SegmentationColourObject *> &objects,
	std::vector<std::vector<procs::SegmentationColourObject *>> &lines)
{
	procs::SegmentationColourObject *obj;
	while (!objects.empty())
	{
		obj = objects.back();
		objects.pop_back();
		lines.emplace_back();
		lines.back().push_back(obj);
		std::cout << "Start" << "(" << obj->bBox.x << ", " << obj->bBox.y << ", " << obj->bBox.width << ", " << obj->bBox.width << ") // " << objects.size() << " left" << std::endl;
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			std::cout << "-- " << procs::verticalIntersectionFactor((*it)->bBox, obj->bBox) << "(" << (*it)->bBox.x << ", " << (*it)->bBox.y << ", " << (*it)->bBox.width << ", " << (*it)->bBox.width << ")" << std::endl;
			if (procs::verticalIntersectionFactor((*it)->bBox, obj->bBox) > 0)
			{
				std::cout << "     <<<< added" << std::endl;
				lines.back().push_back(*it);
				it = --objects.erase(it);
			}
		}
	}
}

void procs::GridFinder::find(std::vector<procs::SegmentationColourObject*> &objects,
	std::vector<procs::SegmentationColourObject*> &grid)
{
	/*
	std::vector<std::vector<procs::SegmentationColourObject*>> lines;
	std::list<procs::SegmentationColourObject*> os;
	for (auto* obj : objects)
		os.push_back(obj);
	this->groupByLines(os, lines);
	for (auto line : lines)
	{
		std::cout << "LINE: " << std::endl;
		for (auto obj : line)
		{
			std::cout << "-- " << obj->bBox.x << ":" << obj->bBox.y << std::endl;
		}
	}
	*/

	std::vector<procs::SegmentationColourObject*>
		os(objects),
		line;
	this->findLine(line, os);
}

void procs::GridFinder::findLine(std::vector<SegmentationColourObject *> line,
	std::vector<SegmentationColourObject *> objects)
{ }

