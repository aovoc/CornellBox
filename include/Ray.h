#ifndef Ray_h
#define Ray_h

#include "glm/glm.hpp"

struct Ray {
 glm::vec3 o; // Ray Origin
 glm::vec3 d; // Ray Direction
 glm::vec3 inv_d; // Inverse of each Ray Direction component

 Ray() = default;
 Ray(const glm::vec3& o, const glm::vec3& d)
 : o(o), d(d), inv_d(glm::vec3(1/d.x, 1/d.y, 1/d.z)) { }
};

#endif
