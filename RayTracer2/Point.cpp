#pragma once
using namespace std;
#include <cmath>

class Point {
public:
	Point() {}
	Point(float i, float j, float k) { x = i; y = j; z = k; }
	float x = 0;
	float y = 0;
	float z = 0;
	inline Point operator-(const Point& a) {
		return Point(x - a.x, y - a.y, z - a.z);
	}
	inline Point operator+(const Point& a) {
		return Point(a.x + x, a.y + y, a.z + z);
	}
	inline Point operator+(const float& a) {
		return Point(a + x, a + y, a + z);
	}
	inline Point operator*(const float& a) {
		return Point(a * x, a * y, a * z);
	}
	inline Point operator/(const float& a) {
		return Point(x / a, y / a, z / a);
	}

	inline float dot(const Point& a) {
		return a.x * x + a.y * y + a.z * z;
	}

	inline Point norm() {
		float m = sqrt(x * x + y * y + z * z);
		return Point(x / m, y / m, z / m);
	}

	inline Point cross(const Point& a) {
		return Point(x * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
	}

	inline float magnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	inline float distance(Point p) {
		float xm = p.x - x;
		float ym = p.y - y;
		float zm = p.z - z;
		return sqrt(xm*xm+ym*ym+zm*zm);
	}

	// this both tests the cross product as well as shows the steps for triangle intersection
	bool testCross() {
		// 3 points for a triangle
		Point a = Point(-1, -1, 0);
		Point b = Point(1, -1, 0);
		Point c = Point(0, 1, 0);

		// get the two edges
		Point A = b - a;
		Point B = c - a;

		// the cross product gives us the triangles norm
		Point C = A.cross(B);
		if (int(C.x) != 0 or int(C.y) != 0 or int(C.z) != 4) {
			return false;
		}
		return true;
	}
};
