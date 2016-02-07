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

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Shape/TwoSidedMaterial.h>

namespace titania {
namespace puck {

X3DMaterialEditor::X3DMaterialEditor () :
	X3DAppearanceEditorInterface (),
	                     preview (X3D::createBrowser (getBrowserWindow () -> getMasterBrowser ())),
	                 appearances (),
	                materialNode (),
	          materialNodeBuffer (),
	                    material (),
	            twoSidedMaterial (),
	          isTwoSidedMaterial (false),
	                    undoStep (),
	                    changing (false),
	                diffuseColor (this, getDiffuseColorButton (), getDiffuseColorAdjustment (), getDiffuseColorBox (), "diffuseColor"),
	               specularColor (this, getSpecularColorButton (), getSpecularColorAdjustment (), getSpecularColorBox (), "specularColor"),
	               emissiveColor (this, getEmissiveColorButton (), getEmissiveColorAdjustment (), getEmissiveColorBox (), "emissiveColor"),
	            ambientIntensity (this, getAmbientIntensityAdjustment (), getAmbientIntensityBox (), "ambientIntensity"),
	                   shininess (this, getShininessAdjustment (), getShininessBox (), "shininess"),
	                transparency (this, getTransparencyAdjustment (), getTransparencyBox (), "transparency"),
	           separateBackColor (this, getSeparateBackColorCheckButton (),  "separateBackColor"),
	            backDiffuseColor (this, getBackDiffuseColorButton (), getBackDiffuseColorAdjustment (), getBackDiffuseColorBox (), "backDiffuseColor"),
	           backSpecularColor (this, getBackSpecularColorButton (), getBackSpecularColorAdjustment (), getBackSpecularColorBox (), "backSpecularColor"),
	           backEmissiveColor (this, getBackEmissiveColorButton (), getBackEmissiveColorAdjustment (), getBackEmissiveColorBox (), "backEmissiveColor"),
	        backAmbientIntensity (this, getBackAmbientIntensityAdjustment (), getBackAmbientIntensityBox (), "backAmbientIntensity"),
	               backShininess (this, getBackShininessAdjustment (), getBackShininessBox (), "backShininess"),
	            backTransparency (this, getBackTransparencyAdjustment (), getBackTransparencyBox (), "backTransparency")
{
	addChildren (materialNodeBuffer);
	materialNodeBuffer .addInterest (this, &X3DMaterialEditor::set_node);

	preview -> setAntialiasing (4);
}

void
X3DMaterialEditor::initialize ()
{
	getPreviewBox () .pack_start (*preview, true, true, 0);

	preview -> initialized () .addInterest (this, &X3DMaterialEditor::set_browser);
	preview -> set_opacity (0);
	preview -> show ();
}

void
X3DMaterialEditor::set_browser ()
{
	preview -> initialized () .removeInterest (this, &X3DMaterialEditor::set_browser);
	preview -> initialized () .addInterest (this, &X3DMaterialEditor::set_initialized);

	try
	{
		preview -> loadURL ({ get_ui ("Editors/MaterialEditorPreview.x3dv") }, { });
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DMaterialEditor::set_initialized ()
{
	preview -> initialized () .removeInterest (this, &X3DMaterialEditor::set_initialized);
	preview -> set_opacity (1);

	try
	{
		preview -> getExecutionContext () -> getNamedNode ("Appearance") -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	set_preview ();
	set_whichChoice (getConfig () -> getInteger ("whichChoice"));
}

void
X3DMaterialEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & appearance : appearances)
		appearance -> material () .removeInterest (this, &X3DMaterialEditor::set_material);

	appearances = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });

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
	std::string text;

	text += "#X3D V3.3 utf8 Titania\n";
	text += "\n";
	text += "# " + getCurrentContext () -> getWorldURL () + "\n";
	text += "\n";
	text += "META \"titania magic\" \"Material\"\n";
	text += "\n";
	text += "Transform {\n";
	text += "  children Shape {\n";
	text += "    appearance Appearance {\n";
	text += "      material " + (isTwoSidedMaterial ? twoSidedMaterial -> toString () : material -> toString ()) + "\n";
	text += "    }\n";
	text += "    geometry Sphere { }\n";
	text += "  }\n";
	text += "}";

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
			twoSidedMaterial -> backEmissiveColor () .addInterest (backMaterial -> emissiveColor ());

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
	getConfig () -> setItem ("whichChoice", value);

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
					getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (appearance), field, nullptr, undoStep);
					break;
				}
				case 1:
				{
					getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (appearance), field, X3D::SFNode (material), undoStep);
					break;
				}
				case 2:
				{
					getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (appearance), field, X3D::SFNode (twoSidedMaterial), undoStep);
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

	getPaletteFaceCombo () .set_visible (isTwoSidedMaterial);

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
	undoStep .reset ();

	auto          tuple     = getSelection <X3D::X3DMaterialNode> (appearances, "material");
	const int32_t active    = std::get <1> (tuple);
	const bool    hasParent = std::get <2> (tuple);
	const bool    hasField  = (active not_eq -2);

	materialNode = std::move (std::get <0> (tuple));

	material           = materialNode;
	twoSidedMaterial   = materialNode;
	isTwoSidedMaterial = twoSidedMaterial;

	if (not material)
	{
		material = new X3D::Material (getCurrentContext ());
		material -> setup ();
	}

	if (not twoSidedMaterial)
	{
		twoSidedMaterial = new X3D::TwoSidedMaterial (getCurrentContext ());
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

	getSelectMaterialBox    () .set_sensitive (hasParent);
	getMaterialComboBoxText () .set_sensitive (hasField);

	if (isTwoSidedMaterial)
		getMaterialUnlinkButton () .set_sensitive (active > 0 and twoSidedMaterial -> getCloneCount () > 1);
	else
		getMaterialUnlinkButton () .set_sensitive (active > 0 and material -> getCloneCount () > 1);

	getPaletteFaceCombo () .set_visible (isTwoSidedMaterial);

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
{ }

} // puck
} // titania
