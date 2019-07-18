#pragma once
#include "Point.cpp"
#include "Color.cpp"
#include "Triangle.cpp"
#include <vector>

using namespace std;
class Circle {
public:
	Circle() {};
	Point center;
	Color color;
	float radius;

	bool intersect(Point & o, Point & r, float & t) {
		// get p
		// p = origin + t * ray
		// px = ox + t*rx
		// py = oy + t*ry
		// pz = oz + t*rz
		// (px-cx)^2 + (py-cy)^2 + (pz-cz)^2 = r^2
		// 4 equations 4 unknowns
		// ((ox + t*rx)-cx)^2 + ((oy + t*ry)-cy)^2 + ((oz + t*rz)-cz)^2 = r^2
		// (a-b)(a-b) = a^2 - 2ab + b^2
		// (ox-cx)^2 -2(t*rx*(ox-cx)) + (t*rx)^2  +
		// (oy-cy)^2 -2(t*ry*(oy-cy)) + (t*ry)^2  +
		// (oz-cz)^2 -2(t*rz*(oz-cz)) + (t*rz)^2  
		// = r^2
		// a = (ox-cx)
		// b = (oy-cy)
		// c = (oz-cz)
		// a^2 + b^2 + c^2 +
		// -2t*rx*a + -2t*ry*b + -2t*rz*c +    -->  -6t (rx*a+ry*b+rz*c)
		// t^2*rx^2 + t^2*ry^2 + t^2*rz^2      --> 3t^2*(rx^2 + ry^2 + rz^2)
		// = r^2
		// Use the quadratic equation to solve this system of equations
		// since we have an t^2 + t + const - r^2 = 0
		// the part of b^2 - 4*a*c describes numbers of solutions
		// http://paulbourke.net/geometry/circlesphere/ for equation of intersect
		Point d = o - center; // the a,b,c from the comment
		// don't confuse these a,b,c witht he comment as these are for the quad equation
		float a = r.magnitude() * 3;
		float b = r.x * d.x + r.y * d.y + r.z * d.z * -6;
		float c = d.magnitude() - radius * radius;
		float inside = b * b - 4 * a * c;
		
		// a 0 means one solution which can be ignored
		// a - means no solutions
		// > 0 means 2 solutions which means a hit and a closest part
		if (inside > 0) {
			float t = (b * -1 + sqrt(inside)) / (2 * a);
			return true;
		}

		return false;
	}

	vector<Triangle>convertToTriangles() {
		int density = 10;
		float pi = 3.1415f;
		vector<Triangle> tris;
		for (int i = 0; i < density; ++i) {
			float v = i * (pi / density);
			for (int j = -density; j < density; ++j) {
				float phi = j * (pi / density);
				Triangle t;
				t.p1 = Point(10, 10, 50);
				t.p2 = Point(90, 90, 50);
				t.p3 = Point(90, 10, 50);
				t.col = Color(0, 0, 255);
				tris.push_back(t);
			}
		}
		return tris;
	}
};