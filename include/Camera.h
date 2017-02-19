#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "erand48.inc"
#include "glm/glm.hpp"
#include "ray.h"

class Camera {

private:
	int m_width;
	double m_width_recp;
	int m_height;
	double m_height_recp;
	double m_ratio;
	double m_x_spacing;
	double m_x_spacing_half;
	double m_y_spacing;
	double m_y_spacing_half;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_x_direction;
	glm::vec3 m_y_direction;

public:
	Camera(glm::vec3 position, glm::vec3 target, int width, int height);
	int get_width();
	int get_height();
	Ray get_ray(int x, int y, bool jitter, unsigned short *Xi);

};

#endif //CAMERA_H