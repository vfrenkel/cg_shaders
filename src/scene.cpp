#include <Eigen/Dense>

#include "scene.h"
#include "playercycler.h"
#include "skeleton.h"
#include "revolutejoint.h"
#include "gridpiece.h"
#include "pointlight.h"


void vf_scene_000(Scene *s) {
  PlayerCycler *player = new PlayerCycler(s, Eigen::Vector4f(0.0, 1.0, 0.0, 1.0));
  s->add_node(player);

  // set up lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  float l0_diffuse[4] = { 0.6f, 0.6f, 0.8f, 1.0f };
  float l0_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_AMBIENT, l0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_diffuse);
  
  // define position here but apply glLightfv on position after setting camera.
  float l0_pos[3] = {10.0, 10.0, 0.0};

  float l0_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glLightfv(GL_LIGHT0, GL_SPECULAR, l0_specular);

  s->add_node(new PointLight(s,
			     1.0f,
			     Eigen::Vector4f(l0_pos[0], l0_pos[1], l0_pos[2], 1.0),
			     Eigen::Vector4f(0.0, 0.0, 0.0, 0.0)));

  // THE GRID
  s->add_node(new GridPiece(s));

  s->add_node(new GridPiece(s,
                            Eigen::Vector4f(0.0, 300.0, 300.0, 1.0),
			    Eigen::Vector4f(90.0, 0.0, 0.0, 0.0)));

  s->add_node(new GridPiece(s,
                            Eigen::Vector4f(300.0, 300.0, 0.0, 1.0),
			    Eigen::Vector4f(0.0, 0.0, 90.0, 0.0)));

  s->get_cam()->bind_focus(&(player->pos));
  s->get_cam()->bind_target_dir(&(player->forward_dir));
  s->get_cam()->bind_mouse(&(s->mouse_pos), &(s->mouse_vel));
}


static void init_postproc(Scene *s) {
  s->fbo_ids = new GLuint[1];
  // s->rbo_ids = new GLuint[1];

  glGenFramebuffers(1, s->fbo_ids);
  // glBindFramebuffer(GL_FRAMEBUFFER, s->fbo_ids[0]);

  // glGenRenderbuffers(1, s->rbo_ids);
  // glBindRenderbuffer(GL_RENDERBUFFER, s->rbo_ids[0]);
  // glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, 960, 540);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Scene::Scene() {
  // load up default values.
  this->cam = new Camera(this, 0.0, 5.0, -10.0);
  init_display_lists();
  init_postproc(this);
  // EXTRA: make this based on actual elapsed time.
  this->interp_factor = 1.0;
  this->key_states[256];
  this->mouse_button_states[2];
  this->mouse_pos = Eigen::Vector2f(0.0, 0.0);
  this->mouse_vel = Eigen::Vector2f(0.0, 0.0);
  this->selected = NULL;
  this->num_nodes = 0;

  // clear all key states.
  for (int i = 0; i < 256; i++) {
    this->key_states[i] = false;
  }

  // clear all mouse button states.
  for (int i = 0; i < 2; i++) {
    this->mouse_button_states[i] = false;
  }
  
  // EXTRA: make this load from a scene description file.
  // load scene.
  //vf_scene_002(this);
}

Scene::~Scene() {
  // TODO: do garbage collection...
  
  glDeleteFramebuffers(1, this->fbo_ids);
}

void Scene::init_display_lists() {
  this->display_lists.assign(MAX_DISPLAY_LISTS, 0);
  this->display_lists[TEAPOT_DL] = TeapotMesh::create_display_list();
  this->display_lists[GRID_PIECE_DL] = GridPiece::create_display_list();
  this->display_lists[JOINT_DL] = RevoluteJoint::create_display_list();
}

Camera *Scene::get_cam() {
  return this->cam;
}

float Scene::get_interp_factor() {
  return this->interp_factor;
}

GLuint Scene::get_display_list(DISPLAY_LIST list) {
  return this->display_lists[list];
}

