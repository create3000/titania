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

#ifndef __TITANIA_MATH_MESH_NURBS_TESSELLATOR_H__
#define __TITANIA_MATH_MESH_NURBS_TESSELLATOR_H__

#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Numbers/Vector4.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

namespace titania {
namespace math {

class nurbs_tessellator_elements
{
public:

	///  @name Member access

	const std::vector <vector4 <float>> &
	tex_coords () const
	{ return m_tex_coords; }

	const std::vector <vector3 <float>> &
	normals () const
	{ return m_normals; }

	const std::vector <vector3 <float>> &
	vertices () const
	{ return m_vertices; }


private:

	///  @name Friends

	friend class nurbs_tessellator;

	///  @name Members

	std::vector <vector4 <float>> m_tex_coords;
	std::vector <vector3 <float>> m_normals;
	std::vector <vector3 <float>> m_vertices;

};

class nurbs_tessellator
{
public:

	///  @name Construction

	nurbs_tessellator (const bool debug = false);

	///  @name Member access

	void
	only_triangles (const bool value)
	{ m_only_triangles = value; }

	bool
	only_triangles () const
	{ return m_only_triangles; }

	void
	auto_weight (const size_t value)
	{ m_auto_weight = value; }

	size_t
	auto_weight () const
	{ return m_auto_weight; }

	void
	weight (const size_t index)
	{ m_weight = index; }

	size_t
	weight () const
	{ return m_weight; }

	const std::vector <vector3 <float>> &
	lines () const
	{ return m_lines; }

	const nurbs_tessellator_elements &
	triangles () const
	{ return m_triangles; }

	const nurbs_tessellator_elements &
	quads () const
	{ return m_quads; }

	///  @name Operations

	void
	property (const GLenum property, const float value);

	void
	begin_surface ();

	void
	end_surface ();

	void
	begin_curve ();

	void
	end_curve ();

	void
	begin_trim ();

	void
	end_trim ();

	void
	nurbs_surface (const int32_t sKnotCount,
	               float* const sKnots,
	               const int32_t tKnotCount,
	               float* const tKnots,
	               const int32_t sStride,
	               const int32_t tStride,
	               float* const control,
	               const int32_t sOrder,
	               const int32_t tOrder,
	               const GLenum type);

	void
	nurbs_curve (const int32_t knotCount,
	             float* const knots,
	             const int32_t stride,
	             float* const control,
	             const int32_t order,
	             const GLenum type);

	void
	piecewise_linear_curve (const int32_t count,
                           float* const data,
                           const int32_t stride,
                           const GLenum type);

	///  @name Destruction

	~nurbs_tessellator ();


private:

	static
	void
	tess_begin_data (const GLenum type, nurbs_tessellator* const self);

	static
	void
	tess_tex_coord_data (float* const texCoord, nurbs_tessellator* const self);

	static
	void
	tess_normal_data (float* const normal, nurbs_tessellator* const self);

	static
	void
	tess_vertex_data (float* const vertex, nurbs_tessellator* const self);

	static
	void
	tess_end_data (nurbs_tessellator* const self);

	static
	void
	tess_error (const GLenum errorCode);

	///  @name Members

	GLUnurbs* const m_tess;
	bool            m_only_triangles;
	bool            m_auto_weight;
	size_t          m_weight;

	GLenum                        m_type;
	std::vector <vector4 <float>> m_tex_coords;
	std::vector <vector3 <float>> m_normals;
	std::vector <vector3 <float>> m_vertices;

	std::vector <vector3 <float>> m_lines;
	nurbs_tessellator_elements    m_triangles;
	nurbs_tessellator_elements    m_quads;

};

} // math
} // titania

#endif
