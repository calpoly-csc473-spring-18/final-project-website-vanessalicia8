#include "Parse.h"
#include "Object.h"
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

using namespace std;


glm::vec3 Parse::Vect3(std::istringstream & Stream)
{
   glm::vec3 v;
   v.x = v.y = v.z = 0.f;
   std::stringbuf buf;
   
   Stream.ignore(numeric_limits<streamsize>::max(), '<');
   Stream.get(buf, '>');
   Stream.ignore(numeric_limits<streamsize>::max(), '>');
   
   std::string line = buf.str(); // be careful...
   int read = sscanf(line.c_str(), "%f, %f, %f", &v.x, &v.y, &v.z);
   
   if (read != 3)
   {
      cerr << "Expected to read 3 vector elements but found '" << line << "'" << endl;
   }
   
   return v;
}

glm::vec4 Parse::Vect4(std::istringstream & Stream)
{
   glm::vec4 v;
   v.x = v.y = v.z = v.a = 0.f;
   std::stringbuf buf;
   
   Stream.ignore(numeric_limits<streamsize>::max(), '<');
   Stream.get(buf, '>');
   Stream.ignore(numeric_limits<streamsize>::max(), '>');
   
   std::string line = buf.str(); // be careful...
   int read = sscanf(line.c_str(), "%f, %f, %f, %f", &v.x, &v.y, &v.z, &v.w);
   if (read != 4)
   {
      cerr << "Expected to read 4 vector elements but found '" << line << "'" << endl;
   }
   return v;
}

Object * Parse::ParseCamera(std::istringstream & Stream)
{
   glm::vec3 location;
   glm::vec3 up;
   glm::vec3 right;
   glm::vec3 look_at;
   
   std::stringbuf buf;
   
   Stream.ignore(1, '{');
   Stream.get(buf, '}');
   Stream.ignore(numeric_limits<streamsize>::max(), '}');
   
   std::string content = buf.str();
   std::istringstream cam(content);
   std::string token;
   int n = 0;
   while(n < 4)
   {
      cam >> token;
      if(token == "location")
      {
         location = Parse::Vect3(cam);
      }
      else if(token == "up")
      {
         up = Parse::Vect3(cam);
      }
      else if(token == "right")
      {
         right = Parse::Vect3(cam);
      }
      else if(token == "look_at")
      {
         look_at = Parse::Vect3(cam);
      }
      n++;
   }
   Object *c =  new Camera(location, up, right, look_at);
   return c;
}

glm::mat4 Parse::ParseScale(std::istringstream & Stream, glm::mat4 model)
{
   glm::vec3 scale = Parse::Vect3(Stream);
   model = glm::scale(glm::mat4(1.f), scale) * model;
   return model;
}
glm::mat4 Parse::ParseTranslate(std::istringstream & Stream, glm::mat4 model)
{
   glm::vec3 translate = Parse::Vect3(Stream);
   model = glm::translate(glm::mat4(1.f), translate) * model;
   return model;
}
glm::mat4 Parse::ParseRotate(std::istringstream & Stream, glm::mat4 model)
{
   glm::vec3 rotate = Parse::Vect3(Stream);
   model = glm::rotate(glm::mat4(1.f), glm::radians(rotate.z), glm::vec3(0,0,1)) * model;
   model = glm::rotate(glm::mat4(1.f), glm::radians(rotate.y), glm::vec3(0,1,0)) * model;
   model = glm::rotate(glm::mat4(1.f), glm::radians(rotate.x), glm::vec3(1,0,0)) * model;
   return model;
}


Pigment * Parse::ParsePigment(std::istringstream & Stream)
{
   std::string color_type;
   Stream >> color_type;
   if(color_type == "rgb")
   {
      glm::vec3 color = Parse::Vect3(Stream);
      Pigment * p = new Pigment(color_type, color);
      return p;
   }
 
   else
   {
    glm::vec4 color = Parse::Vect4(Stream);
    Pigment * p = new Pigment(color_type, color);
    return p;
   }
}

