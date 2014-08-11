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
#include "X3DLightEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DLightEditorInterface::m_widgetName = "LightEditor";

void
X3DLightEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

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
	m_Window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_Widget);
	m_Widget -> set_name ("Widget");
	m_builder -> get_widget ("LightExpander", m_LightExpander);
	m_LightExpander -> set_name ("LightExpander");
	m_builder -> get_widget ("LightBox", m_LightBox);
	m_LightBox -> set_name ("LightBox");
	m_builder -> get_widget ("GlobalCheckButton", m_GlobalCheckButton);
	m_GlobalCheckButton -> set_name ("GlobalCheckButton");
	m_builder -> get_widget ("OnCheckButton", m_OnCheckButton);
	m_OnCheckButton -> set_name ("OnCheckButton");
	m_builder -> get_widget ("ColorBox", m_ColorBox);
	m_ColorBox -> set_name ("ColorBox");
	m_builder -> get_widget ("ColorButton", m_ColorButton);
	m_ColorButton -> set_name ("ColorButton");
	m_builder -> get_widget ("IntensityBox", m_IntensityBox);
	m_IntensityBox -> set_name ("IntensityBox");
	m_builder -> get_widget ("IntensityScale", m_IntensityScale);
	m_IntensityScale -> set_name ("IntensityScale");
	m_builder -> get_widget ("AmbientIntensityBox", m_AmbientIntensityBox);
	m_AmbientIntensityBox -> set_name ("AmbientIntensityBox");
	m_builder -> get_widget ("AmbientIntensityScale", m_AmbientIntensityScale);
	m_AmbientIntensityScale -> set_name ("AmbientIntensityScale");
	m_builder -> get_widget ("DirectionalLightExpander", m_DirectionalLightExpander);
	m_DirectionalLightExpander -> set_name ("DirectionalLightExpander");
	m_builder -> get_widget ("DirectionalLightDirectionBox", m_DirectionalLightDirectionBox);
	m_DirectionalLightDirectionBox -> set_name ("DirectionalLightDirectionBox");
	m_builder -> get_widget ("DirectionalLightDirectionXSpinButton", m_DirectionalLightDirectionXSpinButton);
	m_DirectionalLightDirectionXSpinButton -> set_name ("DirectionalLightDirectionXSpinButton");
	m_builder -> get_widget ("DirectionalLightDirectionYSpinButton", m_DirectionalLightDirectionYSpinButton);
	m_DirectionalLightDirectionYSpinButton -> set_name ("DirectionalLightDirectionYSpinButton");
	m_builder -> get_widget ("DirectionalLightDirectionZSpinButton", m_DirectionalLightDirectionZSpinButton);
	m_DirectionalLightDirectionZSpinButton -> set_name ("DirectionalLightDirectionZSpinButton");
	m_builder -> get_widget ("DirectionalLightNormalToolBox", m_DirectionalLightNormalToolBox);
	m_DirectionalLightNormalToolBox -> set_name ("DirectionalLightNormalToolBox");
	m_builder -> get_widget ("PointLightExpander", m_PointLightExpander);
	m_PointLightExpander -> set_name ("PointLightExpander");
	m_builder -> get_widget ("PointLightLocationBox", m_PointLightLocationBox);
	m_PointLightLocationBox -> set_name ("PointLightLocationBox");
	m_builder -> get_widget ("PointLightLocationXSpinButton", m_PointLightLocationXSpinButton);
	m_PointLightLocationXSpinButton -> set_name ("PointLightLocationXSpinButton");
	m_builder -> get_widget ("PointLightLocationYSpinButton", m_PointLightLocationYSpinButton);
	m_PointLightLocationYSpinButton -> set_name ("PointLightLocationYSpinButton");
	m_builder -> get_widget ("PointLightLocationZSpinButton", m_PointLightLocationZSpinButton);
	m_PointLightLocationZSpinButton -> set_name ("PointLightLocationZSpinButton");
	m_builder -> get_widget ("PointLightAttenuationBox", m_PointLightAttenuationBox);
	m_PointLightAttenuationBox -> set_name ("PointLightAttenuationBox");
	m_builder -> get_widget ("PointLightAttenuationXSpinButton", m_PointLightAttenuationXSpinButton);
	m_PointLightAttenuationXSpinButton -> set_name ("PointLightAttenuationXSpinButton");
	m_builder -> get_widget ("PointLightAttenuationYSpinButton", m_PointLightAttenuationYSpinButton);
	m_PointLightAttenuationYSpinButton -> set_name ("PointLightAttenuationYSpinButton");
	m_builder -> get_widget ("PointLightAttenuationZSpinButton", m_PointLightAttenuationZSpinButton);
	m_PointLightAttenuationZSpinButton -> set_name ("PointLightAttenuationZSpinButton");
	m_builder -> get_widget ("PointLightRadiusSpinButton", m_PointLightRadiusSpinButton);
	m_PointLightRadiusSpinButton -> set_name ("PointLightRadiusSpinButton");
	m_builder -> get_widget ("SpotLightExpander", m_SpotLightExpander);
	m_SpotLightExpander -> set_name ("SpotLightExpander");
	m_builder -> get_widget ("SpotLightAttenuationBox", m_SpotLightAttenuationBox);
	m_SpotLightAttenuationBox -> set_name ("SpotLightAttenuationBox");
	m_builder -> get_widget ("SpotLightAttenuationXSpinButton", m_SpotLightAttenuationXSpinButton);
	m_SpotLightAttenuationXSpinButton -> set_name ("SpotLightAttenuationXSpinButton");
	m_builder -> get_widget ("SpotLightAttenuationYSpinButton", m_SpotLightAttenuationYSpinButton);
	m_SpotLightAttenuationYSpinButton -> set_name ("SpotLightAttenuationYSpinButton");
	m_builder -> get_widget ("SpotLightAttenuationZSpinButton", m_SpotLightAttenuationZSpinButton);
	m_SpotLightAttenuationZSpinButton -> set_name ("SpotLightAttenuationZSpinButton");
	m_builder -> get_widget ("SpotLightLocationBox", m_SpotLightLocationBox);
	m_SpotLightLocationBox -> set_name ("SpotLightLocationBox");
	m_builder -> get_widget ("SpotLightLocationXSpinButton", m_SpotLightLocationXSpinButton);
	m_SpotLightLocationXSpinButton -> set_name ("SpotLightLocationXSpinButton");
	m_builder -> get_widget ("SpotLightLocationYSpinButton", m_SpotLightLocationYSpinButton);
	m_SpotLightLocationYSpinButton -> set_name ("SpotLightLocationYSpinButton");
	m_builder -> get_widget ("SpotLightLocationZSpinButton", m_SpotLightLocationZSpinButton);
	m_SpotLightLocationZSpinButton -> set_name ("SpotLightLocationZSpinButton");
	m_builder -> get_widget ("SpotLightRadiusSpinButton", m_SpotLightRadiusSpinButton);
	m_SpotLightRadiusSpinButton -> set_name ("SpotLightRadiusSpinButton");
	m_builder -> get_widget ("SpotLightBeamWidthSpinButton", m_SpotLightBeamWidthSpinButton);
	m_SpotLightBeamWidthSpinButton -> set_name ("SpotLightBeamWidthSpinButton");
	m_builder -> get_widget ("SpotLightCutOffAngleSpinButton", m_SpotLightCutOffAngleSpinButton);
	m_SpotLightCutOffAngleSpinButton -> set_name ("SpotLightCutOffAngleSpinButton");
	m_builder -> get_widget ("SpotLightDirectionBox", m_SpotLightDirectionBox);
	m_SpotLightDirectionBox -> set_name ("SpotLightDirectionBox");
	m_builder -> get_widget ("SpotLightDirectionXSpinButton", m_SpotLightDirectionXSpinButton);
	m_SpotLightDirectionXSpinButton -> set_name ("SpotLightDirectionXSpinButton");
	m_builder -> get_widget ("SpotLightDirectionYSpinButton", m_SpotLightDirectionYSpinButton);
	m_SpotLightDirectionYSpinButton -> set_name ("SpotLightDirectionYSpinButton");
	m_builder -> get_widget ("SpotLightDirectionZSpinButton", m_SpotLightDirectionZSpinButton);
	m_SpotLightDirectionZSpinButton -> set_name ("SpotLightDirectionZSpinButton");
	m_builder -> get_widget ("SpotLightNormalToolBox", m_SpotLightNormalToolBox);
	m_SpotLightNormalToolBox -> set_name ("SpotLightNormalToolBox");

	// Call construct handler of base class.
	construct ();
}

X3DLightEditorInterface::~X3DLightEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
