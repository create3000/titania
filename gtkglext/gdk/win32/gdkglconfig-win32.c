/* GdkGLExt - OpenGL Extension to GDK
 * Copyright (C) 2002-2004  Naofumi Yasufuku
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <gdk/gdk.h>

#include "gdkglwin32.h"
#include "gdkglprivate-win32.h"
#include "gdkglconfig-win32.h"
#include "gdkglwindow-win32.h"

static GdkGLWindow            *_gdk_win32_gl_config_impl_create_gl_window (GdkGLConfig *glconfig,
                                                                           GdkWindow   *window,
                                                                           const int   *attrib_list);
static GdkScreen              *_gdk_win32_gl_config_impl_get_screen       (GdkGLConfig *glconfig);
static gboolean                _gdk_win32_gl_config_impl_get_attrib       (GdkGLConfig *glconfig,
                                                                           int          attribute,
                                                                           int         *value);
static GdkVisual              *_gdk_win32_gl_config_impl_get_visual       (GdkGLConfig *glconfig);
static gint                    _gdk_win32_gl_config_impl_get_depth        (GdkGLConfig *glconfig);
static PIXELFORMATDESCRIPTOR  *_gdk_win32_gl_config_impl_get_pfd          (GdkGLConfig *glconfig);

G_DEFINE_TYPE (GdkGLConfigImplWin32,            \
               gdk_gl_config_impl_win32,        \
               GDK_TYPE_GL_CONFIG_IMPL)

static void
gdk_gl_config_impl_win32_init (GdkGLConfigImplWin32 *self)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();

  memset (&self->pfd, 0, sizeof(self->pfd));
  GDK_GL_NOTE_FUNC_PRIVATE ();
  self->screen = NULL;
  GDK_GL_NOTE_FUNC_PRIVATE ();
  self->depth = 0;
}

static void
gdk_gl_config_impl_win32_finalize (GObject *object)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();

  G_OBJECT_CLASS (gdk_gl_config_impl_win32_parent_class)->finalize (object);
}

static void
gdk_gl_config_impl_win32_class_init (GdkGLConfigImplWin32Class *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  klass->get_pfd = _gdk_win32_gl_config_impl_get_pfd;

  klass->parent_class.create_gl_window = _gdk_win32_gl_config_impl_create_gl_window;
  klass->parent_class.get_screen       = _gdk_win32_gl_config_impl_get_screen;
  klass->parent_class.get_attrib       = _gdk_win32_gl_config_impl_get_attrib;
  klass->parent_class.get_visual       = _gdk_win32_gl_config_impl_get_visual;
  klass->parent_class.get_depth        = _gdk_win32_gl_config_impl_get_depth;

  object_class->finalize = gdk_gl_config_impl_win32_finalize;
}

/*
 * This code is based on lib/glut/win32_glx.c of GLUT by Nate Robins.
 */
