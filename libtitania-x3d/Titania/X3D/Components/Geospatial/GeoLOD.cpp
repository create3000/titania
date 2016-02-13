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

#include "GeoLOD.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Tools/Geospatial/GeoLODTool.h"

namespace titania {
namespace X3D {

const ComponentType GeoLOD::component      = ComponentType::GEOSPATIAL;
const std::string   GeoLOD::typeName       = "GeoLOD";
const std::string   GeoLOD::containerField = "children";

GeoLOD::Fields::Fields () :
	      rootUrl (new MFString ()),
	    child1Url (new MFString ()),
	    child2Url (new MFString ()),
	    child3Url (new MFString ()),
	    child4Url (new MFString ()),
	       center (new SFVec3d ()),
	        range (new SFFloat (10)),
	level_changed (new SFInt32 ()),
	     rootNode (new MFNode ()),
	     children (new MFNode ())
{ }

GeoLOD::GeoLOD (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	       X3DChildNode (),
	   X3DBoundedObject (),
	X3DGeospatialObject (),
	             fields (),
	          rootGroup (new Group (getBrowser () -> getPrivateScene ())),
	         rootInline (new Inline (executionContext)),
	       child1Inline (new Inline (executionContext)),
	       child2Inline (new Inline (executionContext)),
	       child3Inline (new Inline (executionContext)),
	       child4Inline (new Inline (executionContext)),
	   keepCurrentLevel (false)
{
	addType (X3DConstants::GeoLOD);

	addField (inputOutput,    "metadata",      metadata ());
	addField (initializeOnly, "geoSystem",     geoSystem ());
	addField (initializeOnly, "rootUrl",       rootUrl ());
	addField (initializeOnly, "child1Url",     child1Url ());
	addField (initializeOnly, "child2Url",     child2Url ());
	addField (initializeOnly, "child3Url",     child3Url ());
	addField (initializeOnly, "child4Url",     child4Url ());
	addField (initializeOnly, "center",        center ());
	addField (initializeOnly, "range",         range ());
	addField (outputOnly,     "level_changed", level_changed ());
	addField (initializeOnly, "geoOrigin",     geoOrigin ());
	addField (initializeOnly, "rootNode",      rootNode ());
	addField (initializeOnly, "bboxSize",      bboxSize ());
	addField (initializeOnly, "bboxCenter",    bboxCenter ());
	addField (outputOnly,     "children",      children ());

	center () .setUnit (UnitCategory::LENGTH);
	range ()  .setUnit (UnitCategory::LENGTH);
		
	center () .isGeospatial (true);

	addChildren (rootGroup, rootInline, child1Inline, child2Inline, child3Inline, child4Inline);
}

X3DBaseNode*
GeoLOD::create (X3DExecutionContext* const executionContext) const
{
	return new GeoLOD (executionContext);
}

void
GeoLOD::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();
	X3DGeospatialObject::initialize ();

	rootNode () .addInterest (rootGroup -> children ());

	rootGroup -> isPrivate (true);
	rootGroup -> children () = rootNode ();
	rootGroup -> setup ();

	rootInline   -> checkLoadState () .addInterest (this, &GeoLOD::set_rootLoadState);
	child1Inline -> checkLoadState () .addInterest (this, &GeoLOD::set_childLoadState);
	child2Inline -> checkLoadState () .addInterest (this, &GeoLOD::set_childLoadState);
	child3Inline -> checkLoadState () .addInterest (this, &GeoLOD::set_childLoadState);
	child4Inline -> checkLoadState () .addInterest (this, &GeoLOD::set_childLoadState);

	rootUrl ()   .addInterest (rootInline -> url ());
	child1Url () .addInterest (child1Inline -> url ());
	child2Url () .addInterest (child2Inline -> url ());
	child3Url () .addInterest (child3Inline -> url ());
	child4Url () .addInterest (child4Inline -> url ());

	rootInline   -> load () = true;
	child1Inline -> load () = false;
	child2Inline -> load () = false;
	child3Inline -> load () = false;
	child4Inline -> load () = false;

	rootInline   -> url () = rootUrl ();
	child1Inline -> url () = child1Url ();
	child2Inline -> url () = child2Url ();
	child3Inline -> url () = child3Url ();
	child4Inline -> url () = child4Url ();

	rootInline   -> setup ();
	child1Inline -> setup ();
	child2Inline -> setup ();
	child3Inline -> setup ();
	child4Inline -> setup ();
}

void
GeoLOD::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	rootGroup    -> setExecutionContext (executionContext -> getBrowser () -> getPrivateScene ());
	rootInline   -> setExecutionContext (executionContext);
	child1Inline -> setExecutionContext (executionContext);
	child2Inline -> setExecutionContext (executionContext);
	child3Inline -> setExecutionContext (executionContext);
	child4Inline -> setExecutionContext (executionContext);

