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

#ifndef __TITANIA_EDITORS_APPEARANCE_EDITOR_APPEARANCE_EDITOR_H__
#define __TITANIA_EDITORS_APPEARANCE_EDITOR_APPEARANCE_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "X3DMaterialEditor.h"
#include "X3DPointPropertiesEditor.h"
#include "X3DLinePropertiesEditor.h"
#include "X3DFillPropertiesEditor.h"
#include "X3DBlendModeEditor.h"
#include "X3DUsedMaterialsEditor.h"
#include "X3DMaterialPaletteEditor.h"

namespace titania {
namespace puck {

class AppearanceEditor :
	virtual public X3DAppearanceEditorInterface,
	public X3DMaterialEditor,
	public X3DPointPropertiesEditor,
	public X3DLinePropertiesEditor,
	public X3DFillPropertiesEditor,
	public X3DBlendModeEditor,
	public X3DUsedMaterialsEditor,
	public X3DMaterialPaletteEditor
{
public:

	///  @name Construction

	AppearanceEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~AppearanceEditor () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	virtual
	void
	set_selection (const X3D::MFNode & selection) final override;

	///  @name Member access

	virtual
	const X3D::BrowserPtr &
	getPreview () const final override
	{ return X3DMaterialEditor::getPreview (); }

	virtual
	const X3D::X3DPtr <X3D::X3DMaterialNode> &
	getMaterial () const
	{ return X3DMaterialEditor::getMaterial (); }

	///  @name Event handlers

	virtual
	void
	on_appearance_unlink_clicked () final override;

	virtual
	void
	on_appearance_toggled () final override;

	void
	set_appearance ();

	void
	set_node ();

	void
	connectAppearance (const X3D::SFNode &);

	virtual
	void
	store () final override;

	///  @name Members

	X3D::X3DPtrArray <X3D::X3DShapeNode> shapeNodes;
	X3D::X3DPtr <X3D::X3DAppearanceNode> appearanceNode;
	X3D::SFTime                          appearanceBuffer;
	X3D::UndoStepPtr                     undoStep;
	bool                                 changing;

};

} // puck
} // titania

#endif
