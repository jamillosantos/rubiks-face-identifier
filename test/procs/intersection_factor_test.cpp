/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 19, 2016
 */

#include <gtest/gtest.h>
#include <opencv2/core/types.hpp>
#include <procs/gridfinder.h>

GTEST_TEST(procs, vertical_intersection_factor__fullintersection)
{
	cv::Rect2i
		r1(10, 10, 20, 10),
		r2(50, 10, 20, 10);
	ASSERT_EQ(1.0, procs::verticalIntersectionFactor(r1, r2));
	ASSERT_EQ(1.0, procs::verticalIntersectionFactor(r2, r1));
}

GTEST_TEST(procs, vertical_intersection_factor__coincident)
{
	cv::Rect2i
		r1(10, 10, 20, 10);
	ASSERT_EQ(1.0, procs::verticalIntersectionFactor(r1, r1));
}

GTEST_TEST(procs, vertical_intersection_factor__within)
{
	cv::Rect2i
		r1(15, 15, 50, 50),
		r2(20, 20, 20, 20);
	ASSERT_EQ(1.0, procs::verticalIntersectionFactor(r1, r2));
	ASSERT_EQ(1.0, procs::verticalIntersectionFactor(r2, r1));
}

GTEST_TEST(procs, vertical_intersection_factor__half)
{
	cv::Rect2i
		r1(10, 10, 20, 10),
		r2(50, 15, 20, 10);
	ASSERT_EQ(0.5, procs::verticalIntersectionFactor(r1, r2));
	ASSERT_EQ(0.5, procs::verticalIntersectionFactor(r2, r1));
}

GTEST_TEST(procs, vertical_intersection_factor__no_intersection__touching)
{
	cv::Rect2i
		r1(10, 10, 20, 10),
		r2(50, 20, 20, 10);
	ASSERT_EQ(0.0, procs::verticalIntersectionFactor(r1, r2));
	ASSERT_EQ(0.0, procs::verticalIntersectionFactor(r2, r1));
}

GTEST_TEST(procs, vertical_intersection_factor__no_intersection__not_even_close)
{
	cv::Rect2i
		r1(10, 10, 20, 10),
		r2(500, 200, 20, 10);
	ASSERT_EQ(0.0, procs::verticalIntersectionFactor(r1, r2));
	ASSERT_EQ(0.0, procs::verticalIntersectionFactor(r2, r1));
}