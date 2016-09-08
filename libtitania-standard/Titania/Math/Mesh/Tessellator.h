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

#include <GL/gl.h>
#include <GL/glu.h>

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

	using Data = std::tuple <Args ...>;

	///  @name Construction

	template <class ... A>
	tessellator_vertex (const math::vector3 <Type> & point, A && ... data) :
		m_point (point),
		m_data  (std::forward <A> (data) ...)
	{ }

	///  @name Member access

	math::vector3 <Type> &
	point ()
	{ return m_point; }

	const math::vector3 <Type> &
	point () const
	{ return m_point; }

	void
	data (const Data & value)
	{ m_data = value; }

	const Data &
	data () const
	{ return m_data; }


private:

	///  @name Members

	math::vector3 <Type> m_point;
	Data                 m_data;

};

template <class Type, class ... Args>
class tessellator_polygon_element
{
public:

	///  @name Member types

	using vertex         = tessellator_vertex <Type, Args ...>;
	using vertex_array   = std::vector <vertex*>;
	using const_iterator = basic::reference_iterator <typename vertex_array::const_iterator, vertex>;
	using size_type      = typename vertex_array::size_type;

	///  @name Construction

	tessellator_polygon_element (GLenum type) :
		    m_type (type),
		m_vertices ()
	{ }

	///  @name Member access

	const GLenum &
	type () const
	{ return m_type; }

	const vertex &
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


private:

	///  @name Friends

	friend class tessellator <Type, Args ...>;

	///  @name Members

	GLenum       m_type;
	vertex_array m_vertices;

};

template <class Type, class ... Args>
class tessellator
{
public:

	///  @name Member types

	using polygon_element       = tessellator_polygon_element <Type, Args ...>;
	using polygon_element_array = std::vector <polygon_element>;
	using vertex                = tessellator_vertex <Type, Args ...>;
	using combine_function      = std::function <void (vertex &, const vertex* const [4], const float [4])>;

	///  @name Construction

	tessellator (const bool = false);

	///  @name Operations

	void
	property (const GLenum, const double);

	void
	normal (const math::vector3 <Type> &);

	void
	combine (const combine_function & value)
	{ m_combine = value; }

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
	add_vertex (const math::vector3 <Type> &, A && ... args);

	///  @name Members access

	const polygon_element_array &
	polygon () const
	{ return m_polygon; }

	polygon_element
	triangles () const;

	///  @name Destruction

	~tessellator ();


private:

	///  @name Operations

	static
	void
	tessBeginData (const GLenum, tessellator* const);

	static
	void
	tessVertexData (vertex* const, tessellator* const);

	static
	void
	tessCombineData (const double [3], const vertex* const [4], const float [4], void**, tessellator* const);

	static
	void
	tessEndData (tessellator* const);

	static
	void tessError (GLenum);

	///  @name Members

	GLUtesselator*        m_tess;
	std::deque <vertex>   m_vertices;
	polygon_element_array m_polygon;
	combine_function      m_combine;

};

template <class Type, class ... Args>
tessellator <Type, Args ...>::tessellator (const bool debug) :
	    m_tess (gluNewTess ()),
	m_vertices (),
	 m_polygon (),
	 m_combine ()
{
	if (m_tess)
	{
		gluTessProperty (m_tess, GLU_TESS_BOUNDARY_ONLY, GLU_FALSE);
		gluTessCallback (m_tess, GLU_TESS_BEGIN_DATA,   _GLUfuncptr (&tessellator::tessBeginData));
		gluTessCallback (m_tess, GLU_TESS_VERTEX_DATA,  _GLUfuncptr (&tessellator::tessVertexData));
		gluTessCallback (m_tess, GLU_TESS_COMBINE_DATA, _GLUfuncptr (&tessellator::tessCombineData));
		gluTessCallback (m_tess, GLU_TESS_END_DATA,     _GLUfuncptr (&tessellator::tessEndData));

		if (debug)
			gluTessCallback (m_tess, GLU_TESS_ERROR, _GLUfuncptr (&tessellator::tessError));
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
tessellator <Type, Args ...>::normal (const math::vector3 <Type> & normal)
{
	gluTessNormal (m_tess, normal .x (), normal .y (), normal .z ());
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::begin_polygon ()
{
	m_polygon  .clear ();
	m_vertices .clear ();

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
tessellator <Type, Args ...>::add_vertex (const math::vector3 <Type> & point, A && ... args)
{
	m_vertices .emplace_back (point, std::forward <A> (args) ...);

	gluTessVertex (m_tess, m_vertices .back () .point () .data (), &m_vertices .back ());
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tessBeginData (const GLenum type, tessellator* const self)
{
	self -> m_polygon .emplace_back (type);
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tessVertexData (vertex* const vertex, tessellator* const self)
{
	self -> m_polygon .back () .m_vertices .emplace_back (vertex);
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tessCombineData (const double coords [3], const vertex* const vertices [4],
                                               const float weight [4], void** outData,
                                               tessellator* const self)
{
	self -> m_vertices .emplace_back (math::vector3 <Type> (coords [0], coords [1], coords [2]), vertices [0] -> data ());

	*outData = &self -> m_vertices .back ();

	if (self -> m_combine)
		self -> m_combine (self -> m_vertices .back (), vertices, weight);
}

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tessEndData (tessellator* const self)
{ }

template <class Type, class ... Args>
void
tessellator <Type, Args ...>::tessError (const GLenum err_no)
{
	#ifdef TITANIA_DEBUG
	std::clog << "Warning: tessellation error: '" << (char*) gluErrorString (err_no) << "'." << std::endl;
	#endif
}

template <class Type, class ... Args>
typename tessellator <Type, Args ...>::polygon_element
tessellator <Type, Args ...>::triangles () const
{
	polygon_element element (GL_TRIANGLES);

	auto & vertices = element .m_vertices;

	for (const auto & polygonElement : this -> polygon ())
	{
		switch (polygonElement .type ())
		{
			case GL_TRIANGLE_FAN :
				{
					for (size_t i = 1, size = polygonElement .size () - 1; i < size; ++ i)
					{
						// Add triangle to polygon.
						vertices .emplace_back (const_cast <vertex*> (&polygonElement [0]));
						vertices .emplace_back (const_cast <vertex*> (&polygonElement [i]));
						vertices .emplace_back (const_cast <vertex*> (&polygonElement [i + 1]));
					}

					break;
				}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygonElement .size () - 2; i < size; ++ i)
				{
					// Add triangle to polygon.
					vertices .emplace_back (const_cast <vertex*> (&polygonElement [math::is_odd (i) ? i + 1 : i]));
					vertices .emplace_back (const_cast <vertex*> (&polygonElement [math::is_odd (i) ? i : i + 1]));
					vertices .emplace_back (const_cast <vertex*> (&polygonElement [i + 2]));
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygonElement .size (); i < size; i += 3)
				{
					// Add triangle to polygon.
					vertices .emplace_back (const_cast <vertex*> (&polygonElement [i]));
					vertices .emplace_back (const_cast <vertex*> (&polygonElement [i + 1]));
					vertices .emplace_back (const_cast <vertex*> (&polygonElement [i + 2]));
				}

				break;
			}
			default:
				break;
		}
	}

	return element;
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
