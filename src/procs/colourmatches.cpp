/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 21, 2016
 */

#include "colourmatches.h"
#include "visitors.h"

bool procs::SegmentationColourObjectGroup::matches(procs::SegmentColourVisitor &visitor, procs::SegmentationColourObject *obj)
{
	for (SegmentationColourObject* c : this->objects)
	{
		if (visitor.matches(c->average, obj->average))
			return true;
	}
	return false;
}

void procs::ColourMatches::match(const std::vector<SegmentationColourObject *> grid,
	std::vector<procs::SegmentationColourObjectGroup> &groups)
{
	auto tmpGrid(grid);
	SegmentationColourObject* obj;
	// procs::SegmentColourRGBProportionVisitor visitor(cv::Vec3d(0.1, 0.1, 0.1));
	procs::SegmentColourRBByGProportionVisitor visitor(0.3);

	bool found;
	while (!tmpGrid.empty())
	{
		obj = tmpGrid.back();
		tmpGrid.pop_back();
		found = false;
		for (SegmentationColourObjectGroup &group : groups)
		{
			if (group.matches(visitor, obj))
			{
				found = true;
				group.objects.push_back(obj);
				break;
			}
		}
		if (!found)
		{
			groups.emplace_back();
			groups.back().objects.push_back(obj);
		}
	}
}
