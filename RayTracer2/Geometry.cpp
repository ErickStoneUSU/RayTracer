#pragma once
#include <vector>
#include "Material.cpp"
#include "Point.cpp"

class Geometry {
public:
	virtual ~Geometry() {}
	float transparency = 0; // 0 to 1
	float specular = 0; // 0 to 1
	Color color;
	float thickness = 1; // 1 is vacuum/air
	float diffuse = 1;
	virtual bool boundingBoxIntersect(Point& o, Point& dir, vector<Geometry *> boundedList) { 
		return false; 
	};
	virtual Point norm(Point& p) { 
		return p; 
	};
	virtual bool intersect(Point& o, Point& dir, float& t, Geometry& contactObj, Point contactPoint, Point & surfaceNormmal) { 
		return false; 
	};
};