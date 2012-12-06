/* GdkGLExt - OpenGL Extension to GDK
 * Copyright (C) 2012 Thomas Zimmermann
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

#include <gdk/gdkgldebug.h>
#include <gdk/win32/gdkglwin32.h>

#include "gdkglwindow-win32.h"

struct _GdkWin32GLWindow
{
  GdkGLWindow parent;
};

struct _GdkWin32GLWindowClass
{
  GdkGLWindowClass parent_class;
};

G_DEFINE_TYPE (GdkWin32GLWindow,
               gdk_win32_gl_window,
               GDK_TYPE_GL_WINDOW);

static void
gdk_win32_gl_window_init (GdkWin32GLWindow *self)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();
}

static void
gdk_win32_gl_window_finalize (GObject *object)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();

  G_OBJECT_CLASS (gdk_win32_gl_window_parent_class)->finalize (object);
}

static void
gdk_win32_gl_window_class_init (GdkWin32GLWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  object_class->finalize = gdk_win32_gl_window_finalize;
}

PIXELFORMATDESCRIPTOR *
gdk_win32_gl_window_get_pfd (GdkGLWindow *glwindow)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), NULL);

  return GDK_GL_WINDOW_IMPL_WIN32_GET_CLASS (glwindow->impl)->get_pfd(glwindow);
}

int
gdk_win32_gl_window_get_pixel_format (GdkGLWindow *glwindow)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_WINDOW (glwindow), 0);

  return GDK_GL_WINDOW_IMPL_WIN32_GET_CLASS (glwindow->impl)->get_pixel_format(glwindow);
}

HDC
gdk_win32_gl_window_get_hdc (GdkGLWindow *glwindow)
{
  g_return_val_if_fail(GDK_IS_WIN32_GL_WINDOW (glwindow), NULL);

  return GDK_GL_WINDOW_IMPL_WIN32_GET_CLASS (glwindow->impl)->get_hdc(glwindow);
}

void
gdk_win32_gl_window_release_hdc (GdkGLWindow *glwindow)
{
  g_return_if_fail(GDK_IS_WIN32_GL_WINDOW (glwindow));

  GDK_GL_WINDOW_IMPL_WIN32_GET_CLASS (glwindow->impl)->release_hdc(glwindow);
}
