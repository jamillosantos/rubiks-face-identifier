/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#include "gridtracer.h"

procs::GridTracer::GridTracer(data::configuration::GridDefinition &definition)
	: _definition(definition), guide(9)
{
	for (int i = 0; i < 9; ++i)
		this->guide[i] = cv::Point2d((i/3)-1, (i%3)-1);
}

void procs::GridTracer::trace(std::vector<cv::Point2i> &points)
{
	cv::Point2i ptDiff = (this->_definition.center - this->_definition.corner);

	double theta = std::atan2(
			(double)(this->_definition.center.y - this->_definition.corner.y),
			(double)(this->_definition.corner.x - this->_definition.center.x)
		),
		distance = cv::sqrt(ptDiff.x*ptDiff.x + ptDiff.y*ptDiff.y);
	unsigned int i = 0;
	for (cv::Point2d p : this->guide)
	{
		points[i].x = this->_definition.center.x - ((double)p.x * std::cos(theta) * distance);
		points[i].y = this->_definition.center.y - ((double)p.y * std::sin(theta) * distance);
		++i;
	}
}
