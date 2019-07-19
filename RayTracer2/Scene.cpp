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
	Geometry getForground() {
		Geometry g = Geometry();
		Triangle t1 = Triangle();
		g.m.f.color = 250;
		g.m.thickness = 1;
		g.reflectivity = 1;
		g.transparency = 0;

		t1.p1 = Point(10, 10, 30);
		t1.p2 = Point(90, 90, 30);
		t1.p3 = Point(90, 10, 33);
		t1.col = Color(255, 0, 0);
		g.t.push_back(t1);

		t1.p3 = Point(10, 90, 37);
		t1.col = Color(0, 0, 255);
		g.t.push_back(t1);

		Triangle t2 = Triangle();
		t2.p1 = Point(90, 90, 35);
		t2.p2 = Point(100, 100, 35);
		t2.p3 = Point(90, 10, 37);
		t2.col = Color(0, 255, 0);
		g.t.push_back(t2);

		return g;
	}
	void getScene() {
		cam.point = Point(DIM * DIM / 2, DIM * DIM / 2, 0);
		geo.push_back(getForground());

		Light light = Light(Point(0, 0, 0), Spectrum(450));
		l.push_back(light);

		light = Light(Point(50, 50, 30), Spectrum(650));
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