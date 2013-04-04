uniform sampler2D tex;
varying vec3 vert_pos;

void main() {
  vec4 proc_color = vec4(0.0);
  gl_FragColor = gl_Color;
  float res = 5;

  // this is a proper checkerboard done with uv coordinates (useful when tweaking uv map, can see distortion)
  if ( int( floor(res*gl_TexCoord[0].s) + floor(res*gl_TexCoord[0].t) ) % 2 == 1) {
    gl_FragColor += vec4(0.5, 0.0, 0.0, 0.0);
  } else {
    gl_FragColor += vec4(0.0, 0.5, 0.0, 0.0);
  }


  gl_FragColor.w = 1.0; //make sure its opaque.
}