static void
gdk_gl_config_parse_attrib_list (const int             *attrib_list,
                                 gsize                  n_attribs,
                                 PIXELFORMATDESCRIPTOR *pfd)
{
  int *p;
  int i;
  gboolean buffer_size_is_specified = FALSE;
  BYTE buffer_size;
  int layer_plane;

  GDK_GL_NOTE_FUNC_PRIVATE ();

  memset (pfd, 0, sizeof (PIXELFORMATDESCRIPTOR));

  /* Specifies the size of this data structure. */
  pfd->nSize = sizeof (PIXELFORMATDESCRIPTOR);
  /* Specifies the version of this data structure. This value should be set to 1. */
  pfd->nVersion = 1;

  /* Defaults. */

  /* A set of bit flags that specify properties of the pixel buffer.
     PFD_SUPPORT_GDI and PFD_DOUBLEBUFFER are mutually exclusive in
     the current generic implementation. */
  /* PFD_DRAW_TO_WINDOW or PFD_DRAW_TO_BITMAP is not specified at this stage.
     The flag is set by gdk_gl_window_new(). */
  pfd->dwFlags = PFD_SUPPORT_OPENGL |
                 PFD_SUPPORT_GDI;

  /* Specifies the type of pixel data. */
  pfd->iPixelType = PFD_TYPE_COLORINDEX;

  /* Specifies the number of color bitplanes in each color buffer.
     For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes.
     For color-index pixels, it is the size of the color-index buffer. */
  pfd->cColorBits = 32;		/* Max */

  /* Ignored. Earlier implementations of OpenGL used this member,
     but it is no longer used. */
  layer_plane = 0;
  pfd->iLayerType = PFD_MAIN_PLANE;

  p = (int *) attrib_list;

  for (i = 0; (i < n_attribs) && (*p != GDK_GL_ATTRIB_LIST_NONE); ++i)
    {
      switch (*p)
        {
          case GDK_GL_USE_GL:
	          /* The buffer supports OpenGL drawing. */
            pfd->dwFlags |= PFD_SUPPORT_OPENGL;
            break;
          case GDK_GL_BUFFER_SIZE:
	          /* Specifies the number of color bitplanes in each color buffer. */
            pfd->cColorBits = *(++p);
	          buffer_size_is_specified = TRUE;
            ++i;
            break;
          case GDK_GL_LEVEL:
            layer_plane = *(++p);
	          /* Ignored. Earlier implementations of OpenGL used this member,
        	     but it is no longer used. */
        	  if (layer_plane > 0)
        	    pfd->iLayerType = PFD_OVERLAY_PLANE;
        	  else if (layer_plane < 0)
        	    pfd->iLayerType = PFD_UNDERLAY_PLANE;
            ++i;
            break;
          case GDK_GL_RGBA:
        	  /* RGBA pixels. */
            pfd->iPixelType = PFD_TYPE_RGBA;
            break;
          case GDK_GL_DOUBLEBUFFER:
	          /* The buffer is double-buffered. */
        	  pfd->dwFlags &= ~PFD_SUPPORT_GDI;
            pfd->dwFlags |= PFD_DOUBLEBUFFER;
            break;
          case GDK_GL_STEREO:
	          /* The buffer is stereoscopic.
        	     This flag is not supported in the current generic implementation. */
            pfd->dwFlags |= PFD_STEREO;
            break;
          case GDK_GL_AUX_BUFFERS:
	          /* Specifies the number of auxiliary buffers.
        	     Auxiliary buffers are not supported. */
            pfd->cAuxBuffers = *(++p);
            ++i;
            break;
          case GDK_GL_RED_SIZE:
        	  /* Specifies the number of red bitplanes in each RGBA color buffer.
        	     Not used by ChoosePixelFormat. */
            pfd->cRedBits = *(++p);
            ++i;
            break;
          case GDK_GL_GREEN_SIZE:
        	  /* Specifies the number of green bitplanes in each RGBA color buffer.
        	     Not used by ChoosePixelFormat. */
            pfd->cGreenBits = *(++p);
            ++i;
            break;
          case GDK_GL_BLUE_SIZE:
        	  /* Specifies the number of blue bitplanes in each RGBA color buffer.
        	     Not used by ChoosePixelFormat. */
            pfd->cBlueBits = *(++p);
            ++i;
            break;
          case GDK_GL_ALPHA_SIZE:
	          /* Specifies the number of alpha bitplanes in each RGBA color buffer.
        	     Alpha bitplanes are not supported.  */
        	  pfd->cAlphaBits = *(++p);
            ++i;
            break;
          case GDK_GL_DEPTH_SIZE:
	          /* Specifies the depth of the depth (z-axis) buffer. */
            pfd->cDepthBits = *(++p);
            ++i;
            break;
          case GDK_GL_STENCIL_SIZE:
        	  /* Specifies the depth of the stencil buffer. */
            pfd->cStencilBits = *(++p);
            ++i;
            break;
          case GDK_GL_ACCUM_RED_SIZE:
        	  /* Specifies the number of red bitplanes in the accumulation buffer.
        	     Not used by ChoosePixelFormat. */
        	  pfd->cAccumRedBits = *(++p);
            ++i;
        	  break;
          case GDK_GL_ACCUM_GREEN_SIZE:
        	  /* Specifies the number of green bitplanes in the accumulation buffer.
        	     Not used by ChoosePixelFormat. */
        	  pfd->cAccumGreenBits = *(++p);
            ++i;
        	  break;
          case GDK_GL_ACCUM_BLUE_SIZE:
	          /* Specifies the number of blue bitplanes in the accumulation buffer.
        	     Not used by ChoosePixelFormat. */
        	  pfd->cAccumBlueBits = *(++p);
            ++i;
        	  break;
          case GDK_GL_ACCUM_ALPHA_SIZE:
        	  /* Specifies the number of alpha bitplanes in the accumulation buffer.
        	     Not used by ChoosePixelFormat.*/
        	  pfd->cAccumAlphaBits = *(++p);
            ++i;
            break;
        }
      ++p;
    }

  /* If GDK_GL_BUFFER_SIZE is not specified. */
  if (!buffer_size_is_specified)
    {
      buffer_size = pfd->cRedBits + pfd->cGreenBits + pfd->cBlueBits;
      if (buffer_size != 0)
      	pfd->cColorBits = buffer_size;
    }

  /* Specifies the total number of bitplanes in the accumulation buffer. */
  /* Nate Robins says ...
     I believe that WGL only used the cAccumRedBits,
     cAccumBlueBits, cAccumGreenBits, and cAccumAlphaBits fields
     when returning info about the accumulation buffer precision.
     Only cAccumBits is used for requesting an accumulation buffer. */
  pfd->cAccumBits = pfd->cAccumRedBits +
                    pfd->cAccumGreenBits +
                    pfd->cAccumBlueBits +
                    pfd->cAccumAlphaBits;
}

