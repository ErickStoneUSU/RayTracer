#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Color.cpp"
#include "Settings.cpp"

const string base = "C:\\Users\\erick\\Projects\\School\\RayTracer2\\RayTracer2\\Image\\";
using namespace std;
struct PPMMaker {
	PPMMaker() {};

	void writeHeader() {
		ofstream file;
		file.open(base + "header.ppm");
		file << "P3\n" << XDIM << " " << YDIM << "\n" << "255\n";
		file.close();
	}

	void writeBlock(vector<vector<Color>> cList, int startX, int startY) {
		ofstream file;
		file.open(base + "body_" + to_string(startX) + "_" + to_string(startY) +".ppm");
		for (int i = 0; i < DIM; ++i) {
			for (int j = 0; j < DIM; ++j) {
				file << cList[i][j].r << "   " << cList[i][j].g << "   " << cList[i][j].b;
				if (j != DIM - 1) {
					file << "     ";
				}
			}

			if (i != DIM - 1) {
				file << "\n";
			}
		}
		file.close();
	}

	void mergeFile() {
		// 1. for each block row
			// 2. read a line from each open file
			// if end of file close
			
		// ORDER:
		// blockrow,blockcolumn,row,column
		// we want to ready one line at a time for each i block
		// when all the lines for the i blocks are completed
		// then increment j
		// i is the block row 
		// j is the block column
		// k is y
		// l is x
		for (int i = 0; i < DIM; ++i) {
			vector<ifstream> v;
			ofstream out;
			out.open(base + "merged.ppm");
			for (int j = 0; j < DIM; ++j) {
				ifstream f(base + "body_" + to_string(i) + "_" + to_string(j) + ".ppm");
				v.push_back(f);
			}
			string line;
			while (!v[0].eof()) {
				for (auto& file : v) {
					getline(file, line);
					line.erase(remove(line.begin(), line.end(), '\n'), line.end());
					out << line;
					if (v.front != file) {
						out << "   ";
					}
				}
				if (!v[0].eof()){
					out << "\n";
				}
			}
		}
		// for each file that has 
		// pull the first line from each file
		// pull the second
	}
};