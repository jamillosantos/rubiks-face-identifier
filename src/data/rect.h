/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 26, 2016
 */

#ifndef MOTE_VISION_RECT_H
#define MOTE_VISION_RECT_H

#include <opencv2/core/types.hpp>

namespace mote
{
namespace data
{
/**
 * Represents a rectangle, based on the OpenCV implementaiton of a Rect.
 */
template <typename T>
class Rect
	: public cv::Rect_<T>
{
public:
	Rect()
	{ }

	Rect(T _x, T _y, T _width, T _height)
		: cv::Rect_<T>(_x, _y, _width, _height)
	{ }

	Rect(const cv::Rect_<T> &r)
		: cv::Rect_<T>(r)
	{ }

	Rect(const cv::Point_<T> &org, const cv::Size_<T> &sz)
		: cv::Rect_<T>(org, sz)
	{ }

	Rect(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2)
		: cv::Rect_<T>(pt1, pt2)
	{ }

	/**
	 * Returns the coordinate of the right boundary of the rectangle.
	 *
	 * @return
	 */
	T r() const
	{
		return this->x + this->width;
	}

	/**
	 * Updates the width to set the right boundary to the passed value.
	 *
	 * @param r New right boundary value
	 * @return This instance
	 */
	Rect<T>& r(const T r)
	{
		this->width = r - this->x;
		return *this;
	}

	/**
	 * Returns the coordinate of the bottom boundary of the rectangle.
	 *
	 * @return
	 */
	T b() const
	{
		return this->y + this->height;
	}

	/**
	 * Updates the height to set the bottom boundary to the passed value.
	 *
	 * @param b New bottom boundary value
	 * @return This instance
	 */
	Rect<T>& b(const T b)
	{
		this->height = b - this->y;
		return *this;
	}

	/**
	 * An alias for the property `x`.
	 *
	 * @return The left boundary value.
	 */
	T l() const
	{
		return this->x;
	}

	/**
	 * Sets the left boundary updating the width according to the new value.
	 *
	 * @return The left boundary value.
	 */
	Rect<T>& l(const T x)
	{
		this->width -= (x - this->x);
		this->x = x;
		return *this;
	}

	/**
	 * An alias for the property `y`.
	 *
	 * @return The top boundary value.
	 */
	T t() const
	{
		return this->y;
	}

	/**
	 * Sets the top boundary updating the height according to the new value.
	 *
	 * @return The left boundary value.
	 */
	Rect<T>& t(const T y)
	{
		this->height -= (y - this->y);
		this->y = y;
		return *this;
	}
};
}
}


#endif //MOTE_VISION_RECT_H
