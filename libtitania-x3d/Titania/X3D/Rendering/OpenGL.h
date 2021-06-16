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

#ifndef __TITANIA_X3D_RENDERING_OPEN_GL_H__
#define __TITANIA_X3D_RENDERING_OPEN_GL_H__

#include <gtkmm/drawingarea.h>

extern "C"
{
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glx.h>
#endif
}

#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

class PolygonModeLock
{
public:

	PolygonModeLock (GLenum type) :
		m_polygonMode ()
	{
		glGetIntegerv (GL_POLYGON_MODE, m_polygonMode);

		glPolygonMode (GL_FRONT_AND_BACK, type);
	}

	GLenum
	mode () const
	{ return m_polygonMode [0]; }

	~PolygonModeLock ()
	{
		glPolygonMode (GL_FRONT_AND_BACK, m_polygonMode [0]);
	}

private:

	GLint m_polygonMode [2]; // Front & back
};

class PolygonOffsetLock
{
public:

	PolygonOffsetLock (GLenum type, float factor, float units) :
		   m_type (type),
		m_enabled (glIsEnabled (type)),
		 m_factor (0),
		  m_units (0)
	{
		glGetFloatv (GL_POLYGON_OFFSET_FACTOR, &m_factor);
		glGetFloatv (GL_POLYGON_OFFSET_UNITS,  &m_units);

		glEnable (m_type);
		glPolygonOffset (factor, units);
	}

	~PolygonOffsetLock ()
	{
		if (not m_enabled)
			glDisable (m_type);

		glPolygonOffset (m_factor, m_units);
	}

private:

	GLenum m_type;
	bool   m_enabled;
	float  m_factor;
	float  m_units;
};

class DepthTestLock
{
public:

	 DepthTestLock (GLenum function) :
		 m_enabled (glIsEnabled (GL_DEPTH_TEST)),
		m_function ()
	{
		glGetIntegerv (GL_DEPTH_FUNC, &m_function);

		glEnable (GL_DEPTH_TEST);
		glDepthFunc (function);
	}

	~DepthTestLock ()
	{
		if (not m_enabled)
			glDisable (GL_DEPTH_FUNC);

		glDepthFunc (m_function);
	}

private:

	bool  m_enabled;
	GLint m_function;
};

} // X3D
} // titania

#endif
