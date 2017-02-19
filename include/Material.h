#ifndef _Material_h_
#define _Material_h_

#include "glm/glm.hpp"


enum MaterialType {
	DIFF, SPEC, REFR, ROUGHPLASTIC};


class Material {

public:
	MaterialType type;
	glm::vec3 color;
	glm::vec3 emission;
	float alpha;

public:
	Material() = default;
	Material(MaterialType type_, glm::vec3 color_, glm::vec3 emission_, float alpha_ = 0)
		: type(type_), color(color_), emission(emission_), alpha(alpha_){}

};


#endif // !Material_h_
