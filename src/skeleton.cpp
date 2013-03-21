
#include <GL/glew.h>
#include <GL/glut.h>

#include <stack>
#include <math.h>

#include "scene.h"
#include "revolutejoint.h"
#include "linalg.h"
#include "model.h"
#include "skeleton.h"

#include <iostream>

Skeleton::Skeleton(Scene *scene, RevoluteJoint *root_joint, Eigen::Vector4f pos, Eigen::Vector4f rot)
  : SceneNode(scene, SKELETON, pos, rot)
{
  this->root = root_joint;
}

// a is the number of the angle down the chain from the start we are finding partial deriv with.
// static Eigen::Vector4f dt_da(int a, RevoluteJoint *start, RevoluteJoint *end) {
//   Eigen::Matrix4f T_final = Eigen::Matrix4f::Identity();
//   Eigen::Matrix4f T_final_no_da = Eigen::Matrix4f::Identity();
//   float d_a = 0.001f;
//   int curr_a = 0;

//   //std::cout << "a: " << a << std::endl;

//   Eigen::Vector4f start_offset = Eigen::Vector4f::Zero();
//   start_offset[3] = 1.0f;
//   // calculate start offset by applying all transforms to a point at origin.
//   RevoluteJoint *curr_ptr = start->root;
//   while (curr_ptr != start) {
//     start_offset = curr_ptr->T * start_offset;
//     curr_ptr = curr_ptr->child;
//   }

//   Eigen::Vector4f end_relative = Eigen::Vector4f::Zero();
//   end_relative[3] = 1.0f;
//   curr_ptr = start->root;
//   while(curr_ptr != end) {
//     end_relative = curr_ptr->T * end_relative;
//     curr_ptr = curr_ptr->child;
//   }
//   end_relative -= start_offset;
//   end_relative[3] = 1.0f;

//   //std::cout << "relative end pos: " << relative_end_pos << std::endl;
  
//   curr_ptr = start;
//   while (curr_ptr != end->child) {
//     Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
//     float angle;
//     if (curr_a == a) {
//       angle = curr_ptr->angle + d_a;
//     } else {
//       angle = curr_ptr->angle;
//     }
//     //angle *= PI/180.0f;
    
//     T(0,0) = cos(angle);
//     T(0,1) = -sin(angle);
//     T(1,0) = sin(angle);
//     T(1,1) = cos(angle);

//     if (curr_ptr->parent != start->parent) {
//       T(0,3) = curr_ptr->pos[0] - curr_ptr->parent->pos[0];
//       T(1,3) = curr_ptr->pos[1] - curr_ptr->parent->pos[1];
//     } else {
//       T(0,3) = 0.0;
//       T(1,3) = 0.0;
//     }

//     T_final *= T;

//     curr_ptr = curr_ptr->child;
//     curr_a++;
//   }

//   //std::cout << "T_final: " << T_final << std::endl;

//   curr_ptr = start;
//   while (curr_ptr != end->child) {
//     Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
//     float angle = curr_ptr->angle;
//     //angle *= PI/180.0f;
    
//     T(0,0) = cos(angle);
//     T(0,1) = -sin(angle);
//     T(1,0) = sin(angle);
//     T(1,1) = cos(angle);

//     if (curr_ptr->parent != start->parent) {
//       T(0,3) = curr_ptr->pos[0] - curr_ptr->parent->pos[0];
//       T(1,3) = curr_ptr->pos[1] - curr_ptr->parent->pos[1];
//     } else {
//       T(0,3) = 0.0;
//       T(1,3) = 0.0;
//     }

//     T_final_no_da *= T;

//     curr_ptr = curr_ptr->child;
//     curr_a++;
//   }
  

//   // check displacement of end effector due to the d_a in the ath joint angle.
//   Eigen::Vector4f d_t = (T_final * end_relative) - (T_final_no_da * end_relative);
//   //std::cout << "difference in end effector: " << d_t << std::endl;
//   //std::cout << "derivative of end effector: " << d_t / d_a << std::endl;
//   d_t /= d_a;
//   //d_t[3] = 0.0f;
//   return d_t;
// }

