/**
 * @author J. Santos <jamillo@gmail.com>
 * @date July 24, 2016
 */

#ifndef MOTE_VISION_COLOUR_H
#define MOTE_VISION_COLOUR_H

#include <cstdint>

#include <json/json.h>
#include "serializable.h"
#include "pixel.h"

namespace mote
{
namespace data
{
/**
 * Colour abstrction on the system.
 */
class Colour
	: public Serializable
{ };

/**
 * RGB Colour definition.
 */
struct RGBColour
	: public Colour
{
public:
	static RGBColour red;
	static RGBColour blue;
	static RGBColour green;
	static RGBColour white;
	static RGBColour black;
	static RGBColour yellow;
	static mote::data::RGBColour cyan;
	static mote::data::RGBColour magenta;

private:
	mote::data::Pixel* _pixel;
	bool _ownsPixel;
public:
	RGBColour(uint8_t r, uint8_t g, uint8_t b);
	RGBColour(mote::data::Pixel *pixel);
	RGBColour(const RGBColour& colour);
	RGBColour();
	virtual ~RGBColour();

	mote::data::Pixel* pixel();
	mote::data::Pixel* pixel() const;
	RGBColour& pixel(mote::data::Pixel* pixel, bool own=false);
	bool ownsPixel() const;

	/**
	 * Intensity of the colour (average).
	 *
	 * @return
	 */
	virtual double intensity();

	/**
	 * Replaces the channel value only if the equivalent channel of the `colour` param passed is greater than the
	 * current value.
	 *
	 * @param colour Colour that will be used for comparision.
	 * @return This instance of the colour.
	 */
	virtual RGBColour& minimum(const RGBColour& colour);

	/**
	 * Replaces the channel value only if the equivalent channel of the `colour` param passed is smaller than the
	 * current value.
	 *
	 * @param colour Colour that will be used for comparision.
	 * @return This instance of the colour.
	 */
	virtual RGBColour& maximum(const RGBColour& colour);

	RGBColour& operator=(const RGBColour& colour);

	virtual bool operator==(const RGBColour& colour) const;
	virtual bool operator!=(const RGBColour& colour) const;

	RGBColour operator+(const RGBColour& colour);
	RGBColour operator-(const RGBColour& colour);
	RGBColour operator*(const RGBColour& colour);
	RGBColour operator/(const RGBColour& colour);

	virtual RGBColour &operator+=(const RGBColour& colour);
	virtual RGBColour &operator-=(const RGBColour& colour);
	virtual RGBColour &operator*=(const RGBColour& colour);
	virtual RGBColour &operator/=(const RGBColour& colour);

	virtual void fromJson(const Json::Value &json) override;

	virtual void toJson(Json::Value &json) const override;
};
}
}


#endif //MOTE_VISION_COLOUR_H
