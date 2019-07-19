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

using namespace std;
using namespace concurrency;
void mainLoop();
bool rayTrace(Triangle t, Point camera, Point ray, float& tempt);
float getRadiance(Scene& s, int& objNums, Point p, Point ray, int depth);

float getRadiance(Scene & s, int & objNums, Point p, Point ray, int depth) {
	// only allow a certain level for light bounces
	if (depth > 4){ return 0; }

	// radiance is the amount of light / the area
	// the area is the cosine of the angle
	float t = 999999;
	int closestVar = -1;
	float incident_light = 0;
	float amount_reflected = 0;
	float radiance = 0;
	Point nextPoint;
	Point nextRay;
	
	// get closest object
	for (int m = 0; m < objNums; ++m) {	
		float tempt = 999999;
		Point tempPoint;
		Point tempRay;
		if (s.o[m].intersect(p, ray, tempt, tempPoint, tempRay)) {
			if (tempt < t) {
				t = tempt;
				closestVar = m;
				nextPoint = tempPoint;
				nextRay = tempRay;
			}
		}
	}

	// IMPROVED:
	// 1. If object has any translucency
		// 1. get reflection ray
		// 2. recurse reflected ray
		// 3. get refraction ray
		// 4. recurse refraction ray
		// 5. compute fresnel equation

	// 2. If the object has any diffuse
		// 1. get absorbancy
		// 2. for each light
			// 3. for each object
				// 4. test if the object is in the way of the light 
				//    and the closest object to the film (what we hit earlier on)
				// 5. if not blocked, then add the lights brightness with any reductions for distance, material, and color
	// 3. return the object color * the summed light brightness

	if (closestVar != -1) {
		//radiance = s.o[closestVar].ambient + s.o[closestVar].getEmission();
		for (Light l : s.l) {
			float distance;
			if (l.intersect(nextPoint, nextRay, distance)) {
				// color = color + getAmountOfLight(nextPoint, nextRay)
				radiance += l.s.color / (distance * distance); // today is there better??
			}
		}

		// cast reflected ray
		// if reflected ray intersects obj, take into account its contribution
		float specular = s.o[closestVar].m.f.color;
		radiance += (specular * getRadiance(s, objNums, nextPoint, nextRay, ++depth));
	}
	return 0;
}

void mainLoop() {
	vector<float> vecOfRandomNums(DIM);
	for (int i = 0; i < vecOfRandomNums.size(); ++i) { vecOfRandomNums[i] = rand() % 100 / 10000.0; }

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
	int objNums = s.o.size();
	Point cam = s.cam.point;
	const Color black = Color(0, 0, 0);
#pragma omp parallel for
	for (int i = 0; i < DIM; ++i)
	{
		for (int j = 0; j < DIM; ++j)
		{
			for (int k = 0; k < DIM; ++k)
			{
				for (int l = 0; l < DIM; ++l)
				{
					// recurse down bounces to get radiance
					Point p = Point(float(j * DIM + l), float(i * DIM + k), 25) - cam;
					int radiance = 0;
					// sample
					for (int m = 0; m < 5; ++m) {
						radiance += getRadiance(s, objNums, cam + vecOfRandomNums[m], p, 0);
					}
					radiance = radiance / 5;
					cList[k][l] = c.getColor(radiance);
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