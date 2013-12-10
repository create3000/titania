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

#include "MaterialEditor.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

MaterialEditor::MaterialEditor (BrowserWindow* const browserWindow, X3D::MFNode nodes) :
	          X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DMaterialEditorInterface (get_ui ("Dialogs/MaterialEditor.ui"), gconf_dir ()),
	            browserSurface (X3D::createBrowser (browserWindow -> getBrowser ())),
	               appearances (),
	                  material (new X3D::Material (browserWindow -> getBrowser () -> getExecutionContext ())),
	          twoSidedMaterial (new X3D::TwoSidedMaterial (browserWindow -> getBrowser () -> getExecutionContext ())),
	                  undoStep (),
	               initialized (false)
{
	browserSurface -> set_antialiasing (4);

	getWindow () .set_transient_for (getBrowserWindow () -> getWindow ());

	initDialog (getDiffuseDialog (),  &MaterialEditor::on_diffuseColor);
	initDialog (getSpecularDialog (), &MaterialEditor::on_specularColor);
	initDialog (getEmissiveDialog (), &MaterialEditor::on_emissiveColor);

	initDialog (getBackDiffuseDialog (),  &MaterialEditor::on_backDiffuseColor);
	initDialog (getBackSpecularDialog (), &MaterialEditor::on_backSpecularColor);
	initDialog (getBackEmissiveDialog (), &MaterialEditor::on_backEmissiveColor);

	material -> setup ();
	twoSidedMaterial -> setup ();

	// Find Appearances

	X3D::traverse (nodes, [&] (X3D::SFNode & node)
	               {
	                  auto appearance = dynamic_cast <X3D::Appearance*> (node .getValue ());

	                  if (appearance)
								appearances .emplace_back (appearance);

	                  return true;
						});

	// Find last Material and TwoSidedMaterial node

	size_t materialIndex = 0;

	for (const auto & appearance : basic::reverse_adapter (appearances))
	{
		X3D::X3DSFNode <X3D::Material> m (appearance -> material ());

		if (m)
		{
			material -> ambientIntensity () = m -> ambientIntensity ();
			material -> diffuseColor ()     = m -> diffuseColor ();
			material -> specularColor ()    = m -> specularColor ();
			material -> emissiveColor ()    = m -> emissiveColor ();
			material -> shininess ()        = m -> shininess ();
			material -> transparency ()     = m -> transparency ();
			break;
		}

		++ materialIndex;
	}

	size_t twoSidedMaterialIndex = 0;

	for (const auto & appearance : basic::reverse_adapter (appearances))
	{
		X3D::X3DSFNode <X3D::TwoSidedMaterial> m (appearance -> material ());

		if (m)
		{
			twoSidedMaterial -> ambientIntensity () = m -> ambientIntensity ();
			twoSidedMaterial -> diffuseColor ()     = m -> diffuseColor ();
			twoSidedMaterial -> specularColor ()    = m -> specularColor ();
			twoSidedMaterial -> emissiveColor ()    = m -> emissiveColor ();
			twoSidedMaterial -> shininess ()        = m -> shininess ();
			twoSidedMaterial -> transparency ()     = m -> transparency ();

			twoSidedMaterial -> backAmbientIntensity () = m -> backAmbientIntensity ();
			twoSidedMaterial -> backDiffuseColor ()     = m -> backDiffuseColor ();
			twoSidedMaterial -> backSpecularColor ()    = m -> backSpecularColor ();
			twoSidedMaterial -> backEmissiveColor ()    = m -> backEmissiveColor ();
			twoSidedMaterial -> backShininess ()        = m -> backShininess ();
			twoSidedMaterial -> backTransparency ()     = m -> backTransparency ();
			break;
		}

		++ twoSidedMaterialIndex;
	}

	getFrontAndBackButton () .set_active (twoSidedMaterialIndex < materialIndex);

	on_frontAndBackButton_toggled ();
}

void
MaterialEditor::initialize ()
{
	X3DMaterialEditorInterface::initialize ();

	browserSurface -> initialized () .addInterest (this, &MaterialEditor::set_splashScreen);

	// Insert Surface, this will initialize the Browser.
	getPreviewBox () .pack_start (*browserSurface, true, true, 0);

	// Show Surface and start the X3D Main Loop.
	browserSurface -> show ();
}

