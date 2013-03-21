#ifndef __GRIDPIECE_H_
#define __GRIDPIECE_H_

#include <vector>
#include <Eigen/Dense>

class GridPiece : public SceneNode {
public:
  GridPiece( Scene *scene,
	     Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
	     Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );

  virtual void step();
  virtual void render();

  static GLuint create_display_list();
};

#endif //__GRIDPIECE_H_
