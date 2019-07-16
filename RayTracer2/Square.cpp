#pragma once
#include "Circle.cpp"
#include <algorithm>

class Square : public Geometry {
public:
	Square() { ; };
	Square(Point& a, Point& b, Point& c, Point& d) { setup(a, b, c, d, Color(0, 0, 0)); };
	Square(Point& a, Point& b, Point& c, Point& d, Color e) { setup(a, b, c, d, e); };

	// points
	Point p1;
	Point p2;
	Point p3;
	Point p4;
	// edges
	Point p12;
	Point p13;
	Point p14;
	Point p23;
	Point p24;
	Point p34;
	// norms
	Point n123;
	Point n234;
	Point n321;
	Point n432;
	
	Circle * boundingBox;

	void setup(Point& a, Point& b, Point& c, Point& d, Color e) {
		color = e;
		p1 = a; p2 = b; p3 = c;
		p12 = p1 - p2;
		p13 = p1 - p3;
		p14 = p1 - p4;
		p23 = p2 - p3;
		p24 = p2 - p4;
		p34 = p3 - p4;
		n123 = (p2 - p1).cross(p3 - p1).norm();
		n234 = (p3 - p2).cross(p4 - p2).norm();
		n321 = (p2 - p3).cross(p1 - p3).norm();
		n432 = (p3 - p4).cross(p2 - p4).norm();
		Point center((p1.x + p2.x + p3.x + p4.x) * 0.25f, (p1.y + p2.y + p3.y + p4.y) * 0.25f, (p1.z + p2.z + p3.z + p4.z) * 0.25f);
		float d1 = center.distance(p1);
		float d2 = center.distance(p2);
		float d3 = center.distance(p3);
		float d4 = center.distance(p4);
		
		boundingBox = new Circle(center,max(max(d1,d2),max(d3,d4)),e);
	}

	// a and b form the edge
	// p is the point on the triangle
	// n is the normalized triangle vector
	bool checkEdges(Point & n, Point p) {
		float dist = n.dot(p);
		if (dist < 0) {
			dist = n234.dot(p);
		}
		return dist > 0;
	};

	// todo is this faster than the intersect method?
	bool boundingBoxIntersect(Point& origin, Point& ray, vector<Geometry*>& boundedList) {
		vector<Geometry*> temp;
		if (boundingBox->boundingBoxIntersect(origin, ray, temp)) {
			boundedList.push_back(new Square(*this));
			return true;
		}
		return false;
	};


	float intersect(Point& origin, Point& ray, float& t, Geometry* & g, Point& p, Point& surfaceNormal) {
		
		// use the planes normal (triangles norm) to find the point P on the triangle
		// distance from origin to plane
		Point n = n123;
		float dist = n123.dot(p1);
		if (dist < 0) {
			n = n234;
			dist = n234.dot(p1);
		}
		t = (n.dot(origin) + dist) / n.dot(ray);
		p = ray * t;
		if (n.dot(p) < 0) {
			return false;
		}

		// find t in a parametric equation to find P
		// R(t) = (1-t)C+tr
		// A*a+B*b+C*c / D = t
		// P = (a*t, b*t, c*t) -- this is the case when the camera is at 0,0,0
		
		g = new Square(*this);

		Point contactPoint = origin + ray * t;
		Point u = p2 - p1;
		Point v = p3 - p1;
		surfaceNormal = Point(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);

		// if p is outside of any of the edges, there is no intersection
		return true;
	};
};
