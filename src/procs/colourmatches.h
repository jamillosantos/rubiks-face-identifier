/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 21, 2016
 */

#ifndef RUBIKS_COLOURMATCHES_H
#define RUBIKS_COLOURMATCHES_H

#include "segmentationcolourobject.h"
#include "segmentcolour.h"

namespace procs
{

class SegmentationColourObjectGroup
{
public:
	std::vector<SegmentationColourObject*> objects;

	bool matches(SegmentColourVisitor& visitor, SegmentationColourObject *obj);
};

class ColourMatches
{
public:
	void match(const std::vector<SegmentationColourObject*> grid, std::vector<SegmentationColourObjectGroup>& groups);
};
}


#endif //RUBIKS_COLOURMATCHES_H
