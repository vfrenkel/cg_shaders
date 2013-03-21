#include "GLSLProgram.h"
#include <assert.h>
#include "utils/macros.h"

int GLSLProgram::check_shader(GLuint shader)
{
    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if ( status != GL_TRUE )
    {
        char log[2048];
        int len;
        glGetShaderInfoLog(shader, 2048, (GLsizei*)&len, log);
        fprintf(stderr, "Error: shader(%04d), Info Log: %s\n", (int)shader, log);
        glDeleteShader(shader);
        return ERROR_RETURN;
    }
    return SUCC_RETURN;
}

GLuint GLSLProgram::compile_program(
        const char* vsource, const char* fsource, const char* gsource, 
        GLenum gsInput, GLenum gsOutput)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader   = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vsource, 0);
    glShaderSource(fragShader, 1, &fsource, 0);

    glCompileShader(vertexShader);
    if ( _FAILED(check_shader(vertexShader)) )
    {
        fprintf(stderr, "Fail to compile vertex shader:\n");
        fprintf(stderr, "%s\n", vsource);
        return 0;
    }

    glCompileShader(fragShader);
    if ( _FAILED(check_shader(fragShader)) )
    {
        fprintf(stderr, "Fail to compile fragment shader:\n");
        fprintf(stderr, "%s\n", fsource);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);

    //// create geometry shader if given 
    if ( gsource )
    {
        GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
        glShaderSource(geometryShader, 1, &gsource, 0);
        glCompileShader(geometryShader);
        if ( _FAILED(check_shader(geometryShader)) )
        {
            fprintf(stderr, "Fail to compile geometry shader:\n");
            fprintf(stderr, "%s\n", gsource);
            return 0;
        }

        glAttachShader(program, geometryShader);
        glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, gsInput);
        glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, gsOutput);
        glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, 4);
    }

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if ( status != GL_TRUE )
    {
        char log[2048];
        int len;
        glGetProgramInfoLog(program, 2048, (GLsizei*)&len, log);
        fprintf(stderr, "Error: program(%04d), Info Log: %s\n", (int)program, log);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void GLSLProgram::bind_texture(const char* name, GLuint tex, GLenum target, GLint uint)
{
    GLint loc = glGetUniformLocation(prog_, name);
    assert(loc >= 0);

    glActiveTexture(GL_TEXTURE0 + uint);
    glBindTexture(target, tex);
    glUniform1i(loc, uint);

    glActiveTexture(GL_TEXTURE0);
}

void GLSLProgram::set_uniform_1f(const char* name, GLfloat x)
{
    GLint loc = glGetUniformLocation(prog_, name);
    assert(loc >= 0);
    glUniform1f(loc, x);
}

void GLSLProgram::set_uniform_2f(const char* name, GLfloat x, GLfloat y)
{
    GLint loc = glGetUniformLocation(prog_, name);
    assert(loc >= 0);
    glUniform2f(loc, x, y);
}

void GLSLProgram::set_uniform_3f(const char* name, GLfloat x, GLfloat y, GLfloat z)
{
    GLint loc = glGetUniformLocation(prog_, name);
    assert(loc >= 0);
    glUniform3f(loc, x, y, z);
}
