// using the gl_ stuff that is already in camera space.

void main() {
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec4 specular;

  vec4 v = gl_ModelViewMatrix * gl_Vertex;

  gl_FrontColor = vec4(0.0);
  // not assuming directional light model, compute light direction
  // for each vertex. Point light model...
  vec3 light_dir = normalize(gl_LightSource[0].position - v);
  vec3 light_refl_dir = normalize(reflect(light_dir, normal));
  float diffuse_intensity = max(dot(normal, light_dir), 0);

  gl_FrontColor = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * diffuse_intensity;

  if (diffuse_intensity > 0.0) {
    vec3 h = normalize(light_dir - normalize(v));
    vec3 specular_intensity = max(dot(light_refl_dir, h), 0);
    gl_FrontColor += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(dot(h,normal), 5.0);
  }
  
  
  
  gl_Position = ftransform();
}


