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

#ifndef __TITANIA_X3D_COMPONENTS_SHADERS_X3DSHADER_NODE_H__
#define __TITANIA_X3D_COMPONENTS_SHADERS_X3DSHADER_NODE_H__

#include "../Shape/X3DAppearanceChildNode.h"

namespace titania {
namespace X3D {

class ShapeContainer;
class SoftParticle;

class X3DShaderNode :
	public X3DAppearanceChildNode
{
public:

	///  @name Fields

	SFBool &
	activate ()
	{ return *fields .activate; }

	const SFBool &
	activate () const
	{ return *fields .activate; }

	SFBool &
	isSelected ()
	{ return *fields .isSelected; }

	const SFBool &
	isSelected () const
	{ return *fields .isSelected; }

	SFBool &
	isValid ()
	{ return *fields .isValid; }

	const SFBool &
	isValid () const
	{ return *fields .isValid; }

	SFString &
	language ()
	{ return *fields .language; }

	const SFString &
	language () const
	{ return *fields .language; }

	///  @name Operations

	virtual
	void
	enable () = 0;

	virtual
	void
	disable () = 0;

	void
	select ();

	void
	deselect ();

	///  @name Pipeline

	virtual
	void
	setGlobalUniforms (X3DRenderObject* const renderObject) = 0;

	virtual
	void
	setLocalUniforms (ShapeContainer* const context) = 0;

	virtual
	void
	enableFloatAttrib (const std::string &, const GLuint, const size_t) = 0;
	
	virtual
	void
	disableFloatAttrib (const std::string &) = 0;

	virtual
	void
	enableIntegerAttrib (const std::string &, const GLuint, const size_t) = 0;

	virtual
	void
	disableIntegerAttrib (const std::string &) = 0;

	virtual
	void
	enableMatrix3Attrib (const std::string &, const GLuint) = 0;
	
	virtual
	void
	disableMatrix3Attrib (const std::string &) = 0;
	
	virtual
	void
	enableMatrix4Attrib (const std::string &, const GLuint) = 0;
	
	virtual
	void
	disableMatrix4Attrib (const std::string &) = 0;

	virtual
	void
	enableColorAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) = 0;

	virtual
	void
	disableColorAttrib () = 0;

	virtual
	void
	enableTexCoordAttrib (const std::vector <GLuint> & buffer,
	                      const GLenum type,
	                      const std::vector <GLsizei> & stride,
	                      const std::vector <GLvoid*> & pointer) = 0;

	virtual
	void
	disableTexCoordAttrib () = 0;

	virtual
	void
	enableNormalAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) = 0;

	virtual
	void
	disableNormalAttrib () = 0;

	virtual
	void
	enableVertexAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) = 0;
	
	virtual
	void
	disableVertexAttrib () = 0;

	virtual
	void
	setParticle (const size_t id, const SoftParticle & particle, const Matrix4d & modelViewMatrix) = 0;

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;


protected:

	///  @name Construction

	X3DShaderNode ();


private:

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const activate;
		SFBool* const isSelected;
		SFBool* const isValid;
		SFString* const language;
	};

	Fields fields;
	
	size_t selected;

};

} // X3D
} // titania

#endif
