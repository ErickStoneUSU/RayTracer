#pragma once
#include "Circle.cpp"
class Light { 
public: 
	Light() { ; }
	Light(Point poi, float inte, Color col) { point = poi; intensity = inte; color = col; }
	Color color;
	float intensity;
	Point point;
};