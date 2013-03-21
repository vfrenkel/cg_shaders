#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

#include "scene.h"

class Scene;

class Camera {
 private:
  Scene *scene;

  Eigen::Vector4f *focus;
  Eigen::Vector4f *up;
  Eigen::Vector2f *target_dir;
  Eigen::Vector2f *mouse_pos;
  Eigen::Vector2f *mouse_vel;
  float lag;

  void step_follow();
  void step_throw();
  void step_turntable();

 public:
  Eigen::Vector4f pos;

  bool is_follow;

  Camera();
  Camera(Scene *scene, float posx, float posy, float posz);
  ~Camera();

  void bind_focus(Eigen::Vector4f *target);
  void bind_up(Eigen::Vector4f *up);
  void bind_target_dir(Eigen::Vector2f *target_dir);
  void bind_mouse(Eigen::Vector2f *mouse_pos, Eigen::Vector2f *mouse_vel);

  void step();
  void transform_GL();
};


#endif //__CAMERA_H_


