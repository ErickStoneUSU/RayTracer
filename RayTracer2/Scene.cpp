#include "Light.cpp"
#include "Settings.cpp"
#include "Mesh.cpp"
#include <tuple>
#include <list>

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
		t1.color = Color(255, 255, 255);
		t1.specular = 0;
		t1.transparency = 0;

		Point p1(-100, -100, 80);
		Point p2(100, 100, 80);
		Point p3(-100, 100, 80);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
//		t1.texture.setPatternToCheckers();
		geo.push_back(new Triangle(t1));

		//t1.p3 = Point(10, -10, 80);
		//geo.push_back(new Triangle(t1));

		//t1.p1 = Point(-10, 10, 80);
		//t1.p2 = Point( 10, -10, 80);
		//t1.p3 = Point(10, 10, 80);
		//geo.push_back(new Triangle(t1));

		//t1.p3 = Point(-10, -10, 80);
		//geo.push_back(new Triangle(t1));
	}
	void getTopBackGround() {
		Triangle t1 = Triangle();
		t1.color = Color(255, 255, 255);
		t1.transparency = 1;
		t1.specular = 1;

		// floor
		Point p1(-10, 10, 120);
		Point p2(10, 10, 120);
		Point p3(40, 40, 115);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		geo.push_back(new Triangle(t1));

		t1.p3 = Point(-40, 40, 115);
		geo.push_back(new Triangle(t1));
	}
	void getBottomBackGround() {
		Triangle t1 = Triangle();
		t1.color = Color(255, 255, 255);
		t1.transparency = 1;
		t1.specular = 0;

		// floor
		Point p1(-10, -10, 120);
		Point p2(10, -10, 120);
		Point p3(40, -40, 115);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		geo.push_back(new Triangle(t1));

		t1.p3 = Point(-40, -40, 115);
		geo.push_back(new Triangle(t1));
	}
	void getRightBackGround() {
		Triangle t1 = Triangle();
		t1.color = Color(255, 255, 255);
		t1.transparency = 1;
		t1.specular = 0.5;

		Point p1(-10, -10, 120);
		Point p2(-40, -40, 115);
		Point p3(-10, 10, 120);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		geo.push_back(new Triangle(t1));

		t1.p2 = Point(-40, 40, 115);
		geo.push_back(new Triangle(t1));
	}
	void getLeftBackGround() {
		Triangle t1 = Triangle();
		t1.color = Color(255, 255, 255);
		t1.transparency = 0;
		t1.specular = 0.5;

		Point p1(10, -10, 120);
		Point p2(40, -40, 115);
		Point p3(10, 10, 120);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		geo.push_back(new Triangle(t1));

		t1.p2 = Point(40, 40, 115);
		geo.push_back(new Triangle(t1));
	}

	// todo figure this out
	void getMesh(float rotate, float scale, Point move) {
		const float density = 0.5f;
		Triangle t = Triangle();
		t.p1.z = move.z;
		t.p2.z = move.z;
		t.p3.z = move.z;
		t.specular = 0;
		t.transparency = 0;
		t.color = Color(255, 255, 255);

		for (int k = 1; k < 3; ++k) {
			for (float i = 0; i < 1; i = i + density) {
				Mesh m;
				for (float j = 0; j < 1; j = j + density) {
					t.p1.x = i + move.x;
					t.p1.y = j + move.y;
					t.p2.x = (i - 3 + k) + move.x;
					t.p2.y = (j - 3 - k) + move.y;
					t.p3.x = (i + 3) + move.x;
					t.p3.y = (j + 3) + move.y;
					t.color.b = 255 - (i * 255);
					t.color.g = 255 / k;
					t.setup();
					m.tris.push_back(t);
				}
				geo.push_back(new Mesh(m));
			}
		}
	}

	void getMesh2(float rotate, float scale, Point move) {
		Mesh m;
		Point p1(0, 0, 0);
		Point p2(-1, 0, 0);
		Point p3(0, -1, 0);
		Triangle t1 = Triangle();
		t1.color = Color(255, 255, 255);
		t1.specular = 0;
		t1.transparency = 0;

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;

		m.tris.push_back((t1 * scale) + move);
		m.tris.push_back((t1 * scale) + move + Point(0,3,0));
		//m.tris.push_back(((t1 * scale) + move) * 2);
		geo.push_back(new Mesh(m));
		/*Mesh n = m;
		geo.push_back(new Mesh(n + Point(3, 0, 1)));
		Mesh o = m;
		geo.push_back(new Mesh(o + Point(-3, 0, 1)));*/

	}

	void getCircle() {
		Circle cir(Point(-6, 6, 75), 3, Color(255, 0, 0));
		//cir.texture.setPatternToCheckers();
		cir.thickness = 1.5;
		cir.specular = 0;
		cir.transparency = 0;
		
		geo.push_back(new Circle(cir));

		cir.color = Color(255, 255, 255);
		cir.center = Point(6, 0, 75);
		cir.specular = 0;
		cir.transparency = 0.8;
		//geo.push_back(new Circle(cir));

		cir.color = Color(255, 255, 255);
		cir.center = Point(0, 0, 75);
		cir.specular = 0;
		cir.transparency = 0;
		//geo.push_back(new Circle(cir));
	}

	void getLight() {
		Light light = Light(Point(10, 0, 65), 100, Color(255, 255, 255));
		//l.push_back(light);

		light = Light(Point(0, 0, 60), 0.1, Color(255, 255, 255));
		//l.push_back(light);

		light = Light(Point(8, 0, 60), 0.1, Color(255, 0, 0));
		l.push_back(light);
		
		light = Light(Point(0, 0, 60), 0.1, Color(0, 255, 0));
		l.push_back(light);

		light = Light(Point(-8, 0, 60), 0.1, Color(0, 0, 255));
		l.push_back(light);

		//light = Light(Point(0, -8, 85), 800, Color(0, 0, 255));
		//l.push_back(light);

		//light = Light(Point(0, 8, 85), 800, Color(0, 0, 255));
		//l.push_back(light);
	}
	void getScene() {
		// x++ -> right
		// y++ -> down
		// be in the middle of the film
		cam.point = Point(0.1, 0, 0);
		//getForeground();
		getBackGround();
		//getMesh2(1, 2, Point(0, 0, 75));
		//getTopBackGround();
		//getBottomBackGround();
		//getLeftBackGround();
		//getRightBackGround();
		getCircle();
		getLight();
	}
	Scene() {
		getScene();
	}
};