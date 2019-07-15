#include "Point.cpp"
#include <vector>
#include <map>
#include "Color.cpp"

typedef tuple<int, int, int> three_tuple;
struct Texture {
	map<pair<int, int>, Color> uvSpace;
	map<three_tuple, Color> xyzSpace;
};