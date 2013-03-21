#include "pointlight.h"
  
PointLight::PointLight( Scene *scene,
			float intensity,
			Eigen::Vector4f pos,
			Eigen::Vector4f rot )
  : SceneNode(scene, LIGHT, pos, rot), intensity(intensity)
{ }

// for moving light, varying intensity, etc. nothing for now...
void PointLight::step() {
  
}

// nothing to render for now, maybe an optional glowing ball/bulb later.
void PointLight::render() {

}
