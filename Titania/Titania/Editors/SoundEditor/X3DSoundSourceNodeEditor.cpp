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

#include "X3DSoundSourceNodeEditor.h"

#include "../../Bits/String.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../ComposedWidgets/MFStringURLWidget.h"

namespace titania {
namespace puck {

X3DSoundSourceNodeEditor::X3DSoundSourceNodeEditor () :
	X3DSoundEditorInterface (),
	                enabled (this, getSoundSourceEnabledCheckButton (), "enabled"),
	            description (this, getSoundSourceDescriptionTextView (), "description"),
	                    url (new MFStringURLWidget (this,
	                         getSoundSourceURLTreeView (),
	                         getSoundSourceURLCellRendererText (),
	                         getSoundSourceURLAddButton (),
	                         getSoundSourceURLRemoveButton (),
	                         getSoundSourceURLReloadButton (),
	                         getSoundSourceURLChooserColumn (),
	                         "url")),
	                  speed (this,
	                         getSoundSourceSpeedAdjustment (),
	                         getSoundSourceSpeedSpinButton (),
	                         "speed"),
	                  pitch (this,
	                         getSoundSourcePitchAdjustment (),
	                         getSoundSourcePitchSpinButton (),
	                         "pitch"),
	                   loop (this, getSoundSourceLoopToggleButton (), "loop"),
	                 sounds (),
	  soundSourceNodeBuffer (),
	        soundSourceNode (),
	              audioClip (),
	           movieTexture (),
	               undoStep (),
	               changing (false)
{
	addChildObjects (sounds, soundSourceNodeBuffer, soundSourceNode, audioClip, movieTexture);

	getSoundSourceLoopToggleButton () .signal_toggled () .connect (sigc::mem_fun (this, &X3DSoundSourceNodeEditor::on_sound_source_loop_toggled));
}

void
X3DSoundSourceNodeEditor::initialize ()
{
	soundSourceNodeBuffer .addInterest (&X3DSoundSourceNodeEditor::set_node, this);
}

void
X3DSoundSourceNodeEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & sound : sounds)
		sound -> source () .removeInterest (&X3DSoundSourceNodeEditor::set_source, this);

	sounds = getNodes <X3D::Sound> (selection, { X3D::X3DConstants::Sound });

	for (const auto & sound : sounds)
		sound -> source () .addInterest (&X3DSoundSourceNodeEditor::set_source, this);

	set_source ();
}

/***********************************************************************************************************************
 *
 *  sound source
 *
 **********************************************************************************************************************/

void
X3DSoundSourceNodeEditor::on_sound_source_unlink_clicked ()
{
	unlinkClone (sounds, "source", undoStep);
}

