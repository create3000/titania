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
	                   loop (this, getSoundSourceLoopCheckButton (), "loop"),
	              startTime (this,
	                         getSoundSourceStartTimeAdjustment (),
	                         getSoundSourceStartTimeBox (),
	                         "startTime"),
	              resumeTime (this,
	                         getSoundSourceResumeTimeAdjustment (),
	                         getSoundSourceResumeTimeBox (),
	                         "resumeTime"),
	              pauseTime (this,
	                         getSoundSourcePauseTimeAdjustment (),
	                         getSoundSourcePauseTimeBox (),
	                         "pauseTime"),
	               stopTime (this,
	                         getSoundSourceStopTimeAdjustment (),
	                         getSoundSourceStopTimeBox (),
	                         "stopTime"),
	               isPaused (this, getSoundSourceIsPausedCheckButton (), "isPaused"),
	               isActive (this, getSoundSourceIsActiveCheckButton (), "isActive"),
	               cycleTime (this,
	                         getSoundSourceCycleTimeAdjustment (),
	                         getSoundSourceCycleTimeSpinButton (),
	                         "cycleTime"),
	            elapsedTime (this,
	                         getSoundSourceElapsedTimeAdjustment (),
	                         getSoundSourceElapsedTimeSpinButton (),
	                         "elapsedTime"),
	       duration_changed (this,
	                         getSoundSourceDurationAdjustment (),
	                         getSoundSourceDurationSpinButton (),
	                         "duration_changed"),
	                 sounds (),
	  soundSourceNodeBuffer (),
	        soundSourceNode (),
	              audioClip (),
	           movieTexture (),
	               undoStep (),
	      startTimeUndoStep (),
	     resumeTimeUndoStep (),
	      pauseTimeUndoStep (),
	       stopTimeUndoStep (),
	               changing (false)
{
	addChildren (sounds, soundSourceNodeBuffer, soundSourceNode, audioClip, movieTexture);
}

void
X3DSoundSourceNodeEditor::initialize ()
{
	soundSourceNodeBuffer .addInterest (this, &X3DSoundSourceNodeEditor::set_node);
}

