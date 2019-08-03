#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Color.cpp"
#include "Settings.cpp"
#include <tuple>

const string base = "Image\\";
using namespace std;

struct PPMMaker {
	PPMMaker() {};

	void writeHeader() {
		ofstream file;
		file.open(base + "header.ppm");
		file << "P3\n" << XDIM << " " << YDIM << "\n" << "255\n";
		file.close();
	}

	void getAveragedValue(int & i, int & j, vector<vector<Color>> & cList, int & r, int & g, int & b) {
		r = cList[i - 1][j - 1].r + cList[i + 1][j - 1].r +
			cList[i - 1][j + 1].r + cList[i + 1][j + 1].r;
		g = cList[i - 1][j - 1].g + cList[i + 1][j - 1].g +
			cList[i - 1][j + 1].g + cList[i + 1][j + 1].g;
		r = cList[i - 1][j - 1].b + cList[i + 1][j - 1].b +
			cList[i - 1][j + 1].b + cList[i + 1][j + 1].b;
		r = r >> 2;
		g = g >> 2;
		b = b >> 2;
	}

	void writeBlock(vector<vector<Color>> cList, int startX, int startY) {
		ofstream file;
		file.open(base + "body_" + to_string(startX) + "_" + to_string(startY) +".ppm");
		for (int i = 0; i < DIM; ++i) {
			for (int j = 0; j < DIM; ++j) {
				//if (i > 0 && j > 0 && i < DIM - 1 && j < DIM - 1) {
				//	int r, g, b;
				//	getAveragedValue(i, j, cList, r, g, b); // apply smoothing filter -- maybe not needed
				//	file << r << "   " << g << "   " << b;
				//}
				//else {
					file << cList[i][j].r << "   " << cList[i][j].g << "   " << cList[i][j].b;
				//}
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
		ofstream out;
		out.open(base + "amerged.ppm");
		out << "P3\n" << XDIM << " " << YDIM << "\n" << "255\n";
		for (int i = 0; i < DIM; ++i) {
			vector<ifstream *> v;
			
			for (int j = 0; j < DIM; ++j) {
				ifstream* f = new ifstream(base + "body_" + to_string(i) + "_" + to_string(j) + ".ppm", ios::in);
				v.push_back(f);
			}
			string line;
			while (!(*(v[0])).eof()) {
				for (auto& file : v) {
					getline(*file, line);
					//line.erase(remove(line.begin(), line.end(), '\n'), line.end());
					out << line << "   ";
				}
				out << "\n";
			}
			for (auto& file : v) {
				(*file).close();
			}
		}
		out.close();
	}

	// from https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
	vector<string> split(const char* str, char c = ' ')
	{
		vector<string> result;

		do
		{
			const char* begin = str;

			while (*str != c && *str)
				str++;

			result.push_back(string(begin, str));
		} while (0 != *str++);

		return result;
	}

	void stereogram() {
		ifstream in;
		in.open(base + "amerged.ppm");
		ofstream out;
		out.open(base + "stereogram.ppm");
		out << "P3\n" << XDIM << " " << YDIM << "\n" << "255\n";
		string line;
		
		while (!in.eof()) {
			
			getline(in,line);
			// exclude non relevant lines such as header
			if (line.length() > 20) {
				// get the rgb values
				vector <tuple<int, int, int>> lineNums;
				vector<string> spl = split(line.c_str(), ' ');
				for (int i = 0; i < spl.size() - 2; i += 3) {
					int r = stoi(spl[i]);
					int g = stoi(spl[i+1]);
					int b = stoi(spl[i+2]);
					lineNums.push_back( tuple<int,int,int>(r,g,b));
				}
			}
		}
		for (int i = 0; i < DIM; ++i) {
			vector<ifstream*> v;

			for (int j = 0; j < DIM; ++j) {
				ifstream* f = new ifstream(base + "body_" + to_string(i) + "_" + to_string(j) + ".ppm", ios::in);
				v.push_back(f);
			}
			string line;
			while (!(*(v[0])).eof()) {
				for (auto& file : v) {
					getline(*file, line);
					//line.erase(remove(line.begin(), line.end(), '\n'), line.end());
					out << line << "   ";
				}
				out << "\n";
			}
			for (auto& file : v) {
				(*file).close();
			}
		}
		out.close();
	}
};