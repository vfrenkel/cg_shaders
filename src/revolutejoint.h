#ifndef __REVOLUTEJOINT_H_
#define __REVOLUTEJOINT_H_

#include <Eigen/Dense>

#include <vector>
#include <glm/glm.hpp>
#include "model.h"

class RevoluteJoint : public SceneNode {
private:
  // display list representation.
  int dlist_rep;

public:
  RevoluteJoint *root;
  RevoluteJoint *parent;
  // TODO: revamp engine so it uses a list of children instead of these crappy variables.
  RevoluteJoint *child;
  RevoluteJoint *child2;
  Eigen::Vector4f rot_axis;
  float length;
  float angle;
  Eigen::Matrix4f T;

  bool start_joint;
  bool end_effector;

  // TODO: perhaps you should take out position and rotation from scene nodes...
  // makes things like this joint awkward, since it doesn't necessarily want to use
  // these attributes.
  RevoluteJoint( Scene *scene,
		 RevoluteJoint *parent,
		 Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
		 Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );

  static GLuint create_display_list();

  virtual void step();
  virtual void render();

  void calculate_transform();
};


#endif // __REVOLUTEJOINT_H_

