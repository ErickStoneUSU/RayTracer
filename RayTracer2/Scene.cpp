#include "Circle.cpp"
#include "Triangle.cpp"
#include "Light.cpp"
#include "Point.cpp"
#include "Color.cpp"
#include "Settings.cpp"

using namespace std;

// HEADERS
struct Camera { Point point; };

class Scene {
public:
	vector<Triangle> o;
	vector<Light> l;
	Camera cam;
	void getScene() {
		cam.point = Point(DIM * DIM / 2, DIM * DIM / 2, 0);

		Triangle t = Triangle();
		t.p1 = Point(10, 10, 50);
		t.p2 = Point(90, 90, 50);
		t.p3 = Point(90, 10, 50);
		t.col = Color(255, 0, 0);
		t.m.f.color = 250;
		o.push_back(t);

		t.p1 = Point(10, 10, 50);
		t.p2 = Point(90, 90, 50);
		t.p3 = Point(10, 90, 50);
		t.col = Color(0, 255, 0);
		t.m.f.color = 0;
		o.push_back(t);

		Light light = Light(Point(0, 0, 0), Spectrum(450));
		l.push_back(light);

		Circle c = Circle();
		c.center = Point(200, 200, 55);
		c.radius = 5;
		c.color = Color(200, 200, 200);
		vector<Triangle> ct = c.convertToTriangles();
		for (Triangle it : ct) {
			o.push_back(it);
		}
	}
	Scene() {
		getScene();
	}
};