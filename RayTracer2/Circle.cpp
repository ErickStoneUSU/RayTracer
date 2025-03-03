#pragma once
#include "Geometry.cpp"

class Circle : public Geometry {
public:
	Circle() { ; }
	Circle(Point& poi, float& rad, Color col) { center = poi; radius = rad; color = col; invRadius = -1 / rad; rsq = radius * radius; };
	Circle(Point poi, float rad, Color col) { center = poi; radius = rad; color = col; invRadius = -1 / rad; rsq = radius * radius;
	};
	Point center;
	float radius;
	float rsq;
	float invRadius; // precalculate invRadius because this is hit a lot
	const float SELF_AVOID_T = .001;

	Point norm(Point p) {
		return ((p - center) * invRadius).norm();
	}

	bool intersect(Point& o, Point& r, float& t, Geometry & contactObj, Point& contactPoint, Point & surfaceNormal) const {
		Point oc = o - center;

		const float b = oc.dot(r) * 2;
		const float ac = oc.dot(oc) - rsq;

		float delta = b * b - 4 * ac;

		if (delta < 0) {
			t = FLT_MAX;
			return false;
		}

		// quadratic equation
		const float sol1 = (-b - sqrt(delta)) / 2;
		const float sol2 = (-b + sqrt(delta)) / 2;

		if (sol2 < .001) { 
			t = FLT_MAX;
			return false;
		}

		if (sol1 >= .001) {
			t = sol1;
		}
		else {
			t = sol2;
		}
		contactPoint = o + r * t;
		surfaceNormal = (contactPoint - center).norm();
		contactObj = (*this);
		return true;
	}

	// minimal rough approx
	bool boundingBoxIntersect(Point& o, Point& r, vector<Geometry *>& boundedList) {
		// get p
		// p = origin + t * ray
		// px = ox + t*rx
		// py = oy + t*ry
		// pz = oz + t*rz
		// (px-cx)^2 + (py-cy)^2 + (pz-cz)^2 = r^2
		// 4 equations 4 unknowns
		// ((ox + t*rx)-cx)^2 + ((oy + t*ry)-cy)^2 + ((oz + t*rz)-cz)^2 = r^2
		// (a-b)(a-b) = a^2 - 2ab + b^2
		// (ox-cx)^2 -2(t*rx*(ox-cx)) + (t*rx)^2  +
		// (oy-cy)^2 -2(t*ry*(oy-cy)) + (t*ry)^2  +
		// (oz-cz)^2 -2(t*rz*(oz-cz)) + (t*rz)^2  
		// = r^2
		// a = (ox-cx)
		// b = (oy-cy)
		// c = (oz-cz)
		// a^2 + b^2 + c^2 +
		// -2t*rx*a + -2t*ry*b + -2t*rz*c +    -->  -6t (rx*a+ry*b+rz*c)
		// t^2*rx^2 + t^2*ry^2 + t^2*rz^2      --> 3t^2*(rx^2 + ry^2 + rz^2)
		// = r^2
		// Use the quadratic equation to solve this system of equations
		// since we have an t^2 + t + const - r^2 = 0
		// the part of b^2 - 4*a*c describes numbers of solutions
		// http://paulbourke.net/geometry/circlesphere/ for equation of intersect
		Point d = o - center; // the a,b,c from the comment
		// don't confuse these a,b,c with the comment as these are for the quad equation
		float a = r.magnitude() * 3;
		float b = r.dot(d) * -6;
		float inside = b * b - 4 * a * rsq;

		// a 0 means one solution which can be ignored
		// a - means no solutions
		// > 0 means 2 solutions which means a hit and a closest part
		if (inside > 0) {
			Circle * c = new Circle(*this);
			boundedList.push_back(c);
			return true;
		}

		return false;
	};
};