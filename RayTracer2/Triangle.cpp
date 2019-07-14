#pragma once
#include "Circle.cpp"
#include <algorithm>

class Triangle : public Geometry{
public:
	Triangle() { ; };
	Triangle(Point & a, Point & b, Point & c) { p1 = a; p2 = b; p3 = c; };
	Triangle(Point& a, Point& b, Point& c, Color d) { p1 = a; p2 = b; p3 = c; color = d; };
	
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
		return n.dot(c) > 0;
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

	// todo is this faster than the intersect method?
	bool boundingBoxIntersect(Point& origin, Point& ray, vector<Geometry*>& boundedList) {
		float t;
		Triangle tri;
		Point p;
		//return intersect(origin, ray, t, tri, p, surfaceNormal);
		// get the centroid
		float x = (p1.x + p2.x + p3.x) * 0.333f;
		float y = (p1.y + p2.y + p3.y) * 0.333f;
		float z = (p1.z + p2.z + p3.z) * 0.333f;

		Point center(x, y, z);

		// find the point furthest from the centroid
		float d1 = center.distance(p1);
		float d2 = center.distance(p2);
		float d3 = center.distance(p3);

		// see if in bounds of the circle created
		Circle c; 
		c.center = Point(x, y, z);
		c.radius = max(max(d1, d2),d3);// distance to furthest point
		vector<Geometry*> temp;
		if (c.boundingBoxIntersect(origin, ray, temp)) {
			boundedList.push_back(new Triangle(*this));
			return true;
		}
		return false;
	};


	float intersect(Point& origin, Point& ray, float& t, Geometry & g, Point & p, Point & surfaceNormal) {
		// get the norm
		Point ab = p2 - p1;
		Point ac = p3 - p1;
		Point n = ac.cross(ab).norm(); // the normal cross product is the vector coming out from the triangle

		// use the planes normal (triangles norm) to find the point P on the triangle
		// distance from origin to plane
		float dist = n.dot(p1);

		// find t in a parametric equation to find P
		// R(t) = (1-t)C+tr
		// A*a+B*b+C*c / D = t
		// P = (a*t, b*t, c*t) -- this is the case when the camera is at 0,0,0
		t = (n.dot(origin) + dist) / n.dot(ray);
		p = ray * t;
		g = (*this);

		Point contactPoint = origin + ray * t;
		Point u = p2 - p1;
		Point v = p3 - p1;
		surfaceNormal = Point(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);

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

	void getSubDivide(vector<Triangle> & tl) {
		Point ab = (p1 + p2) / 2;
		Point bc = (p2 + p3) / 2;
		Point ac = (p1 + p3) / 2;
		
		tl.push_back(Triangle(p1, ac, ab, Color(0,255,0)));
		tl.push_back(Triangle(ac, p3, bc, Color(255, 0, 0)));
		tl.push_back(Triangle(ac, ab, bc, Color(255, 255, 255)));
		tl.push_back(Triangle(ab, bc, p2, Color(0, 0, 255)));
	}

	Point norm(Point& p) {
		return p;
	}
};
