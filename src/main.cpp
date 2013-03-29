
#include <GL/glew.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include "GLScreenCapturer.h"

#include "scene.h"

Scene *SCENE;

void display(void) {
  SCENE->step_and_render();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // TODO: refactor code so it uses a scene variable that contains the near and far clip planes.
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 500.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearAccum(0.0,0.0,0.0,1.0);
  glClear(GL_ACCUM_BUFFER_BIT);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glClearAccum(0.0,0.0,0.0,1.0);
  glClear(GL_ACCUM_BUFFER_BIT);

  //glShadeModel(GL_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void key_pressed(unsigned char key, int x, int y) {
  SCENE->key_states[key] = true;
}

void key_up(unsigned char key, int x, int y) {
  SCENE->key_states[key] = false;
  
  if (SCENE->sticky_key_states[key]) {
    SCENE->sticky_key_states[key] = false;
  } else {
    SCENE->sticky_key_states[key] = true;
  }
}

// keep track of mouse variables.
void mouse_movement(int x, int y) {
  static int prev_x = x;
  static int prev_y = y;

  SCENE->mouse_pos[0] = x;
  SCENE->mouse_pos[1] = y;

  SCENE->mouse_vel[0] = prev_x - x;
  SCENE->mouse_vel[1] = prev_y - y;

  prev_x = x;
  prev_y = y;
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    SCENE->mouse_button_states[0] = true;
    //select_joints(x, y);
    mouse_movement(x, y);
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    SCENE->mouse_button_states[0] = false;
  }
}

void motion(int x, int y) {
  mouse_movement(x, y);
}

int main(int argc, char **argv) {
  // TODO: make selector for shader to use.
  // // ask user what scene to use.
  // std::cout << "refer to README for joint controls." << std::endl;

  // std::cout << "Enter scene number to load:" << std::endl;
  // std::cout << "0 - FK simple joint chain." << std::endl;
  // std::cout << "1 - FK hand." << std::endl;
  // std::cout << "2 - IK simple joint chain..." << std::endl;
  // std::cout << "Choice: ";
  // int scene_choice;
  // std::cin >> scene_choice;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  // TODO: pass width and height to scene.
  glutInitWindowSize(960,540);
  glutInitWindowPosition(100,100);
  glutCreateWindow("CYCLER: Experimental");
  init();
  glewInit();

  SCENE = new Scene();

  vf_scene_000(SCENE);

  // switch (scene_choice) {
  // case 0:
  //   vf_scene_000(SCENE);
  //   break;
  // case 1:
  //   vf_scene_001(SCENE);
  //   break;
  // case 2:
  //   vf_scene_002(SCENE);
  //   break;
  // default:
  //   return -1;
  // }

  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(key_pressed);
  glutKeyboardUpFunc(key_up);

  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(mouse_movement);

  glutMainLoop();
  
  return 0;
}
