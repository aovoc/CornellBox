#include "BBox.h"
#include <algorithm>

BBox::BBox(const glm::vec3& min, const glm::vec3& max)
: min(min), max(max) { extent = max - min; }

BBox::BBox(const glm::vec3& p)
: min(p), max(p) { extent = max - min; }
 
void BBox::expandToInclude(const glm::vec3& p) {
	if (min.x > p.x) min.x = p.x;
	if (min.y > p.y) min.y = p.y;
	if (min.z > p.z) min.z = p.z;

	if (max.x < p.x) max.x = p.x;
	if (max.y < p.y) max.y = p.y;
	if (max.z < p.z) max.z = p.z;
	extent = max - min;
}

void BBox::expandToInclude(const BBox& b) {
	expandToInclude(b.min);
	expandToInclude(b.max);
}

uint32_t BBox::maxDimension() const {
 uint32_t result = 0;
 if(extent.y > extent.x) result = 1;
 if(extent.z > extent.y) result = 2;
 return result;
}

float BBox::surfaceArea() const {
 return 2.f*( extent.x*extent.z + extent.x*extent.y + extent.y*extent.z );
}

bool BBox::intersect(const Ray& ray, float *tnear, float *tfar) const {
	float tx1 = (min.x - ray.o.x)*ray.inv_d.x;
	float tx2 = (max.x - ray.o.x)*ray.inv_d.x;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	float ty1 = (min.y - ray.o.y)*ray.inv_d.y;
	float ty2 = (max.y - ray.o.y)*ray.inv_d.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	float tz1 = (min.z - ray.o.z)*ray.inv_d.z;
	float tz2 = (max.z - ray.o.z)*ray.inv_d.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));
	*tnear = tmin;
	*tfar = tmax;

	return tmax >= tmin;
}
