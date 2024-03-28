#ifndef CAMERA_H
#define CAMERA_H

#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb\stb_image_write.h"

#include "raytracer.h"

#include "color.h"
#include "hittable.h"

#include "material.h"

#include <iostream>

point3 lookfrom = point3(0, 0, -350);

class camera {
public:
	float aspect_ratio = 1.0;
	int image_width = 100;
	int image_height = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;
	color background = color(0, 0, 0);

	float vfov = 90;

	point3 lookat = point3(0, 0, 0);
	vec3   vup = vec3(0, 1, 0);

	float defocus_angle = 0;
	float focus_dist = 10;

	hittable_list lights;

    void render(const hittable& world) {
		initialize();

		omp_set_num_threads(3);

		std::vector<color> colors(image_width * image_height);

		//std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		#pragma omp parallel for collapse(3)
		for (int j = 0; j < image_height; ++j) {
			//std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0, 0, 0);

				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = get_ray(i, j, sample);
					pixel_color += ray_color(r, max_depth, world);
				}

				colors[i + (image_width * j)] = pixel_color;
			}
		}

		uint8_t* pixel_colors = new uint8_t[image_width * image_height * 3];

		uint32_t curr = 0;

		for (color pixel : colors) {
			//Test to write to ppm file
			//write_color(std::cout, pixel, samples_per_pixel);

			float r = pixel.x();
			float g = pixel.y();
			float b = pixel.z();

			float scale = 1.0f / samples_per_pixel;

			r *= scale;
			g *= scale;
			b *= scale;

			r = linear_to_gamma(r);
			g = linear_to_gamma(g);
			b = linear_to_gamma(b);

			static const interval intensity(0.000f, 0.999f);

			//Write the value of each RGB component
			pixel_colors[++curr] = static_cast<uint8_t>(256 * intensity.clamp(g));
			pixel_colors[++curr] = static_cast<uint8_t>(256 * intensity.clamp(b));
			pixel_colors[++curr] = static_cast<uint8_t>(256 * intensity.clamp(r));
		}
		
		stbi_write_png("image.png", image_width, image_height, 3, pixel_colors, image_width * 3);
		

		std::clog << "\rDone.                 \n";

    }

private:
	point3 camera_center;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	point3 pixel00_loc;

	vec3 u, v, w;

	vec3   defocus_disk_u;
	vec3   defocus_disk_v;

    void initialize() {
		//Camera
		camera_center = lookfrom;

		float focal_length = (lookfrom - lookat).length();

		float theta = degrees_to_radians(vfov);
		float h = tan(theta / 2);
		float viewport_height = 2 * h * focus_dist;

		float viewport_width = viewport_height * (static_cast<float>(image_width) / static_cast<float>(image_height));

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		vec3 viewport_u = viewport_width * u;;
		vec3 viewport_v = viewport_height * -v;

		pixel_delta_u = viewport_u / static_cast<float>(image_width);
		pixel_delta_v = viewport_v / static_cast<float>(image_height);

		point3 viewport_upper_left = camera_center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

	color ray_color(const ray& r, int depth, const hittable& world) const {
		hit_record rec;

		if (depth <= 0) {
			return color(0, 0, 0);
		}

		if (world.hit(r, interval(0.001f, infinity), rec)) {
			ray scattered;
			color attenuation;
			color k_spec;
			if (rec.mat->scatter(r, rec, attenuation, scattered, world, k_spec)) {
				attenuation += k_spec * ray_color(scattered, depth - 1, world);
			}
			return attenuation;
		}

		return background;

	};

	ray get_ray(int i, int j, int sample) const {
		point3 pixel_center = pixel00_loc + (static_cast<float>(i) * pixel_delta_u) + (static_cast<float>(j) * pixel_delta_v);
		point3 pixel_sample = pixel_center + pixel_sample_square(sample);

		point3 ray_origin = camera_center;
		vec3 ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	//For supersampling
	vec3 pixel_sample_square(int sample) const {
		int grids = (samples_per_pixel / 2) + 1;

		float px = sample / (float)grids;
		float py = sample / (float)grids;

		px -= 0.5f;
		py -= -0.5f;

		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}
};

#endif