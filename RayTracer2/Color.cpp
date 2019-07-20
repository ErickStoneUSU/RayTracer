#pragma once
using namespace std;
class Color { 
public:
	Color() { r = 0; g = 0; b = 0; }
	Color(int i, int j, int k) { r = i; g = j; b = k; } 
	int r, g, b;
	inline Color operator+(const Color& a) {
		Color c(a.r + r, a.g + g, a.b + b);
		c.r = (c.r < 0) ? 0 : (c.r > 255) ? 255 : c.r;
		c.g = (c.g < 0) ? 0 : (c.g > 255) ? 255 : c.g;
		c.b = (c.b < 0) ? 0 : (c.b > 255) ? 255 : c.b;
		return c;
	}
	inline Color operator*(const float& a) {
		float ar = a * r;
		float ag = a * g;
		float ab = a * b;
		return Color(
			(ar < 0) ? 0 : (ar > 255) ? 255 : ar,
			(ag < 0) ? 0 : (ag > 255) ? 255 : ag,
			(ab < 0) ? 0 : (ab > 255) ? 255 : ab
		);
	}

	// https://en.wikipedia.org/wiki/Light for the values
	Color getColor(float w) {
		if (w > 900) { return Color(0, 0, 0); } // outside visible spectrum
		else if (w > 620) { return Color(255, 0, 0); }
		else if (w > 590) { return Color(255, wToRGB(w, 620, 590), 0); }
		else if (w > 570) { return Color(wToRGB(w, 590, 570), 255, 0); }
		else if (w > 495) { return Color(0, 255, wToRGB(w, 570, 495)); }
		else if (w > 450) { return Color(0, wToRGB(w, 495, 450), 255); }
		else if (w > 380) { return Color(wToRGB(w, 450, 380), 0, 255); }
		return Color(0, 0, 0); // outside visible spectrum
	};

	int wToRGB(float w, int up, int down) {
		// normalize the scale to 0 to 1 then multiply by 255
		return int(255 * ((w - down) / (up - down)));
	};
};