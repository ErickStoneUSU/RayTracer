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
	Geometry getForeground() {
		Geometry g = Geometry();
		Triangle t1 = Triangle();
		g.m.f.color = 250;
		g.m.thickness = 1;
		g.reflectivity = 1;
		g.transparency = 1;

		// bottom
		t1.p1 = Point(-200, 0, 25);
		t1.p2 = Point(200, 200, 20);
		t1.p3 = Point(200, 0, 20);
		t1.col = Color(255, 0, 0);
		g.t.push_back(t1);

		t1.p3 = Point(0, 200, 20);
		t1.col = Color(0, 255, 0);
		g.t.push_back(t1);

		
		return g;
	}
	Geometry getBackGround() {
		Geometry g = Geometry();
		Triangle t1 = Triangle();
		g.m.f.color = 250;
		g.m.thickness = 1;
		g.reflectivity = 1;
		g.transparency = 1;

		// back
		t1.p1 = Point(10, 80, 25);
		t1.p2 = Point(100, 80, 25);
		t1.p3 = Point(100, -80, 25);
		t1.col = Color(255, 255, 255);
		g.t.push_back(t1);

		// right
		t1.p1 = Point(0, 0, 19);
		t1.p2 = Point(0, 100, 19);
		t1.p3 = Point(100, 0, 19);
		t1.col = Color(0, 0, 255);
		//g.t.push_back(t1);

		t1.p1 = Point(200, 0, 10);
		t1.col = Color(0, 0, 255);
		//g.t.push_back(t1);

		return g;
	}
	Geometry getCircle() {
		Geometry g = Geometry();
		g.m.f.color = 250;
		g.m.thickness = 1;
		g.reflectivity = 1;
		g.transparency = 1;

		Circle c;
		c.center = Point(DIM * DIM / 2, DIM * DIM / 2, 25);
		c.radius = 100;
		c.convertToTriangles(g.t);

	}
	void getScene() {
		// x++ -> right
		// y++ -> down
		// be in the middle of the film
		cam.point = Point(DIM * DIM / 2, DIM * DIM / 2, 0);
		geo.push_back(getForeground());
		//geo.push_back(getBackGround());

		Light light = Light(Point(DIM * DIM / 2.0 + 100, DIM * DIM / 2.0, 0), Spectrum(625)); // red wavelength
		l.push_back(light);

		light = Light(Point(DIM * DIM / 2.0 - 100, DIM * DIM / 2.0, 0), Spectrum(497)); // green and blue together
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