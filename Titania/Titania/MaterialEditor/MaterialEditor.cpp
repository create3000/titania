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

#include "MaterialEditor.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

MaterialEditor::MaterialEditor (BrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DMaterialEditorInterface (get_ui ("Dialogs/MaterialEditor.xml"), gconf_dir ()),
	                   preview (X3D::createBrowser (browserWindow -> getBrowser ())),
	               appearances (),
	            materialBuffer (),
	                  material (),
	          twoSidedMaterial (),
	        isTwoSidedMaterial (false),
	                  undoStep (),
	                  changing (false)
{
	materialBuffer .addParent (getBrowser ());
	materialBuffer .addInterest (this, &MaterialEditor::set_node);

	preview -> set_antialiasing (4);

	initDialog (getDiffuseDialog (),  &MaterialEditor::on_diffuseColor_changed);
	initDialog (getSpecularDialog (), &MaterialEditor::on_specularColor_changed);
	initDialog (getEmissiveDialog (), &MaterialEditor::on_emissiveColor_changed);

	initDialog (getBackDiffuseDialog (),  &MaterialEditor::on_backDiffuseColor_changed);
	initDialog (getBackSpecularDialog (), &MaterialEditor::on_backSpecularColor_changed);
	initDialog (getBackEmissiveDialog (), &MaterialEditor::on_backEmissiveColor_changed);
}

void
MaterialEditor::initialize ()
{
	X3DMaterialEditorInterface::initialize ();

	getPreviewBox () .pack_start (*preview, true, true, 0);

	preview -> show ();
	preview -> initialized () .addInterest (this, &MaterialEditor::set_initialized);

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &MaterialEditor::set_selection);

	set_selection ();
}

