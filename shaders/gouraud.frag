
varying vec4 normal;
void main() {
  vec4 lightDir = normalize(vec4(gl_LightSource[0].position));
  float intensity = dot(lightDir, normal);
  
  gl_FragColor = gl_FrontMaterial.diffuse * intensity;
  gl_FragColor.w = 1.0;
}
