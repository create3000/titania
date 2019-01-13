/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "NurbsTessellator.h"

#include "../Functional.h"
#include <Titania/LOG.h>

namespace titania {
namespace math {

nurbs_tessellator::nurbs_tessellator (const bool debug) :
	      m_tess (gluNewNurbsRenderer ()),
	      m_type (0),
	m_tex_coords (),
	   m_normals (),
	  m_vertices (),
	 m_triangles (),
	     m_quads ()
{
	if (not m_tess)
		return;

	gluNurbsProperty (m_tess, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);

	gluNurbsCallbackData (m_tess, this);

	gluNurbsCallback (m_tess, GLU_NURBS_BEGIN_DATA,         _GLUfuncptr (&nurbs_tessellator::tess_begin_data));
	gluNurbsCallback (m_tess, GLU_NURBS_TEXTURE_COORD_DATA, _GLUfuncptr (&nurbs_tessellator::tess_tex_coord_data));
	gluNurbsCallback (m_tess, GLU_NURBS_NORMAL_DATA,        _GLUfuncptr (&nurbs_tessellator::tess_normal_data));
	gluNurbsCallback (m_tess, GLU_NURBS_VERTEX_DATA,        _GLUfuncptr (&nurbs_tessellator::tess_vertex_data));
	gluNurbsCallback (m_tess, GLU_NURBS_END_DATA,           _GLUfuncptr (&nurbs_tessellator::tess_end_data));
	gluNurbsCallback (m_tess, GLU_ERROR,                    _GLUfuncptr (&nurbs_tessellator::tess_error));

	// Options
	
	glEnable (GL_AUTO_NORMAL);
}

void
nurbs_tessellator::property (const GLenum property, const float value)
{
	gluNurbsProperty (m_tess, property, value);
}

void
nurbs_tessellator::begin_surface ()
{
	gluBeginSurface (m_tess);
}

void
nurbs_tessellator::nurbs_surface (int32_t sKnotCount,
                            float* sKnots,
                            int32_t tKnotCount,
                            float* tKnots,
                            int32_t sStride,
                            int32_t tStride,
                            float* control,
                            int32_t sOrder,
                            int32_t tOrder,
                            GLenum type)
{
	gluNurbsSurface (m_tess,
	                 sKnotCount,
	                 sKnots,
	                 tKnotCount,
	                 tKnots,
	                 sStride,
	                 tStride,
	                 control,
	                 sOrder,
	                 tOrder,
	                 type);
}

void
nurbs_tessellator::nurbs_curve (int32_t knotCount,
                                float* knots,
                                int32_t stride,
                                float* control,
                                int32_t order,
                                GLenum type)
{
	gluNurbsCurve (m_tess,
	               knotCount,
	               knots,
	               stride,
	               control,
	               order,
	               type);
}

void
nurbs_tessellator::pwl_curve (int32_t count,
                              float* data,
                              int32_t stride,
                              GLenum type)
{
	gluPwlCurve (m_tess,
	             count,
	             data,
	             stride,
	             type);
}

void
nurbs_tessellator::begin_trim ()
{
	gluBeginTrim (m_tess);
}

void
nurbs_tessellator::end_trim ()
{
	gluEndTrim (m_tess);
}

void
nurbs_tessellator::end_surface ()
{
	gluEndSurface (m_tess);
}

void
nurbs_tessellator::tess_begin_data (GLenum type, nurbs_tessellator* self)
{
	self -> m_type = type;

	self -> m_tex_coords .clear ();
	self -> m_normals    .clear ();
	self -> m_vertices   .clear ();
}

void
nurbs_tessellator::tess_tex_coord_data (float* texCoord, nurbs_tessellator* self)
{
	self -> m_tex_coords .emplace_back (texCoord [0], texCoord [1], texCoord [2], texCoord [3]);
}

void
nurbs_tessellator::tess_normal_data (float* normal, nurbs_tessellator* self)
{
	self -> m_normals .emplace_back (normal [0], normal [1], normal [2]);
}

void
nurbs_tessellator::tess_vertex_data (float* vertex, nurbs_tessellator* self)
{
	self -> m_vertices .emplace_back (vertex [0], vertex [1], vertex [2]);
}

void
nurbs_tessellator::tess_end_data (nurbs_tessellator* self)
{
	auto & m_tex_coords = self -> m_tex_coords;
	auto & m_normals    = self -> m_normals;
	auto & m_vertices   = self -> m_vertices;

	switch (self -> m_type)
	{
		case GL_TRIANGLE_FAN:
		{
			auto & m_triangles_tex_coord = self -> m_triangles .m_tex_coords;
			auto & m_triangles_normals   = self -> m_triangles .m_normals;
			auto & m_triangles_vertices  = self -> m_triangles .m_vertices;

		   for (size_t i = 1, size = m_vertices .size () - 1; i < size; ++ i)
		   {
				size_t i1 = 0;
				size_t i2 = i;
				size_t i3 = i + 1;

				m_triangles_tex_coord .emplace_back (m_tex_coords [i1]);
				m_triangles_tex_coord .emplace_back (m_tex_coords [i2]);
				m_triangles_tex_coord .emplace_back (m_tex_coords [i3]);

				m_triangles_normals .emplace_back (m_normals [i1]);
				m_triangles_normals .emplace_back (m_normals [i2]);
				m_triangles_normals .emplace_back (m_normals [i3]);

				m_triangles_vertices .emplace_back (m_vertices [i1]);
				m_triangles_vertices .emplace_back (m_vertices [i2]);
				m_triangles_vertices .emplace_back (m_vertices [i3]);
		   }

			break;
		}
		case GL_TRIANGLE_STRIP:
		{
			auto & m_triangles_tex_coord = self -> m_triangles .m_tex_coords;
			auto & m_triangles_normals   = self -> m_triangles .m_normals;
			auto & m_triangles_vertices  = self -> m_triangles .m_vertices;

			for (size_t i = 0, size = m_vertices .size () - 2; i < size; ++ i)
			{
				size_t i1, i2, i3;
				
				if (is_odd (i))
				{
					i1 = i;
					i2 = i + 1;
					i3 = i + 2;
				}
				else
				{
					i1 = i;
					i2 = i + 2;
					i3 = i + 1;
				}

				m_triangles_tex_coord .emplace_back (m_tex_coords [i1]);
				m_triangles_tex_coord .emplace_back (m_tex_coords [i2]);
				m_triangles_tex_coord .emplace_back (m_tex_coords [i3]);

				m_triangles_normals .emplace_back (m_normals [i1]);
				m_triangles_normals .emplace_back (m_normals [i2]);
				m_triangles_normals .emplace_back (m_normals [i3]);

				m_triangles_vertices .emplace_back (m_vertices [i1]);
				m_triangles_vertices .emplace_back (m_vertices [i2]);
				m_triangles_vertices .emplace_back (m_vertices [i3]);
			}

			break;
		}
		case GL_TRIANGLES:
		{
			auto & m_triangles_tex_coord = self -> m_triangles .m_tex_coords;
			auto & m_triangles_normals   = self -> m_triangles .m_normals;
			auto & m_triangles_vertices  = self -> m_triangles .m_vertices;

			m_triangles_tex_coord .insert (m_triangles_tex_coord .end (), 
			                               m_tex_coords .cbegin (),
			                               m_tex_coords .cend ());

			m_triangles_normals .insert (m_triangles_normals .end (), 
			                             m_normals .cbegin (),
			                             m_normals .cend ());

			m_triangles_vertices .insert (m_triangles_vertices .end (), 
			                              m_vertices .cbegin (),
			                              m_vertices .cend ());

			break;
		}
		case GL_QUAD_STRIP:
		{
			auto & m_quads_tex_coord = self -> m_quads .m_tex_coords;
			auto & m_quads_normals   = self -> m_quads .m_normals;
			auto & m_quads_vertices  = self -> m_quads .m_vertices;

			for (size_t i = 0, size = m_vertices .size () - 2; i < size; i += 2)
			{
				size_t i1 = i;
				size_t i2 = i + 1;
				size_t i3 = i + 3;
				size_t i4 = i + 2;

				m_quads_tex_coord .emplace_back (m_tex_coords [i1]);
				m_quads_tex_coord .emplace_back (m_tex_coords [i2]);
				m_quads_tex_coord .emplace_back (m_tex_coords [i3]);
				m_quads_tex_coord .emplace_back (m_tex_coords [i4]);

				m_quads_normals .emplace_back (m_normals [i1]);
				m_quads_normals .emplace_back (m_normals [i2]);
				m_quads_normals .emplace_back (m_normals [i3]);
				m_quads_normals .emplace_back (m_normals [i4]);

				m_quads_vertices .emplace_back (m_vertices [i1]);
				m_quads_vertices .emplace_back (m_vertices [i2]);
				m_quads_vertices .emplace_back (m_vertices [i3]);
				m_quads_vertices .emplace_back (m_vertices [i4]);
			}

			break;
		}
		case GL_QUADS:
		{
			auto & m_quads_tex_coord = self -> m_quads .m_tex_coords;
			auto & m_quads_normals   = self -> m_quads .m_normals;
			auto & m_quads_vertices  = self -> m_quads .m_vertices;

			m_quads_tex_coord .insert (m_quads_tex_coord .end (), 
			                           m_tex_coords .cbegin (),
			                           m_tex_coords .cend ());

			m_quads_normals .insert (m_quads_normals .end (), 
			                         m_normals .cbegin (),
			                         m_normals .cend ());

			m_quads_vertices .insert (m_quads_vertices .end (), 
			                          m_vertices .cbegin (),
			                          m_vertices .cend ());

			break;
		}
		default:
			break;
	}
}

void
nurbs_tessellator::tess_error (GLenum errorCode)
{
	__LOG__ << gluErrorString (errorCode) << std::endl;
}

nurbs_tessellator::~nurbs_tessellator ()
{
	glDisable (GL_AUTO_NORMAL);

	if (m_tess)
		gluDeleteNurbsRenderer (m_tess);
}

} // math
} // titania