void
MaterialEditor::set_initialized ()
{
	preview -> initialized () .removeInterest (this, &MaterialEditor::set_initialized);

	try
	{
		preview -> loadURL ({ get_ui ("Dialogs/MaterialEditorPreview.x3dv") });

		preview -> getExecutionContext () -> getNamedNode ("Appearance") -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	set_preview ();
	set_whichChoice (getConfig () .getInteger ("whichChoice"));
}

void
MaterialEditor::set_selection ()
{
	undoStep .reset ();

	for (const auto & appearance : appearances)
		appearance -> material () .removeInterest (this, &MaterialEditor::set_material);

	appearances = getSelection <X3D::Appearance> ({ X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> material () .addInterest (this, &MaterialEditor::set_material);

	set_material ();
}

/***********************************************************************************************************************
 *
 *  Copy & Paste
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_copy ()
{
	std::string text = "#X3D V3.3 utf8 Titania\n"
	                   "\n"
	                   "# " + getExecutionContext () -> getWorldURL () + "\n"
	                   "\n"
	                   "META \"titania magic\" \"Material\"\n"
	                   "\n"
	                   "Transform {\n"
	                   "  children Shape {\n"
	                   "    appearance Appearance {\n"
	                   "      material " + (isTwoSidedMaterial ? twoSidedMaterial -> toString () : material -> toString ()) + "\n"
	                   "    }\n"
	                   "    geometry Sphere { }\n"
	                   "  }\n"
	                   "}";

	Gtk::Clipboard::get () -> set_text (text);
}

void
MaterialEditor::on_paste ()
{
	getBrowserWindow () -> getPasteMenuItem () .activate ();
}

/***********************************************************************************************************************
 *
 *  Preview
 *
 **********************************************************************************************************************/

void
MaterialEditor::set_preview ()
{
	try
	{
		const X3D::AppearancePtr appearance (preview -> getExecutionContext () -> getNamedNode ("Appearance"));

		if (appearance)
		{
			if (appearance -> material ())
				appearance -> material () -> removeInterest (*preview, &X3D::Browser::addEvent);

			switch (getMaterialComboBoxText () .get_active_row_number ())
			{
				case 0:
				{
					appearance -> material () = nullptr;
					break;
				}
				case 1:
				{
					appearance -> material () = material;
					break;
				}
				case 2:
				{
					appearance -> material () = twoSidedMaterial;
					break;
				}
				default:
				{
					if (isTwoSidedMaterial)
						appearance -> material () = twoSidedMaterial;
					else
						appearance -> material () = material;

					break;
				}
			}

			if (appearance -> material ())
				appearance -> material () -> addInterest (*preview, &X3D::Browser::addEvent);
		}

		const X3D::AppearancePtr backAppearance (preview -> getExecutionContext () -> getNamedNode ("BackAppearance"));

		if (backAppearance)
		{
			X3D::MaterialPtr backMaterial (new X3D::Material (preview -> getExecutionContext ()));

			twoSidedMaterial -> backDiffuseColor ()  .addInterest (backMaterial -> diffuseColor ());
			twoSidedMaterial -> backSpecularColor () .addInterest (backMaterial -> specularColor ());
			twoSidedMaterial -> backEmissiveColor () .addInterest (backMaterial -> specularColor ());

			twoSidedMaterial -> backAmbientIntensity () .addInterest (backMaterial -> ambientIntensity ());
			twoSidedMaterial -> backShininess ()        .addInterest (backMaterial -> shininess ());
			twoSidedMaterial -> backTransparency ()     .addInterest (backMaterial -> transparency ());

			backMaterial -> ambientIntensity () = twoSidedMaterial -> backAmbientIntensity ();
			backMaterial -> diffuseColor ()     = twoSidedMaterial -> backDiffuseColor ();
			backMaterial -> specularColor ()    = twoSidedMaterial -> backSpecularColor ();
			backMaterial -> emissiveColor ()    = twoSidedMaterial -> backEmissiveColor ();
			backMaterial -> shininess ()        = twoSidedMaterial -> backShininess ();
			backMaterial -> transparency ()     = twoSidedMaterial -> backTransparency ();
			backMaterial -> setup ();

			backAppearance -> material () = backMaterial;
		}

		const X3D::X3DPtr <X3D::Switch> sphere (preview -> getExecutionContext () -> getNamedNode ("Sphere"));

		if (sphere)
			sphere -> whichChoice () = isTwoSidedMaterial;
	}
	catch (const X3D::X3DError &)
	{ }
}

void
MaterialEditor::on_sphere_clicked ()
{
	set_whichChoice (0);
}

void
MaterialEditor::on_model_clicked ()
{
	set_whichChoice (1);
}

void
MaterialEditor::set_whichChoice (const int32_t value)
{
	getConfig () .setItem ("whichChoice", value);

	try
	{
		const X3D::X3DPtr <X3D::Switch> switchNode (preview -> getExecutionContext () -> getNamedNode ("Switch"));

		if (switchNode)
			switchNode -> whichChoice () = value;
	}
	catch (const X3D::X3DError &)
	{ }
}

/***********************************************************************************************************************
 *
 *  Material
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_material_unlink_clicked ()
{
	unlinkClone (appearances, "material", undoStep);
}

void
MaterialEditor::on_material_changed ()
{
	getFrontBox () .set_sensitive (getMaterialComboBoxText () .get_active_row_number () > 0);
	getBackBox ()  .set_sensitive (getMaterialComboBoxText () .get_active_row_number () > 1);

	if (getMaterialComboBoxText () .get_active_row_number () < 1)
	{
		getDiffuseDialog ()  .hide ();
		getSpecularDialog () .hide ();
		getEmissiveDialog () .hide ();
	}

	if (getMaterialComboBoxText () .get_active_row_number () < 2)
	{
		getBackDiffuseDialog ()  .hide ();
		getBackSpecularDialog () .hide ();
		getBackEmissiveDialog () .hide ();
	}

	if (changing)
		return;

	// Copy material.

	switch (getMaterialComboBoxText () .get_active_row_number ())
	{
		case 1:
		{
			if (isTwoSidedMaterial)
			{
				material -> ambientIntensity () = twoSidedMaterial -> ambientIntensity ();
				material -> diffuseColor ()     = twoSidedMaterial -> diffuseColor ();
				material -> specularColor ()    = twoSidedMaterial -> specularColor ();
				material -> emissiveColor ()    = twoSidedMaterial -> emissiveColor ();
				material -> shininess ()        = twoSidedMaterial -> shininess ();
				material -> transparency ()     = twoSidedMaterial -> transparency ();
			}

			break;
		}
		case 2:
		{
			if (not isTwoSidedMaterial)
			{
				twoSidedMaterial -> ambientIntensity () = material -> ambientIntensity ();
				twoSidedMaterial -> diffuseColor ()     = material -> diffuseColor ();
				twoSidedMaterial -> specularColor ()    = material -> specularColor ();
				twoSidedMaterial -> emissiveColor ()    = material -> emissiveColor ();
				twoSidedMaterial -> shininess ()        = material -> shininess ();
				twoSidedMaterial -> transparency ()     = material -> transparency ();
			}

			break;
		}
		default:
			break;
	}

	isTwoSidedMaterial = (getMaterialComboBoxText () .get_active_row_number () == 2);
	
	// Set field.

	addUndoFunction <X3D::SFNode> (appearances, "material", undoStep);

	for (const auto & appearance : appearances)
	{
		try
		{
			auto & field = appearance -> material ();

			field .removeInterest (this, &MaterialEditor::set_material);
			field .addInterest (this, &MaterialEditor::connectMaterial);

			switch (getMaterialComboBoxText () .get_active_row_number ())
			{
				case 0:
				{
					getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, nullptr, undoStep);
					break;
				}
				case 1:
				{
					getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, X3D::SFNode (material), undoStep);
					break;
				}
				case 2:
				{
					getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, X3D::SFNode (twoSidedMaterial), undoStep);
					break;
				}
				default:
					break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (appearances, "material", undoStep);

	if (isTwoSidedMaterial)
		getMaterialUnlinkButton () .set_sensitive (getMaterialComboBoxText () .get_active () > 0 and twoSidedMaterial -> getCloneCount () > 1);
	else
		getMaterialUnlinkButton () .set_sensitive (getMaterialComboBoxText () .get_active () > 0 and material -> getCloneCount () > 1);

	set_preview ();

	getWidget () .queue_draw (); // Update color buttons;
}

void
MaterialEditor::set_material ()
{
	materialBuffer .addEvent ();
}

void
MaterialEditor::set_node ()
{
	if (material)
	{
		material -> diffuseColor ()  .removeInterest (this, &MaterialEditor::set_diffuseColor);
		material -> specularColor () .removeInterest (this, &MaterialEditor::set_specularColor);
		material -> emissiveColor () .removeInterest (this, &MaterialEditor::set_emissiveColor);

		material -> ambientIntensity () .removeInterest (this, &MaterialEditor::set_ambient);
		material -> shininess ()        .removeInterest (this, &MaterialEditor::set_shininess);
		material -> transparency ()     .removeInterest (this, &MaterialEditor::set_transparency);
	}

	if (twoSidedMaterial)
	{
		twoSidedMaterial -> separateBackColor () .removeInterest (this, &MaterialEditor::set_separateBackColor);

		twoSidedMaterial -> diffuseColor ()  .removeInterest (this, &MaterialEditor::set_diffuseColor);
		twoSidedMaterial -> specularColor () .removeInterest (this, &MaterialEditor::set_specularColor);
		twoSidedMaterial -> emissiveColor () .removeInterest (this, &MaterialEditor::set_emissiveColor);

		twoSidedMaterial -> ambientIntensity () .removeInterest (this, &MaterialEditor::set_ambient);
		twoSidedMaterial -> shininess ()        .removeInterest (this, &MaterialEditor::set_shininess);
		twoSidedMaterial -> transparency ()     .removeInterest (this, &MaterialEditor::set_transparency);

		twoSidedMaterial -> backDiffuseColor ()  .removeInterest (this, &MaterialEditor::set_backDiffuseColor);
		twoSidedMaterial -> backSpecularColor () .removeInterest (this, &MaterialEditor::set_backSpecularColor);
		twoSidedMaterial -> backEmissiveColor () .removeInterest (this, &MaterialEditor::set_backEmissiveColor);

		twoSidedMaterial -> backAmbientIntensity () .removeInterest (this, &MaterialEditor::set_backAmbient);
		twoSidedMaterial -> backShininess ()        .removeInterest (this, &MaterialEditor::set_backShininess);
		twoSidedMaterial -> backTransparency ()     .removeInterest (this, &MaterialEditor::set_backTransparency);
	}

	auto       pair     = getNode <X3D::X3DMaterialNode> (appearances, "material");
	const int  active   = pair .second;
	const bool hasField = (active not_eq -2);

	X3D::X3DPtr <X3D::X3DMaterialNode> materialNode = std::move (pair .first);

	material           = materialNode;
	twoSidedMaterial   = materialNode;
	isTwoSidedMaterial = twoSidedMaterial;

	if (not material)
	{
		material = new X3D::Material (getExecutionContext ());
		material -> setup ();
	}

	if (not twoSidedMaterial)
	{
		twoSidedMaterial = new X3D::TwoSidedMaterial (getExecutionContext ());
		twoSidedMaterial -> setup ();
	}

	changing = true;

	switch (active)
	{
		case 0:
		{
			// None
			getMaterialComboBoxText () .set_active (0);
			break;
		}
		case 1:
		{
			// Material or TwoSidedMaterial
			getMaterialComboBoxText () .set_active (isTwoSidedMaterial + 1);
			break;
		}
		default:
		{
			// Inconsistent
			getMaterialComboBoxText () .set_active (-1);
			break;
		}
	}

	getMaterialComboBoxText () .set_sensitive (hasField);

	if (isTwoSidedMaterial)
		getMaterialUnlinkButton () .set_sensitive (active > 0 and twoSidedMaterial -> getCloneCount () > 1);
	else
		getMaterialUnlinkButton () .set_sensitive (active > 0 and material -> getCloneCount () > 1);

	changing = false;

	material -> diffuseColor ()  .addInterest (this, &MaterialEditor::set_diffuseColor);
	material -> specularColor () .addInterest (this, &MaterialEditor::set_specularColor);
	material -> emissiveColor () .addInterest (this, &MaterialEditor::set_emissiveColor);

	material -> ambientIntensity () .addInterest (this, &MaterialEditor::set_ambient);
	material -> shininess ()        .addInterest (this, &MaterialEditor::set_shininess);
	material -> transparency ()     .addInterest (this, &MaterialEditor::set_transparency);

	twoSidedMaterial -> separateBackColor () .addInterest (this, &MaterialEditor::set_separateBackColor);

	twoSidedMaterial -> diffuseColor ()  .addInterest (this, &MaterialEditor::set_diffuseColor);
	twoSidedMaterial -> specularColor () .addInterest (this, &MaterialEditor::set_specularColor);
	twoSidedMaterial -> emissiveColor () .addInterest (this, &MaterialEditor::set_emissiveColor);

	twoSidedMaterial -> ambientIntensity () .addInterest (this, &MaterialEditor::set_ambient);
	twoSidedMaterial -> shininess ()        .addInterest (this, &MaterialEditor::set_shininess);
	twoSidedMaterial -> transparency ()     .addInterest (this, &MaterialEditor::set_transparency);

	twoSidedMaterial -> backDiffuseColor ()  .addInterest (this, &MaterialEditor::set_backDiffuseColor);
	twoSidedMaterial -> backSpecularColor () .addInterest (this, &MaterialEditor::set_backSpecularColor);
	twoSidedMaterial -> backEmissiveColor () .addInterest (this, &MaterialEditor::set_backEmissiveColor);

	twoSidedMaterial -> backAmbientIntensity () .addInterest (this, &MaterialEditor::set_backAmbient);
	twoSidedMaterial -> backShininess ()        .addInterest (this, &MaterialEditor::set_backShininess);
	twoSidedMaterial -> backTransparency ()     .addInterest (this, &MaterialEditor::set_backTransparency);

	set_diffuseColor ();
	set_specularColor ();
	set_emissiveColor ();

	set_ambient ();
	set_shininess ();
	set_transparency ();

	set_separateBackColor ();

	set_backDiffuseColor ();
	set_backSpecularColor ();
	set_backEmissiveColor ();

	set_backAmbient ();
	set_backShininess ();
	set_backTransparency ();

	set_preview ();
}

void
MaterialEditor::connectMaterial (const X3D::SFNode & field)
{
	field .removeInterest (this, &MaterialEditor::connectMaterial);
	field .addInterest (this, &MaterialEditor::set_material);
}

/* ********************************************************************************************************************
*
*  Front Material
*
* ********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *  diffuseColor
 *
 **********************************************************************************************************************/

bool
MaterialEditor::on_diffuse_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> diffuseColor (), material -> diffuseColor (), getDiffuseArea ());
}

void
MaterialEditor::on_diffuse_clicked ()
{
	getDiffuseDialog () .present ();
}

void
MaterialEditor::on_diffuseColor_changed ()
{
	on_color_changed (getDiffuseDialog (),
	                  twoSidedMaterial -> diffuseColor (),
	                  material -> diffuseColor (),
	                  getDiffuseArea (),
	                  &MaterialEditor::set_diffuseColor,
	                  &MaterialEditor::connectDiffuseColor);
}

void
MaterialEditor::set_diffuseColor ()
{
	set_color (getDiffuseDialog (), twoSidedMaterial -> diffuseColor (),  material -> diffuseColor ());
}

void
MaterialEditor::connectDiffuseColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &MaterialEditor::connectDiffuseColor);
	field .addInterest (this, &MaterialEditor::set_diffuseColor);
}