void
MaterialEditor::set_splashScreen ()
{
	browserSurface -> initialized () .removeInterest (this, &MaterialEditor::set_splashScreen);
	browserSurface -> initialized () .addInterest (this, &MaterialEditor::set_initialized);

	try
	{
		browserSurface -> loadURL ({ find_data_file ("ui/Dialogs/Material.x3dv") });
	}
	catch (const X3D::X3DError &)
	{ }
}

void
MaterialEditor::set_initialized ()
{
	updateMaterial ();

	initialized = true;
}
	
void
MaterialEditor::on_copy ()
{
	__LOG__ << std::endl;
}

void
MaterialEditor::on_paste ()
{
	__LOG__ << std::endl;
}

void
MaterialEditor::on_frontAndBackButton_toggled ()
{
	__LOG__ << std::endl;

	// Copy front material

	if (initialized)
	{
		if (getFrontAndBackButton () .get_active ())
		{
			twoSidedMaterial -> ambientIntensity () = material -> ambientIntensity ();
			twoSidedMaterial -> diffuseColor ()     = material -> diffuseColor ();
			twoSidedMaterial -> specularColor ()    = material -> specularColor ();
			twoSidedMaterial -> emissiveColor ()    = material -> emissiveColor ();
			twoSidedMaterial -> shininess ()        = material -> shininess ();
			twoSidedMaterial -> transparency ()     = material -> transparency ();
		}
		else
		{
			material -> ambientIntensity () = twoSidedMaterial -> ambientIntensity ();
			material -> diffuseColor ()     = twoSidedMaterial -> diffuseColor ();
			material -> specularColor ()    = twoSidedMaterial -> specularColor ();
			material -> emissiveColor ()    = twoSidedMaterial -> emissiveColor ();
			material -> shininess ()        = twoSidedMaterial -> shininess ();
			material -> transparency ()     = twoSidedMaterial -> transparency ();
		}
	}

	// Update material
	
	updateAppearance ();

	// Update color buttons

	getDiffuseArea ()  .queue_draw ();
	getSpecularArea () .queue_draw ();
	getEmissiveArea () .queue_draw ();

	getBackDiffuseArea ()  .queue_draw ();
	getBackSpecularArea () .queue_draw ();
	getBackEmissiveArea () .queue_draw ();

	if (getFrontAndBackButton () .get_active ())
	{
		getAmbientScale ()      .set_value (twoSidedMaterial -> ambientIntensity ());
		getShininessScale ()    .set_value (twoSidedMaterial -> shininess ());
		getTransparencyScale () .set_value (twoSidedMaterial -> transparency ());

		getBackAmbientScale ()      .set_value (twoSidedMaterial -> backAmbientIntensity ());
		getBackShininessScale ()    .set_value (twoSidedMaterial -> backShininess ());
		getBackTransparencyScale () .set_value (twoSidedMaterial -> backTransparency ());
	}
	else
	{
		getAmbientScale ()      .set_value (material -> ambientIntensity ());
		getShininessScale ()    .set_value (material -> shininess ());
		getTransparencyScale () .set_value (material -> transparency ());
	}

	// Back expander

	getBackExpander () .set_sensitive (getFrontAndBackButton () .get_active ());

	// Update browser immediately

	getBrowser () -> update ();
}

// Front diffuse color

bool
MaterialEditor::on_diffuse_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> diffuseColor (), material -> diffuseColor (), getDiffuseArea ());
}

void
MaterialEditor::on_diffuse_clicked ()
{
	on_color_clicked (getDiffuseDialog (), twoSidedMaterial -> diffuseColor (), material -> diffuseColor ());
}

void
MaterialEditor::on_diffuseColor ()
{
	on_color (getDiffuseDialog (), twoSidedMaterial -> diffuseColor (), material -> diffuseColor (), getDiffuseArea ());
}

// Front specular color

bool
MaterialEditor::on_specular_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> specularColor (), material -> specularColor (), getSpecularArea ());
}

void
MaterialEditor::on_specular_clicked ()
{
	on_color_clicked (getSpecularDialog (), twoSidedMaterial -> specularColor (), material -> specularColor ());
}

