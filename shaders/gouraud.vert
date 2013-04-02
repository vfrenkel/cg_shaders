uniform vec4 cam_pos;

void main() {
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec4 v = gl_ModelViewMatrix * gl_Vertex;

  gl_FrontColor = vec4(0.0);
  vec3 light_dir = normalize(gl_LightSource[0].position);
  vec3 light_refl_dir = normalize(reflect(light_dir, normal));
  float diffuse_intensity = max(dot(normal, light_dir), 0);
  vec3 eye_dir = normalize(v - cam_pos);

  gl_FrontColor = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * diffuse_intensity;

  if (dot(light_refl_dir, eye_dir) > 0.0) {
     gl_FrontColor += gl_FrontMaterial.specular * gl_LightSource[0].specular
                      * pow(dot(light_refl_dir, eye_dir), gl_FrontMaterial.shininess);
  }

  gl_Position = ftransform();


}




