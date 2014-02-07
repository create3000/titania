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

#include "../UserInterfaces/X3DMaterialEditorInterface.h"
#include "../Undo/UndoStep.h"

namespace titania {
namespace puck {

class BrowserWindow;

class MaterialEditor :
	public X3DMaterialEditorInterface
{
public:

	///  @name Construction

	MaterialEditor (BrowserWindow* const, X3D::MFNode);

	///  @name Destruction

	virtual
	~MaterialEditor ();


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	void
	set_splashScreen ();

	void
	set_initialized ();

	///  @name Event handlers
	
	virtual
	void
	on_copy () final override;
	
	virtual
	void
	on_paste () final override;

	virtual
	void
	on_frontAndBackButton_toggled () final override;

	///  @name Front color handler

	virtual
	bool
	on_diffuse_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_diffuse_clicked () final override;

	void
	on_diffuseColor ();

	virtual
	bool
	on_specular_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_specular_clicked () final override;

	void
	on_specularColor ();

	virtual
	bool
	on_emissive_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_emissive_clicked () final override;

	void
	on_emissiveColor ();

	virtual
	void
	on_ambient () final override;

	virtual
	void
	on_shininess () final override;

	virtual
	void
	on_transparency () final override;

	///  @name Back color handler

	virtual
	bool
	on_backDiffuse_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_backDiffuse_clicked () final override;

	void
	on_backDiffuseColor ();

	virtual
	bool
	on_backSpecular_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_backSpecular_clicked () final override;

	void
	on_backSpecularColor ();

	virtual
	bool
	on_backEmissive_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_backEmissive_clicked () final override;

	void
	on_backEmissiveColor ();

	virtual
	void
	on_backAmbient () final override;

	virtual
	void
	on_backShininess () final override;

	virtual
	void
	on_backTransparency () final override;

	///  @name Operations for all colors

	void
	initDialog (Gtk::ColorSelectionDialog &, void (MaterialEditor::* callback)());

	bool
	on_color_draw (const Cairo::RefPtr <Cairo::Context> &, const X3D::Color3f &, const X3D::Color3f &, Gtk::DrawingArea &);

	void
	on_color_clicked (Gtk::ColorSelectionDialog &, const X3D::Color3f &, const X3D::Color3f &);

	void
	on_color (Gtk::ColorSelectionDialog &, X3D::SFColor &, X3D::SFColor &, Gtk::DrawingArea &);

	void
	updateAppearance ();
	
	void
	updateMaterial ();


	///  @name Helper functions

	Gdk::Color
	toColor (const X3D::Color3f &);

	Gdk::Color
	toColor (float value);

	///  @name Members

	X3D::X3DSFNode <X3D::Browser> browserSurface;

	std::vector <X3D::X3DSFNode <X3D::Appearance>> appearances;
	X3D::X3DSFNode <X3D::Material>                 material;
	X3D::X3DSFNode <X3D::TwoSidedMaterial>         twoSidedMaterial;
	
	std::shared_ptr <UndoStep> undoStep;

	bool initialized;

};

} // puck
} // titania

#endif
