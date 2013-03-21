#ifndef __SCENE_H_
#define __SCENE_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <string>

#include <Eigen/Dense>

#include "camera.h"

#define PI 3.14159265358979323846264338327950288419716939937510L
static const int MAX_DISPLAY_LISTS = 4;

typedef enum {
  OBJECT,
  LIGHT,
  JOINT,
  SKELETON
} SCENE_NODE_TYPE;

typedef enum {
  TEAPOT_DL,
  GRID_PIECE_DL,
  JOINT_DL
} DISPLAY_LIST;

class SceneNode;
class Camera;

/************************************************************
 * Scene Class                                              *
 * stores the current state of the scene, steps and renders *
 * the elements it stores.                                  *
 ************************************************************/
class Scene {
private:
  Camera *cam;
  float interp_factor;
  std::vector<GLuint> display_lists;

public:
  Scene();
  ~Scene();

  std::vector<SceneNode *> lights;
  std::vector<SceneNode *> objects;

  int num_nodes;

  bool key_states[256];
  bool sticky_key_states[256];
  bool mouse_button_states[2];
  Eigen::Vector2f mouse_pos;
  Eigen::Vector2f mouse_vel;

  //TODO: expand this to support a mult-node active selection list.
  SceneNode *selected;

  GLuint *fbo_ids;
  GLuint *rbo_ids;

  void init_display_lists();

  Camera *get_cam();
  float get_interp_factor();
  GLuint get_display_list(DISPLAY_LIST list);
  void add_node(SceneNode *n);

  void step_and_render();

  void set_cam_target(Eigen::Vector4f *target);
};

void vf_scene_000(Scene *s);
void vf_scene_001(Scene *s);
void vf_scene_002(Scene *s);

class SceneNode {
 protected:
  Scene *scene;
  
 public:
  int id; //TODO: use scene to assign a unique id.
  std::string name;
  SCENE_NODE_TYPE type;
  Eigen::Vector4f pos;
  Eigen::Vector4f rot;

  SceneNode();
  SceneNode( Scene *scene,
	     SCENE_NODE_TYPE type,
	     Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
	     Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );
  ~SceneNode();
  
  virtual void step() = 0;
  virtual void render() = 0;
};


class TeapotMesh : public SceneNode {
public:
  float size;

  TeapotMesh( Scene *scene,
	      float size,
	      Eigen::Vector4f pos = Eigen::Vector4f(0.0,0.0,0.0,1.0),
	      Eigen::Vector4f rot = Eigen::Vector4f(0.0,0.0,0.0,0.0) );

  virtual void step();
  virtual void render();
  
  static GLuint create_display_list();
};

// TODO: abandoned approach, clean up or complete.
class InfinitePlaneMesh : public SceneNode {
public:
  InfinitePlaneMesh();
  virtual void step();
  virtual void render();
};


#endif //__SCENE_H_
