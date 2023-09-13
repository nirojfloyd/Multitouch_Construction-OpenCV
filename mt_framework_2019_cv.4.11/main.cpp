#include "Tracking.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main()
{
	Mat cannyImage;
	Mat background;
	Mat frame, original, blurred;
	Mat result;
	int currentFrame = 0; // frame counter
	multimap <int, pair<int, Point2f>> foundObjects; // stores the blobs found


	// fetches the video path
	String pathToVideo = "C:\\Users\\Niroj\\Desktop\\Bachelors\\SoSe\\MultitouchConstruction\\multi - touch(Final)\\mt_camera_raw.AVI";
	VideoCapture video(pathToVideo);


	if (!video.isOpened())
	{
		std::cout << "ERROR: Unable to open the video\n";
		return -1;
	}
	double videoWidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
	double videoHeight = video.get(cv::CAP_PROP_FRAME_HEIGHT);



	video >> background; // get the first frame as background

	TUIO::TuioServer* server = new TUIO::TuioServer();
	Tracking t(videoWidth, videoHeight, *server, currentFrame);  // An object of class Tracking, used in tracking blobs
	vector<Blob> trackedTouches; // tracked touches in vector Blob

	//count frames
	int count = 0;
	while (true) {


		video >> frame; // get a new frame from the video

		if (frame.data == NULL) // terminate the program if there are no more frames
		{
			std::cout << "TERMINATION: Video stream stopped or last frame of video reached.\n";
			break;
		}

		//increment count value
		count++;

		original = frame.clone();

		absdiff(original, background, result); //absolute difference
		blur(result, blurred, Size(17, 17)); // blur 
		absdiff(result, blurred, result);    // absolute differnce again
		//again blur for better result 
		blur(result, result, Size(6, 6));
		cvtColor(result, result, COLOR_BGR2GRAY);
		threshold(result, result, 10, 255, THRESH_BINARY);
		Canny(result, cannyImage, 50, 150);

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(result, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);

		// iterate through all the top-level contours -> "hierarchy" may not be empty!)

		vector<Moments> mu(contours.size());

		// stores the centre of current selected contour
		Point2f p;

		//used to store to touch points detected in each frame
		vector<Point2f> fingers;

		//Use of hierarchy loop
		if (hierarchy.size() > 0)
		{
			for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
			{
				// check contour size (number of points) and area ("blob" size)
				if ((contourArea(Mat(contours.at(idx))) > 10 && contourArea(Mat(contours.at(idx))) < 80) && contours.at(idx).size() > 4)
				{
					RotatedRect rr = fitEllipse(Mat(contours.at(idx)));

					ellipse(original, rr, Scalar(0, 155, 255), 1, 8); // fit & draw ellipse to contour at index
					drawContours(original, contours, idx, Scalar(0, 0, 255), 1, 8, hierarchy); // draw contour at index

					mu[idx] = moments(contours[idx], false);

					// from moments, centre can be extracted

					// x coordinate of centre point
					float cX = mu[idx].m10 / mu[idx].m00;

					// y coordinate of centre point
					float cY = mu[idx].m01 / mu[idx].m00;

					p = Point2f(cX, cY);
					// pair of (contourArea and point)
					pair<int, Point2f> pair = make_pair(contourArea(Mat(contours.at(idx))), p);

					//adding id and pair from previous line .i.e [id, [contourArea , point]]
					foundObjects.emplace(currentFrame, pair);

					//adding only point to fingers
					fingers.emplace_back(p);
				}
			}
		}
		trackedTouches = t.TrackFinger(fingers);
		server->initFrame(TUIO::TuioTime::getSessionTime());
		server->sendFullMessages();


		//writing blob ids on frame
		for (Blob to : trackedTouches) {
			putText(original, to_string(to.getId()), to.getPosition(), FONT_HERSHEY_SCRIPT_COMPLEX, 0.5, Scalar(255, 255, 255));
			putText(cannyImage, to_string(to.getId()), to.getPosition(), FONT_HERSHEY_SCRIPT_COMPLEX, 0.5, Scalar(255, 255, 255));
		}

		if (waitKey(1) == 27) // wait for user input
		{
			std::cout << "TERMINATION: User pressed ESC\n";
			break;
		}
		currentFrame++;

		putText(original, "Frame number:" + to_string(count), Point(100, 20), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);
		putText(cannyImage, "Frame number:" + to_string(count), Point(100, 20), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);

		imshow("Canny", cannyImage);
		imshow("Original", original);

	}
	std::cout << endl;
	std::cout << foundObjects.size() << " Objects found." << endl;
	std::cout << t.getCounter() << " Fingertouches detected." << endl;
	std::cout << "SUCCESS: Program terminated.\n";
	return 1;
}