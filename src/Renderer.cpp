#include <vector>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "Scene.h"
#include "Renderer.h"

// Clamp float to min/max of 0/1
inline float clamp(float x){ return x < 0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(float x){ return int(clamp(x) * 255 + .5); }

Renderer::Renderer(Scene *scene, Camera *camera) {
	m_scene = scene;
	m_camera = camera;
	m_pixel_buffer = new glm::vec3[m_camera->get_width()*m_camera->get_height()];
}

void Renderer::render(int samples, const char *file_path) {
	int width = m_camera->get_width();
	int height = m_camera->get_height();

	clock_t start = clock();

#pragma omp parallel for schedule(dynamic, 1)       // OpenMP
	for (int y = 0; y < height; y++){                       // Loop over image rows
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples, 100.*y / (height - 1));
		unsigned short Xi[3] = { 0, 0, y*y*y };
		for (unsigned short x = 0; x < width; x++){   // Loop cols
			glm::vec3 color;
			for (int k = 0; k < samples; k++){
				Ray ray = m_camera->get_ray(x, y, k > 0, Xi);
				color = color +  m_scene->radiance(ray, 0, Xi);
			}
			m_pixel_buffer[(y)*width + x] = color / (float)samples;
		}
	}
	printf("\n%f sec\n", (float)(clock() - start) / CLOCKS_PER_SEC); 
	FILE *f = fopen(file_path, "w");         // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	for (int i = 0; i < width*height; i++){
		fprintf(f, "%d %d %d ", toInt(m_pixel_buffer[i].x), \
			toInt(m_pixel_buffer[i].y), toInt(m_pixel_buffer[i].z));
	}
}
