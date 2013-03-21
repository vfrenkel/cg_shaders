
uniform vec3 lightDir;
varying vec3 normal;
void main()
{
    float intensity = dot(normalize(lightDir), normalize(normal));
    if ( intensity > 0.95 )
        gl_FragColor = gl_FrontMaterial.diffuse;
    else if ( intensity > 0.5 )
        gl_FragColor = gl_FrontMaterial.diffuse*0.9;
    else if ( intensity > 0.25 )
        gl_FragColor = gl_FrontMaterial.diffuse*0.7;
    else
        gl_FragColor = gl_FrontMaterial.diffuse*0.4;
}
