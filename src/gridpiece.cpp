
#include <Eigen/Dense>

#include "scene.h"
#include "gridpiece.h"
#include "linalg.h"


GridPiece::GridPiece(Scene *scene, Eigen::Vector4f pos, Eigen::Vector4f rot)
  : SceneNode(scene, OBJECT, pos, rot)
{ }

GLuint GridPiece::create_display_list() {
  const int num_bars = 60;
  const float spacing = 10.0f;
  
  const GLfloat red_emissive_material[] = {1.0, 0.0, 0.0};
  const GLfloat green_emissive_material[] = {0.0, 1.0, 0.0};
  const GLfloat blue_emissive_material[] = {0.0, 0.0, 1.0};
  const GLfloat black_out_material[] = {0.0, 0.0, 0.0};

  GLuint list = glGenLists(1);
  glNewList(list, GL_COMPILE);

  glPushMatrix();
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  glTranslatef(-num_bars*spacing/2.0,0.0,-num_bars*spacing/2.0);

  // draw the grid based on num_bars and spacing.
  glBegin(GL_LINES);
  for (unsigned int i = 0; i < num_bars; i++) {
    if (i == 0) { 
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red_emissive_material);
    } else if ((i % 2) == 0) {
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, green_emissive_material);
    } else {
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blue_emissive_material);
    }
    glVertex3f(spacing*i,0,0);
    glVertex3f(spacing*i,0,spacing*num_bars);
    glVertex3f(0,0,spacing*i);
    glVertex3f(spacing*num_bars,0,spacing*i);
  }
  // reset material when done.
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black_out_material);
  glEnd();

  glPopAttrib();
  glPopMatrix();

  glEndList();

  return list;
}

void GridPiece::step() {
  
}

void GridPiece::render() {
  glTranslatef(this->pos[0], this->pos[1], this->pos[2]);
  glRotatef(this->rot[0], 1.0, 0.0, 0.0);
  glRotatef(this->rot[1], 0.0, 1.0, 0.0);
  glRotatef(this->rot[2], 0.0, 0.0, 1.0);

  glCallList(this->scene->get_display_list(GRID_PIECE_DL));
}


