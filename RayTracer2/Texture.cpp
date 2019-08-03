#pragma once
#include "Point.cpp"
#include <vector>
#include "Color.cpp"

struct Texture {
	vector<Color> pattern;
	bool isUV = false;

	void setPatternToCheckers() {
		Color white = Color(255, 255, 255);
		Color black;
		pattern.clear();
		pattern.push_back(white);
		pattern.push_back(black);
	}

	Color getColor(Point nextPoint) {
		Point n = nextPoint.norm();
		if (isUV) {
			// do uv transform to spherical coordinates
			// the x4 was a hack to give some variety
			const float u = 0.5 + atan2(n.z, n.x) * 0.1591549; // 1 / (2 * PI)
			const float v = 0.5 - asin(n.y) * 0.318309; // 1 / PI
			const int index = abs(int(u + v)) % int(pattern.size());
			return pattern[index];
		}
		else {
			const int x = int(nextPoint.x) % int(pattern.size());
			const int y = int(nextPoint.y) % int(pattern.size());
			const int z = int(nextPoint.z) % int(pattern.size());
			const int index = abs(x + y + z) % int(pattern.size());
			return pattern[index];
		}
	}
};