#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {

public:

	triangle(point3 _c0, point3 _c1, point3 _c2, shared_ptr<material> _material)
		: c0(_c0), c1(_c1), c2(_c2), mat(_material) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        //std::clog << '\n' << c0 << ' ' << c1 << ' ' << c2 << '\n';

        vec3 p = c1 - c0;
        vec3 q = c2 - c0;

        vec3 s = r.origin() - c0;

        vec3 tmp1 = cross(r.direction(), q);
        float dot1 = dot(tmp1, p);

        if (-0.0001f < dot1 && dot1 < 0.0001f)
            return false;

        float f = 1.0f / dot1;
        float u = f * dot(s, tmp1);
        if (u < 0.0f || u > 1.0f)
            return false;
        vec3 tmp2 = cross(s, p);
        float v = f * dot(r.direction(), tmp2);
        if (v < 0.0f || u + v > 1.0f)
            return false;

        rec.t = f * dot(q, tmp2);
        rec.p = r.at(rec.t);

        vec3 outward_normal = unit_vector(cross(c0 - c1, c0 - c2));
        rec.set_face_normal(r, outward_normal);

        rec.mat = mat;

        return true;

	}

private:

	point3 c0;
	point3 c1;
	point3 c2;

	shared_ptr<material> mat;

};

#endif