/*
 * Find an appropriate pixel format.
 * Basic idea of this code is ripped from FLTK.
 */
/*< private >*/
int
_gdk_win32_gl_config_impl_find_pixel_format (HDC                          hdc,
                                             CONST PIXELFORMATDESCRIPTOR *req_pfd,
                                             PIXELFORMATDESCRIPTOR       *found_pfd)
{
  PIXELFORMATDESCRIPTOR pfd, chosen_pfd;
  int pixel_format = 0;
  int i;

  GDK_GL_NOTE_FUNC_PRIVATE ();

  memset (&chosen_pfd, 0, sizeof (chosen_pfd));

  for (i = 1; ; i++)
    {
      if (DescribePixelFormat (hdc, i, sizeof (pfd), &pfd) == 0)
	break;

      if (~(pfd.dwFlags)   &  req_pfd->dwFlags)      continue;
      if (pfd.iPixelType   != req_pfd->iPixelType)   continue;
      if (pfd.cColorBits   <  req_pfd->cColorBits)   continue;
      if (pfd.cAlphaBits   <  req_pfd->cAlphaBits)   continue;
      if (pfd.cAccumBits   <  req_pfd->cAccumBits)   continue;
      if (pfd.cDepthBits   <  req_pfd->cDepthBits)   continue;
      if (pfd.cStencilBits <  req_pfd->cStencilBits) continue;
      if (pfd.cAuxBuffers  <  req_pfd->cAuxBuffers)  continue;
      /* if (pfd.iLayerType   != req_pfd->iLayerType)   continue; */

      /* Check whether pfd is better than chosen_pfd. */
      if (pixel_format != 0)
	{
	  /* Offering overlay is better. */
	  if ((pfd.bReserved & 0x0f) && !(chosen_pfd.bReserved & 0x0f)) {}
	  /* More color bitplanes is better. */
	  else if (pfd.cColorBits > chosen_pfd.cColorBits) {}
	  /* pfd is not better than chosen_pfd. */
	  else continue;
	}

      pixel_format = i;
      chosen_pfd = pfd;
    }

  *found_pfd = chosen_pfd;

  return pixel_format;
}

/*
 * Setup PFD.
 */

