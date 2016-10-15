/**
 * @author J. Santos <jamillo@gmail.com>
 * @date August 17, 2016
 */

#ifndef MOTE_VISION_DATA_PIXEL_H
#define MOTE_VISION_DATA_PIXEL_H

#include <cstdint>

namespace mote
{
namespace data
{
struct Pixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;

	Pixel();

	Pixel(uint8_t r, uint8_t g, uint8_t b);

	Pixel(const Pixel& px);

	bool is(uint8_t r, uint8_t g, uint8_t b);

	double intensity() const;

	double diffIntensity(const Pixel &p) const;
};
}
}


#endif //MOTE_VISION_DATA_PIXEL_H
