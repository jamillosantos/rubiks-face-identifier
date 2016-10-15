/**
 * @author J. Santos <jamillo@gmail.com>
 * @date July 25, 2016
 */

#ifndef MOTE_VISION_COLOUR_DEFINITION_H
#define MOTE_VISION_COLOUR_DEFINITION_H

#include "colour.h"

namespace mote
{
namespace data
{
/**
 * Colour range definition for aalab colour detection method.
 */
class ColourRange
	: public RGBColour
{
public:
	ColourRange(const uint8_t r, const uint8_t g, const uint8_t b, const int redGreen, const int redBlue, const int greenBlue);
	ColourRange(const uint8_t r, const uint8_t g, const uint8_t b);
	ColourRange(const Pixel &pixel);
	ColourRange(const ColourRange& colour);
	ColourRange(const RGBColour& colour);
	ColourRange();

	/**
	 * Red - green.
	 */
	int redGreen;

	/**
	 * Red - blue.
	 */
	int redBlue;

	/**
	 * Green - blue.
	 */
	int greenBlue;

	virtual RGBColour& minimum(const RGBColour &colour) override;
	virtual RGBColour& maximum(const RGBColour &colour) override;

	virtual void update();

	ColourRange operator+(const ColourRange& colour);
	ColourRange operator-(const ColourRange& colour);
	ColourRange operator*(const ColourRange& colour);
	ColourRange operator/(const ColourRange& colour);

	virtual ColourRange& operator+=(const ColourRange& colour);
	virtual ColourRange& operator-=(const ColourRange& colour);
	virtual ColourRange& operator*=(const ColourRange& colour);
	virtual ColourRange& operator/=(const ColourRange& colour);

	virtual void toJson(Json::Value &json) const override;

	virtual void fromJson(const Json::Value &json) override;
};

class ColourDefinition
	: public Serializable
{
public:
	ColourDefinition();
	ColourDefinition(const ColourDefinition& definition);
	ColourDefinition(const ColourRange& min, const ColourRange& max);
	ColourDefinition(const ColourRange&& min, const ColourRange&& max);

	ColourRange min;
	ColourRange max;

	bool operator==(const ColourDefinition &colourDefinition) const;

	bool operator!=(const ColourDefinition &colourDefinition) const;

	bool isMatch(const mote::data::Pixel &p) const;
	bool isMatch(const mote::data::RGBColour &colour) const;

	void addPixel(const mote::data::Pixel& pixel);
	void addPixel(const mote::data::RGBColour& colour);

	virtual void fromJson(const Json::Value &json) override;
	virtual void toJson(Json::Value &json) const override;
};
}
}

#endif //MOTE_VISION_COLOUR_DEFINITION_H
