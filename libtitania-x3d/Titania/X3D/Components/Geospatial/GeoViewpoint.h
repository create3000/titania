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

#ifndef __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_VIEWPOINT_H__
#define __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_VIEWPOINT_H__

#include "../Geospatial/X3DGeospatialObject.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

class GeoViewpoint :
	virtual public X3DViewpointNode,
	virtual public X3DGeospatialObject
{
public:

	///  @name Construction

	GeoViewpoint (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
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

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields

	virtual
	SFVec3d &
	position ()
	{ return *fields .position; }

	virtual
	const SFVec3d &
	position () const
	{ return *fields .position; }

	virtual
	SFVec3d &
	centerOfRotation ()
	{ return *fields .centerOfRotation; }

	virtual
	const SFVec3d &
	centerOfRotation () const
	{ return *fields .centerOfRotation; }

	virtual
	SFFloat &
	fieldOfView ()
	{ return *fields .fieldOfView; }

	virtual
	const SFFloat &
	fieldOfView () const
	{ return *fields .fieldOfView; }

	virtual
	SFFloat &
	speedFactor ()
	{ return *fields .speedFactor; }

	virtual
	const SFFloat &
	speedFactor () const
	{ return *fields .speedFactor; }

	///  @name Member access

	virtual
	void
	setPosition (const Vector3d &) final override;

	virtual
	Vector3d
	getPosition () const final override;

	virtual
	void
	setOrientation (const Rotation4d &) final override;

	virtual
	Rotation4d
	getOrientation () const final override;

	virtual
	void
	setCenterOfRotation (const Vector3d & value) final override;

	virtual
	Vector3d
	getCenterOfRotation () const final override;

	virtual
	Vector3d
	getUpVector () const final override;

	virtual
	double
	getMaxFarValue () const final override;

	virtual
	double
	getSpeedFactor () const final override;

	virtual
	Vector3d
	getScreenScale (const Vector3d & point, const Vector4i & viewport) const override;

	virtual
	Vector2d
	getViewportSize (const Vector4i & viewport, const double nearValue) const override;

	virtual
	Matrix4d
	getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool limit = false) const override;

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Depeciated fields

	MFString &
	navType ()
	{ return *fields .navType; }

	const MFString &
	navType () const
	{ return *fields .navType; }

	SFBool &
	headlight ()
	{ return *fields .headlight; }

	const SFBool &
	headlight () const
	{ return *fields .headlight; }

	///  @name Event handlers

	void
	set_position ();

	///  @name Operations

	double
	getFieldOfView () const;

	virtual
	std::pair <double, double>
	getLookAtDistance (const Box3d &) const final override;

	virtual
	void
	bindToLayer (X3DLayerNode* const) final override;

	virtual
	void
	unbindFromLayer (X3DLayerNode* const) final override;

	virtual
	void
	removeFromLayer (X3DLayerNode* const) final override;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3d* const position;
		SFVec3d* const centerOfRotation;
		SFFloat* const fieldOfView;
		SFFloat* const speedFactor;

		MFString* navType;
		SFBool* headlight;
	};

	Fields fields;

	Vector3d                coord;
	double                  elevation;
	X3DPtr <NavigationInfo> navigationInfoNode;

};

} // X3D
} // titania

#endif
