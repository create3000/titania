/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_X3DBROWSER_VIEW_H__
#define __TITANIA_BROWSER_NOTEBOOK_BROWSER_VIEW_X3DBROWSER_VIEW_H__

#include "../../UserInterfaces/X3DBrowserPanelInterface.h"

#include "BrowserPanelType.h"

namespace titania {
namespace puck {

class NotebookPage;

class X3DBrowserPanel :
	virtual public X3DBrowserPanelInterface
{
public:

	///  @name Member access

	const X3D::BrowserPtr &
	getLocalBrowser () const
	{ return browser; }

	///  @name Operations

	void
	setBrowserRatio (const bool set, const double ratio);

	virtual
	void
	lookAtSelection () final override;

	virtual
	void
	lookAtAll () final override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DBrowserPanel () override;


protected:

	///  @name Construction

	X3DBrowserPanel ();

	virtual
	void
	initialize () override;

	///  @name Member access

	void
	setType (const BrowserPanelType value);

	BrowserPanelType
	getType () const;

	virtual
	void
	setLocalBrowser (const X3D::BrowserPtr & value);

	const X3D::X3DPtr <X3D::X3DLayerNode> &
	getLayer () const
	{ return layerNode; }

	void
	setShading (const size_t id, const std::string & shading);
	
	std::string
	getShading (const size_t id) const;

	///  @name Event handlers

	virtual
	void
	on_map () override;

	virtual
	void
	on_unmap () override;


private:

	///  @name Construction

	X3D::BrowserPtr
	createBrowser (const BrowserPanelType type);

	///  @name Member access

	void
	setBrowserPanelType (const size_t id, const BrowserPanelType browserPanelType);
	
	BrowserPanelType
	getBrowserPanelType (const size_t id) const;

	int32_t
	getPlane () const;

	void
	setLayer (const int32_t layerNumber);

	void
	setLayer (const X3D::X3DPtr <X3D::X3DLayerNode> & layer);

	///  @name Event handlers

	void
	set_initialized ();

	void
	set_type ();

	void
	set_dependent_browser ();

	void
	set_fixed_pipeline ();

	void
	set_viewer ();

	void
	set_background_texture ();

	void
	set_background_texture_transparency ();

	void
	set_live ();

	void
	set_execution_context ();

	void
	set_navigationInfoStack ();

	void
	set_viewpoint ();

	void
	connectViewpoint ();

	void
	set_grid ();

	///  @name Static members

	static const std::vector <BrowserPanelType>                defaultTypes;
	static const std::map <BrowserPanelType, std::string>      names;
	static const std::map <BrowserPanelType, X3D::Vector3d>    axes;
	static const std::map <BrowserPanelType, X3D::Vector3d>    positions;
	static const std::map <BrowserPanelType, X3D::Rotation4d>  orientations;

	///  @name Members

	BrowserPanelType                    type;
	X3D::BrowserPtr                     browser;
	X3D::X3DPtr <X3D::X3DLayerNode>     layerNode;
	X3D::X3DPtr <X3D::NavigationInfo>   navigationInfoNode;
	X3D::X3DPtr <X3D::X3DViewpointNode> viewpoint;
	X3D::X3DPtr <X3D::GridTool>         gridTool;

	Gtk::AspectFrame* aspectFrame;
};

} // puck
} // titania

#endif
