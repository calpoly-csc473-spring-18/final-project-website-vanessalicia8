#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <memory>
#include <iomanip>
#include <math.h>
#include <cmath>
#include "Parse.h"
#include "Object.h"
#include "BVH.h"
#include "Ray.h"
#include "Pigment.h"
#include "Finish.h"
#include "Image.h"
#include <unistd.h>
#include <algorithm>

glm::vec3 alignSampleVector(glm::vec3 sample, glm::vec3 normal)
{
   const glm::vec3 up = glm::vec3(0,0,1);
   if(up == normal)
      return sample;
   if(up == -normal)
      return -sample;
   float angle = acos(glm::dot(up, normal));
   glm::vec3 axis = glm::cross(up, normal);
   glm::mat4 matrix = glm::mat4(1.f);
   
   matrix = glm::rotate(glm::mat4(1.f), angle, axis) * matrix;
   return glm::vec3(matrix * glm::vec4(sample,1.f));
}


glm::vec3 generateCosineWeightedPoint(const float u, const float v, glm::vec3 normal)
{
   float radial = glm::sqrt(u);
   float theta = 2.0 * M_PI * v;
   float x = radial * cos(theta);
   float y = radial * sin(theta);
   
   glm:: vec3 point = glm::vec3(x,y,glm::sqrt(1-u));
   return alignSampleVector(point, normal);
}


glm::vec3 CalculateRefractionVector(const glm::vec3 & view, glm::vec3 normal, const float ior)
{
   float iorRatio;
   
   if (glm::dot(view, normal) >= 0.f)
      iorRatio = 1.f / ior;
   else
   {
      iorRatio = ior / 1.f;
      normal = -normal;
   }
   const float c1 = glm::dot(view, normal);
   const float c2 = 1 - (iorRatio * iorRatio) * (1 - (c1 * c1));
   
   if (c2 < 0.f)
      return glm::vec3(0.f, 0.f, 0.f);
   return glm::normalize((-view * iorRatio) + normal * (iorRatio * c1 - glm::sqrt(c2)));
}
glm::vec2 closest_shape(const glm::vec3 position,const glm::vec3 direction, std::vector <Object *> shapes)
{
   glm::vec2 closest;
   std::vector<float> t_values;
   float t = 5000;
   float index = -1;
   for(Object *ob : shapes)
   {
      glm::vec4 t_pos = ob->inv_model*glm::vec4(position,1.f); //for transforms
      glm::vec4 t_dir = ob->inv_model*glm::vec4(direction,0.f);
      
      float T = ob->intersect(glm::vec3(t_pos.x,t_pos.y,t_pos.z),glm::vec3(t_dir.x,t_dir.y,t_dir.z));//transformed_ray
      t_values.push_back(T);
   }
   for(int k = 0; k < t_values.size(); k++)
   {
      float temp = t_values[k];
      if(temp > 0 && temp < t)
      {
         t = temp;
         index = k;
      }
   }
   closest = glm::vec2(t, index);
   return closest;
}
void check_intersect(const glm::vec3 position,const glm::vec3 direction, bvh_node * node, float * T, Object * &closest)
{
   if(node->left == NULL && node->right == NULL)
   {
      float t = node->intersect(position, direction); //intersects box?
      if(t > 0)
      {
         //if(node->objects[0]->getType() != "Box")
         glm::vec4 t_pos = node->objects[0]->inv_model*glm::vec4(position,1.f); //for transforms
         glm::vec4 t_dir = node->objects[0]->inv_model*glm::vec4(direction,0.f);
         float s = node->objects[0]->intersect(glm::vec3(t_pos.x,t_pos.y,t_pos.z),glm::vec3(t_dir.x,t_dir.y,t_dir.z)); //intersects object?
         if(s > 0 && s < *T)
         {
            *T = s;
            closest = node->objects[0];
         }
      }
      return;
   }
   
   float t = node->intersect(position, direction);
   if(t >= 0 && t < *T)
   {
      if(node->left != NULL)
      {
         check_intersect(position, direction, node->left, T, closest);
      }
      if(node->right != NULL)
      {
         check_intersect(position, direction, node->right, T, closest);
      }
   }
}

