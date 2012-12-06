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

#ifndef __GDK_GL_PRIVATE_X11_H__
#define __GDK_GL_PRIVATE_X11_H__

#include <gdk/gdkprivate.h>

#include <gdk/gdkglprivate.h>

G_BEGIN_DECLS

void _gdk_x11_gl_print_glx_info (Display *xdisplay,
                                 int      screen_num);

#define GDK_GL_CONTEXT_IS_DESTROYED(glcontext) \
  ( ((GdkGLContextImplX11 *) (glcontext->impl))->is_destroyed )

#define GDK_GL_WINDOW_IS_DESTROYED(glwindow) \
  ( ((GdkGLWindowImplX11 *) (glwindow->impl))->is_destroyed )

G_END_DECLS

#endif /* __GDK_GL_PRIVATE_X11_H__ */
