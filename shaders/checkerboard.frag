uniform sampler2D tex;
varying vec3 vert_pos;

void main() {
  vec4 proc_color = vec4(0.0);
  gl_FragColor = gl_Color;
  float res = 5;
  
  // a projection of a checkerboard, from top.
  if ( int( floor(res*vert_pos.x) + floor(res*vert_pos.y) ) % 2 == 1 ) {
    gl_FragColor += vec4(0.2, 0.2, 0.2, 0.0);
  } else {
    gl_FragColor -= vec4(0.2, 0.2, 0.2, 0.0);
  }

  gl_FragColor.w = 1.0; //make sure its opaque.
}
