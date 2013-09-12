/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_COMPONENTS_NAVIGATION_NAVIGATION_INFO_H__
#define __TITANIA_X3D_COMPONENTS_NAVIGATION_NAVIGATION_INFO_H__

#include "../../Rendering/LightContainer.h"
#include "../../Types/Geometry.h"
#include "../Core/X3DBindableNode.h"
#include "../Lighting/DirectionalLight.h"
#include <memory>

namespace titania {
namespace X3D {

enum class TransitionType {
	TELEPORT,
	LINEAR,
	ANIMATE
};

class NavigationInfo :
	public X3DBindableNode
{
public:

	NavigationInfo (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	MFString &
	type ()
	{ return *fields .type; }

	const MFString &
	type () const
	{ return *fields .type; }

	MFFloat &
	avatarSize ()
	{ return *fields .avatarSize; }

	const MFFloat &
	avatarSize () const
	{ return *fields .avatarSize; }

	SFFloat &
	speed ()
	{ return *fields .speed; }

	const SFFloat &
	speed () const
	{ return *fields .speed; }

	SFBool &
	headlight ()
	{ return *fields .headlight; }

	const SFBool &
	headlight () const
	{ return *fields .headlight; }

	SFFloat &
	visibilityLimit ()
	{ return *fields .visibilityLimit; }

	const SFFloat &
	visibilityLimit () const
	{ return *fields .visibilityLimit; }

	MFString &
	transitionType ()
	{ return *fields .transitionType; }

	const MFString &
	transitionType () const
	{ return *fields .transitionType; }

	SFTime &
	transitionTime ()
	{ return *fields .transitionTime; }

	const SFTime &
	transitionTime () const
	{ return *fields .transitionTime; }

	SFBool &
	transitionStart ()
	{ return fields .transitionStart; }

	const SFBool &
	transitionStart () const
	{ return fields .transitionStart; }

	SFBool &
	transitionComplete ()
	{ return *fields .transitionComplete; }

	const SFBool &
	transitionComplete () const
	{ return *fields .transitionComplete; }

	float
	getCollisionRadius () const;

	float
	getAvatarHeight () const;

	float
	getStepHeight () const;

	float
	getNearPlane () const;

	float
	getFarPlane () const;
	
	TransitionType
	getTransitionType () const;

	void
	transitionStart (NavigationInfo*)
	{ }

	void
	transitionStop ()
	{ }

	void
	enable ();

	void
	disable ();

	virtual
	void
	traverse (const TraverseType);

	virtual
	void
	dispose ();


private:

	virtual
	void
	initialize ();

	void
	set_headlight ();

	virtual
	void
	bindToLayer (X3DLayerNode* const);

	virtual
	void
	unbindFromLayer (X3DLayerNode* const);

	struct Fields
	{
		Fields ();

		MFString* const type;
		MFFloat* const avatarSize;
		SFFloat* const speed;
		SFBool* const headlight;
		SFFloat* const visibilityLimit;
		MFString* const transitionType;
		SFTime* const transitionTime;
		SFBool transitionStart;
		SFBool* const transitionComplete;
	};

	Fields fields;

	X3DSFNode <DirectionalLight>     directionalLight;
	std::unique_ptr <LightContainer> light;

};

} // X3D
} // titania

#endif
