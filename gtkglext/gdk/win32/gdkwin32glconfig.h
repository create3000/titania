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

#if !defined (__GDKGLWIN32_H_INSIDE__) && !defined (GDK_GL_COMPILATION)
#error "Only <gdk/gdkglwin32.h> can be included directly."
#endif

#ifndef __GDK_WIN32_GL_CONFIG_H__
#define __GDK_WIN32_GL_CONFIG_H__

#include <gdk/gdkwin32.h>
#include <gdk/gdkgl.h>

G_BEGIN_DECLS

#define GDK_TYPE_WIN32_GL_CONFIG            (gdk_win32_gl_config_get_type ())
#define GDK_WIN32_GL_CONFIG(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_WIN32_GL_CONFIG, GdkWin32GLConfig))
#define GDK_WIN32_GL_CONFIG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_WIN32_GL_CONFIG, GdkWin32GLConfigClass))
#define GDK_IS_WIN32_GL_CONFIG(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_WIN32_GL_CONFIG))
#define GDK_IS_WIN32_GL_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_WIN32_GL_CONFIG))
#define GDK_WIN32_GL_CONFIG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_WIN32_GL_CONFIG, GdkWin32GLConfigClass))

#ifdef INSIDE_GDK_GL_WIN32
typedef struct _GdkWin32GLConfig GdkWin32GLConfig;
#else
typedef GdkGLConfig GdkWin32GLConfig;
#endif
typedef struct _GdkWin32GLConfigClass GdkWin32GLConfigClass;

GType                  gdk_win32_gl_config_get_type (void);

GdkGLConfig           *gdk_win32_gl_config_new_for_display        (GdkDisplay *display,
                                                                   const int  *attrib_list,
                                                                   gsize       n_attribs);
GdkGLConfig           *gdk_win32_gl_config_new_for_screen         (GdkScreen *screen,
                                                                   const int *attrib_list,
                                                                   gsize      n_attribs);
GdkGLConfig           *gdk_win32_gl_config_new_from_pixel_format  (int pixel_format);

PIXELFORMATDESCRIPTOR *gdk_win32_gl_config_get_pfd                (GdkGLConfig *glconfig);

G_END_DECLS

#ifdef INSIDE_GDK_GL_WIN32
#define GDK_GL_CONFIG_PFD(glconfig)          (&(GDK_GL_CONFIG_IMPL_WIN32 (glconfig->impl)->pfd))
#else
#define GDK_GL_CONFIG_PFD(glconfig)          (gdk_win32_gl_config_get_pfd (glconfig))
#endif

#endif /* __GDK_WIN32_GL_CONFIG_H__ */
