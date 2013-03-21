#include "GLScreenCapturer.h"
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <fstream>
#include "utils/macros.h"

static char _filename[GLScreenCapturer::MAX_FILE_NAME_LEN];

GLScreenCapturer::GLScreenCapturer(const char* name):m_cnt(0), 
        m_buffer(NULL), m_bufSize(0)
{
    if ( strlen(name) >= MAX_FILE_NAME_FORMAT_LEN )
    {
        fprintf(stderr, "File name format string is too long: %s\n", name);
        exit(1);
    }
    strcpy(m_formatStr, name);
}

int GLScreenCapturer::capture()
{
    snprintf(_filename, MAX_FILE_NAME_LEN, m_formatStr, m_cnt++);
    return capture(_filename);
}

int GLScreenCapturer::capture(const char* file)
{
    uint32_t w = glutGet(GLUT_WINDOW_WIDTH);
    uint32_t h = glutGet(GLUT_WINDOW_HEIGHT);
    uint32_t sz = w * h * 3;

    if ( sz > m_bufSize )
        SAFE_DELETE_ARRAY(m_buffer);
    m_bufSize = sz;
    m_buffer  = new GLubyte[sz];

    std::ofstream fout(file, std::ofstream::binary);
    if ( fout.fail() ) 
    {
        fprintf(stderr, "Cannot open file %s to write\n", file);
        return ERROR_RETURN;
    }

    glFinish();
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
    fout << "P6\n" << w << ' ' << h << " 255\n";
    for(uint32_t i = 1;i <= h;++ i)
        fout.write((const char*)m_buffer+3*w*(h-i), 3*w);
    fout.close();

    return SUCC_RETURN;
}
