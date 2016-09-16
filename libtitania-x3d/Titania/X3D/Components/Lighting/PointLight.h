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

#ifndef __TITANIA_X3D_COMPONENTS_LIGHTING_POINT_LIGHT_H__
#define __TITANIA_X3D_COMPONENTS_LIGHTING_POINT_LIGHT_H__

#include "../Lighting/X3DLightNode.h"

namespace titania {
namespace X3D {

class PointLight :
	virtual public X3DLightNode
{
public:

	///  @name Construction

	PointLight (X3DExecutionContext* const);

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

	///  @name Fields

	virtual
	SFVec3f &
	attenuation ()
	{ return *fields .attenuation; }

	virtual
	const SFVec3f &
	attenuation () const
	{ return *fields .attenuation; }

	virtual
	SFVec3f &
	location ()
	{ return *fields .location; }

	virtual
	const SFVec3f &
	location () const
	{ return *fields .location; }

	virtual
	SFFloat &
	radius ()
	{ return *fields .radius; }

	virtual
	const SFFloat &
	radius () const
	{ return *fields .radius; }

	///  @name Member access

	virtual
	float
	getRadius () const;

	virtual
	size_t
	getShadowMapSize () const;

	///  @name Operations

	virtual
	void
	draw (const GLenum) override;

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const, const size_t, const Matrix4d &) final override;

	virtual
	void
	addTool () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;

	///  @name Operations

	virtual
	bool
	renderShadowMap (LightContainer* const) final override;


private:

	static
	constexpr Matrix4d
	getBiasMatrix ()
	{
		return Matrix4d (1.0 / 6.0, 0.0,  0.0, 0.0,
		                 0.0,  0.25, 0.0, 0.0,
		                 0.0,  0.0,  0.5, 0.0,
		                 1.0 / 6.0, 0.25, 0.5, 1.0);
	}

	///  @name Operations

	double
	getFarValue (const Box3d & box, const Vector3d &) const;

	///  @name Event handlers

	void
	eventsProcessed ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3f* const attenuation;
		SFVec3f* const location;
		SFFloat* const radius;
	};

	Fields fields;

	GLfloat glAmbient [4];
	GLfloat glDiffuseSpecular [4];
	GLfloat glPosition [4];

};

} // X3D
} // titania

#endif
