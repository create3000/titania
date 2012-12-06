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

#ifndef __GDK_GL_WIN32_H__
#define __GDK_GL_WIN32_H__

#ifndef STRICT
#define STRICT                  /* We want strict type checks */
#endif
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

/* MinGW's wingdi.h bug. */
#ifndef PFD_SWAP_LAYER_BUFFERS
#define PFD_SWAP_LAYER_BUFFERS      0x00000800
#endif

#include <gdk/gdkwin32.h>

#include <GL/gl.h>

#define __GDKGLWIN32_H_INSIDE__

#include <gdk/win32/gdkwin32glconfig.h>
#include <gdk/win32/gdkwin32glcontext.h>
#include <gdk/win32/gdkwin32glquery.h>
#include <gdk/win32/gdkwin32glwindow.h>

#undef __GDKGLWIN32_H_INSIDE__

#endif /* __GDK_GL_WIN32_H__ */
