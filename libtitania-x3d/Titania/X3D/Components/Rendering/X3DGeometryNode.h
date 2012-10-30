/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
 ******************************************************************************/

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_X3DGEOMETRY_NODE_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_X3DGEOMETRY_NODE_H__

#include "../../Browser/Hit.h"
#include "../../Types/Geometry.h"
#include "../Core/X3DNode.h"
#include "../Texturing/TextureCoordinateGenerator.h"

namespace titania {
namespace X3D {

class X3DGeometryNode :
	virtual public X3DNode
{
public:

	const Box3f
	getBBox ();

	bool
	intersect (const Line3f &, Hit*) const;

	bool
	isTransparent () { return glNumColors == 4; }

	virtual
	void
	draw ();

	virtual
	void
	display ();

	virtual
	void
	dispose ();


protected:

	X3DGeometryNode ();

	virtual
	void
	initialize ();

	typedef std::map <size_t, std::vector <size_t>> VertexMap;
	void
	setGLSolid (const bool value) { solid = value; }

	void
	setGLCCW (const GLenum value) { ccw = value; }

	std::vector <GLfloat>*
	getGLTexCoord () { return &glTexCoord; }

	void
	setTextureCoordinateGenerator (TextureCoordinateGenerator* value) { textureCoordinateGenerator = value; }

	void
	setGLNumColors (const GLint value) { glNumColors = value; }

	std::vector <GLfloat>*
	getGLColors () { return &glColors; }

	std::vector <GLfloat>*
	getGLNormals () { return &glNormals; }

	std::vector <GLfloat>*
	getGLPoints () { return &glPoints; }

	void
	setGLMode (const GLenum value) { glMode = value; }

	void
	setGLIndices (const GLsizei value) { glIndices = value; }

	GLsizei
	getGLIndices () { return glIndices; }

	virtual
	Box3f
	createBBox ();

	void
	refineNormals (std::vector <Vector3f> &, const VertexMap &, const float, const bool);

	void
	clear ();

	virtual
	void
	build ();

	virtual
	void
	eventsProcessed ();


private:

	void
	update ();

	void
	transfer ();

	bool                        static_draw;
	Box3f                       bbox;
	bool                        solid;
	GLenum                      ccw;
	std::vector <GLfloat>       glTexCoord;
	TextureCoordinateGenerator* textureCoordinateGenerator;
	GLint                       glNumColors;
	std::vector <GLfloat>       glColors;
	std::vector <GLfloat>       glNormals;
	std::vector <GLfloat>       glPoints;
	GLenum                      glMode;
	GLsizei                     glIndices;
	GLuint                      texCoordBufferId;
	GLuint                      colorBufferId;
	GLuint                      normalBufferId;
	GLuint                      pointBufferId;

};

} // X3D
} // titania

#endif
