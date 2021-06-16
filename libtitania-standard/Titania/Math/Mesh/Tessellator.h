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

#ifndef __TITANIA_MATH_MESH_TESSELLATOR_H__
#define __TITANIA_MATH_MESH_TESSELLATOR_H__

#include <Titania/Basic/ReferenceIterator.h>
#include <Titania/Math/Numbers/Vector3.h>
#include <Titania/Math/Functional.h>

extern "C"
{
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
}

#include <iostream>
#include <functional>
#include <tuple>
#include <deque>
#include <vector>

#include <Titania/LOG.h>

namespace titania {
namespace math {

// https://www.opengl.org/sdk/docs/man2/xhtml/gluTessCallback.xml
// http://www.glprogramming.com/red/chapter11.html

template <class Type, class ... Args>
class tessellator;

template <class Type, class ... Args>
class tessellator_vertex
{
public:

	///  @name Member types

	using data_type = std::tuple <Args ...>;

	///  @name Construction

	template <class ... A>
	tessellator_vertex (const vector3 <Type> & point, A && ... data) :
		m_point (point),
		m_data  (std::forward <A> (data) ...)
	{ }

	///  @name Member access

	vector3 <Type> &
	point ()
	{ return m_point; }

	const vector3 <Type> &
	point () const
	{ return m_point; }

	void
	data (const data_type & value)
	{ m_data = value; }

	const data_type &
	data () const
	{ return m_data; }


private:

	///  @name Members

	vector3 <Type> m_point;
	data_type      m_data;

};

template <class Type, class ... Args>
class tessellator_polygon
{
public:

	///  @name Member types

	using vertex_type       = tessellator_vertex <Type, Args ...>;
	using vertex_array_type = std::vector <vertex_type*>;
	using const_iterator    = basic::reference_iterator <typename vertex_array_type::const_iterator, vertex_type>;
	using size_type         = typename vertex_array_type::size_type;

	///  @name Construction

	tessellator_polygon (GLenum type) :
		    m_type (type),
		m_vertices ()
	{ }

	tessellator_polygon (const tessellator_polygon & other) :
		    m_type (other .m_type),
		m_vertices (other .m_vertices)
	{ }

	tessellator_polygon (tessellator_polygon && other) :
		    m_type (other .m_type),
		m_vertices (std::move (other .m_vertices))
	{ }

	///  @name Assignment operators

	tessellator_polygon &
	operator = (const tessellator_polygon & other)
	{
		m_type     = other .m_type;
		m_vertices = other .m_vertices;
	}

	tessellator_polygon &
	operator = (tessellator_polygon && other)
	{
		m_type     = other .m_type;
		m_vertices = std::move (other .m_vertices);
	}

	///  @name Member access

	const GLenum &
	type () const
	{ return m_type; }

	const vertex_type &
	operator [ ] (size_t i) const
	{ return *m_vertices [i]; }

	const_iterator
	begin () const
	{ return const_iterator (m_vertices .begin ()); }

	const_iterator
	end () const
	{ return const_iterator (m_vertices .end ()); }

	size_type
	size () const
	{ return m_vertices .size (); }

	bool
	empty () const
	{ return m_vertices .empty (); }


private:

	///  @name Friends

	friend class tessellator <Type, Args ...>;

	///  @name Members

	GLenum            m_type;
	vertex_array_type m_vertices;

};

template <class Type, class ... Args>
class tessellator
{
public:

	///  @name Member types

	using polygon_type          = tessellator_polygon <Type, Args ...>;
	using polygon_array_type    = std::vector <polygon_type>;
	using vertex_type           = tessellator_vertex <Type, Args ...>;
	using data_type             = typename vertex_type::data_type;
	using combine_function_type = std::function <data_type (const vector3 <Type> &, const vertex_type* const [4], const float [4])>;

	///  @name Construction

	tessellator (const bool = false);

	///  @name Operations

	void
	property (const GLenum property, const GLdouble value);

	void
	normal (const vector3 <Type> & normal);

	void
	combine (const combine_function_type & value);

	void
	clear ();

	void
	begin_polygon ();

	void
	end_polygon ();

	void
	begin_contour ();

	void
	end_contour ();

	template <class ... A>
	void
	add_vertex (const vector3 <Type> & point, A && ... args);

	///  @name Members access

	const polygon_array_type &
	polygons () const
	{ return m_polygons; }

	polygon_type
	triangles () const;

	///  @name Destruction

	~tessellator ();


private:

	///  @name Operations

	static
	void
	tess_begin_data (const GLenum type, tessellator* const self);

	static
	void
	tess_vertex_data (vertex_type* const vertex, tessellator* const self);

	static
	void
	tess_combine_data (const double coords [3], const vertex_type* const vertices [4],
	                   const float weight [4], void** outData,
	                   tessellator* const self);

	static
	void
	tess_end_data (tessellator* const self);

	static
	void tess_error (const GLenum errorCode);

	///  @name Members

