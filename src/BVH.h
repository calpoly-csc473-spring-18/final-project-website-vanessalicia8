#ifndef BVH
#define BVH
#include "Object.h"
#include <glm/glm.hpp>
#include <string>
#include <algorithm>
#include <vector>


class bvh_node {
public:
   AABB currentbox;
   bvh_node * left;
   bvh_node * right;
   std::vector<Object *> objects;
   
   bvh_node();
   void recursive_tree_build(std::vector<Object *> objs, int axis /* 0=x, 1=y, 2=z */);
   float intersect(glm::vec3 pos, glm::vec3 dir);
   void sort_objects_on_axis(std::vector<Object *> &objs, int axis);
   void calculateBBox();
   
/*private:
   bool sort_on_x(Object * i, Object * j);
   bool sort_on_y(Object * i, Object * j);
   bool sort_on_z(Object * i, Object * j);*/
};
#endif

