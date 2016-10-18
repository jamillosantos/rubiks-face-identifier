#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class ColourDefinition
{
private:
	std::vector<cv::Vec3b> colours;
public:
	cv::Vec3d ratio;
	cv::Vec3d ratioError;
	uint8_t ratioIndex;


	cv::Vec3b min;
	cv::Vec3b max;

	cv::Vec3d ratioMax;
	cv::Vec3d ratioMin;

	cv::Vec3d sum;
	cv::Vec3d avg;

	cv::Vec3d colour;
	cv::Vec3d error;

	ColourDefinition()
		: ratio(0, 0, 0), ratioError(0, 0, 0), min(255, 255, 255), max(0, 0, 0), ratioMin(1, 1, 1), ratioMax(0, 0, 0),
		  sum(0, 0, 0), avg(0, 0, 0), colour(1, 1, 1), error(0, 0, 0)
	{ }

	void addColour(const cv::Vec3b &colour)
	{
		this->colours.push_back(colour);
		this->sum += colour;
		uint8_t max = 0;
		for (uint8_t i = 0; i < 3; i++)
		{
			this->avg.val[i] = (this->sum.val[i] / this->colours.size());
			if (colour.val[i] < this->min.val[i])
				this->min.val[i] = colour.val[i];
			if (colour.val[i] > this->max.val[i])
				this->max.val[i] = colour.val[i];
			this->error.val[i] = (this->max.val[i] - this->min.val[i]) / 2;
			this->colour.val[i] = this->min.val[i] + this->error.val[i];
			if (this->colour.val[i] > this->colour.val[max])
				max = i;
		}
		this->ratioIndex = max;
		for (uint8_t i = 0; i < 3; i++)
		{
			this->ratio.val[i] = this->colour.val[i] / this->colour.val[max];
			if (this->ratio.val[i] < this->ratioMin.val[i])
				this->ratioMin.val[i] = this->ratio.val[i];
			if (this->ratio.val[i] > this->ratioMax.val[i])
				this->ratioMax.val[i] = this->ratio.val[i];
			this->ratioError.val[i] = (this->ratioMax.val[i] - this->ratioMin.val[i])/2.0;
			this->ratio.val[i] = this->ratioMin.val[i] + this->ratioError.val[i];
		}
	}

	bool matches(const cv::Vec3b &colour)
	{
		double r;
		for (uint8_t i = 0; i < 3; ++i)
		{
			r = std::abs((colour.val[i]/(double)((colour.val[this->ratioIndex] == 0) ? 1 : colour.val[this->ratioIndex])) - (this->ratio.val[i]));
			if (r > this->ratioError.val[i])
				return false;
		}
		/*
		for (uint8_t i = 0; i < 3; ++i)
		{
			if (std::abs(colour.val[i] - this->colour.val[i]) > this->error.val[i])
				return false;
		}
		*/
		return true;
	}
};

struct WindowClickHandlerData
{
	ColourDefinition *cd;
	cv::Mat *image;
	bool mouseDown;
};

void colourRatio(const cv::Vec3b &in, cv::Vec3d &out)
{
	out.val[0] = in.val[0]/255.0;
	out.val[1] = in.val[1]/255.0;
	out.val[2] = in.val[2]/255.0;
}

double colourAvgNormalized(const cv::Vec3b &in)
{
	return ((in.val[0] + in.val[1] + in.val[2]) / 3.0) / 255.0;
}

void clickHandler(int event, int x, int y, int flags, void *userdata)
{
	WindowClickHandlerData *handler = (WindowClickHandlerData *) userdata;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		handler->mouseDown = true;
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		handler->mouseDown = false;

		std::cout << "    Pt: " << x << ":" << y << std::endl;
		std::cout << "Colour: " << handler->cd->ratio << std::endl;
		std::cout << " error: " << handler->cd->ratioError << std::endl;
		std::cout << "   min: " << handler->cd->ratioMin << std::endl;
		std::cout << "   max: " << handler->cd->ratioMax << std::endl;
	}

	if (
		handler->mouseDown
		&& (
			(event == CV_EVENT_MOUSEMOVE)
			|| (event == CV_EVENT_LBUTTONDOWN)
		)
	)
	{
		cv::Vec3b px = handler->image->at<cv::Vec3b>(y, x);
		cv::Vec3d pixel;
		colourRatio(px, pixel);
		handler->cd->addColour(px);
	}
}

int main(int argc, char *argv[])
{
	cv::Mat image;
	image = cv::imread(std::string(argv[1]), CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data) // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cv::resize(image, image, cv::Size(320, 320));

	cv::namedWindow("Original");
	imshow("Original", image);

	cv::Mat converted(image.rows, image.cols, image.type());
	cv::namedWindow("Converted");

	ColourDefinition cd;
	WindowClickHandlerData windowClickHandlerData;
	windowClickHandlerData.cd = &cd;
	windowClickHandlerData.image = &image;
	windowClickHandlerData.mouseDown = false;
	cv::setMouseCallback("Original", clickHandler, &windowClickHandlerData);

	char key;
	cv::Vec3d cr;
	cv::Vec3b *px, *pxC;
	double avg;
	while (key != 'q')
	{
		if (!windowClickHandlerData.mouseDown)
		{
			converted.zeros(image.rows, image.cols, image.type());
			for (unsigned int y = 0; y < image.rows; y++)
			{
				px = image.ptr<cv::Vec3b>(y);
				pxC = converted.ptr<cv::Vec3b>(y);
				for (unsigned int x = 0; x < image.cols; x++, ++px, ++pxC)
				{
					if ((x == 54) && (y == 168))
					{
						std::cout << "START DEBUGGER" << std::endl;
					}
					if (cd.matches(*px))
					{
						pxC->val[0] = cd.avg.val[0];
						pxC->val[1] = cd.avg.val[1];
						pxC->val[2] = cd.avg.val[2];
					}
				}
			}
			imshow("Converted", converted);
		}
		key = cv::waitKey(0);
		if (key == 'c')
		{
			//
		}
	}

	return 0;
}