static gboolean
gdk_win32_gl_config_impl_setup_pfd (CONST PIXELFORMATDESCRIPTOR *req_pfd,
                                    PIXELFORMATDESCRIPTOR       *pfd)
{
  HDC hdc;
  PIXELFORMATDESCRIPTOR temp_pfd;
  PIXELFORMATDESCRIPTOR w_pfd, b_pfd;
  int w_pf, b_pf;

  GDK_GL_NOTE_FUNC_PRIVATE ();

  /* Get DC. */
  hdc = GetDC (NULL);
  if (hdc == NULL)
    {
      g_warning ("cannot get DC");
      return FALSE;
    }

  w_pfd = *req_pfd;
  w_pfd.dwFlags &= ~PFD_DRAW_TO_BITMAP;
  w_pfd.dwFlags |= PFD_DRAW_TO_WINDOW;
  w_pf = _gdk_win32_gl_config_impl_find_pixel_format (hdc, &w_pfd, &w_pfd);

  GDK_GL_NOTE (MISC, g_message (" -- pixel format for windows = 0x%x", w_pf));
  GDK_GL_NOTE (MISC, _gdk_win32_gl_print_pfd (&w_pfd));

  b_pfd = *req_pfd;
  b_pfd.dwFlags &= ~PFD_DRAW_TO_WINDOW;
  b_pfd.dwFlags |= PFD_DRAW_TO_BITMAP;
  b_pf = _gdk_win32_gl_config_impl_find_pixel_format (hdc, &b_pfd, &b_pfd);

  GDK_GL_NOTE (MISC, g_message (" -- pixel format for bitmaps = 0x%x", b_pf));
  GDK_GL_NOTE (MISC, _gdk_win32_gl_print_pfd (&b_pfd));

  /* Release DC. */
  ReleaseDC (NULL, hdc);

  if (w_pf == 0 && b_pf == 0)
    return FALSE;

  if (w_pf == 0)
    {
      temp_pfd = b_pfd;
      temp_pfd.dwFlags = req_pfd->dwFlags;
    }
  else if (b_pf == 0)
    {
      temp_pfd = w_pfd;
      temp_pfd.dwFlags = req_pfd->dwFlags;
    }
  else
    {
      temp_pfd = w_pfd;
      temp_pfd.dwFlags = req_pfd->dwFlags;
      if (temp_pfd.cColorBits > b_pfd.cColorBits)
        {
          temp_pfd.cColorBits  = b_pfd.cColorBits;
          temp_pfd.cRedBits    = b_pfd.cRedBits;
          temp_pfd.cRedShift   = b_pfd.cRedShift;
          temp_pfd.cGreenBits  = b_pfd.cGreenBits;
          temp_pfd.cGreenShift = b_pfd.cGreenShift;
          temp_pfd.cBlueBits   = b_pfd.cBlueBits;
          temp_pfd.cBlueShift  = b_pfd.cBlueShift;
        }
      if (temp_pfd.cAlphaBits > b_pfd.cAlphaBits)
        {
          temp_pfd.cAlphaBits  = b_pfd.cAlphaBits;
          temp_pfd.cAlphaShift = b_pfd.cAlphaShift;
        }
      if (temp_pfd.cAccumBits > b_pfd.cAccumBits)
        {
          temp_pfd.cAccumBits      = b_pfd.cAccumBits;
          temp_pfd.cAccumRedBits   = b_pfd.cAccumRedBits;
          temp_pfd.cAccumGreenBits = b_pfd.cAccumGreenBits;
          temp_pfd.cAccumBlueBits  = b_pfd.cAccumBlueBits;
          temp_pfd.cAccumAlphaBits = b_pfd.cAccumAlphaBits;
        }
      temp_pfd.cDepthBits   = MIN (temp_pfd.cDepthBits,   b_pfd.cDepthBits);
      temp_pfd.cStencilBits = MIN (temp_pfd.cStencilBits, b_pfd.cStencilBits);
      temp_pfd.cAuxBuffers  = MIN (temp_pfd.cAuxBuffers,  b_pfd.cAuxBuffers);
    }

  *pfd = temp_pfd;

  return TRUE;
}

