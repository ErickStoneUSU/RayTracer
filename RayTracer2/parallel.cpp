#include <vector>
#include <Windows.h>
#include <stdio.h>

using namespace std;

// HEADERS
struct Scene { vector<Light> l; vector<Object> o; Camera c; };

struct Camera {};

struct Light { Spectrum s; };
struct Spectrum { Color c; Color getColor(float frequency); int wToRGB(float w, int up, int down); }; // maybe more light spectrum frequencies
struct Color { int r, g, b; Color(int i, int j, int k) { r = i; g = j; b = k; } };

struct Object { Instance i; Material m; };

struct Material { Texture t; Surface f; };
struct Texture {};
struct Surface {}; // Diffuse (plastic), specular (metal), dielectric (glass), retroreflective (cloth) 

struct Instance { Geometry g; };
struct Geometry { Mesh m; };
struct Mesh {};



// METHODS

struct Spectrum {
	Color getColor(float w) {
		if (w > 781) { return Color(0,0,0); } // outside visible spectrum
		else if (w > 645) { return Color(255, 0, 0); }
		else if (w > 580) { return Color(255, wToRGB(w, 645, 580), 0); }
		else if (w > 510) { return Color(wToRGB(w, 645, 580), 255, 0); }
		else if (w > 490) { return Color(0, 255, wToRGB(w, 645, 580)); }
		else if (w > 440) { return Color(0, wToRGB(w, 645, 580), 255); }
		else if (w > 380) { return Color(wToRGB(w, 645, 580), 0, 255); }
		return Color(0, 0, 0); // outside visible spectrum
	};

	int wToRGB(float w, int up, int down) {
		// normalize the scale to 0 to 1 then multiply by 255
		return int(255 * (w - down) / (up - down));
	};
};
