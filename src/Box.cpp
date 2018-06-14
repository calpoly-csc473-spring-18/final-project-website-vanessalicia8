#include "Object.h"
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <glm/gtc/epsilon.hpp>

Box::Box(glm::vec3 mini, glm::vec3 maxi, Pigment *p, Finish *f, glm::mat4 mod)
{
   slot1 = min = mini;
   slot2 = glm::vec3(p->color.x, p->color.y, p->color.z);
   slot3 = max = maxi;
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
Box::~Box(){}
float Box::intersect(const glm::vec3 raypos, const glm::vec3 raydir) //,float &t
{
   glm::vec3 P0 = raypos;
   glm::vec3 d = raydir;
   
   if(d.x==0)
   {
      if(P0.x < min.x || P0.x > max.x)
         return -1;
   }
   if(d.y==0)
   {
      if(P0.y < min.y || P0.y > max.y)
         return -1;
   }
   if(d.z==0)
   {
      if(P0.z < min.z || P0.z > max.z)
         return -1;
   }
   
   float txmin = (min.x - P0.x)/d.x;
   float tymin = (min.y - P0.y)/d.y;
   float tzmin = (min.z - P0.z)/d.z;
   float txmax = (max.x - P0.x)/d.x;
   float tymax = (max.y - P0.y)/d.y;
   float tzmax = (max.z - P0.z)/d.z;
   

   if(txmin > txmax)
      std::swap(txmin, txmax);
   
   if(tymin > tymax)
      std::swap(tymin, tymax);

   if(tzmin > tzmax)
      std::swap(tzmin, tzmax);

   float smallestMax = std::min(std::min(txmax,tymax),tzmax);
   float largestMin = std::max(std::max(txmin,tymin),tzmin);
   
   if(smallestMax < largestMin || smallestMax < 0)
      return -1;

   if (largestMin > 0)
      return largestMin;
   else
      return smallestMax; //if inside box, tgmax is exit intersection
}


 glm::vec3 Box::getNormal(const glm::vec3 intersection)
 {
    if(glm::epsilonEqual(intersection.x,min.x,0.0001f))
       return glm::vec3(-1, 0, 0);
    else if(glm::epsilonEqual(intersection.x,max.x,0.0001f))
       return glm::vec3(1, 0, 0);
    else if(glm::epsilonEqual(intersection.y,min.y,0.0001f))
       return glm::vec3(0, -1, 0);
    else if(glm::epsilonEqual(intersection.y,max.y,0.0001f))
       return glm::vec3(0, 1, 0);
    else if(glm::epsilonEqual(intersection.z,min.z,0.0001f))
       return glm::vec3(0, 0, -1);
    else //if(glm::epsilonEqual(intersection.z,max.z,0.0001f))
       return glm::vec3(0, 0, 1);
 }
glm::vec3 Box::getCenter()
{
   return glm::vec3((min.x+max.x)/2.0,(min.y+max.y)/2.0,(min.z+max.z)/2.0);
}

std::string Box::getType()
{
   return "Box";
}
std::string Box::toString()
{
   std::ostringstream output;
   output << "- Type: Box\n" << "- Minimum: {" << min.x << " " << min.y << " " << min.z << "}\n" << "- Maximum: {"<< max.x << " " << max.y << " " << max.z << "}\n" << pg->toString() << "\n" << fin->toString();
   return output.str();
}