// calculate proper angles for each joint from start to end for the end effector reach the target.
static void ik_solve(Scene *scene, ik_info *iki) {
  //Eigen::Vector4f t = i.end->pos;
  
  // shift target position of the end effector in its plane based on which key was pressed.
  if (scene->key_states['t']) {
    (*iki->end_effector)[1] += 0.1f;
  }

  if (scene->key_states['g']) {
    (*iki->end_effector)[1] -= 0.1f;
  }

  if (scene->key_states['f']) {
    (*iki->end_effector)[0] -= 0.1f;
  }

  if (scene->key_states['h']) {
    (*iki->end_effector)[0] += 0.1f;
  }

  // count number of joints
  int num_joints = 0;
  RevoluteJoint *curr_ptr = iki->start;
  while (curr_ptr != iki->end) {
    num_joints++;
    curr_ptr = curr_ptr->child;
  }
  
  //std::cout << "num_joints: " << num_joints << std::endl;
  
  int iter_count = 0;

  while (iter_count < 10) {

    Eigen::Vector4f local_end_eff = (*iki->end_effector);
    Eigen::Vector4f start_offset = Eigen::Vector4f::Zero();
    start_offset[3] = 1.0f;
    // calculate start offset by applying all transforms to a point at origin.
    //TODO: carefully remove start_offset if you're sure you don't need it.
    // decided to ditch the offset stuff and do delta_p in world space, seems to work fine..
    curr_ptr = iki->start->root;
    while (curr_ptr != iki->start->child) {
      start_offset = curr_ptr->T * start_offset;
      curr_ptr = curr_ptr->child;
    }

    //local_end_eff -= start_offset;
    local_end_eff[3] = 1.0f;

    //std::cout << "local_end_eff: " << local_end_eff << std::endl;

    Eigen::Vector4f end_relative = Eigen::Vector4f::Zero();
    float end_angle_accum = 0.0f;
    curr_ptr = iki->start->root;
    while (curr_ptr != iki->end->child) {
      end_angle_accum += curr_ptr->angle;
      end_relative[0] += curr_ptr->length * cos (end_angle_accum * PI / 180.0f);
      end_relative[1] += curr_ptr->length * sin(end_angle_accum * PI / 180.0f);
      curr_ptr = curr_ptr->child;
    }
    //end_relative -= start_offset;

    end_relative[3] = 1.0f;

    Eigen::Vector4f delta_p = local_end_eff - end_relative;
    //delta_p[3] = 1.0f;
    //std::cout << "end pos from start joint: " << end_relative << std::endl;
    //std::cout << "delta_p: " << delta_p << std::endl;

    // std::cout << "start_offset: " << start_offset << std::endl;
    // std::cout << "local_end_eff: " << local_end_eff << std::endl;
    // std::cout << "end_relative: " << end_relative << std::endl;

    // create the jacobian matrix
    Eigen::MatrixXf J = Eigen::MatrixXf::Zero(4, num_joints);

    for (int joint_count = 0; joint_count < num_joints; joint_count++) {
      float angle_accum = 0.0f;
      int angle_count = 0;
      curr_ptr = iki->start;
      while (curr_ptr != iki->end) {
	// use derivatives of cos & sin at current joint angle for jacobian entry.
	// accumulate as you go down the joints.
	//std::cout << "angle: " << curr_ptr->angle << std::endl;
	angle_accum += curr_ptr->angle;
	if (angle_count >= joint_count) {
	  J(0, joint_count) += -curr_ptr->length * sin(angle_accum * PI / 180.0f);
	  J(1, joint_count) += curr_ptr->length * cos(angle_accum * PI / 180.0f);
	}
	curr_ptr = curr_ptr->child;
	angle_count++;
      }
    }

    //std::cout << "J: \n" << J << std::endl;

    // iterate a few times max to calculate new angles for joints.
    Eigen::MatrixXf I = Eigen::MatrixXf::Identity(num_joints, num_joints);
    float perturb = 0.01f;

    Eigen::MatrixXf A = J.transpose() * J + perturb * I;
    Eigen::VectorXf b = J.transpose() * delta_p;

    //std::cout << "A:\n" << A << std::endl;
    //std::cout << "b:\n" << b << std::endl;

    Eigen::VectorXf angles(num_joints);
    A.lu().solve(b, &angles);

    //angles = J.transpose() * (J * J.transpose() + perturb * I).inverse() * delta_p;

    //std::cout << "angles: \n" << angles << std::endl;

    // assign new angles to joints in the ik chain.
    int a_counter = 0;
    curr_ptr = iki->start;
    while (curr_ptr != iki->end) {
      curr_ptr->angle += angles[a_counter];
      curr_ptr->calculate_transform();
      a_counter++;
      curr_ptr = curr_ptr->child;
    }
    
    iter_count++;
  }

}

void Skeleton::step() {
  if (this->ik_mode) {

    for (std::vector<ik_info>::iterator it = this->ik_controls.begin(); it != this->ik_controls.end(); it++) {
      ik_info i = *it;

      // render out the ik end effector handle.
      glPushMatrix();
      //std::cout << "end effector: " << i.end_effector << std::endl;
      glTranslatef((*i.end_effector)[0], (*i.end_effector)[1], (*i.end_effector)[2]);
      glutSolidSphere(1.0f, 8, 8);
      glPopMatrix();

      ik_solve(this->scene, &i);
    }

    RevoluteJoint *curr_ptr = this->root;
    while (curr_ptr) {
      curr_ptr->calculate_transform();
      curr_ptr = curr_ptr->child;
    }
  } else {
    RevoluteJoint *curr_ptr = this->root;
    while (curr_ptr) {
      curr_ptr->step();
      
      RevoluteJoint *child_ptr = curr_ptr->child2;
      while (child_ptr) {
	child_ptr->step();
	child_ptr = child_ptr->child;
      }

      curr_ptr = curr_ptr->child;
    }
  }
}


void Skeleton::render() {
  glInitNames();
  glPushName(0);

  // no time for proper tree and depth first traversal, this hack-sauce will make you cry. -_-
  glPushMatrix();
  RevoluteJoint *curr_ptr = this->root;
  while (curr_ptr) {
    glLoadName(curr_ptr->id);
    curr_ptr->render();
    if (curr_ptr->child2) {
      glPushMatrix();
      RevoluteJoint *child_ptr = curr_ptr->child2;
      while (child_ptr) {
	child_ptr->render();
	child_ptr = child_ptr->child;
      }
      glPopMatrix();
    }
    curr_ptr = curr_ptr->child;
  }
  glPopMatrix();
}


