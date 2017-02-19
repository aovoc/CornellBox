#include "Scene.h"
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>
#include "glm/glm.hpp"

#ifndef M_PI 
#define M_PI 3.141592653589793238462643	
#endif

#define  epsilon glm::vec3(0.001, 0.001, 0.001)

Scene::Scene(const char* filename)
{
	
	std::string mtlBasePath;
	std::string inputFile = filename;
	unsigned long pos = inputFile.find_last_of("/");
	mtlBasePath = inputFile.substr(0, pos + 1);
	//const char* filename = "scene01.obj";
	std::cout << "Loading " << filename << std::endl;
	std::string err;
	bool triangulate = true;
	bool ret = tinyobj::LoadObj(m_shapes, m_materials, err, filename, mtlBasePath.c_str(), triangulate);

	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}

	printf(" - Generating BVH...\n\n");

	long shapes_size, indices_size, materials_size;
	shapes_size = m_shapes.size();
	materials_size = m_materials.size();

	// Load materials/textures from obj
	for (int i = 0; i < materials_size; i++) {
		glm::vec3 color(m_materials[i].color[0], m_materials[i].color[1], m_materials[i].color[2]);
		glm::vec3 emission(m_materials[i].emission[0], m_materials[i].emission[1], m_materials[i].emission[2]);
		materials.push_back(Material(m_materials[i].type, color, emission, m_materials[i].alpha ));
	}

	// Load triangles from obj
	for (int i = 0; i < shapes_size; i++) {
		indices_size = m_shapes[i].mesh.indices.size() / 3;
		for (size_t f = 0; f < indices_size; f++) {

			// Triangle vertex coordinates
			glm::vec3 v0_ = glm::vec3(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 2]
				);

			glm::vec3 v1_ = glm::vec3(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 2]
				);

			glm::vec3 v2_ = glm::vec3(
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 1],
				m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 2]
				);

			//Triangle normal coordinates
			glm::vec3 n0_ = glm::vec3(
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f] * 3],
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f] * 3 + 1],
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f] * 3 + 2]
				);

			glm::vec3 n1_ = glm::vec3(
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f + 1] * 3],
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 1],
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 2]
				);

			glm::vec3 n2_ = glm::vec3(
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f + 2] * 3],
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 1],
				m_shapes[i].mesh.normals[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 2]
				);

			tris.push_back(new Triangle(v0_, v1_, v2_ , n0_, n1_, n2_, &materials[m_shapes[i].mesh.material_ids[f]]));
		}
	}

	bvh = BVH(&tris);
	printf("Load the scene successfully!\n");
}



glm::vec3 Scene::radiance(const Ray &r, int depth, unsigned short *Xi){
	IntersectionInfo I;
	if (!bvh.getIntersection(r, &I, false)) 
		return glm::vec3(); // if miss, return black
	glm::vec3 f = I.mtl->color;
	float p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
	glm::vec3 n = normalize(I.object->getNormal(I));
	glm::vec3 nl = dot(n, r.d) < 0 ? n : n*(float)(-1);
	//	n = (x - obj.p).norm(), nl = n.dot(r.d) < 0 ? n : n*-1, f = obj.c;
	if (++depth > 5) 
		if (erand48(Xi) < p)
			f = f*(1 / p); 
		else return I.mtl->emission; //R.R.
	if (depth > 100) return I.mtl->emission; 
	
	if (I.mtl->type == DIFF){                  // Ideal DIFFUSE reflection
		float r1 = 2 * M_PI*erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
		glm::vec3 w = nl, 
			u = normalize(cross((fabs(w.x) > .1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), w)), 
			v = cross(w,u);
		glm::vec3 d = normalize(u*(float)cos(r1) * r2s + v*sin(r1)*r2s + w*sqrt(1 - r2));
		return I.mtl->emission + mult(f, radiance(Ray(I.hit - r.d * epsilon, d), depth, Xi));
	}
	else if (I.mtl->type == SPEC)            // Ideal SPECULAR reflection
		return I.mtl->emission + mult(f, radiance(Ray(I.hit - r.d * epsilon, normalize(r.d - n * (float)2 * dot(n, r.d))), depth, Xi));



	else if (I.mtl->type == REFR){
		Ray reflRay(I.hit - r.d * epsilon, r.d - n * (float)2 * dot(n, r.d));     // Ideal dielectric REFRACTION
		bool into = dot(n, nl) > 0;                // Ray from outside going in?
		float nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = dot(r.d, nl), cos2t;
		if ((cos2t = 1 - nnt*nnt*(1 - ddn*ddn)) < 0)    // Total internal reflection
			return I.mtl->emission + mult(f, radiance(reflRay, depth, Xi));
		glm::vec3 tdir = normalize(r.d*nnt - n*((into ? 1 : -1)*(ddn*nnt + sqrt(cos2t))));
		float a = nt - nc, b = nt + nc, R0 = a*a / (b*b), c = 1 - (into ? -ddn : dot(tdir, n));
		float Re = R0 + (1 - R0)*c*c*c*c*c, Tr = 1 - Re, P = .25 + .5*Re, RP = Re / P, TP = Tr / (1 - P);
		return I.mtl->emission + mult(f, (depth > 2 ? (erand48(Xi) < P ?   // Russian roulette
			radiance(reflRay, depth, Xi)*RP : radiance(Ray(I.hit + r.d * epsilon, tdir), depth, Xi)*TP) :
			radiance(reflRay, depth, Xi)*Re + radiance(Ray(I.hit + r.d * epsilon, tdir), depth, Xi)*Tr));
	}

	float r1 = 2 * M_PI*erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
	glm::vec3 w = nl,
		u = normalize(cross((fabs(w.x) > .1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), w)),
		v = cross(w, u);
	glm::vec3 d = normalize(u*(float)cos(r1) * r2s + v*sin(r1)*r2s + w*sqrt(1 - r2));



	return I.mtl->emission + I.mtl->alpha * mult(f, radiance(Ray(I.hit - r.d * epsilon, d), depth, Xi))
		+(1-I.mtl->alpha) * mult(f, radiance(Ray(I.hit - r.d * epsilon, normalize(r.d - n * (float)2 * dot(n, r.d))), depth, Xi));
		
}


void Scene::add(const char* filename){
}