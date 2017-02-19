#ifndef Triangle_h_
#define Triangle_h_


#include <cmath>
#include "Object.h"
#include "glm_vec3_func.h"
using glm::vec3;


struct Triangle : public Object {
	vec3 v0, v1, v2;
	vec3 n0, n1, n2;
	vec3 e1, e2;
	vec3 n;
	Material *mtl;
	vec3 center; // Center of the Triangle;

	Triangle() = default;
	Triangle(vec3 v0_, vec3 v1_, vec3 v2_, 
		vec3 n0_, vec3 n1_, vec3 n2_,
		Material *m_ = NULL)
		:v0(v0_), v1(v1_), v2(v2_), 
		n0(n0_), n1(n1_), n2(n2_),
		mtl(m_)
	{
		e1 = v1 - v0, e2 = v2 - v0;
		n = normalize(cross(e1, e2));
		center = (v0 + v1 + v2) / (float)3;
	}

	bool getIntersection(const Ray& ray, IntersectionInfo* I) const {
		float u, v, t_temp = 0;

		vec3 pvec = cross(ray.d, e2);
		double det = dot(e1, pvec);
		if (det == 0) return false;
		double invDet = 1. / det;
		vec3 tvec = ray.o - v0;
		u = dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;
		vec3 qvec = cross(tvec, e1);
		v = dot(ray.d, qvec) * invDet;
		if (v < 0 || u + v > 1) return false;
		t_temp = dot(e2, qvec) * invDet; // Set distance along ray to intersection
		if (t_temp > 1e-9){    // Fairly arbritarily small value, scared to change
			I->t = t_temp;     // it as it works.
			I->object = this;
			I->hit = ray.o + ray.d * I->t;
			I->mtl = mtl;
			I->u = u;
			I->v = v;
			return true;
		}
		return false;
	}

	vec3 getNormal(const IntersectionInfo& I) const {
		return (1 - I.u - I.v) * n0 + I.u * n1 + I.v * n2;
	}

	BBox getBBox() const {
		vec3 min = vec3(
			std::fmin(std::fmin(v0.x, v1.x), v2.x),
			std::fmin(std::fmin(v0.y, v1.y), v2.y),
			std::fmin(std::fmin(v0.z, v1.z), v2.z)
			);
		vec3 max = vec3(
			std::fmax(std::fmax(v0.x, v1.x), v2.x),
			std::fmax(std::fmax(v0.y, v1.y), v2.y),
			std::fmax(std::fmax(v0.z, v1.z), v2.z)
			);
		return BBox(min, max);
	}

	vec3 getCentroid() const {
		return center;
	}

};



#endif 