	X3DChildNode::setExecutionContext (executionContext);
}

Box3d
GeoLOD::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		const size_t level = level_changed ();

		switch (level)
		{
			case 0:
			{
				if (rootNode () .empty ())
					return rootInline -> getBBox ();

				return rootGroup -> getBBox ();
			}
			case 1:
			{
				Box3d bbox;
				
				bbox += child1Inline -> getBBox ();
				bbox += child2Inline -> getBBox ();
				bbox += child3Inline -> getBBox ();
				bbox += child4Inline -> getBBox ();

				return bbox;
			}
			default:
				return Box3d ();
		}
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

void
GeoLOD::set_rootLoadState ()
{
	if (level_changed () not_eq 0)
		return;

	if (not rootNode () .empty ())
		return;

	if (rootInline -> checkLoadState () == COMPLETE_STATE)
		children () = rootInline -> getRootNodes ();
}

void
GeoLOD::set_childLoadState ()
{
	if (level_changed () not_eq 1)
		return;

	children () .clear ();

	if (child1Inline -> checkLoadState () == COMPLETE_STATE)
		children () .insert (children () .end (),
		                     child1Inline -> getRootNodes () .begin (),
		                     child1Inline -> getRootNodes () .end ());

	if (child2Inline -> checkLoadState () == COMPLETE_STATE)
		children () .insert (children () .end (),
		                     child2Inline -> getRootNodes () .begin (),
		                     child2Inline -> getRootNodes () .end ());

	if (child3Inline -> checkLoadState () == COMPLETE_STATE)
		children () .insert (children () .end (),
		                     child3Inline -> getRootNodes () .begin (),
		                     child3Inline -> getRootNodes () .end ());

	if (child4Inline -> checkLoadState () == COMPLETE_STATE)
		children () .insert (children () .end (),
		                     child4Inline -> getRootNodes () .begin (),
		                     child4Inline -> getRootNodes () .end ());
}

size_t
GeoLOD::getLevel (const TraverseType type) const
{
	const double distance = getDistance (type);

	if (distance < range ())
		return 1;

	return 0;
}

double
GeoLOD::getDistance (const TraverseType type) const
{
	Matrix4d modelViewMatrix = getModelViewMatrix (type);

	modelViewMatrix .translate (getCoord (center ()));

	return math::abs (modelViewMatrix .origin ());
}

void
GeoLOD::traverse (const TraverseType type)
{
	const int32_t level = getLevel (type);

	if (level not_eq level_changed ())
	{
		level_changed () = level;

		switch (level)
		{
			case 0:
			{
				if (rootNode () .empty ())
				{
					if (rootInline -> checkLoadState () == COMPLETE_STATE)
						children () = rootInline -> getRootNodes ();
				}
				else
					children () = rootNode ();

				child1Inline -> load () = false;
				child2Inline -> load () = false;
				child3Inline -> load () = false;
				child4Inline -> load () = false;

				break;
			}
			case 1:
			{
				child1Inline -> load () = true;
				child2Inline -> load () = true;
				child3Inline -> load () = true;
				child4Inline -> load () = true;

				break;
			}
		}
	}

	switch (level)
	{
		case 0:
		{
			if (rootNode () .empty ())
				rootInline -> traverse (type);
			else
				rootGroup -> traverse (type);

			break;
		}
		case 1:
		{
			child1Inline -> traverse (type);
			child2Inline -> traverse (type);
			child3Inline -> traverse (type);
			child4Inline -> traverse (type);
			break;
		}
	}
}

void
GeoLOD::addTool ()
{
	X3DChildNode::addTool (new GeoLODTool (this));
}

void
GeoLOD::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
