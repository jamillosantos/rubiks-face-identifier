/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 26, 2016
 */

#ifndef MOTE_VISION_DATA_FLOODFILLSTATE_H
#define MOTE_VISION_DATA_FLOODFILLSTATE_H

#include <boost/algorithm/clamp.hpp>
#include <opencv2/core/types.hpp>

namespace mote
{
namespace procs
{
/**
 * This class prepresents the state of the flood fill algorithm implemented for the segment colour detection.
 *
 * Based on a AALabs (UofM) implementation. For more information visit: https://gitlab.com/aalab/autman
 */
class FloodFillState
{
private:
	std::size_t _size;

	unsigned int _sumX;
	unsigned int _sumY;

	cv::Rect2i _bBox;

	unsigned int _sumRed;
	unsigned int _sumGreen;
	unsigned int _sumBlue;
public:
	FloodFillState();

	/**
	 * Adds a point to the state.
	 *
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param pixel Colour of the pixel
	 */
	void addPoint(const unsigned int x, const unsigned int y, const cv::Vec3b& pixel);
	/**
	 * @see addPoint(unsigned int, unsigned int, const Pixel&)
	 * @param p Coordinate
	 * @param pixel Colour of the pixel
	 */
	void addPoint(const cv::Point2f& p, const cv::Vec3b& pixel);

	/**
	 * Boundary box of all added point.
	 *
	 * @return Self.
	 */
	cv::Rect2i& bBox();

	std::size_t size() const;
	FloodFillState& size(std::size_t size);

	unsigned int sumX() const;
	FloodFillState& sumX(unsigned int sumX);

	unsigned int sumY() const;
	FloodFillState& sumY(unsigned int sumY);

	int x() const;
	int y() const;

	/**
	 * Average colour.
	 *
	 * @return
	 */
	cv::Vec3b averageColour() const;

	/**
	 * Clean the variables putting it to the initial state.
	 */
	void clear();
};
}
}

#endif //MOTE_VISION_DATA_FLOODFILLSTATE_H
