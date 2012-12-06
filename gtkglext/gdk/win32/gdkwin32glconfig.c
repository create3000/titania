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

#include "gdkglconfig-win32.h"

struct _GdkWin32GLConfig
{
  GdkGLConfig parent;
};

struct _GdkWin32GLConfigClass
{
  GdkGLConfigClass parent_class;
};

G_DEFINE_TYPE (GdkWin32GLConfig, gdk_win32_gl_config, GDK_TYPE_GL_CONFIG);

static void
gdk_win32_gl_config_init (GdkWin32GLConfig *self)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();
}

static void
gdk_win32_gl_config_finalize (GObject *object)
{
  GDK_GL_NOTE_FUNC_PRIVATE ();

  G_OBJECT_CLASS (gdk_win32_gl_config_parent_class)->finalize (object);
}

static void
gdk_win32_gl_config_class_init (GdkWin32GLConfigClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  GDK_GL_NOTE_FUNC_PRIVATE ();

  object_class->finalize = gdk_win32_gl_config_finalize;
}

/**
 * gdk_win32_gl_config_new_for_display:
 * @display: display.
 * @attrib_list: (array length=n_attribs): the attribute list.
 * @n_attribs: the number of attributes and values in attrib_list
 *
 * Creates a #GdkGLConfig on the given display.
 *
 * Return value: the new #GdkGLConfig.
 **/
GdkGLConfig *
gdk_win32_gl_config_new_for_display(GdkDisplay *display, const int *attrib_list, gsize n_attribs)
{
  GdkGLConfig *glconfig;
  GdkGLConfig *impl;

  g_return_val_if_fail(GDK_IS_WIN32_DISPLAY(display), NULL);

  glconfig = g_object_new(GDK_TYPE_WIN32_GL_CONFIG, NULL);

  g_return_val_if_fail(glconfig != NULL, NULL);

  impl = _gdk_win32_gl_config_impl_new(glconfig, attrib_list, n_attribs);

  if (impl == NULL)
    g_object_unref(glconfig);

  g_return_val_if_fail(impl != NULL, NULL);

  return glconfig;
}

/**
 * gdk_win32_gl_config_new_for_screen:
 * @screen: target screen.
 * @attrib_list: (array length=n_attribs): the attribute list.
 * @n_attribs: the number of attributes and values in attrib_list
 *
 * Creates a #GdkGLConfig on the given display.
 *
 * Return value: the new #GdkGLConfig.
 **/
GdkGLConfig *
gdk_win32_gl_config_new_for_screen(GdkScreen *screen, const int *attrib_list, gsize n_attribs)
{
  GdkGLConfig *glconfig;
  GdkGLConfig *impl;

  glconfig = g_object_new(GDK_TYPE_WIN32_GL_CONFIG, NULL);

  g_return_val_if_fail(glconfig != NULL, NULL);

  impl = _gdk_win32_gl_config_impl_new(glconfig, attrib_list, n_attribs);

  if (impl == NULL)
    g_object_unref(glconfig);

  g_return_val_if_fail(impl != NULL, NULL);

  return glconfig;
}

GdkGLConfig *
gdk_win32_gl_config_new_from_pixel_format (int pixel_format)
{
  GdkGLConfig *glconfig;
  GdkGLConfig *impl;

  GDK_GL_NOTE_FUNC ();

  glconfig = g_object_new(GDK_TYPE_WIN32_GL_CONFIG, NULL);

  g_return_val_if_fail(glconfig != NULL, NULL);

  impl = _gdk_win32_gl_config_impl_new_from_pixel_format(glconfig,
                                                         pixel_format);
  if (impl == NULL)
    g_object_unref(glconfig);

  g_return_val_if_fail(impl != NULL, NULL);

  return glconfig;
}

PIXELFORMATDESCRIPTOR *
gdk_win32_gl_config_get_pfd (GdkGLConfig *glconfig)
{
  g_return_val_if_fail (GDK_IS_WIN32_GL_CONFIG (glconfig), NULL);

  return GDK_GL_CONFIG_IMPL_WIN32_CLASS (glconfig->impl)->get_pfd(glconfig);
}
