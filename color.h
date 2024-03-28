#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>


using color = vec3;

inline float linear_to_gamma(float linear_component)
{
	return sqrt(linear_component);
}


//Only used in testing
void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {

	float r = pixel_color.x();
	float g = pixel_color.y();
	float b = pixel_color.z();

	float scale = 1.0f / samples_per_pixel;

	r *= scale;
	g *= scale;
	b *= scale;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const interval intensity(0.000f, 0.999f);

	out << static_cast<int>(256 * intensity.clamp(r)) << ' '
		<< static_cast<int>(256 * intensity.clamp(g)) << ' '
		<< static_cast<int>(256 * intensity.clamp(b)) << '\n';

}

#endif