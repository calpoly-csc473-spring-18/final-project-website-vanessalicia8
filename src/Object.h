#ifndef OBJECT
#define OBJECT
#include "Pigment.h"
#include "Finish.h"
#include "Ray.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Object{
public:
   glm::vec3 slot1;
   glm::vec3 slot2;
   glm::vec3 slot3;
   glm::vec3 slot4;
   glm::mat4 model;
   glm::mat4 inv_model;
   glm::mat4 normal_mat;
   Pigment *pig;
   Finish *fin;
   float save1;
   float save2;
   float save3;
   float save4;
   float save5;
   float save6;
   float save7;
   float save8;
   virtual glm::vec3 getNormal(const glm::vec3 intersection) = 0;
   virtual glm::vec3 getCenter() = 0;
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir) = 0;
   virtual std::string getType() = 0;
   virtual std::string toString() = 0;
};


class Camera:public Object{
public:
   glm::vec3 location;
   glm::vec3 up;
   glm::vec3 right;
   glm::vec3 look_at;
   
   Camera(glm::vec3 loc, glm::vec3 u, glm::vec3 r, glm::vec3 look);
   ~Camera();
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};


class LightSource:public Object{
public:
   glm::vec3 position;
   Pigment *pg;
   LightSource(glm::vec3 pos, Pigment *p);
   ~LightSource();
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};


class AABB:public Object{
public:
   glm::vec3 min;
   glm::vec3 max;
   AABB();
   void Reset(glm::vec3 pt);
   void AddPoint(glm::vec3 pt);
   void AddBox(AABB other);
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual std::vector<glm::vec3> compute_8_vertices();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};


class RayObject: public Object{
public:
   virtual glm::vec3 getNormal(const glm::vec3 intersection) = 0;
   virtual glm::vec3 getCenter() = 0;
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir) = 0;
   virtual std::string getType() = 0;
   virtual std::string toString() = 0;
};


class Sphere:public RayObject{
public:
   glm::vec3 center;
   float rad;
   Pigment *pg;
   Finish *fin;
   
   Sphere(glm::vec3 cent, float r, Pigment *p, Finish *f, glm::mat4 mod);
   ~Sphere();
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};


class Plane:public RayObject{
public:
   glm::vec3 normal;
   float dist;
   Pigment *pg;
   Finish *fin;
   
   Plane(glm::vec3 norm, float d, Pigment *p, Finish *f, glm::mat4 mod);
   ~Plane();
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};


class Triangle:public RayObject{
public:
   glm::vec3 point1;
   glm::vec3 point2;
   glm::vec3 point3;
   Pigment *pg;
   Finish *fin;
   
   Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, Pigment *p, Finish *f, glm::mat4 mod);
   ~Triangle();
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};

class Box:public RayObject{
public:
   glm::vec3 min;
   glm::vec3 max;
   Pigment *pg;
   Finish *fin;
   
   Box(glm::vec3 mini, glm::vec3 maxi, Pigment *p, Finish *f, glm::mat4 mod);
   ~Box();
   virtual glm::vec3 getNormal(const glm::vec3 intersection);
   virtual glm::vec3 getCenter();
   virtual float intersect(const glm::vec3 raypos, const glm::vec3 raydir);
   virtual std::string getType();
   virtual std::string toString();
};



#endif
