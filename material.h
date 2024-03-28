#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracer.h"
#include "hittable_list.h"
#include "light.h"

extern point3 lookfrom;

std::vector<light> lights;

class hit_record;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, const hittable& world, color& spec) const = 0;

};

class custom : public material {

public:

    custom(color _diffuse, color _specular, float _shine) :
        diffuse(_diffuse), specular(_specular), shine(_shine) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, const hittable& world, color& spec)
        const override {

        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);

        color ret = color(0.0f, 0.0f, 0.0f);

        for (const light& _light : lights) {
            hit_record lightRec;
            vec3 toLight = unit_vector(_light.point() - rec.p);
            vec3 toEye = unit_vector(lookfrom - rec.p);
            ray lightRay = ray(rec.p, toLight);
            float minDist = (rec.p - _light.point()).length();
            vec3 lightReflect = -reflect(toLight, rec.normal);

            if (!world.hit(lightRay, interval(0.001f, minDist - 0.01f), lightRec)) {
                float diffFactor = dot(toLight, rec.normal);

                if (diffFactor > 0.001f) {
                    color diffAdd = diffuse * diffFactor * _light.diff();
                    ret += diffAdd;

                    float specFactor = dot(lightReflect, toEye);

                    if (specFactor > 0.001f) {
                        color specAdd = specular * std::pow(specFactor, shine) * _light.spec();

                        ret += specAdd;
                    }
                }
            }
        }

        spec = specular;

        attenuation = ret;

        return (dot(specular, vec3(1.0f, 1.0f, 1.0f)) > 0);
    }

private:
    color diffuse;
    color specular;
    float shine;
};

#endif