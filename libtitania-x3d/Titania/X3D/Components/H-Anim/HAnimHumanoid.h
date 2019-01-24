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

#ifndef __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_HUMANOID_H__
#define __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_HUMANOID_H__

#include "../Core/X3DChildNode.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

class HAnimHumanoid :
	virtual public X3DChildNode,
	virtual public X3DBoundedObject
{
public:

	///  @name Construction

	HAnimHumanoid (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	virtual
	SFString &
	name ()
	{ return *fields .name; }

	virtual
	const SFString &
	name () const
	{ return *fields .name; }

	virtual
	SFString &
	version ()
	{ return *fields .version; }

	virtual
	const SFString &
	version () const
	{ return *fields .version; }

	virtual
	MFString &
	info ()
	{ return *fields .info; }

	virtual
	const MFString &
	info () const
	{ return *fields .info; }

	virtual
	SFVec3f &
	translation ()
	{ return *fields .translation; }

	virtual
	const SFVec3f &
	translation () const
	{ return *fields .translation; }

	virtual
	SFRotation &
	rotation ()
	{ return *fields .rotation; }

	virtual
	const SFRotation &
	rotation () const
	{ return *fields .rotation; }

	virtual
	SFVec3f &
	scale ()
	{ return *fields .scale; }

	virtual
	const SFVec3f &
	scale () const
	{ return *fields .scale; }

	virtual
	SFRotation &
	scaleOrientation ()
	{ return *fields .scaleOrientation; }

	virtual
	const SFRotation &
	scaleOrientation () const
	{ return *fields .scaleOrientation; }

	virtual
	SFVec3f &
	center ()
	{ return *fields .center; }

	virtual
	const SFVec3f &
	center () const
	{ return *fields .center; }

	virtual
	MFNode &
	viewpoints ()
	{ return *fields .viewpoints; }

	virtual
	const MFNode &
	viewpoints () const
	{ return *fields .viewpoints; }

	virtual
	MFNode &
	sites ()
	{ return *fields .sites; }

	virtual
	const MFNode &
	sites () const
	{ return *fields .sites; }

	virtual
	MFNode &
	joints ()
	{ return *fields .joints; }

	virtual
	const MFNode &
	joints () const
	{ return *fields .joints; }

	virtual
	MFNode &
	segments ()
	{ return *fields .segments; }

	virtual
	const MFNode &
	segments () const
	{ return *fields .segments; }

	virtual
	MFNode &
	skeleton ()
	{ return *fields .skeleton; }

	virtual
	const MFNode &
	skeleton () const
	{ return *fields .skeleton; }

	virtual
	SFNode &
	skinNormal ()
	{ return *fields .skinNormal; }

	virtual
	const SFNode &
	skinNormal () const
	{ return *fields .skinNormal; }

	virtual
	SFNode &
	skinCoord ()
	{ return *fields .skinCoord; }

	virtual
	const SFNode &
	skinCoord () const
	{ return *fields .skinCoord; }

	virtual
	MFNode &
	skin ()
	{ return *fields .skin; }

	virtual
	const MFNode &
	skin () const
	{ return *fields .skin; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const override;

	virtual
	const X3DPtr <Transform> &
	getTransform () const
	{ return transformNode; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Input/Output

	///  Inserts this object into @a ostream in VRML Classic Encoding.
	virtual
	void
	toStream (std::ostream & ostream) const override;

	///  Inserts this object into @a ostream in X3D XML Encoding.
	virtual
	void
	toXMLStream (std::ostream & ostream) const override;

	///  Inserts this object into @a ostream in X3D JSON Encoding.
	virtual
	void
	toJSONStream (std::ostream & ostream) const override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~HAnimHumanoid () override;


protected:

	///  @name Contruction

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_joints ();
	
	void
	set_skinNormal ();
	
	void
	set_skinCoord ();
	
	///  @name Operations

	void
	skinning (const TraverseType type, X3DRenderObject* const renderObject);

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFString* const name;
		SFString* const version;
		MFString* const info;
		SFVec3f* const translation;
		SFRotation* const rotation;
		SFVec3f* const scale;
		SFRotation* const scaleOrientation;
		SFVec3f* const center;
		MFNode* const viewpoints;
		MFNode* const sites;
		MFNode* const joints;
		MFNode* const segments;
		MFNode* const skeleton;
		SFNode* const skinNormal;
		SFNode* const skinCoord;
		MFNode* const skin;
	};

	Fields fields;

	///  @name Members

	X3DPtr <Group>             viewpointsNode;
	X3DPtr <Group>             skeletonNode;
	X3DPtr <Group>             skinNode;
	X3DPtr <Transform>         transformNode;
	X3DPtrArray <HAnimJoint>   jointNodes;
	X3DPtr <X3DNormalNode>     skinNormalNode;
	X3DPtr <X3DCoordinateNode> skinCoordNode;
	X3DPtr <X3DNormalNode>     normalNode;
	X3DPtr <X3DCoordinateNode> coordNode;
};

} // X3D
} // titania

#endif