/***********************************************************************************************************************
 *
 *  specularColor
 *
 **********************************************************************************************************************/

bool
MaterialEditor::on_specular_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> specularColor (), material -> specularColor (), getSpecularArea ());
}

void
MaterialEditor::on_specular_clicked ()
{
	getSpecularDialog () .present ();
}

void
MaterialEditor::on_specularColor_changed ()
{
	on_color_changed (getSpecularDialog (),
	                  twoSidedMaterial -> specularColor (),
	                  material -> specularColor (),
	                  getSpecularArea (),
	                  &MaterialEditor::set_specularColor,
	                  &MaterialEditor::connectSpecularColor);
}

void
MaterialEditor::set_specularColor ()
{
	set_color (getSpecularDialog (), twoSidedMaterial -> specularColor (), material -> specularColor ());
}

void
MaterialEditor::connectSpecularColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &MaterialEditor::connectSpecularColor);
	field .addInterest (this, &MaterialEditor::set_specularColor);
}

/***********************************************************************************************************************
 *
 *  emissiveColor
 *
 **********************************************************************************************************************/

bool
MaterialEditor::on_emissive_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> emissiveColor (), material -> emissiveColor (), getEmissiveArea ());
}

void
MaterialEditor::on_emissive_clicked ()
{
	getEmissiveDialog () .present ();
}

