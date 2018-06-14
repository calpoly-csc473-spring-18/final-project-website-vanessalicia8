#ifndef PIGMENT
#define PIGMENT
#include <string>
#include <glm/glm.hpp>

class Pigment{
public:
   std::string color_type;
   glm::vec4 color;
   Pigment(std::string typ, glm::vec3 col);
   Pigment(std::string typ, glm::vec4 col);
   ~Pigment();
   virtual std::string getType();
   virtual std::string toString();
};

#endif