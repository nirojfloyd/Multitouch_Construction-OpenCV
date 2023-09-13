#pragma once

#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <vector>
#include <algorithm>
#include "Blob.h"
#include "TuioServer.h"
#include "TuioCursor.h"
#include "TuioTime.h"


class Tracking {
public:
	Tracking(double, double, TUIO::TuioServer&, int);
	std::vector<Blob> TrackFinger(std::vector< cv::Point2f>);
	void findNearestNeighbour(Blob, std::vector< cv::Point2f>&);
	int getCounter();
	~Tracking();

private:
	double maxDist;
	int counter;
	std::vector<Blob> pastBlobs;
	double videoWidth;
	double videoHeight;
	TUIO::TuioServer* server;
	int currentFrame;
	TUIO::TuioCursor* lastCursor;
	
};