#include <cstdio>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "BVH.h"
#include "Sphere.h"
#include "Triangle.h"
#include "tiny_obj_loader.h"
#include "Scene.h"
#include "Renderer.h"
#include "Camera.h"
#include "glm/glm.hpp"
using std::vector;

std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;


int main(int argc, char **argv) {
	Scene scene("../res/scene01.obj");
	Camera camera = Camera(glm::vec3(0, 5, 14) , glm::vec3(0, 5, -14), 1024, 768);
	Renderer renderer(&scene, &camera);
	renderer.render(100, "../result/scene01.ppm");

	system("pause");
	return 0;
}
