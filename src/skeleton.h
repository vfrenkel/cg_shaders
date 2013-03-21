#ifndef __SKELETON_H_
#define __SKELETON_H_

#include <Eigen/Dense>

#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include "model.h"
#include "revolutejoint.h"

typedef struct ik_info_struct {
  RevoluteJoint *start; // first joint in the IK chain.
  RevoluteJoint *end; //joint the end effector is attached to.
  Eigen::Vector4f *end_effector; // the target.
} ik_info;

class Skeleton : public SceneNode {
public:
  RevoluteJoint *root;

  bool ik_mode;
  std::vector<ik_info> ik_controls;
  

  // TODO: perhaps you should take out position and rotation from scene nodes...
  // makes things like this joint awkward, since it doesn't necessarily want to use
  // these attributes.
  Skeleton( Scene *scene,
	    RevoluteJoint *root_joint,
	    Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
	    Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );
  
  virtual void step();
  virtual void render();
};


#endif // __SKELETON_H_

