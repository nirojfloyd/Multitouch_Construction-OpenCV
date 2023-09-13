#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>

class Blob {
public:
	Blob();
	Blob(int, cv::Point2f);
	cv::Point2f getPosition();
	void setPosition(cv::Point2f);
	int getId();
	bool operator==(const Blob&) const;
private:
	int id;
	cv::Point2f position;
	int size;
};