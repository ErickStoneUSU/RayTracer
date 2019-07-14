#pragma once
#include "Circle.cpp"
class Light { 
public: 
	Light() { ; }
	Light(Point poi, float inte, Color col) { point = poi; intensity = inte; color = col; }
	Color color;
	float intensity;
	Point point;

	bool intersect(Point& o, Point& dir, float & distance, Circle & contactObj, Point & contactPoint) {
		return Circle(point, 0.01, Color()).intersect(o, dir, distance, contactObj, contactPoint);
	};

	Point norm(Point& p) {
		return p;
	}
};