#include "raytracer.h"

#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "quad.h"

#include "camera.h"

#include "color.h"
#include "material.h"
#include "light.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

hittable_list world;

void readVector(std::ifstream& ifs, vec3& vec);
void readMaterial(std::ifstream&, const std::string&, color&, color&, float&);
std::string readLight(std::ifstream&);
std::string readSphere(std::ifstream&);
std::string readQuad(std::ifstream&);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cerr << "Error: Incorrect number of arguments\n"
			<< "Correct: "
			<< argv[0]
			<< " [input file] " << '>' << "[output file]\n";
		return -1;
	}

	camera cam;

	std::ifstream ifs(argv[1]);
	std::string str;
	ifs >> str;

	while (ifs) {
		if (str == "ANTIALIAS") {
			ifs >> cam.samples_per_pixel >> str;
		}
		else if (str == "BACKGROUND") {
			ifs >> cam.background[0] >> cam.background[1] >> cam.background[2] >> str;
		}
		else if (str == "MAXDEPTH") {
			ifs >> cam.max_depth >> str;
		}
		else if (str == "LIGHT") {
			str = readLight(ifs);
		}
		else if (str == "SPHERE") {
			str = readSphere(ifs);
		}
		else if (str == "QUAD") {
			str = readQuad(ifs);
		}
		else if (str == "RESOLUTION") {
			ifs >> cam.image_width >> cam.image_height >> str;
		}
		else if (str.substr(0, 2) == "//") {
			std::getline(ifs, str);
			ifs >> str;
		}
		else {
			ifs >> str;
		}

	}

	cam.render(world);
}

//Different methods for reading in files

void readVector(std::ifstream& ifs, vec3& vec) {

	ifs >> vec[0] >> vec[1] >> vec[2];

}

void readMaterial(std::ifstream& ifs, const std::string& str, color& diff, color& spec, float& shiny) {

	if (str == "DIFF") {
		readVector(ifs, diff);
	}
	else if (str == "SPEC") {
		readVector(ifs, spec);
	}
	else if (str == "SHININESS")
		ifs >> shiny;

}

std::string readLight(std::ifstream& ifs) {
	point3 pos;
	color diffuse;
	color specular;

	std::string str;
	while (ifs) {
		ifs >> str;
		if (str == "POS")
			readVector(ifs, pos);
		else if (str == "DIFF")
			readVector(ifs, diffuse);
		else if (str == "SPEC")
			readVector(ifs, specular);
		else if (str.substr(0, 2) == "//")
			std::getline(ifs, str);
		else
			break;
	}

	lights.push_back(light(pos, diffuse, specular));

	return str;
}

std::string readSphere(std::ifstream& ifs) {
	point3 pos;
	float radius = 0.0f;
	color diff;
	color spec;
	float shiny;

	std::string str;
	while (ifs) {
		ifs >> str;
		if (str == "POS")
			readVector(ifs, pos);
		else if (str == "RADIUS")
			ifs >> radius;
		else if (str == "DIFF" || str == "SPEC" || str == "SHININESS")
			readMaterial(ifs, str, diff, spec, shiny);
		else if (str.substr(0, 2) == "//")
			std::getline(ifs, str);
		else
			break;
	}
	shared_ptr<custom> mat = make_shared<custom>(diff, spec, shiny);

	world.add(make_shared<sphere>(pos, radius, mat));

	return str;
}

std::string readQuad(std::ifstream& ifs) {
	std::vector<point3> corners;
	std::string str;
	color diff;
	color spec;
	float shiny;

	while (ifs) {
		ifs >> str;
		if (str == "POS") {
			point3 c;
			readVector(ifs, c);
			corners.push_back(c);
		}
		else if (str == "DIFF" || str == "SPEC" || str == "SHININESS") {
			readMaterial(ifs, str, diff, spec, shiny);
		}
		else if (str.substr(0, 2) == "//") {
			std::getline(ifs, str);
		}
		else {
			break;
		}
	}

	shared_ptr<custom> mat = make_shared<custom>(diff, spec, shiny);

	corners.push_back(corners[1] + corners[2] - corners[0]);

	world.add(make_shared<quad>(corners[0], corners[1], corners[2], mat));

	return str;
}