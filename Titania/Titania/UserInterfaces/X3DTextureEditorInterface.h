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
#ifndef __TMP_GLAD2CPP_TEXTURE_EDITOR_H__
#define __TMP_GLAD2CPP_TEXTURE_EDITOR_H__

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DTextureEditorInterface :
	public X3DDialogInterface
{
public:

	template <class ... Arguments>
	X3DTextureEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const std::string & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getAlphaAdjustment () const
	{ return m_alphaAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getAnisotropicDegreeAdjustment () const
	{ return m_anisotropicDegreeAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getBorderWidthAdjustment () const
	{ return m_borderWidthAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getBoundryModeListStore () const
	{ return m_boundryModeListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getFunctionListStore () const
	{ return m_functionListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMagnificationFilterListStore () const
	{ return m_magnificationFilterListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getMinificationFilterListStore () const
	{ return m_minificationFilterListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getModeListStore () const
	{ return m_modeListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getRotationAdjustment () const
	{ return m_rotationAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getScaleSAdjustment () const
	{ return m_scaleSAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getScaleTdjustment () const
	{ return m_scaleTdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getSourceListStore () const
	{ return m_sourceListStore; }

	const Glib::RefPtr <Gtk::ListStore> &
	getTextureCompressionListStore () const
	{ return m_textureCompressionListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTexturePriorityAdjustment () const
	{ return m_texturePriorityAdjustment; }

	const Glib::RefPtr <Gtk::ListStore> &
	getTextureUnitListStore () const
	{ return m_textureUnitListStore; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslateSAdjustment () const
	{ return m_translateSAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getTranslateTAdjustment () const
	{ return m_translateTAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_window; }

	Gtk::Box &
	getWidget () const
	{ return *m_widget; }

	Gtk::ImageMenuItem &
	getCopyMenuItem () const
	{ return *m_copyMenuItem; }

	Gtk::ImageMenuItem &
	getPasteMenuItem () const
	{ return *m_pasteMenuItem; }

	Gtk::Box &
	getPreviewBox () const
	{ return *m_previewBox; }

	Gtk::Expander &
	getMultiTextureExpander () const
	{ return *m_multiTextureExpander; }

	Gtk::Expander &
	getTextureExpander () const
	{ return *m_textureExpander; }

	Gtk::Grid &
	getTextureGrid () const
	{ return *m_textureGrid; }

	Gtk::Expander &
	getTexturePropertiesExpander () const
	{ return *m_texturePropertiesExpander; }

	Gtk::Expander &
	getTextureTransformExpander () const
	{ return *m_textureTransformExpander; }

	Gtk::Grid &
	getTextureTranformGrid () const
	{ return *m_textureTranformGrid; }

	Gtk::SpinButton &
	getTranslateSButton () const
	{ return *m_translateSButton; }

	Gtk::SpinButton &
	getTranslateTButton () const
	{ return *m_translateTButton; }

	Gtk::Scale &
	getRotationScale () const
	{ return *m_rotationScale; }

	Gtk::SpinButton &
	getScaleSButton () const
	{ return *m_scaleSButton; }

	Gtk::SpinButton &
	getScaleTButton () const
	{ return *m_scaleTButton; }

	virtual
	~X3DTextureEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_alphaAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_anisotropicDegreeAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_borderWidthAdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_boundryModeListStore;
	Glib::RefPtr <Gtk::ListStore>  m_functionListStore;
	Glib::RefPtr <Gtk::ListStore>  m_magnificationFilterListStore;
	Glib::RefPtr <Gtk::ListStore>  m_minificationFilterListStore;
	Glib::RefPtr <Gtk::ListStore>  m_modeListStore;
	Glib::RefPtr <Gtk::Adjustment> m_rotationAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_scaleSAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_scaleTdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_sourceListStore;
	Glib::RefPtr <Gtk::ListStore>  m_textureCompressionListStore;
	Glib::RefPtr <Gtk::Adjustment> m_texturePriorityAdjustment;
	Glib::RefPtr <Gtk::ListStore>  m_textureUnitListStore;
	Glib::RefPtr <Gtk::Adjustment> m_translateSAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_translateTAdjustment;
	Gtk::Window*                   m_window;
	Gtk::Box*                      m_widget;
	Gtk::ImageMenuItem*            m_copyMenuItem;
	Gtk::ImageMenuItem*            m_pasteMenuItem;
	Gtk::Box*                      m_previewBox;
	Gtk::Expander*                 m_multiTextureExpander;
	Gtk::Expander*                 m_textureExpander;
	Gtk::Grid*                     m_textureGrid;
	Gtk::Expander*                 m_texturePropertiesExpander;
	Gtk::Expander*                 m_textureTransformExpander;
	Gtk::Grid*                     m_textureTranformGrid;
	Gtk::SpinButton*               m_translateSButton;
	Gtk::SpinButton*               m_translateTButton;
	Gtk::Scale*                    m_rotationScale;
	Gtk::SpinButton*               m_scaleSButton;
	Gtk::SpinButton*               m_scaleTButton;

};

} // puck
} // titania

#endif
