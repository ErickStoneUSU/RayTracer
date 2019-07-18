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
	void getScene() {
		cam.point = Point(DIM * DIM / 2, DIM * DIM / 2, 0);

		Geometry g = Geometry();
		Triangle t1 = Triangle();

		t1.p1 = Point(10, 10, 50);
		t1.p2 = Point(90, 90, 50);
		t1.p3 = Point(90, 10, 53);
		t1.col = Color(255, 0, 0);
		g.m.f.color = 250;
		g.reflectivity = 1;
		g.transparency = 0;
		g.t.push_back(t1);
		t1.p3 = Point(10, 90, 47);
		t1.col = Color(0, 0, 255);
		g.t.push_back(t1);
		g.m.thickness = 1;
		g.reflectivity = 0;
		g.transparency = .1;
		geo.push_back(g);

		Geometry g2 = Geometry();
		Triangle t2 = Triangle();
		t2.p1 = Point(90, 90, 55);
		t2.p2 = Point(100, 100, 45);
		t2.p3 = Point(90, 10, 47);
		t2.col = Color(0, 255, 0);

		g2.m.f.color = 300;
		g2.t.push_back(t2);
		g2.reflectivity = .5;
		g2.transparency = .5;
		g2.m.thickness = 1;
		geo.push_back(g2);

		Light light = Light(Point(0, 0, 0), Spectrum(450));
		l.push_back(light);

		Circle c = Circle();
		c.center = Point(200, 200, 55);
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