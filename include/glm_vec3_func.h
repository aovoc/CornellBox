#ifndef glm_vec3_func_h_
#define glm_vec3_func_h_

inline glm::vec3 mult(glm::vec3 a, glm::vec3 b){ 
	return glm::vec3(a.x*b.x, a.y*b.y, a.z*b.z); }

inline float dot(glm::vec3 a, glm::vec3 b){
	return a.x * b.x + a.y *b.y + a.z * b.z;
}

inline glm::vec3 normalize(glm::vec3 a){
	return glm::vec3(a / sqrt(dot(a, a)));
}

inline glm::vec3 cross(glm::vec3 a, glm::vec3 b){
	return glm::vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

#endif