#pragma once
#include "Point.cpp"
#include "Color.cpp"
#include "Material.cpp"
#include <vector>

using namespace std;

class Triangle {
public:
	Triangle() {};
	Triangle(Point & a, Point & b, Point & c) { p1 = a; p2 = b; p3 = c; };
	Material m;
	Point p1;
	Point p2;
	Point p3;
	Color col;

	// a and b form the edge
	// p is the point on the triangle
	// n is the normalized triangle vector
	bool checkEdge(Point& a, Point& b, Point& p, Point& n) {
		Point ba = b - a;
		Point pa = p - a;
		Point c = ba.cross(pa);
		return n.dot(c) >= 0;
	};

	vector<float> barycentricCheckEdge(Point& a, Point& b, Point& p, Point& n, float& area) {
		Point ba = b - a;
		Point pa = p - a;
		Point c = ba.cross(pa);
		float u = (c.magnitude() / 2) / area;
		vector<float> res;
		res.push_back(n.dot(c));
		res.push_back(u);
		return res;
	};


	float intersect(Point& origin, Point& ray, float& t, Point & p, Point & reflectRay) {
		// get the norm
		Point ab = (p1 - p2);
		Point ac = p1 - p3;
		Point n = ac.cross(ab).norm();

		// use the planes normal (triangles norm) to find the point P on the triangle
		// distance from origin to plane
		float D = n.dot(p1);

		// find t in a parametric equation to find P
		// R(t) = (1-t)C+tr
		// A*a+B*b+C*c / D = t
		// P = (a*t, b*t, c*t) -- this is the case when the camera is at 0,0,0
		t = (n.dot(origin) + n.dot(p1)) / n.dot(ray);
		p = ray * t;

		// equation from https://www.fabrizioduroni.it/2017/08/25/how-to-calculate-reflection-vector.html
		reflectRay = origin * 2 * origin.dot(ray) - ray;

		// if p is outside of any of the edges, there is no intersection
		return checkEdge(p2, p1, p, n) &&
			checkEdge(p3, p2, p, n) &&
			checkEdge(p1, p3, p, n);
	};

	bool barycentricIntersect(Point& origin, Point& film, float& u, float& v, float& D) {
		// get the norm
		Point ab = p1 - p2;
		Point ac = p1 - p3;
		Point n = ac.cross(ab);

		// barycentric uses the area of the triangle for a weighted 3 way division of the triangle about the points with P at its center
		float area = n.magnitude() * 0.5f;

		// use the planes normal (triangles norm) to find the point P on the triangle
		// distance from origin to plane
		D = n.dot(p1);

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
