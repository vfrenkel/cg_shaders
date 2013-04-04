varying vec3 vert_pos;

void main() {
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);

  vec4 v = gl_ModelViewMatrix * gl_Vertex;

  vec3 light_dir = normalize(gl_LightSource[0].position);
  vec3 light_refl_dir = normalize(reflect(light_dir, normal));
  float diffuse_intensity = max(dot(normal, light_dir), 0);
  vec3 h = normalize(light_dir - normalize(v));

  gl_FrontColor = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * diffuse_intensity;

  if (diffuse_intensity > 0.0) {
    gl_FrontColor += gl_FrontMaterial.specular * gl_LightSource[0].specular
                     * pow(dot(h,normal), gl_FrontMaterial.shininess);
  }  
  
  
  gl_TexCoord[0] = gl_MultiTexCoord0;
  vert_pos = gl_Vertex;
  gl_Position = ftransform();
}




