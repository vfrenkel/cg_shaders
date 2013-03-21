#ifndef __POINTLIGHT_H_
#define __POINTLIGHT_H_

#include <Eigen/Dense>

#include "scene.h"

class PointLight : public SceneNode {
public:
  float intensity;
  
  PointLight( Scene *scene,
	      float intensity,
	      Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
	      Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );

  virtual void step();
  virtual void render();

  static GLuint create_display_list();
};



#endif //__POINTLIGHT_H_
