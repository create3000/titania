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

#ifndef __TITANIA_MATERIAL_EDITOR_MATERIAL_EDITOR_H__
#define __TITANIA_MATERIAL_EDITOR_MATERIAL_EDITOR_H__

#include "../ComposedWidgets.h"
#include "../Undo/UndoStep.h"
#include "../UserInterfaces/X3DMaterialEditorInterface.h"

#include "../ComposedWidgets/SFColorButton.h"

namespace titania {
namespace puck {

class BrowserWindow;

class MaterialEditor :
	public X3DMaterialEditorInterface
{
public:

	///  @name Construction

	MaterialEditor (BrowserWindow* const);

	///  @name Destruction

	virtual
	~MaterialEditor ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	set_initialized ();

	void
	set_selection ();

	///  @name Copy & Paste

	virtual
	void
	on_copy () final override;

	virtual
	void
	on_paste () final override;

	///  @name preview

	void
	set_preview ();

	virtual
	void
	on_sphere_clicked () final override;

	virtual
	void
	on_model_clicked () final override;

	void
	set_whichChoice (const int32_t);

	///  @name material

	virtual
	void
	on_material_unlink_clicked () final override;

	virtual
	void
	on_material_changed () final override;

	void
	set_material ();

	void
	set_node ();

	void
	set_widgets ();

	void
	connectMaterial (const X3D::SFNode &);

	///  @name Members

	X3D::BrowserPtr                    preview;
	X3D::X3DPtrArray <X3D::Appearance> appearances;
	X3D::X3DPtr <X3D::X3DMaterialNode> materialNode;
	X3D::SFTime                        materialNodeBuffer;
	X3D::MaterialPtr                   material;
	X3D::TwoSidedMaterialPtr           twoSidedMaterial;
	bool                               isTwoSidedMaterial;
	UndoStepPtr                        undoStep;
	bool                               changing;

	SFColorButton diffuseColor;
	SFColorButton specularColor;
	SFColorButton emissiveColor;

	X3DFieldAdjustment <X3D::SFFloat> ambientIntensity;
	X3DFieldAdjustment <X3D::SFFloat> shininess;
	X3DFieldAdjustment <X3D::SFFloat> transparency;

	X3DFieldToggleButton <X3D::SFBool> separateBackColor;

	SFColorButton backDiffuseColor;
	SFColorButton backSpecularColor;
	SFColorButton backEmissiveColor;

	X3DFieldAdjustment <X3D::SFFloat> backAmbientIntensity;
	X3DFieldAdjustment <X3D::SFFloat> backShininess;
	X3DFieldAdjustment <X3D::SFFloat> backTransparency;

};

} // puck
} // titania

#endif
