#include "Settings.cpp"
#include <vector>
#include <Windows.h>
#include <stdio.h>
#include <ppl.h>
#include "Scene.cpp"
#include "PPMMaker.cpp"
#include "Point.cpp"
#include "Color.cpp"
#include <iostream>
#include <time.h>
#include <algorithm>
#include "Shapes_Full_Spheres.cpp"

using namespace std;
using namespace concurrency;
void mainLoop();
bool rayTrace(Triangle t, Point camera, Point ray, float& tempt);
Color getRadiance(Scene& s, int& objNums, Point p, Point ray, int depth);
const Color black(0, 0, 0);

Point getReflectRay(Point & p, Point & ray) {
	// V = p
	// N = ray
	// equation from https://www.fabrizioduroni.it/2017/08/25/how-to-calculate-reflection-vector.html
	float dotted = p.dot(ray);
	return p + ray * (-2 * dotted);
}

// todo
Point getRefractRay(Point& p, Point& ray, float indRef1, float indRef2) {
	// https://en.wikipedia.org/wiki/Snell%27s_law
	float dir = p.dot(ray);
	float cos1 = 0;
	float ratio = 0;
	int flip = 1;

	// any angles that are not between 0 and 180 are chopped
	// ratio is flipped if we are exiting of entering the medium
	if (dir > 0) {
		cos1 = min(dir, 1);
		// light entering medium
		ratio = indRef2 / indRef1;
		flip = -1;
	}
	else {
		cos1 = abs(max(dir, -1));
		// light exiting medium
		ratio = indRef1 / indRef2;
	}	

	// cos@2 = sqrt(1 - (n1/n2)^2*(1-cos@1^2))
	// vref = (n1/n2)*l + (n1/n2*cos@1-cos@2)*n
	
	float cos2 = 1 - ratio * ratio * (1 - cos1 * cos1);

	if (cos2 < 0) {
		return Point(0, 0, 0); // direction vector
	}
	
	return p * ratio + ray * (ratio * cos1 - sqrt(cos2)) * flip;
}

