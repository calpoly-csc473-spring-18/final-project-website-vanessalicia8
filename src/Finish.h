#ifndef FINISH
#define FINISH
#include <string>

class Finish{
public:
   float ambient;
   float diffuse;
   float specular;
   float roughness;
   float reflection;
   float refraction;
   float index;
   Finish(float amb, float dif, float spec, float rough, float refl, float refr, float ior);
   ~Finish();
   virtual std::string toString();
};
#endif