static void
gdk_win32_gl_config_impl_init_attrib (GdkGLConfig *glconfig)
{
  PIXELFORMATDESCRIPTOR *pfd;

  pfd = GDK_GL_CONFIG_PFD (glconfig);

  /* RGBA mode? */
  glconfig->impl->is_rgba = (pfd->iPixelType == PFD_TYPE_RGBA) ? TRUE : FALSE;

  /* Layer plane. */
  if (pfd->bReserved != 0)
    {
      glconfig->impl->layer_plane = pfd->bReserved & 0x0f;
      if (glconfig->impl->layer_plane == 0)
        glconfig->impl->layer_plane = -1 * ((pfd->bReserved & 0xf0) >> 4);
    }
  else
    {
      glconfig->impl->layer_plane = 0;
    }

  /* Double buffering is supported? */
  glconfig->impl->is_double_buffered = (pfd->dwFlags & PFD_DOUBLEBUFFER) ? TRUE : FALSE;

  /* Stereo is supported? (not work on Windows) */
  glconfig->impl->is_stereo = (pfd->dwFlags & PFD_STEREO) ? TRUE : FALSE;

  /* Number of aux buffers */
  glconfig->impl->n_aux_buffers = pfd->cAuxBuffers;

  /* Has alpha bits? */
  glconfig->impl->has_alpha = pfd->cAlphaBits ? TRUE : FALSE;

  /* Has depth buffer? */
  glconfig->impl->has_depth_buffer = pfd->cDepthBits ? TRUE : FALSE;

  /* Has stencil buffer? */
  glconfig->impl->has_stencil_buffer = pfd->cStencilBits ? TRUE : FALSE;

  /* Has accumulation buffer? */
  glconfig->impl->has_accum_buffer = pfd->cAccumBits ? TRUE : FALSE;

  /* Number of multisample buffers (not supported yet) */
  glconfig->impl->n_sample_buffers = 0;
}

static GdkGLConfig *
gdk_win32_gl_config_impl_new_common (GdkGLConfig *glconfig,
                                     GdkScreen   *screen,
                                     const int   *attrib_list,
                                     gsize        n_attribs)
{
  GdkGLConfigImplWin32 *win32_impl;
  PIXELFORMATDESCRIPTOR pfd;

  GDK_GL_NOTE_FUNC_PRIVATE ();

  /*
   * Parse GLX style attrib_list.
   */

  gdk_gl_config_parse_attrib_list (attrib_list, n_attribs, &pfd);

  GDK_GL_NOTE (MISC, _gdk_win32_gl_print_pfd (&pfd));

  /*
   * Setup PFD.
   */

  if (!gdk_win32_gl_config_impl_setup_pfd (&pfd, &pfd))
    return NULL;

  GDK_GL_NOTE (MISC, g_message (" -- created PFD"));
  GDK_GL_NOTE (MISC, _gdk_win32_gl_print_pfd (&pfd));

  /*
   * Instantiate the GdkGLConfigImplWin32 object.
   */

  win32_impl = g_object_new (GDK_TYPE_GL_CONFIG_IMPL_WIN32, NULL);

  win32_impl->pfd = pfd;

  win32_impl->screen = screen;

  /*
   * Set depth (number of bits per pixel).
   */

  win32_impl->depth = pfd.cRedBits + pfd.cGreenBits + pfd.cBlueBits;

  /*
   * Setup GdkGLConfig
   */

  glconfig->impl = GDK_GL_CONFIG_IMPL (win32_impl);

  /*
   * Init configuration attributes.
   */

  gdk_win32_gl_config_impl_init_attrib (glconfig);

  return glconfig;
}

GdkGLConfig *
_gdk_win32_gl_config_impl_new (GdkGLConfig *glconfig,
                               const int   *attrib_list,
                               gsize        n_attribs)
{
  GdkScreen *screen;

  GDK_GL_NOTE_FUNC ();

  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);
  g_return_val_if_fail (attrib_list != NULL, NULL);

  screen = gdk_screen_get_default ();

  return gdk_win32_gl_config_impl_new_common (glconfig, screen, attrib_list, n_attribs);
}

GdkGLConfig *
_gdk_win32_gl_config_impl_new_for_screen (GdkGLConfig *glconfig,
                                          GdkScreen   *screen,
                                          const int   *attrib_list,
                                          gsize        n_attribs)
{
  GDK_GL_NOTE_FUNC ();

  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);
  g_return_val_if_fail (attrib_list != NULL, NULL);

  return gdk_win32_gl_config_impl_new_common (glconfig, screen, attrib_list, n_attribs);
}

