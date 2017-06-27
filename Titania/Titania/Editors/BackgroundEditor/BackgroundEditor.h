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

#ifndef __TITANIA_EDITORS_BACKGROUND_EDITOR_BACKGROUND_EDITOR_H__
#define __TITANIA_EDITORS_BACKGROUND_EDITOR_BACKGROUND_EDITOR_H__

#include "../../UserInterfaces/X3DBackgroundEditorInterface.h"
#include "X3DBackgroundEditor.h"

#include "../../ComposedWidgets.h"
#include "../../ComposedWidgets/MFColorButton.h"
#include "../../ComposedWidgets/BackgroundTool.h"
#include "../../Editors/BindableNodeList/BackgroundList.h"

#include <Titania/X3D/Components/Texturing/X3DTextureNode.h>

namespace titania {
namespace puck {

class TexturePreview;
class BackgroundTool;

class BackgroundEditor :
	virtual public X3DBackgroundEditorInterface,
	public X3DBackgroundEditor
{
public:

	///  @name Construction

	BackgroundEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~BackgroundEditor () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	void
	set_background (const X3D::X3DPtr <X3D::X3DBackgroundNode> &);

	///  @name Event handlers

	void
	set_texture (const std::shared_ptr <TexturePreview> &, const X3D::X3DPtr <X3D::X3DTextureNode> &);

	void
	on_sky_index_changed ();
	
	void
	on_sky_color_index_changed ();

	void
	on_ground_index_changed ();
	
	void
	on_ground_color_index_changed ();

	// Action buttons

	virtual
	void
	on_new_background_popup_clicked () final override;

	virtual
	void
	on_new_background_clicked () final override;

	virtual
	void
	on_new_texture_background_clicked () final override;

	virtual
	void
	on_remove_background_clicked () final override;

	virtual
	void
	store () final override;

	///  @name Members

	std::unique_ptr <BackgroundList>     backgroundList;
	NameEntry                            nodeName;
	BackgroundTool                       sky;
	MFColorButton                        skyColor;
	X3DFieldAdjustment <X3D::MFFloat>    skyAngle;
	BackgroundTool                       ground;
	MFColorButton                        groundColor;
	X3DFieldAdjustment <X3D::MFFloat>    groundAngle;
	X3DFieldAdjustment <X3D::SFFloat>    transparency;
	std::shared_ptr <TexturePreview>     frontPreview;
	std::shared_ptr <TexturePreview>     backPreview;
	std::shared_ptr <TexturePreview>     leftPreview;
	std::shared_ptr <TexturePreview>     rightPreview;
	std::shared_ptr <TexturePreview>     topPreview;
	std::shared_ptr <TexturePreview>     bottomPreview;
	X3D::X3DPtr <X3D::X3DBackgroundNode> backgroundNode;
	bool                                 changing;

};

} // puck
} // titania

#endif
