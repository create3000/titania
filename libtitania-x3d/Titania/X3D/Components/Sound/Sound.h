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

#ifndef __TITANIA_X3D_COMPONENTS_SOUND_SOUND_H__
#define __TITANIA_X3D_COMPONENTS_SOUND_SOUND_H__

#include "../../Browser/Sound/X3DMediaStream.h"

#include "../Sound/X3DSoundNode.h"

namespace titania {
namespace X3D {

class X3DSoundSourceNode;

class Sound :
	virtual public X3DSoundNode
{
public:

	///  @name Construction

	Sound (X3DExecutionContext* const);

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
	SFFloat &
	intensity ()
	{ return *fields .intensity; }

	virtual
	const SFFloat &
	intensity () const
	{ return *fields .intensity; }

	virtual
	SFBool &
	spatialize ()
	{ return *fields .spatialize; }

	virtual
	const SFBool &
	spatialize () const
	{ return *fields .spatialize; }

	virtual
	SFVec3f &
	location ()
	{ return *fields .location; }

	virtual
	const SFVec3f &
	location () const
	{ return *fields .location; }

	virtual
	SFVec3f &
	direction ()
	{ return *fields .direction; }

	virtual
	const SFVec3f &
	direction () const
	{ return *fields .direction; }

	virtual
	SFFloat &
	minBack ()
	{ return *fields .minBack; }

	virtual
	const SFFloat &
	minBack () const
	{ return *fields .minBack; }

	virtual
	SFFloat &
	minFront ()
	{ return *fields .minFront; }

	virtual
	const SFFloat &
	minFront () const
	{ return *fields .minFront; }

	virtual
	SFFloat &
	maxBack ()
	{ return *fields .maxBack; }

	virtual
	const SFFloat &
	maxBack () const
	{ return *fields .maxBack; }

	virtual
	SFFloat &
	maxFront ()
	{ return *fields .maxFront; }

	virtual
	const SFFloat &
	maxFront () const
	{ return *fields .maxFront; }

	virtual
	SFFloat &
	priority ()
	{ return *fields .priority; }

	virtual
	const SFFloat &
	priority () const
	{ return *fields .priority; }

	virtual
	SFNode &
	source ()
	{ return *fields .source; }

	virtual
	const SFNode &
	source () const
	{ return *fields .source; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	virtual
	void
	addTool () override;

	///  @name Construction

	virtual
	~Sound ();


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_source ();

	///  @name Operations

	void
	getEllipsoidParameter (Matrix4d modelViewMatrix, const float & back, const float & front, float & radius, float & distance)
	throw (std::domain_error);

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFFloat* const intensity;
		SFBool* const spatialize;
		SFVec3f* const location;
		SFVec3f* const direction;
		SFFloat* const minBack;
		SFFloat* const minFront;
		SFFloat* const maxBack;
		SFFloat* const maxFront;
		SFFloat* const priority;
		SFNode* const source;
	};

	Fields fields;

	X3DPtr <X3DSoundSourceNode> sourceNode;

};

} // X3D
} // titania

#endif
