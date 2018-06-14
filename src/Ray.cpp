#include "Ray.h"
#include <glm/glm.hpp>

Ray::Ray(glm::vec3 pos, glm::vec3 dir)
{
   position = pos;
   direction = dir;
}
Ray::~Ray(){}
