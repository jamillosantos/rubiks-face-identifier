/**
 * @author J. Santos <jamillo@gmail.com>
 * @date July 24, 2016
 */

#ifndef MOTE_VISION_DATA_SERIALIZABLE_H
#define MOTE_VISION_DATA_SERIALIZABLE_H

#include <json/value.h>

namespace mote
{
namespace data
{
/**
 * Interface that describes the method signatures for JSON de/serialization.
 */
class Serializable
{
public:
	/**
	 * Deserialize the data from a Json value.
	 * @param json Json that will be used as source of data.
	 */
	virtual void fromJson(const Json::Value &json) = 0;

	/**
	 * Serializes the data to a JSON value.
	 * @param json Json that will be used as desteny of data.
	 */
	virtual void toJson(Json::Value &json) const = 0;
};
}
}

#endif //MOTE_VISION_DATA_SERIALIZABLE_H
