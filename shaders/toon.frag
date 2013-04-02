
varying vec3 normal;
void main()
{
    vec3 lightDir = gl_LightSource[0].position;
    float intensity = dot(normalize(lightDir), normalize(normal));
    if ( intensity > 0.95 )
        gl_FragColor = gl_FrontMaterial.diffuse;
    else if ( intensity > 0.5 )
        gl_FragColor = gl_FrontMaterial.diffuse*0.75;
    else if ( intensity > 0.25 )
        gl_FragColor = gl_FrontMaterial.diffuse*0.5;
    else
        gl_FragColor = gl_FrontMaterial.diffuse*0.2;
    gl_FragColor.w = 1.0;
}
