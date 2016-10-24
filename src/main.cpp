
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include <json/json.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <procs/segmentcolour.h>
#include <procs/gridfinder.h>
#include <procs/visitors.h>
#include <procs/colourmatches.h>

void mouseCallbackHandler(int event, int x, int y, int flags, void* userdata)
{
	if (event == CV_EVENT_LBUTTONDOWN)
		std::cout << "(" << x << ", " << y << ")" << std::endl;
}

int main(int argc, char *argv[])
{
	cv::Mat image;
	Json::Value jsonOptions;

	std::vector<cv::Point2i> gridPoints;

	bool isVideo = (argc < 2) || (strlen(argv[1]) < 2);
	std::unique_ptr<cv::VideoCapture> video;
	if (isVideo)
	{
		int videoIndex = 0;
		if (argc > 1)
			videoIndex = boost::lexical_cast<int>(argv[1]);
		video.reset(new cv::VideoCapture(videoIndex));
		video->set(CV_CAP_PROP_FRAME_WIDTH, 320);
		video->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
		video->read(image);
	}
	else
	{
		std::string imagePath(std::string(argv[1]) + ".png");
		std::string jsonPath(std::string(argv[1]) + ".json");

		std::cout << "Loading: '" << imagePath << "'." << std::endl;
		if (boost::filesystem::exists(imagePath))
			image = cv::imread(imagePath);
		else
			std::cerr << "'" << imagePath << "' does not exists." << std::endl;
		Json::Reader reader;
		std::ifstream streamOptions(jsonPath);
		if (streamOptions.good())
			reader.parse(streamOptions, jsonOptions, false);
		else
		{
			std::cerr << "'" << jsonPath << "' does not exists." << std::endl;
		}
	}

	Json::Value &jsonCoords = jsonOptions["coords"];

	cv::namedWindow("Original");
	cv::namedWindow("Converted");
	cv::namedWindow("Debug");

	cv::setMouseCallback("Original", mouseCallbackHandler, nullptr);

	cv::Mat out(image.rows, image.cols, image.type());
	cv::Mat debugMat(image.rows, image.cols, image.type());

	procs::SegmentColour segmentColour(image.rows * image.cols);
	procs::SegmentColourRGWeightGWeightVisitor visitor(cv::Vec2d(0.02, 0.05));
	std::vector<procs::SegmentationColourObject> results;
	std::vector<procs::SegmentationColourObject*> resultsFiltered;

	char key;

	unsigned int seq = 0;

	for (unsigned int i = 0; isVideo || (i < 1); ++i)
	{
		if (isVideo)
		{
			results.clear();
			video->read(image);
		}

		out.setTo(cv::Scalar(0));
		debugMat.setTo(cv::Scalar(0));

		segmentColour.action(image, out, 500, 10, 100, 1, visitor, results, debugMat);
		std::cout << results.size() << " results found." << std::endl;
		resultsFiltered.clear();
		for (procs::SegmentationColourObject &obj : results)
		{
			if (
				(std::abs(1.0 - (obj.bBox.height/(double)obj.bBox.width)) <= 0.1) // Proportion
				&& (std::abs(1.0 - (obj.size / (double)obj.bBox.area())) <= 0.05) // Area
			)
				resultsFiltered.push_back(&obj);
		}
		std::cout << resultsFiltered.size() << " filtered." << std::endl;
		std::sort(resultsFiltered.begin(), resultsFiltered.end(), [](procs::SegmentationColourObject* a, procs::SegmentationColourObject* b) {
			return a->bBox.area() > b->bBox.area();
		});
		unsigned int n = 0;
		for (const procs::SegmentationColourObject *obj : resultsFiltered)
		{
			cv::rectangle(out, obj->bBox, cv::Scalar(255, 0, 0));
		}

		cv::Scalar green(0, 255, 0);
		cv::Vec3b *px;
		std::vector<procs::SegmentationColourObject*> grid;
		std::vector<procs::SegmentationColourObject*> gridTmp;
		for (Json::ValueConstIterator it = jsonCoords.begin(); it != jsonCoords.end(); ++it)
		{
			cv::Point2i pt((*it)["x"].asInt(), (*it)["y"].asInt());
			cv::circle(out, pt, 5, green);
			px = out.ptr<cv::Vec3b>(pt.y, pt.x);
			px->val[0] = 0;
			px->val[1] = 255;
			px->val[2] = 0;
			gridTmp.clear();
			for (
				std::vector<procs::SegmentationColourObject*>::reverse_iterator it = resultsFiltered.rbegin();
				it != resultsFiltered.rend();
				++it
			)
			{
				if ((*it)->bBox.contains(pt))
				{
					grid.push_back(*it);
					break;
				}
			}
		}

		/*
		procs::GridFinder finder;
		finder.find(results, grid);
		 */
		for (const procs::SegmentationColourObject *obj : grid)
		{
			cv::rectangle(out, obj->bBox, cv::Scalar(0, 255, 0));
			cv::putText(out, std::to_string(n++), obj->center, CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
		}

		/*
		procs::SegmentColourRBByGProportionVisitor proportionVisitor(0.3);
		if (proportionVisitor.matches(grid[1]->average, grid[8]->average))
			std::cout << "It matches!" << std::endl;
		else
			std::cout << "Does not match!" << std::endl;
		*/

		procs::ColourMatches colourMatches;
		std::vector<procs::SegmentationColourObjectGroup> groups;
		colourMatches.match(grid, groups);
		n = 0;
		for (const procs::SegmentationColourObjectGroup &group : groups)
		{
			for (const procs::SegmentationColourObject* obj : group.objects)
			{
				cv::putText(image, std::to_string(n), obj->center, CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
			}
			++n;
		}

		cv::imshow("Original", image);
		cv::imshow("Converted", out);
		cv::imshow("Debug", debugMat);

		if (isVideo)
		{
			key = cv::waitKey(20);
			if (key == '\n')
			{
				seq ++;
				cv::imwrite(std::to_string(seq) + ".png", image);
				cv::imwrite(std::to_string(seq) + "_out.png", out);
				cv::imwrite(std::to_string(seq) + "_debug.png", debugMat);
			}
			else if (key == 'q')
				break;
		}
		else
			cv::waitKey(0);
	}
	return 0;
}