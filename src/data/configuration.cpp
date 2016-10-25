/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#include <iostream>
#include "configuration.h"

void data::Configuration::fromJson(const Json::Value &json)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (it.name() == "cubeSize")
			this->cubeSize = it->asDouble();
		else if (it.name() == "distance")
			this->distance = it->asDouble();
		else if (it.name() == "camera")
			this->camera.fromJson(*it);
		else if (it.name() == "grid")
			this->grid.fromJson(*it);
		// TODO: Throw an exception
		/*
		else
			throw boost::exception(std::string("Property not expected: ") + it.name() + ".");
		*/
	}
}

void data::Configuration::toJson(Json::Value &json)
{
	json["cubeSize"] = this->cubeSize;
	json["distance"] = this->distance;
	this->camera.toJson(json["camera"]);
	this->grid.toJson(json["grid"]);
}

void data::configuration::Camera::fromJson(const Json::Value &json)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (it.name() == "camera")
			this->camera = it->asString();
		else if (it.name() == "width")
			this->width = it->asInt();
		else if (it.name() == "height")
			this->height = it->asInt();
		// TODO: Throw an exception
		/*
		else
			throw boost::exception(std::string("Property not expected: ") + it.name() + ".");
		*/
	}
}

void data::configuration::Camera::toJson(Json::Value &json)
{
	json["camera"] = this->camera;
	json["width"] = this->width;
	json["height"] = this->height;
}

void data::configuration::GridDefinition::fromJson(const Json::Value &json)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (it.name() == "center")
			data::configuration::fromJson(*it, this->center);
		else if (it.name() == "corner")
			data::configuration::fromJson(*it, this->corner);
		// TODO: Throw an exception
		/*
		else
			throw boost::exception(std::string("Property not expected: ") + it.name() + ".");
		*/
	}
}

void data::configuration::GridDefinition::toJson(Json::Value &json)
{
	data::configuration::toJson(this->center, json["center"]);
	data::configuration::toJson(this->corner, json["corner"]);
}

void ::data::configuration::fromJson(const Json::Value &json, cv::Point2i &point)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (it.name() == "x")
			point.x = it->asInt();
		else if (it.name() == "y")
			point.y = it->asInt();
		// TODO: Throw an exception
		/*
		else
			throw boost::exception(std::string("Property not expected: ") + it.name() + ".");
		*/
	}
}

void ::data::configuration::toJson(const cv::Point2i &point, Json::Value &json)
{
	json["x"] = point.x;
	json["y"] = point.y;
}
