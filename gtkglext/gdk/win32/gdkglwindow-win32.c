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

#include "gdkglwin32.h"
#include "gdkglprivate-win32.h"
#include "gdkglconfig-win32.h"
#include "gdkglcontext-win32.h"
#include "gdkglwindow-win32.h"

static GdkGLContext           *_gdk_win32_gl_window_impl_create_gl_context  (GdkGLWindow  *glwindow,
                                                                             GdkGLContext *share_list,
                                                                             gboolean      direct,
                                                                             int           render_type);
static gboolean                _gdk_win32_gl_window_impl_is_double_buffered (GdkGLWindow  *glwindow);
static void                    _gdk_win32_gl_window_impl_swap_buffers       (GdkGLWindow  *glwindow);
static void                    _gdk_win32_gl_window_impl_wait_gl            (GdkGLWindow  *glwindow);
static void                    _gdk_win32_gl_window_impl_wait_gdk           (GdkGLWindow  *glwindow);
static GdkGLConfig            *_gdk_win32_gl_window_impl_get_gl_config      (GdkGLWindow  *glwindow);
static PIXELFORMATDESCRIPTOR  *_gdk_win32_gl_window_impl_get_pfd            (GdkGLWindow  *glwindow);
static int                     _gdk_win32_gl_window_impl_get_pixel_format   (GdkGLWindow  *glwindow);
static HDC                     _gdk_win32_gl_window_impl_get_hdc            (GdkGLWindow  *glwindow);
static void                    _gdk_win32_gl_window_impl_release_hdc        (GdkGLWindow  *glwindow);

G_DEFINE_TYPE (GdkGLWindowImplWin32,
               gdk_gl_window_impl_win32,
               GDK_TYPE_GL_WINDOW_IMPL)

static void
gdk_gl_window_impl_win32_init (GdkGLWindowImplWin32 *self)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();

  self->hwnd = NULL;
  memset (&self->pfd, 0, sizeof(self->pfd));
  self->pixel_format = 0;
  self->glconfig = NULL;
  self->hdc = NULL;
  self->is_destroyed = 0;
  self->need_release_dc = 0;
}

static void
_gdk_win32_gl_window_impl_destroy (GdkGLWindow *glwindow)
{
  GdkGLWindowImplWin32 *impl = GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  if (impl->is_destroyed)
    return;

  /* Get DC. */
  if (impl->hdc == NULL)
    {
      impl->hdc = GetDC (impl->hwnd);
      if (impl->hdc == NULL)
        return;
    }

  if (impl->hdc == wglGetCurrentDC ())
    {
      glFinish ();

      GDK_GL_NOTE_FUNC_IMPL ("wglMakeCurrent");
      wglMakeCurrent (NULL, NULL);
    }

  /* Release DC. */
  if (impl->need_release_dc)
    ReleaseDC (impl->hwnd, impl->hdc);
  impl->hdc = NULL;

  impl->hwnd = NULL;

  impl->is_destroyed = TRUE;
}

static void
gdk_gl_window_impl_win32_finalize (GObject *object)
{
  GdkGLWindowImplWin32 *impl = GDK_GL_WINDOW_IMPL_WIN32 (object);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  _gdk_win32_gl_window_impl_destroy (GDK_GL_WINDOW (object));

  g_object_unref (G_OBJECT (impl->glconfig));

  G_OBJECT_CLASS (gdk_gl_window_impl_win32_parent_class)->finalize (object);
}

