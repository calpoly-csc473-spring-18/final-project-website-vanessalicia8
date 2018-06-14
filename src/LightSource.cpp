#include "Object.h"
#include "Pigment.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

LightSource::LightSource(glm::vec3 pos, Pigment *p)
{
   slot1 = position = pos;
   slot2 = glm::vec3(p->color.x, p->color.y, p->color.z);
   pg = p;
}
LightSource::~LightSource(){}
float LightSource::intersect(const glm::vec3 raypos, const glm::vec3 raydir)
{
   return -1;
}
glm::vec3 LightSource::getNormal(const glm::vec3 intersection)
{
   return glm::vec3(0);
}
glm::vec3 LightSource::getCenter()
{
   return glm::vec3(0);
}
std::string LightSource::getType()
{
   return "light_source";
}
std::string LightSource::toString()
{
   std::ostringstream output;
   output << "- Location: {" << position.x << " "<< position.y << " "<< position.z << "}\n" << pg->toString();
   return output.str();
}