void
MaterialEditor::on_emissiveColor_changed ()
{
	on_color_changed (getEmissiveDialog (),
	                  twoSidedMaterial -> emissiveColor (),
	                  material -> emissiveColor (),
	                  getEmissiveArea (),
	                  &MaterialEditor::set_emissiveColor,
	                  &MaterialEditor::connectEmissiveColor);
}

void
MaterialEditor::set_emissiveColor ()
{
	set_color (getEmissiveDialog (), twoSidedMaterial -> emissiveColor (), material -> emissiveColor ());
}

void
MaterialEditor::connectEmissiveColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &MaterialEditor::connectEmissiveColor);
	field .addInterest (this, &MaterialEditor::set_emissiveColor);
}

/***********************************************************************************************************************
 *
 *  ambientIntensity
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_ambient_changed ()
{
	if (changing)
		return;

	on_value_changed (twoSidedMaterial -> ambientIntensity (),
	                  material -> ambientIntensity (),
	                  getAmbientAdjustment () -> get_value (),
	                  &MaterialEditor::set_ambient,
	                  &MaterialEditor::connectAmbient);
}

void
MaterialEditor::set_ambient ()
{
	changing = true;

	getAmbientAdjustment () -> set_value (isTwoSidedMaterial ? twoSidedMaterial -> ambientIntensity () : material -> ambientIntensity ());

	changing = false;
}

void
MaterialEditor::connectAmbient (const X3D::SFFloat & field)
{
	field .removeInterest (this, &MaterialEditor::connectAmbient);
	field .addInterest (this, &MaterialEditor::set_ambient);
}

/***********************************************************************************************************************
 *
 *  shininess
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_shininess_changed ()
{
	if (changing)
		return;

	on_value_changed (twoSidedMaterial -> shininess (),
	                  material -> shininess (),
	                  getShininessAdjustment () -> get_value (),
	                  &MaterialEditor::set_shininess,
	                  &MaterialEditor::connectShininess);
}

void
MaterialEditor::set_shininess ()
{
	changing = true;

	getShininessAdjustment () -> set_value (isTwoSidedMaterial ? twoSidedMaterial -> shininess () : material -> shininess ());

	changing = false;
}

void
MaterialEditor::connectShininess (const X3D::SFFloat & field)
{
	field .removeInterest (this, &MaterialEditor::connectShininess);
	field .addInterest (this, &MaterialEditor::set_shininess);
}

/***********************************************************************************************************************
 *
 *  ransparency
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_transparency_changed ()
{
	if (changing)
		return;

	on_value_changed (twoSidedMaterial -> transparency (),
	                  material -> transparency (),
	                  getTransparencyAdjustment () -> get_value (),
	                  &MaterialEditor::set_transparency,
	                  &MaterialEditor::connectTransparency);
}

void
MaterialEditor::set_transparency ()
{
	changing = true;

	getTransparencyAdjustment () -> set_value (isTwoSidedMaterial ? twoSidedMaterial -> transparency () : material -> transparency ());

	changing = false;
}

void
MaterialEditor::connectTransparency (const X3D::SFFloat & field)
{
	field .removeInterest (this, &MaterialEditor::connectTransparency);
	field .addInterest (this, &MaterialEditor::set_transparency);
}

/* ********************************************************************************************************************
*
*  Back Material
*
* ********************************************************************************************************************/