void
X3DSoundSourceNodeEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & sound : sounds)
		sound -> source () .removeInterest (this, &X3DSoundSourceNodeEditor::set_source);

	sounds = getNodes <X3D::Sound> (selection, { X3D::X3DConstants::Sound });

	for (const auto & sound : sounds)
		sound -> source () .addInterest (this, &X3DSoundSourceNodeEditor::set_source);

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

		soundSourceNode -> enabled () = last -> enabled ();
		// ...
	}

	// Set field.

	addUndoFunction <X3D::SFNode> (sounds, "source", undoStep);

	for (const auto & sound : sounds)
	{
		try
		{
			auto & field = sound -> source ();

			field .removeInterest (this, &X3DSoundSourceNodeEditor::set_source);
			field .addInterest (this, &X3DSoundSourceNodeEditor::connectSource);

			if (getSoundSourceComboBoxText () .get_active_row_number () > 0)
				getBrowserWindow () -> replaceNode (getCurrentContext (), sound, field, soundSourceNode, undoStep);
			else
				getBrowserWindow () -> replaceNode (getCurrentContext (), sound, field, nullptr, undoStep);
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
	field .removeInterest (this, &X3DSoundSourceNodeEditor::connectSource);
	field .addInterest (this, &X3DSoundSourceNodeEditor::set_source);
}

void
X3DSoundSourceNodeEditor::set_node ()
{
	if (soundSourceNode)
	{
		soundSourceNode -> isEvenLive (false);

		soundSourceNode -> isPaused () .removeInterest (this, &X3DSoundSourceNodeEditor::set_playing);
		soundSourceNode -> isActive () .removeInterest (this, &X3DSoundSourceNodeEditor::set_playing);
	}

	undoStep           .reset ();
	startTimeUndoStep  .reset ();
	resumeTimeUndoStep .reset ();
	pauseTimeUndoStep  .reset ();
	stopTimeUndoStep   .reset ();

	auto  tuple             = getSelection <X3D::X3DSoundSourceNode> (sounds, "source");
	const int32_t active    = std::get <1> (tuple);
	const bool    hasParent = std::get <2> (tuple);
	const bool    hasField  = (active not_eq -2);

	soundSourceNode = std::move (std::get <0> (tuple));

	audioClip    = soundSourceNode;
	movieTexture = soundSourceNode;

	if (not audioClip)
		audioClip = getCurrentContext () -> createNode <X3D::AudioClip> ();

	if (not movieTexture)
		movieTexture = getCurrentContext () -> createNode <X3D::MovieTexture> ();

	if (not soundSourceNode)
		soundSourceNode = audioClip;

	soundSourceNode -> isEvenLive (true);
	soundSourceNode -> isPaused () .addInterest (this, &X3DSoundSourceNodeEditor::set_playing);
	soundSourceNode -> isActive () .addInterest (this, &X3DSoundSourceNodeEditor::set_playing);

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
	set_playing ();

	changing = false;
}

void
X3DSoundSourceNodeEditor::set_widgets ()
{
	const X3D::MFNode nodes = { soundSourceNode };

	enabled          .setNodes (nodes);
	description      .setNodes (nodes);
	url ->            setNodes (nodes);
	speed            .setNodes (nodes);
	pitch            .setNodes (nodes);
	loop             .setNodes (nodes);
	startTime        .setNodes (nodes);
	resumeTime       .setNodes (nodes);
	pauseTime        .setNodes (nodes);
	stopTime         .setNodes (nodes);
	isPaused         .setNodes (nodes);
	isActive         .setNodes (nodes);
	cycleTime        .setNodes (nodes);
	elapsedTime      .setNodes (nodes);
	duration_changed .setNodes (nodes);
}

void
X3DSoundSourceNodeEditor::set_playing ()
{
	if (soundSourceNode -> isActive ())
	{
		if (soundSourceNode -> isPaused ())
		{
			getSoundSourceStartTimeButton  () .set_sensitive (false);
			getSoundSourcePauseTimeButton  () .set_sensitive (false);
			getSoundSourceResumeTimeButton () .set_sensitive (true);
			getSoundSourceStopTimeButton   () .set_sensitive (true);
		}
		else
		{
			getSoundSourceStartTimeButton  () .set_sensitive (false);
			getSoundSourcePauseTimeButton  () .set_sensitive (true);
			getSoundSourceResumeTimeButton () .set_sensitive (false);
			getSoundSourceStopTimeButton   () .set_sensitive (true);
		}
	}
	else
	{
		getSoundSourceStartTimeButton  () .set_sensitive (true);
		getSoundSourcePauseTimeButton  () .set_sensitive (false);
		getSoundSourceResumeTimeButton () .set_sensitive (false);
		getSoundSourceStopTimeButton   () .set_sensitive (false);
	}
}

void
X3DSoundSourceNodeEditor::on_sound_source_start_time_clicked ()
{
	addUndoFunction (soundSourceNode, soundSourceNode -> startTime (), startTimeUndoStep);

	soundSourceNode -> startTime () = chrono::now ();

	addRedoFunction (soundSourceNode, soundSourceNode -> startTime (), startTimeUndoStep);
}

void
X3DSoundSourceNodeEditor::on_sound_source_resume_time_clicked ()
{
	addUndoFunction (soundSourceNode, soundSourceNode -> resumeTime (), resumeTimeUndoStep);

	soundSourceNode -> resumeTime () = chrono::now ();

	addRedoFunction (soundSourceNode, soundSourceNode -> resumeTime (), resumeTimeUndoStep);
}

void
X3DSoundSourceNodeEditor::on_sound_source_pause_time_clicked ()
{
	addUndoFunction (soundSourceNode, soundSourceNode -> pauseTime (), pauseTimeUndoStep);

	soundSourceNode -> pauseTime () = chrono::now ();

	addRedoFunction (soundSourceNode, soundSourceNode -> pauseTime (), pauseTimeUndoStep);
}

void
X3DSoundSourceNodeEditor::on_sound_source_stop_time_clicked ()
{
	addUndoFunction (soundSourceNode, soundSourceNode -> stopTime (), stopTimeUndoStep);

	soundSourceNode -> stopTime () = chrono::now ();

	addRedoFunction (soundSourceNode, soundSourceNode -> stopTime (), stopTimeUndoStep);
}

X3DSoundSourceNodeEditor::~X3DSoundSourceNodeEditor ()
{ }

} // puck
} // titania
