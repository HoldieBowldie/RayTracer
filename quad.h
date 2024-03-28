#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "vec3.h"

class quad : public hittable {

public:

    quad(point3 _v0, point3 _v1, point3 _v2, shared_ptr<material> _material)
        : v0(_v0), v1(_v1), v2(_v2), mat(_material) {

        q = v0;
        u = v1 - v0;
        v = v2 - v0;

        vec3 n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, q);
        w = n / dot(n, n);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        float denom = dot(normal, r.direction());

        if (fabs(denom) < 1e-8)
            return false;

        float t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        point3 intersection = r.at(t);
        
        vec3 planar_hitpt_vector = intersection - q;
        float alpha = dot(w, cross(planar_hitpt_vector, v));
        float beta = dot(w, cross(u, planar_hitpt_vector));

        if ((alpha < 0) || (1 < alpha) || (beta < 0) || (1 < beta)) {
            return false;
        }
        
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

private:

    point3 v0;
    point3 v1;
    point3 v2;

    point3 q;
    vec3 u;
    vec3 v;


    shared_ptr<material> mat;

    vec3 normal;
    float D;
    vec3 w;

};

#endif