void
X3DSoundSourceNodeEditor::on_sound_source_changed ()
{
	getSoundSourceNodeBox () .set_sensitive (getSoundSourceComboBoxText () .get_active_row_number () > 0);

	if (changing)
		return;

	// Copy X3DFontStyleNode.

	if (getSoundSourceComboBoxText () .get_active_row_number () > 0)
	{
		const auto last = soundSourceNode;

		switch (getSoundSourceComboBoxText () .get_active_row_number ())
		{
			case 1:
			{
				soundSourceNode = audioClip;
				break;
			}
			case 2:
			{
				soundSourceNode = movieTexture;
				break;
			}
			default:
				break;
		}

		soundSourceNode -> enabled ()     = last -> enabled ();
		soundSourceNode -> description () = last -> description ();
		soundSourceNode -> speed ()       = last -> speed ();
		soundSourceNode -> pitch ()       = last -> pitch ();
		soundSourceNode -> loop ()        = last -> loop ();

		try
		{
			soundSourceNode -> setField <X3D::MFString> ("url", last -> getField <X3D::MFString> ("url"));
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	// Set field.

	const auto & executionContext = X3D::MakePtr (getSelectionContext (sounds, true));

	addUndoFunction <X3D::SFNode> (sounds, "source", undoStep);

	for (const auto & sound : sounds)
	{
		try
		{
			auto & field = sound -> source ();

			field .removeInterest (&X3DSoundSourceNodeEditor::set_source, this);
			field .addInterest (&X3DSoundSourceNodeEditor::connectSource, this);

			if (getSoundSourceComboBoxText () .get_active_row_number () > 0)
				X3D::X3DEditor::replaceNode (executionContext, sound, field, soundSourceNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, sound, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (sounds, "source", undoStep);

	getSoundSourceUnlinkButton () .set_sensitive (getSoundSourceComboBoxText () .get_active () > 0 and soundSourceNode -> getCloneCount () > 1);

	set_widgets ();
}

void
X3DSoundSourceNodeEditor::set_source ()
{
	soundSourceNodeBuffer .addEvent ();
}

void
X3DSoundSourceNodeEditor::connectSource (const X3D::SFNode & field)
{
	field .removeInterest (&X3DSoundSourceNodeEditor::connectSource, this);
	field .addInterest (&X3DSoundSourceNodeEditor::set_source, this);
}

void
X3DSoundSourceNodeEditor::set_node ()
{
	if (soundSourceNode)
	{
		soundSourceNode -> isEvenLive (false);

		soundSourceNode -> isPaused ()         .removeInterest (&X3DSoundSourceNodeEditor::set_active,      this);
		soundSourceNode -> isActive ()         .removeInterest (&X3DSoundSourceNodeEditor::set_active,      this);
		soundSourceNode -> elapsedTime ()      .removeInterest (&X3DSoundSourceNodeEditor::set_elapsedTime, this);
		soundSourceNode -> duration_changed () .removeInterest (&X3DSoundSourceNodeEditor::set_duration,    this);
	}

	undoStep .reset ();

	const auto executionContext  = X3D::MakePtr (getSelectionContext (sounds, true));
	auto       tuple             = getSelection <X3D::X3DSoundSourceNode> (sounds, "source");
	const      int32_t active    = std::get <1> (tuple);
	const      bool    hasParent = std::get <2> (tuple);
	const      bool    hasField  = (active not_eq -2);

	soundSourceNode = std::move (std::get <0> (tuple));

	audioClip    = soundSourceNode;
	movieTexture = soundSourceNode;

	if (not audioClip)
		audioClip = executionContext -> createNode <X3D::AudioClip> ();

	if (not movieTexture)
		movieTexture = executionContext -> createNode <X3D::MovieTexture> ();

	if (not soundSourceNode)
		soundSourceNode = audioClip;

	soundSourceNode -> isEvenLive (true);
	soundSourceNode -> isPaused ()         .addInterest (&X3DSoundSourceNodeEditor::set_active, this);
	soundSourceNode -> isActive ()         .addInterest (&X3DSoundSourceNodeEditor::set_active, this);
	soundSourceNode -> elapsedTime ()      .addInterest (&X3DSoundSourceNodeEditor::set_elapsedTime, this);
	soundSourceNode -> duration_changed () .addInterest (&X3DSoundSourceNodeEditor::set_duration,    this);

	changing = true;

	if (active > 0)
	{
		switch (soundSourceNode -> getType () .back ())
		{
			case X3D::X3DConstants::AudioClip:
			{
				getSoundSourceComboBoxText () .set_active (1);
				break;
			}
			case X3D::X3DConstants::MovieTexture:
			{
				getSoundSourceComboBoxText () .set_active (2);
				break;
			}
			default:
				break;
		}
	}
	else if (active == 0)
		getSoundSourceComboBoxText () .set_active (0);
	else
		getSoundSourceComboBoxText () .set_active (-1);

	getSelectSoundSourceBox ()    .set_sensitive (hasParent);
	getSoundSourceComboBoxText () .set_sensitive (hasField);
	getSoundSourceUnlinkButton () .set_sensitive (active > 0 and soundSourceNode -> getCloneCount () > 1);

	set_widgets ();
	set_active ();
	set_duration ();

	changing = false;
}

void
X3DSoundSourceNodeEditor::set_widgets ()
{
	const X3D::MFNode nodes = { soundSourceNode };

	enabled     .setNodes (nodes);
	description .setNodes (nodes);
	url ->       setNodes (nodes);
	speed       .setNodes (nodes);
	pitch       .setNodes (nodes);
	loop        .setNodes (nodes);
}

void
X3DSoundSourceNodeEditor::on_sound_source_loop_toggled ()
{
	if (getSoundSourceLoopToggleButton () .get_active () and not soundSourceNode -> isActive ())
		soundSourceNode -> stopTime () = X3D::SFTime::now ();
}

void
X3DSoundSourceNodeEditor::on_sound_source_stop_clicked ()
{
	soundSourceNode -> stopTime () = X3D::SFTime::now ();

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DSoundSourceNodeEditor::on_sound_source_play_pause_clicked ()
{
	if (soundSourceNode -> isActive ())
	{
		if (soundSourceNode -> isPaused ())
			soundSourceNode -> resumeTime () = X3D::SFTime::now ();

		else
			soundSourceNode -> pauseTime () = X3D::SFTime::now ();
	}
	else
		soundSourceNode -> startTime () = X3D::SFTime::now ();

	getBrowserWindow () -> getCurrentPage () -> setModified (true);
}

void
X3DSoundSourceNodeEditor::set_active ()
{
	if (soundSourceNode -> isActive () and not soundSourceNode -> isPaused ())
		getSoundSourcePlayPauseImage () .set (Gtk::StockID ("gtk-media-pause"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));

	else
		getSoundSourcePlayPauseImage () .set (Gtk::StockID ("gtk-media-play"), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
}

void
X3DSoundSourceNodeEditor::set_elapsedTime ()
{
	const auto time = std::fmod (soundSourceNode -> elapsedTime (), soundSourceNode -> duration_changed ());

	getSoundSourceElapsedTimeLabel () .set_text (strftime (time, 3));
}

void
X3DSoundSourceNodeEditor::set_duration ()
{
	getSoundSourceDurationLabel () .set_text (strftime (soundSourceNode -> duration_changed () < 0 ? 0 : soundSourceNode -> duration_changed (), 3));
}

X3DSoundSourceNodeEditor::~X3DSoundSourceNodeEditor ()
{ }

} // puck
} // titania