void
MaterialEditor::on_specularColor ()
{
	on_color (getSpecularDialog (), twoSidedMaterial -> specularColor (), material -> specularColor (), getSpecularArea ());
}

// Front emissive color

bool
MaterialEditor::on_emissive_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> emissiveColor (), material -> emissiveColor (), getEmissiveArea ());
}

void
MaterialEditor::on_emissive_clicked ()
{
	on_color_clicked (getEmissiveDialog (), twoSidedMaterial -> emissiveColor (), material -> emissiveColor ());
}

void
MaterialEditor::on_emissiveColor ()
{
	on_color (getEmissiveDialog (), twoSidedMaterial -> emissiveColor (), material -> emissiveColor (), getEmissiveArea ());
}

// Front scale widgets

void
MaterialEditor::on_ambient ()
{
	if (getFrontAndBackButton () .get_active ())
		twoSidedMaterial -> ambientIntensity () = getAmbientScale () .get_value ();

	else
		material -> ambientIntensity () = getAmbientScale () .get_value ();

	updateAppearance ();
}

void
MaterialEditor::on_shininess ()
{
	if (getFrontAndBackButton () .get_active ())
		twoSidedMaterial -> shininess () = getShininessScale () .get_value ();

	else
		material -> shininess () = getShininessScale () .get_value ();

	updateAppearance ();
}

void
MaterialEditor::on_transparency ()
{
	if (getFrontAndBackButton () .get_active ())
		twoSidedMaterial -> transparency () = getTransparencyScale () .get_value ();

	else
		material -> transparency () = getTransparencyScale () .get_value ();

	updateAppearance ();
}

// Back diffuse color

bool
MaterialEditor::on_backDiffuse_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> backDiffuseColor (), twoSidedMaterial -> backDiffuseColor (), getBackDiffuseArea ());
}

void
MaterialEditor::on_backDiffuse_clicked ()
{
	on_color_clicked (getBackDiffuseDialog (), twoSidedMaterial -> backDiffuseColor (), twoSidedMaterial -> backDiffuseColor ());
}

void
MaterialEditor::on_backDiffuseColor ()
{
	on_color (getBackDiffuseDialog (), twoSidedMaterial -> backDiffuseColor (), twoSidedMaterial -> backDiffuseColor (), getBackDiffuseArea ());
}

// Back specular color

bool
MaterialEditor::on_backSpecular_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> backSpecularColor (), twoSidedMaterial -> backSpecularColor (), getBackSpecularArea ());
}

void
MaterialEditor::on_backSpecular_clicked ()
{
	on_color_clicked (getBackSpecularDialog (), twoSidedMaterial -> backSpecularColor (), twoSidedMaterial -> backSpecularColor ());
}

void
MaterialEditor::on_backSpecularColor ()
{
	on_color (getBackSpecularDialog (), twoSidedMaterial -> backSpecularColor (), twoSidedMaterial -> backSpecularColor (), getBackSpecularArea ());
}

// Back emissive color

bool
MaterialEditor::on_backEmissive_draw (const Cairo::RefPtr <Cairo::Context> & context)
{
	return on_color_draw (context, twoSidedMaterial -> backEmissiveColor (), twoSidedMaterial -> backEmissiveColor (), getBackEmissiveArea ());
}

void
MaterialEditor::on_backEmissive_clicked ()
{
	on_color_clicked (getBackEmissiveDialog (), twoSidedMaterial -> backEmissiveColor (), twoSidedMaterial -> backEmissiveColor ());
}

void
MaterialEditor::on_backEmissiveColor ()
{
	on_color (getBackEmissiveDialog (), twoSidedMaterial -> backEmissiveColor (), twoSidedMaterial -> backEmissiveColor (), getBackEmissiveArea ());
}

// Back scale widgets

void
MaterialEditor::on_backAmbient ()
{
	twoSidedMaterial -> backAmbientIntensity () = getBackAmbientScale () .get_value ();

	updateAppearance ();
}

void
MaterialEditor::on_backShininess ()
{
	twoSidedMaterial -> backShininess () = getBackShininessScale () .get_value ();

	updateAppearance ();
}

void
MaterialEditor::on_backTransparency ()
{
	twoSidedMaterial -> backTransparency () = getBackTransparencyScale () .get_value ();

	updateAppearance ();
}