Object * closest_obj(const glm::vec3 position,const glm::vec3 direction, std::vector <Object *> planes, bvh_node * root, float * T)
{
   Object * closest = nullptr;
   check_intersect(position,direction,root,T, closest);
   glm::vec2 cl = closest_shape(position,direction,planes);
   
   if(*T != 5000)//possible
   {
      if(cl.y != -1)//possible plane
      {
         if(*T < cl.x)
            return closest;
         *T = cl.x;
         return planes[(int)cl.y];
      }
      return closest;
   }
   if(cl.y != -1)//possible plane
   {
      *T = cl.x;
      return planes[(int)cl.y];
   }
   return nullptr;
}

glm::vec3 trace(const glm::vec3 position,const glm::vec3 direction, std::vector <Object *> light_sources, std::vector <Object *> shapes, std::vector <Object *> planes, bvh_node * root, int depth, int bounce,float * p, glm::vec4 flags)
{
   float t;
   int index;
   glm::vec3 color = glm::vec3(0,0,0);
   glm::vec3 pos = position;
   glm::vec3 dir = glm::normalize(direction);
   glm::vec3 view = -glm::normalize(direction);
   glm::vec3 normal = glm::vec3(0,0,0);
   Object * closest = nullptr;
   
   if(flags.z > 0)
   {
      t = 5000;
      closest = closest_obj(position, direction, planes, root, &t);
   }
   else
   {
      glm::vec2 close = closest_shape(position, direction, shapes);
      t = close.x;
      index = close.y;
      if(t != 5000)
         closest = shapes[index];
   }

   *p = t;
   if(closest != nullptr)
   {
      glm::vec3 intersection = pos + (t*dir);
      float reflect = closest->save5;
      glm::vec3 obj_color = closest->slot2;
      float filter = 0;
      if(closest->pig->getType() == "rgbf")
         filter = closest->pig->color.w;
      
      if(closest->getType() == "Sphere")
      {
         glm::vec4 obj_inter = (closest->inv_model*glm::vec4(intersection,1.f)) - glm::vec4(closest->slot1,1.f);
         normal = glm::normalize(glm::vec3(obj_inter.x, obj_inter.y, obj_inter.z)); //intersection point - center
      }
      else if(closest->getType() == "Plane")
         normal = glm::normalize(closest->slot1);
      else if(closest->getType() == "Triangle")
         normal = glm::normalize(glm::cross(closest->slot3-closest->slot1,closest->slot4-closest->slot1)); //b-a x c-a
      else if(closest->getType() == "Box")
      {
         glm::vec4 obj_inter = (closest->inv_model*glm::vec4(intersection,1.f));
         normal = glm::normalize(closest->getNormal(glm::vec3(obj_inter.x, obj_inter.y, obj_inter.z)));
      }
      
      glm::mat4 normal_matrix= closest->normal_mat;
      glm::vec4 nor = normal_matrix*glm::vec4(normal,0.f);
      normal = glm::normalize(glm::vec3(nor.x, nor.y, nor.z));
      
      for(Object *l : light_sources)
      {
         bool in_shadow = false;
         const glm::vec3 light_vector = glm::normalize(l->slot1 - intersection);//lpos-intersection pt
         const glm::vec3 origin = intersection + (light_vector *0.001f);
      
         for(Object *obj : shapes)
         {
            glm::vec4 t_lv = obj->inv_model*glm::vec4(light_vector,0.f);//lpos-intersection pt
            glm::vec4 t_o = obj->inv_model*glm::vec4(origin,1.f);
            float S = obj->intersect(glm::vec3(t_o.x,t_o.y,t_o.z),glm::vec3(t_lv.x,t_lv.y,t_lv.z));
            if(S >= 0.f)
            {
               if(S < glm::length(l->slot1 - intersection))
                  in_shadow = true;
            }
         }
   
         glm::vec3 half = glm::normalize(light_vector + view);
         
         if(in_shadow == false)
         {
            //Blinn-Phong
            float roughness = closest->save4;
            float alpha = (2.0/(roughness*roughness))-2.0;
            float rdif = closest->save2 * (glm::clamp(glm::dot(normal,light_vector), 0.0f,1.0f));
            float rspec = closest->save3 * (glm::pow(glm::clamp(glm::dot(half, normal), 0.0f,1.0f), alpha));
            color+= obj_color * l->slot2 * (rdif + rspec) * (1-reflect) * (1-filter); //local value
         }
      }
      
      if(depth > 0)
      {
         float fresnel = 0;
         float ior = closest->save7;
         if(flags.x > 0)
         {
            float f0 = ((ior-1)*(ior-1))/((ior+1)*(ior+1));
            float dot = std::abs(glm::dot(normal,view));
            fresnel = f0+(1-f0)*glm::pow((1-dot),5);
         }
         float reflectionContribution = (1.f - filter) * (reflect) + filter * fresnel;
         if(filter > 0) //refractive
         {
            glm::vec3 refraction = CalculateRefractionVector(view,normal,ior);
            const bool entering = glm::dot(normal, view) >= 0.f;
            if(refraction != glm::vec3(0.f))
            {
               float next_t;
               glm::vec3 refraction_color = trace(intersection + refraction* 0.001f, refraction, light_sources, shapes, planes, root, depth-1, bounce, &next_t, flags);
               if(entering)
               {
                  if(flags.y > 0) //beers
                  {
                     glm::vec3 refraction_color = trace(intersection + refraction* 0.001f, refraction, light_sources, shapes, planes, root, depth-1, bounce, &next_t, flags);
                     if(next_t != 5000)
                     {
                        float med_dist = next_t;
                        glm::vec3 absorbance = glm::vec3((1-obj_color.x)*0.15*-med_dist,(1-obj_color.y)*0.15*-med_dist,(1-obj_color.z)*0.15*-med_dist);
                        glm::vec3 attenuation = glm::vec3(exp(absorbance.x),exp(absorbance.y),exp(absorbance.z));
                        color+= filter *(1-fresnel) * attenuation * refraction_color;
                     }
                  }
                  else //no beers
                     color+= filter *(1-fresnel)* obj_color * refraction_color;
               }
               else //exiting
                  color+= filter*(1-fresnel)* refraction_color;
            }
            else
            {
               reflectionContribution+= filter*(1-fresnel);
            }
         }
         if(reflect > 0 || filter > 0) //reflective
         {
            const glm::vec3 reflection = glm::normalize(normal * glm::dot(view, normal) * 2.f - view);
            const glm::vec3 reflectionColor = obj_color * trace(intersection + reflection* 0.001f, reflection, light_sources, shapes, planes, root, depth-1, bounce, p, flags);
            color+= reflectionContribution * reflectionColor;
         }
      }
      
      
      if(flags.a > 0)
      {
         if(bounce > 0)
         {
            glm::vec3 ambient = glm::vec3(0);
            int num_pts = 16;
            if(bounce == 2)
               num_pts = 64;
            int dim = glm::sqrt(num_pts);
            for(int k = 0; k < dim; k++)
            {
               for(int l = 0; l < dim; l++)
               {
                  const float u = (rand() / (float)RAND_MAX)/(float)dim + float(k)/(float)(dim);
                  const float v = (rand() / (float)RAND_MAX)/(float)dim + float(l)/(float)(dim);
                  glm::vec3 point = generateCosineWeightedPoint(u,v, normal);
                  ambient+= trace(intersection+ normal*0.001f, point, light_sources, shapes, planes, root, depth, bounce-1, p, flags);
               }
            }
            ambient *= 1.f / num_pts;
            color += ambient;
         }
      }
      else
         color += obj_color * closest->save1 * (1-reflect) * (1-filter);
      
   }

   color.x = glm::clamp(color.x, 0.0f,1.0f);
   color.y = glm::clamp(color.y, 0.0f,1.0f);
   color.z = glm::clamp(color.z, 0.0f,1.0f);
   return color;
}