Finish * Parse::ParseFinish(std::istringstream & Stream)
{
   float amb = 0.0;
   float dif = 0.0;
   float spec = 0.0;
   float rough = 0.0;
   float refl = 0.0;
   float refra = 0.0;
   float ior = 0.0;
   
   
   std::stringbuf buf;
   
   Stream.ignore(numeric_limits<streamsize>::max(), '{');
   Stream.get(buf, '}');
   Stream.ignore(numeric_limits<streamsize>::max(), '}');
   std::string content = buf.str();
   std::istringstream material(content);
   std::string token;
   while(material >> token)
   {
      if(token == "ambient")
      {
         material >> token;
         amb = std::stof(token, nullptr);
      }
      else if(token == "diffuse")
      {
         material >> token;
         dif = std::stof(token, nullptr);
      }
      
      else if(token == "specular")
      {
         material >> token;
         spec = std::stof(token, nullptr);
      }
      else if(token == "roughness")
      {
         material >> token;
         rough = std::stof(token, nullptr);
      }
      else if(token == "reflection")
      {
         material >> token;
         refl = std::stof(token, nullptr);
      }
      
      else if(token == "refraction")
      {
         material >> token;
         refra = std::stof(token, nullptr);
      }
      
      else if(token == "ior")
      {
         material >> token;
         ior = std::stof(token, nullptr);
      }
   }
   Finish *fin = new Finish(amb, dif, spec, rough, refl, refra, ior);
   return fin;
}


Object * Parse::ParseSphere(std::istringstream & Stream)
{
   glm::vec3 center;
   float radius;
   Pigment *p;
   Finish *f;
   std::string token;
   glm::mat4 ModelMatrix = glm::mat4(1.f);
   
   
   Stream.ignore(numeric_limits<streamsize>::max(), '{');
   
   center = Parse::Vect3(Stream);
   Stream.ignore(numeric_limits<streamsize>::max(), ',');

   Stream >> token;
   radius = std::stof(token, nullptr);

   std::string temp;
   while(Stream >> token)
   {
      if(token == "pigment")
      {
         std::stringbuf b;
         Stream.ignore(numeric_limits<streamsize>::max(), '{');
         Stream.get(b, '}');
         Stream.ignore(1, '}');
         std::string pgmt = b.str();
         std::istringstream pm(pgmt);
         std::string col;
         pm >> col;
         
         p = Parse::ParsePigment(pm);
         
      }
      
      else if (token == "finish")
      {
         f = Parse::ParseFinish(Stream);
      }
      else if(token == "translate")
      {
         ModelMatrix = Parse::ParseTranslate(Stream, ModelMatrix);
      }
      else if(token == "scale")
      {
         ModelMatrix = Parse::ParseScale(Stream, ModelMatrix);
      }
      else if(token == "rotate")
      {
         ModelMatrix = Parse::ParseRotate(Stream, ModelMatrix);
      }
      else if(token == "}")
      {
         break;
      }
   }
   
   Object *s = new Sphere(center, radius, p, f, ModelMatrix);
   return s;

}

Object * Parse::ParsePlane(std::istringstream & Stream)
{
   glm::vec3 normal;
   float distance = 0.0;
   Pigment *p;
   Finish *f;
   glm::mat4 ModelMatrix = glm::mat4(1.f);
   
   std::string token;
   Stream.ignore(numeric_limits<streamsize>::max(), '{');
   
   normal = Parse::Vect3(Stream);
   Stream.ignore(numeric_limits<streamsize>::max(), ',');

   Stream >> token;
   distance = std::stof(token, nullptr);
   
   
   std::string temp;
   while(Stream >> token)
   {
      if(token == "pigment")
      {
         std::stringbuf b;
         Stream.ignore(numeric_limits<streamsize>::max(), '{');
         Stream.get(b, '}');
         Stream.ignore(1, '}');
         std::string pgmt = b.str();
         std::istringstream pm(pgmt);
         std::string col;
         pm >> col;
         if(col == "color")
         {
            p = Parse::ParsePigment(pm);
         }
      }
      
      else if (token == "finish")
      {
         f = Parse::ParseFinish(Stream);
      }
      else if(token == "translate")
      {
         ModelMatrix = Parse::ParseTranslate(Stream, ModelMatrix);
      }
      else if(token == "scale")
      {
         ModelMatrix = Parse::ParseScale(Stream, ModelMatrix);
      }
      else if(token == "rotate")
      {
         ModelMatrix = Parse::ParseRotate(Stream, ModelMatrix);
      }
      else if(token == "}")
      {
         break;
      }
   }
   Object *pl = new Plane(normal, distance, p,f, ModelMatrix);
   return pl;
}


