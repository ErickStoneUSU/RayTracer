#pragma once
#include "Color.cpp"

using namespace std;

class Spectrum{
public:
	Spectrum() { ; }
	Spectrum(float b) { waveLength = b; }
	float waveLength;
};