int main(int argc, char **argv)
{
	if(argc < 3) {
      std::cout << "Usage: raytrace sceneinfo <input_filename>" << std::endl;
      std::cout << "Usage: raytrace raycast <input_filename> <width> <height>" << std::endl;
      std::cout << "Usage: raytrace pixelray <input_filename> <width> <height> <x> <y>" << std::endl;
      std::cout << "Usage: raytrace firsthit <input_filename> <width> <height> <x> <y>" << std::endl;
      std::cout << "Usage: raytrace render <input_filename> <width> <height> [-fresnel]" << std::endl;
      std::cout << "Usage: raytrace pixelcolor <input_filename> <width> <height> <x> <y>" << std::endl;
		return 0;
	}
   std::string filename = "output.png";
   std::string command(argv[1]);
   std::string input_filename(argv[2]); //input filename
   
   float width = 0;
   float height = 0;
   if(argc > 3)
   {
      width = atoi(argv[3]);
      height = atoi(argv[4]);
   }
   float x = 0;
   float y = 0;
   int s = 1;
   int frnl = 0;
   int brs = 0;
   int sds = 0;
   int gi = 0;
   int GI = 0;
   int dof = 0;
   
   if(command == "render")
   {
      for(int i = 5; argc > i; i++)
      {
         std::string flag(argv[i]);
         if(flag == "-fresnel")
            frnl = 1;
         else if(flag == "-beers")
            brs = 1;
         else if(flag == "-sds")
            sds = 1;
         else if(flag == "-gi")
         {
            gi = 1;
            GI = 2;
         }
         else if(flag == "-dof")
            dof = 1;
         else
         {
            const char * sample = &flag[4];
            s = atoi(sample);
         }
      }
   }
   
   else if(argc > 5)
   {
      x = atoi(argv[5]);
      y = atoi(argv[6]);
   }
   glm::vec4 flags = glm::vec4(frnl,brs,sds,gi);
   
   auto image = std::make_shared<Image>(width, height);
   
   
   std::ifstream FileHandle(input_filename);
   std::string String;
   
   FileHandle.seekg(0, std::ios::end);
   String.reserve((uint) FileHandle.tellg());
   FileHandle.seekg(0, std::ios::beg);
   String.assign((std::istreambuf_iterator<char>(FileHandle)), std::istreambuf_iterator<char>());
   std::istringstream iss(String);
   
   Parse p;
   std::vector <Object *> scene = p.ParseString(iss); //should contain all objects in scene
   std::vector <Object *> light_sources;
   std::vector <Object *> shapes;
   std::vector <Object *> planes;
   std::vector <Object *> objs;
   bvh_node * root = new bvh_node();
   Object * Cam;
   
   int num_lights = 0;
   int num_shapes = 0;
   
   for(int i = 0; i < scene.size(); i++)
   {
      if(scene[i]->getType() == "camera")
      {
         Cam = scene[i];
      }
      else if(scene[i]->getType() == "light_source")
      {
         light_sources.push_back(scene[i]);
         num_lights++;
      }
      else
      {
         shapes.push_back(scene[i]);
         if(scene[i]->getType() == "Plane")
            planes.push_back(scene[i]);
         else
            objs.push_back(scene[i]);
         num_shapes++;
      }
   }
   
   float Us = -0.5 + ((x+0.5)/width);
   float Vs = -0.5 + ((y+0.5)/height);
   float Ws = -1.0;
   glm::vec3 u = Cam->slot3;
   glm::vec3 v = glm::normalize(Cam->slot2);
   glm::vec3 w = glm::normalize(Cam->slot1 - Cam->slot4);
   glm::vec3 d = glm::normalize((Us*u) + (Vs*v) + (Ws*w));
   
   
   if(command == "sceneinfo")
   {
      std::cout <<"Camera: "<<std::endl;
      std::cout <<Cam->toString() <<"\n\n---\n\n"<<std::endl;
      
      
      std::cout <<num_lights<<" light(s)\n\n" <<std::endl;
      for(int i = 0; i < light_sources.size(); i++)
      {
         std::cout <<"Light[" << i << "]:\n" <<std::endl;
         std::cout <<light_sources[i]->toString() <<std::endl;
      }
      
      std::cout <<"\n\n---\n\n"<<std::endl;
      std::cout <<num_shapes<<" object(s)\n\n" <<std::endl;
      
      for(int i = 0; i < shapes.size(); i++)
      {
         std::cout <<"Object[" << i << "]:\n" <<std::endl;
         std::cout <<shapes[i]->toString() <<"\n"<<std::endl;
      }
      return 0;
   }
   else if(command == "pixelray")
   {
      std::ostringstream output;
      output <<std::setprecision(4)<< "Pixel: [" << x << ", " << y << "] Ray: {" << Cam->slot1.x << " " << Cam->slot1.y << " " << Cam->slot1.z << "} -> {" << d.x << " " << d.y << " " << d.z << "}";
      std::cout <<output.str()<<std::endl;

   }
   else if(command == "firsthit"|| command == "pixelcolor")
   {
      std::vector<float> t_values;
      float t = 5000;
      int index = -1;
      for(Object *ob: shapes)
      {
         float t = ob->intersect(Cam->slot1, d);
         t_values.push_back(t);
      }
      for(int k = 0;k<t_values.size();k++)
      {
         float temp = t_values[k];
         if(temp > 0 && temp < t)
         {
            
            t = temp;
            index = k;
         }
      }
      
      float T = t;
      Object * closest = shapes[index];
      std::ostringstream output;
      output <<std::setprecision(4)<< "Pixel: [" << x << ", " << y << "] Ray: {" << Cam->slot1.x << " " << Cam->slot1.y << " " << Cam->slot1.z << "} -> {" << d.x << " " << d.y << " " << d.z << "}\n";
      if(t != 5000)
      {
         output << "T = " << T <<"\n";
         output << "Object Type: " << closest->getType() <<"\n";
         if(command == "firsthit")
            output << "Color: "  << closest->slot2.x << " " << closest->slot2.y << " " << closest->slot2.z;
         else //pixelcolor
         {
            output << "BRDF: Blinn-Phong\n";
            float p;
            glm::vec3 color = trace(Cam->slot1, d, light_sources, shapes, planes, root, 6, GI, &p, flags);
            output << "Color: ("  << round(color.x*255) << ", " << round(color.y*255) << ", " << round(color.z*255) << ")";
         }
      }
      else
      {
         output <<"No Hit";
      }
      std::cout <<output.str()<<std::endl;
   }
   
   else if(command == "raycast")
   {
      for (int j = 0; j < height; j++) {
         for (int i = 0; i < width; i++) {
            float U = -0.5 + (((float)i+0.5)/width);
            float V = -0.5 + (((float)j+0.5)/height);
            glm::vec3 direction = glm::normalize((U*u) + (V*v) + (Ws*w));
            std::vector<float> t_values;
            float t = 5000;
            int index = -1;
            for(Object *ob : shapes)
            {
               float T = ob->intersect(Cam->slot1, direction);
               t_values.push_back(T);
            }
            for(int k = 0; k < t_values.size(); k++)
            {
               float temp = t_values[k];
               if(temp > 0 && temp < t)
               {
                  t = temp;
                  index = k;
               }
            }
            
            if(t != 5000)
            {
               Object * closest = shapes[index];
               image->setPixel(i,j,(closest->slot2.x)*255.0,(closest->slot2.y)*255.0,(closest->slot2.z)*255.0);
            }
            else
               image->setPixel(i,j,0,0,0);
         }
      }
      image->writeToFile(filename);
   }
   
   else if(command == "render")
   {
      const float focalLength = 15;
      const int num_samples = 16;
      const float r = 1.f;
      if(sds == 1)
         root->recursive_tree_build(objs, 0);
      
         for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
               glm::vec3 color = glm::vec3(0,0,0);
               for(int m = 0; m<s; m++){
                  for(int n = 0; n<s; n++){
                     float U = -0.5 + ((i*s+m+0.5)/(width*s));
                     float V = -0.5 + ((j*s+n+0.5)/(height*s));
                     glm::vec3 direction = (U*u) + (V*v) + (Ws*w);
                     glm::vec3 aimpoint = Cam->slot1 + focalLength * direction; //location + fL * direction
                     direction = glm::normalize(direction);
                     float p;
                     if(dof == 0)
                        color += trace(Cam->slot1, direction, light_sources, shapes, planes, root, 6, GI, &p, flags);
                     else
                     {
                        for(int di = 0; di < num_samples; di++)
                        {
                           float du = rand() / float(RAND_MAX+1);
                           float dv = rand() / float(RAND_MAX+1);
                           glm::vec3 new_origin = Cam->slot1 - (r/2) * U - (r/2) * V + r * du * U + r * dv * V;
                           glm::vec3 new_dir = glm::normalize(aimpoint-new_origin);
                           color += trace(new_origin+(new_dir*0.001f), new_dir, light_sources, shapes, planes, root, 6, GI, &p, flags);
                        }
                        color /= num_samples;
                     }
                  }
               }
               color = color /((float)(s*s));
               image->setPixel(i,j,color.x*255.0,color.y*255.0,color.z*255.0);
            }
         }
      image->writeToFile(filename);
   }
   return 0;
}
