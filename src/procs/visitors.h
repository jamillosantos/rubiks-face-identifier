/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 21, 2016
 */

#ifndef RUBIKS_VISITORS_H
#define RUBIKS_VISITORS_H

#include "segmentcolour.h"

namespace procs
{

class SegmentColourAverageDistanceVisitor
	: public procs::SegmentColourVisitor
{
public:
	virtual bool matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const override;
};

class SegmentColourRGWeightGWeightVisitor
	: public procs::SegmentColourVisitor
{
private:
	cv::Vec2d threshold;
public:
	SegmentColourRGWeightGWeightVisitor(const cv::Vec2d &threshold);

	virtual bool matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const override;
};

class SegmentColourRGBProportionVisitor
	: public procs::SegmentColourVisitor
{
private:
	cv::Vec3d threshold;
public:
	SegmentColourRGBProportionVisitor(const cv::Vec3d &threshold);

	virtual bool matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const override;
};

class SegmentColourRBByGProportionVisitor
	: public procs::SegmentColourVisitor
{
private:
	double threshold;
public:
	SegmentColourRBByGProportionVisitor(double threshold);

	virtual bool matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const override;
};
}


#endif //RUBIKS_VISITORS_H
