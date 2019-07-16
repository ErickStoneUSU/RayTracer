#pragma once
#include "Triangle.cpp"
class Mesh : public Geometry {
public:
	Mesh() { ; }
	Mesh(Point cent, Point rotat, Color col) { center = cent; rotation = rotat; color = col; }
	Point center;
	Point rotation; // todo decide how to handle this
	vector<Triangle> tris;
	Circle boundingBox;
	float intensity;

	Point norm(Point& p) {
		return p;
	}

	void setBoundingBox() {
		float tempRadius = 0;
		for (auto t : tris) {
			float d1 = (t.p1 - center).magnitude();
			float d2 = (t.p2 - center).magnitude();
			float d3 = (t.p3 - center).magnitude();
			if (d1 > tempRadius) {
				tempRadius = d1;
			}
			if (d2 > tempRadius) {
				tempRadius = d2;
			}
			if (d3 > tempRadius) {
				tempRadius = d3;
			}
		}
		boundingBox.center = center;
		boundingBox.radius = tempRadius;
	}

	// intersect on a circle is minimal
	bool boundingBoxIntersect(Point& o, Point& dir) {
		return boundingBox.boundingBoxIntersect(o, dir);
	};

	bool intersect(Point& o, Point& dir, float& distance, Triangle & contactObj, Point & contactPoint, Point & surfaceNormal) {
		// find intersection in list of triangles
		
		float dist = 999999;
		Triangle g;
		for (Triangle t : tris) {
			float temp = 999999;
			Point tempPoint;
			
			if (t.intersect(o, dir, temp, g, tempPoint, surfaceNormal)) {
				if (temp < dist) {
					dist = temp;
					contactPoint = tempPoint;
					contactObj = g;
				}
			}
		}

		if (dist == 999999) {
			return false;
		}

		return true;
	};
};