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
#ifndef __TMP_GLAD2CPP_SOUND_EDITOR_H__
#define __TMP_GLAD2CPP_SOUND_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for SoundEditor.
 */
class X3DSoundEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DSoundEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DSoundEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	virtual
	const std::string &
	getWidgetName () const
	{ return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundDirectionXAdjustment () const
	{ return m_SoundDirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundDirectionYAdjustment () const
	{ return m_SoundDirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundDirectionZAdjustment () const
	{ return m_SoundDirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundIntensityAdjustment () const
	{ return m_SoundIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundLocationXAdjustment () const
	{ return m_SoundLocationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundLocationYAdjustment () const
	{ return m_SoundLocationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundLocationZAdjustment () const
	{ return m_SoundLocationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMaxBackAdjustment () const
	{ return m_SoundMaxBackAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMaxFrontAdjustment () const
	{ return m_SoundMaxFrontAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMinBackAdjustment () const
	{ return m_SoundMinBackAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundMinFrontAdjustment () const
	{ return m_SoundMinFrontAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSoundPriorityAdjustment () const
	{ return m_SoundPriorityAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Box &
	getSoundActionBox () const
	{ return *m_SoundActionBox; }

	Gtk::Button &
	getNewSoundButton () const
	{ return *m_NewSoundButton; }

	Gtk::Button &
	getRemoveSoundButton () const
	{ return *m_RemoveSoundButton; }

	Gtk::Button &
	getIndexButton () const
	{ return *m_IndexButton; }

	Gtk::Notebook &
	getSoundNotebook () const
	{ return *m_SoundNotebook; }

	Gtk::Expander &
	getSoundExpander () const
	{ return *m_SoundExpander; }

	Gtk::Grid &
	getSoundBox () const
	{ return *m_SoundBox; }

	Gtk::CheckButton &
	getSoundSpatializeCheckButton () const
	{ return *m_SoundSpatializeCheckButton; }

	Gtk::Box &
	getSoundNameBox () const
	{ return *m_SoundNameBox; }

	Gtk::Entry &
	getSoundNameEntry () const
	{ return *m_SoundNameEntry; }

	Gtk::Button &
	getSoundRenameButton () const
	{ return *m_SoundRenameButton; }

	Gtk::Box &
	getSoundIntensityBox () const
	{ return *m_SoundIntensityBox; }

	Gtk::Scale &
	getSoundIntensityScale () const
	{ return *m_SoundIntensityScale; }

	Gtk::Box &
	getSoundLocationBox () const
	{ return *m_SoundLocationBox; }

	Gtk::Box &
	getSoundNormalToolBox () const
	{ return *m_SoundNormalToolBox; }

	Gtk::Box &
	getSoundDirectionBox () const
	{ return *m_SoundDirectionBox; }

	Gtk::SpinButton &
	getSoundDirectionXSpinButton () const
	{ return *m_SoundDirectionXSpinButton; }

	Gtk::SpinButton &
	getSoundDirectionYSpinButton () const
	{ return *m_SoundDirectionYSpinButton; }

	Gtk::SpinButton &
	getSoundDirectionZSpinButton () const
	{ return *m_SoundDirectionZSpinButton; }

	Gtk::Box &
	getSoundPriorityBox () const
	{ return *m_SoundPriorityBox; }

	Gtk::Scale &
	getSoundPriorityScale () const
	{ return *m_SoundPriorityScale; }

	Gtk::SpinButton &
	getSoundMinBackSpinButton () const
	{ return *m_SoundMinBackSpinButton; }

	Gtk::SpinButton &
	getSoundMinFrontSpinButton () const
	{ return *m_SoundMinFrontSpinButton; }

	Gtk::SpinButton &
	getSoundMaxBackSpinButton () const
	{ return *m_SoundMaxBackSpinButton; }

	Gtk::SpinButton &
	getSoundMaxFrontSpinButton () const
	{ return *m_SoundMaxFrontSpinButton; }

	Gtk::Expander &
	getSoundSourceExpander () const
	{ return *m_SoundSourceExpander; }

	Gtk::Grid &
	getSoundSourceBox () const
	{ return *m_SoundSourceBox; }

	///  @name Signal handlers

	virtual
	void
	on_new_sound_clicked () = 0;

	virtual
	void
	on_remove_sound_clicked () = 0;

	virtual
	void
	on_index_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DSoundEditorInterface ();


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DEditorInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	static const std::string m_widgetName;

	///  @name Members

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_SoundDirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundDirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundDirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundLocationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundLocationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundLocationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundMaxBackAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundMaxFrontAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundMinBackAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundMinFrontAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SoundPriorityAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Box*                      m_SoundActionBox;
	Gtk::Button*                   m_NewSoundButton;
	Gtk::Button*                   m_RemoveSoundButton;
	Gtk::Button*                   m_IndexButton;
	Gtk::Notebook*                 m_SoundNotebook;
	Gtk::Expander*                 m_SoundExpander;
	Gtk::Grid*                     m_SoundBox;
	Gtk::CheckButton*              m_SoundSpatializeCheckButton;
	Gtk::Box*                      m_SoundNameBox;
	Gtk::Entry*                    m_SoundNameEntry;
	Gtk::Button*                   m_SoundRenameButton;
	Gtk::Box*                      m_SoundIntensityBox;
	Gtk::Scale*                    m_SoundIntensityScale;
	Gtk::Box*                      m_SoundLocationBox;
	Gtk::Box*                      m_SoundNormalToolBox;
	Gtk::Box*                      m_SoundDirectionBox;
	Gtk::SpinButton*               m_SoundDirectionXSpinButton;
	Gtk::SpinButton*               m_SoundDirectionYSpinButton;
	Gtk::SpinButton*               m_SoundDirectionZSpinButton;
	Gtk::Box*                      m_SoundPriorityBox;
	Gtk::Scale*                    m_SoundPriorityScale;
	Gtk::SpinButton*               m_SoundMinBackSpinButton;
	Gtk::SpinButton*               m_SoundMinFrontSpinButton;
	Gtk::SpinButton*               m_SoundMaxBackSpinButton;
	Gtk::SpinButton*               m_SoundMaxFrontSpinButton;
	Gtk::Expander*                 m_SoundSourceExpander;
	Gtk::Grid*                     m_SoundSourceBox;

};

} // puck
} // titania

#endif
