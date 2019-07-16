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
				t.col = Color(255, 0, 0);
				tris.push_back(t);
			}
		}
		return tris;
	}
};