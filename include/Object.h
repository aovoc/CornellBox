#ifndef Object_h_
#define Object_h_

#include <cmath>
#include "glm_vec3_func.h"
#include "IntersectionInfo.h"
#include "Material.h"
#include "glm/glm.hpp"
#include "Ray.h"
#include "BBox.h"
using glm::vec3;

class Object {
public:
  //! All "Objects" must be able to test for intersections with rays.
  virtual bool getIntersection(
      const Ray& ray,
      IntersectionInfo* intersection)
    const = 0;

  //! Return an object normal based on an intersection
  virtual vec3 getNormal(const IntersectionInfo& I) const = 0;

  //! Return a bounding box for this object
  virtual BBox getBBox() const = 0;

  //! Return the centroid for this object. (Used in BVH Sorting)
  virtual vec3 getCentroid() const = 0;

};

#endif
