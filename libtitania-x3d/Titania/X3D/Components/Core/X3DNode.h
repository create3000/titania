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

#ifndef __TITANIA_X3D_COMPONENTS_CORE_X3DNODE_H__
#define __TITANIA_X3D_COMPONENTS_CORE_X3DNODE_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/X3DConstants.h"
#include "../../Fields.h"

#include "../../Rendering/OpenGL.h"
#include "../../Types/MatrixStack.h"

namespace titania {
namespace X3D {

class MetadataSet;
class X3DScene;
class X3DLayerNode;
class NavigationInfo;
class X3DViewpointNode;

class X3DNode :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	virtual
	SFNode &
	metadata ()
	{ return *fields .metadata; }

	virtual
	const SFNode &
	metadata () const
	{ return *fields .metadata; }

	///  @name Metadata handling

	void
	setBoolean (const std::string &, const bool)
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	bool
	getBoolean (const std::string &, const bool = false) const
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	void
	setDouble (const std::string &, const double)
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	double
	getDouble (const std::string &, const double = 0) const
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	void
	setFloat (const std::string &, const float)
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	float
	getFloat (const std::string &, const float = 0) const
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	void
	setInteger (const std::string &, const int32_t)
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	int32_t
	getInteger (const std::string &, const int32_t = 0) const
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	void
	setString (const std::string &, const std::string &)
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	const std::string &
	getString (const std::string &, const std::string & = "") const
	throw (Error <INVALID_NAME>,
          Error <DISPOSED>);

	void
	removeMetaData (const std::string & = "")
	throw (Error <DISPOSED>);

	///  @name Member access

	std::vector <X3DLayerNode*>
	getLayers () const;

	Matrix4fStack &
	getModelViewMatrix ()
	{ return modelViewMatrix; }

	const Matrix4fStack &
	getModelViewMatrix () const
	{ return modelViewMatrix; }


protected:

	///  @name Construction

	X3DNode ();

	///  @name Member access

	X3DLayerNode*
	getCurrentLayer () const;

	NavigationInfo*
	getCurrentNavigationInfo () const;

	X3DViewpointNode*
	getCurrentViewpoint () const;

	const Matrix4f &
	getCameraSpaceMatrix () const;

	const Matrix4f &
	getInverseCameraSpaceMatrix () const;

	Matrix4f
	getModelViewMatrix (const TraverseType) const;


private:


	///  @name Member access

	MetadataSet*
	getMetadataSet (const std::deque <std::string> &, const bool = false) const
	throw (Error <INVALID_NAME>,
	       Error <DISPOSED>);

	///  @name Static members

	static Matrix4fStack modelViewMatrix;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const metadata;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
