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

using namespace std;
using namespace concurrency;

int main() {
	Scene s = Scene();
	Color c = Color();
	PPMMaker().writeHeader();
	vector<vector<Color>> cList;

	// allocate the size of the vector
	vector<Color> temp;
	for (int j = 0; j < DIM; ++j)
	{
		temp.push_back(Color(0,0,0));
	}

	for (int i = 0; i < DIM; ++i)
	{
		cList.push_back(temp);
	}


	//#pragma omp parallel for
	for (int i = 0; i < DIM; ++i)
	{
		for (int j = 0; j < DIM; ++j)
		{
			for (int k = 0; k < DIM; ++k)
			{
				for (int l = 0; l < DIM; ++l)
				{
					float tempt = 0.0f;
					for (int m = 0; m < s.o.size(); ++m) {
						if (rayTrace(s.o[m], s.cam.point, Point(float(j), float(i), 25.0f) - s.cam.point, tempt)) {
							cList[k][l] = c.getColor(450);
						}
						else {
							cList[k][l] = Color(0, 0, 0);
						}
					}
				}
			}
			PPMMaker().writeBlock(cList, i, j);
		}
	}
	return 0;
}

bool rayTrace(Triangle t, Point camera, Point ray, float & tempt) {
	if (t.intersect(camera, ray, tempt)) {
		return true;
	}
	else {
		return false;
	}
}

//void mainLoop() {
//	Scene s = Scene();
//	vector<Color> image;
//	int xd = 16;
//	int yd = 16;
//
//	for (int i = 0; i < yd; ++i) {
//		for (int j = 0; j < xd; ++j) {
//			Point point = Point(j, i, 25) - s.cam.point;
//			// get closest object
//			float t = 20000;
//			int closestVar = -1;
//			bool wasTri = true;
//			for (int k = 0; k < int(s.o.size()); ++k) {
//				float tempt;
//				if (s.o[k].intersect(s.cam.point, point, tempt)) {
//					if (tempt < t) {
//						t = tempt;
//						closestVar = k;
//					}
//				}
//			}
//
//			if (closestVar == -1) {
//				image.push_back(Color(0, 0, 0));
//			}
//			else if (wasTri) {
//				//Color col = s.o[closestVar].col;
//				//image.push_back(col);
//			}
//			else {
//				//Color col = s.o[closestVar].color;
//				//image.push_back(col);
//			}
//		}
//	}
//
//	//PPMMaker().writeImage(image);
//}

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