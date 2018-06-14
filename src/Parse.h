#ifndef PARSE
#define PARSE
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include "Object.h"
#include "Pigment.h"
#include "Finish.h"

class Parse
{

public:

	Parse() {}

   static glm::vec3 Vect3(std::istringstream & Stream);
   static glm::vec4 Vect4(std::istringstream & Stream);
   std::vector<Object *> ParseString(std::istringstream & FileContents);
   Object * ParseCamera(std::istringstream & Stream);
   glm::mat4 ParseScale(std::istringstream & Stream, glm::mat4 model);
   glm::mat4 ParseRotate(std::istringstream & Stream, glm::mat4 model);
   glm::mat4 ParseTranslate(std::istringstream & Stream, glm::mat4 model);
   Pigment * ParsePigment(std::istringstream & Stream);
   Finish * ParseFinish(std::istringstream & Stream);
   Object * ParseSphere(std::istringstream & Stream);
   Object * ParsePlane(std::istringstream & Stream);
   Object * ParseTriangle(std::istringstream & Stream);
   Object * ParseBox(std::istringstream & Stream);
   Object * ParseLight(std::istringstream & Stream);
   
};
#endif