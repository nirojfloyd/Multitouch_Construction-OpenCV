#include "Blob.h"
using namespace cv;

Blob::Blob() {
}

Blob::Blob(int i, Point2f p) {
	id = i;
	position = p;
}

Point2f Blob::getPosition() {
	return position;
}

void Blob::setPosition(Point2f newPos) {
	position = newPos;
}

int Blob::getId() {
	return id;
}

bool Blob::operator==(const Blob& t) const
{
	return position == t.position;
}
