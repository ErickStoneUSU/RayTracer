#pragma once
#include <vector>
#include "Material.cpp"
#include "Point.cpp"

class Geometry {
public:
	virtual ~Geometry() {}
	float opacity = 1; // 0 to 1
	float reflectivity = 1; // 0 to 1
	float specular = 1; // 0 to 1
	Color color;
	float thickness = 1; // 1 is vacuum/air
	virtual bool boundingBoxIntersect(Point& o, Point& dir, float& t) { 
		return false; 
	};
	virtual Point norm(Point& p) { 
		return p; 
	};
	virtual bool intersect(Point& o, Point& dir, float& t, Geometry& contactObj, Point contactPoint) { 
		return false; 
	};
};