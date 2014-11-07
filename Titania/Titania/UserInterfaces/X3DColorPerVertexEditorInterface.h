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
#ifndef __TMP_GLAD2CPP_COLOR_PER_VERTEX_EDITOR_H__
#define __TMP_GLAD2CPP_COLOR_PER_VERTEX_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DColorPerVertexEditorInterface :
	public X3DEditorInterface
{
public:

	X3DColorPerVertexEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DColorPerVertexEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getAmbientIntensityAdjustment () const
	{ return m_AmbientIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackAmbientIntensityAdjustment () const
	{ return m_BackAmbientIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackDiffuseColorAdjustment () const
	{ return m_BackDiffuseColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackEmissiveColorAdjustment () const
	{ return m_BackEmissiveColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackShininessAdjustment () const
	{ return m_BackShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackSpecularColorAdjustment () const
	{ return m_BackSpecularColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBackTransparencyAdjustment () const
	{ return m_BackTransparencyAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDiffuseColorAdjustment () const
	{ return m_DiffuseColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getEmissiveColorAdjustment () const
	{ return m_EmissiveColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFillPropertiesHatchColorAdjustment () const
	{ return m_FillPropertiesHatchColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getFillPropertiesHatchStyleAdjustment () const
	{ return m_FillPropertiesHatchStyleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLinePropertiesLinetypeAdjustment () const
	{ return m_LinePropertiesLinetypeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getLinePropertiesLinewidthScaleFactorAdjustment () const
	{ return m_LinePropertiesLinewidthScaleFactorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getShininessAdjustment () const
	{ return m_ShininessAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpecularColorAdjustment () const
	{ return m_SpecularColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTransparencyAdjustment () const
	{ return m_TransparencyAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_PreviewBox; }

	Gtk::ToolButton &
	getLookAtButton () const
	{ return *m_LookAtButton; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	virtual
	void
	on_look_at_all_clicked () = 0;

	virtual
	~X3DColorPerVertexEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_AmbientIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackAmbientIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackDiffuseColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackEmissiveColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackSpecularColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_BackTransparencyAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DiffuseColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_EmissiveColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_FillPropertiesHatchColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_FillPropertiesHatchStyleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LinePropertiesLinetypeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_LinePropertiesLinewidthScaleFactorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ShininessAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpecularColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_TransparencyAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Box*                      m_PreviewBox;
	Gtk::ToolButton*               m_LookAtButton;
	Gtk::Button*                   m_ColorButton;

};

} // puck
} // titania

#endif
