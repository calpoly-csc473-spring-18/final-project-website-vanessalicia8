#include "Object.h"
#include <glm/glm.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>

AABB::AABB()
{
   ;
}

void AABB::Reset(glm::vec3 pt) {
   min = max = pt;
}
   
void AABB::AddPoint(glm::vec3 pt) {
   min.x = glm::min(min.x, pt.x);
   min.y = glm::min(min.y, pt.y);
   min.z = glm::min(min.z, pt.z);
   max.x = glm::max(max.x, pt.x);
   max.y = glm::max(max.y, pt.y);
   max.z = glm::max(max.z, pt.z);
}
   
void AABB::AddBox(AABB other) {
   AddPoint(other.min);
   AddPoint(other.max);
}
glm::vec3 AABB::getNormal(const glm::vec3 intersection)
{
   return glm::vec3(0);
}

glm::vec3 AABB::getCenter()
{
   return glm::vec3((min.x+max.x)/2.0,(min.y+max.y)/2.0,(min.z+max.z)/2.0);
}

std::vector<glm::vec3> AABB::compute_8_vertices()
{
   std::vector<glm::vec3> vertices;
   vertices.push_back(min);
   vertices.push_back(glm::vec3(min.x,min.y,max.z));
   vertices.push_back(glm::vec3(min.x,max.y,min.z));
   vertices.push_back(glm::vec3(max.x,min.y,min.z));
   vertices.push_back(glm::vec3(max.x,max.y,min.z));
   vertices.push_back(glm::vec3(max.x,min.y,max.z));
   vertices.push_back(glm::vec3(min.x,max.y,max.z));
   vertices.push_back(max);
   return vertices;
}

float AABB::intersect(const glm::vec3 raypos, const glm::vec3 raydir)
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
std::string AABB::getType()
{
   return "AABB";
}
std::string AABB::toString()
{
   return "AABB";
}