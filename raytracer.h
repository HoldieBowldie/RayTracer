#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <limits>
#include <memory>

#include <cstdlib>
#include <random>

//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

//Degrees to Radians
inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0f;
}

//Common header
#include "interval.h"

#include "ray.h"
#include "vec3.h"

#endif