GdkGLConfig *
_gdk_win32_gl_config_impl_new_from_pixel_format (GdkGLConfig *glconfig,
                                                 int pixel_format)
{
  GdkGLConfigImplWin32 *win32_impl;

  HDC hdc;
  PIXELFORMATDESCRIPTOR pfd;
  int result;

  GDK_GL_NOTE_FUNC ();

  /*
   * Get PFD.
   */

  /* Get DC. */
  hdc = GetDC (NULL);
  if (hdc == NULL)
    {
      g_warning ("cannot get DC");
      return NULL;
    }

  result = DescribePixelFormat (hdc, pixel_format, sizeof (pfd), &pfd);

  /* Release DC. */
  ReleaseDC (NULL, hdc);

  if (result == 0)
    return NULL;

  GDK_GL_NOTE (MISC, g_message (" -- pixel_format = 0x%x", pixel_format));

  GDK_GL_NOTE (MISC, _gdk_win32_gl_print_pfd (&pfd));

  /*
   * Instantiate the GdkGLConfigImplWin32 object.
   */

  win32_impl = g_object_new (GDK_TYPE_GL_CONFIG_IMPL_WIN32, NULL);

  win32_impl->pfd = pfd;
  win32_impl->screen = gdk_screen_get_default ();

  /*
   * Set depth (number of bits per pixel).
   */

  win32_impl->depth = pfd.cRedBits + pfd.cGreenBits + pfd.cBlueBits;

  /*
   * Setup GdkGLConfig
   */

  glconfig->impl = GDK_GL_CONFIG_IMPL (win32_impl);

  /*
   * Init configuration attributes.
   */

  gdk_win32_gl_config_impl_init_attrib (glconfig);

  return glconfig;
}

static GdkGLWindow *
_gdk_win32_gl_config_impl_create_gl_window (GdkGLConfig *glconfig,
                                            GdkWindow   *window,
                                            const int   *attrib_list)
{
  GdkGLWindow *glwindow;
  GdkGLWindow *impl;

  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);

  glwindow = g_object_new (GDK_TYPE_WIN32_GL_WINDOW, NULL);

  g_return_val_if_fail(glwindow != NULL, NULL);

  impl = _gdk_win32_gl_window_impl_new(glwindow,
                                       glconfig,
                                       window,
                                       attrib_list);
  if (impl == NULL)
    g_object_unref(glwindow);

  g_return_val_if_fail(impl != NULL, NULL);

  return glwindow;
}

static GdkScreen *
_gdk_win32_gl_config_impl_get_screen (GdkGLConfig *glconfig)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);

  return GDK_GL_CONFIG_IMPL_WIN32 (glconfig->impl)->screen;
}

/*
 * This code is based on lib/glut/win32_glx.c of GLUT by Nate Robins.
 */