// Operations for all colors

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
	if (getFrontAndBackButton () .get_active ())
		context -> set_source_rgb (twoSidedColor .r (), twoSidedColor .g (), twoSidedColor .b ());

	else
		context -> set_source_rgb (color .r (), color .g (), color .b ());

	context -> rectangle (0, 0, drawingArea .get_width (), drawingArea .get_height ());
	context -> fill ();

	return true;
}

void
MaterialEditor::on_color_clicked (Gtk::ColorSelectionDialog & dialog, const X3D::Color3f & twoSidedColor, const X3D::Color3f & color)
{
	if (getFrontAndBackButton () .get_active ())
	{
		dialog .get_color_selection () -> set_current_color  (toColor (twoSidedColor));
		dialog .get_color_selection () -> set_previous_color (toColor (twoSidedColor));
	}
	else
	{
		dialog .get_color_selection () -> set_current_color  (toColor (color));
		dialog .get_color_selection () -> set_previous_color (toColor (color));
	}

	dialog .present ();
}

void
MaterialEditor::on_color (Gtk::ColorSelectionDialog & dialog, X3D::SFColor & twoSidedColor, X3D::SFColor & color, Gtk::DrawingArea & drawingArea)
{
	auto rgba = dialog .get_color_selection () -> get_current_rgba ();

	X3D::Color3f color3 (rgba .get_red (), rgba .get_green (), rgba .get_blue ());

	// Update material

	if (getFrontAndBackButton () .get_active ())
		twoSidedColor = color3;

	else
		color = color3;

	// Update materials

	updateAppearance ();

	drawingArea .queue_draw ();
}

void
MaterialEditor::updateAppearance ()
{
	// Update material

	auto lastUndoStep = getBrowserWindow () -> getLastUndoStep ();

	if (undoStep and lastUndoStep == undoStep)
	{
		if (getFrontAndBackButton () .get_active ())
		{
			for (const auto & appearance : appearances)
				appearance -> material () = twoSidedMaterial;
		}
		else
		{
			for (const auto & appearance : appearances)
				appearance -> material () = material;
		}

		// Update materials

		getBrowser () -> update ();

		browserSurface -> addEvent ();
	}
	else
	{
		undoStep = std::make_shared <UndoStep> (_ ("Material Change"));

		undoStep -> addUndoFunction (std::mem_fn (&X3D::X3DBrowser::update), getBrowser ());

		if (getFrontAndBackButton () .get_active ())
		{
			twoSidedMaterial = twoSidedMaterial -> copy (twoSidedMaterial -> getExecutionContext ());
			twoSidedMaterial -> setup ();
			
			updateMaterial ();

			for (const auto & appearance : appearances)
	         getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), appearance -> material (), X3D::SFNode (twoSidedMaterial), undoStep);
		}
		else
		{
			material = material -> copy (material -> getExecutionContext ());
			material -> setup ();
			
			updateMaterial ();

			for (const auto & appearance : appearances)
	         getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), appearance -> material (), X3D::SFNode (material), undoStep);
		}

		undoStep -> addRedoFunction (std::mem_fn (&X3D::X3DBrowser::update), getBrowser ());

		getBrowser () -> update ();

		getBrowserWindow () -> addUndoStep (undoStep);

		browserSurface -> addEvent ();
	}
}

void
MaterialEditor::updateMaterial ()
{
	try
	{
		X3D::X3DSFNode <X3D::Appearance> appearance (browserSurface -> getExecutionContext () -> getNamedNode ("Appearance"));

		if (appearance)
		{
			if (getFrontAndBackButton () .get_active ())
				appearance -> material () = twoSidedMaterial;

			else
				appearance -> material () = material;
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

// Helper functions

Gdk::Color
MaterialEditor::toColor (const X3D::Color3f & color)
{
	Gdk::Color rgb;

	rgb .set_rgb_p (color .r (), color .g (), color .b ());
	return rgb;
}

Gdk::Color
MaterialEditor::toColor (float value)
{
	Gdk::Color rgb;

	rgb .set_rgb_p (value, value, value);
	return rgb;
}

MaterialEditor::~MaterialEditor ()
{
	__LOG__ << std::endl;

	X3D::removeBrowser (browserSurface);
}

} // puck
} // titania
