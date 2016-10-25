/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#include "device.h"

procs::ImageFileDevice::ImageFileDevice(const std::string &_file)
	: _file(_file)
{ }

bool procs::ImageFileDevice::read(cv::Mat &frame)
{
	frame = cv::imread(this->_file);
	return frame.data;
}

void procs::ImageFileDevice::release()
{ }

bool procs::ImageFileDevice::open()
{
	return true;
}

procs::CameraDevice::CameraDevice(const data::configuration::Camera &camera)
	: _camera(camera)
{ }

bool procs::CameraDevice::read(cv::Mat &frame)
{
	if (this->_video)
		return this->_video->read(frame);
	else
		return false;
}

void procs::CameraDevice::release()
{
	if (this->_video)
		this->_video->release();
}

bool procs::CameraDevice::open()
{
	this->_video.reset(new cv::VideoCapture());
	try
	{
		boost::regex r("^(/dev/video)?(\\d+)$");
		boost::smatch match;
		if (boost::regex_search(this->_camera.camera, match, r))
		{
			int i = boost::lexical_cast<int>(match[2].str());
			if (this->_video->open(i))
			{
				this->_video->set(CV_CAP_PROP_FRAME_WIDTH, this->_camera.width);
				this->_video->set(CV_CAP_PROP_FRAME_HEIGHT, this->_camera.height);
				return true;
			}
		}
	}
	catch (boost::bad_lexical_cast&)
	{ }

	return false;
}

procs::Device *procs::DeviceFactory::create(const data::configuration::Camera &camera)
{
	if (boost::filesystem::is_regular_file(camera.camera) && (!boost::algorithm::starts_with(camera.camera, "/dev")))
		return new ImageFileDevice(camera.camera);
	else
		return new CameraDevice(camera);
	return nullptr;
}
