#pragma once
#include "Triangle.cpp"
class Mesh : public Geometry {
public:
	Mesh() { ; }
	vector<Triangle> tris;
	Point center;
	float scale;

	Point norm(Point& p) {
		return p;
	}

	// intersect on a circle is minimal
	bool boundingBoxIntersect(Point& o, Point& dir, vector<Geometry*>& boundedList) {
		bool found = false;
		for (auto t : tris) {
			if (t.boundingBoxIntersect(o, dir, boundedList)) {
				found = true;
			}
		}
		return found;
	};

	inline Mesh operator=(const Mesh& a) {
		tris.clear();
		center = a.center;
		for (auto t : a.tris) {
			tris.push_back(t);
		}
		return *this;
	}

	inline Mesh operator*(const float& a) {
		scale = scale * a;
		for (int i = 0; i < tris.size(); ++i) {
			tris[i].p1 = tris[i].p1 * a;
			tris[i].p2 = tris[i].p2 * a;
			tris[i].p3 = tris[i].p3 * a;
		}
		return *this;
	}

	inline Mesh operator+(const Point& a) {
		center = center + a;
		for (int i = 0; i < tris.size(); ++i) {
			tris[i].p1 = tris[i].p1 + a;
			tris[i].p2 = tris[i].p2 + a;
			tris[i].p3 = tris[i].p3 + a;
		}
		return *this;
	}

	inline Mesh rotate(float degrees) {
		float invScale = 1.0 / scale;
		Point invCenter = center * -1.0;
		for (int i = 0; i < tris.size(); ++i) {
			tris[i].p1 = (tris[i].p1 + invCenter) * invScale;
			tris[i].p2 = (tris[i].p2 + invCenter) * invScale;
			tris[i].p3 = (tris[i].p3 + invCenter) * invScale;
		}
		return *this;
	}
};