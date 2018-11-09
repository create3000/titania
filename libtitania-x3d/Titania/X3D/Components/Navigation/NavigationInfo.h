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

#ifndef __TITANIA_X3D_COMPONENTS_NAVIGATION_NAVIGATION_INFO_H__
#define __TITANIA_X3D_COMPONENTS_NAVIGATION_NAVIGATION_INFO_H__

#include "../../Types/Geometry.h"
#include "../Core/X3DBindableNode.h"

#include <memory>

namespace titania {
namespace X3D {

class LightContainer;

enum class TransitionType :
	uint8_t
{
	TELEPORT,
	LINEAR,
	ANIMATE

};

class NavigationInfo :
	virtual public X3DBindableNode
{
public:

	///  @name Construction

	NavigationInfo (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

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

	SFBool &
	transitionActive ()
	{ return fields .transitionActive; }

	const SFBool &
	transitionActive () const
	{ return fields .transitionActive; }

	///  @name Member access

	const SFEnum <X3DConstants::NodeType> &
	getViewer () const
	{ return viewer; }

	const MFEnum <X3DConstants::NodeType> &
	getAvailableViewers () const
	{ return availableViewers; }

	double
	getCollisionRadius () const;

	double
	getAvatarHeight () const;

	double
	getStepHeight () const;

	double
	getNearValue () const;

	double
	getFarValue (const X3DViewpointNode* const) const;

	TransitionType
	getTransitionType () const;

	///  @name Operations

	virtual
	void
	bindToLayer (X3DLayerNode* const) final override;

	virtual
	void
	unbindFromLayer (X3DLayerNode* const) final override;

	virtual
	void
	removeFromLayer (X3DLayerNode* const) final override;

	void
	transitionStart (NavigationInfo* const)
	{ }

	void
	transitionStop ()
	{ }

	void
	enable (const TraverseType type, X3DRenderObject* const renderObject);

	void
	disable ();

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	///  @name Destruction

	virtual
	~NavigationInfo () final override;


private:

	///  @name Event handlers

	void
	initialize ();

	void
	set_type ();

	void
	set_transitionStart ();

	void
	set_transitionComplete ();

	void
	set_isBound ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

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
		SFBool* const transitionComplete;
		SFBool transitionStart;
		SFBool transitionActive;
	};

	Fields fields;

	SFEnum <X3DConstants::NodeType> viewer;
	MFEnum <X3DConstants::NodeType> availableViewers;

};

} // X3D
} // titania

#endif
