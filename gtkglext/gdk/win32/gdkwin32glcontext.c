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

#include "gdkglcontext-win32.h"

struct _GdkWin32GLContext
{
  GdkGLContext parent;
};

struct _GdkWin32GLContextClass
{
  GdkGLContextClass parent_class;
};

G_DEFINE_TYPE (GdkWin32GLContext, gdk_win32_gl_context, GDK_TYPE_GL_CONTEXT);

static void
gdk_win32_gl_context_init (GdkWin32GLContext *self)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();
}

static void
gdk_win32_gl_context_finalize (GObject *object)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();

  G_OBJECT_CLASS (gdk_win32_gl_context_parent_class)->finalize (object);
}

static void
gdk_win32_gl_context_class_init (GdkWin32GLContextClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  object_class->finalize = gdk_win32_gl_context_finalize;
}

GdkGLContext *
gdk_win32_gl_context_foreign_new (GdkGLConfig  *glconfig,
                                  GdkGLContext *share_list,
                                  HGLRC         hglrc)
{
  GdkGLContext *glcontext;
  GdkGLContextImpl *impl;

  GDK_GL_NOTE_FUNC ();

  glcontext = g_object_new(GDK_TYPE_WIN32_GL_CONTEXT, NULL);

  g_return_val_if_fail(glcontext != NULL, NULL);

  impl = _gdk_win32_gl_context_impl_new_from_hglrc (glcontext,
                                                    glconfig,
                                                    share_list,
                                                    hglrc);
  if (impl == NULL)
    g_object_unref(glconfig);

  g_return_val_if_fail(impl != NULL, NULL);

  return glcontext;
}

HGLRC
gdk_win32_gl_context_get_hglrc (GdkGLContext *glcontext)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONTEXT (glcontext), NULL);

  return GDK_GL_CONTEXT_IMPL_WIN32_CLASS (glcontext->impl)->get_hglrc(glcontext);
}
