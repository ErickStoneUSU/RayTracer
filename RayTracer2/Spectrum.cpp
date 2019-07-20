#pragma once
#include "Color.cpp"

using namespace std;

class Spectrum{
public:
	Spectrum() { ; }
	Spectrum(Color b) { color = b; }
	Color color;
};
