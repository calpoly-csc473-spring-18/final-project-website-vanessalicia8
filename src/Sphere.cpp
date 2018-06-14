#include "Object.h"
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

Sphere::Sphere(glm::vec3 cent, float r, Pigment *p, Finish *f, glm::mat4 mod)
{
   slot1 = center = cent;
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
   save8 = rad = r;
   pig = pg = p;
   fin = f;
}
Sphere::~Sphere(){}
float Sphere::intersect(const glm::vec3 raypos, const glm::vec3 raydir) //,float &t
{
   const glm::vec3 offset = raypos - center;
   const float a = glm::dot(raydir, raydir);
   const float b = 2.f * glm::dot(raydir, offset);
   const float c = glm::dot(offset, offset) - rad*rad;
   
   const float discriminant = b * b - 4.f * a * c;
   
   if (discriminant >= 0.f)
   {
      const float sqrtDiscriminant = sqrt(discriminant);
      float numerator = -b - sqrtDiscriminant;
      if (numerator < 0)
      {
         numerator = -b + sqrtDiscriminant;
      }
      const float t = numerator / (2.f * a);
      if (t >= 0.f)
      {
         return t;
      }
   }
   return -1;
}

glm::vec3 Sphere::getNormal(const glm::vec3 intersection)
{
   return glm::normalize(intersection - center);
}
glm::vec3 Sphere::getCenter()
{
   return center;
}


std::string Sphere::getType()
{
   return "Sphere";
}
std::string Sphere::toString()
{
   std::ostringstream output;
   output << "- Type: Sphere\n" << "- Center: {" << center.x << " " << center.y << " " << center.z << "}\n" << "- Radius: "<< rad << "\n" << pg->toString() << "\n" << fin->toString();
   return output.str();
}