/***********************************************************************************************************************
 *
 *  separateBackColor
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_separateBackColor_toggled ()
{
	if (changing)
		return;

	addUndoFunction (twoSidedMaterial, twoSidedMaterial -> separateBackColor (), undoStep);

	twoSidedMaterial -> separateBackColor () .removeInterest (this, &MaterialEditor::set_separateBackColor);
	twoSidedMaterial -> separateBackColor () .addInterest (this, &MaterialEditor::connectSeparateBackColor);

	twoSidedMaterial -> separateBackColor () = getSeparateBackColorCheckButton () .get_active ();

	addRedoFunction (twoSidedMaterial -> separateBackColor (), undoStep);
}

void
MaterialEditor::set_separateBackColor ()
{
	changing = true;

	getSeparateBackColorCheckButton () .set_active (twoSidedMaterial -> separateBackColor ());

	changing = false;
}

void
MaterialEditor::connectSeparateBackColor (const X3D::SFBool & field)
{
	field .removeInterest (this, &MaterialEditor::connectSeparateBackColor);
	field .addInterest (this, &MaterialEditor::set_separateBackColor);
}

/***********************************************************************************************************************
 *
 *  backDiffuseColor
 *
 **********************************************************************************************************************/

bool
MaterialEditor::on_backDiffuse_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> backDiffuseColor (), twoSidedMaterial -> backDiffuseColor (), getBackDiffuseArea ());
}

