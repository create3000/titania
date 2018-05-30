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

#include "X3DLightEditorInterface.h"

namespace titania {
namespace puck {

void
X3DLightEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DLightEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DLightEditorInterface::create ()
{
	// Get objects.
	m_AmbientIntensityAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientIntensityAdjustment"));
	m_ColorAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorAdjustment"));
	m_DirectionalLightDirectionXAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DirectionalLightDirectionXAdjustment"));
	m_DirectionalLightDirectionYAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DirectionalLightDirectionYAdjustment"));
	m_DirectionalLightDirectionZAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DirectionalLightDirectionZAdjustment"));
	m_IntensityAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("IntensityAdjustment"));
	m_PointLightAttenuationXAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightAttenuationXAdjustment"));
	m_PointLightAttenuationYAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightAttenuationYAdjustment"));
	m_PointLightAttenuationZAdjustment     = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightAttenuationZAdjustment"));
	m_PointLightLocationXAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightLocationXAdjustment"));
	m_PointLightLocationYAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightLocationYAdjustment"));
	m_PointLightLocationZAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightLocationZAdjustment"));
	m_PointLightRadiusAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("PointLightRadiusAdjustment"));
	m_ShadowBiasAdjustment                 = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShadowBiasAdjustment"));
	m_ShadowColorAdjustment                = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShadowColorAdjustment"));
	m_ShadowIntensityAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShadowIntensityAdjustment"));
	m_ShadowMapSizeAdjustment              = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShadowMapSizeAdjustment"));
	m_SpotLightAttenuationXAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightAttenuationXAdjustment"));
	m_SpotLightAttenuationYAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightAttenuationYAdjustment"));
	m_SpotLightAttenuationZAdjustment      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightAttenuationZAdjustment"));
	m_SpotLightBeamWidthAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightBeamWidthAdjustment"));
	m_SpotLightCutOffAngleAdjustment       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightCutOffAngleAdjustment"));
	m_SpotLightDirectionXAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightDirectionXAdjustment"));
	m_SpotLightDirectionYAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightDirectionYAdjustment"));
	m_SpotLightDirectionZAdjustment        = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightDirectionZAdjustment"));
	m_SpotLightLocationXAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightLocationXAdjustment"));
	m_SpotLightLocationYAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightLocationYAdjustment"));
	m_SpotLightLocationZAdjustment         = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightLocationZAdjustment"));
	m_SpotLightRadiusAdjustment            = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpotLightRadiusAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("NewLightPopupButton", m_NewLightPopupButton);
	m_builder -> get_widget ("RemoveLightButton", m_RemoveLightButton);
	m_builder -> get_widget ("IndexButton", m_IndexButton);
	m_builder -> get_widget ("LightExpander", m_LightExpander);
	m_builder -> get_widget ("LightBox", m_LightBox);
	m_builder -> get_widget ("GlobalCheckButton", m_GlobalCheckButton);
	m_builder -> get_widget ("OnCheckButton", m_OnCheckButton);
	m_builder -> get_widget ("ColorBox", m_ColorBox);
	m_builder -> get_widget ("ColorButton", m_ColorButton);
	m_builder -> get_widget ("IntensityBox", m_IntensityBox);
	m_builder -> get_widget ("AmbientIntensityBox", m_AmbientIntensityBox);
	m_builder -> get_widget ("NameBox", m_NameBox);
	m_builder -> get_widget ("NameEntry", m_NameEntry);
	m_builder -> get_widget ("RenameButton", m_RenameButton);
	m_builder -> get_widget ("DirectionalLightExpander", m_DirectionalLightExpander);
	m_builder -> get_widget ("DirectionalLightNormalToolBox", m_DirectionalLightNormalToolBox);
	m_builder -> get_widget ("DirectionalLightDirectionBox", m_DirectionalLightDirectionBox);
	m_builder -> get_widget ("DirectionalLightDirectionXSpinButton", m_DirectionalLightDirectionXSpinButton);
	m_builder -> get_widget ("DirectionalLightDirectionYSpinButton", m_DirectionalLightDirectionYSpinButton);
	m_builder -> get_widget ("DirectionalLightDirectionZSpinButton", m_DirectionalLightDirectionZSpinButton);
	m_builder -> get_widget ("PointLightExpander", m_PointLightExpander);
	m_builder -> get_widget ("PointLightLocationBox", m_PointLightLocationBox);
	m_builder -> get_widget ("PointLightLocationXSpinButton", m_PointLightLocationXSpinButton);
	m_builder -> get_widget ("PointLightLocationYSpinButton", m_PointLightLocationYSpinButton);
	m_builder -> get_widget ("PointLightLocationZSpinButton", m_PointLightLocationZSpinButton);
	m_builder -> get_widget ("PointLightAttenuationBox", m_PointLightAttenuationBox);
	m_builder -> get_widget ("PointLightAttenuationXSpinButton", m_PointLightAttenuationXSpinButton);
	m_builder -> get_widget ("PointLightAttenuationYSpinButton", m_PointLightAttenuationYSpinButton);
	m_builder -> get_widget ("PointLightAttenuationZSpinButton", m_PointLightAttenuationZSpinButton);
	m_builder -> get_widget ("PointLightRadiusSpinButton", m_PointLightRadiusSpinButton);
	m_builder -> get_widget ("SpotLightExpander", m_SpotLightExpander);
	m_builder -> get_widget ("SpotLightAttenuationBox", m_SpotLightAttenuationBox);
	m_builder -> get_widget ("SpotLightAttenuationXSpinButton", m_SpotLightAttenuationXSpinButton);
	m_builder -> get_widget ("SpotLightAttenuationYSpinButton", m_SpotLightAttenuationYSpinButton);
	m_builder -> get_widget ("SpotLightAttenuationZSpinButton", m_SpotLightAttenuationZSpinButton);
	m_builder -> get_widget ("SpotLightLocationBox", m_SpotLightLocationBox);
	m_builder -> get_widget ("SpotLightLocationXSpinButton", m_SpotLightLocationXSpinButton);
	m_builder -> get_widget ("SpotLightLocationYSpinButton", m_SpotLightLocationYSpinButton);
	m_builder -> get_widget ("SpotLightLocationZSpinButton", m_SpotLightLocationZSpinButton);
	m_builder -> get_widget ("SpotLightRadiusSpinButton", m_SpotLightRadiusSpinButton);
	m_builder -> get_widget ("SpotLightNormalToolBox", m_SpotLightNormalToolBox);
	m_builder -> get_widget ("SpotLightDirectionBox", m_SpotLightDirectionBox);
	m_builder -> get_widget ("SpotLightDirectionXSpinButton", m_SpotLightDirectionXSpinButton);
	m_builder -> get_widget ("SpotLightDirectionYSpinButton", m_SpotLightDirectionYSpinButton);
	m_builder -> get_widget ("SpotLightDirectionZSpinButton", m_SpotLightDirectionZSpinButton);
	m_builder -> get_widget ("SpotLightBeamWidthBox", m_SpotLightBeamWidthBox);
	m_builder -> get_widget ("SpotLightBeamWidthSpinButton", m_SpotLightBeamWidthSpinButton);
	m_builder -> get_widget ("SpotLightCutOffAngleSpinButton", m_SpotLightCutOffAngleSpinButton);
	m_builder -> get_widget ("ShadowExpander", m_ShadowExpander);
	m_builder -> get_widget ("LightBox1", m_LightBox1);
	m_builder -> get_widget ("ShadowIntensityBox", m_ShadowIntensityBox);
	m_builder -> get_widget ("ShadowMapSizeSpinButton", m_ShadowMapSizeSpinButton);
	m_builder -> get_widget ("ShadowColorBox", m_ShadowColorBox);
	m_builder -> get_widget ("ShadowColorButton", m_ShadowColorButton);
	m_builder -> get_widget ("ShadowBiasBox", m_ShadowBiasBox);
	m_builder -> get_widget ("ShadowBiasSpinButton", m_ShadowBiasSpinButton);
	m_builder -> get_widget ("NewLightPopover", m_NewLightPopover);
	m_builder -> get_widget ("NewDirectionalLightButton", m_NewDirectionalLightButton);
	m_builder -> get_widget ("NewPointLightButton", m_NewPointLightButton);
	m_builder -> get_widget ("NewSpotLightButton", m_NewSpotLightButton);

	// Connect object Gtk::Adjustment with id 'SpotLightBeamWidthAdjustment'.
	m_SpotLightBeamWidthAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_spot_light_beam_width_value_changed));
	m_SpotLightCutOffAngleAdjustment -> signal_value_changed () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_spot_light_cut_of_angle_value_changed));

	// Connect object Gtk::Button with id 'NewLightPopupButton'.
	m_NewLightPopupButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_new_light_popup_clicked));
	m_RemoveLightButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_remove_light_clicked));
	m_IndexButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_index_clicked));
	m_NewDirectionalLightButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_new_directional_light_clicked));
	m_NewPointLightButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_new_point_light_clicked));
	m_NewSpotLightButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLightEditorInterface::on_new_spot_light_clicked));
}

X3DLightEditorInterface::~X3DLightEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
