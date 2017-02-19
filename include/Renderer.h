#ifndef _Renderer_h_
#define _Renderer_h_

#include "glm/glm.hpp"
#include "Scene.h"
#include "camera.h"

class Renderer {

private:
	Scene *m_scene;
	Camera *m_camera;
	glm::vec3 *m_pixel_buffer;

public:
	Renderer(Scene *scene, Camera *camera);
	void render(int samples = 100, const char *file_path = "image.ppm");
};

#endif