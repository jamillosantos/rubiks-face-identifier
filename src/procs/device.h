/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#ifndef RUBIKS_PROCS_DRIVER_H
#define RUBIKS_PROCS_DRIVER_H

#include <memory>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/videoio_c.h>

#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <data/configuration.h>

namespace procs
{
class Device
{
public:
	virtual bool open() = 0;

	virtual bool read(cv::Mat &frame) = 0;

	virtual void release() = 0;
};

class ImageFileDevice
	: public Device
{
private:
	std::string _file;
public:
	ImageFileDevice(const std::string &_file);

	virtual bool open() override;

	virtual bool read(cv::Mat &frame) override;

	virtual void release() override;
};

class CameraDevice
	: public Device
{
public:
	const data::configuration::Camera &_camera;

	std::unique_ptr<cv::VideoCapture> _video;
public:
	CameraDevice(const data::configuration::Camera &camera);

	virtual bool open() override;

	virtual bool read(cv::Mat &frame) override;

	virtual void release() override;
};

class DeviceFactory
{
public:
	static Device* create(const data::configuration::Camera& camera);
};
}


#endif //RUBIKS_PROCS_DRIVER_H
