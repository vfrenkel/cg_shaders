#include <math.h>

#include "pointlight.h"
  

PointLight::PointLight( Scene *scene,
			float intensity,
			Eigen::Vector4f pos,
			Eigen::Vector4f rot )
  : SceneNode(scene, LIGHT, pos, rot), intensity(intensity)
{ }

// for moving light, varying intensity, etc. nothing for now...
void PointLight::step() {
  // static double frame = 0;
  // this->pos[0] = 50 * cos(frame * PI / 180);
  // this->pos[1] = 50 * sin(frame * PI / 180);
  // this->pos[2] = 50 * cos(frame * PI / 180);
  // frame++;
}

// nothing to render for now, maybe an optional glowing ball/bulb later.
void PointLight::render() {

}
