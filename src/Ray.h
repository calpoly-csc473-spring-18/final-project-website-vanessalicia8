#ifndef RAY
#define RAY
#include <glm/glm.hpp>
class Ray{
public:
   glm::vec3 position;
   glm::vec3 direction;
   Ray(glm::vec3 pos, glm::vec3 dir);
   ~Ray();
};
#endif