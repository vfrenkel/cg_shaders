#ifndef SHADER_INC
#   define SHADER_INC

#define STRINGIFY(A) #A

const char* toonVS = STRINGIFY(
varying vec3 normal;            \n
void main()                     \n
{                               \n
    normal = gl_Normal;         \n
    gl_Position = ftransform(); \n
}                               \n
);

const char* toonFS = STRINGIFY(
uniform vec3 lightDir;          \n
varying vec3 normal;            \n
void main()                     \n
{                               \n
    float intensity = dot(normalize(lightDir), normalize(normal)); \n
    if ( intensity > 0.95 )     \n
        gl_FragColor = gl_FrontMaterial.diffuse; \n
    else if ( intensity > 0.5 ) \n
        gl_FragColor = gl_FrontMaterial.diffuse*0.9; \n
    else if ( intensity > 0.25 ) \n
        gl_FragColor = gl_FrontMaterial.diffuse*0.7; \n
    else                        \n
        gl_FragColor = gl_FrontMaterial.diffuse*0.4; \n
}                               \n
);

#endif
