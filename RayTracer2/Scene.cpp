#include "Light.cpp"
#include "Settings.cpp"
#include "Mesh.cpp"

using namespace std;

// HEADERS
struct Camera { Point point; };

class Scene {
public:
	vector<Geometry*> geo;
	vector<Light> l;
	Camera cam;
	void getForeground() {
		Triangle t1 = Triangle();
		t1.color = Color(0, 0, 0);

		// square
		t1.p1 = Point(0, 0, 20);
		t1.p2 = Point(100, 200, 20);
		t1.p3 = Point(0, 200, 20);
		t1.color = Color(0, 0, 255);
		geo.push_back(new Triangle(t1));

		t1.p3 = Point(100, 0, 20);
		t1.color = Color(0, 255, 0);
		geo.push_back(new Triangle(t1));
	}
	void getBackGround() {
		Triangle t1 = Triangle();
		t1.color = Color(10,10,10);

		// floor
		Point p1(-10, 10, 100);
		Point p2(10, 10, 100);
		Point p3(0, 10, 100);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		geo.push_back(new Triangle(t1));
	}

	// todo figure this out
	void convertToTriangles(Circle & c) {
		Mesh m;
		int density = 10;
		float pi = 3.1415f;

		for (int i = 0; i < density; ++i) {
			float v = i * (pi / density);
			for (int j = -density; j < density; ++j) {
				float phi = j * (pi / density);
				Triangle t;
				t.p1 = Point(10, 10, 50);
				t.p2 = Point(15, 15, 50);
				t.p3 = Point(15, 10, 50);
				t.color = Color(0, 0, 255);
				m.tris.push_back(t);
			}
		}
		geo.push_back(new Mesh(m));
	}

	void getCircle() {
		Circle c;
		c.center = Point(0, 0, 25);
		c.radius = 100;
		c.color = Color(255, 255, 255);
		convertToTriangles(c);
	}
	void getScene() {
		// x++ -> right
		// y++ -> down
		// be in the middle of the film
		cam.point = Point(0, 0, -1000);
		//getForeground();
		//getBackGround();
		
		Circle cir(Point(DIM*DIM / 2, DIM * DIM / 2, 25), 1, Color(255, 255, 255));
		geo.push_back(new Circle(cir));

		Light light = Light(Point(10, 10, 30), 100, Color(255, 0, 0));
		l.push_back(light);
		
		light = Light(Point(0, 0, 30), 100, Color(0, 255, 0));
		//l.push_back(light);
		//geo.push_back(new Light(light));

		light = Light(Point(-10, -10, 30), 100, Color(0, 0, 255));
		//l.push_back(light);
		//geo.push_back(new Light(light));

		/*Circle c = Circle();
		c.center = Point(200, 200, 35);
		c.radius = 5;
		c.color = Color(200, 200, 200);*/
		/*vector<Triangle> ct = c.convertToTriangles();
		for (Triangle it : ct) {
			geo.push_back(it);
		}*/
	}
	Scene() {
		getScene();
	}
};