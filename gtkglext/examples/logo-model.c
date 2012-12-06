/*
 * logo-model.c:
 * Functions for drawing GtkGLExt logo models.
 *
 * written by Naofumi Yasufuku  <naofumi@users.sourceforge.net>
 */

#include <math.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#ifdef GDK_WINDOWING_QUARTZ
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "logo-model.h"

void
logo_draw_cube (void)
{
  static GLfloat n[6][3] = {
    { -1.0,  0.0,  0.0 },
    {  0.0,  1.0,  0.0 },
    {  1.0,  0.0,  0.0 },
    {  0.0, -1.0,  0.0 },
    {  0.0,  0.0,  1.0 },
    {  0.0,  0.0, -1.0 }
  };
  static GLint faces[6][4] = {
    { 0, 1, 2, 3 },
    { 3, 2, 6, 7 },
    { 7, 6, 5, 4 },
    { 4, 5, 1, 0 },
    { 5, 6, 2, 1 },
    { 7, 4, 0, 3 }
  };
  static GLfloat v[8][3] = {
    { -5.5, -5.5, -5.5 },
    { -5.5, -5.5,  5.5 },
    { -5.5,  5.5,  5.5 },
    { -5.5,  5.5, -5.5 },
    {  5.5, -5.5, -5.5 },
    {  5.5, -5.5,  5.5 },
    {  5.5,  5.5,  5.5 },
    {  5.5,  5.5, -5.5 }
  };
  GLint i;

  for (i = 5; i >= 0; i--)
    {
      glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
      glEnd();
    }
}

void
logo_draw_g_plane (void)
{
  static GLfloat n[3] = {
    0.0, 1.0, 0.0
  };
  static GLfloat v[4][3] = {
    { -5.0, 6.5, -5.0 },
    { -5.0, 6.5,  5.0 },
    {  5.0, 6.5,  5.0 },
    {  5.0, 6.5, -5.0 }
  };

  glBegin(GL_QUADS);
    glNormal3f(n[0], n[1], n[2]);
    glVertex3f(v[0][0], v[0][1], v[0][2]);
    glVertex3f(v[1][0], v[1][1], v[1][2]);
    glVertex3f(v[2][0], v[2][1], v[2][2]);
    glVertex3f(v[3][0], v[3][1], v[3][2]);
  glEnd();
}

void
logo_draw_t_plane (void)
{
  static GLfloat n[3] = {
    0.0, 0.0, 1.0
  };
  static GLfloat v[4][3] = {
    { -5.0,  5.0, 6.5 },
    { -5.0, -5.0, 6.5 },
    {  5.0, -5.0, 6.5 },
    {  5.0,  5.0, 6.5 }
  };

  glBegin(GL_QUADS);
    glNormal3f(n[0], n[1], n[2]);
    glVertex3f(v[0][0], v[0][1], v[0][2]);
    glVertex3f(v[1][0], v[1][1], v[1][2]);
    glVertex3f(v[2][0], v[2][1], v[2][2]);
    glVertex3f(v[3][0], v[3][1], v[3][2]);
  glEnd();
}

void
logo_draw_k_plane ()
{
  static GLfloat n[3] = {
    1.0, 0.0, 0.0
  };
  static GLfloat v[4][3] = {
    { 6.5,  5.0,  5.0 },
    { 6.5, -5.0,  5.0 },
    { 6.5, -5.0, -5.0 },
    { 6.5,  5.0, -5.0 }
  };

  glBegin(GL_QUADS);
    glNormal3f(n[0], n[1], n[2]);
    glVertex3f(v[0][0], v[0][1], v[0][2]);
    glVertex3f(v[1][0], v[1][1], v[1][2]);
    glVertex3f(v[2][0], v[2][1], v[2][2]);
    glVertex3f(v[3][0], v[3][1], v[3][2]);
  glEnd();
}

static void
logo_draw_triangle (GLfloat *v0,
		    GLfloat *v1,
		    GLfloat *v2)
{
  GLfloat w0[3], w1[3];
  GLfloat n[3], m;

  w0[0] = v1[0] - v0[0];
  w0[1] = v1[1] - v0[1];
  w0[2] = v1[2] - v0[2];

  w1[0] = v2[0] - v1[0];
  w1[1] = v2[1] - v1[1];
  w1[2] = v2[2] - v1[2];

  n[0] = w0[1]*w1[2] - w0[2]*w1[1];
  n[1] = w0[2]*w1[0] - w0[0]*w1[2];
  n[2] = w0[0]*w1[1] - w0[1]*w1[0];

  m = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];
  if (m > 0.0)
    {
      m = 1.0 / sqrt(m);
      n[0] *= m;
      n[1] *= m;
      n[2] *= m;
    }

  glBegin(GL_TRIANGLES);
    glNormal3fv(&n[0]);
    glVertex3fv(&v0[0]);
    glVertex3fv(&v1[0]);
    glVertex3fv(&v2[0]);
  glEnd();
}

/* AC3D triangle data */
#include "logo-g.h"

void
logo_draw_g (void)
{
  GLint i;

  for (i = 0; i < LOGO_G_V_SIZE; i++)
    logo_draw_triangle(&logo_g_v[i][2][0],
                       &logo_g_v[i][1][0],
                       &logo_g_v[i][0][0]);
}

/* AC3D triangle data */
#include "logo-t.h"

void
logo_draw_t (void)
{
  GLint i;

  for (i = 0; i < LOGO_T_V_SIZE; i++)
    logo_draw_triangle(&logo_t_v[i][2][0],
                       &logo_t_v[i][1][0],
                       &logo_t_v[i][0][0]);
}

/* AC3D triangle data */
#include "logo-k.h"

void
logo_draw_k (void)
{
  GLint i;

  for (i = 0; i < LOGO_K_V_SIZE; i++)
    logo_draw_triangle(&logo_k_v[i][2][0],
                       &logo_k_v[i][1][0],
                       &logo_k_v[i][0][0]);
}
