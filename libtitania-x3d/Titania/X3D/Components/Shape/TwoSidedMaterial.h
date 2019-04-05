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

#ifndef __TITANIA_X3D_COMPONENTS_SHAPE_TWO_SIDED_MATERIAL_H__
#define __TITANIA_X3D_COMPONENTS_SHAPE_TWO_SIDED_MATERIAL_H__

#include "../Shape/X3DMaterialNode.h"

namespace titania {
namespace X3D {

class TwoSidedMaterial :
	public X3DMaterialNode
{
public:

	///  @name Construction

	TwoSidedMaterial (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFBool &
	separateBackColor ()
	{ return *fields .separateBackColor; }

	const SFBool &
	separateBackColor () const
	{ return *fields .separateBackColor; }

	SFFloat &
	ambientIntensity ()
	{ return *fields .ambientIntensity; }

	const SFFloat &
	ambientIntensity () const
	{ return *fields .ambientIntensity; }

	SFColor &
	diffuseColor ()
	{ return *fields .diffuseColor; }

	const SFColor &
	diffuseColor () const
	{ return *fields .diffuseColor; }

	SFColor &
	specularColor ()
	{ return *fields .specularColor; }

	const SFColor &
	specularColor () const
	{ return *fields .specularColor; }

	SFColor &
	emissiveColor ()
	{ return *fields .emissiveColor; }

	const SFColor &
	emissiveColor () const
	{ return *fields .emissiveColor; }

	SFFloat &
	shininess ()
	{ return *fields .shininess; }

	const SFFloat &
	shininess () const
	{ return *fields .shininess; }

	SFFloat &
	transparency ()
	{ return *fields .transparency; }

	const SFFloat &
	transparency () const
	{ return *fields .transparency; }

	SFFloat &
	backAmbientIntensity ()
	{ return *fields .backAmbientIntensity; }

	const SFFloat &
	backAmbientIntensity () const
	{ return *fields .backAmbientIntensity; }

	SFColor &
	backDiffuseColor ()
	{ return *fields .backDiffuseColor; }

	const SFColor &
	backDiffuseColor () const
	{ return *fields .backDiffuseColor; }

	SFColor &
	backSpecularColor ()
	{ return *fields .backSpecularColor; }

	const SFColor &
	backSpecularColor () const
	{ return *fields .backSpecularColor; }

	SFColor &
	backEmissiveColor ()
	{ return *fields .backEmissiveColor; }

	const SFColor &
	backEmissiveColor () const
	{ return *fields .backEmissiveColor; }

	SFFloat &
	backShininess ()
	{ return *fields .backShininess; }

	const SFFloat &
	backShininess () const
	{ return *fields .backShininess; }

	SFFloat &
	backTransparency ()
	{ return *fields .backTransparency; }

	const SFFloat &
	backTransparency () const
	{ return *fields .backTransparency; }

	///  @name Operations

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const) const final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_transparency ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const separateBackColor;
		SFFloat* const ambientIntensity;
		SFColor* const diffuseColor;
		SFColor* const specularColor;
		SFColor* const emissiveColor;
		SFFloat* const shininess;
		SFFloat* const transparency;
		SFFloat* const backAmbientIntensity;
		SFColor* const backDiffuseColor;
		SFColor* const backSpecularColor;
		SFColor* const backEmissiveColor;
		SFFloat* const backShininess;
		SFFloat* const backTransparency;
	};

	Fields fields;

	GLfloat glAmbientColor [4];
	GLfloat glDiffuseColor [4];
	GLfloat glSpecularColor [4];
	GLfloat glEmissiveColor [4];
	GLfloat glShininess;

	GLfloat glBackAmbientColor [4];
	GLfloat glBackDiffuseColor [4];
	GLfloat glBackSpecularColor [4];
	GLfloat glBackEmissiveColor [4];
	GLfloat glBackShininess;

};

} // X3D
} // titania

#endif
