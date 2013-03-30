// not much here... color in between verts is interpolated by openGL.
void main() {
  gl_FragColor = gl_Color;
  gl_FragColor.w = 1.0; //make sure its opaque.
}