void
MaterialEditor::on_backDiffuse_clicked ()
{
	getBackDiffuseDialog () .present ();
}

void
MaterialEditor::on_backDiffuseColor_changed ()
{
	on_color_changed (getBackDiffuseDialog (),
	                  twoSidedMaterial -> backDiffuseColor (),
	                  twoSidedMaterial -> backDiffuseColor (),
	                  getBackDiffuseArea (),
	                  &MaterialEditor::set_backDiffuseColor,
	                  &MaterialEditor::connectBackDiffuseColor);
}

void
MaterialEditor::set_backDiffuseColor ()
{
	set_color (getBackDiffuseDialog (),  twoSidedMaterial -> backDiffuseColor (),  twoSidedMaterial -> backDiffuseColor ());
}

void
MaterialEditor::connectBackDiffuseColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &MaterialEditor::connectBackDiffuseColor);
	field .addInterest (this, &MaterialEditor::set_backDiffuseColor);
}

/***********************************************************************************************************************
 *
 *  backSpecularColor
 *
 **********************************************************************************************************************/

bool
MaterialEditor::on_backSpecular_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> backSpecularColor (), twoSidedMaterial -> backSpecularColor (), getBackSpecularArea ());
}

void
MaterialEditor::on_backSpecular_clicked ()
{
	getBackSpecularDialog () .present ();
}

void
MaterialEditor::on_backSpecularColor_changed ()
{
	on_color_changed (getBackSpecularDialog (),
	                  twoSidedMaterial -> backSpecularColor (),
	                  twoSidedMaterial -> backSpecularColor (),
	                  getBackSpecularArea (),
	                  &MaterialEditor::set_backSpecularColor,
	                  &MaterialEditor::connectBackSpecularColor);
}