static void
gdk_gl_window_impl_win32_class_init (GdkGLWindowImplWin32Class *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  klass->get_pfd          = _gdk_win32_gl_window_impl_get_pfd;
  klass->get_pixel_format = _gdk_win32_gl_window_impl_get_pixel_format;
  klass->get_hdc          = _gdk_win32_gl_window_impl_get_hdc;
  klass->release_hdc      = _gdk_win32_gl_window_impl_release_hdc;

  klass->parent_class.create_gl_context      = _gdk_win32_gl_window_impl_create_gl_context;
  klass->parent_class.is_double_buffered     = _gdk_win32_gl_window_impl_is_double_buffered;
  klass->parent_class.swap_buffers           = _gdk_win32_gl_window_impl_swap_buffers;
  klass->parent_class.wait_gl                = _gdk_win32_gl_window_impl_wait_gl;
  klass->parent_class.wait_gdk               = _gdk_win32_gl_window_impl_wait_gdk;
  klass->parent_class.get_gl_config          = _gdk_win32_gl_window_impl_get_gl_config;
  klass->parent_class.destroy_gl_window_impl = _gdk_win32_gl_window_impl_destroy;

  object_class->finalize = gdk_gl_window_impl_win32_finalize;
}

/*
 * attrib_list is currently unused. This must be set to NULL or empty
 * (first attribute of None). See GLX 1.3 spec.
 */
GdkGLWindow *
_gdk_win32_gl_window_impl_new (GdkGLWindow *glwindow,
                               GdkGLConfig *glconfig,
                               GdkWindow   *window,
                               const int   *attrib_list)
{
  GdkGLWindowImplWin32 *win32_impl;

  HWND hwnd;
  DWORD wndclass_style;
  gboolean need_release_dc;
  HDC hdc = NULL;
  PIXELFORMATDESCRIPTOR pfd;
  int pixel_format;

  GDK_GL_NOTE_FUNC ();

  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), NULL);
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);
  g_return_val_if_fail (GDK_IS_WINDOW (window), NULL);

  hwnd = (HWND) gdk_win32_window_get_handle (window);

  /* Private DC? */
  wndclass_style = GetClassLong (hwnd, GCL_STYLE);
  if (wndclass_style & CS_OWNDC)
    {
      GDK_GL_NOTE (MISC, g_message (" -- Private DC"));
      need_release_dc = FALSE;
    }
  else
    {
      GDK_GL_NOTE (MISC, g_message (" -- Common DC"));
      need_release_dc = TRUE;
    }

  /* Get DC. */
  hdc = GetDC (hwnd);
  if (hdc == NULL)
    {
      g_warning ("cannot get DC");
      goto FAIL;
    }

  /*
   * Choose pixel format.
   */

  pfd = *(GDK_GL_CONFIG_PFD (glconfig));
  /* Draw to window */
  pfd.dwFlags &= ~PFD_DRAW_TO_BITMAP;
  pfd.dwFlags |= PFD_DRAW_TO_WINDOW;

  /* Request pfd.cColorBits should exclude alpha bitplanes. */
  pfd.cColorBits = pfd.cRedBits + pfd.cGreenBits + pfd.cBlueBits;

  GDK_GL_NOTE_FUNC_IMPL ("ChoosePixelFormat");

  pixel_format = ChoosePixelFormat (hdc, &pfd);
  if (pixel_format == 0)
    {
      g_warning ("cannot choose pixel format");
      goto FAIL;
    }

  /*
   * Set pixel format.
   */

  GDK_GL_NOTE_FUNC_IMPL ("SetPixelFormat");

  if (!SetPixelFormat (hdc, pixel_format, &pfd))
    {
      g_warning ("cannot set pixel format");
      goto FAIL;
    }

  DescribePixelFormat (hdc, pixel_format, sizeof (pfd), &pfd);

  GDK_GL_NOTE (MISC, g_message (" -- impl->pixel_format = 0x%x", pixel_format));
  GDK_GL_NOTE (MISC, _gdk_win32_gl_print_pfd (&pfd));

  if (need_release_dc)
    {
      /* Release DC. */
      ReleaseDC (hwnd, hdc);
      hdc = NULL;
    }

  /*
   * Instantiate the GdkGLWindowImplWin32 object.
   */

  win32_impl = g_object_new (GDK_TYPE_GL_WINDOW_IMPL_WIN32, NULL);

  win32_impl->hwnd = hwnd;

  win32_impl->pfd = pfd;
  win32_impl->pixel_format = pixel_format;

  win32_impl->glconfig = glconfig;
  g_object_ref (G_OBJECT (win32_impl->glconfig));

  win32_impl->hdc = hdc;
  win32_impl->need_release_dc = need_release_dc;

  win32_impl->is_destroyed = FALSE;

  glwindow->impl = GDK_GL_WINDOW_IMPL(win32_impl);
  glwindow->window = window;
  g_object_add_weak_pointer (G_OBJECT (glwindow->window),
                             (gpointer *) &(glwindow->window));

  return glwindow;

 FAIL:

  /* Release DC. */
  if (need_release_dc && hdc != NULL)
    ReleaseDC (hwnd, hdc);

  return NULL;
}

