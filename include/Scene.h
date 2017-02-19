#ifndef _Scene_h_
#define _Scene_h_

#include "Material.h"
#include "Triangle.h"
#include "BVH.h"
#include <vector>
#include "erand48.inc"
#include "tiny_obj_loader.h"
class Scene
{
private:
	std::vector<Object*> tris;
	BVH bvh;

public:
	std::vector<tinyobj::shape_t> m_shapes;
	std::vector<tinyobj::material_t> m_materials;
	std::vector<Material> materials;
	Scene() = default;
	Scene(const char* filename);

	glm::vec3 radiance(const Ray &r, int depth, unsigned short *Xi);
	void add(const char* filename);
};

#endif