static gboolean
_gdk_win32_gl_config_impl_get_attrib (GdkGLConfig *glconfig,
                                      int          attribute,
                                      int         *value)
{
  GdkGLConfigImplWin32 *win32_impl;

  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), FALSE);

  win32_impl = GDK_GL_CONFIG_IMPL_WIN32 (glconfig->impl);

  switch (attribute)
    {
    case GDK_GL_USE_GL:
      if (win32_impl->pfd.dwFlags & PFD_SUPPORT_OPENGL)
        {
      	  *value = 1;

          /* Mark J. Kilgard says ...
	           XXX Brad's Matrox Millenium II has problems creating
             color index windows in 24-bit mode (lead to GDI crash)
             and 32-bit mode (lead to black window).  The cColorBits
             filed of the PIXELFORMATDESCRIPTOR returned claims to
             have 24 and 32 bits respectively of color indices. 2^24
             and 2^32 are ridiculously huge writable colormaps.
             Assume that if we get back a color index
             PIXELFORMATDESCRIPTOR with 24 or more bits, the
             PIXELFORMATDESCRIPTOR doesn't really work and skip it.
             -mjk */
#if 0
          if (impl->pfd.iPixelType == PFD_TYPE_COLORINDEX &&
	          win32_impl->pfd.cColorBits >= 24)
            *value = 0;
          else
            *value = 1;
#endif
        }
      else
        {
          *value = 0;
        }
      break;
    case GDK_GL_BUFFER_SIZE:
      /* Nate Robins says ...
	 KLUDGE: if we're RGBA, return the number of bits/pixel,
         otherwise, return 8 (we guessed at 256 colors in CI mode). */
      if (win32_impl->pfd.iPixelType == PFD_TYPE_RGBA)
        *value = win32_impl->pfd.cColorBits;
      else
        *value = 8;
      break;
    case GDK_GL_LEVEL:
      *value = glconfig->impl->layer_plane;
      break;
    case GDK_GL_RGBA:
      *value = win32_impl->pfd.iPixelType == PFD_TYPE_RGBA;
      break;
    case GDK_GL_DOUBLEBUFFER:
      *value = win32_impl->pfd.dwFlags & PFD_DOUBLEBUFFER;
      break;
    case GDK_GL_STEREO:
      *value = win32_impl->pfd.dwFlags & PFD_STEREO;
      break;
    case GDK_GL_AUX_BUFFERS:
      *value = win32_impl->pfd.cAuxBuffers;
      break;
    case GDK_GL_RED_SIZE:
      *value = win32_impl->pfd.cRedBits;
      break;
    case GDK_GL_GREEN_SIZE:
      *value = win32_impl->pfd.cGreenBits;
      break;
    case GDK_GL_BLUE_SIZE:
      *value = win32_impl->pfd.cBlueBits;
      break;
    case GDK_GL_ALPHA_SIZE:
      *value = win32_impl->pfd.cAlphaBits;
      break;
    case GDK_GL_DEPTH_SIZE:
      *value = win32_impl->pfd.cDepthBits;
      break;
    case GDK_GL_STENCIL_SIZE:
      *value = win32_impl->pfd.cStencilBits;
      break;
    case GDK_GL_ACCUM_RED_SIZE:
      *value = win32_impl->pfd.cAccumRedBits;
      break;
    case GDK_GL_ACCUM_GREEN_SIZE:
      *value = win32_impl->pfd.cAccumGreenBits;
      break;
    case GDK_GL_ACCUM_BLUE_SIZE:
      *value = win32_impl->pfd.cAccumBlueBits;
      break;
    case GDK_GL_ACCUM_ALPHA_SIZE:
      *value = win32_impl->pfd.cAccumAlphaBits;
      break;
    default:
      return FALSE;
    }

  return TRUE;
}

static GdkVisual *
_gdk_win32_gl_config_impl_get_visual (GdkGLConfig *glconfig)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);

  /* There is currently no function for retreiving a visual or
   * PIXELFORMATDESCRIPTOR, or so. We just return the system visual and
   * hope for the best.
   */
  return gdk_screen_get_system_visual (GDK_GL_CONFIG_IMPL_WIN32 (glconfig->impl)->screen);
}

static gint
_gdk_win32_gl_config_impl_get_depth (GdkGLConfig *glconfig)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), 0);

  return GDK_GL_CONFIG_IMPL_WIN32 (glconfig->impl)->depth;
}

static PIXELFORMATDESCRIPTOR *
_gdk_win32_gl_config_impl_get_pfd (GdkGLConfig *glconfig)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);

  return &(GDK_GL_CONFIG_IMPL_WIN32 (glconfig->impl)->pfd);
}

