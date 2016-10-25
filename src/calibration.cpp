/**
 * @author J. Santos <jamillo@gmail.com>
 * @date October 25, 2016
 */

#include <data/configuration.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <procs/device.h>
#include <cv.hpp>
#include <json/json.h>
#include <fstream>
#include <procs/gridtracer.h>

struct MouseHandleData
{
	bool mouseDown;

	data::configuration::GridDefinition &gridDefinition;
	procs::GridTracer gridTracer;

	std::vector<cv::Point2i> points;

	MouseHandleData(data::configuration::GridDefinition &gridDefinition)
		: mouseDown(false), points(9), gridDefinition(gridDefinition), gridTracer(gridDefinition)
	{ }

};

void mouseHandler(int event, int x, int y, int flags, void *data)
{
	MouseHandleData* mouseHandleData = (MouseHandleData*)data;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		mouseHandleData->mouseDown = true;
		mouseHandleData->gridDefinition.center.x = x;
		mouseHandleData->gridDefinition.center.y = y;
	}
	else if (
		(
			(event == CV_EVENT_LBUTTONUP)
			|| (event == CV_EVENT_MOUSEMOVE)
		) && (mouseHandleData->mouseDown))
	{
		if (event == CV_EVENT_LBUTTONUP)
			mouseHandleData->mouseDown = false;

		mouseHandleData->gridDefinition.corner.x = x;
		mouseHandleData->gridDefinition.corner.y = y;

		mouseHandleData->gridTracer.trace(mouseHandleData->points);

		if (event == CV_EVENT_LBUTTONUP)
		{
			Json::Value json;
			mouseHandleData->gridDefinition.toJson(json);
			std::cout << json << std::endl;
		}
	}
}

int main(int argc, char *argv[])
{
	namespace po = boost::program_options;

	std::string configFile;
	data::Configuration configuration;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "show this help message")
		("config,c", po::value<std::string>(&configFile), "Configuration file")
		("video,v", po::value<std::string>(&configuration.camera.camera), "Specify the camera");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::cout << desc;
		return 0;
	}
	if (vm.count("config"))
	{
		Json::Reader reader;
		Json::Value jsonConfig;
		std::cout << "Loading configuration from: " << configFile << std::endl;
		std::ifstream configStream(configFile);
		if (configStream.good())
		{
			reader.parse(configStream, jsonConfig);
			configuration.fromJson(jsonConfig);
			Json::Value jsonConfigLoaded;
			configuration.toJson(jsonConfigLoaded);
			std::cerr << jsonConfigLoaded << std::endl;
		}
		else
		{
			std::cerr << "Cannot load configuration file: " << configFile << std::endl;
			return 1;
		}
	}

	std::unique_ptr<procs::Device> device(procs::DeviceFactory::create(configuration.camera));
	if (device->open())
	{
		char key;
		cv::Mat image, out;
		cv::namedWindow("original");
		cv::moveWindow("original", 100, 100);

		MouseHandleData mouseHandleData(configuration.grid);
		mouseHandleData.gridTracer.trace(mouseHandleData.points);

		cv::setMouseCallback("original", mouseHandler, &mouseHandleData);

		cv::namedWindow("out");
		cv::moveWindow("out", configuration.camera.width + 100, 100);

		cv::Scalar
			lineColour(0, 0, 255),
			circleColour(0, 255, 0);

		cv::Mat rotationMatrix;

		cv::Point2i ptDiff;
		cv::Vec3d *tmp;
		cv::Mat identity = cv::getRotationMatrix2D(cv::Point(0, 0), 0, 1);

		while (key != 'q')
		{
			device->read(image);
			out = image.clone();

			cv::line(out, mouseHandleData.gridDefinition.center, mouseHandleData.gridDefinition.corner, lineColour);
			for (auto &p : mouseHandleData.points)
				cv::circle(image, p, 5, circleColour);

			cv::imshow("original", image);
			cv::imshow("out", out);

			key = cv::waitKey(20);
		}
	}
	else
		std::cerr << "Cannot open '" << configuration.camera.camera << "'" << std::endl;
}