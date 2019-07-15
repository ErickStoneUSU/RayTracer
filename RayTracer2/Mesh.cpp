#pragma once
#include "Triangle.cpp"
class Mesh : public Geometry {
public:
	Mesh() { ; }
	Mesh(Point cent, Point rotat, Color col) { center = cent; rotation = rotat; color = col; }
	Point center;
	Point rotation; // todo decide how to handle this
	Circle boundingBox;
	vector<Mesh*> ms;
	vector<Circle*> cirs;
	vector<Triangle*> tris;

	float intensity;
	bool initial = false;

	Point norm(Point& p) {
		return p;
	}

	void populateBoundingBox() {
		// get the radius from the furthest point
		// later get the radius from the furthest bounding box
		for (Geometry* g : geos) {
			if (Mesh * c = dynamic_cast<Mesh*>(g)) {
				c->populateBoundingBox();
				updateBoundingBoxByCircle(c->boundingBox);
			} else if (Triangle* c = dynamic_cast<Triangle*>(g)){
				vector<Point> furthestPoints;
				float furthestDistance = boundingBox.radius * 2;
				getFurthestTriangle(furthestPoints, *c, furthestDistance);
				if (!furthestPoints.empty()) {
					boundingBox.center = (furthestPoints[0] + furthestPoints[1]) / 2;
					boundingBox.radius = furthestDistance / 2;
				}
			} else if (Circle * c = dynamic_cast<Circle*>(g)) {
				updateBoundingBoxByCircle(*c);
			}
		}	
	}

	void updateBoundingBoxByCircle(Circle & c) {
		if (boundingBox.radius == 0) {
			boundingBox.radius = c.radius;
			boundingBox.center = c.center;
			return;
		}
		float dout = ((boundingBox.center - c.center).magnitude() + boundingBox.radius + c.radius);
		float d1in = boundingBox.radius * 2;
		float d2in = c.radius * 2;

		// one circle is outside the other
		if (dout > d1in) {
			boundingBox.radius = dout;
			boundingBox.center = (boundingBox.center + c.center) / 2;
		}
		else if (d2in > d1in) {
			boundingBox.radius = d2in;
			boundingBox.center = (boundingBox.center + c.center) / 2;
		} // otherwise, keep bounding box as c is completely inside
	}

	void getFurthestTriangle(vector<Point> & furthestPoints, Triangle & c, float & furthestDistance) {
		float p12 = (c.p1 - c.p2).magnitude();
		float p13 = (c.p1 - c.p3).magnitude();
		float p23 = (c.p2 - c.p3).magnitude();
		float c1 = (center - c.p1).magnitude() + boundingBox.radius; // its fine that this is a rough approximation
		float c2 = (center - c.p2).magnitude() + boundingBox.radius;
		float c3 = (center - c.p3).magnitude() + boundingBox.radius;

		if (p12 > furthestDistance) { furthestPoints = { c.p1, c.p2 }; furthestDistance = p12; }
		if (p13 > furthestDistance) { furthestPoints = { c.p1, c.p3 }; furthestDistance = p13; }
		if (p23 > furthestDistance) { furthestPoints = { c.p2, c.p3 }; furthestDistance = p23; }
		if (c1 > furthestDistance) { furthestPoints = { center, c.p1 }; furthestDistance = c1; }
		if (c2 > furthestDistance) { furthestPoints = { center, c.p2 }; furthestDistance = c2; }
		if (c3 > furthestDistance) { furthestPoints = { center, c.p3 }; furthestDistance = c3; }
	}

	// do any of the bounding boxes intersect?
	bool boundingBoxIntersect(Point& p, Point& ray, vector<Geometry *> & boundedList) {
		bool found = false;
		vector<Geometry*> tempList;
		if (initial || boundingBox.boundingBoxIntersect(p,ray, tempList)) {
			for (Geometry* g : geos) {
				if (g->boundingBoxIntersect(p, ray, boundedList)) {
					found = true;
				}
			}
		}

		return found;
	};
};