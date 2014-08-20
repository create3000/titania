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

#include "X3DMaterialEditor.h"

#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

X3DMaterialEditor::X3DMaterialEditor () :
	X3DAppearanceEditorInterface ("", ""),
	                     preview (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                 appearances (),
	                materialNode (),
	          materialNodeBuffer (),
	                    material (),
	            twoSidedMaterial (),
	          isTwoSidedMaterial (false),
	                    undoStep (),
	                    changing (false),
	                diffuseColor (getBrowserWindow (), getDiffuseColorButton (), getDiffuseColorAdjustment (), getDiffuseColorBox (), "diffuseColor"),
	               specularColor (getBrowserWindow (), getSpecularColorButton (), getSpecularColorAdjustment (), getSpecularColorBox (), "specularColor"),
	               emissiveColor (getBrowserWindow (), getEmissiveColorButton (), getEmissiveColorAdjustment (), getEmissiveColorBox (), "emissiveColor"),
	            ambientIntensity (getBrowserWindow (), getAmbientIntensityAdjustment (), getAmbientIntensityBox (), "ambientIntensity"),
	                   shininess (getBrowserWindow (), getShininessAdjustment (), getShininessBox (), "shininess"),
	                transparency (getBrowserWindow (), getTransparencyAdjustment (), getTransparencyBox (), "transparency"),
	           separateBackColor (getBrowserWindow (), getSeparateBackColorCheckButton (),  "separateBackColor"),
	            backDiffuseColor (getBrowserWindow (), getBackDiffuseColorButton (), getBackDiffuseColorAdjustment (), getBackDiffuseColorBox (), "backDiffuseColor"),
	           backSpecularColor (getBrowserWindow (), getBackSpecularColorButton (), getBackSpecularColorAdjustment (), getBackSpecularColorBox (), "backSpecularColor"),
	           backEmissiveColor (getBrowserWindow (), getBackEmissiveColorButton (), getBackEmissiveColorAdjustment (), getBackEmissiveColorBox (), "backEmissiveColor"),
	        backAmbientIntensity (getBrowserWindow (), getBackAmbientIntensityAdjustment (), getBackAmbientIntensityBox (), "backAmbientIntensity"),
	               backShininess (getBrowserWindow (), getBackShininessAdjustment (), getBackShininessBox (), "backShininess"),
	            backTransparency (getBrowserWindow (), getBackTransparencyAdjustment (), getBackTransparencyBox (), "backTransparency")
{
	addChildren (materialNodeBuffer);
	materialNodeBuffer .addInterest (this, &X3DMaterialEditor::set_node);

	preview -> set_antialiasing (4);

	setup ();
}

void
X3DMaterialEditor::initialize ()
{
	getPreviewBox () .pack_start (*preview, true, true, 0);

	preview -> show ();
	preview -> initialized () .addInterest (this, &X3DMaterialEditor::set_initialized);

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DMaterialEditor::set_selection);

	set_selection ();
}

void
X3DMaterialEditor::set_initialized ()
{
	preview -> initialized () .removeInterest (this, &X3DMaterialEditor::set_initialized);

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
X3DMaterialEditor::set_selection ()
{
	undoStep .reset ();

	for (const auto & appearance : appearances)
		appearance -> material () .removeInterest (this, &X3DMaterialEditor::set_material);

	appearances = getSelection <X3D::Appearance> ({ X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> material () .addInterest (this, &X3DMaterialEditor::set_material);

	set_material ();
}

/***********************************************************************************************************************
 *
 *  Copy & Paste
 *
 **********************************************************************************************************************/

void
X3DMaterialEditor::on_copy ()
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
X3DMaterialEditor::on_paste ()
{
	getBrowserWindow () -> getPasteMenuItem () .activate ();
}

/***********************************************************************************************************************
 *
 *  Preview
 *
 **********************************************************************************************************************/

void
X3DMaterialEditor::set_preview ()
{
	if (not material or not twoSidedMaterial)
		return;

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
X3DMaterialEditor::on_sphere_clicked ()
{
	set_whichChoice (0);
}

void
X3DMaterialEditor::on_model_clicked ()
{
	set_whichChoice (1);
}

void
X3DMaterialEditor::set_whichChoice (const int32_t value)
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
X3DMaterialEditor::on_material_unlink_clicked ()
{
	unlinkClone (appearances, "material", undoStep);
}

void
X3DMaterialEditor::on_material_changed ()
{
	getFrontBox () .set_sensitive (getMaterialComboBoxText () .get_active_row_number () > 0);
	getBackBox ()  .set_sensitive (getMaterialComboBoxText () .get_active_row_number () > 1);

	//	if (getMaterialComboBoxText () .get_active_row_number () < 1)
	//	{
	//		getDiffuseDialog ()  .hide ();
	//		getSpecularDialog () .hide ();
	//		getEmissiveDialog () .hide ();
	//	}

	//	if (getMaterialComboBoxText () .get_active_row_number () < 2)
	//	{
	//		getBackDiffuseDialog ()  .hide ();
	//		getBackSpecularDialog () .hide ();
	//		getBackEmissiveDialog () .hide ();
	//	}

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

				materialNode = material;
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

				materialNode = twoSidedMaterial;
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

			field .removeInterest (this, &X3DMaterialEditor::set_material);
			field .addInterest (this, &X3DMaterialEditor::connectMaterial);

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

	set_widgets ();
	set_preview ();

	getWidget () .queue_draw (); // Update color buttons;
}

void
X3DMaterialEditor::set_material ()
{
	materialNodeBuffer .addEvent ();
}

void
X3DMaterialEditor::set_node ()
{
	auto       pair     = getNode <X3D::X3DMaterialNode> (appearances, "material");
	const int  active   = pair .second;
	const bool hasField = (active not_eq -2);

	materialNode = std::move (pair .first);

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

	if (not materialNode)
		materialNode = material;

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

	set_widgets ();
	set_preview ();
}

void
X3DMaterialEditor::set_widgets ()
{
	diffuseColor . setNodes ({ materialNode });
	specularColor .setNodes ({ materialNode });
	emissiveColor .setNodes ({ materialNode });

	ambientIntensity .setNodes ({ materialNode });
	shininess        .setNodes ({ materialNode });
	transparency     .setNodes ({ materialNode });

	separateBackColor .setNodes ({ materialNode });

	backDiffuseColor . setNodes ({ materialNode });
	backSpecularColor .setNodes ({ materialNode });
	backEmissiveColor .setNodes ({ materialNode });

	backAmbientIntensity .setNodes ({ materialNode });
	backShininess        .setNodes ({ materialNode });
	backTransparency     .setNodes ({ materialNode });
}

void
X3DMaterialEditor::connectMaterial (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DMaterialEditor::connectMaterial);
	field .addInterest (this, &X3DMaterialEditor::set_material);
}

X3DMaterialEditor::~X3DMaterialEditor ()
{
	X3D::removeBrowser (preview);

	dispose ();
}

} // puck
} // titania
