
#include <GL/glew.h>
#include <GL/glut.h>

#include <Eigen/Dense>

#include <stack>
#include <math.h>

#include "scene.h"
#include "revolutejoint.h"
#include "linalg.h"
#include "model.h"

RevoluteJoint::RevoluteJoint(Scene *scene, RevoluteJoint *parent, Eigen::Vector4f pos, Eigen::Vector4f rot)
  : SceneNode(scene, JOINT, pos, rot)
{
  this->parent = parent;
  if (this->parent) {
    if (this->parent->child) {
      this->parent->child2 = this;
    } else {
      this->parent->child = this;
    }
  }

  this->root = this;
  while (this->root->parent) {
    this->root = this->root->parent;
  }

  this->child = NULL;
  this->child2 = NULL;
  this->T = Eigen::Matrix4f::Identity();
  this->rot_axis = Eigen::Vector4f(0.0, 0.0, 1.0, 0.0);
  if (this->parent) {
    this->parent->length = (this->pos - this->parent->pos).norm();
  }
  this->angle = 0.0f;
}


// creates a unit length, +x facing joint representation which can be scaled then oriented.
GLuint RevoluteJoint::create_display_list() {

  GLuint list = glGenLists(1);
  glNewList(list, GL_COMPILE);

  glPushMatrix();

  //glutSolidSphere(1.0f, 20, 20);
  //glScalef(5.0f, 1.0f, 1.0f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  glutSolidCone(0.5f, 1.0f, 10, 10);

  glPopMatrix();

  glEndList();

  return list;
}

void RevoluteJoint::calculate_transform() {

  // recalculate rotation matrix for this joint's frame of reference.
  if (this->rot_axis[2] == 1.0) {
    this->T(0,0) = cos(this->angle * PI / 180.0f);
    this->T(0,1) = -sin(this->angle * PI / 180.0f);
    this->T(1,0) = sin(this->angle * PI / 180.0f);
    this->T(1,1) = cos(this->angle * PI / 180.0f);
  } 
  if (this->rot_axis[1] == 1.0) {
    this->T(0,0) = cos(this->angle * PI / 180.0f);
    this->T(0,2) = -sin(this->angle * PI / 180.0f);
    this->T(2,0) = sin(this->angle * PI / 180.0f);
    this->T(2,2) = cos(this->angle * PI / 180.0f);
  }

  if (this->parent) {
    this->T(0,3) = pos[0] - this->parent->pos[0];
    this->T(1,3) = pos[1] - this->parent->pos[1];
  }
}

void RevoluteJoint::step() {
  //TODO: set up mouse picking and dragging, use that to manipulate joints instead of keys.

  if (this->name == "joint_1") {
    if (this->scene->key_states['u']) {
      this->angle += 1.0f;
    } else if (this->scene->key_states['j']) {
      this->angle -= 1.0f;
    }
  }

  if (this->name == "joint_2") {
    if (this->scene->key_states['i']) {
      this->angle += 1.0f;
    } else if (this->scene->key_states['k']) {
      this->angle -= 1.0f;
    }
  }

  if (this->name == "joint_3") {
    if (this->scene->key_states['o']) {
      this->angle += 1.0f;
    } else if (this->scene->key_states['l']) {
      this->angle -= 1.0f;
    }
  }
  
  if (this->name == "joint_6") {
    if (this->scene->key_states['r']) {
      this->angle += 1.0f;
    } else if (this->scene->key_states['f']) {
      this->angle -= 1.0f;
    }
  }

  if (this->name == "joint_7") {
    if (this->scene->key_states['t']) {
      this->angle += 1.0f;
    } else if (this->scene->key_states['g']) {
      this->angle -= 1.0f;
    }
  }

  if (this->name == "joint_8") {
    if (this->scene->key_states['y']) {
      this->angle += 1.0f;
    } else if (this->scene->key_states['h']) {
      this->angle -= 1.0f;
    }
  }
  

  this->calculate_transform();
  //std::cout << "T[0][0]: " << this->T[0][0] << std::endl;
}

void RevoluteJoint::render() {

  const GLfloat white_emissive_material[] = {0.50, 0.50, 0.50};
  const GLfloat green_emissive_material[] = {0.0, 0.50, 0.0};

  Eigen::Matrix4f T_final_mat = this->T;

  // convert to a vector for opengl, column major.
  GLfloat T_final[16];
  int fi = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      T_final[fi] = T_final_mat(j,i);
      fi++;
    }
  }

  // orient the RevoluteJoint based on current calculated final transformation matrix.
  glMultMatrixf(T_final);


  glPushMatrix();
  // scale joint to length.
  glScalef(5.0f, 1.0f, 1.0f);

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  if (this->scene->selected == this) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, green_emissive_material);
  } else {
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white_emissive_material);
  }
  // call representation display list.
  glCallList(this->scene->get_display_list(JOINT_DL));
  glPopAttrib();

  glPopMatrix();
}


