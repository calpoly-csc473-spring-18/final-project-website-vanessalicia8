#include "Object.h"
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

Plane::Plane(glm::vec3 norm, float d, Pigment *p, Finish *f, glm::mat4 mod)
{
   slot1 = normal = norm;
   slot2 = glm::vec3(p->color.x, p->color.y, p->color.z);
   save1 = f->ambient;
   save2 = f->diffuse;
   save3 = f->specular;
   save4 = f->roughness;
   save5 = f->reflection;
   save6 = f->refraction;
   save7 = f->index;
   model = mod;
   inv_model = glm::inverse(model);
   normal_mat = glm::transpose(inv_model);
   dist = d;
   pig = pg = p;
   fin = f;
}
Plane::~Plane(){}
float Plane::intersect(const glm::vec3 raypos, const glm::vec3 raydir)
{
   glm::vec3 o = raypos;
   glm::vec3 d = glm::normalize(raydir);
   float numer = dist - glm::dot(o,glm::normalize(normal));
   float denom = glm::dot(d,glm::normalize(normal));
   float t = numer/denom;
   return t;
}

glm::vec3 Plane::getNormal(const glm::vec3 intersection)
{
   return glm::normalize(normal);
}
glm::vec3 Plane::getCenter()
{
   return glm::vec3(0);
}

std::string Plane::getType()
{
   return "Plane";
}
std::string Plane::toString()
{
   std::ostringstream output;
   output << "- Type: Plane\n" << "- Normal: {" << normal.x <<" "<< normal.y <<" "<< normal.z << "}\n" << "- Distance: " << dist << "\n" << pg->toString() << "\n"<< fin->toString();
   return output.str();
}



