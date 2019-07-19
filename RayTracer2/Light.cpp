#pragma once
#include "Spectrum.cpp"
#include "Material.cpp"
#include "Point.cpp"
class Light { 
public: 
	Light() { ; }
	Light(Point i, Spectrum j) { p = i; s = j; }
	// add intensity
	// color instead of spectrum at least until you get everything else working
	Point p;
	Spectrum s;
	float intensity;
	Color color;

	bool intersect(Point& origin, Point& ray, float & distance) {
		// get the norm
		// math from https://onlinemschool.com/math/assistance/cartesian_coordinate/p_line/
		Point rayFromLightToObject = origin - p;
		Point newVector = rayFromLightToObject.cross(ray);

		// if it is ""reasonably"" close, give it to them
		// this gives us the distance of the point to the line
		distance = newVector.magnitude() / ray.magnitude();
		if (distance < 75) {
			return true;
		}

		// if p is outside of any of the edges, there is no intersection
		return false;
	};
};