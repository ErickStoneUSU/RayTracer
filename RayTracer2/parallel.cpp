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

using namespace std;
using namespace concurrency;
void mainLoop();
bool rayTrace(Triangle t, Point camera, Point ray, float& tempt);


void mainLoop() {
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
	const int objNums = s.o.size();
	Point cam = s.cam.point;
	const Color black = Color(0, 0, 0);
//#pragma omp parallel for
	for (int i = 0; i < DIM; ++i)
	{
		for (int j = 0; j < DIM; ++j)
		{
			for (int k = 0; k < DIM; ++k)
			{
				float t = 999999;
				int closestVar = -1;
				for (int l = 0; l < DIM; ++l)
				{
					for (int m = 0; m < objNums; ++m) {
						float tempt = 0;
						if (rayTrace(s.o[m], cam, Point(float(i*DIM + k), float(j*DIM + l), 0) - cam, tempt)) {
							if (tempt < t && tempt > 0) {
								t = tempt;
								closestVar = m;
							}
						}
					}

					if (closestVar == -1) {
						cList[k][l] = black;
					}
					else {
						cList[k][l] = s.o[closestVar].col;
					}
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

bool rayTrace(Triangle t, Point camera, Point ray, float & tempt) {
	if (t.intersect(camera, ray, tempt)) {
		return true;
	}
	else {
		return false;
	}
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

int main() {
	mainLoop();
	PPMMaker().mergeFile();
	return 0;
}