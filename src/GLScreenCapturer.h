/*
 * GLScreenCapturer.h
 * author: Changxi Zheng (cxz@cs.columbia.edu)
 */
#ifndef GL_SCREEN_CAPTURER_H
#   define GL_SCREEN_CAPTURER_H

#include <stdint.h>
#if defined(__APPLE__) || defined(MACOSX)
#   include <OpenGL/gl.h>
#else
#   include <GL/glut.h>
#endif

class GLScreenCapturer
{
    public:
        enum
        {
            MAX_FILE_NAME_FORMAT_LEN = 80,
            MAX_FILE_NAME_LEN = 128
        };

        //! Constructor with the given file name pattern
        GLScreenCapturer(const char* name);

        int capture(const char* file);     // capture the current frame
        //! Capture the current frame and dump to the file 
        //  whose name is implicitly specified.
        int capture();

    private:
        uint32_t    m_cnt;              // frame counter
        char        m_formatStr[128];   // format string
        GLubyte*    m_buffer;
        uint32_t    m_bufSize;
};

#endif