	GLUtesselator*           m_tess;
	std::deque <vertex_type> m_vertices;
	polygon_array_type       m_polygons;
	combine_function_type    m_combine;

};

template <class Type, class ... Args>
tessellator <Type, Args ...>::tessellator (const bool debug) :
	    m_tess (gluNewTess ()),
	m_vertices (),
	m_polygons (),
	 m_combine ()
{
	if (m_tess)
	{
		gluTessProperty (m_tess, GLU_TESS_BOUNDARY_ONLY, GLU_FALSE);
		gluTessCallback (m_tess, GLU_TESS_BEGIN_DATA,   (void (*)()) &tessellator::tess_begin_data);
		gluTessCallback (m_tess, GLU_TESS_VERTEX_DATA,  (void (*)()) &tessellator::tess_vertex_data);
		gluTessCallback (m_tess, GLU_TESS_END_DATA,     (void (*)()) &tessellator::tess_end_data);

		if (debug)
			gluTessCallback (m_tess, GLU_TESS_ERROR, (void (*)()) (&tessellator::tess_error));
	}
}

template <class Type, class ... Args>
inline
void
tessellator <Type, Args ...>::property (const GLenum property, const GLdouble value)
{
	gluTessProperty (m_tess, property, value);
}

template <class Type, class ... Args>
inline
void
tessellator <Type, Args ...>::normal (const vector3 <Type> & normal)
{
	gluTessNormal (m_tess, normal .x (), normal .y (), normal .z ());
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::combine (const combine_function_type & value)
{
	m_combine = value;

	if (m_combine)
		gluTessCallback (m_tess, GLU_TESS_COMBINE_DATA, (void (*)()) &tessellator::tess_combine_data);
	else
		gluTessCallback (m_tess, GLU_TESS_COMBINE_DATA, nullptr);
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::clear ()
{
	m_polygons .clear ();
	m_vertices .clear ();
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::begin_polygon ()
{
	gluTessBeginPolygon (m_tess, this);
}

template <class Type, class ... Args>
inline
void
tessellator <Type, Args ...>::end_polygon ()
{
	gluEndPolygon (m_tess);
}

template <class Type, class ... Args>
inline
void
tessellator <Type, Args ...>::begin_contour ()
{
	gluTessBeginContour (m_tess);
}

template <class Type, class ... Args>
inline
void
tessellator <Type, Args ...>::end_contour ()
{
	gluTessEndContour (m_tess);
}

template <class Type, class ... Args>
template <class ... A>
inline
void
tessellator <Type, Args ...>::add_vertex (const vector3 <Type> & point, A && ... args)
{
	m_vertices .emplace_back (point, std::forward <A> (args) ...);

	gluTessVertex (m_tess, m_vertices .back () .point () .data (), &m_vertices .back ());
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tess_begin_data (const GLenum type, tessellator* const self)
{
	self -> m_polygons .emplace_back (type);
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tess_vertex_data (vertex_type* const vertex, tessellator* const self)
{
	self -> m_polygons .back () .m_vertices .emplace_back (vertex);
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tess_combine_data (const double coords [3], const vertex_type* const vertices [4],
                                                 const float weight [4], void** outData,
                                                 tessellator* const self)
{
	vector3 <Type> point (coords [0], coords [1], coords [2]);

	self -> m_vertices .emplace_back (point, self -> m_combine (point, vertices, weight));

	*outData = &self -> m_vertices .back ();
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tess_end_data (tessellator* const self)
{ }

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tess_error (const GLenum errorCode)
{
	#ifdef TITANIA_DEBUG
	std::clog << "Warning: tessellation error: '" << (char*) gluErrorString (errorCode) << "'." << std::endl;
	#endif
}

template <class Type, class ... Args>
typename tessellator <Type, Args ...>::polygon_type
tessellator <Type, Args ...>::triangles () const
{
	polygon_type triangles (GL_TRIANGLES);

	auto & vertices = triangles .m_vertices;

	for (auto & polygon : this -> polygons ())
	{
		switch (polygon .type ())
		{
			case GL_TRIANGLE_FAN:
			{
				for (size_t i = 1, size = polygon .size () - 1; i < size; ++ i)
				{
					// Add triangle to polygon.
					vertices .emplace_back (polygon .m_vertices [0]);
					vertices .emplace_back (polygon .m_vertices [i]);
					vertices .emplace_back (polygon .m_vertices [i + 1]);
				}

				break;
			}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygon .size () - 2; i < size; ++ i)
				{
					// Add triangle to polygon.
					vertices .emplace_back (polygon .m_vertices [is_odd (i) ? i + 1 : i]);
					vertices .emplace_back (polygon .m_vertices [is_odd (i) ? i : i + 1]);
					vertices .emplace_back (polygon .m_vertices [i + 2]);
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygon .size (); i < size; i += 3)
				{
					// Add triangle to polygon.
					vertices .emplace_back (polygon .m_vertices [i]);
					vertices .emplace_back (polygon .m_vertices [i + 1]);
					vertices .emplace_back (polygon .m_vertices [i + 2]);
				}

				break;
			}
			default:
				break;
		}
	}

	return triangles;
}

template <class Type, class ... Args>
inline
tessellator <Type, Args ...>::~tessellator ()
{
	if (m_tess)
		gluDeleteTess (m_tess);
}

} // math
} // titania

#endif