Object * Parse::ParseTriangle(std::istringstream & Stream)
{
   glm::vec3 point1;
   glm::vec3 point2;
   glm::vec3 point3;
   Pigment *p;
   Finish *f;
   glm::mat4 ModelMatrix = glm::mat4(1.f);
   
   std::string token;
   Stream.ignore(numeric_limits<streamsize>::max(), '{');
   
   point1 = Parse::Vect3(Stream);
   Stream.ignore(numeric_limits<streamsize>::max(), ',');
   
   point2 = Parse::Vect3(Stream);
   Stream.ignore(numeric_limits<streamsize>::max(), ',');
   
   point3 = Parse::Vect3(Stream);
   
   std::string temp;
   while(Stream >> token)
   {
      if(token == "pigment")
      {
         std::stringbuf b;
         Stream.ignore(numeric_limits<streamsize>::max(), '{');
         Stream.get(b, '}');
         Stream.ignore(1, '}');
         std::string pgmt = b.str();
         std::istringstream pm(pgmt);
         std::string col;
         pm >> col;
         if(col == "color")
         {
            p = Parse::ParsePigment(pm);
         }
      }
      
      else if (token == "finish")
      {
         f = Parse::ParseFinish(Stream);
      }
      else if(token == "translate")
      {
         ModelMatrix = Parse::ParseTranslate(Stream, ModelMatrix);
      }
      else if(token == "scale")
      {
         ModelMatrix = Parse::ParseScale(Stream, ModelMatrix);
      }
      else if(token == "rotate")
      {
         ModelMatrix = Parse::ParseRotate(Stream, ModelMatrix);
      }
      else if(token == "}")
      {
         break;
      }
   }

   Object *tr = new Triangle(point1, point2, point3, p,f, ModelMatrix);
   return tr;
}

Object * Parse::ParseBox(std::istringstream & Stream)
{
   glm::vec3 min;
   glm::vec3 max;
   Pigment *p;
   Finish *f;
   glm::mat4 ModelMatrix = glm::mat4(1.f);
   
   std::string token;
   Stream.ignore(numeric_limits<streamsize>::max(), '{');
   
   min = Parse::Vect3(Stream);
   Stream.ignore(numeric_limits<streamsize>::max(), ',');
   
   max = Parse::Vect3(Stream);
   
   std::string temp;
   while(Stream >> token)
   {
      if(token == "pigment")
      {
         std::stringbuf b;
         Stream.ignore(numeric_limits<streamsize>::max(), '{');
         Stream.get(b, '}');
         Stream.ignore(1, '}');
         std::string pgmt = b.str();
         std::istringstream pm(pgmt);
         std::string col;
         pm >> col;
         if(col == "color")
         {
            p = Parse::ParsePigment(pm);
         }
      }
      
      else if (token == "finish")
      {
         f = Parse::ParseFinish(Stream);
      }
      else if(token == "translate")
      {
         ModelMatrix = Parse::ParseTranslate(Stream, ModelMatrix);
      }
      else if(token == "scale")
      {
         ModelMatrix = Parse::ParseScale(Stream, ModelMatrix);
      }
      else if(token == "rotate")
      {
         ModelMatrix = Parse::ParseRotate(Stream, ModelMatrix);
      }
      else if(token == "}")
      {
         break;
      }
   }
   Object *bx = new Box(min, max, p,f, ModelMatrix);
   return bx;
}





Object * Parse::ParseLight(std::istringstream & Stream)
{
   glm::vec3 position;
   Pigment *p;
   
   std::stringbuf buf;
   Stream.ignore(1, '{');
   Stream.get(buf, '}');
   Stream.ignore(numeric_limits<streamsize>::max(), '}');
   std::string content = buf.str();
   std::istringstream light(content);
   position = Parse::Vect3(light);
   
   std::string token;
   light >> token;
   //if(token == "color")
   {
      p = Parse::ParsePigment(light);
   }
   
   Object *l = new LightSource(position, p);
   return l;
}


std::vector<Object *> Parse::ParseString(std::istringstream & FileContents) //maybe have arguments for camera & vector of lightsources?
{
   std::string token;
   
   std::string test; //
   vector<Object *> obj;
   while(FileContents >> token)
   {
      if(token.substr(0,2) == "//")
      {
         std::getline(FileContents, test);
      }
      
      else if(token == "camera")
      {
         obj.push_back(Parse::ParseCamera(FileContents));
      }
      
      else if(token == "light_source")
      {
         obj.push_back(Parse::ParseLight(FileContents));
      }
      
      else if(token == "sphere")
      {
         obj.push_back(Parse::ParseSphere(FileContents));
      }
      
      else if(token == "plane")
      {
         obj.push_back(Parse::ParsePlane(FileContents));
      }
      else if(token == "triangle")
      {
         obj.push_back(Parse::ParseTriangle(FileContents));
      }
      else if(token == "box")
      {
         obj.push_back(Parse::ParseBox(FileContents));
      }
   }
   return obj;
}





 