// simple, single interface to add nodes,
// let Scene worry about how to organize the nodes by type.
void Scene::add_node(SceneNode *n) {
  n->id = this->num_nodes++;

  switch (n->type) {
  case SKELETON:
  case OBJECT:
    this->objects.push_back(n);
    break;
  case LIGHT:
    this->lights.push_back(n);
    break;
  default:
    std::cout << "error: attempted to add node of unknown type to scene" << std::endl;
  }
}

void Scene::step_and_render() {
  glClearColor(0.01,0.01,0.3,1.0);
  //glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // render camera
  this->cam->step();
  this->cam->transform_GL();

  // position lights
  //TODO: replace with loop that goes through all lights and sets appropriate GL_LIGHT#
  float l0_pos[3];
  l0_pos[0] = this->lights[0]->pos[0];
  l0_pos[1] = this->lights[0]->pos[1];
  l0_pos[2] = this->lights[0]->pos[2];
  glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);

  for (std::vector<SceneNode *>::iterator n = this->objects.begin(); n != this->objects.end(); n++) {
    (*n)->step();

    glPushMatrix();
    (*n)->render();
    glPopMatrix();
  }

  for (std::vector<SceneNode *>::iterator n = this->lights.begin(); n != this->lights.end(); n++) {
    (*n)->step();
  }

  glutSwapBuffers();
}

SceneNode::SceneNode()
  : scene(NULL), type(OBJECT), pos(Eigen::Vector4f(0.0,0.0,0.0,0.0)), rot(Eigen::Vector4f(0.0,0.0,0.0,0.0))
{ }

SceneNode::~SceneNode() {
  
}

SceneNode::SceneNode( Scene *scene,
		      SCENE_NODE_TYPE type,
		      Eigen::Vector4f pos,
		      Eigen::Vector4f rot )
  : scene(scene), type(type), pos(pos), rot(rot)
{
  //TODO: keep central list of all used names and nodes they map to,
  // then automatically assign a default name that hasn't been used.
  this->name = "scene_node";
}

TeapotMesh::TeapotMesh( Scene *scene,
		        float size,
		        Eigen::Vector4f pos,
			Eigen::Vector4f rot )
  : SceneNode(scene, OBJECT, pos, rot), size(size)
{ }

GLuint TeapotMesh::create_display_list() {
  GLuint list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  glPushMatrix();
  glutSolidTeapot(10.0f);
  glPopMatrix();
  glEndList();
  
  return list;
}

void TeapotMesh::step() {
  // TODO: check the keyboard states and transform accordingly.
  
  if (this->rot[0] > 360.0f) {
    this->rot[0] -= 360.0f;
  } else {
    this->rot[0] += 2.0f;
  }

  if (this->rot[1] > 360.0f) {
    this->rot[1] -= 360.0f;
  } else {
    this->rot[1] += 2.0f;
  }

  if (this->rot[2] > 360.0f) {
    this->rot[2] -= 360.0f;
  } else {
    this->rot[2] += 1.0f;
  }
}

void TeapotMesh::render() {
  glTranslatef(this->pos[0], this->pos[1], this->pos[2]);

  glRotatef(this->rot[0], 1.0, 0.0, 0.0);
  glRotatef(this->rot[1], 0.0, 1.0, 0.0);
  glRotatef(this->rot[2], 0.0, 0.0, 1.0);

  glCallList(this->scene->get_display_list(TEAPOT_DL));
}


// TODO: abandoned approach, clean up or complete.
InfinitePlaneMesh::InfinitePlaneMesh() {
  
}

void InfinitePlaneMesh::step() {

}

void InfinitePlaneMesh::render() {
  glBegin(GL_QUADS);
  glVertex4f(1, 0, 1, 0.00001);
  glVertex4f(-1, 0, 1, 0.00001);
  glVertex4f(-1, 0, -1, 0.00001);
  glVertex4f(1, 0, -1, 0.00001);
  glEnd();
}
