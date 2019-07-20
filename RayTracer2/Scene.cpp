#include "Circle.cpp"
#include "Triangle.cpp"
#include "Light.cpp"
#include "Point.cpp"
#include "Color.cpp"
#include "Settings.cpp"
#include "Geometry.cpp"

using namespace std;

// HEADERS
struct Camera { Point point; };

class Scene {
public:
	vector<Geometry> geo;
	vector<Light> l;
	Camera cam;
	Geometry getBox() {
		Geometry g = Geometry();
		Triangle t1 = Triangle();
		g.m.f.color = 250;
		g.m.thickness = 1;
		g.reflectivity = 0;
		g.transparency = 0;

		// bottom
		t1.p1 = Point(0, 0, 20);
		t1.p2 = Point(100, 100, 20);
		t1.p3 = Point(100, 0, 20);
		t1.col = Color(255, 0, 20);
		g.t.push_back(t1);

		t1.p3 = Point(0, 100, 20);
		t1.col = Color(255, 0, 20);
		g.t.push_back(t1);

		// left
		t1.p1 = Point(100, 0, 20);
		t1.p2 = Point(100, 100, 20);
		t1.p3 = Point(100, 0, 10);
		t1.col = Color(255, 255, 0);
		g.t.push_back(t1);

		t1.p1 = Point(100, 0, 10);
		t1.col = Color(255, 255, 0);
		g.t.push_back(t1);
		return g;
	}
	Geometry getForground() {
		return getBox();
	}
	void getScene() {
		// x++ -> right
		// y++ -> down
		cam.point = Point(DIM * DIM / 2 + 100, DIM * DIM / 2 + 100, 0);
		geo.push_back(getForground());

		Light light = Light(Point(0, 0, 0), Spectrum(Color(255,255,255)));
		l.push_back(light);

		light = Light(Point(50, 50, 30), Spectrum(Color(255,255,255)));
		l.push_back(light);

		Circle c = Circle();
		c.center = Point(200, 200, 35);
		c.radius = 5;
		c.color = Color(200, 200, 200);
		/*vector<Triangle> ct = c.convertToTriangles();
		for (Triangle it : ct) {
			geo.push_back(it);
		}*/
	}
	Scene() {
		getScene();
	}
};