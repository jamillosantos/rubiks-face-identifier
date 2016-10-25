/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#ifndef RUBIKS_CONFIGURATION_H
#define RUBIKS_CONFIGURATION_H

#include <json/value.h>
#include <opencv2/core/types.hpp>

namespace data
{

namespace configuration
{

class ConfigurationPart
{
public:
	virtual void fromJson(const Json::Value &json) = 0;

	virtual void toJson(Json::Value &json) = 0;
};

class Camera
	: public ConfigurationPart
{
public:
	std::string camera;
	int width;
	int height;

	virtual void fromJson(const Json::Value &json) override;

	virtual void toJson(Json::Value &json) override;
};

class GridDefinition
	: public ConfigurationPart
{
public:
	cv::Point2i center;
	cv::Point2i corner;

	virtual void fromJson(const Json::Value &json) override;

	virtual void toJson(Json::Value &json) override;
};

void fromJson(const Json::Value &json, cv::Point2i &point);
void toJson(const cv::Point2i &point, Json::Value &json);
}

class Configuration
	: public configuration::ConfigurationPart
{
public:
	double cubeSize;
	double distance;
	configuration::Camera camera;
	configuration::GridDefinition grid;

	virtual void fromJson(const Json::Value &json) override;

	virtual void toJson(Json::Value &json) override;
};
}

#endif //RUBIKS_CONFIGURATION_H