/*< private >*/
void
_gdk_win32_gl_print_pfd (PIXELFORMATDESCRIPTOR *pfd)
{
  g_message (" -- pfd->dwFlags & PFD_DRAW_TO_WINDOW      = %s",
             (pfd->dwFlags & PFD_DRAW_TO_WINDOW)      ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_DRAW_TO_BITMAP      = %s",
             (pfd->dwFlags & PFD_DRAW_TO_BITMAP)      ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_SUPPORT_GDI         = %s",
             (pfd->dwFlags & PFD_SUPPORT_GDI)         ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_SUPPORT_OPENGL      = %s",
             (pfd->dwFlags & PFD_SUPPORT_OPENGL)      ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_GENERIC_ACCELERATED = %s",
             (pfd->dwFlags & PFD_GENERIC_ACCELERATED) ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_GENERIC_FORMAT      = %s",
             (pfd->dwFlags & PFD_GENERIC_FORMAT)      ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_NEED_PALETTE        = %s",
             (pfd->dwFlags & PFD_NEED_PALETTE)        ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_NEED_SYSTEM_PALETTE = %s",
             (pfd->dwFlags & PFD_NEED_SYSTEM_PALETTE) ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_DOUBLEBUFFER        = %s",
             (pfd->dwFlags & PFD_DOUBLEBUFFER)        ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_STEREO              = %s",
             (pfd->dwFlags & PFD_STEREO)              ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_SWAP_LAYER_BUFFERS  = %s",
             (pfd->dwFlags & PFD_SWAP_LAYER_BUFFERS)  ? "TRUE" : "FALSE");

  g_message (" -- pfd->dwFlags & PFD_DEPTH_DONTCARE        = %s",
             (pfd->dwFlags & PFD_DEPTH_DONTCARE)        ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_DOUBLEBUFFER_DONTCARE = %s",
             (pfd->dwFlags & PFD_DOUBLEBUFFER_DONTCARE) ? "TRUE" : "FALSE");
  g_message (" -- pfd->dwFlags & PFD_STEREO_DONTCARE       = %s",
             (pfd->dwFlags & PFD_STEREO_DONTCARE)       ? "TRUE" : "FALSE");

  g_message (" -- pfd->dwFlags & PFD_SWAP_COPY     = %s",
             (pfd->dwFlags & PFD_SWAP_COPY)     ? "TRUE" : "FALSE");
  g_message (" -- pfd_win32->dwFlags & PFD_SWAP_EXCHANGE = %s",
             (pfd->dwFlags & PFD_SWAP_EXCHANGE) ? "TRUE" : "FALSE");

  g_message (" -- pfd->iPixelType = %d (%s)",
             pfd->iPixelType,
             (pfd->iPixelType == PFD_TYPE_RGBA) ? "PFD_TYPE_RGBA" : "PFD_TYPE_COLORINDEX");

  g_message (" -- pfd->cColorBits      = %d", pfd->cColorBits);
  g_message (" -- pfd->cRedBits        = %d", pfd->cRedBits);
  g_message (" -- pfd->cRedShift       = %d", pfd->cRedShift);
  g_message (" -- pfd->cGreenBits      = %d", pfd->cGreenBits);
  g_message (" -- pfd->cGreenShift     = %d", pfd->cGreenShift);
  g_message (" -- pfd->cBlueBits       = %d", pfd->cBlueBits);
  g_message (" -- pfd->cBlueShift      = %d", pfd->cBlueShift);
  g_message (" -- pfd->cAlphaBits      = %d", pfd->cAlphaBits);
  g_message (" -- pfd->cAlphaShift     = %d", pfd->cAlphaShift);
  g_message (" -- pfd->cAccumBits      = %d", pfd->cAccumBits);
  g_message (" -- pfd->cAccumRedBits   = %d", pfd->cAccumRedBits);
  g_message (" -- pfd->cAccumGreenBits = %d", pfd->cAccumGreenBits);
  g_message (" -- pfd->cAccumBlueBits  = %d", pfd->cAccumBlueBits);
  g_message (" -- pfd->cAccumAlphaBits = %d", pfd->cAccumAlphaBits);
  g_message (" -- pfd->cDepthBits      = %d", pfd->cDepthBits);
  g_message (" -- pfd->cStencilBits    = %d", pfd->cStencilBits);
  g_message (" -- pfd->cAuxBuffers     = %d", pfd->cAuxBuffers);

  /* Ignored */
  g_message (" -- pfd->iLayerType = %d", pfd->iLayerType);

  g_message (" -- pfd->bReserved & 0x0f        = %d", pfd->bReserved & 0x0f);
  g_message (" -- (pfd->bReserved & 0xf0) >> 4 = %d", (pfd->bReserved & 0xf0) >> 4);

  /* Ignored */
  g_message (" -- pfd->dwLayerMask = 0x%lx", pfd->dwLayerMask);

  g_message (" -- pfd->dwVisibleMask = 0x%lx", pfd->dwVisibleMask);

  g_message (" -- pfd->dwDamageMask = 0x%lx", pfd->dwDamageMask);
}
