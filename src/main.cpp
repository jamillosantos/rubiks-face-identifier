#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <procs/segmentcolour.h>
#include <bits/unique_ptr.h>
#include <procs/gridfinder.h>

class Visitor
	: public procs::SegmentColourVisitor
{
public:
	virtual bool matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const override
	{
		if (
			std::abs(
				((base.val[0] + base.val[1] + base.val[2]) / 3.0) -
				((neighbour.val[0] + neighbour.val[1] + neighbour.val[2]) / 3.0)
			) / 255.0 > 0.02
			)
			return false;
		cv::Vec3b diff = (base - neighbour);
		unsigned int max = 0;
		for (unsigned int i = 1; i < 3; i++)
		{
			if (base.val[max] < base.val[i])
				max = i;
		}
		cv::Vec3d
			baseRatio = (base / base.val[max]),
			neightbourRatio = (neighbour / neighbour.val[max]);
		for (unsigned int i = 0; i < 3; i++)
		{
			if (std::abs(baseRatio.val[i] - neightbourRatio.val[i]) > 0.1)
				return false;
		}
		return true;
	}
};

class VisitorGreenWeight
	: public procs::SegmentColourVisitor
{
public:
	virtual bool matches(const cv::Vec3b &base, const cv::Vec3b &neighbour) const override
	{
		double
			rbRatio = std::abs(((base.val[0] + base.val[2]) / 2.0) - ((neighbour.val[0] + neighbour.val[2]) / 2.0)) / 255.0,
			gRatio = std::abs(base.val[1] - neighbour.val[1])/255.0;
		if ((rbRatio > 0.03) || (gRatio > 0.1))
			return false;
		/*
		cv::Vec3b diff = (base - neighbour);
		unsigned int max = 0;
		for (unsigned int i = 1; i < 3; i++)
		{
			if (base.val[max] < base.val[i])
				max = i;
		}
		cv::Vec3d
			baseRatio = (base / base.val[max]),
			neightbourRatio = (neighbour / neighbour.val[max]);
		for (unsigned int i = 0; i < 3; i++)
		{
			if (std::abs(baseRatio.val[i] - neightbourRatio.val[i]) > 0.1)
				return false;
		}*/
		return true;
	}
};

int main(int argc, char *argv[])
{
	cv::Mat image;

	bool isVideo = (argc < 2);

	std::unique_ptr<cv::VideoCapture> video;
	if (isVideo)
	{
		video.reset(new cv::VideoCapture(1));
		video->set(CV_CAP_PROP_FRAME_WIDTH, 320);
		video->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	}

	cv::namedWindow("Original");
	cv::namedWindow("Converted");
	cv::namedWindow("Debug");

	if (isVideo)
		video->read(image);
	else
		image = cv::imread(argv[1]);

	cv::Mat out(image.rows, image.cols, image.type());
	cv::Mat debugMat(image.rows, image.cols, image.type());

	procs::SegmentColour segmentColour(image.rows * image.cols);
	VisitorGreenWeight visitor;
	std::vector<procs::SegmentationColourObject> results;

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

		segmentColour.action(image, out, 500, 10, 25, 1, visitor, results, debugMat);

		std::cout << results.size() << " results found." << std::endl;
		unsigned int n = 0;
		for (const procs::SegmentationColourObject &obj : results)
		{
			cv::rectangle(out, obj.bBox, cv::Scalar(255, 0, 0));
			// cv::putText(out, std::to_string(n++), obj.center, CV_FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(255, 255, 255));
		}

		procs::GridFinder finder;
		std::vector<procs::SegmentationColourObject*> grid;
		finder.find(results, grid);
		for (const procs::SegmentationColourObject *obj : grid)
		{
			cv::rectangle(out, obj->bBox, cv::Scalar(0, 255, 0));
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