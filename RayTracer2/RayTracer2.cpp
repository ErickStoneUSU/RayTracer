/******************************************************************************

							  Online C++ Compiler.
			   Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>
#include "RayTracer2.h"

using namespace std;


const int MAX_VAL = 60000;
const int XDIM = 100;
const int YDIM = 100;
void testPPM();
bool testCross();

struct Material {
	float diffuse = 0;
	float reflectivity = 0;
};

struct Point {
	float x = 0;
	float y = 0;
	float z = 0;
	inline Point operator-(const Point& a) {
		Point b;
		b.x = a.x - x;
		b.y = a.y - y;
		b.z = a.z - z;
		return b;
	}
	inline Point operator+(const Point& a) {
		Point b;
		b.x = a.x + x;
		b.y = a.y + y;
		b.z = a.z + z;
		return b;
	}
	inline Point operator*(const float& a) {
		Point b;
		b.x = x * a;
		b.y = y * a;
		b.z = z * a;
		return b;
	}
	inline Point operator/(const float& a) {
		Point b;
		b.x = x / a;
		b.y = y / a;
		b.z = z / a;
		return b;
	}

	inline float dot(const Point& a) {
		return a.x * x + a.y * y + a.z * z;
	}

	inline Point norm() {
		float magnitude = sqrt(x * x + y * y + z * z);
		Point point;
		point.x = x / magnitude;
		point.y = y / magnitude;
		point.z = z / magnitude;
		return point;
	}

	inline Point cross(const Point& a) {
		Point point;
		point.x = x * a.z - z * a.y;
		point.y = z * a.x - x * a.z;
		point.z = x * a.y - y * a.x;
		return point;
	}

	inline float magnitude() {
		return sqrt(x * x + y * y + z * z);
	}
};

float dot(const Point& a, const Point& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Color {
	int r = 0;
	int g = 0;
	int b = 0;
	inline Color operator+(const Color& a) {
		Color c;
		c.r = a.r + r;
		c.g = a.g + g;
		c.b = a.b + b;
		r = (r < 0) ? 0 : (r > 255) ? 255 : r;
		g = (g < 0) ? 0 : (g > 255) ? 255 : g;
		b = (b < 0) ? 0 : (b > 255) ? 255 : b;
		return c;
	}
	inline Color operator*(const float& a) {
		Color c;
		c.r = a * r;
		c.g = a * g;
		c.b = a * b;
		r = (r < 0) ? 0 : (r > 255) ? 255 : r;
		g = (g < 0) ? 0 : (g > 255) ? 255 : g;
		b = (b < 0) ? 0 : (b > 255) ? 255 : b;
		return c;
	}
};

struct Light {
	Point point;
	float amplitude = 0;
	float frequency = 0;
	float energy = 0;
	float waveLength = 0;
};

class Circle {
public:
	Material material;
	Point center;
	Color color;
	float radius = 0;
	float intersect(Point origin, Point film) {
		// y = mx + b  
		  // at 0,0 origin b is 0 and m = p2y/p2x
		// these represent the lines is both planes
		// ray to film
		if (film.x == 0) {
			film.x = 0.1f;
		}

		float yx = film.y / film.x;
		float zx = film.z / film.x;

		// center of circle
		float yxc = center.y / center.x;
		float zxc = center.z / center.x;

		// math equation for distance from point on a line
		// https://www.geeksforgeeks.org/find-points-at-a-given-distance-on-a-line-of-given-slope/
		float xleft = center.x + radius * sqrt(1 / (1 + yxc * yxc));
		float yleft = center.y + radius * -1.0f * yxc * sqrt(1 / (1 + yxc * yxc));
		float zleft = center.z + radius * -1.0f * zxc * sqrt(1 / (1 + zxc * zxc));

		float yxl = yleft / xleft;
		float zxl = zleft / xleft;

		float xright = center.x - radius * sqrt(1 / (1 + yxc * yxc));
		float yright = center.y - radius * -1.0f * yxc * sqrt(1 / (1 + yxc * yxc));
		float zright = center.z - radius * -1.0f * zxc * sqrt(1 / (1 + zxc * zxc));

		float yxr = yright / xright;
		float zxr = zright / xright;

		// these represent the lines in both planes for the circle
		// 1. translate circle 0,0
		// 2. rotate 90 degrees in both directions
		// 3. tranlate circle back to origin
		// the two translated points are the upper and lower bounds of the ray hit
		//cout << yxl << "   " << yx << "   " << yxr << "   " << zxl << "   " << zx << "   " << zxr << "\n";
		if (yxl <= yx && yx <= yxr && zxl <= zx && zx <= zxr) {
			// hit so return the distance to the sphere intersect
			return 1.0f;
		}

		return MAX_VAL;
	};
	bool intersect2(Point origin, Point direction, float& t) {
		// todo how does this work??
		Point rayToCircle = origin - center;
		// the 2 * ??
		float dotproducts = 2 * rayToCircle.dot(direction);
		float somevalue = rayToCircle.dot(rayToCircle) - radius * radius; // ?? itself in the same direction??
		float disc = dotproducts * dotproducts - 4 * somevalue;
		if (disc < 0) {
			return false;
		}
		else {
			disc = sqrt(disc);
			float t0 = -dotproducts - disc;
			float t1 = -dotproducts + disc;
			t = (t0 < t1) ? t0 : t1;
			return true;
		}
	}

	Point norm(Point& pi) {
		return (pi - center) / radius;
	}
};

struct Film {
	Point point;
	float xDim;
	float yDim;
	float upRotate; // ignore for now
	float sideRotate; // ignore for now
};

struct Camera {
	Point point;
};

struct Triangle {
	Point p1;
	Point p2;
	Point p3;
	
	// a and b form the edge
	// p is the point on the triangle
	// n is the normalized triangle vector
	bool checkEdge(Point& a, Point& b, Point& p, Point& n) {
		Point ba = b - a;
		Point pa = p - a;
		Point c = ba.cross(pa);
		return n.dot(c) >= 0;
	};

	vector<float> barycentricCheckEdge(Point& a, Point& b, Point& p, Point& n, float & area) {
		Point ba = b - a;
		Point pa = p - a;
		Point c = ba.cross(pa);
		float u = (c.magnitude() / 2) / area;
		vector<float> res;
		res.push_back(n.dot(c));
		res.push_back(u);
		return res;
	};

	float intersect(Point & origin, Point & film, float & t) {
		// get the norm
		Point ab = p1 - p2;
		Point ac = p1 - p3;
		Point n = ac.cross(ab).norm();

		// use the planes normal (triangles norm) to find the point P on the triangle
		// distance from origin to plane
		float D = n.dot(p1);

		// find t in a parametric equation to find P
		// R(t) = (1-t)C+tr
		// A*a+B*b+C*c / D = t
		// P = (a*t, b*t, c*t) -- this is the case when the camera is at 0,0,0
		t = (n.dot(origin) + n.dot(p1)) / n.dot(film);
		Point p = film * t;

		// if p is outside of any of the edges, there is no intersection
		return checkEdge(p2,p1,p,n) && 
			checkEdge(p3,p2,p,n) && 
			checkEdge(p1,p3,p,n);
	};

	bool barycentricIntersect(Point& origin, Point& film, float & u, float & v) {
		// get the norm
		Point ab = p1 - p2;
		Point ac = p1 - p3;
		Point n = ac.cross(ab);

		// barycentric uses the area of the triangle for a weighted 3 way division of the triangle about the points with P at its center
		float area = n.magnitude() * 0.5;

		// use the planes normal (triangles norm) to find the point P on the triangle
		// distance from origin to plane
		float D = n.dot(p1);

		// find t in a parametric equation to find P
		// R(t) = (1-t)C+tr
		// A*a+B*b+C*c / D = t
		// P = (a*t, b*t, c*t) -- this is the case when the camera is at 0,0,0
		float t = (n.dot(origin) + n.dot(p1)) / n.dot(film);
		Point p = film * t;

		// if p is outside of any of the edges, there is no intersection
		bool p1p2 = checkEdge(p2, p1, p, n);
		if (!p1p2) {
			return false;
		}
		vector<float> resA = barycentricCheckEdge(p3, p2, p, n, area);
		if (resA[0] < 0) {
			return false;
		}
		u = resA[1];
		vector<float> resB = barycentricCheckEdge(p1, p3, p, n, area);
		if (resB[0] < 0) {
			return false;
		}
		v = resB[1];
		return true;

	};
};

struct Scene {
	vector<Circle> circleList;
	vector<Triangle> triangleList;
	vector<Light> lightList;
	Camera cam;
	Film fil;
};

Point getPoint(float x, float y, float z) {
	Point point;
	point.x = x;
	point.y = y;
	point.z = z;
	return point;
}

Color getColor(int r, int g, int b) {
	Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

Scene getScene() {
	Scene s;
	s.cam.point = getPoint(50, 50, 0);

	Triangle t;
	t.p1 = getPoint(10,10,1);
	t.p2 = getPoint(90, 90, 1);
	t.p3 = getPoint(90, 10, 1);
	s.triangleList.push_back(t);

	t.p1 = getPoint(10, 10, 1);
	t.p2 = getPoint(90, 90, 1);
	t.p3 = getPoint(10, 90, 1);
	s.triangleList.push_back(t);

	Film film;
	film.xDim = 500;
	film.yDim = 500;
	film.point = getPoint(0, 0, 1);
	s.fil = film;

	Light light;
	light.energy = 1; // eV
	light.point = getPoint(0, 0., 50);
	s.lightList.push_back(light);

	Circle c = Circle();
	c.center = getPoint(250, 250, 50);
	c.radius = 20;
	c.color = getColor(0, 255, 0);
	s.circleList.push_back(c);
	return s;
}

void writeImage(vector<Color>& colorList) {
	ofstream file;
	file.open("preImage1.ppm");
	file << "P3\n" << XDIM << " " << YDIM << "\n" << "255\n";

	for (int i = 0; i < XDIM; ++i) {
		for (int j = 0; j < YDIM; ++j) {
			const Color c = colorList.at(i * XDIM + j);
			file << c.r << "   " << c.g << "   " << c.b;
			if (j != YDIM - 1) {
				file << "     ";
			}
		}

		if (i != XDIM - 1) {
			file << "\n";
		}
	}
	file.close();
}

void mainLoop() {
	Scene s = getScene();
	vector<Color> image;
	
	for (int i = 0; i < YDIM; ++i) {
		for (int j = 0; j < XDIM; ++j) {
			Point point = getPoint(j, i, 1);
			float t = 20000;
			for (int k = 0; k < int(s.circleList.size()); ++k){
				if (s.triangleList[k].intersect(s.cam.point, point, t)) {
					Point pi = s.cam.point + point * t;
					Point L = s.lightList[0].point - pi;
					Point N = s.circleList[0].norm(pi);
					//float dt = dot(L,N,s.circleList[0].center, s.circleList[0].radius);
					float dt = dot(L.norm(), N.norm());
					if (dt < 0) {
						dt = dt * -1.0f;
					}
					Color col = (getColor(0, 0, 255) + getColor(255, 255, 255) * dt);
				
					//float strength = rayTrace(s, ray, 0);
					//Color color = strengthToColor(strength);
					image.push_back(col);
				}
				else {
					image.push_back(getColor(0, 0, 0));
				}
			}			
		}
	}

	writeImage(image);
}




int main()
{
	mainLoop();
	return 0;
}



// camera point -> (0,0,0) and film point -> (i,j,1)
// get ray 
// point = fp + cp = (i,j,1)
// magnitude = sqrt((x+x)^2 + (y+y)^2 + (z+z)^2) = sqrt(i^2 + j^2 + 1)
// an intersection happens if the norm vector pointing at obj matches
	// norm vector pointing at film
// a ray is a length, angle, and theta
// if we pick an arbitrary big length, 
// then we only care about angle and theta
// norm(film - eye) = ray ?? why
// check for intersection with all objects
// this means that all objects will need an intersect type method
// if no hit, then return backgrounnd color	WSZ
// else, cast the nearest hit
// maybe todo later, seperate the front item detection and  keep a cache of any pixel dominators
// get ambience
// store in matrix

void testPPM() {
	vector<Color> image;
	for (int i = 0; i < YDIM; ++i) {
		for (int j = 0; j < XDIM; ++j) {
			if (i < 500 && j < 500) {
				image.push_back(getColor(0, 0, 255));
			}
			else if (j < 500) {
				image.push_back(getColor(0, 255, 0));
			}
			else if (i < 500) {
				image.push_back(getColor(255, 0, 0));
			}
			else {
				image.push_back(getColor(255, 255, 255));
			}
		}
	}
	writeImage(image);
}

// this both tests the cross product as well as shows the steps for triangle intersection
bool testCross() {
	// 3 points for a triangle
	Point a = getPoint(-1, -1, 0);
	Point b = getPoint(1, -1, 0);
	Point c = getPoint(0, 1, 0);

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