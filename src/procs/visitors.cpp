/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 21, 2016
 */

#include "visitors.h"

bool procs::SegmentColourAverageDistanceVisitor::matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const
{
	if (
		std::abs(
			((base.val[0] + base.val[1] + base.val[2]) / 3.0) -
			((neighbour.val[0] + neighbour.val[1] + neighbour.val[2]) / 3.0)
		) / 255.0 > 0.02
		)
		return false;
	cv::Vec3b diff = (base - neighbour);
	unsigned int max = 0;
	for (unsigned int i = 1; i < 3; i++)
	{
		if (base.val[max] < base.val[i])
			max = i;
	}
	cv::Vec3d
		baseRatio = (base / base.val[max]),
		neightbourRatio = (neighbour / neighbour.val[max]);
	for (unsigned int i = 0; i < 3; i++)
	{
		if (std::abs(baseRatio.val[i] - neightbourRatio.val[i]) > 0.1)
			return false;
	}
	return true;
}

procs::SegmentColourRGWeightGWeightVisitor::SegmentColourRGWeightGWeightVisitor(const cv::Vec2d &threshold)
	: threshold(threshold)
{ }

bool procs::SegmentColourRGWeightGWeightVisitor::matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const
{
	double
		rbRatio = std::abs(((base.val[0] + base.val[2]) / 2.0) - ((neighbour.val[0] + neighbour.val[2]) / 2.0)) / 255.0,
		gRatio = std::abs(base.val[1] - neighbour.val[1]) / 255.0;
	if ((rbRatio > this->threshold.val[0]) || (gRatio > this->threshold.val[1]))
		return false;
	return true;
}

procs::SegmentColourRGBProportionVisitor::SegmentColourRGBProportionVisitor(const cv::Vec3d &threshold)
	: threshold(threshold)
{}

bool procs::SegmentColourRGBProportionVisitor::matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const
{
	int max = 0;
	for (int i = 1; i < 3; ++i)
	{
		if (base.val[max] < base.val[i])
			max = i;
	}
	for (int i = 0; i < 3; ++i)
	{
		if ((max != i) && (std::abs((base.val[i] / (double) base.val[max]) - (neighbour.val[i] / (double) neighbour.val[max])) > this->threshold.val[i]))
			return false;
	}
	return true;
}

procs::SegmentColourRBByGProportionVisitor::SegmentColourRBByGProportionVisitor(double threshold)
	: threshold(threshold)
{ }

bool procs::SegmentColourRBByGProportionVisitor::matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const
{
	return (
		(std::abs((base.val[0] / (double)base.val[1]) - (neighbour.val[0] / (double)neighbour.val[1])) < this->threshold)
		&& (std::abs((base.val[2] / (double)base.val[1]) - (neighbour.val[2] / (double)neighbour.val[1])) < this->threshold)
	);
}