void
MaterialEditor::set_backSpecularColor ()
{
	set_color (getBackSpecularDialog (), twoSidedMaterial -> backSpecularColor (), twoSidedMaterial -> backSpecularColor ());
}

void
MaterialEditor::connectBackSpecularColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &MaterialEditor::connectSpecularColor);
	field .addInterest (this, &MaterialEditor::set_backSpecularColor);
}

/***********************************************************************************************************************
 *
 *  backEmissiveColor
 *
 **********************************************************************************************************************/

bool
MaterialEditor::on_backEmissive_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> backEmissiveColor (), twoSidedMaterial -> backEmissiveColor (), getBackEmissiveArea ());
}

void
MaterialEditor::on_backEmissive_clicked ()
{
	getBackEmissiveDialog () .present ();
}

void
MaterialEditor::on_backEmissiveColor_changed ()
{
	on_color_changed (getBackEmissiveDialog (),
	                  twoSidedMaterial -> backEmissiveColor (),
	                  twoSidedMaterial -> backEmissiveColor (),
	                  getBackEmissiveArea (),
	                  &MaterialEditor::set_backEmissiveColor,
	                  &MaterialEditor::connectBackEmissiveColor);
}

void
MaterialEditor::set_backEmissiveColor ()
{
	set_color (getBackEmissiveDialog (), twoSidedMaterial -> backEmissiveColor (), twoSidedMaterial -> backEmissiveColor ());
}

void
MaterialEditor::connectBackEmissiveColor (const X3D::SFColor & field)
{
	field .removeInterest (this, &MaterialEditor::connectEmissiveColor);
	field .addInterest (this, &MaterialEditor::set_backEmissiveColor);
}

/***********************************************************************************************************************
 *
 *  backAmbientIntensity
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_backAmbient_changed ()
{
	if (changing)
		return;

	on_value_changed (twoSidedMaterial -> backAmbientIntensity (),
	                  twoSidedMaterial -> backAmbientIntensity (),
	                  getBackAmbientAdjustment () -> get_value (),
	                  &MaterialEditor::set_backAmbient,
	                  &MaterialEditor::connectBackAmbient);
}

void
MaterialEditor::set_backAmbient ()
{
	changing = true;

	getBackAmbientAdjustment () -> set_value (twoSidedMaterial -> backAmbientIntensity ());

	changing = false;
}

void
MaterialEditor::connectBackAmbient (const X3D::SFFloat & field)
{
	field .removeInterest (this, &MaterialEditor::connectBackAmbient);
	field .addInterest (this, &MaterialEditor::set_backAmbient);
}

/***********************************************************************************************************************
 *
 *  backShininess
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_backShininess_changed ()
{
	if (changing)
		return;

	on_value_changed (twoSidedMaterial -> backShininess (),
	                  twoSidedMaterial -> backShininess (),
	                  getBackShininessAdjustment () -> get_value (),
	                  &MaterialEditor::set_backShininess,
	                  &MaterialEditor::connectBackShininess);
}

void
MaterialEditor::set_backShininess ()
{
	changing = true;

	getBackShininessAdjustment () -> set_value (twoSidedMaterial -> backShininess ());

	changing = false;
}

void
MaterialEditor::connectBackShininess (const X3D::SFFloat & field)
{
	field .removeInterest (this, &MaterialEditor::connectBackShininess);
	field .addInterest (this, &MaterialEditor::set_backShininess);
}

/***********************************************************************************************************************
 *
 *  backTransparency
 *
 **********************************************************************************************************************/

void
MaterialEditor::on_backTransparency_changed ()
{
	if (changing)
		return;

	on_value_changed (twoSidedMaterial -> backTransparency (),
	                  twoSidedMaterial -> backTransparency (),
	                  getBackTransparencyAdjustment () -> get_value (),
	                  &MaterialEditor::set_backTransparency,
	                  &MaterialEditor::connectBackTransparency);
}

void
MaterialEditor::set_backTransparency ()
{
	changing = true;

	getBackTransparencyAdjustment () -> set_value (twoSidedMaterial -> backTransparency ());

	changing = false;
}

