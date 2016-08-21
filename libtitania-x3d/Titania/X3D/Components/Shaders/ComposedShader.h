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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_COMPONENTS_SHADERS_COMPOSED_SHADER_H__
#define __TITANIA_X3D_COMPONENTS_SHADERS_COMPOSED_SHADER_H__

#include "../Networking/LoadSensor.h"
#include "../Shaders/X3DProgrammableShaderObject.h"
#include "../Shaders/X3DShaderNode.h"

namespace titania {
namespace X3D {

class ComposedShader :
	public X3DShaderNode,
	public X3DProgrammableShaderObject
{
public:

	///  @name Construction

	ComposedShader (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Fields

	MFNode &
	parts ()
	{ return *fields .parts; }

	const MFNode &
	parts () const
	{ return *fields .parts; }

	///  @name Member access

	virtual
	GLuint
	getProgramId () const final override
	{ return programId; }

	///  @name Operations

	virtual
	void
	addUserDefinedField (const AccessType, const std::string &, X3DFieldDefinition* const)
	throw (Error <INVALID_NAME>,
	       Error <INVALID_FIELD>,
	       Error <DISPOSED>) final override;

	virtual
	void
	removeUserDefinedField (const std::string &)
	throw (Error <DISPOSED>) final override;

	virtual
	void
	enable () final override;

	virtual
	void
	disable () final override;

	virtual
	void
	draw () final override;

	///  @name Pipeline

	virtual
	void
	setGlobalUniforms (ShapeContainer* const context) final override
	{ X3DProgrammableShaderObject::setGlobalUniforms (context); }

	virtual
	void
	setLocalUniforms (ShapeContainer* const context) final override
	{ X3DProgrammableShaderObject::setLocalUniforms (context); }

	virtual
	void
	enableColorAttrib (const GLuint buffer) final override
	{ X3DProgrammableShaderObject::enableColorAttrib (buffer); }

	virtual
	void
	enableTexCoordAttrib (const std::vector <GLuint> & buffers) final override
	{ X3DProgrammableShaderObject::enableTexCoordAttrib (buffers); }

	virtual
	void
	enableNormalAttrib (const GLuint buffer) final override
	{ X3DProgrammableShaderObject::enableNormalAttrib (buffer); }

	virtual
	void
	enableVertexAttrib (const GLuint buffer) final override
	{ X3DProgrammableShaderObject::enableVertexAttrib (buffer); }
	
	virtual
	void
	disableColorAttrib () final override
	{ X3DProgrammableShaderObject::disableColorAttrib (); }
	
	virtual
	void
	disableTexCoordAttrib () final override
	{ X3DProgrammableShaderObject::disableTexCoordAttrib (); }
	
	virtual
	void
	disableNormalAttrib () final override
	{ X3DProgrammableShaderObject::disableNormalAttrib (); }
	
	virtual
	void
	disableVertexAttrib () final override
	{ X3DProgrammableShaderObject::disableVertexAttrib (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_activate ();

	void
	set_loaded ();

	///  @name Operations

	void
	printProgramInfoLog () const;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFNode* const parts;
	};

	Fields fields;

	X3DPtr <LoadSensor> loadSensor;
	GLuint              programId;

};

} // X3D
} // titania

#endif
