#include <vector>
#include "Triangle.cpp"
#include "Circle.cpp"

class Geometry {
public:
	vector<Triangle> t;
	float transparency = 0; // 0 to 1
	float reflectivity = 0; // 0 to 1
	Material m;
	Circle boundingBox;

	void populateCircle() {}
	void populateTriangle() {}
};