#include "Finish.h"
#include <string>
#include <iostream>
#include <sstream>

Finish::Finish(float amb, float dif, float spec, float rough, float refl, float refr, float ior)
{
   ambient = amb;
   diffuse = dif;
   specular = spec;
   roughness = rough;
   reflection = refl;
   refraction = refr;
   index = ior;
}

Finish::~Finish(){}

std::string Finish::toString()
{
   std::ostringstream output;
   output << "- Material:\n  - Ambient: " << ambient << "\n  - Diffuse: " << diffuse;
   if(specular != 0)
      output << "\n  - Specular: " << specular;
   if(roughness != 0)
      output << "\n  - Roughness: " << roughness;
   if(reflection != 0)
      output << "\n  - Reflection: " << reflection;
   if(refraction != 0)
      output << "\n  - Refraction: " << refraction;
   if(index != 0)
      output << "\n  - IOR: " << index;
   return output.str();
}
