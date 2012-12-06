/* readtex.h */

#ifndef READTEX_H
#define READTEX_H


#ifdef GDK_WINDOWING_QUARTZ
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


extern GLboolean
LoadRGBMipmaps( const char *imageFile, GLint intFormat );


extern GLboolean
LoadRGBMipmaps2( const char *imageFile, GLenum target,
                 GLint intFormat, GLint *width, GLint *height );


extern GLubyte *
LoadRGBImage( const char *imageFile,
              GLint *width, GLint *height, GLenum *format );


#endif
