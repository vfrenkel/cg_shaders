#ifndef __MODEL_H_
#define __MODEL_H_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "glm/glm.hpp"


// http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
typedef struct {
  std::vector<glm::vec4> vertices;
  std::vector<glm::vec3> normals;
  std::vector<GLushort> elements;

  GLuint *vbo_ids;
} Model;

// TODO: extend to support material definitions.
static Model load_model(const char *obj_filename, const char *mtl_filename) {
  Model model;

  std::ifstream in(obj_filename, std::ios::in);
  if (!in) { std::cerr << "Cannot open " << obj_filename << std::endl; exit(1); }

  std::string line;
  while (getline(in, line)) {
    if (line.substr(0,2) == "v ") {
      std::istringstream s(line.substr(2));
      glm::vec4 v;
      s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
      model.vertices.push_back(v);
    } else if (line.substr(0,2) == "f ") {
      std::istringstream s(line.substr(2));
      GLushort a,b,c;
      s >> a; s >> b; s >> c;
      a--; b--; c--;
      model.elements.push_back(a); model.elements.push_back(b); model.elements.push_back(c);
    }
  }

  model.normals.resize(model.vertices.size(), glm::vec3(0.0,0.0,0.0));
  for (int i = 0; i < model.elements.size(); i += 3) {
    GLushort ia = model.elements[i];
    GLushort ib = model.elements[i+1];
    GLushort ic = model.elements[i+2];
    glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(model.vertices[ib]) - glm::vec3(model.vertices[ia]),
						 glm::vec3(model.vertices[ic]) - glm::vec3(model.vertices[ib])));
    model.normals[ia] = model.normals[ib] = model.normals[ic] = normal;
  }

  return model;
}

static void prep_buffers(Model &m) {
    glGenBuffers(3, m.vbo_ids);

    glBindBuffer(GL_ARRAY_BUFFER, m.vbo_ids[0]);
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(GLfloat), &(m.vertices), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo_ids[1]);
    glBufferData(GL_ARRAY_BUFFER, m.normals.size(), &(m.normals), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.vbo_ids[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.elements.size() * sizeof(GLushort), &(m.elements), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void draw_model(Model &m) {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(4, GL_FLOAT, 0, m.vertices.data());
  glNormalPointer(GL_FLOAT, 0, m.normals.data());

  glDrawElements(GL_TRIANGLES, m.elements.size(), GL_UNSIGNED_SHORT, m.elements.data());
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

// TODO: complete this when you learn about vertex/fragment shaders...
// this is supposed to blur the drawn model's pixels and boost their brightness to create a glow effect,
// but while we haven't covered shaders, i'll just put motion blur here as a placeholder.
static void draw_model_to_fbo(Model &m, GLuint fbo_id, GLuint rbo_id) {
  //glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
  draw_model(m);

  // motion blur.
  glAccum(GL_MULT, 0.80);
  glAccum(GL_ACCUM, 0.20);
  glAccum(GL_RETURN, 1.0);

  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif //__MODEL_H_
