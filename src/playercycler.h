#ifndef __PLAYERCYCLER_H_
#define __PLAYERCYCLER_H_

#include <vector>
#include "model.h"
#include "GLSLProgram.h"

class PlayerCycler : public SceneNode {
private:
  Model model;
  
public:
  Eigen::Vector2f forward_dir;

  GLSLProgram *toon_shader;
  GLSLProgram *blinn_phong_shader;
  GLSLProgram *gouraud_phong_shader;

  PlayerCycler( Scene *scene,
	        Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
		Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );

  virtual void step();
  virtual void render();
};


#endif // __PLAYER_CYCLER_H_

