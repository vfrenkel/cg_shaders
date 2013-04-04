
uniform sampler2D tex;
varying vec3 normal;

void main() {
    vec4 texel_color = texture2D(tex, gl_TexCoord[0].st);
    vec3 light_dir = normalize(gl_LightSource[0].position);

    // calculate the normal based on normal map.
    vec3 normal = 2.0 * texel_color - 1.0;

    float diffuse_intensity = max(dot(light_dir, normalize(normal)), 0);


    gl_FragColor = diffuse_intensity;
    gl_FragColor.w = 1.0;
}


