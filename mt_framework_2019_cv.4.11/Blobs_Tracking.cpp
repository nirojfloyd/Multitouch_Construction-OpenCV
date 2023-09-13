#include "Tracking.h"
using namespace std;
using namespace cv;


//Default constructor

Tracking::Tracking(double w, double h, TUIO::TuioServer& s, int c)
{
	counter = 1;
	maxDist = 35;
	videoWidth = w;
	videoHeight = h;
	server = &s;
	currentFrame = c;
}


//Track fingers
vector<Blob> Tracking::TrackFinger(vector<Point2f> fingers)
{
	//find the nearest neighbour of finger points from past frame contours to current ones
	for (int i = 0; i < pastBlobs.size(); i++) {
		findNearestNeighbour(pastBlobs[i], fingers);
	}

	// if the size of current frame points(fingers) is higher than , add them to pastBlobs
	if (fingers.size() > pastBlobs.size()) {
		for (Point2f finger : fingers) {
			Blob t(counter, finger);
			pastBlobs.push_back(t);
			
			TUIO::TuioCursor* tcur = new TUIO::TuioCursor(t.getId(), t.getId(),
				t.getPosition().x / videoWidth, t.getPosition().y / videoHeight);
			server->addExternalTuioCursor(tcur);
			counter++;
		}
	}
	return pastBlobs;
}

void Tracking::findNearestNeighbour(Blob t, vector<Point2f>& fingers) {
	Point2f maxim(9999999, 9999999);
	pair<Point2f, int> nearestNeighbor(maxim, 9999999);
	double distance = 0;

	for (Point2f finger : fingers) {

		//using the norm function from opencv to calculate distance between points
		distance = norm(t.getPosition() - finger);
		if (distance <= maxDist && distance <= nearestNeighbor.second) {
			nearestNeighbor = make_pair(finger, distance);
		}
	}

	// if the point is actually the max point then just remove it from the pastBlobs vector
	if (nearestNeighbor.first == maxim) {
		pastBlobs.erase(remove(pastBlobs.begin(), pastBlobs.end(), t));
		
		server->removeTuioCursor(server->getTuioCursor(t.getId()));

		return;
	}

	auto itr = find(pastBlobs.begin(), pastBlobs.end(), t);
	int x = std::distance(pastBlobs.begin(), itr);
	pastBlobs[x].setPosition(nearestNeighbor.first);
	server->updateTuioCursor(server->getTuioCursor(
		pastBlobs[x].getId()),
		pastBlobs[x].getPosition().x / videoWidth,
		pastBlobs[x].getPosition().y / videoHeight
	);

	fingers.erase(remove(fingers.begin(), fingers.end(), nearestNeighbor.first));

}


Tracking::~Tracking()
{
	server->~TuioServer();
	
}

int Tracking::getCounter()
{
	return counter;
}
