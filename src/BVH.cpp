#include "BVH.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include<iostream>

bool sort_on_x(Object * i, Object * j)
{
   return i->getCenter().x < j->getCenter().x;
}

bool sort_on_y(Object * i, Object * j)
{
   return i->getCenter().y < j->getCenter().y;
}

bool sort_on_z(Object * i, Object * j)
{
   return i->getCenter().z < j->getCenter().z;
}


bvh_node::bvh_node() {
   //this->currentbox = NULL;
   this->left = NULL;
   this->right = NULL;
   //this->objects = NULL;
}
void bvh_node::recursive_tree_build(std::vector<Object *> objs, int axis)/* 0=x, 1=y, 2=z */
{
   if (objs.size() <= 1) {
      this->objects = objs;
      this->calculateBBox();
      //std::cout<< this->currentbox.min.x<<" " << this->currentbox.min.y<<" "<< this->currentbox.min.z<<std::endl;
      //std::cout<< this->currentbox.max.x<<" " << this->currentbox.max.y<<" "<< this->currentbox.max.z<<"\n"<<std::endl;
      return;
   }
   
   sort_objects_on_axis(objs, axis);
   
   int mid = objs.size()/2;
   
   this->left = new bvh_node();
   this->right = new bvh_node();
   
   this->left->recursive_tree_build(std::vector<Object *>(objs.begin(),objs.begin()+mid), (axis + 1) % 3);
   this->right->recursive_tree_build(std::vector<Object *>(objs.begin()+mid,objs.end()), (axis + 1) % 3);
   
   this->calculateBBox();
   //std::cout<< this->currentbox.min.x<<" " << this->currentbox.min.y<<" "<< this->currentbox.min.z<<std::endl;
   //std::cout<< this->currentbox.max.x<<" " << this->currentbox.max.y<<" "<< this->currentbox.max.z<<"\n"<<std::endl;
}
void bvh_node::calculateBBox()
{
   AABB box;
   if(this->left == NULL && this->right == NULL)
   {
      for (Object * ob : this->objects)
      {
         if(ob->getType()=="Sphere")
         {
            box.Reset(ob->slot1 - glm::vec3(ob->save8));
            box.AddPoint(ob->slot1 + glm::vec3(ob->save8));
         }
         else if(ob->getType()=="Triangle")
         {
            box.Reset(ob->slot1);
            box.AddPoint(ob->slot3);
            box.AddPoint(ob->slot4);
         }
         else if(ob->getType()=="Box")
         {
            box.Reset(ob->slot1);
            box.AddPoint(ob->slot3);
         }
         
         std::vector<glm::vec3> verts = box.compute_8_vertices();
         
         for (glm::vec3 & v : verts) {
            glm::vec4 t_vert = ob->model * glm::vec4(v,1.f);
            v = glm::vec3(t_vert.x, t_vert.y, t_vert.z);
         }
         
         box.Reset(verts[0]);
         for (int i = 1; i < 8; ++ i)
            box.AddPoint(verts[i]);
      }
   }
   else
   {
      if(this->left != NULL)
         box.AddBox(this->left->currentbox);
      if(this->right != NULL)
         box.AddBox(this->right->currentbox);
   }
   this->currentbox = box;
}

float bvh_node::intersect(glm::vec3 pos, glm::vec3 dir)
{
   return currentbox.intersect(pos, dir);
}


void bvh_node::sort_objects_on_axis(std::vector<Object *> &objs, int axis)
{
   if(axis==0)
   {
      sort(objs.begin(), objs.end(), sort_on_x);
   }
   else if(axis==1)
   {
      sort(objs.begin(), objs.end(), sort_on_y);
   }
   else if(axis==2)
   {
      sort(objs.begin(), objs.end(), sort_on_z);
   }
}
