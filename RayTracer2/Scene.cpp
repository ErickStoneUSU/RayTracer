#include "Light.cpp"
#include "Settings.cpp"
#include "Mesh.cpp"

using namespace std;

// HEADERS
struct Camera { Point point; };

class Scene {
public:
	Mesh gMesh;
	vector<Light> l;
	Camera cam;
	void getForeground() {
		Mesh mtemp;
		Triangle t1 = Triangle();
		t1.color = Color(0, 0, 0);

		// square
		t1.p1 = Point(0, 0, 20);
		t1.p2 = Point(100, 200, 20);
		t1.p3 = Point(0, 200, 20);
		t1.color = Color(0, 0, 255);
		mtemp.geos.push_back(new Triangle(t1));

		t1.p3 = Point(100, 0, 20);
		t1.color = Color(0, 255, 0);
		mtemp.geos.push_back(new Triangle(t1));
		gMesh.geos.push_back(new Mesh(mtemp));
	}
	void getBackGround() {
		Mesh mtemp;
		Triangle t1 = Triangle();
		t1.color = Color(150, 150, 150);
		t1.specular = 0;
		t1.transparency = 0;
		t1.diffuse = 1;

		Point p1(-10, -10, 80);
		Point p2(10, 10, 80);
		Point p3(-10, 10, 80);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		mtemp.tris.push_back(new Triangle(t1));

		t1.p3 = Point(10, -10, 80);
		mtemp.tris.push_back(new Triangle(t1));

		t1.p1 = Point(-10, 10, 80);
		t1.p2 = Point( 10, -10, 80);
		t1.p3 = Point(10, 10, 80);
		mtemp.tris.push_back(new Triangle(t1));

		t1.p3 = Point(-10, -10, 80);
		mtemp.tris.push_back(new Triangle(t1));

		gMesh.ms.push_back(new Mesh(mtemp));
	}
	void getTopBackGround() {
		Mesh mtemp;
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
		mtemp.geos.push_back(new Triangle(t1));

		t1.p3 = Point(-40, 40, 115);
		mtemp.geos.push_back(new Triangle(t1));
		gMesh.geos.push_back(new Mesh(mtemp));
	}
	void getBottomBackGround() {
		Mesh mtemp;
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
		mtemp.geos.push_back(new Triangle(t1));

		t1.p3 = Point(-40, -40, 115);
		mtemp.geos.push_back(new Triangle(t1));

		gMesh.geos.push_back(new Mesh(mtemp));
	}
	void getRightBackGround() {
		Mesh mtemp;
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
		mtemp.geos.push_back(new Triangle(t1));

		t1.p2 = Point(-40, 40, 115);
		mtemp.geos.push_back(new Triangle(t1));
		gMesh.geos.push_back(new Mesh(mtemp));
	}
	void getLeftBackGround() {
		Mesh mtemp;
		Triangle t1 = Triangle();
		t1.color = Color(255, 255, 255);
		t1.transparency = 1;
		t1.specular = 0.5;

		Point p1(10, -10, 120);
		Point p2(40, -40, 115);
		Point p3(10, 10, 120);

		t1.p1 = p1;
		t1.p2 = p2;
		t1.p3 = p3;
		mtemp.geos.push_back(new Triangle(t1));

		t1.p2 = Point(40, 40, 115);
		mtemp.geos.push_back(new Triangle(t1));
		gMesh.geos.push_back(new Mesh(mtemp));
	}

	void getCircle() {
		Mesh mtemp;
		Circle cir(Point(-6, 6, 75), 3, Color(100, 100, 100));
		cir.thickness = 1.1;
		cir.specular = 0.2;
		cir.transparency = 0.2;
		cir.diffuse = 0.6;
		//geo.push_back(new Circle(cir));


		//cir.center = Point(6, 0, 75);
		//geo.push_back(new Circle(cir));

		cir.center = Point(0, 0, 75);
		mtemp.geos.push_back(new Circle(cir));

		//cir.center = Point(0, -6, 70);
		//cir.specular = 0.5;
		//cir.radius = 3;
		//cir.transparency = 0.5;
		//geo.push_back(new Circle(cir));


		//cir.center = Point(0, 6, 100);
		//cir.specular = 0.5;
		//cir.transparency = 0.5;
		//geo.push_back(new Circle(cir));
		gMesh.geos.push_back(new Mesh(mtemp));
	}

	void getLight() {
		Light light = Light(Point(10, 0, 65), 100, Color(0, 255, 0));
		//l.push_back(light);

		light = Light(Point(8, 0, 90), 0.1, Color(0, 255, 0));
		//l.push_back(light);

		light = Light(Point(-10, 0, 50), 0.1, Color(255, 0, 0));
		l.push_back(light);
		
		light = Light(Point(-10, 0, 60), 0.1, Color(0, 255, 0));
		l.push_back(light);

		light = Light(Point(-10, 0, 70), 0.1, Color(0, 0, 255));
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
		cam.point = Point(0, 0, 0);
		//getForeground();
		getBackGround();
		//getTopBackGround();
		//getBottomBackGround();
		//getLeftBackGround();
		//getRightBackGround();
		getCircle();
		getLight();
		gMesh.initial = true;
		gMesh.populateBoundingBox();
	}
	Scene() {
		getScene();
	}
};