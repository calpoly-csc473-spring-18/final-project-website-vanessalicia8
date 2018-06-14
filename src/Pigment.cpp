#include "Pigment.h"
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>

Pigment::Pigment(std::string typ, glm::vec3 col)
{
   color_type = typ;
   color = glm::vec4(col,0);
}

Pigment::Pigment(std::string typ, glm::vec4 col)
{
   color_type = typ;
   color = col;
}

Pigment::~Pigment(){}

std::string Pigment::getType()
{
   return color_type;
}

std::string Pigment::toString()
{
   std::ostringstream output;
   output << "- Color: {"<< color.x <<" "<< color.y <<" "<< color.z;
   if(color_type == "rgbf")
      output << " " << color.w;
   output << "}";
   return output.str();
}
