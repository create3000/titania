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

#ifndef __TITANIA_X3D_COMPONENTS_LAYERING_LAYER_SET_H__
#define __TITANIA_X3D_COMPONENTS_LAYERING_LAYER_SET_H__

#include "../Core/X3DNode.h"

namespace titania {
namespace X3D {

class X3DLayerNode;

class LayerSet :
	virtual public X3DNode
{
public:

	///  @name Construction

	LayerSet (X3DExecutionContext* const executionContext);

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

	SFInt32 &
	privateActiveLayer ()
	{ return fields .privateActiveLayer; }

	const SFInt32 &
	privateActiveLayer () const
	{ return fields .privateActiveLayer; }

	SFInt32 &
	activeLayer ()
	{ return *fields .activeLayer; }

	const SFInt32 &
	activeLayer () const
	{ return *fields .activeLayer; }

	MFInt32 &
	order ()
	{ return *fields .order; }

	const MFInt32 &
	order () const
	{ return *fields .order; }

	MFNode &
	layers ()
	{ return *fields .layers; }

	const MFNode &
	layers () const
	{ return *fields .layers; }

	///  @name Member access

	int32_t
	getActiveLayerIndex () const;

	void
	setLayer0 (const X3DPtr <X3DLayerNode> &);

	const X3DPtr <X3DLayerNode> &
	getLayer0 () const
	{ return layerNode0; }

	const X3DPtr <X3DLayerNode> &
	getActiveLayer () const
	{ return activeLayerNode; }

	const X3DPtrArray <X3DLayerNode> &
	getLayers () const
	{ return layerNodes; }

	///  @name Operations

	void
	bind (const std::string & viewpointName);

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	virtual
	void
	dispose () final override;

	virtual
	~LayerSet () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_activeLayer ();

	void
	set_layers ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFInt32 privateActiveLayer;
		SFInt32* const activeLayer;
		MFInt32* const order;
		MFNode* const layers;
	};

	Fields fields;

	X3DPtrArray <X3DLayerNode> layerNodes;
	X3DPtr <X3DLayerNode>      layerNode0;
	X3DPtr <X3DLayerNode>      activeLayerNode;

};

} // X3D
} // titania

#endif
