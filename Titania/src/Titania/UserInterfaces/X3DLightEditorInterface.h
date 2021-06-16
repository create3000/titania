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

#ifndef __TMP_GLAD2CPP_LIGHT_EDITOR_H__
#define __TMP_GLAD2CPP_LIGHT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for LightEditor.
 */
class X3DLightEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DLightEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DLightEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DLightEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAmbientIntensityAdjustment () const
	{ return m_AmbientIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionalLightDirectionXAdjustment () const
	{ return m_DirectionalLightDirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionalLightDirectionYAdjustment () const
	{ return m_DirectionalLightDirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionalLightDirectionZAdjustment () const
	{ return m_DirectionalLightDirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIntensityAdjustment () const
	{ return m_IntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightAttenuationXAdjustment () const
	{ return m_PointLightAttenuationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightAttenuationYAdjustment () const
	{ return m_PointLightAttenuationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightAttenuationZAdjustment () const
	{ return m_PointLightAttenuationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightLocationXAdjustment () const
	{ return m_PointLightLocationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightLocationYAdjustment () const
	{ return m_PointLightLocationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightLocationZAdjustment () const
	{ return m_PointLightLocationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightRadiusAdjustment () const
	{ return m_PointLightRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShadowBiasAdjustment () const
	{ return m_ShadowBiasAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShadowColorAdjustment () const
	{ return m_ShadowColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShadowIntensityAdjustment () const
	{ return m_ShadowIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShadowMapSizeAdjustment () const
	{ return m_ShadowMapSizeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightAttenuationXAdjustment () const
	{ return m_SpotLightAttenuationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightAttenuationYAdjustment () const
	{ return m_SpotLightAttenuationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightAttenuationZAdjustment () const
	{ return m_SpotLightAttenuationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightBeamWidthAdjustment () const
	{ return m_SpotLightBeamWidthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightCutOffAngleAdjustment () const
	{ return m_SpotLightCutOffAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightDirectionXAdjustment () const
	{ return m_SpotLightDirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightDirectionYAdjustment () const
	{ return m_SpotLightDirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightDirectionZAdjustment () const
	{ return m_SpotLightDirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightLocationXAdjustment () const
	{ return m_SpotLightLocationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightLocationYAdjustment () const
	{ return m_SpotLightLocationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightLocationZAdjustment () const
	{ return m_SpotLightLocationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightRadiusAdjustment () const
	{ return m_SpotLightRadiusAdjustment; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Button &
	getNewLightPopupButton () const
	{ return *m_NewLightPopupButton; }

	Gtk::Button &
	getRemoveLightButton () const
	{ return *m_RemoveLightButton; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Expander &
	getLightExpander () const
	{ return *m_LightExpander; }

	Gtk::Grid &
	getLightBox () const
	{ return *m_LightBox; }

	Gtk::CheckButton &
	getGlobalCheckButton () const
	{ return *m_GlobalCheckButton; }

	Gtk::CheckButton &
	getOnCheckButton () const
	{ return *m_OnCheckButton; }

	Gtk::Box &
	getColorBox () const
	{ return *m_ColorBox; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	Gtk::Box &
	getIntensityBox () const
	{ return *m_IntensityBox; }

	Gtk::Box &
	getAmbientIntensityBox () const
	{ return *m_AmbientIntensityBox; }

	Gtk::Box &
	getNameBox () const
	{ return *m_NameBox; }

	Gtk::Entry &
	getNameEntry () const
	{ return *m_NameEntry; }

	Gtk::Button &
	getRenameButton () const
	{ return *m_RenameButton; }

	Gtk::Expander &
	getDirectionalLightExpander () const
	{ return *m_DirectionalLightExpander; }

	Gtk::Box &
	getDirectionalLightNormalToolBox () const
	{ return *m_DirectionalLightNormalToolBox; }

	Gtk::Box &
	getDirectionalLightDirectionBox () const
	{ return *m_DirectionalLightDirectionBox; }

	Gtk::SpinButton &
	getDirectionalLightDirectionXSpinButton () const
	{ return *m_DirectionalLightDirectionXSpinButton; }

	Gtk::SpinButton &
	getDirectionalLightDirectionYSpinButton () const
	{ return *m_DirectionalLightDirectionYSpinButton; }

	Gtk::SpinButton &
	getDirectionalLightDirectionZSpinButton () const
	{ return *m_DirectionalLightDirectionZSpinButton; }

	Gtk::Expander &
	getPointLightExpander () const
	{ return *m_PointLightExpander; }

	Gtk::Box &
	getPointLightLocationBox () const
	{ return *m_PointLightLocationBox; }

	Gtk::SpinButton &
	getPointLightLocationXSpinButton () const
	{ return *m_PointLightLocationXSpinButton; }

	Gtk::SpinButton &
	getPointLightLocationYSpinButton () const
	{ return *m_PointLightLocationYSpinButton; }

	Gtk::SpinButton &
	getPointLightLocationZSpinButton () const
	{ return *m_PointLightLocationZSpinButton; }

	Gtk::Box &
	getPointLightAttenuationBox () const
	{ return *m_PointLightAttenuationBox; }

	Gtk::SpinButton &
	getPointLightAttenuationXSpinButton () const
	{ return *m_PointLightAttenuationXSpinButton; }

	Gtk::SpinButton &
	getPointLightAttenuationYSpinButton () const
	{ return *m_PointLightAttenuationYSpinButton; }

	Gtk::SpinButton &
	getPointLightAttenuationZSpinButton () const
	{ return *m_PointLightAttenuationZSpinButton; }

	Gtk::SpinButton &
	getPointLightRadiusSpinButton () const
	{ return *m_PointLightRadiusSpinButton; }

	Gtk::Expander &
	getSpotLightExpander () const
	{ return *m_SpotLightExpander; }

	Gtk::Box &
	getSpotLightAttenuationBox () const
	{ return *m_SpotLightAttenuationBox; }

	Gtk::SpinButton &
	getSpotLightAttenuationXSpinButton () const
	{ return *m_SpotLightAttenuationXSpinButton; }

	Gtk::SpinButton &
	getSpotLightAttenuationYSpinButton () const
	{ return *m_SpotLightAttenuationYSpinButton; }

	Gtk::SpinButton &
	getSpotLightAttenuationZSpinButton () const
	{ return *m_SpotLightAttenuationZSpinButton; }

	Gtk::Box &
	getSpotLightLocationBox () const
	{ return *m_SpotLightLocationBox; }

	Gtk::SpinButton &
	getSpotLightLocationXSpinButton () const
	{ return *m_SpotLightLocationXSpinButton; }

	Gtk::SpinButton &
	getSpotLightLocationYSpinButton () const
	{ return *m_SpotLightLocationYSpinButton; }

	Gtk::SpinButton &
	getSpotLightLocationZSpinButton () const
	{ return *m_SpotLightLocationZSpinButton; }

	Gtk::SpinButton &
	getSpotLightRadiusSpinButton () const
	{ return *m_SpotLightRadiusSpinButton; }

	Gtk::Box &
	getSpotLightNormalToolBox () const
	{ return *m_SpotLightNormalToolBox; }

	Gtk::Box &
	getSpotLightDirectionBox () const
	{ return *m_SpotLightDirectionBox; }

	Gtk::SpinButton &
	getSpotLightDirectionXSpinButton () const
	{ return *m_SpotLightDirectionXSpinButton; }

	Gtk::SpinButton &
	getSpotLightDirectionYSpinButton () const
	{ return *m_SpotLightDirectionYSpinButton; }

	Gtk::SpinButton &
	getSpotLightDirectionZSpinButton () const
	{ return *m_SpotLightDirectionZSpinButton; }

	Gtk::Box &
	getSpotLightBeamWidthBox () const
	{ return *m_SpotLightBeamWidthBox; }

	Gtk::SpinButton &
	getSpotLightBeamWidthSpinButton () const
	{ return *m_SpotLightBeamWidthSpinButton; }

	Gtk::SpinButton &
	getSpotLightCutOffAngleSpinButton () const
	{ return *m_SpotLightCutOffAngleSpinButton; }

	Gtk::Expander &
	getShadowExpander () const
	{ return *m_ShadowExpander; }

	Gtk::Grid &
	getLightBox1 () const
	{ return *m_LightBox1; }

	Gtk::Box &
	getShadowIntensityBox () const
	{ return *m_ShadowIntensityBox; }

	Gtk::SpinButton &
	getShadowMapSizeSpinButton () const
	{ return *m_ShadowMapSizeSpinButton; }

	Gtk::Box &
	getShadowColorBox () const
	{ return *m_ShadowColorBox; }

	Gtk::Button &
	getShadowColorButton () const
	{ return *m_ShadowColorButton; }

	Gtk::Box &
	getShadowBiasBox () const
	{ return *m_ShadowBiasBox; }

	Gtk::SpinButton &
	getShadowBiasSpinButton () const
	{ return *m_ShadowBiasSpinButton; }

	Gtk::Popover &
	getNewLightPopover () const
	{ return *m_NewLightPopover; }

	Gtk::Button &
	getNewDirectionalLightButton () const
	{ return *m_NewDirectionalLightButton; }

	Gtk::Button &
	getNewPointLightButton () const
	{ return *m_NewPointLightButton; }

	Gtk::Button &
	getNewSpotLightButton () const
	{ return *m_NewSpotLightButton; }

	///  @name Signal handlers

	virtual
	void
	on_spot_light_beam_width_value_changed () = 0;

	virtual
	void
	on_spot_light_cut_of_angle_value_changed () = 0;

	virtual
	void
	on_new_light_popup_clicked () = 0;

	virtual
	void
	on_remove_light_clicked () = 0;

	virtual
	void
	on_index_clicked () = 0;

	virtual
	void
	on_new_directional_light_clicked () = 0;

	virtual
	void
	on_new_point_light_clicked () = 0;

	virtual
	void
	on_new_spot_light_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DLightEditorInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_AmbientIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionalLightDirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionalLightDirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionalLightDirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightAttenuationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightAttenuationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightAttenuationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightLocationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightLocationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightLocationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShadowBiasAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShadowColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShadowIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShadowMapSizeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightAttenuationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightAttenuationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightAttenuationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightBeamWidthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightCutOffAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightDirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightDirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightDirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightLocationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightLocationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightLocationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightRadiusAdjustment;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Button* m_NewLightPopupButton;
	Gtk::Button* m_RemoveLightButton;
	Gtk::Button* m_IndexButton;
	Gtk::Expander* m_LightExpander;
	Gtk::Grid* m_LightBox;
	Gtk::CheckButton* m_GlobalCheckButton;
	Gtk::CheckButton* m_OnCheckButton;
	Gtk::Box* m_ColorBox;
	Gtk::Button* m_ColorButton;
	Gtk::Box* m_IntensityBox;
	Gtk::Box* m_AmbientIntensityBox;
	Gtk::Box* m_NameBox;
	Gtk::Entry* m_NameEntry;
	Gtk::Button* m_RenameButton;
	Gtk::Expander* m_DirectionalLightExpander;
	Gtk::Box* m_DirectionalLightNormalToolBox;
	Gtk::Box* m_DirectionalLightDirectionBox;
	Gtk::SpinButton* m_DirectionalLightDirectionXSpinButton;
	Gtk::SpinButton* m_DirectionalLightDirectionYSpinButton;
	Gtk::SpinButton* m_DirectionalLightDirectionZSpinButton;
	Gtk::Expander* m_PointLightExpander;
	Gtk::Box* m_PointLightLocationBox;
	Gtk::SpinButton* m_PointLightLocationXSpinButton;
	Gtk::SpinButton* m_PointLightLocationYSpinButton;
	Gtk::SpinButton* m_PointLightLocationZSpinButton;
	Gtk::Box* m_PointLightAttenuationBox;
	Gtk::SpinButton* m_PointLightAttenuationXSpinButton;
	Gtk::SpinButton* m_PointLightAttenuationYSpinButton;
	Gtk::SpinButton* m_PointLightAttenuationZSpinButton;
	Gtk::SpinButton* m_PointLightRadiusSpinButton;
	Gtk::Expander* m_SpotLightExpander;
	Gtk::Box* m_SpotLightAttenuationBox;
	Gtk::SpinButton* m_SpotLightAttenuationXSpinButton;
	Gtk::SpinButton* m_SpotLightAttenuationYSpinButton;
	Gtk::SpinButton* m_SpotLightAttenuationZSpinButton;
	Gtk::Box* m_SpotLightLocationBox;
	Gtk::SpinButton* m_SpotLightLocationXSpinButton;
	Gtk::SpinButton* m_SpotLightLocationYSpinButton;
	Gtk::SpinButton* m_SpotLightLocationZSpinButton;
	Gtk::SpinButton* m_SpotLightRadiusSpinButton;
	Gtk::Box* m_SpotLightNormalToolBox;
	Gtk::Box* m_SpotLightDirectionBox;
	Gtk::SpinButton* m_SpotLightDirectionXSpinButton;
	Gtk::SpinButton* m_SpotLightDirectionYSpinButton;
	Gtk::SpinButton* m_SpotLightDirectionZSpinButton;
	Gtk::Box* m_SpotLightBeamWidthBox;
	Gtk::SpinButton* m_SpotLightBeamWidthSpinButton;
	Gtk::SpinButton* m_SpotLightCutOffAngleSpinButton;
	Gtk::Expander* m_ShadowExpander;
	Gtk::Grid* m_LightBox1;
	Gtk::Box* m_ShadowIntensityBox;
	Gtk::SpinButton* m_ShadowMapSizeSpinButton;
	Gtk::Box* m_ShadowColorBox;
	Gtk::Button* m_ShadowColorButton;
	Gtk::Box* m_ShadowBiasBox;
	Gtk::SpinButton* m_ShadowBiasSpinButton;
	Gtk::Popover* m_NewLightPopover;
	Gtk::Button* m_NewDirectionalLightButton;
	Gtk::Button* m_NewPointLightButton;
	Gtk::Button* m_NewSpotLightButton;

};

} // puck
} // titania

#endif
