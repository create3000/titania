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

#ifndef __DRAW_SHAPES_H__
#define __DRAW_SHAPES_H__

#include <glib.h>

G_BEGIN_DECLS

void draw_cube         (gboolean solid,
                        double   size);

void draw_sphere       (gboolean solid,
                        double   radius,
                        int      slices,
                        int      stacks);

void draw_cone         (gboolean solid,
                        double   base,
                        double   height,
                        int      slices,
                        int      stacks);

void draw_torus        (gboolean solid,
                        double   inner_radius,
                        double   outer_radius,
                        int      nsides,
                        int      rings);

void draw_tetrahedron  (gboolean solid);

void draw_octahedron   (gboolean solid);

void draw_dodecahedron (gboolean solid);

void draw_icosahedron  (gboolean solid);

void draw_teapot       (gboolean solid,
                        double   scale);

G_END_DECLS

#endif /* __DRAW_SHAPES_H__ */