void
MaterialEditor::connectBackTransparency (const X3D::SFFloat & field)
{
	field .removeInterest (this, &MaterialEditor::connectBackTransparency);
	field .addInterest (this, &MaterialEditor::set_backTransparency);
}

/***********************************************************************************************************************
 *
 *  Operations for all colors
 *
 **********************************************************************************************************************/

void
MaterialEditor::initDialog (Gtk::ColorSelectionDialog & dialog, void (MaterialEditor::* callback)())
{
	dialog .get_color_selection () -> set_has_palette (true);
	dialog .get_color_selection () -> signal_color_changed () .connect (sigc::mem_fun (*this, callback));

	dialog .property_ok_button () .get_value () -> hide ();
	dialog .property_cancel_button () .get_value () -> hide ();
}

bool
MaterialEditor::on_color_draw (const Cairo::RefPtr <Cairo::Context> & context, const X3D::Color3f & twoSidedColor, const X3D::Color3f & color, Gtk::DrawingArea & drawingArea)
{
	if (isTwoSidedMaterial)
		context -> set_source_rgb (twoSidedColor .r (), twoSidedColor .g (), twoSidedColor .b ());

	else
		context -> set_source_rgb (color .r (), color .g (), color .b ());

	context -> rectangle (0, 0, drawingArea .get_width (), drawingArea .get_height ());
	context -> fill ();

	return true;
}

void
MaterialEditor::on_color_changed (Gtk::ColorSelectionDialog & dialog,
                                  X3D::SFColor & twoSidedColor,
                                  X3D::SFColor & color,
                                  Gtk::DrawingArea & drawingArea,
                                  void (MaterialEditor::* set_color)(),
                                  void (MaterialEditor::* connectColor) (const X3D::SFColor &))
{
	drawingArea .queue_draw ();

	if (changing)
		return;

	const auto rgba   = dialog .get_color_selection () -> get_current_rgba ();
	const auto color3 = X3D::Color3f (rgba .get_red (), rgba .get_green (), rgba .get_blue ());

	// Update material

	if (isTwoSidedMaterial)
	{
		addUndoFunction (twoSidedMaterial, twoSidedColor, undoStep);

		twoSidedColor .removeInterest (this, set_color);
		twoSidedColor .addInterest (this, connectColor);

		twoSidedColor = color3;

		addRedoFunction (twoSidedColor, undoStep);
	}
	else
	{
		addUndoFunction (material, color, undoStep);

		color .removeInterest (this, set_color);
		color .addInterest (this, connectColor);

		color = color3;

		addRedoFunction (color, undoStep);
	}
}

void
MaterialEditor::set_color (Gtk::ColorSelectionDialog & dialog, const X3D::Color3f & twoSidedColor, const X3D::Color3f & color)
{
	changing = true;

	if (isTwoSidedMaterial)
	{
		dialog .get_color_selection () -> set_current_color  (toColor (twoSidedColor));
		dialog .get_color_selection () -> set_previous_color (toColor (twoSidedColor));
	}
	else
	{
		dialog .get_color_selection () -> set_current_color  (toColor (color));
		dialog .get_color_selection () -> set_previous_color (toColor (color));
	}

	changing = false;
}

void
MaterialEditor::on_value_changed (X3D::SFFloat & backField,
                                  X3D::SFFloat & field,
                                  const double value,
                                  void (MaterialEditor::* set_value)(),
                                  void (MaterialEditor::* connectValue) (const X3D::SFFloat &))
{
	if (isTwoSidedMaterial)
	{
		addUndoFunction (twoSidedMaterial, backField, undoStep);

		backField .removeInterest (this, set_value);
		backField .addInterest (this, connectValue);

		backField = value;

		addRedoFunction (backField, undoStep);
	}
	else
	{
		addUndoFunction (material, field, undoStep);

		field .removeInterest (this, set_value);
		field .addInterest (this, connectValue);

		field = value;

		addRedoFunction (field, undoStep);
	}
}

/***********************************************************************************************************************
 *
 *  Helper functions
 *
 **********************************************************************************************************************/

Gdk::Color
MaterialEditor::toColor (const X3D::Color3f & value)
{
	Gdk::Color color;

	color .set_rgb_p (value .r (), value .g (), value .b ());
	return color;
}

MaterialEditor::~MaterialEditor ()
{
	X3D::removeBrowser (preview);
}

} // puck
} // titania
