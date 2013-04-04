// using the gl_ stuff that is already in camera space.

void main() {
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);

  vec4 world_vertex = gl_ModelViewMatrix * gl_Vertex;

  vec3 light_dir = normalize(gl_LightSource[0].position);
  vec3 light_refl_dir = normalize(reflect(light_dir, normal));
  float diffuse_intensity = max(dot(normal, light_dir), 0);
  vec3 half_vector = normalize(light_dir - normalize(world_vertex));

  gl_FrontColor = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * diffuse_intensity;

  if (diffuse_intensity > 0.0) {
    gl_FrontColor += gl_FrontMaterial.specular * gl_LightSource[0].specular
                     * pow(dot(half_vector,normal), gl_FrontMaterial.shininess);
  }  
  
  gl_Position = ftransform();
}




