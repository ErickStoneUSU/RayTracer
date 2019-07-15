#include <Windows.h>
#include <stdio.h>
#include <ppl.h>
#include <iostream>
#include <time.h>
#include "Scene.cpp"
#include "PPMMaker.cpp"


using namespace std;
using namespace concurrency;
void mainLoop();
bool rayTrace(Triangle t, Point camera, Point ray, float& tempt);
Color getRadiance(Scene& s, int& objNums, Point p, Point ray, int depth);
const Color black(0, 0, 0);

Point getReflectRay(Point & p, Point & ray, Point & surfaceNormal) {
	// V = p
	// N = ray
	// equation from https://www.fabrizioduroni.it/2017/08/25/how-to-calculate-reflection-vector.html
	return ray - surfaceNormal * (ray.dot(surfaceNormal) * 2);
}

// todo
Point getRefractRay(Point& p, Point& ray, float indRef1, float indRef2, Point& surfaceNormal) {
	// https://en.wikipedia.org/wiki/Snell%27s_law
	float cos1 = -surfaceNormal.dot(ray);
	float cos2,ratio = 0;
	int flip = 1;


	// any angles that are not between 0 and 180 are chopped
	// ratio is flipped if we are exiting of entering the medium
	if (cos1 < 0) {
		// light entering medium
		ratio = indRef2 / indRef1;
		surfaceNormal = surfaceNormal * -1;
		cos1 = -cos1;
	}
	else {
		// light exiting medium
		ratio = indRef1 / indRef2;
	}	

	// cos@2 = sqrt(1 - (n1/n2)^2*(1-cos@1^2))
	// vref = (n1/n2)*l + (n1/n2*cos@1-cos@2)*n
	
	cos2 = 1 - ratio * ratio * (1 - cos1 * cos1);

	if (cos2 < 0) {
		return ray - (surfaceNormal * (2 * ray.dot(surfaceNormal)));
	}
	
	return (p * ratio) + (surfaceNormal * (ratio * cos1 - sqrt(cos2)));
}

// cast the next point to the ray pointing to the light
// return false if an object hits that ray and is closer
bool castShadowRay(Scene & s, Point & p, Point &ray) {
	float tempt = 0;
	Point contactPoint;
	for (int i = 0; i < s.geo.size(); ++i) {
		// do not cast shadow rays at lights
		if (Circle * c = dynamic_cast<Circle*>(s.geo[i])) {
			bool res = dynamic_cast<Circle*>(s.geo[i])->boundingBoxIntersect(p, ray); 
			if (!res){
				return false;
			}
		}
		else if (Mesh * c = dynamic_cast<Mesh*>(s.geo[i])) {
			bool res = dynamic_cast<Mesh*>(s.geo[i])->boundingBoxIntersect(p, ray);
			if (!res) {
				return false;
			}
		}
		else if (Triangle * c = dynamic_cast<Triangle*>(s.geo[i])) {
			Triangle tri;
			Point surfaceNormal;
			bool res = dynamic_cast<Triangle*>(s.geo[i])->boundingBoxIntersect(p, ray);
			if (!res) {
				return false;
			}
		}
	}
	return true;
}

void getClosestObject(Scene & s, int & objNums, Point & p, Point & ray, Geometry & closestObj, float & t, bool & found, Point & surfaceNormal, Point & contactPoint) {
	// get closest object
	
	for (int i = 0; i < s.geo.size(); ++i){
		float tempt = FLT_MAX;
		;

		// if the bounding box hits, then check the triangles
		// todo figure out how to ignore points if inside a negative sphere
		if (Circle * c = dynamic_cast<Circle*>(s.geo[i])) {
			Circle contactObj;
			if (dynamic_cast<Circle*>(s.geo[i])->boundingBoxIntersect(p, ray)) {
				if (dynamic_cast<Circle*>(s.geo[i])->intersect(p, ray, tempt, contactObj, contactPoint, surfaceNormal)) {
					if (tempt < t) {
						t = tempt;
						closestObj = *s.geo[i];
						found = true;
					}
				}
			}
		} else if (Mesh * c = dynamic_cast<Mesh*>(s.geo[i])) {
			Triangle contactObj;
			if (dynamic_cast<Mesh*>(s.geo[i])->boundingBoxIntersect(p, ray)) {
				if (dynamic_cast<Mesh*>(s.geo[i])->intersect(p, ray, tempt, contactObj, contactPoint, surfaceNormal)) {
					if (tempt < t) {
						t = tempt;
						closestObj = *s.geo[i];
						found = true;
					}
				}
			}
		} else if (Triangle *c = dynamic_cast<Triangle*>(s.geo[i])) {
			Triangle contactObj;
			if (dynamic_cast<Triangle*>(s.geo[i])->boundingBoxIntersect(p, ray)) {
				if (dynamic_cast<Triangle*>(s.geo[i])->intersect(p, ray, tempt, contactObj, contactPoint, surfaceNormal)) {
					if (tempt < t) {
						t = tempt;
						closestObj = *s.geo[i];
						found = true;
					}
				}
			}
		}
		
	}
}



Color getColor(Scene & s, int & objNums, Point p, Point ray, int depth, float refInd) {
	// only allow a certain level for light bounces
	if (depth > 10){ return black; }

	// radiance is the amount of light / the area
	// the area is the cosine of the angle
	
	float t = FLT_MAX;
	Geometry closestObj;
	bool found = false;
	Point surfaceNormal;
	Point nextPoint;
	getClosestObject(s, objNums,p, ray, closestObj, t, found, surfaceNormal, nextPoint);
	
	// if an object is found
	if (found) {	
		Color specular(0, 0, 0); 
		Color refraction(0, 0, 0);
		Color diffuse(0, 0, 0);

		if (closestObj.specular + closestObj.transparency > 1) {
			cout << "Object found with specular + transparency > 1";
		}

		if (closestObj.specular > 0.0)
		{
			specular = getColor(s, objNums, nextPoint, getReflectRay(p, ray, surfaceNormal), ++depth, refInd) * closestObj.specular;
		}
		if (closestObj.transparency > 0.0)
		{
			refraction = getColor(s, objNums, nextPoint, getRefractRay(p, ray, refInd, closestObj.thickness, surfaceNormal), ++depth, closestObj.thickness) * closestObj.transparency;
		}
		
		// get diffuse component
		for (Light l : s.l) {
			// todo get the point on the circle closest to the nextPoint
			Point lightRay = l.point - nextPoint;
			// todo which is less expensive checking the light intersection or checking the shadow?
			if (castShadowRay(s, nextPoint, lightRay)) {
				// the amount that is reflected in the direction of the light
				// lambertian shading
				diffuse = diffuse + l.color * (max(surfaceNormal.dot(lightRay.norm()), 0) * closestObj.specular);
			}
		}

		// get reflective component
		return specular + // specular
			diffuse + // diffuse
			refraction; // refraction
	}
	return black;
}


void mainLoop() {
	vector<float> vecOfRandomNums(1000);
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
	const float offset = -DIM * DIM / 2;
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
					// normalize to be a scale of 0 to 1
					// the offset positions 0,0 in the middle
					// the huge z avoids fish eye by having the beam mostly focus forward
					Point p = (cam - Point(float(j * DIM + l) + offset, float(i * DIM + k) + offset, -800)).norm();

					Color color(0,0,0);

					for (int m = 0; m < 10; ++m) {
						color = color + (getColor(s, objNums, cam + vecOfRandomNums[m], p, 0, 1) * 0.1);
					}
					cList[k][l] = color;
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