static GdkGLContext *
_gdk_win32_gl_window_impl_create_gl_context (GdkGLWindow  *glwindow,
                                             GdkGLContext *share_list,
                                             gboolean      direct,
                                             int           render_type)
{
  GdkGLContext *glcontext;
  GdkGLContextImpl *impl;

  glcontext = g_object_new(GDK_TYPE_WIN32_GL_CONTEXT, NULL);

  g_return_val_if_fail(glcontext != NULL, NULL);

  impl = _gdk_win32_gl_context_impl_new(glcontext,
                                        GDK_GL_DRAWABLE (glwindow),
                                        share_list,
                                        direct,
                                        render_type);
  if (impl == NULL)
    g_object_unref(glcontext);

  g_return_val_if_fail(impl != NULL, NULL);

  return glcontext;
}

static gboolean
_gdk_win32_gl_window_impl_is_double_buffered (GdkGLWindow *glwindow)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), FALSE);

  return gdk_gl_config_is_double_buffered (GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl)->glconfig);
}

static void
_gdk_win32_gl_window_impl_swap_buffers (GdkGLWindow *glwindow)
{
  GdkGLWindowImplWin32 *win32_impl;
  HDC hdc;

  g_return_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow));

  if (GDK_GL_WINDOW_IS_DESTROYED (glwindow))
    return;

  win32_impl = GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl);

  /* Get DC. */
  hdc = GDK_GL_WINDOW_IMPL_WIN32_HDC_GET (win32_impl);

  GDK_GL_NOTE_FUNC_IMPL ("SwapBuffers");

  SwapBuffers (hdc);

  /* Release DC. */
  GDK_GL_WINDOW_IMPL_WIN32_HDC_RELEASE (win32_impl);
}

static void
_gdk_win32_gl_window_impl_wait_gl (GdkGLWindow *glwindow)
{
  g_return_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow));

  glFinish ();

  /* Release DC. */
  GDK_GL_WINDOW_IMPL_WIN32_HDC_RELEASE (GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl));
}

static void
_gdk_win32_gl_window_impl_wait_gdk (GdkGLWindow *glwindow)
{
  g_return_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow));

  GdiFlush ();

  /* Get DC. */
  GDK_GL_WINDOW_IMPL_WIN32_HDC_GET (GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl));
}

static GdkGLConfig *
_gdk_win32_gl_window_impl_get_gl_config (GdkGLWindow *glwindow)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), NULL);

  return GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl)->glconfig;
}

PIXELFORMATDESCRIPTOR *
_gdk_win32_gl_window_impl_get_pfd (GdkGLWindow *glwindow)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), NULL);

  return &(GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl)->pfd);
}

int
_gdk_win32_gl_window_impl_get_pixel_format (GdkGLWindow *glwindow)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), 0);

  return GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl)->pixel_format;
}

HDC
_gdk_win32_gl_window_impl_get_hdc (GdkGLWindow *glwindow)
{
  g_return_val_if_fail(GDK_IS_WIN32_GL_WINDOW (glwindow), NULL);

  return GDK_GL_WINDOW_IMPL_WIN32_HDC_GET (GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl));
}

void
_gdk_win32_gl_window_impl_release_hdc (GdkGLWindow *glwindow)
{
  g_return_if_fail(GDK_IS_WIN32_GL_WINDOW (glwindow));

  GDK_GL_WINDOW_IMPL_WIN32_HDC_RELEASE (GDK_GL_WINDOW_IMPL_WIN32 (glwindow->impl));
}
