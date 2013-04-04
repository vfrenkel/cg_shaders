/******************************************************************************
 *  File: GLSLProgram.h
 *  Copyright (c) 2010 by Changxi Zheng
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
#ifndef RENDERER_GLSL_PROGRAM_H
#   define RENDERER_GLSL_PROGRAM_H

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>

#include <fstream>
#include <string>

class GLSLProgram
{
    public:
        GLSLProgram(const char* vpath, const char* fpath):prog_(0)
        {
	  // read in from the given file path.
	  std::fstream vinput(vpath);
	  std::string vstring;
	  vstring.assign( std::istreambuf_iterator<char>(vinput),
			  std::istreambuf_iterator<char>() );
	  /* std::string vstring( std::istreambuf_iterator<char>(vinput), */
	  /* 		       std::istreambuf_iterator<char>() ); */

	  std::fstream finput(fpath);
	  std::string fstring;
	  fstring.assign( std::istreambuf_iterator<char>(finput),
			  std::istreambuf_iterator<char>() );
	  /* std::string fstring( std::istreambuf_iterator<char>(finput), */
	  /* 		       std::istreambuf_iterator<char>() ); */

	  const char* vsource = vstring.c_str();
	  const char* fsource = fstring.c_str();

	  if ( !(prog_ = compile_program(vsource, fsource)) )
	    {
	      fprintf(stderr, "Fail to create GLSL program\n");
	      exit(1);
            }
        }

        ~GLSLProgram()
        {
            if ( prog_ ) glDeleteProgram(prog_);
        }

        void enable()
        {
            glUseProgram(prog_);
        }

        void disable()
        {
            glUseProgram(0);
        }

        void load_texture(const char* filename);
    	/*
         * bind texture
         * \param uint the ID of the texture when using multi-texture
         */
        void bind_texture(GLenum target, GLint uint);

        void set_uniform_1f(const char* name, GLfloat x);
        void set_uniform_2f(const char* name, GLfloat x, GLfloat y);
        void set_uniform_3f(const char* name, GLfloat x, GLfloat y, GLfloat z);
        void set_uniform_4f(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

    private:
        GLuint compile_program(const char* vsource, const char* fsource, 
                               const char* gsource = NULL, 
                               GLenum gsInput = GL_POINTS, 
                               GLenum gsOutput = GL_TRIANGLE_STRIP);
        int check_shader(GLuint shader);

    private:
        GLuint      prog_;
	GLuint texture; //TODO: consider making this a pointer.
};

#endif
