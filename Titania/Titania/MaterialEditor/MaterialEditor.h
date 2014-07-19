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

#include "../Undo/UndoStep.h"
#include "../UserInterfaces/X3DMaterialEditorInterface.h"

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
	connectMaterial (const X3D::SFNode &);

	///  @name Front color handler

	///  @name diffuseColor

	virtual
	bool
	on_diffuse_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_diffuse_clicked () final override;

	void
	on_diffuseColor_changed ();

	void
	set_diffuseColor ();

	void
	connectDiffuseColor (const X3D::SFColor &);

	///  @name specularColor

	virtual
	bool
	on_specular_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_specular_clicked () final override;

	void
	on_specularColor_changed ();

	void
	set_specularColor ();

	void
	connectSpecularColor (const X3D::SFColor &);

	///  @name emissiveColor

	virtual
	bool
	on_emissive_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_emissive_clicked () final override;

	void
	on_emissiveColor_changed ();

	void
	set_emissiveColor ();

	void
	connectEmissiveColor (const X3D::SFColor &);

	///  @name ambient

	virtual
	void
	on_ambient_changed () final override;

	void
	set_ambient ();

	void
	connectAmbient (const X3D::SFFloat &);

	///  @name shininess

	virtual
	void
	on_shininess_changed () final override;

	void
	set_shininess ();

	void
	connectShininess (const X3D::SFFloat &);

	///  @name transparency

	virtual
	void
	on_transparency_changed () final override;

	void
	set_transparency ();

	void
	connectTransparency (const X3D::SFFloat &);

	///  @name Back color handler

	///  @name separateBackColor

	virtual
	void
	on_separateBackColor_toggled () final override;

	void
	set_separateBackColor ();

	void
	connectSeparateBackColor (const X3D::SFBool &);

	///  @name backDiffuseColor

	virtual
	bool
	on_backDiffuse_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_backDiffuse_clicked () final override;

	void
	on_backDiffuseColor_changed ();

	void
	set_backDiffuseColor ();

	void
	connectBackDiffuseColor (const X3D::SFColor &);

	///  @name backSpecularColor

	virtual
	bool
	on_backSpecular_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_backSpecular_clicked () final override;

	void
	on_backSpecularColor_changed ();

	void
	set_backSpecularColor ();

	void
	connectBackSpecularColor (const X3D::SFColor &);

	///  @name backEmissiveColor

	virtual
	bool
	on_backEmissive_draw (const Cairo::RefPtr <Cairo::Context> &) final override;

	virtual
	void
	on_backEmissive_clicked () final override;

	void
	on_backEmissiveColor_changed ();

	void
	set_backEmissiveColor ();

	void
	connectBackEmissiveColor (const X3D::SFColor &);

	///  @name backAmbientIntensity

	virtual
	void
	on_backAmbient_changed () final override;

	void
	set_backAmbient ();

	void
	connectBackAmbient (const X3D::SFFloat &);

	///  @name backShininess

	virtual
	void
	on_backShininess_changed () final override;

	void
	set_backShininess ();

	void
	connectBackShininess (const X3D::SFFloat &);

	///  @name backTransparency

	virtual
	void
	on_backTransparency_changed () final override;

	void
	set_backTransparency ();

	void
	connectBackTransparency (const X3D::SFFloat &);

	///  @name Operations for all colors

	void
	initDialog (Gtk::ColorSelectionDialog &, void (MaterialEditor::* callback)());

	bool
	on_color_draw (const Cairo::RefPtr <Cairo::Context> &, const X3D::Color3f &, const X3D::Color3f &, Gtk::DrawingArea &);

	void
	on_color_changed (Gtk::ColorSelectionDialog &,
	                  X3D::SFColor &,
	                  X3D::SFColor &,
	                  Gtk::DrawingArea &,
	                  void (MaterialEditor::*)(),
	                  void (MaterialEditor::*) (const X3D::SFColor &));

	void
	set_color (Gtk::ColorSelectionDialog &, const X3D::Color3f &, const X3D::Color3f &);

	void
	on_value_changed (X3D::SFFloat &,
	                  X3D::SFFloat &,
	                  const double,
	                  void (MaterialEditor::*)(),
	                  void (MaterialEditor::*) (const X3D::SFFloat &));

	///  @name Helper functions

	Gdk::Color
	toColor (const X3D::Color3f &);

	///  @name Members

	X3D::BrowserPtr                    preview;
	X3D::X3DPtrArray <X3D::Appearance> appearances;
	X3D::MaterialPtr                   material;
	X3D::TwoSidedMaterialPtr           twoSidedMaterial;
	bool                               isTwoSidedMaterial;
	UndoStepPtr                        undoStep;
	bool                               changing;

};

} // puck
} // titania

#endif
