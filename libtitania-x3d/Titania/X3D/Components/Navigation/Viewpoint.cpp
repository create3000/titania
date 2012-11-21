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

#include "Viewpoint.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>

namespace titania {
namespace X3D {

/*
class FieldDefinition :
	public X3DFieldDefinition
{
public:

	FieldDefinition (const AccessType &, const basic::id &);
	
	virtual
	X3DFieldDefinition*
	copy () const;

	virtual
	const X3DType*
	getType () const;

	virtual
	const basic::id
	getTypeName () const { return value -> getTypeName (); }

	AccessType
	getAccessType () const;

	///  @name Stream Handling
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	toStream (std::ostream &) const;

	virtual
	~FieldDefinition ();


private:

	const AccessType                accessType;
	const X3DFieldDefinition* const value;

};

FieldDefinition::FieldDefinition (const AccessType & accessType, const basic::id & name) :
	accessType (accessType)
{
	setName (name);
}

X3DFieldDefinition*
FieldDefinition::copy () const
{
	return new FieldDefinition (accessType, getName ());
}

const X3DType*
FieldDefinition::getType () const
{
	return type;
}

AccessType
FieldDefinition::getAccessType () const
{
	return accessType;
}

void
FieldDefinition::fromStream (std::istream &)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

void
FieldDefinition::toStream (std::ostream & stream) const
{
	stream
		<< Generator::AccessTypes [this]
		<< Generator::Space
		<< getTypeName ()
		<< Generator::Space
		<< getName ();
}

FieldDefinition::~FieldDefinition ()
{
	delete value;
}

typedef std::vector <FieldDefinition> FieldDefinitionArrayN;

class NodeTypeN :
	public X3DType
{
public:

	template <class ... Args>
	NodeTypeN (const std::string &, const basic::id &, const Args & ...);

	virtual
	const basic::id
	getTypeName () const { return type .getName (); }

	template <class ... Args>
	void
	addFieldDefinition (const AccessType &, const basic::id &const Args & ...);

	void
	addFieldDefinition (const AccessType &, const basic::id &);


private:

	FieldDefinitionArrayN fieldDefinitions;

	static const Type type;

};

const Type NodeTypeN::type ("NodeType");

template <class ... Args>
NodeTypeN::NodeTypeN (const std::string & component, const basic::id & name, const Args & ... args) :
	   X3DType (name)        
{
	addFieldDefinition (args ...);
}

template <class ... Args>
void
NodeTypeN::addFieldDefinition (const AccessType & accessType, const basic::id & name, const X3DFieldDefinition* const value, const Args & ... args)
{
	fieldDefinitions .emplace_back (accessType, name);
}

void
NodeTypeN::addFieldDefinition (const AccessType & accessType, const basic::id & name, const X3DFieldDefinition* const value)
{
	fieldDefinitions .emplace_back (accessType, name);
}

static
const NodeTypeN type ("Navigation",
                      "Viewpoint",
                      inputOutput, "metadata",          SFNode <X3DBaseNode> ::type,
                      inputOnly,   "set_bind",          SFBool::type,
                      inputOutput, "fieldOfView",       SFFloat::type,
                      inputOutput, "jump",              SFBool::type,
                      inputOutput, "retainUserOffsets", SFBool::type,
                      inputOutput, "position",          SFVec3f::type,
                      inputOutput, "orientation",       SFRotation::type,
                      inputOutput, "centerOfRotation",  SFVec3f::type,
                      inputOutput, "description",       SFString::type,
                      outputOnly,  "bindTime",          SFTime::type,
                      outputOnly,  "isBound",           SFBool::type);

*/

Viewpoint::Viewpoint (X3DExecutionContext* const executionContext, bool addToList) :
	X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DViewpointNode (addToList),  
	        position (0, 0, 10),  // SFVec3f [in,out] position           0 0 10        (-∞,∞)
	     fieldOfView (0.785398)   // SFFloat [in,out] fieldOfView        π/4           (0,π)
{
	setComponent ("Navigation");
	setTypeName ("Viewpoint");

	appendField (inputOutput, "metadata",          metadata);
	appendField (inputOutput, "description",       description);
	appendField (inputOnly,   "set_bind",          set_bind);
	appendField (inputOutput, "position",          position);
	appendField (inputOutput, "orientation",       orientation);
	appendField (inputOutput, "centerOfRotation",  centerOfRotation);
	appendField (inputOutput, "fieldOfView",       fieldOfView);
	appendField (inputOutput, "jump",              jump);
	appendField (inputOutput, "retainUserOffsets", retainUserOffsets);
	appendField (outputOnly,  "bindTime",          bindTime);
	appendField (outputOnly,  "isBound",           isBound);
}

X3DBasicNode*
Viewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new Viewpoint (executionContext, true);
}

void
Viewpoint::initialize ()
{
	X3DViewpointNode::initialize ();

	jump .addInterest (this, &Viewpoint::set_jump);

	set_jump ();
}

void
Viewpoint::set_jump ()
{
	if (not jump)
	{
		Matrix4f differenceMatrix;
		differenceMatrix .translate (position);
		differenceMatrix .rotate (orientation);
		setDifferenceMatrix (differenceMatrix);
	}
}

void
Viewpoint::display ()
{
	const Matrix4f & currentMatrix = getCurrentLayer () -> getViewpoint () -> getCurrentMatrix ();
	Matrix4f         matrix        = currentMatrix * ModelViewMatrix4f ();

	setTransformationMatrix (matrix);

	if (jump)
	{
		matrix .translate (position);
		matrix .rotate (orientation);
		setMatrix (matrix);
	}
	else
	{
		setMatrix (matrix * getDifferenceMatrix ());
	}
}

void
Viewpoint::reshape (const float zNear, const float zFar)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	GLfloat viewport [4];
	glGetFloatv (GL_VIEWPORT, viewport);

	GLfloat width  = viewport [2];
	GLfloat height = viewport [3];

	float ratio = std::tan (fieldOfView / 2) * zNear;

	if (width > height)
	{
		float aspect = width * ratio / height;
		glFrustum (-aspect, aspect, -ratio, ratio, zNear, zFar);
	}
	else
	{
		float aspect = height * ratio / width;
		glFrustum (-ratio, ratio, -aspect, aspect, zNear, zFar);
	}

	glMatrixMode (GL_MODELVIEW);
}

} // X3D
} // titania
