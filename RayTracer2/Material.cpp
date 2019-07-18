#pragma once
#include "Texture.cpp"

struct Surface { float color; }; // Diffuse (plastic), specular (metal), dielectric (glass), retroreflective (cloth) 
struct Material { Texture t; Surface f; float thickness = 1; }; 
// default thickness to be space for index of refraction