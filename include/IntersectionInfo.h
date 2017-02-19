#ifndef IntersectionInfo_h_
#define IntersectionInfo_h_

class Object;
class Material;

struct IntersectionInfo {
	float u, v;
	float t; // Intersection distance along the ray
	const Object* object; // Object that was hit
	glm::vec3 hit; // Location of the intersection
	const Material* mtl;
};

#endif
