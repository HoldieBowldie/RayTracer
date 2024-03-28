#ifndef LIGHT_H
#define LIGHT_H

#include "hittable.h"
#include "vec3.h"
#include "color.h"

class light {

public:

	light(point3 _center, color _diffuse, color _specular) :
		center(_center), diffuse(_diffuse), specular(_specular) {}

	point3 point() const { return center; }

	color diff() const { return diffuse; }

	color spec() const { return specular; }

private:

	point3 center;
	color diffuse;
	color specular;

};

#endif