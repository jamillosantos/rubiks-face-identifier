/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#ifndef RUBIKS_GRIDTRACER_H
#define RUBIKS_GRIDTRACER_H

#include <data/configuration.h>

namespace procs
{
class GridTracer
{
private:
	data::configuration::GridDefinition &_definition;
	std::vector<cv::Point2i> guide;
public:
	GridTracer(data::configuration::GridDefinition &definition);

	void trace(std::vector<cv::Point2i> &points);
};
}


#endif //RUBIKS_GRIDTRACER_H
