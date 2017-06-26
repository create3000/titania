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

#ifndef __TITANIA_COMPOSED_WIDGETS_MFCOLOR_BUTTON_H__
#define __TITANIA_COMPOSED_WIDGETS_MFCOLOR_BUTTON_H__

#include "../ComposedWidgets/X3DComposedWidget.h"

namespace titania {
namespace puck {

class MFColorButton :
	public X3DComposedWidget
{
public:

	///  @name Construction

	MFColorButton (X3DBaseInterface* const,
	               Gtk::Button &,
	               const Glib::RefPtr <Gtk::Adjustment> &,
	               Gtk::Widget &,
	               Gtk::Button &,
	               Gtk::Button &,
	               Gtk::ScrolledWindow &,
	               const std::string &);

	///  @name Signals

	sigc::signal<void> &
	signal_index_changed ()
	{ return index_changed; }

	const sigc::signal<void> &
	signal_index_changed () const
	{ return index_changed; }

	///  @name Member access

	void
	setColorsSize (const int value)
	{ colorsSize = value; }

	int
	getColorsSize () const
	{ return colorsSize; }

	void
	setColorsGap (const int value)
	{ colorsGap = value; }

	int
	getColorsGap () const
	{ return colorsGap; }

	void
	setColorsBorder (const X3D::Vector4i value)
	{ colorsBorder = value; }

	const X3D::Vector4i &
	getColorsBorder () const
	{ return colorsBorder; }

	void
	setIndex (const int32_t);

	int32_t
	getIndex () const
	{ return index; }

	bool
	getHide () const
	{ return hide; }

	void
	setHide (const int value)
	{ hide = value; }

	void
	setNodes (const X3D::MFNode &);

	const X3D::MFNode &
	getNodes ()
	{ return nodes; }

	///  @name Destruction

	virtual
	~MFColorButton () final override;


private:

	///  @name Event handlers

	void
	on_color_changed ();

	void
	on_value_changed ();

	void
	set_color (const int, const X3D::Color3f &);

	void
	set_field ();

	void
	set_buffer ();

	void
	connect (const X3D::MFColor &);

	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> &);

	void
	on_clicked ();

	Gdk::RGBA
	to_rgba (const X3D::Color3f &);

	void
	on_add_color_clicked ();

	void
	on_remove_color_clicked ();

	bool
	on_colors_configure_event (GdkEventConfigure* const);

	bool
	on_colors_key_press_event (GdkEventKey*);

	bool
	on_colors_button_release_event (GdkEventButton*);

	bool
	on_colors_draw (const Cairo::RefPtr <Cairo::Context> &);

	size_t
	getColumns (const double, const double, const double, const X3D::Vector4i &);

	bool
	on_button_press_event (GdkEventButton*);

	void
	on_copy ();

	void
	on_paste ();

	///  @name Members

	sigc::signal<void>                   index_changed;
	Gtk::Button &                        colorButton;
	const Glib::RefPtr <Gtk::Adjustment> valueAdjustment;
	Gtk::Widget &                        widget;
	Gtk::Button &                        addColorButton;
	Gtk::Button &                        removeColorButton;
	Gtk::ScrolledWindow &                colorsScrolledWindow;
	Gtk::DrawingArea                     drawingArea;
	Gtk::ColorSelectionDialog            dialog;
	Gtk::DrawingArea                     colorsDrawingArea;
	Gtk::Menu                            menu;
	X3D::MFNode                          nodes;
	X3D::SFNode                          node;
	const std::string                    name;
	int32_t                              index;
	bool                                 hide;
	X3D::UndoStepPtr                     undoStep;
	int                                  input;
	bool                                 changing;
	X3D::SFTime                          buffer;
	X3D::Vector3f                        hsv;
	int                                  colorsSize;
	int                                  colorsGap;
	X3D::Vector4i                        colorsBorder;
	X3D::ClipboardPtr                    clipboard;

};

} // puck
} // titania

#endif