// cast the next point to the ray pointing to the light
// return false if an object hits that ray and is closer
bool castShadowRay(Scene & s, int & objNums, Point p, Point ray, float dist) {
	float tempt = 0;
	for (int m = 0; m < objNums; ++m) {
		if (s.geo[m].boundingBox.intersect(p, ray, tempt)) {
			for (int n = 0; n < s.geo[m].t.size(); ++n) {
				Point contactPoint;
				if (s.geo[m].t[n].intersect(p, ray, tempt, contactPoint)) {
					if ((contactPoint - p).magnitude() < dist) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void getClosestObject(Scene & s, int & objNums, Point & p, Point & ray, int & closestObj, int & closestTri, Point & nextPoint, float & t) {
	// get closest object
	for (int m = 0; m < objNums; ++m) {
		float tempt = 999999;
		Point contactPoint;

		// if the bounding box hits, then check the triangles
		// todo figure out how to ignore points if inside a negative sphere
		//if (s.geo[m].boundingBox.intersect(p, ray, tempt)) {
			for (int n = 0; n < s.geo[m].t.size(); ++n) {
				if (s.geo[m].t[n].intersect(p, ray, tempt, contactPoint)) {
					if (tempt < t) {
						t = tempt;
						closestObj = m;
						closestTri = n;
						nextPoint = contactPoint;
					}
				}
			}
		//}
	}
}



Color getRadiance(Scene & s, int & objNums, Point p, Point ray, int depth, float refInd) {
	// only allow a certain level for light bounces
	if (depth > 4){ return black; }

	// radiance is the amount of light / the area
	// the area is the cosine of the angle
	
	float t = 999999;
	int closestObj = -1;
	int closestTri = -1;
	vector<float> radiance;
	Point nextPoint;
	getClosestObject(s, objNums,p, ray, closestObj, closestTri, nextPoint, t);
	
	// if an object is found
	if (closestObj != -1) {
		//radiance = Color(100,100,100); // ambience
		// add diffuse to radiance
		// add specular to radiance
		// if the object has any diffuse, get the color from the material itself.
		if (s.geo[closestObj].transparency > 0) {
			for (Light l : s.l) {
				Point ray = l.p - nextPoint;
				float dist = ray.magnitude();
				if (castShadowRay(s, objNums, nextPoint, ray, dist)) {
					for (Geometry g : s.geo) {
						float distance;
						// is there a light intersection
						if (l.intersect(nextPoint, ray, distance)) {
							// color = color + getAmountOfLight(nextPoint, nextRay)
							radiance.push_back(l.s.intensity * (1 / (distance)); // today is there better??
						}
					}
				}
			}
		}

		// If object is reflective, get the color from reflection ray
		if (s.geo[closestObj].reflectivity > 0) {
			Point reflectRay = getReflectRay(p,ray);
			radiance = (radiance + getRadiance(s, objNums, nextPoint, reflectRay, ++depth, refInd)) * (s.geo[closestObj].reflectivity);
		}

		// If the object has any transparency then recurse the refraction ray
		if (s.geo[closestObj].transparency > 0) {
			Point refractRay;
			refractRay = getRefractRay(p, ray, refInd, s.geo[closestObj].m.thickness);
			radiance = (radiance + getRadiance(s, objNums, nextPoint, refractRay, ++depth, s.geo[closestObj].m.thickness));
		}

		return (s.geo[closestObj].t[closestTri].col * s.geo[closestObj ].transparency) + radiance;
	}
	return black;
}

void mainLoop() {
	vector<float> vecOfRandomNums(50);
	for (int i = 0; i < vecOfRandomNums.size(); ++i) { vecOfRandomNums[i] = rand() % 100 / 100000.0; }

	Scene s = Scene();
	Color c = Color();
	vector<vector<Color>> cList;

	// allocate the size of the vector
	vector<Color> temp;
	for (int j = 0; j < DIM; ++j)
	{
		temp.push_back(Color(0, 0, 0));
	}

	for (int i = 0; i < DIM; ++i)
	{
		cList.push_back(temp);
	}
	clock_t start, end;
	start = clock();
	int objNums = s.geo.size();
	Point cam = s.cam.point;
	float max = 1 / (DIM * DIM);
	float offset = DIM * DIM / 2;
#pragma omp parallel for
	for (int i = 0; i < DIM; ++i)
	{
		for (int j = 0; j < DIM; ++j)
		{
			for (int k = 0; k < DIM; ++k)
			{
				for (int l = 0; l < DIM; ++l)
				{
					// todo consider moving the random to a random point on the found triangle
					// normalize to ba a scale of 0 to 1
					Point p = Point(float(j * DIM + l) - offset, float(i * DIM + k) - offset , 5).norm();
					//Color color(0,0,0);
					// sample
					//for (int m = 0; m < 2; ++m) {
						//color = color + (getRadiance(s, objNums, cam + vecOfRandomNums[m], p, 0) * .5);
					//}
					cList[k][l] = getRadiance(s, objNums, cam, p, 0, 1);
				}
			}
			PPMMaker().writeBlock(cList, i, j);
		}
	}
	end = clock();
	double duration_sec = (double(end) - double (start)) / CLOCKS_PER_SEC;
	std::cout << duration_sec;
	return;
}

// old notes
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
	// determine how much of the ray is should add to 100% as all rays must do one of these three:
	// 1. reflected
	// 2. absorbed
	// 3. refracted
	// if reflected is > 0
	// get the ray reflected from the next point
	// if refracted is > 0
	// get the ray refracted from the next point
	// the light that a pixel recieves is:
	// 1. reduced by color
	// 2. reduced by absortion 
	// 3. split by reflection and refraction
	// 4. consider both paths (perhaps choose one randomly)
	// the Fresnel Equation can tell us how to mix reflection and refraction
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction

int main() {
	mainLoop();
	PPMMaker().mergeFile();
	return 0;
}