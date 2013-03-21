#include <math.h>

#include "utils/macros.h"

#include "camera.h"
#include "linalg.h"
#include "scene.h"

Camera::Camera() {
  this->pos = Eigen::Vector4f(0.0, 6.0, -10.0, 1.0);

  this->focus = new Eigen::Vector4f(0.0,0.0,0.0,0.0);

  this->up = new Eigen::Vector4f(0.0,1.0,0.0,0.0);

  this->target_dir = new Eigen::Vector2f(0.0, 1.0);

  this->mouse_pos = NULL;
  this->mouse_vel = NULL;

  this->lag = 12.0;
  
  this->is_follow = false;

  this->scene = NULL;
}

Camera::Camera(Scene *scene, float posx, float posy, float posz) {
  this->pos = Eigen::Vector4f(posx, posy, posz, 1.0);

  this->focus = new Eigen::Vector4f(0.0,0.0,0.0,0.0);

  this->up = new Eigen::Vector4f(0.0,1.0,0.0,0.0);

  this->target_dir = new Eigen::Vector2f(0.0, 1.0);
  
  this->mouse_pos = NULL;
  this->mouse_vel = NULL;
  
  this->lag = 12.0;
  
  this->is_follow = false;

  this->scene = scene;
}

// TODO: add actual clean up.
Camera::~Camera() {

}

void Camera::step_follow() {
  this->pos[0] = (*this->focus)[0] - this->lag * (*this->target_dir)[0];
  this->pos[1] = (*this->focus)[1] + 5.0;
  this->pos[2] = (*this->focus)[2] - this->lag * (*this->target_dir)[1];
}

void Camera::step_throw() {
  float dist_to_target = distance3(this->pos, *(this->focus));
  const float threshold = 130.0f;
  
  // throw the camera ahead of bike.
  if (dist_to_target > threshold) {
    this->pos[0] = (*this->focus)[0] + threshold * (*this->target_dir)[0];
    this->pos[2] = (*this->focus)[2] + threshold * (*this->target_dir)[1];
  }
}

void Camera::step_turntable() {
  static float angle = 0.0f;
  static float distance = 10.0f;

  if(this->scene->key_states['a']) {
    angle -= 0.05f;
  } else if (this->scene->key_states['d']) {
    angle += 0.05f;
  }
  
  if (this->scene->key_states['w']) {
    distance -= 0.1f;
  } else if (this->scene->key_states['s']) {
    distance += 0.1f;
  }

  this->pos[0] = distance*sin(angle);
  this->pos[1] = distance/2;
  this->pos[2] = distance*cos(angle);
}

void Camera::step() {
  if (this->is_follow) {
    step_follow();
  } else {
    step_throw();
  }
}

void Camera::transform_GL() {
  //TODO: figure out what is wrong with this->up...
  gluLookAt(this->pos[0], this->pos[1], this->pos[2],
  	    (*this->focus)[0], (*this->focus)[1], (*this->focus)[2],
  	    0.0, 1.0, 0.0);
}
void Camera::bind_focus(Eigen::Vector4f *target) {
  this->focus = target;
}

void Camera::bind_up(Eigen::Vector4f *up) {
  this->up = up;
}

void Camera::bind_target_dir(Eigen::Vector2f *target_dir) {
  this->target_dir = target_dir;
}

void Camera::bind_mouse(Eigen::Vector2f *mouse_pos, Eigen::Vector2f *mouse_vel) {
  this->mouse_pos = mouse_pos;
  this->mouse_vel = mouse_vel;
}
