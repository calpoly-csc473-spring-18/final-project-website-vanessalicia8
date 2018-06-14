//#include "Camera.h" //idk
#include "Object.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

Camera::Camera(glm::vec3 loc, glm::vec3 u, glm::vec3 r, glm::vec3 look)
{
   slot1 = location = loc;
   slot2 = up = u;
   slot3 = right = r;
   slot4 = look_at = look;
}

Camera::~Camera(){}
float Camera::intersect(const glm::vec3 raypos, const glm::vec3 raydir){
   return -1;
}
glm::vec3 Camera::getNormal(const glm::vec3 intersection)
{
   return glm::vec3(0);
}
std::string Camera::getType()
{
   return "camera";
}
glm::vec3 Camera::getCenter()
{
   return glm::vec3(0);
}

std::string Camera::toString()
{
   std::ostringstream output;
   output << "- Location: {" << location.x <<" "<< location.y <<" "<< location.z << "}\n";
   output << "- Up: {"<< up.x <<" "<< up.y << " "<< up.z << "}\n";
   output << "- Right: {"<< right.x << " "<< right.y << " "<< right.z << "}\n";
   output << "- Look at: {"<< look_at.x << " " << look_at.y <<" "<< look_at.z << "}";
   return output.str();
}

