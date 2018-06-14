#include "Object.h"
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, Pigment *p, Finish *f, glm::mat4 mod)
{
   slot1 = point1 = p1;
   slot2 = glm::vec3(p->color.x, p->color.y, p->color.z);
   slot3 = point2 = p2;
   slot4 = point3 = p3;
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
   pig = pg = p;
   fin = f;
}
Triangle::~Triangle(){}
float Triangle::intersect(const glm::vec3 raypos, const glm::vec3 raydir) //,float &t
{
   const glm::vec3 abc = point1 - point2;
   const glm::vec3 def = point1 - point3;
   const glm::vec3 ghi = raydir;
   const glm::vec3 jkl = point1 - raypos;
   
   const float ei_hf = def.y * ghi.z - ghi.y * def.z;
   const float gf_di = ghi.x * def.z - def.x * ghi.z;
   const float dh_eg = def.x * ghi.y - def.y * ghi.x;
   
   const float denom = abc.x * ei_hf + abc.y * gf_di + abc.z * dh_eg;
   
   if (denom == 0.f)
      return -1;
   
   const float beta = (jkl.x * ei_hf + jkl.y * gf_di + jkl.z * dh_eg) / denom;
   
   if (beta < 0 || beta > 1)
      return -1;
   
   const float ak_jb = abc.x * jkl.y - jkl.x * abc.y;
   const float jc_al = jkl.x * abc.z - abc.x * jkl.z;
   const float bl_kc = abc.y * jkl.z - jkl.y * abc.z;
   
   const float gamma = (ghi.z * ak_jb + ghi.y * jc_al + ghi.x * bl_kc) / denom;
   
   if (gamma < 0 || gamma > 1 - beta)
      return -1;
   
   const float t = (-def.z * ak_jb + -def.y * jc_al + -def.x * bl_kc) / denom;
   
   if (t < 0)
      return -1;
   
   return t;
}

glm::vec3 Triangle::getNormal(const glm::vec3 intersection)
{
   return glm::normalize(glm::cross(point2-point1,point3-point1));
}

glm::vec3 Triangle::getCenter()
{
   float bary = (1.0/3.0);
   return bary*point1+ bary*point2 + bary*point3;
}

std::string Triangle::getType()
{
   return "Triangle";
}
std::string Triangle::toString()
{
   std::ostringstream output;
   output << "- Type: Triangle\n" << "- Point1: {" << point1.x << " " << point1.y << " " << point1.z << "}\n";
   output << "- Point2: {" << point2.x << " " << point2.y << " " << point2.z << "}\n";
   output << "- Point3: {" << point3.x << " " << point3.y << " " << point3.z << "}\n";
   output << pg->toString() << "\n" << fin->toString();
   return output.str();
}
