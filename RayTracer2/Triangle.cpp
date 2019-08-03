#pragma once
#include "Circle.cpp"
#include <algorithm>
#include <tuple>
using namespace std;

class Triangle : public Geometry{
public:
	Triangle() { ; };
	Triangle(Point& a, Point& b, Point& c) { setup(a, b, c); };
	Triangle(Point& a, Point& b, Point& c, Color d) { setup(a, b, c); color = d; };
	
	Point p1;
	Point p2;
	Point p3;
	Point p12;
	Point p13;
	Point p21;
	Point p23;
	Point p31;
	Point p32;
	Point n;
	float dist;
	Circle boundingBox;
	Color color;
	bool hasTexture = false;
	Texture texture;

	void setTriangle(tuple<float, float, float>& a, const tuple<float, float, float>& b, const tuple<float, float, float>& c, Color col) {
		p1 = Point(get<0>(a), get<1>(a), get<2>(a));
		p2 = Point(get<0>(b), get<1>(b), get<2>(b));
		p3 = Point(get<0>(c), get<1>(c), get<2>(c));
		color = col;
	}

	void setup(Point& a, Point& b, Point& c) {
		p1 = a; p2 = b; p3 = c;
		p12 = p1 - p2;
		p13 = p1 - p3;
		p21 = p2 - p1;
		p23 = p2 - p3;
		p31 = p3 - p1;
		p32 = p3 - p2;
		n = (p31).cross(p21).norm();
		dist = n.dot(p1);

		Point center((p1.x + p2.x + p3.x) * (1/3), (p1.y + p2.y + p3.y) * (1/3), (p1.z + p2.z + p3.z) * (1/3));
		float d1 = center.distance(p1);
		float d2 = center.distance(p2);
		float d3 = center.distance(p3);
		float mdist = max(max(d1, d2), d3);

		boundingBox.center = center;
		boundingBox.radius = mdist;
	}

	void setup() {
		p1 = p1 + 0.1;
		p12 = p1 - p2;
		p13 = p1 - p3;
		p21 = p2 - p1;
		p23 = p2 - p3;
		p31 = p3 - p1;
		p32 = p3 - p2;
		n = (p31).cross(p21).norm();
		dist = n.dot(p1);

		Point center((p1.x + p2.x + p3.x) * (1 / 3), (p1.y + p2.y + p3.y) * (1 / 3), (p1.z + p2.z + p3.z) * (1 / 3));
		float d1 = center.distance(p1);
		float d2 = center.distance(p2);
		float d3 = center.distance(p3);
		float mdist = max(max(d1, d2), d3);

		boundingBox.center = center;
		boundingBox.radius = mdist;
	}

	// todo is this faster than the intersect method?
	bool boundingBoxIntersect(Point& origin, Point& ray, vector<Geometry*>& boundedList) {
		vector<Geometry*> temp;
		if (boundingBox.boundingBoxIntersect(origin, ray, temp)) {
			boundedList.push_back(new Triangle(*this));
			return true;
		}
		return false;
	};

	bool checkEdge(Point& a, Point& b, Point& p, Point& n) {
		Point ba = b - a;
		Point pa = p - a;
		Point c = ba.cross(pa);
		return n.dot(c) > 0;
	};

	float intersect(Point& origin, Point& ray, float& t, Geometry& g, Point& p, Point& surfaceNormal) {
		// get the norm
		Point u = p2 - p1;
		Point v = p3 - p1;
		surfaceNormal = v.cross(u).norm();
		t = (surfaceNormal.dot(origin) + surfaceNormal.dot(p1)) / surfaceNormal.dot(ray);
		p = ray * t;
		g = (*this);
		Point contactPoint = origin + ray * t;

		// if p is outside of any of the edges, there is no intersection
		return checkEdge(p2, p1, p, surfaceNormal) &&
			checkEdge(p3, p2, p, surfaceNormal) &&
			checkEdge(p1, p3, p, surfaceNormal);
	};

	void getSubDivide(vector<Triangle> & tl) {
		Point ab = (p1 + p2) / 2;
		Point bc = (p2 + p3) / 2;
		Point ac = (p1 + p3) / 2;
		
		tl.push_back(Triangle(p1, ac, ab, Color(0,255,0)));
		tl.push_back(Triangle(ac, p3, bc, Color(255, 0, 0)));
		tl.push_back(Triangle(ac, ab, bc, Color(255, 255, 255)));
		tl.push_back(Triangle(ab, bc, p2, Color(0, 0, 255)));
	}

	inline Triangle operator*(const float& a) {
		p1 = p1 * a;
		p2 = p2 * a;
		p3 = p3 * a;
		setup();
		return *this;
	}

	inline Triangle operator+(const Point& a) {
		p1 = p1 + a;
		p2 = p2 + a;
		p3 = p3 + a;
		setup();
		return *this;
	}
};
