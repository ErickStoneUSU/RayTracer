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

	void getAveragedValue(int& i, int& j, vector<vector<Color>>& cList, int& r, int& g, int& b) {
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
		file.open(base + "body_" + to_string(startX) + "_" + to_string(startY) + ".ppm");
		for (int i = 0; i < DIM; ++i) {
			for (int j = 0; j < DIM; ++j) {
				//if (i > 0 && j > 0 && i < DIM - 1 && j < DIM - 1) {
				//	int r, g, b;
				//	getAveragedValue(i, j, cList, r, g, b); // apply smoothing filter -- maybe not needed
				//	file << r << "   " << g << "   " << b;
				//}
				//else {
				file << cList[i][j].r << " " << cList[i][j].g << " " << cList[i][j].b;
				//}
				if (j != DIM - 1) {
					file << " ";
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
					out << line << " ";
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

	void stereogram(bool debug, float lefteye, float righteye, float eyeNormToPlane) {
		for (int test = 1; test < 100; test += 1) {
			const int eyeWidth = DIM * DIM / 2;
			ifstream in;
			in.open(base + "amerged.ppm");
			ofstream out;
			if (debug) {
				out.open(base + "aastereogram"+to_string(test)+".ppm");
			}
			else {
				out.open(base + "aastereogram" + to_string(test) + ".ppm");
			}
			out << "P3\n" << XDIM * 2 - 2 << " " << YDIM << "\n" << "255\n";
			string line;
			bool even = false;
			int cou = 0;
			while (!in.eof()) {

				getline(in, line);

				// populate rgb values into lineNums
				// exclude non relevant lines such as header
				if (line.length() > 20) {
					++cou;
					// get the rgb values
					vector <tuple<int, int, int>> lineNums;
					vector<string> spl = split(line.c_str(), ' ');
					int r;
					int g;
					int b;
					int sequence = 0;
					for (int i = 0; i < spl.size() - 2; ++i) {
						if (spl[i] != "") {
							if (sequence == 0) {
								r = stoi(spl[i]);
								sequence += 1;
							}
							else if (sequence == 1) {
								g = stoi(spl[i]);
								sequence += 1;

							}
							else if (sequence == 2) {
								b = stoi(spl[i]);
								sequence = 0;
								lineNums.push_back(tuple<int, int, int>(r, g, b));
							}
						}
					}

					//populate random values into outVector equivalent to size of input
					vector <tuple<int, int, int>> outVector;
					for (int i = 0; i < int((lineNums.size() - 1) / 2); ++i) {

						/*if (i % ((XDIM * 2 - 2) / 8) == 0) {
							outVector.push_back(tuple<int, int, int>(25, 25, 25));
						}
						else {*/
							if (debug) {
								outVector.push_back(tuple<int, int, int>(0, 0, 0));
							}
							else {
								outVector.push_back(tuple<int, int, int>(rand() % 255, rand() % 255, rand() % 255));
							}
						/*}*/
					}

					// populate right side with offset
					int outSize = outVector.size();
					for (int i = 0; i < outSize; ++i) {
						outVector.push_back(outVector[i]);
					}

					// populate 3 and 4
					outSize = outVector.size();
					for (int i = 0; i < outSize; ++i) {
						outVector.push_back(outVector[i]);
					}


					// build the depth map onto the random image
					for (int i = 0; i < lineNums.size(); ++i) {
						// if the image has depth
						// use the depth to get the locations of intersect and shift them by 1
						// replicate across the distance across the image
						float m = get<0>(lineNums[i]);
						if (m > 0) {
							// has depth

							// triangle with smaller triangle ratio
							// side / side == smaller side / smaller side
							float filmRightDist = (righteye - i) * (eyeNormToPlane - m) / (eyeNormToPlane - m + 255) + i;
							float filmLeftDist = (i - lefteye) * (eyeNormToPlane - m) / (eyeNormToPlane - m + 255) + i;
							
							if ((filmRightDist + i < outVector.size()) && (i - filmLeftDist > 0)) {
								auto color = i - filmLeftDist;
								if (debug) {
									for (int j = i + filmRightDist; j < outVector.size(); j += filmLeftDist + filmRightDist) {
										outVector[j] = tuple<int, int, int>(255, 255, 255);
									}
									for (int j = i - filmLeftDist; j > 0; j -= filmLeftDist + filmRightDist) {
										outVector[j] = tuple<int, int, int>(255, 255, 255);
									}
								}
								else {
									for (int j = i + filmRightDist; j < outVector.size(); j += filmLeftDist + filmRightDist) {
										outVector[j] = outVector[i - filmLeftDist - 1];
									}
									for (int j = i - filmLeftDist; j > 0; j -= filmLeftDist + filmRightDist) {
										outVector[j] = outVector[i - filmLeftDist - 1];
									}
								}

							}
						}
					}

					for (int i = 0; i < outVector.size(); ++i) {
						out << get<0>(outVector[i]) << " " << get<1>(outVector[i]) << " " << get<2>(outVector[i]);
						if (i != outVector.size() - 1) {
							out << " ";
						}
					}
					out << "\n";
				}
			}

			out.close();
			in.close();
		}
	}

};